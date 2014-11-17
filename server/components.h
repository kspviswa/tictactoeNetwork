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

#include "tictac.h"

using namespace std;
using namespace tictac;

typedef vector<unsigned long> vectLong;

#define SMARTPEER_CLIENT_PORT 9000
#define SMARTPEER_SERVER_PORT 10000

#define INIT_MATCH 1
#define ATTACH_MATCH 2

class CMatch;
class CConn;

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

typedef enum EMatchState eMatchState;

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
public:
	unsigned long nMatchId;
	string sPlayer1;
	string sIP1;
	string sPlayer2;
	string sIP2;
	eMatchState eState;
	unsigned long nLastSnapshotTime;
};

class CController
{
public :
	int init();

	int initOrAttachNewMatch(CPlayer *pPlayer);
	long FindPlayerMatch(unsigned long nIPv4);
	CPlayer* returnPlayer(unsigned long nIndex);
	unsigned long FindMatchByPlayer(unsigned long nIPv4);
	CMatchStatistics* returnStatistics(unsigned long nMatchId);
	void returnHtmlMatchStatistics();
	void attachPlayer(CPlayer *pPlayer);

	// public APIs
	unsigned long doSnapshotService();
	unsigned long doSocketListen();
	unsigned long doReporting();

	void processIncomingMessage(tictacpacket thePacket);
	long parsePacket(unsigned char *pSrc, unsigned long nLen);
	int sendResponse(tictacpacket *pSrcPacket);
	int processRegisterMessage(tictacpacket *pPacket);
	int processSnapshotMessage(tictacpacket *pPacket);
	int processTerminateMessage(tictacpacket *pPacket);
	int processEndMessage(tictacpacket *pPacket);

	int sendPacketToClient(tictacpacket *pPacket);

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

};

class CConn
{
public:
	void prepareAddress(struct sockaddr_in *pSrc);
	void sendMessage(void *pMsg, unsigned long nLen);

public:
	unsigned long nSockId;
	struct sockaddr_in clientAddr;
	unsigned long nPortId;
};
#endif /* COMPONENTS_H_ */
