#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameCoin.h"
#include "CGameMap.h"

namespace game_framework
{
	CGameCoin::CGameCoin(int iXPos, int iYPos)
	{
		this->iXPos = iXPos;
		this->iYPos = iYPos;

		this->iStepID = 0;

		this->iLEFT = 80;
		this->bTOP = true;
		this->bDelete = false;
	}

	CGameCoin::~CGameCoin(void)
	{
		
	}

	void CGameCoin::Update()
	{
		if (iLEFT > 0) 
		{
			iLEFT -= 5;
			iYPos = iYPos + (bTOP ? -5 : 5);

		}
		else if (bTOP) 
		{
			bTOP = false;
			iLEFT = 60;
		}
		else
			bDelete = true;
	}

	void CGameCoin::Draw(CGameMap* map)
	{
		map->getBlock(22)->OnDraw(iXPos + (int)map->getXPos(), iYPos, false);
	}

	int CGameCoin::getXPos()
	{
		return iXPos;
	}

	int CGameCoin::getYPos()
	{
		return iYPos;
	}

	bool CGameCoin::getDelete()
	{
		return bDelete;
	}
}