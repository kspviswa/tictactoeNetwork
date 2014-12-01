#include <iostream>
#include <pthread.h>

#include "packetProcess.h"
#include "transport.h"
#include "tictac.h"

/*
using namespace std;
using namespace tictac;
*/

Socket::Socket()
{
    /*Initialize size variable to be used later on*/
    m_sockFD    = 0;
    m_addr_size = sizeof(m_serverAddr);
    bzero(&m_buffer,      sizeof(m_buffer));
    bzero(&m_serverStorage, sizeof(sockaddr_storage));
    bzero(&m_serverAddr,  sizeof(sockaddr_in));
    printf("\r Inside Function = %s with SockFD= %d\r\n", __func__, m_sockFD);
}

Socket::~Socket()
{

}

/* This function needs to be invoked during Registration of a client */
int Socket::Create( string selfIP,  ulong UdpPort)
{
    /*Create UDP socket*/
    m_sockFD = socket(AF_INET, SOCK_DGRAM, 0);

    printf(" Local Socket = %d\n",m_sockFD);

    /*Configure settings in address struct*/
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(UdpPort);
    //m_serverAddr.sin_addr.s_addr = inet_addr(selfIP.c_str());
    m_serverAddr.sin_addr.s_addr = inet_addr(selfIP.c_str());
    memset(m_serverAddr.sin_zero, '\0', sizeof m_serverAddr.sin_zero);

    Bind();

    return m_sockFD;
}

bool Socket::Bind()
{
    /*Bind socket with address struct*/
    bind(m_sockFD, (struct sockaddr *) &m_serverAddr, sizeof( m_serverAddr));

    /*Initialize size variable to be used later on*/
    m_addr_size = sizeof m_serverStorage;

    return true;
}

/* This is a wrapper function to invoke the recvData Method from a thread */
static void *helper_function (void *ptr)
{
    printf("\r I am Inside Function = %s \r\n", __func__);
    return ((UDPDatagram *)ptr)->recvData();
}

void TCPStream::sendData (Socket *)
{

};

void *TCPStream::recvData (void)
{

};

UDPDatagram :: UDPDatagram()
{
    serverSocket = new Socket;
    clientSocket = new Socket;
}

UDPDatagram :: ~UDPDatagram()
{
    delete (serverSocket) ;
    delete (clientSocket) ;

    serverSocket = NULL;
    clientSocket = NULL;
}

void UDPDatagram::sendDataToServer(string serverIP, string data)
{
	
    printf("\r ServerIp = %s, sendDataToServer = %s\r\n", serverIP.c_str(), data.c_str());

    /*Create UDP socket*/
    int nSock  = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in m_serverAddr;
    printf(" Local Socket = %d\n", nSock);

    /*Configure settings in address struct*/
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(SMARTPEER_SERVER_PORT);
    /* get the server ip from sandeep */
    m_serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    memset(m_serverAddr.sin_zero, '\0', sizeof m_serverAddr.sin_zero);

    /*Send message to server*/
    sendto( nSock, 
	    data.c_str(), 
	    data.length(),
	    0,
	    (struct sockaddr *)&m_serverAddr,
	    sizeof(sockaddr));

    /* sandeep - Replace Below constant string with data from pDataBuffer*/
    //memcpy(serverSocket->m_buffer, data.c_str(), data.length());

    //sendData (serverSocket);

    //memset(serverSocket->m_buffer, 0, BUFFER_SIZE);

}

void UDPDatagram::sendDataToClient(string peerIP, string data)
{
    printf("\r sendDataToClient = %s\r\n", data.c_str());

    /*Create UDP socket*/
    int nSock  = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in m_serverAddr;
    printf(" Local Socket = %d\n", nSock);

    /*Configure settings in address struct*/
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(SMARTPEER_CLIENT_LISTENING_PORT_CLIENT);

    /* get the client ip from sandeep */
    m_serverAddr.sin_addr.s_addr = inet_addr(peerIP.c_str());
    memset(m_serverAddr.sin_zero, '\0', sizeof m_serverAddr.sin_zero);

    /*Send message to server*/
    sendto( nSock, 
	    data.c_str(), 
	    data.length(),
	    0,
	    (struct sockaddr *)&m_serverAddr,
	    sizeof(sockaddr));

    /* sandeep - Replace Below constant string with data from pDataBuffer*/
/*
    memcpy(clientSocket->m_buffer, data.c_str(), data.length());

    sendData (clientSocket);

    memset(clientSocket->m_buffer, 0, BUFFER_SIZE);
*/
}

void UDPDatagram::sendData (Socket *pSocket)
{
    int nBytes = 0;

    nBytes = strlen(pSocket->m_buffer) + 1;

    printf("\r Socket FD = %d \r\n", pSocket->m_sockFD);
    printf("\r Buffer    = %s \r\n", pSocket->m_buffer);
    printf("\r Bytes Tx  = %d \r\n", nBytes);

    /*Send message to server*/
    sendto( pSocket->m_sockFD, 
	    pSocket->m_buffer, 
	    nBytes,
	    0,
	    (struct sockaddr *)&pSocket->m_serverAddr,
	    pSocket->m_addr_size);
}

void *UDPDatagram::recvData (void)
{
    Socket *pSocket;
    int nBytes = 0;

    struct timeval tv;
    fd_set rfds;

    printf("\r!!! Ready to Listen Read Sockets !!! \r\n");

    while(1)
    {
	/* Wait up to Ten seconds. */
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	FD_ZERO(&rfds);
	FD_SET(serverSocket->m_sockFD, &rfds);
	FD_SET(clientSocket->m_sockFD, &rfds);

	/*  
         * Try to receive any incoming UDP datagram.  
         * Data can come from server of Peer Client.
         * Address and port of requesting client will be stored on m_serverStorage variable 
         */
        if(-1 == select(5, &rfds, NULL, NULL, &tv))
        {
            perror("select()");
        }
	else if(FD_ISSET(serverSocket->m_sockFD, &rfds))
	{
            cout << "\r Started Receiving Data from Server \r\n";
	    nBytes = recvfrom(serverSocket->m_sockFD, 
		    serverSocket->m_buffer,
		    BUFFER_SIZE,
		    0,
		    (struct sockaddr *)&serverSocket->m_serverStorage, 
		    &serverSocket->m_addr_size);
            cout << serverSocket->m_buffer << endl;

            /* Send the packet to Proto Buff for Decoding the incoming Message */
            processDataComingFromSocket(serverSocket->m_buffer, strlen(serverSocket->m_buffer));          
	}
	else if(FD_ISSET(clientSocket->m_sockFD, &rfds))
	{
            cout << "\r Started Receiving Data from Client \r\n";
	    nBytes = recvfrom(clientSocket->m_sockFD, 
		    clientSocket->m_buffer,
		    BUFFER_SIZE,
		    0,
		    (struct sockaddr *)&clientSocket->m_serverStorage, 
		    &clientSocket->m_addr_size);
            cout << clientSocket->m_buffer << endl;

            /* Send the packet to Proto Buff for Decoding the incoming Message */
            processDataComingFromSocket(clientSocket->m_buffer, strlen(clientSocket->m_buffer));          
	}

        cout << " Waiting On Select Call " << endl;
        memset(serverSocket->m_buffer, 0, sizeof (serverSocket->m_buffer));
    }

    return 0;
}

void UDPDatagram::processDataComingFromSocket (char *pProtoBuf,  int nLen)
{
    /* Invoke Google Proto Buffer API to convert 
     * Serial Raw Data into Respective Message Processing
     */
    CPlayer player;
    player.parsePacket((unsigned char *)pProtoBuf, nLen);
}

