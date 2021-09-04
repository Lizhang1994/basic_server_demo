#ifndef _GAMECHANNEL_H_
#define _GAMECHANNEL_H_

#include "ZinxTCP.h"

class GameRole;
class GameProtocol;

class GameChannelFac : public IZinxTcpConnFact{
public:
virtual ZinxTcpData *CreateTcpDataChannel(int _fd) override;

};

class GameChannel : public ZinxTcpData{
public:
GameChannel(int _fd);	
virtual ~GameChannel();

virtual AZinxHandler *GetInPutNextStage(BytesMsg &_oInput) override;
GameProtocol *m_poBoundProto = NULL;
GameRole *m_poBoundRole = NULL;
};

#endif
