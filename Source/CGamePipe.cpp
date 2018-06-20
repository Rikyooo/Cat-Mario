#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGamePipe.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	CGamePipe::CGamePipe(int iType, int iLX, int iLY, int iRX, int iRY, int newPlayerPosX, int newPlayerPosY, int newCurrentLevel, int newLevelType, bool newMoveMap, int iDelay, int iSpeed, bool underWater)
	{
		this->iType = iType;

		this->iLX = iLX;
		this->iLY = iLY;
		this->iRX = iRX;
		this->iRY = iRY;

		this->newPlayerPosX = newPlayerPosX;
		this->newPlayerPosY = newPlayerPosY;

		this->newCurrentLevel = newCurrentLevel;
		this->newLevelType = newLevelType;
		this->newMoveMap = newMoveMap;
		this->newUnderWater = newUnderWater;

		this->iDelay = iDelay;

		this->iSpeed = iSpeed;
	}

	CGamePipe::~CGamePipe(void)
	{
	}

	void CGamePipe::checkUse(CGameMap * map)
	{
		if (iType == 0 || iType == 2) 
		{
			if (-(int)map->getXPos() + map->getPlayer()->GetX() >= iLX * 32 + 4 && -(int)map->getXPos() + map->getPlayer()->GetX() + map->getPlayer()->GetHitBoxX() < (iRX + 1) * 32 - 4) 
			{
				setEvent();
			}
		}
	}

	void CGamePipe::setEvent()
	{
	}
}