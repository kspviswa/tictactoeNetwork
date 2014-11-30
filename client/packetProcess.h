#ifndef PACKETPROCESS_H_
#define PACKETPROCESS_H_

#include "tictac.h"

using namespace std;
using namespace tictac;

class CPlayer
{
    public:
        CPlayer();
        ~CPlayer();
        long parsePacket(unsigned char *pSrc, unsigned long nLen);
        void processOutgoingMessage(unsigned char *pBuffer);
	void processIncomingMessage(tictacpacket thePacket);
	int processOkMessage(tictacpacket *pPacket);
	int processStartMessage(tictacpacket *pPacket);
	int processResumeMessage(tictacpacket *pPacket);
	int processMoveMessage(tictacpacket *pPacket);
        int processEndMessage(tictacpacket *pPacket);
};
#endif
