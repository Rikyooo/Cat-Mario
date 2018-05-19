#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameGoombas.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	CGameGoombas::CGameGoombas(int iX, int iY, int iBlockID, bool moveDirection)
	{
		this->fXPos = (float)iX;
		this->fYPos = (float)iY;
		this->iBlockID = iBlockID;
		this->moveDirection = moveDirection;
		this->moveSpeed = 1;
		this->iHitBoxY = 29;
	}

	CGameGoombas::~CGameGoombas(void)
	{
	}

	void CGameGoombas::Update(CGameMap* map)
	{
		if (minionState == 0) 
			updateXPos(map);
		else if (minionState == -2) 
			CGameMinion::minionDeathAnimation();
		else if (map->getPlayer()->GetTimeTick() - 10 >= deadTime)
			minionState = -1;
	}

	void CGameGoombas::Draw(CGameMap * map, int iBlockID)
	{
		if (minionState != -2)
			map->getMinionBlock(iBlockID)->OnDraw((int)fXPos + (int)map->getXPos(), (int)fYPos);
		/*else {
			iIMG->DrawVert(rR, (int)fXPos + (int)map->getXPos(), (int)fYPos + 2);
		}*/
	}

	void CGameGoombas::collisionWithPlayer(bool TOP, CGameMap* map)
	{
		/*if (map->getPlayer()->getStarEffect())
		{
			setMinionState(-2);
		}
		else */
		if (TOP) 
		{
			if (minionState == 0) 
			{
				minionState = 1;
				iBlockID = map->getLevelType() == 0 || map->getLevelType() == 4 ? 0 : map->getLevelType() == 1 ? 9 : 11;
				deadTime = map->getPlayer()->GetTimeTick();
				map->getPlayer()->resetJump();
				map->getPlayer()->startJump(1);
				//points(100);
				//CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cSTOMP);
			}
		}
		else 
		{
			map->playerDeath(true);
		}
	}

	void CGameGoombas::setMinionState(int minionState, CGameMap* map)
	{
		this->minionState = minionState;

		if (this->minionState == 1)
			deadTime = map->getPlayer()->GetTimeTick();

		CGameMinion::setMinionState(minionState, map);
	}
}