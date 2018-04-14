#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	CGamePlayer::CGamePlayer()
	{
	}

	void CGamePlayer::LoadBitmap()
	{
		player.LoadBitmap(IDB_PLAYER_1, RGB(160, 180, 250));
	}

	void CGamePlayer::OnMove(CGameMap* map)
	{
		const int STEP_SIZE = 11;

		if (isMovingUp)
			if ((map->IsEmpty(x, y - 1) && map->IsEmpty(x + Width(), y - 1)) && jumpingCount < 6)
			{
				jumpingCount++;
				y -= 33;
			}
			else if (!map->IsEmpty(x, y + 1) || !map->IsEmpty(x + Width(), y + 1))//防上方陷入
			{
				jumpingCount = 6;
				y += STEP_SIZE;
			}
		/*if (jumpingcount < 10)
		{
		jumpingcount++;
		y -= 30 / jumpingcount;
		}
		else
		{
		jumpingcount = 0;
		}*/
		if ((map->IsEmpty(x, y + Height() + 1) && map->IsEmpty(x + Width(), y + Height() + 1)) || y + Height() >= SIZE_Y)
		{
			y += STEP_SIZE;
			isFalling = true;
		}
		else if (!map->IsEmpty(x, y + Height() - 1) || !map->IsEmpty(x + Width(), y + Height() - 1))  //防下方陷入
			y = (y + Height()) / BLOCK_WIDTH_HEIGHT * BLOCK_WIDTH_HEIGHT - Height();
		else
		{
			jumpingCount = 0;
			isMovingUp = false;
			isFalling = false;
		}

		if (isMovingLeft)
			if (x - 1 > map->GetScreenX())
				if (map->IsEmpty(x - 1, y) && map->IsEmpty(x - 1, y + Height() - 1))
					x -= STEP_SIZE;

		if (isMovingRight)
			if (x + 1 < BLOCK_WIDTH_HEIGHT * 53)
				if (map->IsEmpty(x + Width() + 1, y) && map->IsEmpty(x + Width() + 1, y + Height() - 1))
				{
					x += STEP_SIZE;

					if ((x - map->GetScreenX() > SIZE_X * 2 / 3) && (map->GetScreenX() + SIZE_X < BLOCK_WIDTH_HEIGHT * 54 - STEP_SIZE))
						map->SetScreenX(STEP_SIZE);
				}
	}

	void CGamePlayer::OnShow(CGameMap* map)
	{
		player.SetTopLeft(x - map->GetScreenX(), y - map->GetScreenY());
		player.ShowBitmap();
	}

	int CGamePlayer::Width()
	{
		return player.Width();
	}

	int CGamePlayer::Height()
	{
		return player.Height();
	}

	void CGamePlayer::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CGamePlayer::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CGamePlayer::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CGamePlayer::SetXY(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	int CGamePlayer::GetX()
	{
		return x;
	}
	int CGamePlayer::GetY()
	{
		return y;
	}

	void CGamePlayer::SetJump(bool state)
	{
		isJumping = state;
	}

	bool CGamePlayer::GetJump()
	{
		return !isFalling;
	}
}