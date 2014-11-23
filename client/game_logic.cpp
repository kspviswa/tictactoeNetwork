#include"headers.h"
#include"tictac.h"
using namespace tictac;

tgameState  gLocalState;
tgameState  gRemoteState;

int main ()
{
	InitializeState();
	return SUCCESS;
}

// Send move to other player 
UINT1 SendMove( char * stateString )
{

	UINT2 StateData = 0;
	UINT1 retVal = FAILURE;

	// Get the bitwise value from the UI string data for player state
	if (StringToData ( &StateData, stateString ) == FAILURE)
	{
		SET_ERROR("StringToData failed \n");
		retVal = FAILURE;    
	}

	// Without any position change submit button
	if (StateData == gLocalState.StateData )
	{
		SET_ERROR("Invalid Move \n");
		retVal = FAILURE;    
	}

	// *TATA* Function to update the peer side
	// 


	//Update the Stable state after successful sent to player
	if (retVal == SUCCESS)
	{
		SetStableState(StateData, LOCAL_STATE);
	}

	return retVal;
}

/* Got a move from other player updating to own board */
UINT1 UpdateBoard( UINT2 StateData )
{
	char StateString[10] = {0};
	UINT1 retVal = SUCCESS;

	if (DataToString ( StateString, gRemoteState, gLocalState ) == FAILURE)
	{
		retVal = FAILURE;    
	}

	// Call GUI function to update the GUI(arg - StateString)


	// Calculate Game Status and inform GUI
	CalculateGameStatus(StateString);

	return retVal;
}

/* Register with with player to start the play */
/* GUI eill call this with player name */
UINT1 RegisterPlayer(char *playerName )
{
	/*tictacpacket register_msg;
	register_msg.set_msgtype(tictacpacket::RESUME);
	register_msg.set_state(pNewPlayer->strState);
		
			= TRANS_registerPlayer(playerName);
	*/
	return 0;
}

/* Get the current status of the game and send it */
UINT1 SendSnapShot()
{
	/*tictacpacket snap_msg;

	snap_msg.set_state(gLocalState::state);
	snap_msg.set_state(gLocalState::playerName);*/
	return 0;
}
