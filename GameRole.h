#ifndef _GAMEROLE_H_
#define _GAMEROLE_H_

#include "zinx.h"
#include "AOI_World.h"
#include "GameMsg.h"

class GameChannel;
class GameProtocol;

class GameRole : public Irole, public AOI_Player{
    private:
        int mPlayerld = 0;
        std::string mPlayerName;
        int hp;
        float x;
        float y;
        float z;
        float v; /* x,y,z represents player's postion,v means orientation  */
    public:
        GameRole();
        virtual ~GameRole();
        static int smRoleCount;
        AOI_World *mCurrentWorld = nullptr;
        virtual bool Init() override; /* inherit from Irole   */
        virtual UserData *ProcMsg(UserData &_poUserData) override;
        virtual void Fini() override;
        GameChannel *m_poChannel = NULL;
        GameProtocol *m_poProtocol = NULL;

        virtual int GetX() override;  /* inherit from AOI_Player*/
        virtual int GetY() override;

        GameMsg *MakeLogonSyncPid(); /*  create player's ID and player's name when online */
        GameMsg *MakeTalkBroadcast(std::string _talkContent); /* create talk broadcast  */
        GameMsg *MakeInitPosBroadcast(); /* create birth postion broadcast  */
        GameMsg *MakeNewPosBroadcast();  /* create new postion broadcast when player moved  */
        GameMsg *MakeLogoffSyncPid(); /* create player's ID and player's name when offline  */
        GameMsg *MakeSurPlays();  /* create surrounding players postion  */
        GameMsg *MakeChangeWorldResponse(int srcld, int targetld);

        void ViewDisappear(std::list<AOI_Player *> &oldList, std::list<AOI_Player *> &newList); /* handle view lost  */
        void ViewAppear(std::list<AOI_Player *> &oldList, std::list<AOI_Player *> &newList); /* handle view appear  */

        void ProcNewPosition(float _x,float _y,float _z,float _v);
        void ProcTalkContent(std::string Content);
        void ProcChangeWorld(int srcld, int targetld);

};

#endif
