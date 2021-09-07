#include "GameChannel.h"
#include "GameRole.h"
#include "GameProtocol.h"
#include "GameMsg.h"

GameChannel::GameChannel(int _fd) : ZinxTcpData(_fd){}

GameChannel::~GameChannel(){  /*Destruction while delete related Role and Protocol   */
    ZinxKernel::Zinx_Del_Role(*m_poRole);
    ZinxKernel::Zinx_Del_Proto(*m_poProtocol);
    delete m_poRole;
    delete m_poProtocol;
}

AZinxHandler *GameChannel::GetInputNextStage(BytesMsg &_oInput){ /* return protocol object  */
return m_poBoundProto;
}

ZinxTcpData *GameChannel::CreateTcpDataChannel(int _fd){  /* create TcpData type object,create protocol object and role object and bound with pochannel,add protocol object and role into Kernel */
auto poChannel = new GameChannel(_fd);
auto poProtocol = new GameProtocol();
auto poRole = new GameRole();

poChannel->m_poProtocol = poProtocol;
poChannel->m_poRole = poRole;

poProtocol->m_poChannel = poChannel;
poProtocol->m_poRole = poRole;

poRole->m_poChannel = poChannel;
poRole->m_poProtocol = poProtocol;

ZinxKernel::Zinx_Add_Proto(*poProtocol);
ZinxKernel::Zinx_Add_Role(*poRole);

return poChannel;
}
