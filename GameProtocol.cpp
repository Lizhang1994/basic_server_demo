#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameRole.h"
#include "GameMsg.h"
using namespace std;

GameProtocol::GameProtocol(){
GameMsg *poMsg = nullptr;
std::string *retBuf = nullptr;
}
GameProtocol::~GameProtocol(){
if(poMsg != nullptr){
delete poMsg;
}
if(retBuf != nullptr){
delete retBuf;
}
}

/* return GameMsg after translate TCP data stream  */
UserData *GameProtocol::raw2request(std::string _szInput){ // based on Tag Length Value format
poMsg = new GameMsg();
m_szLastBuffer.append(_szInput); /* cacche the message received this time and combine with remaining message  */

while(m_szLastBuffer.size() >= 8){ /* if size >= minimum massage length, recycle  */
int ilength = m_szLastBuffer[0] | m_szLastBuffer[1] | m_szLastBuffer[2] | m_szLastBuffer[3]; // length field
int iType = m_szLastBuffer[4] | m_szLastBuffer[5] | m_szLastBuffer[6] | m_szLastBuffer[7]; // Type field

if(m_szLastBuffer.size() < (unsigned int)(8 + ilength)){break;} 
// if data field length less than specific length,break and wait next received data

string szSingleData = m_szLastBuffer.substr(8,ilength);/* cut out data for szSingleData object, put it into the list of returned object  */

GameSingleTLV *poSingleMsg = new GameSingleTLV((GameSingleTLV::GameMsgType)iType,szSingleData);
poMsg->m_GameMsgList.push_back(poSingleMsg);

m_szLastBuffer.erase(0,8 + ilength);/* remove handled data stream  */
}

return poMsg;

}
/* return the byte stream after message content encoding   */
std::string *GameProtocol::response2raw(UserData &_oUserData){
*poMsg = dynamic_cast<GameMsg *>(&_oUserData);
retBuf = new string;
for(auto it : poMsg->m_szLastBuffer){
    std::string buf = it->Serialize();
    int iType = it->m_MsgType; /* message type  */
    int ilength = buf.size();  /* message length  */
/* string push_back, ilength & 0xff means high level reset to 0 and set ilength. ilength>>8 means shift right 8 bits   */
    retBuf->push_back(ilength & 0xff);
    retBuf->push_back((ilength>>8) & 0xff);
    retBuf->push_back((ilength>>16) & 0xff);
    retBuf->push_back((ilength>>24) & 0xff);

    retBuf->push_back(iType & 0xff);
    retBuf->push_back((iType>>8) & 0xff);
    retBuf->push_back((iType>>16) & 0xff);
    retBuf->push_back((iType>>24) & 0xff);

    retBuf->append(buf);
}
return retBuf;
}

/* return bound poChannel  */
Ichannel *GameProtocol::GetMsgSender(BytesMsg &_oBytes){
return m_poChannel;
}
/* return bound poRole  */
Irole *GameProtocol::GetMsgProcessor(UserDataMsg &_oUserDataMsg){
return m_poRole;
}
