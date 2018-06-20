#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMushroom.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	CGameMushroom::CGameMushroom(int iXPos, int fYPos, bool powerUP, int iX, int iY) 
	{
		this->fXPos = (float)iXPos;
		this->fYPos = (float)fYPos - 2;
		this->moveSpeed = 2;
		this->inSpawnState = true;
		this->minionSpawned = true;
		this->inSpawnY = 30;
		this->moveDirection = false;
		this->powerUP = powerUP;
		if (powerUP)
			this->iBlockID = 1;
		else
			this->iBlockID = 2;
		this->collisionOnlyWithPlayer = true;

		this->minionState = 0;

		this->iX = iX;
		this->iY = iY;
		this->iHitBoxX = 29;
		this->iHitBoxY = 30;
	}

	CGameMushroom::~CGameMushroom(void) 
	{
	}

	/* ******************************************** */

	void CGameMushroom::Update(CGameMap* map) 
	{
		if (inSpawnState) 
		{
			if (inSpawnY <= 0) 
			{
				inSpawnState = false;
			}
			else 
			{
				if (fYPos > -5) 
				{
					inSpawnY -= 2;
					fYPos -= 2;
				}
				else 
				{
					inSpawnY -= 1;
					fYPos -= 1;
				}
			}
		}
		else 
		{
			updateXPos(map);
		}
	}

	bool CGameMushroom::updateMinion(CGameMap* map) 
	{
		if (!inSpawnState)
			minionPhysics(map);

		return minionSpawned;
	}

	void CGameMushroom::Draw(CGameMap* map, int iBlockID)
	{
		if (minionState >= 0) 
		{
			map->getMinionBlock(iBlockID)->OnDraw((int)fXPos + (int)map->getXPos(), (int)fYPos, false);
			if (inSpawnState) 
				map->getBlock(map->getLevelType() == 0 || map->getLevelType() == 4 ? 10 : 56)->OnDraw((int)fXPos + (int)map->getXPos(), (int)fYPos + (32 - inSpawnY) - map->getMapBlock(iX, iY)->getYPos(), false);
		}
	}

	/* ******************************************** */

	void CGameMushroom::collisionWithPlayer(bool TOP, CGameMap* map)
	{
		if (!inSpawnState && minionState == 0) 
		{
			if (powerUP) 
			{
				map->getPlayer()->setPowerLVL(map->getPlayer()->getPowerLVL() + 1);
				map->getPlayer()->SetY((float)(map->getPlayer()->GetY()-48));
			}
			else 
			{
				map->getPlayer()->Death(true);
				//CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cONEUP);
			}

			minionState = -1;
		}
	}

	void CGameMushroom::setMinionState(int minionState)
	{
	}
}