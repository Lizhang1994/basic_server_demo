#include "AOI_World.h"

using namespace std;

AOI_Grid::AOI_Grid(int _gid):iGID(_gid){}
AOI_Grid::~AOI_Grid(){}

int AOI_World::Xwidth(){return maxX - minX;}
int AOI_World::Ywidth(){return maxY - minY;}

AOI_World::AOI_World(int _minx, int _maxx, int _miny, int _maxy, int _xcnt, int _ycnt){
this->minX = _minx;
this->maxX = _maxx;
this->minY = _miny;
this->maxY = _maxy;
this->Xcnt = _xcnt;
this->Ycnt = _ycnt;

this->m_Grids.reserve(_xcnt * _ycnt); /* reserve space for grids   */
for(int i = 0; i< _xcnt * _ycnt;i++){
    this->m_Grids.push_back(new AOI_Grid(i));
}
}

AOI_World::~AOI_World(){
for(auto it : m_Grids){
    delete it;
}
}

std::list<AOI_Player*> AOI_World::GetSurPlayers(AOI_Player* _player){
int row = 0, col = 0, index = 0;
std::list<AOI_Player*>retPlayers; /* list for surrounding players  */
row = (_player->GetY() - this->minY) / ((this->maxY - this->minY) / this->Ycnt);/*  row number and column number  */
col = (_player->GetX() - this->minX) / ((this->maxX - this->minX) / this->Xcnt);

pair<int,int>row_col = {
    make_pair(row - 1,col - 1),
    make_pair(row - 1,col),
    make_pair(row - 1,col + 1),
    
    make_pair(row,col - 1),
    make_pair(row,col),
    make_pair(row,col + 1),
    
    make_pair(row + 1,col - 1),
    make_pair(row + 1,col),
    make_pair(row + 1,col + 1),
};

for(auto it : row_col){
    if(it.first < 0 || it.first >= this->Ycnt){
    continue;  /* Is row legal  */
    }

    if(it.sencond < 0 || it.second >= this->Xcnt){
    continue;  /* Is column legal  */
    }

    index = it.first * this->Xcnt + it.second;  /* count the grid number  */

    for(auto pit : m_grids[index]->m_player){
        retPlayers.push_back(pit);
    }

}
return retPlayers;
}

void AOI_World::AddPlayer(AOI_Player *_player){
int index = 0;
index = Calculate_Grid_Index(_player->GetX(), _player->GetY());

if(index < 0 || index >= this->Xcnt * this->Ycnt){
return; /* if grid is illegal,return  */
}
m_grids[index]->m_player.push_back(_player);
}

void AOI_World::DelPlayer(AOI_Player *_player){
int index = 0;
index = Calculate_Grid_Index(_player->GetX(), _player->GetY());

if(index < 0 || index >= this->Xcnt * this->Ycnt){
return; /* if grid is illegal,return  */
}
m_grids[index]->m_player.remove(_player);
}

bool AOI_World::GridChanged(AOI_Player *_player, int _newX,int _newY){
int oldindex = 0, newindex = 0;
oldindex = Calculate_Grid_Index(_player->GetX(), _player->GetY());
newindex = Calculate_Grid_Index(_newX, _newY);
return oldindex != newindex;
}

int AOI_World::Calculate_Grid_Index(int x, int y){
int row = 0, col = 0, index = 0;

row = (y - this->minY) / ((this->maxY - this->minY) / this->Ycnt);
col = (x - this->minX) / ((this->maxX - this->minX) / this->Xcnt);

if(row < 0 || row >= this->Ycnt || col < 0 || col >= this->Xcnt){
return -1;
}

index = row * this->Xcnt + col;
return index;
}

