#include<stdio.h>
#include<string.h>
#include<sys/statfs.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/times.h>
#include<sys/resource.h>
#include<sys/wait.h>
#include<semaphore.h>
#include"tictac.h"

#define NOT_INTIALISED  'Z'
#define FILL_X          'X'
#define FILL_O          'O'
#define UINT4      unsigned int
#define UINT1      unsigned char
#define UINT2      short unsigned int

// Game Status
#define RUNNING 1
#define WON     2
#define DRAW    3
#define LOST    4


#define LOCAL_STATE		1
#define REMOTE_STATE	2
typedef sem_t * tSemId;

#define MAX_SQUARE  9
#define FAILURE		1
#define SUCCESS		0
#define	DEF_PLAYER_NAME	"player 1"


typedef struct gameState {
					UINT2 StateData;
					UINT1 gameFlag;
					char playerName[20];
				 }tgameState;

UINT1 SetStableState ( UINT2 StateData, UINT1 stateFlag );
UINT1 GetStableState( UINT2 *StateData, UINT1 stateFlag );
UINT1 SendMove( char * stateString );
UINT1 UpdateBoard( UINT2 StateData );
UINT1 RegisterPlayer();
UINT1 GetSnapShot();
UINT1 SemTake (tSemId SemId);
UINT1 SemGive (tSemId SemId);
UINT1 DataToString (char * StateString, tgameState LocalState, tgameState RemoteState );
UINT1 StringToData ( UINT2 *StateData, char *StateString);
UINT1 CalculateGameStatus( char *square );
UINT1 InitializeState();
#define SET_ERROR(str) printf("%s", str);

