#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameKoppa.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	CGameKoppa::CGameKoppa(int iX, int iY, int minionState, bool moveDirection, int iBlockID)
	{
		this->fXPos = (float)iX;
		this->fYPos = (float)iY;
		this->minionState = minionState;

		this->moveDirection = moveDirection;
		this->flyDirection = moveDirection;

		this->moveSpeed = 1;

		this->iBlockID = iBlockID;

		switch (minionState) {
		case 0:
			this->iHitBoxX = 32;
			this->iHitBoxY = 41;
			break;
		case 1:
			this->iHitBoxX = 32;
			this->iHitBoxY = 41;
			break;
		case 2:
			this->iHitBoxX = 32;
			this->iHitBoxY = 30;
			this->moveSpeed = 0;
			this->killOtherUnits = true;
			break;
		case 3:
			this->iHitBoxX = 32;
			this->iHitBoxY = 41;
			this->moveDirection = true;
			break;
		}

		this->iDistance = 7 * 32;
	}

	CGameKoppa::~CGameKoppa()
	{
	}

	void CGameKoppa::Update(CGameMap * map)
	{
		if (minionState == 0) 
		{
			updateXPos(map);
			if (jumpState == 0) {

				startJump(1);
			}
		}
		else if (minionState == 1) 
		{
			updateXPos(map);
		}
		else if (minionState == 2) 
		{
			updateXPos(map);
		}
		else if (minionState == -2) 
		{
			CGameMinion::minionDeathAnimation();
		}
		else if (minionState != 3 && timeGetTime() - 500 >= (unsigned)deadTime) 
		{
			minionState = -1;
		}
	}

	void CGameKoppa::Draw(CGameMap * map, int iBlockID)
	{
		if (minionState != -2) 
		{
			if (moveDirection)
				map->getMinionBlock(iBlockID)->OnDraw((int)fXPos + (int)map->getXPos(), (int)fYPos);
			else
				map->getMinionBlock(iBlockID)->OnDraw((int)fXPos + (int)map->getXPos(), (int)fYPos);
		}
		/*else {
			iIMG->DrawVert(rR, (int)fXPos + (int)CCore::getMap()->getXPos(), (int)fYPos + 2);
		}*/
	}

	void CGameKoppa::minionPhysics(CGameMap* map)
	{
		if (minionState == 3) 
		{
			if (flyDirection) 
			{
				if (iDistance > 0) 
				{
					fYPos -= 2;
					iDistance -= 2;
				}
				else 
				{
					iDistance = 7 * 32;
					flyDirection = !flyDirection;
				}
			}
			else 
			{
				if (iDistance > 0) 
				{
					fYPos += 2;
					iDistance -= 2;
				}
				else 
				{
					iDistance = 7 * 32;
					flyDirection = !flyDirection;
				}
			}
		}
		else if (jumpState == 1) 
		{
			if (minionState == 0) 
			{
				updateYPos(-2, map);
				currentJumpDistance += 2;

				if (jumpDistance <= currentJumpDistance) 
				{
					jumpState = 2;
				}
			}
			else 
			{
				CGameMinion::physicsState1(map);
			}
		}
		else 
		{
			if (!map->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !map->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true) && !onAnotherMinion) 
			{
				if (minionState == 0) 
				{
					updateYPos(2, map);

					jumpState = 2;

					if (fYPos >= SIZE_Y) 
					{
						minionState = -1;
					}
				}
				else 
				{
					CGameMinion::physicsState2(map);
				}
			}
			else 
			{
				jumpState = 0;
				onAnotherMinion = false;
			}
		}
	}

	void CGameKoppa::updateXPos(CGameMap* map)
	{
		// ----- LEFT
		if (moveDirection) 
		{
			if (map->checkCollisionLB((int)fXPos - moveSpeed, (int)fYPos - 2, iHitBoxY, true) || map->checkCollisionLT((int)fXPos - moveSpeed, (int)fYPos + 2, true)) 
			{
				moveDirection = !moveDirection;
			}
			else 
			{
				bool LB = map->checkCollisionLB((int)fXPos + iHitBoxX / 2, (int)fYPos, iHitBoxY + 2, false), RB = map->checkCollisionRB((int)fXPos + iHitBoxX / 2, (int)fYPos, iHitBoxX, iHitBoxY + 2, false);
				if ((minionState == 1 && iBlockID != 4) && ((!LB && RB))) 
				{
					moveDirection = !moveDirection;
					fXPos += moveSpeed;
				}
				else 
				{
					fXPos -= moveSpeed;
				}
			}
		}
		// ----- RIGHT
		else {
			if (map->checkCollisionRB((int)fXPos + moveSpeed, (int)fYPos - 2, iHitBoxX, iHitBoxY, true) || map->checkCollisionRT((int)fXPos + moveSpeed, (int)fYPos + 2, iHitBoxX, true)) 
			{
				moveDirection = !moveDirection;
			}
			else 
			{
				bool LB = map->checkCollisionLB((int)fXPos - iHitBoxX / 2, (int)fYPos, iHitBoxY + 2, false), RB = map->checkCollisionRB((int)fXPos - iHitBoxX / 2, (int)fYPos, iHitBoxX, iHitBoxY + 2, false);
				if ((minionState == 1 && iBlockID != 4) && ((LB && !RB))) 
				{
					moveDirection = !moveDirection;
					fXPos -= moveSpeed;
				}
				else 
				{
					fXPos += moveSpeed;
				}
			}
		}

		if (fXPos < -iHitBoxX) 
		{
			minionState = -1;
		}
	}

	void CGameKoppa::collisionWithPlayer(bool TOP, CGameMap * map)
	{
		if (TOP) 
		{
			if (minionState == 0 || minionState == 3) 
			{
				minionState = 1;
				setMinion();
				map->getPlayer()->resetJump();
				map->getPlayer()->startJump(1);
				map->getPlayer()->SetY((float)map->getPlayer()->GetY() - 4);
				//CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cSTOMP);
			}
			else if (minionState == 1) 
			{
				minionState = 2;
				setMinion();
				map->getPlayer()->resetJump();
				map->getPlayer()->startJump(1);
				map->getPlayer()->SetY((float)map->getPlayer()->GetY() - 4);
				//CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cSTOMP);
			}
			else 
			{
				if (moveSpeed > 0) 
				{
					moveSpeed = 0;
				}
				else {
					if ((fXPos + iHitBoxX) / 2 < (map->getPlayer()->GetX() - map->getXPos() + map->getPlayer()->Width()) / 2) 
					{
						moveDirection = true;
					}
					else {
						moveDirection = false;
					}

					moveSpeed = 6;
				}

				map->getPlayer()->SetY((float)map->getPlayer()->GetY() - 4);
				map->getPlayer()->resetJump();
				map->getPlayer()->startJump(1);
				//CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cSTOMP);
			}
		}
		else {
			if (minionState == 2) 
			{
				if (moveSpeed == 0) 
				{
					//moveDirection = !CCore::getMap()->getPlayer()->getMoveDirection();
					moveDirection = (fXPos + iHitBoxX / 2 < map->getPlayer()->GetX() - map->getXPos() + map->getPlayer()->Width() / 2);
					if (moveDirection) fXPos -= map->getPlayer()->getMoveSpeed() + 1;
					else fXPos += map->getPlayer()->getMoveSpeed() + 1;
					moveSpeed = 6;
					//CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cSTOMP);
				}
				else 
				{
					map->playerDeath(true);
				}
			}
			else 
			{
				map->playerDeath(true);
			}
		}
	}

	void CGameKoppa::collisionEffect()
	{
		if (minionState != 2 && moveSpeed != 0) 
			moveDirection = !moveDirection;
	}

	void CGameKoppa::setMinionState(int minionState)
	{
		this->minionState = minionState;

		if (this->minionState == 3) {
			deadTime = timeGetTime();
		}

		CGameMinion::setMinionState(minionState);
	}

	void CGameKoppa::setMinion()
	{
		switch (minionState) {
		case 0: case 3:
			this->iHitBoxX = 32;
			this->iHitBoxY = 32;
			break;
		case 1:
			this->iHitBoxX = 32;
			this->iHitBoxY = 32;
			break;
		case 2:
			this->iHitBoxX = 32;
			this->iHitBoxY = 28;
			this->moveSpeed = 0;
			this->killOtherUnits = true;
			break;
		}

		switch (iBlockID) {
		//case 7:
		//	iBlockID = 4;
		//	break;
		//case 14:
		//	iBlockID = 12;
		//	break;
		//case 17:
		//	iBlockID = 15;
		//	break;
		case 7:
			iBlockID = 6;
			break;
		//case 12:
		//	iBlockID = 13;
		//	break;
		//case 15:
		//	iBlockID = 16;
		//	break;
		}
	}
}