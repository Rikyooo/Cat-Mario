#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMinion.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	CGameMinion::CGameMinion(void) 
	{
		this->minionState = 0;
		this->iHitBoxX = this->iHitBoxY = 32;

		this->killOtherUnits = false;
		this->minionSpawned = false;
		this->collisionOnlyWithPlayer = false;

		this->moveDirection = true;
		this->moveSpeed = 1;

		this->jumpState = 0;
		this->startJumpSpeed = 6.65f;
		this->currentFallingSpeed = 2.2f;

		this->currentJumpSpeed = 0;
		this->jumpDistance = 0;
		this->currentJumpDistance = 0;

		this->deadTime = -1;

		this->onAnotherMinion = false;
	}

	CGameMinion::~CGameMinion(void) 
	{
	}

	/* ******************************************** */

	void CGameMinion::Update(CGameMap* map) { }
	void CGameMinion::Draw(CGameMap* map, int iBlockID) { }

	bool CGameMinion::updateMinion(CGameMap* map)
	{
		if (!minionSpawned)
			Spawn(map);
		else
			minionPhysics(map);

		return minionSpawned;
	}

	void CGameMinion::minionPhysics(CGameMap* map) 
	{
		if (jumpState == 1)
			physicsState1(map);
		else
			if (!map->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !map->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true) && !onAnotherMinion)
				physicsState2(map);
			else 
			{
				jumpState = 0;
				onAnotherMinion = false;
			}
	}

	void CGameMinion::physicsState1(CGameMap* map)
	{
		updateYPos(-(int)currentJumpSpeed, map);
		currentJumpDistance += (int)currentJumpSpeed;

		currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f);

		if (currentJumpSpeed < 2.5f)
			currentJumpSpeed = 2.5f;

		if (jumpDistance <= currentJumpDistance)
			jumpState = 2;
	}

	void CGameMinion::physicsState2(CGameMap* map)
	{
		currentFallingSpeed *= 1.06f;

		if (currentFallingSpeed > startJumpSpeed)
			currentFallingSpeed = startJumpSpeed;

		updateYPos((int)currentFallingSpeed, map);

		jumpState = 2;

		if (fYPos >= SIZE_Y)
			minionState = -1;
	}

	void CGameMinion::updateXPos(CGameMap* map)
	{
		// ----- LEFT
		if (moveDirection) 
			if (map->checkCollisionLB((int)fXPos - moveSpeed, (int)fYPos - 2, iHitBoxY, true) || map->checkCollisionLT((int)fXPos - moveSpeed, (int)fYPos + 2, true)) 
			{
				moveDirection = !moveDirection;
				/*if (killOtherUnits && fXPos > -map->getXPos() - 64 && fXPos < -map->getXPos() + SIZE_X + 64 + iHitBoxX) 
					CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBLOCKHIT);*/
			}
			else
				fXPos -= (jumpState == 0 ? moveSpeed : moveSpeed / 2.0f);
		// ----- RIGHT
		else
			if (map->checkCollisionRB((int)fXPos + moveSpeed, (int)fYPos - 2, iHitBoxX, iHitBoxY, true) || map->checkCollisionRT((int)fXPos + moveSpeed, (int)fYPos + 2, iHitBoxX, true)) 
			{
				moveDirection = !moveDirection;
				/*if (killOtherUnits && fXPos > -map->getXPos() - 64 && fXPos < -map->getXPos() + SIZE_X + 64 + iHitBoxX) 
					CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBLOCKHIT);*/
			}
			else
				fXPos += (jumpState == 0 ? moveSpeed : moveSpeed / 2.0f);

		if (fXPos < -iHitBoxX)
			minionState = -1;
	}

	void CGameMinion::updateYPos(int iN, CGameMap* map) 
	{
		if (iN > 0) 
			if (!map->checkCollisionLB((int)fXPos + 2, (int)fYPos + iN, iHitBoxY, true) && !map->checkCollisionRB((int)fXPos - 2, (int)fYPos + iN, iHitBoxX, iHitBoxY, true)) 
				fYPos += iN;
			else 
			{
				if (jumpState == 1) 
					jumpState = 2;

				updateYPos(iN - 1, map);
			}
		else if (iN < 0)
			if (!map->checkCollisionLT((int)fXPos + 2, (int)fYPos + iN, true) && !map->checkCollisionRT((int)fXPos - 2, (int)fYPos + iN, iHitBoxX, true))
				fYPos += iN;
			else {
				if (jumpState == 1)
					jumpState = 2;

				updateYPos(iN + 1, map);
			}

		if ((int)fYPos % 2 == 1)
			fYPos += 1;
	}

	void CGameMinion::collisionEffect() 
	{
		if (minionSpawned)
			moveDirection = !moveDirection;
	}

	void CGameMinion::collisionWithPlayer(bool TOP, CGameMap* map) { }

	void CGameMinion::collisionWithAnotherUnit() { };

	void CGameMinion::lockMinion() { }

	/* ******************************************** */

	void CGameMinion::Spawn(CGameMap* map) 
	{
		if ((fXPos >= -map->getXPos() && fXPos <= -map->getXPos() + SIZE_X) || (fXPos + iHitBoxX >= -map->getXPos() && fXPos + iHitBoxX <= -map->getXPos() + SIZE_X))
			minionSpawned = true;
	}

	void CGameMinion::startJump(int iH) 
	{
		jumpState = 1;
		currentJumpSpeed = startJumpSpeed;
		jumpDistance = 32 * iH + 16.0f;
		currentJumpDistance = 0;
	}

	void CGameMinion::resetJump() {
		jumpState = 0;
		currentFallingSpeed = 2.7f;
	}

	void CGameMinion::minionDeathAnimation() 
	{
		fXPos += (moveDirection ? -1.5f : 1.5f);
		fYPos += 2 * (deadTime > 8 ? -1 : deadTime > 2 ? 1 : 4.25f);

		if (deadTime > 0)
			--deadTime;

		if (fYPos > SIZE_Y)
			minionState = -1;
	}

	/* ******************************************** */

	int CGameMinion::getBlockID() 
	{
		return iBlockID;
	}

	void CGameMinion::setBlockID(int iBlockID) 
	{
		this->iBlockID = iBlockID;
	}

	int CGameMinion::getXPos() 
	{
		return (int)fXPos;
	}

	int CGameMinion::getYPos() 
	{
		return (int)fYPos;
	}

	void CGameMinion::setYPos(int iYPos) 
	{
		this->fYPos = (float)iYPos;
	}

	int CGameMinion::getHitBoxX()
	{
		return iHitBoxX;
	}

	int CGameMinion::getHitBoxY()
	{
		return iHitBoxY;
	}

	bool CGameMinion::getMoveDirection()
	{
		return moveDirection;
	}

	void CGameMinion::setMoveDirection(bool moveDirection)
	{
		this->moveDirection = moveDirection;
	}

	bool CGameMinion::getCollisionOnlyWithPlayer()
	{
		return collisionOnlyWithPlayer;
	}

	bool CGameMinion::getMinionSpawned()
	{
		return minionSpawned;
	}

	int CGameMinion::getDeadTime()
	{
		return deadTime;
	}

	bool CGameMinion::getKillOtherUnits()
	{
		return killOtherUnits;
	}

	int CGameMinion::getMoveSpeed()
	{
		return moveSpeed;
	}

	void CGameMinion::setOnAnotherMinion(bool onAnotherMinion)
	{
		this->onAnotherMinion = onAnotherMinion;
	}

	int CGameMinion::getMinionState() 
	{
		return minionState;
	}

	void CGameMinion::setMinionState(int minionState)
	{
		this->minionState = minionState;
		if (minionState == -2) {
			deadTime = 16;
			fYPos -= iHitBoxY / 4;
			collisionOnlyWithPlayer = true;
			/*CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cSHOT);*/
		}
	}

	bool CGameMinion::getPowerUP() 
	{ 
		return true; 
	}
}