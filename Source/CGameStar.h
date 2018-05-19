#pragma once

#include "CGameMinion.h"

namespace game_framework
{
class CGameMap;

class CGameStar : public CGameMinion
{
    public:
        CGameStar(int iXPos, int iYPos, int iX, int iY);
        ~CGameStar(void);

        void Update(CGameMap* map);
        bool updateMinion(CGameMap* map);

        void minionPhysics(CGameMap * map);

        void Draw(CGameMap* map, int iBlockID);

        void collisionWithPlayer(bool TOP, CGameMap* map);

        void setMinionState(int minionState, CGameMap* map);
    private:
        bool inSpawnState;
        int inSpawnY;
        int iX, iY; // inSpawnState draw Block
};
}