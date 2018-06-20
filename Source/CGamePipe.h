#pragma once
namespace game_framework
{
	class CGameMap;
	class CGamePlayer;

	class CGamePipe
	{
	private:
		// ----- 0 = VERTICAL, 1 = HORIZONTAL -> VERTICAL, 2 = VERTICAL -> VERTICAL
		int iType;
		// ----- X, Y LEFT Block Position
		int iLX, iLY;
		// ----- X, Y RIGHT Block Position
		int iRX, iRY;
		int newPlayerPosX, newPlayerPosY;
		// ----- MAP LVL ID
		int newCurrentLevel;
		int newLevelType;
		bool newMoveMap;
		bool newUnderWater;

		int iDelay;

		int iSpeed;
	public:
		CGamePipe(int iType, int iLX, int iLY, int iRX, int iRY, int newPlayerPosX, int newPlayerPosY, int newCurrentLevel, int newLevelType, bool newMoveMap, int iDelay, int iSpeed, bool underWater);
		~CGamePipe(void);

		void checkUse(CGameMap * map);
		void setEvent();
	};
}
