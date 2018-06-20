#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameFlag.h"
#include "CGameMap.h"

namespace game_framework
{
	CGameFlag::CGameFlag(int iXPos, int iYPos, int height) 
	{
		this->iXPos = iXPos;
		this->iYPos = iYPos;
		this->height = height;
		this->iBlockID = 30;
	}

	CGameFlag::~CGameFlag(void) 
	{
	}

	void CGameFlag::Update() 
	{
		if (height >= 4)
		{
			height -= 4;
			iYPos += 4;
		}
	}

	void CGameFlag::Draw(CGameMap * map) 
	{
		map->getBlock(iBlockID)->OnDraw((int)(iXPos + map->getXPos()), iYPos, false);

	}
}