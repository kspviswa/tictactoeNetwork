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
#include "time.h"
#include "components.h"
#include "tictac.h"
#include <sstream>

string IntToString(unsigned long n)
{
	stringstream ss;
	ss << n;
	return ss.str();
}

void CController::doReporting()
{

	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	char sendBuff[1025];
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SMARTPEER_HTTP_PORT);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	cout << "Starting HTTP reporting service" << endl;
	while(1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
		string sHtml = returnHtmlMatchStatistics();

		send(connfd, "HTTP/1.0 200 OK\n\n", 17, 0);
		write(connfd, sHtml.c_str(), sHtml.length());

		close(connfd);
		sleep(1);
	}
}

string CController::returnHtmlMatchStatistics()
{
	time_t tnow = time(0);
	string cstrTime = ctime(&tnow);

	string sHtml = "<html><head><title>TicTacToe N/w Statistics</title></head>";
	sHtml += string("<body> <H1> TIC-TAC-TOE MATCH CONTROLLER WEB INTERFACE </H1> <BR> <P>");
	sHtml += string("<h3> Live Match Report @ ") + string(cstrTime) + string(" </h3> <br><br>");

	//Get the list of statistics
	map<unsigned long, CMatchStatistics*> stat;
	map<unsigned long, CMatchStatistics*>::iterator iter;

	// index is IPv4 of connection
        map<unsigned long, CPlayer*> playstat;
        map<unsigned long, CPlayer*>::iterator iter2;

	pthread_mutex_lock(&lockPlayers);
	playstat = _mapPlayers;
	pthread_mutex_unlock(&lockPlayers);


	pthread_mutex_lock(&lockStatistics);
	stat = _mapStatistics;
	pthread_mutex_unlock(&lockStatistics);

	if(playstat.size() > 0)
	{
		// Get the player statistics first
		string sPlayerstat = "<h3><u>Registered players</u></h3><br><p>";
		sPlayerstat += string("<table border='5'><tr><th>Name</th><th>IP Address</th></tr>");


		for(iter2 = playstat.begin(); iter2 != playstat.end(); ++iter2)
		{
			sPlayerstat += string("<tr><td>");
			sPlayerstat += iter2->second->sName;
			sPlayerstat += string("</td><td>");
			sPlayerstat += iter2->second->sIP;
			sPlayerstat += string("</td></tr>");
			//sPlayerstat += string("<tr><td>");
			//sPlayerstat += iter->second->sPlayer2;
			//sPlayerstat += string("</td><td>");
			//sPlayerstat += iter->second->sIP2;
			//sPlayerstat += string("</td></tr>");
		}

		sPlayerstat += string("</table>");

		// Get the match statistics
		string sMatchstat = "<h3><u>Live match updates</u></h3><br><p>";
		sMatchstat += string("<table border='5'><tr><th>Match ID</th><th>Player 1</th><th>Player 2</th><th>Match state</th><th>P1-State</th><th>P2-State</th></tr>");
		string sMatchstat2;
		for(iter = stat.begin(); iter != stat.end(); ++iter)
		{
			sMatchstat2 += string("<tr><td>");

			sMatchstat2 += IntToString(iter->second->nMatchId);
			sMatchstat2 += string("</td><td>");
			sMatchstat2 += iter->second->sPlayer1;
			sMatchstat2 += string("</td><td>");
			sMatchstat2 += iter->second->sPlayer2;
			sMatchstat2 += string("</td><td>");
			sMatchstat2 += iter->second->returnStrStateMatch(iter->second->eState);
			sMatchstat2 += string("</td><td>");
			sMatchstat2 += iter->second->returnStrStatePlayer(iter->second->eP1State);
			sMatchstat2 += string("</td><td>");
			sMatchstat2 += iter->second->returnStrStatePlayer(iter->second->eP2State);

			sMatchstat2 += string("</td></tr>");
		}

		sMatchstat += sMatchstat2;
		sMatchstat += string("</table>");

		// Combine player & match statistics
		sHtml += sPlayerstat + sMatchstat;

	}
	else
	{
		sHtml += string("<marquee><h3> No match has started. Watch out this space soon.....</h3></marquee>");
	}

//	//lockStatistics
//
//	sHtml += string("<table> <tr><th>Match ID</th><th> Player 1 </th> <th> Player 1 [IP] </th> <th> Player 2 </th> <th> Player 2 [IP] </th> <</tr>");

	sHtml += string("</body></html>");
	return sHtml;
}

void* processMessageHelper(void *pArg)
{
	if(pArg)
	{
		//cout << "Inside processMessageHelper" << endl;
		CWrk *wrk = (CWrk*)pArg;
		CController *pCtrl = (CController*)wrk->pObject;
		int nLen = wrk->nLen;
		pCtrl->parsePacket((unsigned char*)wrk->pData, nLen);
		if(wrk)
		{
			free(wrk->pData);
			free(wrk);
		}
	}
	pthread_exit(NULL);
}

void CController::doSocketListen()
{
	bool bInitStatus = false, bTerminate = false;
	int result;
	int status;
	fd_set         readfds;
	struct timeval selTimeout;
	struct sockaddr_in serverSock, clientSock;
	socklen_t sLen = sizeof(clientSock);
	unsigned char szData[2048];
	long nDataLen;

	int nServerSocket;

	cout << "Starting UDP Server service" << endl;

	if ((nServerSocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		cout << "Unable to start server. Socket call failed. :-(" << endl;
		return;
	}

	memset(&serverSock, 0, sLen);
	memset(&clientSock, 0, sLen);

	/* loop until we're asked to terminate */
	while(!bTerminate)
	{

		FD_ZERO(&readfds);
		/* set our usual timeout */
		selTimeout.tv_sec  = SELECT_TIMEOUT_SEC;
		selTimeout.tv_usec = SELECT_TIMEOUT_USEC;

		FD_SET(nServerSocket, &readfds);

		if( !bInitStatus )
		{

			serverSock.sin_family = AF_INET;
			serverSock.sin_port = htons(SMARTPEER_SERVER_PORT);
			serverSock.sin_addr.s_addr = htonl(INADDR_ANY);
			if (bind(nServerSocket, (sockaddr*)&serverSock, sizeof(serverSock))==-1)
			{
				cout << "Server cannot bind to specified port" << endl;
				return;
			}

			bInitStatus = true;
			cout << "Started server @ " << SMARTPEER_SERVER_PORT << endl;
		}
		/*
		 * Wait for something to happen on at least one of the sockets
		 */
		status = select(FD_SETSIZE, &readfds, NULL, NULL, 0/*&selTimeout*/) ;

		/*
		 * Catch timeout or error
		 */
		if ( 0 == status )
		{

			continue;

		}

		/* interrupted system call */
		else if (0 > status && EINTR == errno)
		{

			continue;
		}
		/* temporary resource shortage */
		else if (0 > status && EAGAIN == errno)
		{

			continue;
		}
		else if (0 > status)
		{
			cout << "Select returned abort. No luck " << endl;
			return;
		}

		/*
		 * See if an event occurred on this one
		 */
		//cout << "Incoming Message" << endl;

		int nRet;
		unsigned char *pData = (unsigned char*) malloc(2048);
		CWrk *pWork = new CWrk();

		if(!pData || !pWork)
		{
			cout << "Memory allocation failure for work items" << endl;
			return;
		}

		pWork->nLen = recvfrom(nServerSocket, pData, 2048, 0, (sockaddr*)&clientSock, &sLen);

		if(pWork->nLen < 0)
		{
			cout << "recvFrom failed" << endl;
			free(pData);
			free(pWork);
		}

		pWork->pObject = this;
		pWork->pData = pData;

		pthread_t newThread;
		pthread_create(&newThread, NULL, processMessageHelper, pWork);
	}

}

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

	//cout << "Inside parsePacket" << endl;
	if(!thePacket.IsInitialized())
	{
		cout << "Malformed packet" << endl;
	}

	// Switch processing w.r.t incoming type
	switch(thePacket.msgtype())
	{
	case tictacpacket::REGISTER :
		// A new client arrived. Please do the needful
		cout << "Register packet has arrived" << endl;
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
		nRetId = iter->first;
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
		pRet = iter->second;
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

unsigned long StringToIpInt(string SIP)
{
	unsigned long nIP;
	inet_pton(AF_INET, SIP.c_str(), &nIP);
	return nIP;
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
			// Check whether re-registration happens
			if(pNewMatch->m_hPlayer1 == pPlayer)
			{
				cout << "Re-Registration detected. Silently discarded" << endl;
				pthread_mutex_unlock(&lockMatches);
				return nRet;
			}
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
	cout << "Processing register packet" << endl;
	// Find a match first
	long nMatch = FindPlayerMatch(pPacket->ipv4());
	CPlayer *pNewPlayer;

	if(nMatch == -1)
	{
		cout << "New player arrived" << endl;
		// Looks like a new player. Register them
		pNewPlayer = new CPlayer();

		pNewPlayer->sName = pPacket->playername();
		pNewPlayer->nIPv4 = pPacket->ipv4();
		pNewPlayer->sIP = IpAddrToString(pNewPlayer->nIPv4);
		pNewPlayer->pMatch = NULL;
		pNewPlayer->strState = "Initial-State";
		pNewPlayer->nSnapshotTime = time(0);

		struct sockaddr_in clientAddr;
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_port = htons(SMARTPEER_CLIENT_PORT);
		clientAddr.sin_addr.s_addr = pNewPlayer->nIPv4;

		pNewPlayer->connObject.prepareAddress(&clientAddr);

		// Add to our inventory
		attachPlayer(pNewPlayer);
	}
	else
	{
		cout << "Returning player " << nMatch << endl;
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
            if(pMatch->m_hPlayer2 == pNewPlayer)
            {
                dResponse.set_ipv4opp(pMatch->m_hPlayer1->nIPv4);
                dResponse.set_playername(pMatch->m_hPlayer1->sName);

                cout << "Sending " << pMatch->m_hPlayer1->sName << endl;
            }
            else
            {
                dResponse.set_ipv4opp(pMatch->m_hPlayer2->nIPv4);
                dResponse.set_playername(pMatch->m_hPlayer2->sName);

                cout << "Sending " << pMatch->m_hPlayer2->sName << endl;
            }
			dResponse.set_msgtype(tictacpacket::RESUME);
			dResponse.set_state(pNewPlayer->strState);

            cout << "Sending state " << pNewPlayer->strState << endl; 
		}
		else
		{
			// Start a new match / attach to the new match
			switch(initOrAttachNewMatch(pNewPlayer))
			{

			case INIT_MATCH:
			{
				// cook a OK RESPONSE and send out
				dResponse.set_msgtype(tictacpacket::OK);
				break;
			}
			case ATTACH_MATCH:
			{
				// Create a report for the new match
				CMatchStatistics *pStat = new CMatchStatistics();
				pStat->eP1State = PLAYING;
				pStat->eP2State = PLAYING;
				pStat->nMatchId = pNewMatch->nMatchId;
				pStat->sIP1 = pNewMatch->m_hPlayer1->sIP;
				pStat->sIP2 = pNewMatch->m_hPlayer2->sIP;
				pStat->sPlayer1 = pNewMatch->m_hPlayer1->sName;
				pStat->sPlayer2 = pNewMatch->m_hPlayer2->sName;

				pthread_mutex_lock(&lockStatistics);

				_mapStatistics.insert(std::make_pair(pStat->nMatchId, pStat));

				pthread_mutex_unlock(&lockStatistics);

				// cook a START RESPONSE and send out
				dResponse.set_msgtype(tictacpacket::START);
				dResponse.set_ipv4opp(pNewMatch->m_hPlayer1->nIPv4);
				dResponse.set_playername(pNewMatch->m_hPlayer1->sName);
                cout << "Sending " << pNewMatch->m_hPlayer1->sName << " details to player " << pNewMatch->m_hPlayer2->sName << endl;
                cout << "IP address of " << pNewMatch->m_hPlayer1->sName << " is " << IpAddrToString(pNewMatch->m_hPlayer1->nIPv4) << endl;
                cout << "IP address of " << pNewMatch->m_hPlayer2->sName << " is " << IpAddrToString(pNewMatch->m_hPlayer2->nIPv4) << endl;
				pNewMatch = NULL;				
				break;
			}
			default:
			cout << "Default case" << endl;
			// cook a OK RESPONSE and send out
			dResponse.set_msgtype(tictacpacket::OK);
			break;
			}
		}

		// Cook a packet & send out
		sendPacketToClient(&dResponse, pNewPlayer);
	}
	return 0;
}

string CMatchStatistics::returnStrStateMatch(eMatchState nstate)
{
	string state = "Improper-State";
	switch(nstate)
	{
	case MATCH_READY :
		state = "Ready";
		break;
	case MATCH_STARTED:
		state = "Started";
		break;
	case MATCH_ENDED :
		state = "Ended";
		break;
	case MATCH_TIE:
		state = "Tied";
		break;
	case MATCH_SUSPENDED:
		state = "Suspended";
		break;
	case MATCH_CANCELLED :
		state = "Cancelled";
	default:
		break;
	}
	return state;
}
string CMatchStatistics::returnStrStatePlayer(ePlayerState nState)
{
	string state = "Improper-State";
	switch(nState)
	{
	case PLAYING :
		state = "Playing";
		break;
	case WON :
		state = "Won";
		break;
	case LOST :
		state = "Lost";
		break;
	case TIE :
		state = "Tied";
		break;
	case TERMINATED :
		state = "Terminated";
		break;
	default:
		break;
	}
	return state;
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
	if(pPacket && !pPacket->state().empty())
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

			detachPlayers(pMatch);
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

			detachPlayers(pMatch);
		}
	}
	return 0;
}

void CController::detachPlayers(CMatch *pMatch)
{
	if(pMatch && pMatch->m_hPlayer1 && pMatch->m_hPlayer2)
	{
		pthread_mutex_lock(&lockPlayers);
		_mapPlayers.erase(pMatch->m_hPlayer1->nIPv4);
		_mapPlayers.erase(pMatch->m_hPlayer2->nIPv4);
		pthread_mutex_unlock(&lockPlayers);
	}

	//ToDo : I still have the CPlayer object undestructed. May eat memory.
}

/**
 * parsePacket()
 * This function will be called after reading the incoming raw bytes on the socket
 * Input : pointer to source buffer, that was filled from socket
 * Output : parsing result.
 */
long CController::parsePacket(unsigned char *pSrc, unsigned long nLen)
{
	//cout << "Inside parse packet" << endl;
	tictacpacket thePacket;
	if(thePacket.ParseFromArray(pSrc, nLen))
	{
		// If parse is successful, then proceed to process the packet
		processIncomingMessage(thePacket);
		return 0; // success
	}
	else
{
	cout << "Parse packet failed!!!!!!!" << endl;
		return -1; // Failure
}
}

int CController::sendPacketToClient(tictacpacket *pSrcPacket, CPlayer *pPlayer)
{
	if(pPlayer && pSrcPacket)
	{
		if(pPlayer->connObject.nSockId > 0)
		{
			string data = pSrcPacket->SerializeAsString();
			cout << "Sending data to client" << endl;
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


void CConn::prepareAddress(struct sockaddr_in *pSrc)
{
	if(pSrc)
	{
		memcpy(&clientAddr, pSrc, sizeof(struct sockaddr_in));
		this->nSockId = socket(AF_INET, SOCK_DGRAM, 0);
	}
}

int CConn::sendMessage(void *pMsg, unsigned long nLen)
{
	int nRet;
	nRet = sendto(nSockId,pMsg,nLen,0,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
	cout << "Sent " << nRet << "bytes to" << IpAddrToString(clientAddr.sin_addr.s_addr) << endl;
	return nRet;
}
