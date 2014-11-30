/*
 * components.h
 *
 *  Created on: 20-Oct-2014
 *      Author: kspviswa
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "tictac.h"

using namespace std;
using namespace tictac;

typedef vector<unsigned long> vectLong;

#define SMARTPEER_CLIENT_PORT 9000
#define SMARTPEER_SERVER_PORT 10000
#define SMARTPEER_HTTP_PORT 11000

#define SELECT_TIMEOUT_SEC 10
#define SELECT_TIMEOUT_USEC (SELECT_TIMEOUT_SEC * 1000)

string IpAddrToString(unsigned long nIpv4);
unsigned long StringToIpInt(string SIP);
string IntToString(unsigned long n);


#define INIT_MATCH 1
#define ATTACH_MATCH 2

class CMatch;

class CConn
{
public:
	void prepareAddress(struct sockaddr_in *pSrc);
	int sendMessage(void *pMsg, unsigned long nLen);

public:
	unsigned long nSockId;
	struct sockaddr_in clientAddr;
	unsigned long nPortId;
};

class CWrk
{
public:
	void *pObject;
	void *pData;
	int nLen;
};

class CPlayer
{
public:
	string sName;
	string sIP;
	unsigned long nIPv4;
	CMatch *pMatch;
	CConn connObject;
	string strState;
	unsigned long nSnapshotTime;
};

typedef vector<CPlayer*> vPlayerHandles;

enum EMatchState
{
	MATCH_READY=0,
	MATCH_STARTED,
	MATCH_ENDED,
	MATCH_TIE,
	MATCH_SUSPENDED,
	MATCH_CANCELLED,
	MATCH_OTHER
};

enum EPlayerState
{
	PLAYING=0,
	WON=1,
	LOST=2,
	TIE=3,
	TERMINATED=4
};

typedef enum EMatchState eMatchState;
typedef enum EPlayerState ePlayerState;

class CMatch
{
public:
	CPlayer* m_hPlayer1;
	CPlayer* m_hPlayer2;
	eMatchState m_MatchState;
	unsigned long nMatchId;
};

class CMatchStatistics
{
public:
	void saveStatistics(CPlayer *p1, CPlayer *p2);
	string returnStrStateMatch(eMatchState nstate);
	string returnStrStatePlayer(ePlayerState nState);
public:
	unsigned long nMatchId;
	string sPlayer1;
	string sIP1;
	string sPlayer2;
	string sIP2;
	eMatchState eState;
	ePlayerState eP1State;
	ePlayerState eP2State;
	//unsigned long nLastSnapshotTime;
};

class CController
{
public :
	int init();

	int initOrAttachNewMatch(CPlayer *pPlayer);
	long FindPlayerMatch(unsigned long nIPv4);
	CPlayer* returnPlayer(unsigned long nIndex);
	//unsigned long FindMatchByPlayer(unsigned long nIPv4);
	CMatchStatistics* returnStatistics(unsigned long nMatchId);
	string returnHtmlMatchStatistics();
	void attachPlayer(CPlayer *pPlayer);

	// public APIs
	void doSnapshotService();
	void doSocketListen();
	void doReporting();

	void processIncomingMessage(tictacpacket thePacket);
	long parsePacket(unsigned char *pSrc, unsigned long nLen);
	//int sendResponse(tictacpacket *pSrcPacket);
	int processRegisterMessage(tictacpacket *pPacket);
	int processSnapshotMessage(tictacpacket *pPacket);
	int processTerminateMessage(tictacpacket *pPacket);
	int processEndMessage(tictacpacket *pPacket);

	int sendPacketToClient(tictacpacket *pPacket, CPlayer *pPlayer);

public:
	// index is incrementing number
	map<unsigned long, CMatch*> _mapMatches;
	//pointer to new match
	CMatch *pNewMatch;

	pthread_mutex_t lockMatches;

	// index is match id of _mapMatches
	map<unsigned long, CMatchStatistics*> _mapStatistics;
	pthread_mutex_t lockStatistics;

	// index is IPv4 of connection
	map<unsigned long, CPlayer*> _mapPlayers;
	pthread_mutex_t lockPlayers;

	// Server IP details
	string sServerIp;
	unsigned long nSIP;
};

#endif /* COMPONENTS_H_ */
