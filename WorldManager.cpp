#include "WorldManager.h"
#include "AOI_World.h"

WorldManager WorldManager::smManager;

WorldManager& WorldManager::GetInstance(){
return smManager;
}

AOI_World* WorldManager::GetWorld(int id){
return mVecWorld[id];
}

WorldManager::WorldManager(){
mVecWorld.reserve(3);
mVecWorld[0] = nullptr;

mVecWorld[1] = new AOI_World(0,400,0,400,10,20);
mVecWorld[1]->mWorldld = 1;

mVecWorld[2] = new AOI_World(0,140,0,140,1,1);
mVecWOrld[2]->mWorldld = 2;
}

WorldManager::~WorldManager(){
for(auto it : mVecWorld){
    if(nullptr != it){
        delete it;
    }
}

}
