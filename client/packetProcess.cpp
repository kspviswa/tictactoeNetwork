
/*
 * components.cpp
 *
 *  Created on: 23-Nov-2014
 *      Author: Venkat 
 */

#include <netinet/in.h>
#include <arpa/inet.h>
#include "components.h"
#include "tictac.h"


Class CPlayer
{
    public:
	parsePacket(unsigned char *pSrc, unsigned long nLen);

};


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
	case tictacpacket:MOVE:
            processEndMessage(&thePacket);
	    break;
	default:
	  cout << "Not processed" << endl;
    }
}

int CPlayer::processOkMessage(tictacpacket *pPacket)
{
    return 0;
}
int CPlayer::processStartMessage(tictacpacket *pPacket)
{
    return 0;
}
int CPlayer::processResumeMessage(tictacpacket *pPacket)
{
    return 0;
}
int CPlayer::processMoveMessage(tictacpacket *pPacket)
{
    return 0;
}

void CPlayer::processOutgoingMessage(unsigned char *pBuffer)
{
    tictacpacket thePacket
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
    if(thePacket.ParseFromArray(pSrc, nLen))
    {
	// If parse is successful, then proceed to process the packet
	processIncomingMessage(thePacket);
	return 0; // success
    }
    else
	return -1; // Failure
}
