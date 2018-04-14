namespace game_framework
{
	class CGameEnemy
	{
	public:
		CGameEnemy(int);
		void LoadBitmap();
		void OnMove(CGameMap *map);
		void OnShow(CGameMap *map);
		int Height();
		int Width();
		void SetXY(int, int);
		int GetX();
		int GetY();
		bool IsAlive();
		void SetIsAlive(bool);
		bool HitPlayer(CGamePlayer *player);
	private:
		CMovingBitmap enemy;
		int x, y; //���ڵ�ͼ�ϵĵ�����
		int movingSpeed;
		bool isAlive;
	};
}