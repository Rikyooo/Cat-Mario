#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameBlock.h"

/* ******************************************** */
namespace game_framework
{
	CGameBlock::CGameBlock() {}

	CGameBlock::CGameBlock(int blockID, int bmpID, bool isCollision, bool isAlive, bool isUse, bool isVisible)
	{
		this->blockID = blockID;
		this->bmpID = bmpID;
		this->isCollision = isCollision;
		this->isAlive = isAlive;
		this->isUse = isUse;
		this->isVisible = isVisible;	
	}

	CGameBlock::~CGameBlock() {}

	void CGameBlock::LoadBitmap()
	{
		bmp.LoadBitmap(bmpID, RGB(0,0,255));
	}

	void CGameBlock::OnDraw(int x, int y, bool isHorizontalFlip)
	{
		bmp.SetTopLeft(x, y);
		bmp.ShowBitmap(isHorizontalFlip);
	}

	/* ******************************************** */

	int CGameBlock::getBlockID() 
	{
		return blockID;
	}

	void CGameBlock::setBlockID(int blockID) 
	{
		this->blockID = blockID;
	}

	bool CGameBlock::IsCollision()
	{
		return isCollision;
	}

	bool CGameBlock::IsAlive()
	{
		return isAlive;
	}

	bool CGameBlock::IsUse()
	{
		return isUse;
	}

	bool CGameBlock::IsVisible()
	{
		return isVisible;
	}
}