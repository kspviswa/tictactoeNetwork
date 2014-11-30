#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "transport.h"


using namespace std;

int main()
{
    UDPDatagram udpPacket;
    //    udpPacket.sendData(clientSocket);
    //    udpPacket.recvData(serverSocket);

    pthread_t listeningThread;
    int  iret1 = 0, option = 0;

    /* 
     * Create Two sockets as part of client Startup Sequence
     * One socket listening towards the Server
     * another socket listening towars the Peer - Client
     */
    udpPacket.getServerSocket()->Create(SMARTPEER_SERVER_PORT);
    udpPacket.getClientSocket()->Create(SMARTPEER_CLIENT_PORT);

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
 
    //pthread_join( listeningThread, NULL);
    do
    {
        cout << "\r Enter 0 - To Exit \r\n";
        cout << "\r Enter 1 - To Send Data to Client Socket \r\n";
        cout << "\r Enter 2 - To Send Data to Server Socket \r\n";
        cin >> option;

	switch (option)
	{
            case 0:
                cout << "!!! Bye Bye !!!" ;
		break;

	    case 1:
                udpPacket.sendDataToClient("MESSAGE FROM CLIENT");              
		break;

	    case 2:
                udpPacket.sendDataToServer("MESSAGE FROM SERVER");
		break;

	    default:
		    cout << "Invalid Entry ... Please try again";
		    break;

	}
    }while(option);


    return (0);
}

