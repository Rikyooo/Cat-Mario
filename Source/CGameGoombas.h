#pragma once
#include "CGameMinion.h"

namespace game_framework
{
	class CGameMap;
	class CGamePlayer;

	class CGameGoombas : public CGameMinion
	{
	private:
		// ----- MinionState, 0 = Alive, 1 = Dead, -1 = Destroy
	public:
		CGameGoombas(int iX, int iY, int iBlockID, bool moveDirection);
		~CGameGoombas(void);
		void Update(CGameMap* map);
		void Draw(CGameMap* map, int iBlockID);
		void collisionWithPlayer(bool TOP, CGameMap* map);
		void setMinionState(int minionState, CGameMap* map);
	};
}