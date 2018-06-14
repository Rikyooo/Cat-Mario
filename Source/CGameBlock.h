#pragma once

namespace game_framework
{
	class CGameBlock
	{
	public:
		CGameBlock();
		CGameBlock(int blockID, int bmpID, bool isCollision, bool isAlive, bool isUse, bool isVisible);
		~CGameBlock(void);

		void LoadBitmap();
		void OnDraw(int x, int y, bool isHorizontalFlip);

		int getBlockID();
		void setBlockID(int iID);

		bool IsCollision();
		bool IsAlive();
		bool IsUse();
		bool IsVisible();

	private:
		CMovingBitmap bmp;
		int bmpID;
		int blockID;
		bool isCollision;
		bool isAlive;
		bool isUse;
		bool isVisible;
	};
}