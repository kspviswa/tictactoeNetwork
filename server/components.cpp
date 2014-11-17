/*
 * components.cpp
 *
 *  Created on: 06-Nov-2014
 *      Author: kspviswa
 */

#include <netinet/in.h>
#include <arpa/inet.h>
#include "components.h"
#include "tictac.h"

/**
 * processIncomingMessage()
 * This function will be called, after populating the tictacpacket.
 * This function will process the packet, as per the Message Type.
 * Input : A populate tictacpacket
 * Output : nothing.
 */
void CController::processIncomingMessage(tictacpacket thePacket)
{
	/**
	 * When we have thePacket, it is quit safe to predict that,
	 * the thePacket is complete. else parse would have been failed.
	 */

	if(!thePacket.IsInitialized())
	{
		cout << "Malformed packet" << endl;
	}

	// Switch processing w.r.t incoming type
	switch(thePacket.msgtype())
	{
	case tictacpacket::REGISTER :
		// A new client arrived. Please do the needful
		processRegisterMessage(&thePacket);
		break;
	case tictacpacket::SNAPSHOTPUT:
		// A client has sent their snapshot. Proceed.
		processSnapshotMessage(&thePacket);
		break;
	case tictacpacket::TERMINATE:
		// A client has decided to quit. Do the needful.
		processTerminateMessage(&thePacket);
		break;
	case tictacpacket::END:
		// A match has ended. Record it.
		processEndMessage(&thePacket);
		break;
	default:
		cout << "Not processed" << endl;
	}
}

 long CController::FindPlayerMatch(unsigned long nIPv4)
{
	 long nRetId = -1;
	 std::map<unsigned long, CPlayer*>::iterator iter;

	 // lock & do the work
	 pthread_mutex_lock(&lockPlayers);
	 iter = _mapPlayers.find(nIPv4);

	 if(iter != _mapPlayers.end())
	 {
		 nRetId = iter->first();
	 }
	 pthread_mutex_unlock(&lockPlayers);

	 return nRetId;
}

CPlayer* CController::returnPlayer(unsigned long nIndex)
{
	CPlayer *pRet = NULL;
	std::map<unsigned long, CPlayer*>::iterator iter;

	// lock & do the work
	pthread_mutex_lock(&lockPlayers);
	iter = _mapPlayers.find(nIndex);

	if(iter != _mapPlayers.end())
	{
		pRet = iter->second();
	}
	else
	{
		pRet = NULL;
	}
	pthread_mutex_unlock(&lockPlayers);

	return pRet;
}

string IpAddrToString(unsigned long nIpv4)
{
	char szIP[INET_ADDRSTRLEN];
	string sIP;

	inet_ntop(AF_INET, &nIpv4, szIP, INET_ADDRSTRLEN);

	sIP.assign(szIP, strlen(szIP));

	return sIP;
}

void CController::attachPlayer(CPlayer *pPlayer)
{
	if(pPlayer)
	{
		// lock & do the work
		pthread_mutex_lock(&lockPlayers);
		_mapPlayers.insert(std::make_pair(pPlayer->nIPv4, pPlayer));
		pthread_mutex_unlock(&lockPlayers);
	}
}

int CController::initOrAttachNewMatch(CPlayer *pPlayer)
{
	int nRet = -1;
	if(pPlayer)
	{
		pthread_mutex_lock(&lockMatches);
		if(pNewMatch)
		{
			// Some body already waiting
			nRet = ATTACH_MATCH;
			pNewMatch->m_hPlayer2 = pPlayer;
			pNewMatch->nMatchId = _mapMatches.size() + 1;
			_mapMatches.insert(std::make_pair(pNewMatch->nMatchId, pNewMatch));
			pNewMatch->m_hPlayer1->pMatch = pNewMatch;
			pNewMatch->m_hPlayer2->pMatch = pNewMatch;
		}
		else
		{
			// Brand new match
			pNewMatch = new CMatch();
			pNewMatch->m_hPlayer1 = pPlayer;
			nRet = INIT_MATCH;

		}
		pthread_mutex_unlock(&lockMatches);
	}

	return nRet;
}
/**
 * processRegisterMessage()
 * This function will open the register packet and tries to
 * register a player in its database.
 * The response can be either OK or RESUME.
 * The incoming pointer tictacpacket will be filled and will
 * be returned to client via arbitary UDP Socket.
 */

int CController::processRegisterMessage(tictacpacket *pPacket)
{
	/*
	 * The algo is to search the _mapPlayers with incoming ipv4
	 * If you find a match, then proceed with either RESUME / OK / START
	 */

	// Find a match first
	long nMatch = FindPlayerMatch(pPacket->ipv4());
	CPlayer *pNewPlayer;

	if(nMatch != -1)
	{
		// Looks like a new player. Register them
		pNewPlayer = new CPlayer();

		pNewPlayer->sName = pPacket->playername();
		pNewPlayer->nIPv4 = pPacket->ipv4();
		pNewPlayer->sIP = IpAddrToString(pNewPlayer->nIPv4);
		pNewPlayer->pMatch = NULL;
		pNewPlayer->strState = "Initial-State";
		pNewPlayer->nSnapshotTime = time(0);

		// Add to our inventory
		attachPlayer(pNewPlayer);
	}
	else
	{
		pNewPlayer = returnPlayer(nMatch);
	}

	if(pNewPlayer)
	{
		// look for resume
		CMatch *pMatch = NULL;

		pthread_mutex_lock(&lockMatches);
		pMatch = pNewPlayer->pMatch;
		pthread_mutex_unlock(&lockMatches);

		tictacpacket dResponse;
		dResponse.set_ipv4(pNewPlayer->nIPv4);

		if(pMatch)
		{
			dResponse.set_ipv4opp(pMatch->m_hPlayer2->nIPv4);
			dResponse.set_msgtype(tictacpacket::RESUME);
			dResponse.set_state(pNewPlayer->strState);
		}
		else
		{
			// Start a new match / attach to the new match
			switch(initOrAttachNewMatch(pNewPlayer))
			{

			case INIT_MATCH:
				// cook a OK RESPONSE and send out
				dResponse.set_msgtype(tictacpacket::OK);
				break;
			case ATTACH_MATCH:
				// cook a START RESPONSE and send out
				dResponse.set_ipv4opp(pMatch->m_hPlayer2->nIPv4);
				break;
			default:
				break;
			}
		}

		// Cook a packet & send out
		sendPacketToClient(&dResponse);
	}
	return 0;
}
/**
 * parsePacket()
 * This function will be called after reading the incoming raw bytes on the socket
 * Input : pointer to source buffer, that was filled from socket
 * Output : parsing result.
 */
long CController::parsePacket(unsigned char *pSrc, unsigned long nLen)
{
	tictacpacket thePacket;
	if(thePacket.ParseFromArray(pSrc, nLen))
	{
		// If parse is successful, then proceed to process the packet
		processIncomingMessage(thePacket);
		return 0; // success
	}
	else
		return -1; // Failure
}


int CController::init()
{
	lockMatches = lockStatistics = lockPlayers = PTHREAD_MUTEX_INITIALIZER;
	if(pthread_mutex_init(&lockMatches, NULL) < 0 ||
			pthread_mutex_init(&lockStatistics, NULL) ||
			pthread_mutex_init(&lockPlayers, NULL))
		return -1;

	return 0;
}


