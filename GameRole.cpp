#include "GameRole.h"
#include "GameProtocol.h"
#include "Msg.pb.h"
#include "GameMsg.h"
#include "WorldManager.h"
#include <random>

using namespace std;
using namespace pb;

int GameRole::smRoleCount = 1;
default_random_engine g_random_engine(time(nullptr));

GameRole::GameRole(){
this->mPlayerld = smRoleCount++;
this->mPlayerName = "Player_" + (char)smRoleCount;
x = 100 + g_random_engine() % 20;  /* random birthplace  */
y = 0;
z = 100 + g_random_engine() % 20;
v = 0;
hp = 1000;

}

GameRole::~GameRole(){}

bool GameRole::Init(){
auto *pMsg = MakeLogonSyncPid(); /* new client connect,send ID and name to client  */
ZinxKernel::Zinx_SendOut(*pMsg,*m_poProtocol); /* send message to protocol layer */

pMsg = MakeInitPosBroadcast(); /* send birth place message  to client*/
ZinxKernel::Zinx_SendOut(*pMsg, *m_poProtocol); /* send message to protocol layer  */

mCurrentWorld = WorldManager::GetInstance().GetWorld(1); /* get first world  */

AOI_World::GetWorld()->AddPlayer(this);
mCurrentWorld->AddPlayer(this);  /* add current player in this world  */

auto players = ZinxKernel::Zinx_GetAllRole(); /* send new player's position to all players  */
pMsg = MakeSurPlays();
ZinxKernel::Zinx_SendOut(*pMsg,*m_poProtocol); /* send all players' position to new player  */

players = mCurrentWorld->GetSurPlayers(this); /* send position to surrounding players  */
for(auto r : players){
    if(this == r){
        continue;
    }
    auto role = dynamic_cast<GameRole*>(r);
    pMsg = MakeInitPosBroadcast();
    ZinxKernel::Zinx_SendOut(*pMsg, *role->m_poProtocol);
}

return true;
}

UserData* GameRole::ProcMsg(UserData &_poUserData){
GameMsg &Msg = dynamic_cast<GameMsg&>(_poUserData);
for(auto single : Msg.m_GameMsgList){
    switch(single->m_MsgType){
        case GameSingleTLV::GAME_MSG_NEW_POSITION:  /* synchro player position  */
            {
                auto pbMsg = dynamic_cast<pb::Position*>(single->m_poGameMsg);
                this->ProcNewPosition(pbMsg->x(),pbMsg->y(),pbMsg->z(),pbMsg->v());
                break;
            }
        case GameSingleTLV::GAME_MSG_TALK_CONTENT:  /* talk message  */
            {
                auto pbMsg = dynamic_cast<pb::Talk*>(single->m_poGameMsg);
                this->ProcTalkContent(pbMsg->content());
                break;
            }
        case GameSingleTLV::GAME_MSG_CHANGE_WORLD: /* change world message  */
            {
                auto pbMsg = dynamic_cast<pb::ChangeWorldRequest*>(single->m_poGameMsg);
                this->ProcChangeWorld(pbMsg->srcld(),pbMsg->targetld());
                break;
            }
        default: 
            break;
    }
}
return nullptr;
}

void GameRole::Fini(){
auto players = ZinxKernel::Zinx_GetAllRole(); /* get all players  */
for(auto it : players){
    if(this == it){
        continue;
    }
    auto role = dynamic_cast<GameRole*>(it);
    auto pMsg = MakeLogoffSyncPid(); /* create logoff message  */
    ZinxKernel::Zinx_SendOut(*pMsg,*role->m_poProtocol); /* send message to all players  */
}

AOI_World::GetWorld()->DelPlayer(this);
}

GameMsg* GameRole::MakeLogonSyncPid(){ /* create logon message about name and ID  */
auto pSyncPid = new SyncPid;
pSyncPid->set_pid(this->mPlayerld);
pSyncPid->set_username(this->mPlayerName);
auto single = new GameSingleTLV(GameSingleTLV::GAME_MSG_LOGON_SYNCPID.pSyncPid); /* serialized objects  */
auto *retMsg = new GameMsg;
retMsg->m_GameMsgList.push_back(single);
return retMsg;
}

GameMsg* GameRole::MakeTalkBroadcast(std::string _talkContent){
auto pMsg = new BroadCast;
pMsg->set_pid(this->mPlayerld);
pMsg->set_username(this->mPlayerName);
pMsg->set_tp(1);
pMsg->set_content(_talkContent);

auto single = new GameSingleTLV(GameSingleTLV::GAME_MSG_BROADCAST,pMsg);
auto *retMsg = new GameMsg;
retMsg->m_GameMsgList.push_back(single);
return retMsg;
}

GameMsg* GameRole::MakeInitPosBroadcast(){  /* broadcast birthplace  */
auto pMsg = new BroadCast;
pMsg->set_pid(this->mPlayerld);
pMsg->set_username(this->mPlayerName);
pMsg->set_tp(2);
auto pos = pMsg->mutable_p();
pos->set_x(x);
pos->set_y(y);
pos->set_z(z);
pos->set_v(v);
pos->set_bloodvalue(hp);

auto single = new GameSingleTLV(GameSingleTLV::GAME_MSG_BROADCAST,pMsg);
auto *retMsg = new GameMsg;
retMsg->m_GameMsgList.push_back(single);

return retMsg;
}

GameMsg* GameRole::MakeNewPosBroadcast(){  /* broadcast new position  */
auto pMsg = new BroadCast;
pMsg->set_pid(this->mPlayerld);
pMsg->set_username(this->mPlayerName);
pMsg->set_tp(4);
auto pos = pMsg->mutable_p();
pos->set_x(this->x);
pos->set_y(this->y);
pos->set_z(this->z);
pos->set_v(this->v);
pos->set_bloodvalue(this->hp);

auto single = new GameSingleTLV(GameSingleTLV::GAME_MSG_BROADCAST,pMsg);
auto *retMsg = new GameMsg;
retMsg->m_GameMsgList.push_back(single);
return retMsg;
}

GameMsg* GameRole::MakeLogoffSyncPid(){  /* logoff message broadcast  */
auto pPid = new SyncPid;
pPid->set_pid(this->mPlayerld);
pPid->set_username(this->mPlayerName);

auto single = new GameSingleTLV(GameSingleTLV::GAME_LOGOFF_SYNCPID,pPid);
auto *retMsg = new GameMsg;
retMsg->m_GameMsgList.push_back(single);
return retMsg;
}

GameMsg* GameRole::MakeSurPlays(){  /* surrounding players broadcast  */
auto pPlayer = new SyncPlayers;
auto players = mCurrentWorld->GetSurPlayers(this);

for(auto it : players){
    if(it == this){
        continue;
    }
    auto role = dynamic_cast<GameRole*>(it);
    auto p = pPlayer->add_ps();
    p->set_pid(this->mPlayerld);
    p->set_username(this->mPlayerName);

    auto pos = p->mutable_p();
    pos->set_x(role->x);
    pos->set_y(role->y);
    pos->set_z(role->z);
    pos->set_v(role->v);
    pos->set_bloodvalue(role->hp);
}
auto single = new GameSingleTLV(GameSingleTLV::GAME_SUR_PLAYER,pPlayer);
auto *retMsg = new GameMsg;
retMsg->m_GameMsgList.push_back(single);
return retMsg;
}

GameMsg* GameRole::MakeChangeWorldResponse(int srcld, int targetld){  /* change world confirm message  */
auto pMsg = new ChangeWorldResponse;
pMsg->set_pid(this->mPlayerld);
pMsg->set_changeres(1);
pMsg->set_srcld(srcld);
pMsg->set_targetld(targetld);

auto pos = pMsg->mutable_p();
pos->set_x(this->x);
pos->set_y(this->y);
pos->set_z(this->z);
pos->set_v(this->v);
pos->set_bloodvalue(this->hp);

auto single = new GameSingleTLV(GameSingleTLV::GAME_MSG_CHANGE_WORLD_RESPONSE,pMsg);
auto *retMsg = new GameMsg;
retMsg->m_GameMsgList.push_back(single);
return retMsg;
}

void GameRole::ProcNewPosition(float _x, float _y, float _z, float _v){  /* handle move message  */
    if(mCurrentWorld->GridChanged(this,_x,_z)){  /* if grid changes  */
        auto oldList = mCurrentWorld->GetSurPlayers(this);
        mCurrentWorld->DelPlayer(this);
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->v = _v;

        auto newList = mCurrentWorld->GetSurPlayers(this);
        ViewDisappear(oldList,newList);
        ViewAppear(oldList,newList);
        mCurrentWorld->AddPlayer(this);
    }else{
        this->x = _x;
        this->y = _y;
        this->z = _z;
        this->v = _v; 
    }
auto players = mCurrentWorld->GetSurPlayers(this);
for(auto it : players){
    if(this == it){
        continue;
    }
    auto role = dynamic_cast<GameRole*>(it);
    auto pMsg = MakeNewPosBroadcast();
    ZinxKernel::Zinx_SendOut(*pMsg,*role->m_poProtocol);
}
}

void GameRole::ProcTalkContent(std::string content){
auto players = ZinxKernel::Zinx_GetAllRole();  /* send talk message to everyone  */
for(auto it : players){
    auto role = dynamic_cast<GameRole*>(it);
    auto pMsg = MakeTalkBroadcast(content);
    ZinxKernel::Zinx_SendOut(*pMsg,*role->m_poProtocol);
}
}

void GameRole::ProcChangeWorld(int srcld,int targetld){
mCurrentWorld->DelPlayer(this);
auto players = mCurrentWorld->GetSurPlayers(this);
for(auto it : players){
    if(it == this){
        continue;
    }
    auto role = dynamic_cast<GameRole*>(it);
    auto pMsg = MakeLogoffSyncPid();
    ZinxKernel::Zinx_SendOut(*pMsg,*role->m_poProtocol);
}
if(1 == targetld){
    x = 100 + g_random_engine() % 20;
    y = 0;
    z = 100 + g_random_engine() % 20;
    v = 0;
    hp = 1000;
}else if(2 == targetld){
    x = 100 + g_random_engine() % 20;
    y = 0;
    z = 100 + g_random_engine() % 20;
    v = 0;
    hp = 1000;
    
}
mCurrentWorld = WorldManager::GetInstance().GetWorld(targetld); /* new world online  */
mCurrentWorld->AddPlayer(this);

players = mCurrentWorld->GetSurPlayers(this); /* send new player's message to surrounding players  */
for(auto it : players){
    if(it == this){
        continue;
    }
    auto role = dynamic_cast<GameRole*>(it);
    auto pMsg = MakeInitPosBroadcast();
    ZinxKernel::Zinx_SendOut(*pMsg,*role->m_poProtocol);
}

auto pMsg = MakeChangeWorldResponse(srcld,targetld);
ZinxKernel::Zinx_SendOut(*pMsg,*m_poProtocol);
pMsg = MakeSurPlays();
ZinxKernel::Zinx_SendOut(*pMsg,*m_poProtocol);
}

void GameRole::ViewDisappear(std::list<AOI_Player*>& oldList,std::list<AOI_Player*>& newList){
vector<AOI_Player*> oldVec(oldList.begin(),oldList.end());
sort(oldVec.begin(),oldVec.end());
vector<AOI_Player*> newVec(newList.begin(),newList.end());
sort(newVec.begin(),newVec.end());
vector<AOI_Player*> dif;
std::set_difference(oldVec.begin(),oldVec.end(),newVec.begin(),newVec.end(),std::inserter(dif,dif.begin()));
for(auto it : dif){
    auto role = dynamic_cast<GameRole*>(it);
    auto pMsg = MakeLogoffSyncPid();
    ZinxKernel::Zinx_SendOut(*pMsg,*role->m_poProtocol); /* send logoff player message to other player  */
    pMsg = role->MakeLogoffSyncPid();
    ZinxKernel::Zinx_SendOut(*pMsg,*m_poProtocol); /* send logoff player message to oneself  */
}

}

void GameRole::ViewAppear(std::list<AOI_Player*>& oldList,std::list<AOI_Player*>& newList){
vector<AOI_Player*> oldVec(oldList.begin(),oldList.end());
sort(oldVec.begin(),oldVec.end());
vector<AOI_Player*> newVec(newList.begin(),newList.end());
sort(newVec.begin(),newVec.end());
vector<AOI_Player*> dif;
std::set_difference(oldVec.begin(),oldVec.end(),newVec.begin(),newVec.end(),std::inserter(dif,dif.begin()));
for(auto it : dif){
    auto role = dynamic_cast<GameRole*>(it);
    auto pMsg = MakeInitPosBroadcast();
    ZinxKernel::Zinx_SendOut(*pMsg,*role->m_poProtocol); /* send logon player message to other player  */
    pMsg = role->MakeInitPosBroadcast();
    ZinxKernel::Zinx_SendOut(*pMsg,*m_poProtocol); /* send logon player message to oneself  */
}
}

int GameRole::GetX(){
return this->x;
}

int GameRole::GetY(){
return this->z;
}
