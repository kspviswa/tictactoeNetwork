/*
 * components.cpp
 *
 *  Created on: 23-Nov-2014
 *      Author: Venkat 
 */

#include <netinet/in.h>
#include <arpa/inet.h>
#include "iostream"
#include "packetProcess.h"
#include "tictac.h"
#include "headers.h"

extern UINT1 gNewResume;
extern tUserData gLocalGameState;

/* CPlayer Constructor */
CPlayer::CPlayer()
{

}

/* CPlayer Destructor  */
CPlayer::~CPlayer()
{

}

/**
 * processIncomingMessage()
 * This function will be called, after populating the tictacpacket.
 * This function will process the packet, as per the Message Type.
 * Input : A populate tictacpacket
 * Output : nothing.
 */
void CPlayer::processIncomingMessage(tictacpacket thePacket)
{
    /**
     * When we have thePacket, it is quit safe to predict that,
     * the thePacket is complete. else parse would have been failed.
     */

    cout << "I am inside Function " << __func__ << endl;

    if(!thePacket.IsInitialized())
    {
	cout << "Malformed packet" << endl;
    }

    // Switch processing w.r.t incoming type
    switch(thePacket.msgtype())
    {
	case tictacpacket::OK :
	    processOkMessage(&thePacket);
	    break;
	case tictacpacket::START:
	    processStartMessage(&thePacket);
	    break;
	case tictacpacket::RESUME:
	    processResumeMessage(&thePacket);
	    break;
	case tictacpacket::MOVE:
            processMoveMessage(&thePacket);
	    break;
	default:
	  cout << "ERROR: Invalid Packet Received" << endl;
	    break;
    }
}

int CPlayer::processOkMessage(tictacpacket *pPacket)
{
    cout << "I am inside Function " << __func__ << endl;

    gLocalGameState.lFlag = FIRST_X;
    gLocalGameState.rFlag = SECOND_O;

    // gui_update_data (gLocalGameState);
    return 0;
}
int CPlayer::processStartMessage(tictacpacket *pPacket)
{
    cout << "I am inside Function " << __func__ << endl;

    gLocalGameState.lFlag = SECOND_O;
    gLocalGameState.rFlag = FIRST_X;

    strcpy(gLocalGameState.rplayerName, pPacket->playername().c_str());
    strcpy(gLocalGameState.PeerIP, IntToString(pPacket->ipv4opp()).c_str());

    // gui_update_data (gLocalGameState); 
    return 0;
}
int CPlayer::processResumeMessage(tictacpacket *pPacket)
{
    cout << "I am inside Function " << __func__ << endl;

    gLocalGameState.lFlag = pPacket->state()[MAX_SQUARE];

    // SANKAR GUI function

    // gui_update_data (gLocalGameState); 
    // gui_update_board();
    gNewResume = SET;

    return 0;
}
int CPlayer::processMoveMessage(tictacpacket *pPacket)
{
    cout << "I am inside Function " << __func__ << endl;

    UpdateBoard(*pPacket);
    strcpy(gLocalGameState.PeerIP, IntToString(pPacket->ipv4opp()).c_str());
    return 0;
}

void CPlayer::processOutgoingMessage(unsigned char *pBuffer)
{
    tictacpacket thePacket;
    cout << "I am inside Function " << __func__ << endl;
}
/**
 * parsePacket()
 * This function will be called after reading the incoming raw bytes on the socket
 * Input : pointer to source buffer, that was filled from socket
 * Output : parsing result.
 */
long CPlayer::parsePacket(unsigned char *pSrc, unsigned long nLen)
{
    tictacpacket thePacket;
    cout << "I am inside Function " << __func__ << endl;

    if(thePacket.ParseFromArray(pSrc, nLen))
    {
	// If parse is successful, then proceed to process the packet
	processIncomingMessage(thePacket);
	return 0; // success
    }
    else
    {
        cout << "ERROR: thePacket.ParseFromArray Failed in Function " << __func__ << "Line = " << __LINE__ << endl;
	return -1; // Failure
    }
}
