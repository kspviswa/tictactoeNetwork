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
#include<unistd.h>
#include"tictac.h"

using namespace tictac;
using namespace std;

#define NOT_INTIALISED  'Z'
#define FIRST_X         'X'
#define SECOND_O        'O'
#define UINT4      unsigned int
#define UINT1      unsigned char
#define UINT2      short unsigned int

#define LOCAL_STATE	1
#define REMOTE_STATE	2
typedef sem_t * tSemId;

#define MAX_SQUARE	9
#define MAX_NAME	10
#define RUNNING		5

#define FAILURE		1
#define SUCCESS		0

#define SET		1
#define RESET		0
#define	SNAP_SHOT_FREQ 20


typedef struct user_data {
				char game_state[MAX_SQUARE];
				UINT1 lFlag;
				UINT1 rFlag;
				char lplayerName[MAX_NAME];
				char rplayerName[MAX_NAME];
			  }tUserData;

/*UINT1 SetStableState ( UINT2 StateData, UINT1 stateFlag );
UINT1 GetStableState( UINT2 *StateData, UINT1 stateFlag );*/
UINT1 SendMove( char * stateString, char * playername );
UINT1 UpdateBoard( tictacpacket remoteChange );
char * RegisterPlayer(char *playerName );
UINT1 GetSnapShot();
/*UINT1 DataToString (char * StateString, tgameState LocalState, tgameState RemoteState );
UINT1 StringToData ( UINT2 *StateData, char *StateString);*/
UINT1 CalculateGameStatus( const char *square );
UINT1 InitializeState();
void *SendSnapShot(void* user_sign);
void form_state ( char *game_state, char *user_sign , tictacpacket *packet);
#define SET_ERROR(str) printf("%s", str);
