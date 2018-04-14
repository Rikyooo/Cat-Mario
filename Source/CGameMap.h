namespace game_framework
{
	//map class
	class CGameMap
	{
	public:
		CGameMap();
		void LoadBitmap();
		void OnShow();
		void OnDraw();
		bool IsEmpty(int, int);
		void SetScreenX(int);
		int GetScreenX();
		int GetScreenY();

	protected:
		static const int BLOCK_ROW = 15;
		static const int BLOCK_COLUMN = 54;
		CMovingBitmap block_floor_1_ground,
			block_floor_2_ground,
			block_fragile_ground,
			block_sturdy_ground,
			block_item_1_common,
			block_item_2_ground,
			mountain,
			cloud_1,
			brush;
		int map_index[BLOCK_ROW][BLOCK_COLUMN];
		int screen_x, screen_y;   //屏幕左上角在地图上的点坐标
	};
}