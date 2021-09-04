#ifndef _GAMEROLE_H_
#define _GAMEROLE_H_

#include "zinx.h"
#include "AOI_World.h"
#include "GameMsg.h"

class GameChannel;
class GameProtocol;

class GameRole : public Irole, public AOI_Player{
    private:
        int pid = 0;
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
        GameChannel *m_BoundChannel = NULL;
        GameProtocol *m_BoundProtocol = NULL;

        virtual int GetX() override;  /* inherit from AOI_Player*/
        virtual int GetY() override;

        GameMsg *MakeLogonSyncPid(); /*  create player's ID and player's name when online */
        GameMsg *MakeTalkBroadcast(std::string _talkContent); /* create talk broadcast  */
        GameMsg *MakeInitPosBroadcast(); /* create birth postion broadcast  */
        GameMsg *MakeNewPosBroadcast();  /* create new postion broadcast when player moved  */
        GameMsg *MakeLogoffSyncPid(); /* create player's ID and player's name when offline  */
        GameMsg *MakeSurPlays();  /* create surrounding players postion  */

    void ViewLost(std::list<AOI_Player *> &newsurlist, std::list<AOI_Player *> &oldsurlist); /* handle view lost  */
    void ViewAppear(std::list<AOI_Player *> &newsurlist, std::list<AOI_Player *> &oldsurlist); /* hanle view appear  */
    void ProcNewPos(float _x,float _y,float _z,float _v);
    void ProcTalkMsg(std::string szContent);

};

#endif
