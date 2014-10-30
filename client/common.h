/**********************************************************************
 * File Name: common.h
 *
 * Author:
 *
 * Description:
 *
 * History :
 *         1.
 *
 *         2.
 *
 *         3.
 *
 ***********************************************************************/
#ifndef COMMON_H
#define COMMON_H

#define SERVER_PORT 2000 
#define CLIENT_PORT 2001

typedef enum msgType
{
    KEEP_ALIVE,
    HELLO_MESSAGE,
    REGISTER_REQ,
    REGISTER_RESP,
    GET_SNAP_SHOT,
    SEND_MOVE,
    SEND_WIN,
    SEND_DRAW,
    START_GAME,
    END_GAME,
    TERMINATE_GAME,
}msgType;

enum MsgResponse
{
    RESPONSE_OK,
    RESPONSE_START,
    RESPONSE_RESUME,
    RESPONSE_ERROR
};

class registerMsg 
{
    /*
       self IP;
       player name;
       local clock;
     */
    public:
	registerMsg();
	~registerMsg();
	msgType sendRegisterMsgToServer(); 
	void processResponse();
};
#endif /* COMMON_H */
