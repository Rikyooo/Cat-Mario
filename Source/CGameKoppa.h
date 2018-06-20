#pragma once
#include "CGameMinion.h"

namespace game_framework
{
	class CGameKoppa : public CGameMinion
	{
	public:
		CGameKoppa(int iX, int iY, int minionState, bool moveDirection, int iBlockID);
		~CGameKoppa();
		void Update(CGameMap* map);
		void Draw(CGameMap* map, int iBlockID);
		void minionPhysics(CGameMap* map);
		void updateXPos(CGameMap* map);
		void collisionWithPlayer(bool TOP, CGameMap* map);
		void collisionEffect();
		void setMinionState(int minionState);
		void setMinion();
	private:
		// ----- MinionState, 0 & 3 = Alive with wings, 1 = Alive, 2 = Dead, 4 = Dead animation, -1 = Destroy
		int iDistance;
		bool flyDirection; // -- minionState 3
	};
}