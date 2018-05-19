#pragma once

namespace game_framework
{
	class CGameMapLevel
	{
	private:
		int iBlockID;

		bool spawnMushroom;
		bool powerUP; // -- true = powerUP, false = 1UP
		bool spawnStar;
		int iNumOfUse;

		// ----- Animation -----

		bool blockAnimation;
		int iYPos;
		bool bYDirection;	// ----- true TOP, false BOTTOM

							// ----- Animation -----

	public:
		CGameMapLevel(void);
		CGameMapLevel(int iBlockID);
		~CGameMapLevel(void);

		void startBlockAnimation();

		int updateYPos();

		/* ----- get & set ----- */

		int getBlockID();
		void setBlockID(int iBlockID);

		int getYPos();

		int getNumOfUse();
		void setNumOfUse(int iNumOfUse);

		bool getSpawnMushroom();
		void setSpawnMushroom(bool spawnMushroom);
		bool getPowerUP();
		void setPowerUP(bool powerUP);
		bool getSpawnStar();
		void setSpawnStar(bool spawnStar);
	};
}