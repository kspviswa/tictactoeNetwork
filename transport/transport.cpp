#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define SMARTPEER_CLIENT_PORT 9000
#define SMARTPEER_SERVER_PORT 10000

using namespace std;

class Socket
{
    public:
	int m_sockFD ;
	char buffer[1024];
	struct sockaddr_in m_serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t m_addr_size;

	Socket();
	~Socket();
	bool Create();
	bool Bind();
	bool Listen(int que);
	bool Accept(Socket &clientSock);
	bool Connect(string address, int port);
	//     int Recieve(char *buff, int buffSize);
	//     int Send(const char *buff, int len);
	bool Close();
};

Socket::Socket()
{
    /*Initialize size variable to be used later on*/
    m_addr_size = sizeof m_serverAddr;
}

Socket::~Socket()
{

}

class IDataCommunication
{
    public:
	IDataCommunication () {};
	~IDataCommunication () {};
	virtual void sendData(Socket *) = 0;
	virtual void recvData(Socket *) = 0;
};

class TCPStream : public IDataCommunication
{
    public:
	void sendData(Socket *);
	void recvData(Socket *);
};

void TCPStream::sendData (Socket *)
{

};

void TCPStream::recvData (Socket *)
{

};

class UDPDatagram : public IDataCommunication
{
    friend class Socket;

    public:
	void sendData(Socket *);
	void recvData(Socket *);
};

void UDPDatagram::sendData (Socket *pSocket)
{
    int nBytes = 0;
    while(1)
    {
	printf("Type a sentence to send to server:\n");
	fgets(pSocket->buffer,1024,stdin);
	printf("You typed: %s",pSocket->buffer);

	nBytes = strlen(pSocket->buffer) + 1;

	/*Send message to server*/
	sendto( pSocket->m_sockFD, 
		pSocket->buffer, 
		nBytes,
		0,
		(struct sockaddr *)&pSocket->m_serverAddr,
		pSocket->m_addr_size);

	/*Receive message from server*/
	nBytes = recvfrom(pSocket->m_sockFD,
		          pSocket->buffer,
		          1024,
		          0,
		          NULL, 
		          NULL);

	printf("Received from server: %s\n",pSocket->buffer);

    }
}

void UDPDatagram::recvData (Socket *pSocket)
{
    int nBytes = 0;
    while(1)
    {
	/* Try to receive any incoming UDP datagram. Address and port of
	   requesting client will be stored on serverStorage variable */
	nBytes = recvfrom(pSocket->m_sockFD, 
                          pSocket->buffer,
                          1024,
                          0,
                          (struct sockaddr *)&pSocket->serverStorage, 
                           &pSocket->m_addr_size);

	/*Convert message received to uppercase*/
	for(int i=0; i < nBytes-1; i++)
	    pSocket->buffer[i] = toupper(pSocket->buffer[i]);

	/*Send uppercase message back to client, using serverStorage as the address*/
	sendto(pSocket->m_sockFD,
               pSocket->buffer,
               nBytes,
               0,
               (struct sockaddr *)&pSocket->serverStorage,
               pSocket->m_addr_size);
    }
}

bool Socket::Create( /* IP Address */ /* Port */)
{
    /*Create UDP socket*/
    m_sockFD = socket(PF_INET, SOCK_DGRAM, 0);

    /*Configure settings in address struct*/
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(7891);
    m_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(m_serverAddr.sin_zero, '\0', sizeof m_serverAddr.sin_zero);

    return m_sockFD;
}

bool Socket::Bind()
{
    /*Bind socket with address struct*/
    bind(m_sockFD, (struct sockaddr *) &m_serverAddr, sizeof( m_serverAddr));

    /*Initialize size variable to be used later on*/
    m_addr_size = sizeof serverStorage;

    return true;
}
int main()
{
    Socket *serverSocket = new Socket;
    Socket *clientSocket = new Socket;
    UDPDatagram udpPacket;
    udpPacket.sendData(clientSocket);
    udpPacket.recvData(serverSocket);
    return (0);
}
