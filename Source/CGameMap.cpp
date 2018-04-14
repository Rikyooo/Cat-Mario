#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"

namespace game_framework
{
	CGameMap::CGameMap() :screen_x(0), screen_y(0)
	{
		//1~17
		int map_init1[15][17] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 4, 3, 4, 3, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }
		};

		//18~34
		int map_init2[15][17] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }
		};

		//35~51
		int map_init3[15][17] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 4, 4, 4, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2 }
		};

		int map_init4[15][3] = { { 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 4, 4, 4 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 1, 1, 1 },
		{ 2, 2, 2 }
		};

		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 17; j++)
				map_index[i][j] = map_init1[i][j];

		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 17; j++)
				map_index[i][17 + j] = map_init2[i][j];

		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 17; j++)
				map_index[i][17 * 2 + j] = map_init3[i][j];

		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 3; j++)
				map_index[i][17 * 3 + j] = map_init4[i][j];
	}

	void CGameMap::LoadBitmap()
	{
		block_floor_1_ground.LoadBitmap(IDB_BLOCK_FLOOR_1_GROUND);
		block_floor_2_ground.LoadBitmap(IDB_BLOCK_FLOOR_2_GROUND);
		block_fragile_ground.LoadBitmap(IDB_BLOCK_FRAGILE_GROUND);
		block_sturdy_ground.LoadBitmap(IDB_BLOCK_STURDY_GROUND);
		block_item_1_common.LoadBitmap(IDB_BLOCK_ITEM_1_COMMON);
		block_item_2_ground.LoadBitmap(IDB_BLOCK_ITEM_2_GROUND);
		mountain.LoadBitmap(IDB_MOUNTAIN, RGB(160, 180, 250));
		cloud_1.LoadBitmap(IDB_CLOUD_1, RGB(160, 180, 250));
		brush.LoadBitmap(IDB_BRUSH, RGB(160, 180, 250));
	}

	void CGameMap::OnShow()
	{
		for (int i = 0; i < BLOCK_COLUMN; i++)
		{
			for (int j = 0; j < BLOCK_ROW; j++)
			{
				int x = BLOCK_WIDTH_HEIGHT * i - screen_x;
				int y = BLOCK_WIDTH_HEIGHT * j - screen_y;

				switch (map_index[j][i])
				{
				case 0:
					break;

				case 1:
					block_floor_1_ground.SetTopLeft(x, y);
					block_floor_1_ground.ShowBitmap();
					break;

				case 2:
					block_floor_2_ground.SetTopLeft(x, y);
					block_floor_2_ground.ShowBitmap();
					break;

				case 3:
					block_item_1_common.SetTopLeft(x, y);
					block_item_1_common.ShowBitmap();
					break;

				case 4:
					block_fragile_ground.SetTopLeft(x, y);
					block_fragile_ground.ShowBitmap();
					break;

				default:
					ASSERT(0);
				}
			}
		}

		mountain.SetTopLeft(BLOCK_WIDTH_HEIGHT - screen_x, SIZE_Y - 33 * 2 - 74 - screen_y);
		mountain.ShowBitmap();
		cloud_1.SetTopLeft(6 * BLOCK_WIDTH_HEIGHT - screen_x, 2 * BLOCK_WIDTH_HEIGHT - screen_y);
		cloud_1.ShowBitmap();
		cloud_1.SetTopLeft(22 * BLOCK_WIDTH_HEIGHT - screen_x, 2 * BLOCK_WIDTH_HEIGHT - screen_y);
		cloud_1.ShowBitmap();
		brush.SetTopLeft(19 * BLOCK_WIDTH_HEIGHT - screen_x, SIZE_Y - 33 * 2 - 28 - screen_y);
		brush.ShowBitmap();
		brush.SetTopLeft(26 * BLOCK_WIDTH_HEIGHT - screen_x, SIZE_Y - 33 * 2 - 28 - screen_y);
		brush.ShowBitmap();
	}

	void CGameMap::OnDraw()
	{
		// not the same class , change to CGameState
	}

	bool CGameMap::IsEmpty(int x, int y)
	{
		int block_x = x / BLOCK_WIDTH_HEIGHT;
		int block_y = y / BLOCK_WIDTH_HEIGHT;
		return map_index[block_y][block_x] == 0;
	}

	void CGameMap::SetScreenX(int step)
	{
		if (step == 0)
			screen_x = 0;
		else
			screen_x += step;
	}

	int CGameMap::GetScreenX()
	{
		return screen_x;
	}

	int CGameMap::GetScreenY()
	{
		return screen_y;
	}
}