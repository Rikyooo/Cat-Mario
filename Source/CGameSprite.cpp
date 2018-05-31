#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameSprite.h"

namespace game_framework
{
	CGameSprite::CGameSprite()
	{
	}

	CGameSprite::CGameSprite(std::vector<int> iSprite, std::vector<unsigned int> iDelay, bool bRotate)
	{
		this->iDelay = iDelay;
		this->bRotate = bRotate;

		this->iCurrentFrame = 0;
		this->iStartFrame = 0;
		this->iEndFrame = iSprite.size() - 1;

		for (int i = 0; i < this->iEndFrame + 1; i++) 
		{
			CMovingBitmap* tempBitmap;
			tempBitmap->LoadBitmap(iSprite[i]);
			this->vSprite.push_back(tempBitmap); 
		}

		this->lTimePassed = 0;
	}

	CGameSprite::~CGameSprite()
	{
		for (std::vector<CMovingBitmap*>::iterator i = vSprite.begin(); i != vSprite.end(); i++) 
			delete (*i);
	}

	void CGameSprite::Update()
	{
		if (timeGetTime() - iDelay[iCurrentFrame] > lTimePassed) 
		{
			lTimePassed = timeGetTime();

			if (iCurrentFrame == iEndFrame) 
				iCurrentFrame = 0;
			else 
				++iCurrentFrame;
		}
	}

	CMovingBitmap* CGameSprite::getTexture()
	{
		return vSprite[iCurrentFrame];
	}

	CMovingBitmap* CGameSprite::getTexture(int iID)
	{
		return vSprite[iID];
	}
}