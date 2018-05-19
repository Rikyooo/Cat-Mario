#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMapLevel.h"

/* ******************************************** */
namespace game_framework
{
	CGameMapLevel::CGameMapLevel(void) { }

	CGameMapLevel::CGameMapLevel(int iBlockID) {
		this->iBlockID = iBlockID;

		this->iNumOfUse = 0;

		this->blockAnimation = false;
		this->iYPos = 0;
		this->bYDirection = true;

		this->spawnMushroom = false;
		this->spawnStar = false;

		this->powerUP = true;
	}

	CGameMapLevel::~CGameMapLevel(void) {

	}

	/* ******************************************** */

	void CGameMapLevel::startBlockAnimation() {
		this->blockAnimation = true;
		this->iYPos = 0;
		this->bYDirection = true;
	}

	int CGameMapLevel::updateYPos() {
		if (blockAnimation) 
		{
			if (bYDirection)
			{
				if (iYPos < 10)
				{
					if (iYPos < 5) {
						iYPos += 2;
					}
					else {
						iYPos += 1;
					}
				}
				else {
					bYDirection = false;
				}
			}
			else {
				if (iYPos > 0) {
					if (iYPos > 5) {
						iYPos -= 2;
					}
					else {
						iYPos -= 1;
					}
				}
				else {
					blockAnimation = false;
				}
			}
		}

		return iYPos;
	}

	/* ******************************************** */

	int CGameMapLevel::getBlockID() 
	{
		return iBlockID;
	}

	void CGameMapLevel::setBlockID(int iBlockID) 
	{
		this->iBlockID = iBlockID;
	}

	int CGameMapLevel::getYPos() 
	{
		return iYPos;
	}

	int CGameMapLevel::getNumOfUse() 
	{
		return iNumOfUse;
	}

	void CGameMapLevel::setNumOfUse(int iNumOfUse) 
	{
		this->iNumOfUse = iNumOfUse;
	}

	bool CGameMapLevel::getSpawnMushroom() 
	{
		return spawnMushroom;
	}

	void CGameMapLevel::setSpawnMushroom(bool spawnMushroom) 
	{
		this->spawnMushroom = spawnMushroom;
	}

	bool CGameMapLevel::getPowerUP() 
	{
		return powerUP;
	}

	void CGameMapLevel::setPowerUP(bool powerUP) 
	{
		this->powerUP = powerUP;
	}

	bool CGameMapLevel::getSpawnStar() 
	{
		return spawnStar;
	}

	void CGameMapLevel::setSpawnStar(bool spawnStar) 
	{
		this->spawnStar = spawnStar;
	}
}