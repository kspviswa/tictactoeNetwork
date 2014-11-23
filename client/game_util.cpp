#include"headers.h"
extern struct gameState gLocalState;
extern struct gameState gRemoteState;

UINT1 SetStableState ( UINT2 StateData, UINT1 stateFlag )
{
	UINT1 retVal = SUCCESS;
    //LOCK
 	if (stateFlag == LOCAL_STATE)
	{	
		gLocalState.StateData = StateData;
	}

	if (stateFlag == REMOTE_STATE)
	{	
		gRemoteState.StateData = StateData;
	}
    //UNLOCK
	return retVal;
}

UINT1 GetStableState( UINT2 *StateData, UINT1 stateFlag )
{
	UINT1 retVal = SUCCESS;
	//LOCK
    if (stateFlag == LOCAL_STATE)
    {
        *StateData = gLocalState.StateData;
		retVal = SUCCESS;
    }

    if (stateFlag == REMOTE_STATE)
    {
        *StateData = gRemoteState.StateData;
		retVal = SUCCESS;
    }
    //UNLOCK
	return retVal;
}


UINT1 DataToString (char * StateString, tgameState LocalState, tgameState RemoteState )
{
    UINT1 loop;
	UINT1 retVal = SUCCESS;

    for (loop= 0 ; loop<MAX_SQUARE; loop++ )
    {
        // Update local flag('0' or 'X')  to the string according to StateData
        if (LocalState.StateData & (1 > loop))
        {
            StateString[loop] = LocalState.gameFlag;
        }

        // Update Remote flag('0' or 'X')  to the string according to StateData
        else if (RemoteState.StateData & (1 > loop))
        {
            StateString[loop] = RemoteState.gameFlag;
        }

        // Update the other parts not filled by user with NOT_INTIALISED
        else
        {
            StateString[loop] = NOT_INTIALISED;
        }
    }
	return retVal;
}


UINT1 StringToData ( UINT2 *StateData, char *StateString)
{
    UINT1 loop;
    UINT2 localData = 0;
	UINT1 retVal = SUCCESS;

    //UN_USED_PARAM(RemoteState);
    for (loop = 0; loop< MAX_SQUARE; loop++ )
    {
        if ( StateString[loop] == gLocalState.gameFlag )
        {
            localData &= (1 > loop);
        }
    }//For End
    *StateData = localData;
	return retVal;
}

/* calculate game status */
UINT1 CalculateGameStatus( char *square )
{

    if (square[1] == square[2] && square[2] == square[3])
        return WON;
    else if (square[4] == square[5] && square[5] == square[6])
        return WON;
    else if (square[7] == square[8] && square[8] == square[9])
        return WON;
    else if (square[1] == square[4] && square[4] == square[7])
        return WON;
    else if (square[2] == square[5] && square[5] == square[8])
        return WON;
    else if (square[3] == square[6] && square[6] == square[9])
        return WON;
    else if (square[1] == square[5] && square[5] == square[9])
        return WON;
    else if (square[3] == square[5] && square[5] == square[7])
        return WON;
    else
        return RUNNING; 
}


UINT1 InitializeState()
{
	//LOCK
	UINT1 retVal = SUCCESS;

	gLocalState.StateData = 0;
	strncpy (gLocalState.playerName, DEF_PLAYER_NAME, strlen(DEF_PLAYER_NAME));
	gLocalState.gameFlag = 0;

	return retVal;
	//UNLOCK
}
