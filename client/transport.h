#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

#define SMARTPEER_CLIENT_LISTENING_PORT_CLIENT 9001
#define SMARTPEER_CLIENT_LISTENING_PORT_SERVER 9000
#define SMARTPEER_SERVER_PORT 10000
#define BUFFER_SIZE 1024

using namespace std;

class Socket
{
    public:
	int m_sockFD ;
	char m_buffer[BUFFER_SIZE];
	struct sockaddr_in m_serverAddr;
	struct sockaddr_storage m_serverStorage;
	socklen_t m_addr_size;

	Socket();
	~Socket();
	int Create(ulong UdpPort);
	bool Bind();
	bool Listen(int que);
	bool Accept(Socket &clientSock);
	bool Connect(string address, int port);
	//     int Recieve(char *buff, int buffSize);
	//     int Send(const char *buff, int len);
	bool Close();
};

class IDataCommunication
{
    public:
	IDataCommunication () {};
	~IDataCommunication () {};
	virtual void sendData(Socket *) = 0;
	virtual void *recvData(void) = 0;
};

class TCPStream : public IDataCommunication
{
    public:
	void sendData(Socket *);
	void *recvData(void);
};

class UDPDatagram : public IDataCommunication
{
    //friend class Socket;


    public:
	Socket *serverSocket;
	Socket *clientSocket;
	UDPDatagram();
	~UDPDatagram();
	void sendData(Socket *);
	void *recvData(void);
        void sendDataToServer(string data);
        void sendDataToClient(string data);
        void processDataComingFromSocket (char *pProtoBuf, int nLen);
	Socket * getServerSocket() {  return serverSocket; }
	Socket * getClientSocket() {  return clientSocket; }
	static void *helper_function (void *ptr)
	{
	    return ((UDPDatagram *)ptr)->recvData();
	}
};

#endif /*__TRANSPORT_H__ */
