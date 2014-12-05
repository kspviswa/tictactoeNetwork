#include "iostream"
#include "headers.h"
#include "tictac.h"
#include <sstream>
#include "transport.h"
#include "headers.h"
#include "../game2.h"

using namespace tictac;
using namespace std;

UINT1 gNewResume = RESET;
/*tictacpacket gLocalGameState;*/
tUserData gLocalGameState;

Game2 *Game_Ref;


/* Create an UPDPacket Object - which will create Two Sockets */  
UDPDatagram udpPacket;
string IntToString(unsigned long n)
{
	stringstream ss;
	ss << n;
	return ss.str();
}

int Store_Pointer(void * gPtr)
{
    Game_Ref = (Game2 *)gPtr;
    printf("Store_pointer = %p\n",gPtr);
    return 0;
}

unsigned long StringToIpInt(string SIP)
{
        unsigned long nIP;
        inet_pton(AF_INET, SIP.c_str(), &nIP);
        return nIP;
}

string IpAddrToString(unsigned long nIpv4)
{
    char szIP[INET_ADDRSTRLEN];
    string sIP;

    inet_ntop(AF_INET, &nIpv4, szIP, INET_ADDRSTRLEN);

    sIP.assign(szIP, strlen(szIP));

    return sIP;
}


/* calculate game status */




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
    string peerIP(gLocalGameState.PeerIP);

    localMove.set_msgtype(tictacpacket::MOVE);
    localMove.set_state(user_record.game_state);
    localMove.set_ipv4opp(StringToIpInt(gLocalGameState.LocalIP));
    localMove.set_ipv4(StringToIpInt(gLocalGameState.LocalIP));
    localMove.set_playername(gLocalGameState.lplayerName);

    //Update the Stable state after successful sent to player
    strncpy(gLocalGameState.game_state, user_record.game_state, MAX_SQUARE);

    //TATA - move call with "localMove"
    string data = localMove.SerializeAsString();
    printf ("User_Record.gamestate = %s\n", user_record.game_state);
    printf ("Data Sent fro Game logic = %s\n", data.c_str());

    udpPacket.sendDataToClient(peerIP, data);

    return retVal;
}

UINT1 UpdateBoard( tictacpacket remoteChange )
{

    UINT1		retVal = FAILURE;

    /*if ( (gNewResume == SET) &&
        (FAILURE == is_board_update(const_cast<char*>(remoteChange.state().c_str()))))
    {
        Game_Ref->show_pop_up("SNAPSHOT was Old, so updatded with current player data\n");
    }*/

    if (strlen(gLocalGameState.PeerIP) == 0)
    {
        memcpy(gLocalGameState.PeerIP, IpAddrToString(remoteChange.ipv4opp()).c_str(),sizeof(gLocalGameState.PeerIP) );
    }

    if (strlen(gLocalGameState.rplayerName) == 0)
    {
        strncpy( gLocalGameState.rplayerName, remoteChange.playername().c_str(), MAX_NAME);
        printf("Local Player = %s - Remote Player = %s\n", gLocalGameState.lplayerName,gLocalGameState.rplayerName);
    }

    strncpy( gLocalGameState.game_state, remoteChange.state().c_str(), MAX_SQUARE);

    printf("Update Board - Local Symbol = %s Peer Address = %s\n", gLocalGameState.lFlag == 'X' ? "X":"O", gLocalGameState.PeerIP);

    // SANKAR GUI function
    //Game_Ref->gui_update_board ( gLocalGameState );

    return retVal;
}
/* Got a move from other player updating to own board */


/* Register with with player to start the play */
/* GUI will call this with player name */
UINT1 RegisterPlayer(tUserData *user_record )
{

    tictacpacket	register_msg;
    tictacpacket	register_resp;
    UINT1 ret_Val = SUCCESS, iret1 = 0;

    memcpy(&gLocalGameState.game_state,"ZZZZZZZZZ",9);

    string serverIP(user_record->ServerIP);
    string selfIP  (user_record->LocalIP);

    cout << "Sandeep Local IP from GUI : "<< selfIP.c_str() << endl;
    cout << "Sandeep Server IP from GUI : "<< serverIP.c_str() << endl;

    // Thread for sending snapshot every 5 seconds
    pthread_t snap_shot_thread;

    // Thread for Receiving Packet from Receive Socket 
    pthread_t listeningThread;

    /* 
     * Create Two sockets as part of client Startup Sequence
     * One socket listening towards the Server
     * another socket listening towars the Peer - Client
     */
    udpPacket.getServerSocket()->Create(selfIP, SMARTPEER_CLIENT_LISTENING_PORT_SERVER);
    udpPacket.getClientSocket()->Create(selfIP, SMARTPEER_CLIENT_LISTENING_PORT_CLIENT);

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
    register_msg.set_ipv4(StringToIpInt(user_record->LocalIP));
    register_msg.set_msgtype ( tictacpacket::REGISTER );
    register_msg.set_playername ( user_record->lplayerName );

    // Register call with "register_msg"
    string data = register_msg.SerializeAsString();
    udpPacket.sendDataToServer(serverIP, data);

    strcpy(gLocalGameState.ServerIP, user_record->ServerIP);
    strcpy(gLocalGameState.LocalIP, user_record->LocalIP);

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


    string serverIP(gLocalGameState.ServerIP);

    while (1)
    {
	strncpy(state_String, gLocalGameState.game_state, MAX_SQUARE);
	state_String[MAX_SQUARE] = *(char *)user_sign;
    snap_msg.set_msgtype(tictacpacket::SNAPSHOTPUT);
	snap_msg.set_state(state_String);
    snap_msg.set_ipv4(StringToIpInt(gLocalGameState.LocalIP));


    // sending snapshot packet to server
    printf("Sending snapshot to server");
    string data = snap_msg.SerializeAsString();
    udpPacket.sendSnapShotToServer(serverIP, data);
    sleep(SNAP_SHOT_FREQ);
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

void recv_data(tUserData &stUserData)
{
    //memcpy(&gLocalGameState.game_state,"OZOZZXZZZ",9);
    //printf("%s\n", "Data Refreshed");

    //README - GUI Thread will poll the updated data from gLocalGameState.. Please update
    //the structure whenever possible.

    memcpy(&stUserData,&gLocalGameState,sizeof(gLocalGameState));
}

/*int main ()
{
    UINT1 retVal = SUCCESS, option;
    tUserData userDate;
    string peerIP, data;
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
		//udpPacket.sendDataToClient("MESSAGE FROM CLIENT");
		break;

	    case '2':
		strcpy(userDate.lplayerName, "Venkat");
		//RegisterPlayer(&userDate);
		break;

	    default:
		cout << "\r Invalid Entry ... Please try again \r\n";
		break;

	}
    }while(option);	
    return retVal;
}*/


