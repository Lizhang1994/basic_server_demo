#ifndef _GAMEPROTOCOL_H
#define _GAMEPROTOCOL_H

#include "zinx.h"

class GameChannel;
class GameRole;

class GameProtocol : public Iprotocol{
   private:
       std::string m_szLastBuffer;
   public:
        GameProtocol();
        virtual ~GameProtocol();

        virtual UserData *raw2request(std::string _szInput) override;
        virtual std::string *response2raw(UserData & _oUserData) override;
        virtual Irole * GetMsgProcessor(UserDataMsg &_oUserDataMsg) override;
        virtual Ichannel * GetMsgSender(BytesMsg &_oBytes) override;
        GameChannel *m_poChannel = NULL;
        GameRole *m_poRole = NULL;

};

#endif
