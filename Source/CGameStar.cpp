#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameStar.h"
#include "CGameMap.h"

namespace game_framework
{
	CGameStar::CGameStar(int iXPos, int iYPos, int iX, int iY)
	{
		this->fXPos = (float)iXPos + 2;
		this->fYPos = (float)iYPos;

		this->collisionOnlyWithPlayer = true;

		this->moveDirection = false;
		this->moveSpeed = 3;

		this->inSpawnState = true;
		this->minionSpawned = true;
		this->inSpawnY = 30;
		this->startJumpSpeed = 4;

		this->iX = iX;
		this->iY = iY;

		this->iBlockID = 5;

		this->iHitBoxX = 28;
		this->iHitBoxY = 32;
	}

	CGameStar::~CGameStar(void)
	{
	}

	void CGameStar::Update(CGameMap * map)
	{
		if (inSpawnState)
		{
			if (inSpawnY <= 0)
				inSpawnState = false;
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
			if (jumpState == 0) 
			{
				startJump(1);
				jumpDistance = 32;
			}

			updateXPos(map);
		}
	}

	bool CGameStar::updateMinion(CGameMap * map)
	{
		if (!inSpawnState)
			minionPhysics(map);

		return minionSpawned;
	}

	void CGameStar::minionPhysics(CGameMap * map)
	{
		if (jumpState == 1)
		{
			if (minionState == 0)
			{
				updateYPos(-4 + (currentJumpDistance > 64 ? 2 : 0), map);
				currentJumpDistance += 2;

				if (jumpDistance <= currentJumpDistance)
					jumpState = 2;
			}
		}
		else
		{
			if (!map->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !map->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true))
				CGameMinion::physicsState2(map);
			else
				jumpState = 0;
		}
	}

	void CGameStar::Draw(CGameMap * map, int iBlockID)
	{
		map->getMinionBlock(iBlockID)->OnDraw((int)fXPos + (int)map->getXPos(), (int)fYPos, false);
		if (inSpawnState)
			map->getBlock(map->getLevelType() == 0 || map->getLevelType() == 4 ? 10 : 56)->OnDraw((int)fXPos + (int)map->getXPos() - 2, (int)fYPos + (32 - inSpawnY) - map->getMapBlock(iX, iY)->getYPos() - 2, false);
	}

	void CGameStar::collisionWithPlayer(bool TOP, CGameMap * map)
	{
		if (!inSpawnState) 
		{
			//map->getPlayer()->setStarEffect(true);
			minionState = -1;
		}
	}

	void CGameStar::setMinionState(int minionState)
	{
	}
}