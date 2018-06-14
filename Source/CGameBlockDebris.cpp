#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameBlockDebris.h"
#include "CGameMap.h"

namespace game_framework
{
	CGameBlockDebris::CGameBlockDebris(int iXPos, int iYPos)
	{
		this->debrisState = 0;
		this->bRotate = false;

		this->vPositionL = new Vector2(iXPos, iYPos);
		this->vPositionR = new Vector2(iXPos + 16, iYPos);
		this->vPositionL2 = new Vector2(iXPos, iYPos + 16);
		this->vPositionR2 = new Vector2(iXPos + 16, iYPos + 16);

		this->fSpeedX = 2.15f;
		this->fSpeedY = 1;
	}

	CGameBlockDebris::~CGameBlockDebris(void)
	{
		delete vPositionL;
		delete vPositionR;
		delete vPositionL2;
		delete vPositionR2;
	}

	void CGameBlockDebris::Update()
	{
		++iFrameID;

		if (iFrameID > 4) {
			bRotate = !bRotate;
			iFrameID = 0;
		}

		vPositionL->setX((int)(vPositionL->getX() - fSpeedX));
		vPositionL->setY((int)(vPositionL->getY() + (fSpeedY - 3.0f) + (fSpeedY < 3 ? fSpeedY < 2.5f ? -3 : -1.5f : fSpeedY < 3.5f ? 1.5f : 3)));

		vPositionR->setX((int)(vPositionR->getX() + fSpeedX));
		vPositionR->setY((int)(vPositionR->getY() + (fSpeedY - 3.0f) + (fSpeedY < 3 ? fSpeedY < 2.5f ? -3 : -1.5f : fSpeedY < 3.5f ? 1.5f : 3)));

		vPositionL2->setX((int)(vPositionL2->getX() - (fSpeedX - 1.1f)));
		vPositionL2->setY((int)(vPositionL2->getY() + (fSpeedY - 1.5f) + (fSpeedY < 1.5 ? fSpeedY < 1.3f ? -3 : -1 : fSpeedY < 1.8f ? 1 : 3)));

		vPositionR2->setX((int)(vPositionR2->getX() + (fSpeedX - 1.1f)));
		vPositionR2->setY((int)(vPositionR2->getY() + (fSpeedY - 1.5f) + (fSpeedY < 1.5 ? fSpeedY < 1.3f ? -3 : -1 : fSpeedY < 1.8f ? 1 : 3)));

		fSpeedY *= 1.09f;
		fSpeedX *= 1.005f;

		if (vPositionL->getY() >= SIZE_Y)
			debrisState = -1;
	}

	void CGameBlockDebris::Draw(CGameMap * map)
	{
		map->getBlock(map->getLevelType() == 0 || map->getLevelType() == 4 ? 23 : map->getLevelType() == 1 ? 65 : 66)->OnDraw(vPositionL->getX() + (int)map->getXPos(), vPositionL->getY(), bRotate);
		map->getBlock(map->getLevelType() == 0 || map->getLevelType() == 4 ? 23 : map->getLevelType() == 1 ? 65 : 66)->OnDraw(vPositionR->getX() + (int)map->getXPos(), vPositionR->getY(), bRotate);
		map->getBlock(map->getLevelType() == 0 || map->getLevelType() == 4 ? 23 : map->getLevelType() == 1 ? 65 : 66)->OnDraw(vPositionL2->getX() + (int)map->getXPos(), vPositionL2->getY(), bRotate);
		map->getBlock(map->getLevelType() == 0 || map->getLevelType() == 4 ? 23 : map->getLevelType() == 1 ? 65 : 66)->OnDraw(vPositionR2->getX() + (int)map->getXPos(), vPositionR2->getY(), bRotate);
	}

	int CGameBlockDebris::getDebrisState()
	{
		return debrisState;
	}
}