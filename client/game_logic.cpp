#include "iostream"
#include"headers.h"
#include"tictac.h"
#include "transport.h"
using namespace tictac;
using namespace std;

UINT1 gNewResume = RESET;
/*tictacpacket gLocalGameState;*/
tUserData gLocalGameState;

/* Create an UPDPacket Object - which will create Two Sockets */  
UDPDatagram udpPacket;

unsigned long StringToIpInt(string SIP)
{
        unsigned long nIP;
        inet_pton(AF_INET, SIP.c_str(), &nIP);
        return nIP;
}

/* calculate game status */
UINT1 CalculateGameStatus( const char *square )
{
    UINT1 ret_val = RUNNING;
    UINT1 loop;

    for (loop=8; loop>=0; loop-- )
    {
	if (square[loop] == NOT_INTIALISED)
	{
	    ret_val = RUNNING;
	    break;
	}
    }
    if (loop == 0)
	ret_val = tictacpacket::DRAW;

    if (square[0] == square[1] && square[1] == square[2])
	ret_val = tictacpacket::WON;
    else if (square[3] == square[4] && square[4] == square[5])
	ret_val = tictacpacket::WON;
    else if (square[6] == square[7] && square[7] == square[8])
	ret_val = tictacpacket::WON;
    else if (square[0] == square[3] && square[3] == square[6])
	ret_val = tictacpacket::WON;
    else if (square[1] == square[4] && square[4] == square[7])
	ret_val = tictacpacket::WON;
    else if (square[2] == square[5] && square[5] == square[8])
	ret_val = tictacpacket::WON;
    else if (square[0] == square[4] && square[4] == square[8])
	ret_val = tictacpacket::WON;
    else if (square[2] == square[4] && square[4] == square[6])
	ret_val = tictacpacket::WON;

    return ret_val;
}

UINT1 is_board_update ( char *remote_str )
{
    UINT1 ret_val = SUCCESS;
    UINT1 remote_move = 0, local_move =0 , loop;

    for (loop=8; loop>=0; loop-- )
    {
	if ( (remote_str[loop] == gLocalGameState.rFlag)
		|| (remote_str[loop] == gLocalGameState.lFlag));
	remote_move++;

	if ( (gLocalGameState.game_state[loop] == gLocalGameState.rFlag) 
		|| (gLocalGameState.game_state[loop] == gLocalGameState.lFlag));
	local_move++;
    }

    if (remote_move > local_move)
	ret_val = FAILURE;

    return ret_val;
}

// Send move to other player 
UINT1 SendMove( tUserData user_record )
{
    UINT1 retVal = FAILURE;
    tictacpacket localMove;

    localMove.set_msgtype(tictacpacket::MOVE);
    localMove.set_state(user_record.game_state);

    //TATA - move call with "localMove"
    string data = localMove.SerializeAsString();
    udpPacket.sendDataToServer(data);

    //Update the Stable state after successful sent to player
    strncpy(gLocalGameState.game_state, user_record.game_state, MAX_SQUARE);

    return retVal;
}

/* Got a move from other player updating to own board */
UINT1 UpdateBoard( tictacpacket remoteChange )
{

    UINT1		retVal = FAILURE;

    if ( (gNewResume == SET) &&
	    (FAILURE == is_board_update(const_cast<char*>(remoteChange.state().c_str()))))
    {
	//show_poop_up("SNAPSHOT was Old, so updatded with current player data\n");
    }

    // SANKAR GUI function
    //gui_update_board(remoteChange.state());

    // Calculate Game Status and inform GUI
    switch( CalculateGameStatus(remoteChange.state().c_str()))
    {
	case tictacpacket::WON:
	    //show_poop_up("game WON!!!\n");
	    break;

	case tictacpacket::LOST:
	    //show_poop_up("game LOST!!!\n");
	    break;

	case tictacpacket::DRAW:
	    //show_poop_up("game DRAW!!!\n");
	    break;

	default:
	    //show_poop_up("game running\n");
	    printf("game running\n");
    }

    //Update the Stable state after successful sent to GUI
    if (retVal == SUCCESS)
    {
	/*gLocalGameState.set_state(remoteChange.state());*/
	strncpy(gLocalGameState.game_state, remoteChange.state().c_str(), MAX_SQUARE);
    }

    return retVal;
}

/* Register with with player to start the play */
/* GUI will call this with player name */
UINT1 RegisterPlayer(tUserData *user_record )
{

    tictacpacket	register_msg;
    tictacpacket	register_resp;
    UINT1 ret_Val = SUCCESS, iret1 = 0;


    // Thread for sending snapshot every 5 seconds
    pthread_t snap_shot_thread;

    // Thread for Receiving Packet from Receive Socket 
    pthread_t listeningThread;

    /* 
     * Create Two sockets as part of client Startup Sequence
     * One socket listening towards the Server
     * another socket listening towars the Peer - Client
     */
    udpPacket.getServerSocket()->Create(SMARTPEER_CLIENT_LISTENING_PORT_SERVER);
    udpPacket.getClientSocket()->Create(SMARTPEER_CLIENT_LISTENING_PORT_CLIENT);

    // Create independent thread each of which will execute function 

    iret1 = pthread_create( &listeningThread, NULL, &UDPDatagram::helper_function, &udpPacket);
    if(iret1)
    {
	fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
	exit(EXIT_FAILURE);
    }
    else
    {
	printf("Thread SUCCESSFULLY created \n");
    }
    register_msg.set_ipv4(StringToIpInt("127.0.0.1"));
    register_msg.set_msgtype ( tictacpacket::REGISTER );
    register_msg.set_playername ( user_record->lplayerName );

    // Register call with "register_msg"
    string data = register_msg.SerializeAsString();
    udpPacket.sendDataToServer(data);

#if 0
    // Choose the user sign based upon registration sequance
    // First player  - 'X'
    // Second player - 'O'
    switch(register_resp.msgtype())
    {
	case tictacpacket::OK:
	    user_record->lFlag = FIRST_X;
	    user_record->rFlag = SECOND_O;
	    break;

	case tictacpacket::START:
	    user_record->lFlag = SECOND_O;
	    user_record->rFlag = FIRST_X;
	    strcpy(user_record->rplayerName, register_msg.playername().c_str());
	    break;

	case tictacpacket::RESUME:
	    user_record->lFlag = register_msg.state()[MAX_SQUARE];
	    // SANKAR GUI function
	    // gui_update_board();
	    gNewResume = SET;
	    break;

	default:

	    printf("resiter failed, unexpected msg_type");
	    //show_poop_up("resiter failed, unexpected msg_type");
    }
#endif
    if (SUCCESS != pthread_create(&snap_shot_thread, NULL, SendSnapShot, (void *) &gLocalGameState.lFlag))
    {
	printf("not able to create snapshot thread\n");
    }

    return ret_Val;
}

/* Get the current status of the game and send it */
// should be called in ragular interval in a separate thread
void *SendSnapShot(void* user_sign)
{
    tictacpacket snap_msg;
    char state_String[MAX_SQUARE+1];

    snap_msg.set_msgtype(tictacpacket::SNAPSHOTPUT);

    while (1)
    {
	strncpy(state_String, gLocalGameState.game_state, MAX_SQUARE);
	state_String[MAX_SQUARE] = *(char *)user_sign;
	snap_msg.set_state(state_String);

	// Viswa sendSnap Call with snap_msg
	//
	usleep(SNAP_SHOT_FREQ);
    }

    return 0;
}

/* End game */
UINT1 EndGame()
{
    tictacpacket end_pkt;
    UINT1		retVal = SUCCESS;

    end_pkt.set_msgtype(tictacpacket::TERMINATE);

    // Other player need to be informed

    // thread for snapshot should be exited
    //TerminateThread ( snap_shot_thread, 0 );
    //CloseHandle ( snap_shot_thread );
    return retVal;

}

int main ()
{
    UINT1 retVal = SUCCESS, option;
    tUserData userDate;
    bzero(&userDate, sizeof(tUserData));

    do
    {
	cout << "\r Enter 0 - To Exit \r\n";
	cout << "\r Enter 1 - To Send Move to Peer Client \r\n";
	cout << "\r Enter 2 - To Register Player \r\n";
	cin >> option;

	switch (option)
	{
	    case '0':
		cout << "!!! Bye Bye !!!" ;
		break;

	    case '1':
		strcpy(userDate.game_state, "OXOXOXZXO");
		udpPacket.sendDataToClient("MESSAGE FROM CLIENT");
		break;

	    case '2':
		strcpy(userDate.lplayerName, "Venkat");
		RegisterPlayer(&userDate);
		break;

	    default:
		cout << "\r Invalid Entry ... Please try again \r\n";
		break;

	}
    }while(option);	
    return retVal;
}


