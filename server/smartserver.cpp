/*
 * smartserver.cpp
 *
 *  Created on: 25-Oct-2014
 *      Author: kspviswa
 */

#include "components.h"
#include "tictac.h"
#include <sstream>

using namespace tictac;

// Global vars
unsigned long nServerport = 0; // Port which server would listen to.
unsigned long nTimer = 0; // Timer to get snapshots

CController theGameController; // Singleton controller for game

void displayErrorAndQuit(int nError)
{
	switch(nError)
	{
	case -1:
		cout << "Less args entered" << endl;
		break;
	case -2:
		cout << "Configure proper port for server" << endl;
		break;
	case -3:
		cout << "Minimum value for the timer should be 1 sec" << endl;
		break;
	default:
	}

	cout << "Usage" << endl;
	cout << "+++++" << endl;
	cout << endl;
	cout << "smartserver <server listening port> <timer for snapshot in secs>" << endl;
	cout << "smartserver -h for help" << endl;
	exit(-1);
}

unsigned long str2uint32 (char const *s)
{
    stringstream ss(s);
    unsigned long nTemp;
    ss >> nTemp;
    return nTemp;
}

/**
 * processIncomingMessage()
 * This function will be called, after populating the tictacpacket.
 * This function will process the packet, as per the Message Type.
 * Input : A populate tictacpacket
 * Output : nothing.
 */
void processIncomingMessage(tictacpacket thePacket)
{
	/**
	 * When we have thePacket, it is quit safe to predict that,
	 * the thePacket is complete. else parse would have been failed.
	 */

	if(!thePacket.IsInitialized())
	{
		cout << "Malformed packet" << endl;
	}

	// Switch processing w.r.t incoming type
	switch(thePacket.msgtype())
	{
	case tictacpacket::REGISTER :
		// A new client arrived. Please do the needful
		break;
	case tictacpacket::SNAPSHOTPUT:
		// A client has sent their snapshot. Proceed.
		break;
	case tictacpacket::TERMINATE:
		// A client has decided to quit. Do the needful.
		break;
	case tictacpacket::END:
		// A match has ended. Record it.
		break;
	default:
		cout << "Not processed" << endl;
	}
}

/**
 * parsePacket()
 * This function will be called after reading the incoming raw bytes on the socket
 * Input : pointer to source buffer, that was filled from socket
 * Output : parsing result.
 */
long parsePacket(unsigned char *pSrc, unsigned long nLen)
{
	tictacpacket thePacket;
	if(thePacket.ParseFromArray(pSrc, nLen))
	{
		// If parse is successful, then proceed to process the packet
		processIncomingMessage(thePacket);
		return 0; // success
	}
	else
		return -1; // Failure
}

/**
 * SnapshotService()
 * This is a daemon thread. It will sleep for nTimer and gets the snapshots
 * of all the players and save it under their statistics.
 */
void SnapshotService(void *pDummy)
{

}

int main(int argc, char *argv[])
{
	/**
	 * As of now I am living with 2 args
	 * Arg 1 => Listening port for the server
	 * Arg 2 => Timer for querying the snapshots from clients.
	 *
	 * So the argc will be prog-name + 2 args = 3
	 */

	// Sanity check
	if(argc < 1 || argc > 3)
	{
		displayErrorAndQuit(-1);
	}

	nServerport = str2uint32(argv[1]);
	nTimer = str2uint32(argv[2]);

	if(nServerport < 1)
	{
		displayErrorAndQuit(-2);
	}

	if(nTimer < 0)
	{
		displayErrorAndQuit(-3);
	}

	/**
	 * Here is what the real logic starts.
	 *
	 * There is going to be 3 threads basically.
	 *
	 * Thread 1 = Socket listening daemon.
	 * Thread 2 = Snapshot timer thread
	 * Thread 3 = Statistics thread.
	 *
	 * Socket listening daemon
	 * +++++++++++++++++++++++
	 *
	 * It will listen on the designated nServerport.
	 * Create a new CConn object for every connection. Create a CPlayer object and attach CConn.
	 * Spawn a thread and call a static method from CPlayer to start processing for that Player.
	 *
	 * Snapshot timer daemon
	 * +++++++++++++++++++++
	 *
	 * Timed daemon. Will sleep for nTimer seconds.
	 * For every wake, walk around the registered clients and get the snapshot and store.
	 *
	 * Statistics daemon
	 * +++++++++++++++++
	 *
	 * HTTP Server running on 9090 port. It will print dynamic pages, every time you have a http request
	 * Print the contents of CStatistics.
	 */
}



