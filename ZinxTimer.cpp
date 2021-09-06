#include "ZinxTimer.h"
#include "ZinxTimerDeliver.h"
#include <iostream>
#include <stdint.h>
#include <sys/timerfd.h>

using namespace std;

ZinxTimer::ZinxTimer(){}
ZinxTimer::~ZinxTimer(){}

bool ZinxTimer::Init(){
int ret = -1;
/* In man 2 timer_settime. when timer expires,it_value = it_interval  */
struct itimerspec tmo = {(1,0),(1,0)};
mFd = timerfd_create(CLOCK_MONOTONIC,0); /* create a timer,notify via file descriptors  */
if(-1 == mFd){
    perror("timerfa_create");
    return false;
}
ret = timerfd_settime(mFd,0,&tmo,NULL); /* set timeout of timer  */
if(-1 == ret){
perror("timerfd_settime");
return false;
}
return true;

}

bool ZinxTimer::ReadFd(std::string &_Input){
uint64_t val;
int ret = -1;

ret = read(mFd,(char*)&val,sizeof(val));
if(ret != sizeof(val)){
perror("read");
return false;
}
_Input.append((char*)(&val),sizeof(val));
return true;
}

bool ZinxTimer::WriteFd(std::string &_Output){
return false;
}

void ZinxTimer::Fini(){
if(mFd = 0){
    close(mFd);
}
}

int ZinxTimer::GetFd(){
return mFd;
}

std::string ZinxTimer::GetChannelInfo(){
return std::string("timer");
}

AZinxHandler *ZinxTimer::GetInputNextStage(BytesMsg &_oInput){
return &ZinxTimerDeliver::GetInstance();
}

