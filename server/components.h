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

using namespace std;

typedef vector<unsigned long> vectLong;

class CMatch;
class CConn;

class CPlayer
{
public:
	string getName();
	void setName(string sNameStr);
	string getIPString();
	void setIPString(unsigned long nIPv4);
	void registerUser(CMatch *pMatchHandle);
	void registerConnection(CConn *pConn);

	CMatch* getMatchHandle();
	CConn* getConnHandle();

	vectLong::iterator getPreviousState();
	void fillPreviousState(vectLong *pDstVect);
	void saveCurrentState(vectLong *pCurrState, unsigned long nSnapshotTime);

public:
	string sName;
	string sIP;
	unsigned long nIPv4;
	CMatch *pMatch;
	CConn connObject;
	vectLong nState;
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
	void initMatch(vPlayerHandles *pPlayers);
	void flagMatchState(eMatchState matchState);

public:
vPlayerHandles m_Players;
eMatchState m_MatchState;
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
public:
	map<unsigned long, CMatch*> _mapMatches;
	map<unsigned long, CMatchStatistics> _mapStatistics;

eMatchState eState;
CPlayer* pWinningPlayer;
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
