#include <iosteam>
#include "color.h"

using namespace std;

#define MAX_POSTION 9

class CPosition
{
public:

int nPos;
int bWin;
int nPlayerLastAccessed;
};

class CPlayer
{
public:

string strName;
int nPlayerId;
int bWin;
int nScore;
int nTotMatchesPlayed;
int nTotMatchesWon;
};

class CBoard
{
public:

CPosition nPositions[MAX_POSTION];

void display();
};

class Play
{
public:

CBoard theBoard;
vector<CPlayer> theplayers;

public:
void init();
int play();

};
