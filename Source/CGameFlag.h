#pragma once

namespace game_framework
{
	class CGameMap;

	class CGameFlag
	{
	public:
		CGameFlag(int iXPos, int iYPos, int height);
		~CGameFlag(void);
		void Update();
		void Draw(CGameMap * map);

	private:
		int iXPos, iYPos;
		int height;
		int iBlockID;
	};
}