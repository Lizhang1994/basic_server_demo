#include "ZinxTimerDeliver.h"
#include <iostream>

using namespace std;
#define ZINX_TIMER_WHEEL_SIZE 8

ZinxTimerDeliver ZinxTimerDeliver::m_single;

ZinxTimerDeliver::ZinxTimerDeliver(){
m_TimerWheel.resize(ZINX_TIMER_WHEEL_SIZE);

}

ZinxTimerDeliver::~ZinxTimerDeliver(){}

bool ZinxTimerDeliver::RegisterProcObject(TimerOutProc &_Proc){
int tmo = 0; /* timeout  */
int index = 0;
int count = 0;
WheelNode node;

tmo = _Proc.GetTimerSec();
if(tmo <= 0){
return false;
}

index = (m_Cur_Index + tmo) % m_TimerWheel.size(); /* calculate time wheel index  */

count = tmo / m_TimerWheel.size(); /* count the turns  */

node.LastCount = count;
node.pProc = &_Proc;
m_TimerWheel[index].push_back(node);

return true;
}

void ZinxTimerDeliver::UnRegisterProcObject(TimerOutProc &_Proc){
for(auto &it : m_TimerWheel){
    for(auto itt = it.begin(); itt != it.end();){
        if(itt->pProc == &_Proc){
            it.erase(itt);
        }else{
            itt++;
        }
    }
}
}

IZinxMsg *ZinxTimerDeliver::InternalHandle(IZinxMsg &_oInput){
uint64_t tmo = 0;
list<WheelNode> registerList;

BytesMsg *pMsg = dynamic_cast<BytesMsg*>(&_oInput);
pMsg->szData.copy((char*)&tmo,sizeof(tmo));
for(uint64_t i = 0; i < tmo; i++){
    m_Cur_Index = (m_Cur_Index + 1) % m_TimerWheel.size();
    for(auto it = m_TimerWheel[m_Cur_Index].begin(); it != m_TimerWheel[m_Cur_Index].end(); ){
        it->LastCount--;
        if(it->LastCount < 0){
            it->pProc>Proc(); /* call timeout handle function  */
            registerList.push_back(*it);  /* save task  */
            it = m_TimerWheel[m_Cur_Index].erase(it);
        }else{
            it++;
        }
    }

    for(auto it : registerList){
        RegisterProcObject(*(it.pProc));
    }

}
return nullptr;
}

AZinxHandler *ZinxTimerDeliver::GetNextHandler(IZinxMsg &_oNextMsg){
return nullptr;
}


