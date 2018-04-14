namespace game_framework
{
	class CGamePlayer
	{
	public:
		CGamePlayer();
		void LoadBitmap();
		void OnMove(CGameMap *map);
		void OnShow(CGameMap *map);
		int Width();
		int Height();
		void SetMovingLeft(bool);
		void SetMovingRight(bool);
		void SetMovingUp(bool);
		void SetXY(int, int);
		int GetX();
		int GetY();
		void SetJump(bool);
		bool GetJump();
	private:
		CMovingBitmap player;
		int x, y;   //玩家在地图上的点坐标
		bool isMovingLeft = false;
		bool isMovingRight = false;
		bool isMovingUp = false;
		int jumpingCount = 0;
		//int fallSpeed = 0;
		bool isFalling = false;
		bool isJumping = true;
	};
}