/*
 * smartserver.cpp
 *
 *  Created on: 25-Oct-2014
 *      Author: kspviswa
 */

#include "components.h"
#include <sstream>

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

void processClients()
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

	unsigned long nServerport = 0;
	unsigned long nTimer = 0;


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



