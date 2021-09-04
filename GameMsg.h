#ifndef _GAMEMSG_H_
#define _GAMEMSG_H_

#include "zinx.h"
#include <list>
#include <google/protobuf/message.h>

class GameSingleTLV : public UserData{
    public:
        enum GameMsgType{
        GAME_MSG_LOGON_SYNCPID = 1, //player's ID and player's name
        GAME_MSG_TALK_CONTENT = 2,  //player's talk content
        GAME_MSG_NEW_POSTION = 3,   // player's new postion
        GAME_MSG_BROADCAST = 200,   // new player in, including ID,name,postion and so on
        GAME_MSG_LOGOFF_SYNCPID = 201, // take off one player, including ID and name
        GAME_MSG_SUR_PLAYER = 202,   // surrounding players postion
        }m_MsgType;
    google::protobuf::Message *m_poGameMsg = NULL;
    GameSingleTLV(GameMsgType _Type, google::protobuf::Message * _poGameMsg);
    GameSingleTLV(GameMsgType _Type, std::string _szInputData);
    ~GameSingleTLV();

    std::string Serialize(); //connect

};

class GameMsg : public UserData{
    public:
        std::list<GameSingleTLV *> m_GameMsgList;
        GameMsg();
        virtual ~GameMsg();

};

#endif
