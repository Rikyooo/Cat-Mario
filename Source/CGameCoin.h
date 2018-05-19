#pragma once

namespace game_framework
{
	class CGameMap;

	class CGameCoin
	{
	public:
		CGameCoin(int iXPos, int iYPos);
		~CGameCoin(void);

		void Update();
		void Draw(CGameMap* map);

		int getXPos();
		int getYPos();
		bool getDelete();

	private:
		int iXPos, iYPos;
		int iLEFT;

		int iStepID;

		bool bTOP;

		bool bDelete;
	};
}
