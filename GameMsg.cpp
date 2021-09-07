#include "GameChannel.h"
#include "GameRole.h"
#include "GameProtocol.h"
#include "GameMsg.h"
#include "Msg.pb.h"

using namespace std;

GameMsg::GameMsg(){}

GameMsg::~GameMsg(){
for(auto single : m_GameMsgList){
delete single;
}
}

GameSingleTLV::GameSingleTLV(GameMsgType _Type, google::protobuf::Message * _poGameMsg){
m_MsgType = _Type;
m_poGameMsg = _poGameMsg;
}

GameSingleTLV::GameSingleTLV(GameMsgType _Type, std::string _szInputData){
m_MsgType = _Type;
switch (_Type){
    case GAME_MSG_LOGON_SYNCPID:
        m_poGameMsg = new pb::SyncPid();
        break;
    case GAME_MSG_TALK_CONTENT:
        m_poGameMsg = new pb::Talk();
        break;
    case GAME_MSG_NEW_POSITION:
        m_poGameMsg = new pb::Position();
        break;
    case GAME_MSG_BROADCAST:
        m_poGameMsg = new pb::BroadCast();
        break;
    case GAME_MSG_LOGOFF_SYNCPID:
        m_poGameMsg = new pb::SyncPid();
        break;
    case GAME_MSG_SUR_PLAYER:
        m_poGameMsg = new pb::SyncPlayers();
        break;
    case GAME_MSG_CHANGE_WORLD:
        m_poGameMsg = new pb::ChangeWorldRequest();
        break;
    default:
        break;
}
if(NULL != m_poGameMsg){
m_poGameMsg->ParseFromString(_szInputData);
}
}

GameSingleTLV::~GameSingleTLV(){
if(NULL != m_poGameMsg){
delete m_poGameMsg;
}
}

std::string GameSingleTLV::Serialize(){
string szRet;
m_poGameMsg->SerializeToString(&szRet);
return szRet;
}

