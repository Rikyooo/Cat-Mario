#pragma once

#include "CGameMinion.h"

namespace game_framework
{
	class CGameMap;

	class CGameMushroom : public CGameMinion
	{
	private:
		bool inSpawnState;
		int inSpawnY;
		bool powerUP;
		int iX, iY; // inSpawnState draw Block

	public:
		CGameMushroom(int iXPos, int iYPos, bool powerUP, int iX, int iY);
		~CGameMushroom(void);

		void Update(CGameMap* map);
		bool updateMinion(CGameMap* map);

		void Draw(CGameMap* map, int iBlockID);

		void collisionWithPlayer(bool TOP, CGameMap* map);

		void setMinionState(int minionState, CGameMap* map);
	};
}