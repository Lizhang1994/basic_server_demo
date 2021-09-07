#include <iostream>
#include "zinx.h"
#include "ZinxTCP.h"
#include "ZinxTimerDeliver.h"
#include "ZinxTimer.h"
#include "GameChannel.h"
#include "GameMsg.h"
#include "GameProtocol.h"
#include "GameRole.h"
#include "Msg.pb.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
using namespace pb;

class TimeOut : public TimerOutProc{
virtual void Proc() override{
auto players = ZinxKernel::Zinx_GetAllRole();
if(0 == players.size()){
    ZinxKernel::Zinx_Exit();  /* no player in game then exit  */
}
}
virtual int GetTimerSec() override{
return 60; /*  timer is 60s. */
}

};

void Daemon(){  /* daemon program  */
int ret = -1, status = 0;
pid_t pid = -1;
int fd = -1;

fd = open("./gamelog.log",O_RDWR|O_CREAT|O_APPEND,0664);
if( -1 == fd){
perror("open");
return;
}
dup2(fd,STDIN_FILENO);  /* monitor stdin and stdout  */
dup2(fd,STDOUT_FILENO);

close(fd);

pid = fork();
if(-1 == pid){
    perror("fork");
    return;
}else if(pid > 0){
    exit(0);
}

ret = setsid(); /* child process create a session  */
if(-1 == ret){
    perror("setsid");
    return;
}
while(1){  /* father process watching, child process execute  */
    pid = fork();
    if(-1 == pid){
        perror("while fork");
        break;
    }else if(pid > 0){
        wait(&status);
        if(WIFEXITED(status)){   /* normal exit  */
            if(0 == WEXITSTATUS(status)){
                exit(0);
            }
        }else{
            cout<<"fault exit!"<<endl;
            continue;
        }
    }else{
        break;
    }
}

}


int main(int argc,char* argv[]){
if((2 == argc)&&("daemon" == string(argv[1]))){
Daemon();
}

TimeOut tOut;
ZinxTimerDeliver::GetInstance().RegisterProcObject(tOut); /* register a timer  */
ZinxTimer *timer = new ZinxTimer;
ZinxTCPListen *tcpListen = new ZinxTCPListen(8888,new GameChannelFac); /* listen about gamechannel!!!  */

ZinxKernel::ZinxKernelInit(); /* kernel initiate  */
ZinxKernel::Zinx_Add_Channel(*timer);
ZinxKernel::Zinx_Add_Channel(*tcpListen); /* timer and tcplisten go up the tree  */
ZinxKernel::Zinx_Run();
ZinxKernel::ZinxKernelFini();

google::protobuf::ShutdownProtobufLibrary();

return 0;
}

