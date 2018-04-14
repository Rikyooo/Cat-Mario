#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "CGamePlayer.h"
#include "CGameEnemy.h"

namespace game_framework
{
	CGameEnemy::CGameEnemy(int speed)
	{
		movingSpeed = speed;
	}

	void CGameEnemy::LoadBitmap()
	{
		enemy.LoadBitmap(IDB_ENEMY_NORMAL, RGB(160, 180, 250));
	}

	void CGameEnemy::OnMove(CGameMap * map)
	{
		if (!isAlive)
			return;
		x -= movingSpeed;
	}

	void CGameEnemy::OnShow(CGameMap * map)
	{
		if (x - 1 > map->GetScreenX() && isAlive)
		{
			enemy.SetTopLeft(x - map->GetScreenX(), y - map->GetScreenY());
			enemy.ShowBitmap();
		}

	}
	int CGameEnemy::Height()
	{
		return enemy.Height();
	}
	int CGameEnemy::Width()
	{
		return enemy.Width();
	}
	void CGameEnemy::SetXY(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	int CGameEnemy::GetX()
	{
		return x;
	}
	int CGameEnemy::GetY()
	{
		return y;
	}

	bool CGameEnemy::IsAlive()
	{
		return isAlive;
	}

	void CGameEnemy::SetIsAlive(bool alive)
	{
		isAlive = alive;
	}

	bool CGameEnemy::HitPlayer(CGamePlayer * player)
	{
		int x_rb = x + 33;
		int y_rb = y + 30;
		int player_x = player->GetX();
		int player_y = player->GetY();
		int player_x_rb = player_x + player->Width();
		int player_y_rb = player_y + player->Height();

		return ((player_x <= x_rb) && (player_y <= y_rb) && (player_x_rb >= x) && (player_y_rb >= y));
	}
}