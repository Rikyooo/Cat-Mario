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
		player.AddBitmap(IDB_PLAYER_1, RGB(160, 180, 250));
		player.AddBitmap(IDB_PLAYER_2, RGB(160, 180, 250));
		player.AddBitmap(IDB_PLAYER_3, RGB(160, 180, 250));
		player.AddBitmap(IDB_PLAYER_4, RGB(160, 180, 250));
	}

	void CGamePlayer::OnMove(CGameMap* map)
	{
		const int STEP_SIZE = 11;

		if (isMovingUp)
			if ((map->IsEmpty(x, y - 1) && map->IsEmpty(x + Width(), y - 1)) && jumpingCount < 6)
			{
				jumpingCount++;
				//player.SetCurrentBitmap(2);   //jumping style = 2
				y -= 33;

				if (!map->IsEmpty(x, y + 1) || !map->IsEmpty(x + Width(), y + 1))//防上方陷入
				{
					jumpingCount = 6;
					y = (y / BLOCK_WIDTH_HEIGHT + 1) * BLOCK_WIDTH_HEIGHT;
				}
			}

		if ((map->IsEmpty(x, y + Height() + 1) && map->IsEmpty(x + Width(), y + Height() + 1)) || y + Height() >= SIZE_Y)
		{
			isFalling = true;
			y += 6;
		}
		else
		{
			jumpingCount = 0;
			isMovingUp = false;
			isFalling = false;

			/*if (!isMovingLeft && !isMovingRight)
				player.SetCurrentBitmap(0);*/
		}
		if (!(y + Height() >= SIZE_Y))
		{
			if (!map->IsEmpty(x, y + Height() - 1) || !map->IsEmpty(x + Width(), y + Height() - 1))  //防下方陷入
				y = (y + Height()) / BLOCK_WIDTH_HEIGHT * BLOCK_WIDTH_HEIGHT - Height();
		}

		if (isMovingLeft)
		{
			if (!isMovingUp && !isFalling)
				player.OnMove();

			if (x - 1 > map->GetScreenX())
				if (map->IsEmpty(x - 1, y) && map->IsEmpty(x - 1, y + Height() - 1))
				{
					x -= STEP_SIZE;

					if (!map->IsEmpty(x + 1, y) || !map->IsEmpty(x + 1, y + Height() - 1))//防左方陷入
						x = (x / BLOCK_WIDTH_HEIGHT + 1) * BLOCK_WIDTH_HEIGHT;
				}
		}

		if (isMovingRight)
		{
			if (!isMovingUp && !isFalling)
				player.OnMove();

			if (x + 1 < BLOCK_WIDTH_HEIGHT * 53)
				if (map->IsEmpty(x + Width() + 1, y) && map->IsEmpty(x + Width() + 1, y + Height() - 1))
				{
					x += STEP_SIZE;

					if (!map->IsEmpty(x - 1, y) || !map->IsEmpty(x - 1, y + Height() - 1))  //防右方陷入
						x = (x + Width()) / BLOCK_WIDTH_HEIGHT * BLOCK_WIDTH_HEIGHT - Width();

					if ((x - map->GetScreenX() > SIZE_X * 2 / 3) && (map->GetScreenX() + SIZE_X < BLOCK_WIDTH_HEIGHT * 54 - STEP_SIZE))
						map->SetScreenX(STEP_SIZE);
				}
		}
			
	}

	void CGamePlayer::OnShow(CGameMap* map)
	{
		player.SetTopLeft(x - map->GetScreenX(), y - map->GetScreenY());
		player.OnShow();
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

	bool CGamePlayer::IsFalling()
	{
		return isFalling;
	}
}