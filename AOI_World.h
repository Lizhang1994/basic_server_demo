#ifndef _AOIWORLD_H_
#define _AOIWORLD_H_

#include <list>
#include <vector>
#include <map>

class AOI_Player{
    public:
        virtual int GetX() = 0;
        virtual int GetY() = 0;
};

class AOI_Grid{
    public:
        AOI_Grid(int _gid);
        ~AOI_Grid();
        std::list <AOI_Player *> m_players; /* save players belong to this grid  */
        int iGID = 0;
};

class AOI_World{
    private:
        int Xwidth();
        int Ywidth();
    public:
        int m_Worldld = 1;
        int minX = 0, maxX = 0, minY = 0, maxY = 0, Xcnt = 0, Ycnt = 0;
        std::vector<AOI_Grid *> m_Grids; /* save grids in one world  */
        std::list<AOI_Player *> GetSurPlayers(AOI_Player *_player); /* save surrounding players  */
        AOI_World(int _minx, int _maxx,int _miny, int _maxy, int _xcnt, int _ycnt);
        void AddPlayer(AOI_Player *_player);
        void DelPlayer(AOI_Player *_player);
        
        int Calculate_Grid_Index(int x,int y);

        bool GridChanged(AOI_Player *_player, int _newX, int _newY); /* judge grid changes  */
//        AOI_World* GetWorld();
        virtual ~AOI_World();

};

#endif
