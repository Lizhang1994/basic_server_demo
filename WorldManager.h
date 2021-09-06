#ifndef _WORLDMANAGER_H_
#define _WORLDMANAGER_H_

#include "AOI_World.h"
#include <vector>

class WorldManager{
    private:
        WorldManager();
        ~WorldManager();
        static WorldManger smManager;
        std::vector<AOI_World*> mVecWorld;
    public:
        static WorldManager &GetInstance();
        AOI_World* GetWorld(int id);

};

#endif
