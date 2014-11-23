/*
 * components.cpp
 *
 *  Created on: 06-Nov-2014
 *      Author: kspviswa
 */

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "components.h"
#include "tictac.h"



/**
 * processIncomingMessage()
 * This function will be called, after populating the tictacpacket.
 * This function will process the packet, as per the Message Type.
 * Input : A populate tictacpacket
 * Output : nothing.
 */
void CController::processIncomingMessage(tictacpacket thePacket, struct sockaddr_in clientAddr)
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
		processRegisterMessage(&thePacket, &clientAddr);
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

int CController::processRegisterMessage(tictacpacket *pPacket, struct sock_addr *pClientAddr)
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

		pNewPlayer->connObject.prepareAddress(pClientAddr);

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
				// Create a report for the new match
				CMatchStatistics *pStat = new CMatchStatistics();
				pStat->eP1State = PLAYING;
				pStat->eP2State = PLAYING;
				pStat->nMatchId = pMatch->nMatchId;
				pStat->sIP1 = pMatch->m_hPlayer1->sIP;
				pStat->sIP2 = pMatch->m_hPlayer2->sIP;
				pStat->sPlayer1 = pMatch->m_hPlayer1->strName;
				pStat->sPlayer2 = pMatch->m_hPlayer2->strName;

				pthread_mutex_lock(&lockStatistics);

				_mapStatistics.insert(std::make_pair(pStat->nMatchId, pStat));

				pthread_mutex_unlock(&lockStatistics);

				// cook a START RESPONSE and send out
				dResponse.set_ipv4opp(pMatch->m_hPlayer2->nIPv4);
				break;
			default:
				break;
			}
		}

		// Cook a packet & send out
		sendPacketToClient(&dResponse, pNewPlayer);
	}
	return 0;
}

/**
 * processSnapshotMessage()
 * Proceed with updating the state information that was sent by a client.
 *
 * Algo
 * ****
 * 1) Get the IPv4
 * 2) Point to the matching player with IPv4
 * 3) If found, then update the state information of that particular client
 * 4) If not, silently discard
 */
int CController::processSnapshotMessage(tictacpacket *pPacket)
{
	if(pPacket & !pPacket->state().empty())
	{
		// Find a match first
		long nMatch = FindPlayerMatch(pPacket->ipv4());
		CPlayer *pNewPlayer;

		pNewPlayer = returnPlayer(nMatch);

		if(pNewPlayer)
		{
			pNewPlayer->strState = pPacket->state();
		}
	}
	return 0;
}

int CController::processTerminateMessage(tictacpacket *pPacket)
{
	if(pPacket)
	{
		// Find a match first
		long nMatch = FindPlayerMatch(pPacket->ipv4());
		CPlayer *pNewPlayer;

		pNewPlayer = returnPlayer(nMatch);

		if(pNewPlayer)
		{
			CMatch *pMatch = pNewPlayer->pMatch;
			if(pMatch)
			{
				pMatch->m_MatchState = MATCH_SUSPENDED;
			}

			CMatchStatistics *pStat;
			map<unsigned long, CMatchStatistics*>::iterator iter;

			pthread_mutex_lock(&lockStatistics);
			iter = _mapStatistics.find(pMatch->nMatchId);
			pthread_mutex_unlock(&lockStatistics);

			if(iter != _mapStatistics.end())
			{
				pStat = iter->second;
			}

			if(pNewPlayer->sName == pStat->sPlayer1)
			{
				pStat->eP1State = TERMINATED;
				pStat->eP2State = WON;
			}
			else
			{
				pStat->eP2State = TERMINATED;
				pStat->eP1State = WON;
			}
		}
	}
	return 0;
}

int CController::processEndMessage(tictacpacket *pPacket)
{
	if(pPacket)
	{
		// Find a match first
		long nMatch = FindPlayerMatch(pPacket->ipv4());
		CPlayer *pNewPlayer;
		bool bPlayer1 = false;

		pNewPlayer = returnPlayer(nMatch);

		if(pNewPlayer)
		{
			CMatch *pMatch = pNewPlayer->pMatch;
			if(pMatch)
			{
				pMatch->m_MatchState = MATCH_ENDED;
			}

			CMatchStatistics *pStat;
			map<unsigned long, CMatchStatistics*>::iterator iter;

			pthread_mutex_lock(&lockStatistics);
			iter = _mapStatistics.find(pMatch->nMatchId);
			pthread_mutex_unlock(&lockStatistics);

			if(iter != _mapStatistics.end())
			{
				pStat = iter->second;
			}

			if(pNewPlayer->sName == pStat->sPlayer1)
			{
				bPlayer1 = true;
			}
			switch(pPacket->endtype())
			{
			case tictacpacket::WON:
				if(bPlayer1)
				{
					pStat->eP1State = WON;
					pStat->eP2State = LOST;
				}
				else
				{
					pStat->eP2State = WON;
					pStat->eP1State = LOST;
				}
				break;
			case tictacpacket::LOST:
				if(bPlayer1)
				{
					pStat->eP1State = LOST;
					pStat->eP2State = WON;
				}
				else
				{
					pStat->eP2State = LOST;
					pStat->eP1State = WON;
				}
				break;
			case tictacpacket::DRAW:
				pStat->eP1State = TIE;
				pStat->eP2State = TIE;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

/**
 * parsePacket()
 * This function will be called after reading the incoming raw bytes on the socket
 * Input : pointer to source buffer, that was filled from socket
 * Output : parsing result.
 */
long CController::parsePacket(unsigned char *pSrc, unsigned long nLen, )
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

int CController::sendPacketToClient(tictacpacket *pSrcPacket, CPlayer *pPlayer)
{
	if(pPlayer && pSrcPacket)
	{
		if(pPlayer->connObject.nSockId > 0)
		{
			string data = pSrcPacket->SerializeAsString();
			pPlayer->connObject.sendMessage((void*)const_cast<char*>(data.c_str()), data.length());
		}
	}
	return 0;
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


void CConn::prepareAddress(struct sock_addr *pSrc)
{
	if(pSrc)
	{
		memcpy(&clientAddr, pSrc, sizeof(struct sock_addr));
		nPortId = htons(SMARTPEER_CLIENT_PORT);
		clientAddr.sin_port = nPortId;
		this->nSockId = socket(AF_INET, SOCK_DGRAM, 0);
	}
}

int CConn::sendMessage(void *pMsg, unsigned long nLen)
{
	int nRet;
	nRet = sendto(nSockId,pMsg,nLen,0,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
	return nRet;
}
