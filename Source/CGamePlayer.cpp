#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	CGamePlayer::CGamePlayer(float fXPos, float fYPos)
	{
		this->fXPos = fXPos;
		this->fYPos = fYPos;

		//this->iSpriteID = 1;
		this->nextFallFrameID = 0;

		this->moveDirection = true;
		this->currentMaxMove = maxMove;
		this->moveSpeed = 0;
		this->bMove = false;
		this->changeMoveDirection = false;

		//this->onPlatformID = -1;

		//this->springJump = false;

		this->iTimePassed = iTimeTick;

		this->isUpPressed = false;
		this->jumpState = 0;
		this->startJumpSpeed = 7.65f;
		this->currentFallingSpeed = 2.7f;
	}

	void CGamePlayer::LoadBitmap()
	{
		player_bmp.LoadBitmap(IDB_PLAYER_1, RGB(160, 180, 250));
	}

	//void CGamePlayer::OnMove(CGameMap* map)
	//{
	//	const int STEP_SIZE = 11;

	//	if (isMovingUp)
	//		if ((map->IsEmpty(x, y - 1) && map->IsEmpty(x + Width(), y - 1)) && jumpingCount < 6)
	//		{
	//			jumpingCount++;
	//			//player.SetCurrentBitmap(2);   //jumping style = 2
	//			y -= 33;

	//			if (!map->IsEmpty(x, y + 1) || !map->IsEmpty(x + Width(), y + 1))//防上方陷入
	//			{
	//				jumpingCount = 6;
	//				y = (y / BLOCK_WIDTH_HEIGHT + 1) * BLOCK_WIDTH_HEIGHT;
	//			}
	//		}

	//	if ((map->IsEmpty(x, y + Height() + 1) && map->IsEmpty(x + Width(), y + Height() + 1)) || y + Height() >= SIZE_Y)
	//	{
	//		isFalling = true;
	//		y += 6;
	//	}
	//	else
	//	{
	//		jumpingCount = 0;
	//		isMovingUp = false;
	//		isFalling = false;

	//		/*if (!isMovingLeft && !isMovingRight)
	//			player.SetCurrentBitmap(0);*/
	//	}
	//	if (!(y + Height() >= SIZE_Y))
	//	{
	//		if (!map->IsEmpty(x, y + Height() - 1) || !map->IsEmpty(x + Width(), y + Height() - 1))  //防下方陷入
	//			y = (y + Height()) / BLOCK_WIDTH_HEIGHT * BLOCK_WIDTH_HEIGHT - Height();
	//	}

	//	if (isMovingLeft)
	//	{
	//		if (!isMovingUp && !isFalling)
	//			bmp.OnMove();

	//		if (x - 1 > map->GetScreenX())
	//			if (map->IsEmpty(x - 1, y) && map->IsEmpty(x - 1, y + Height() - 1))
	//			{
	//				x -= STEP_SIZE;

	//				if (!map->IsEmpty(x + 1, y) || !map->IsEmpty(x + 1, y + Height() - 1))//防左方陷入
	//					x = (x / BLOCK_WIDTH_HEIGHT + 1) * BLOCK_WIDTH_HEIGHT;
	//			}
	//	}

	//	if (isMovingRight)
	//	{
	//		if (!isMovingUp && !isFalling)
	//			bmp.OnMove();

	//		if (x + 1 < BLOCK_WIDTH_HEIGHT * 53)
	//			if (map->IsEmpty(x + Width() + 1, y) && map->IsEmpty(x + Width() + 1, y + Height() - 1))
	//			{
	//				x += STEP_SIZE;

	//				if (!map->IsEmpty(x - 1, y) || !map->IsEmpty(x - 1, y + Height() - 1))  //防右方陷入
	//					x = (x + Width()) / BLOCK_WIDTH_HEIGHT * BLOCK_WIDTH_HEIGHT - Width();

	//				if ((x - map->GetScreenX() > SIZE_X * 2 / 3) && (map->GetScreenX() + SIZE_X < BLOCK_WIDTH_HEIGHT * 54 - STEP_SIZE))
	//					map->SetScreenX(STEP_SIZE);
	//			}
	//	}
	//		
	//}

	/*void CGamePlayer::OnShow(CGameMap* map)
	{
		bmp.SetTopLeft(x - map->GetScreenX(), y - map->GetScreenY());
		bmp.OnShow();
	}*/

	int CGamePlayer::Width()
	{
		return player_bmp.Width();
	}

	int CGamePlayer::Height()
	{
		return player_bmp.Height();
	}

	void CGamePlayer::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CGamePlayer::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CGamePlayer::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void CGamePlayer::SetX(float x)
	{
		this->fXPos = x;
	}

	int CGamePlayer::GetX()
	{
		return (int)fXPos;
	}

	void CGamePlayer::SetY(float y) 
	{
		this->fYPos = y;
	}
	int CGamePlayer::GetY()
	{
		return (int)fYPos;
	}

	void CGamePlayer::SetIsOnland(bool state)
	{
		isOnland = state;
	}

	bool CGamePlayer::IsFalling()
	{
		return isFalling;
	}

	void CGamePlayer::Draw()
	{
		player_bmp.SetTopLeft((int)fXPos, (int)fYPos);
		player_bmp.ShowBitmap();
	}

	void CGamePlayer::Update(CGameMap *map)
	{
		playerPhysics(map);
		movePlayer(map);
	}

	void CGamePlayer::playerPhysics(CGameMap *map)
	{
			if (jumpState == 1) 
			{
				updateYPos(-(int)currentJumpSpeed, map);
				currentJumpDistance += (int)currentJumpSpeed;

				currentJumpSpeed *= (currentJumpDistance / jumpDistance > 0.75f ? 0.972f : 0.986f);

				if (currentJumpSpeed < 2.5f)
					currentJumpSpeed = 2.5f;

				if (!isUpPressed && currentJumpDistance > 64 /*&& !springJump*/) 
				{
					jumpDistance = 16;
					currentJumpDistance = 0;
					currentJumpSpeed = 2.5f;
				}

				if (jumpDistance <= currentJumpDistance)
					jumpState = 2;
			}
			else 
			{
				/*if (onPlatformID == -1) 
				{
					onPlatformID = map->checkCollisionWithPlatform((int)fXPos, (int)fYPos, Width(), Height());
					if (onPlatformID >= 0) 
					{
						if (map->checkCollisionLB((int)(fXPos - map->getXPos() + 2), (int)fYPos + 2, Height(), true) || map->checkCollisionRB((int)(fXPos - map->getXPos() - 2), (int)fYPos + 2, Width(), Height(), true)) 
						{
							onPlatformID = -1;
							resetJump();
						}
						else {
							fYPos = (float)map->getPlatform(onPlatformID)->getYPos() - Height();
							resetJump();
							map->getPlatform(onPlatformID)->turnON();
						}
					}
				}
				else
					onPlatformID = map->checkCollisionWithPlatform((int)fXPos, (int)fYPos, Width(), Height());*/

				//if (onPlatformID >= 0) 
				//{
				//	if (map->checkCollisionLB((int)(fXPos - map->getXPos() + 2), (int)fYPos + 2, Height(), true) || map->checkCollisionRB((int)(fXPos - map->getXPos() - 2), (int)fYPos + 2, Width(), Height(), true)) 
				//	{
				//		onPlatformID = -1;
				//		resetJump();
				//	}
				//	else {
				//		fYPos += map->getPlatform(onPlatformID)->getMoveY();
				//		map->getPlatform(onPlatformID)->moveY();
				//		//if(moveSpeed == 0)
				//		map->setXPos(map->getXPos() - map->getPlatform(onPlatformID)->getMoveX());

				//		jumpState = 0;
				//	}
				//}
				//else 
				if (!map->checkCollisionLB((int)(fXPos - map->getXPos() + 2), (int)fYPos + 2, Height(), true) && !map->checkCollisionRB((int)(fXPos - map->getXPos() - 2), (int)fYPos + 2, Width(), Height(), true)) 
				{

					if (nextFallFrameID > 0)
						--nextFallFrameID;
					else 
					{
						currentFallingSpeed *= 1.05f;
						if (currentFallingSpeed > startJumpSpeed)
							currentFallingSpeed = startJumpSpeed;

						updateYPos((int)currentFallingSpeed, map);
					}

					jumpState = 2;

					//setMarioSpriteID(5);
				}
				else if (jumpState == 2)
					resetJump();
				else
					checkCollisionBot(0, 0, map);
			}
	}

	void CGamePlayer::updateXPos(int displacement, CGameMap *map)
	{
		checkCollisionBot(displacement, 0, map);
		//checkCollisionCenter(displacement, 0, map);
		if (displacement > 0) 
		{
			if (!map->checkCollisionRB((int)(fXPos - map->getXPos() + displacement), (int)fYPos - 2, Width(), Height(), true) && !map->checkCollisionRT((int)(fXPos - map->getXPos() + displacement), (int)fYPos + 2, Width(), true))
			{
				if (fXPos >= 215 && map->getMoveMap() && SIZE_X - map->getXPos() + displacement < map->getMapWidth() * BLOCK_WIDTH_HEIGHT)
					map->moveMap(-displacement, 0);
				else
					fXPos += displacement;
			}
			else 
			{
				updateXPos(displacement - 1, map);
				if (moveSpeed > 1 && jumpState == 0) 
					--moveSpeed;
			}
		}
		else if (displacement < 0) {
			if (!map->checkCollisionLB((int)(fXPos - map->getXPos() + displacement), (int)fYPos - 2, Height(), true) && !map->checkCollisionLT((int)(fXPos - map->getXPos() + displacement), (int)fYPos + 2, true))
			{
				if (fXPos - map->getXPos() + displacement >= 0 && fXPos >= 0)
					fXPos += displacement;
			}
			else 
			{
				updateXPos(displacement + 1, map);
				if (moveSpeed > 1 && jumpState == 0) 
					--moveSpeed;
			}
		}
	}

	void CGamePlayer::updateYPos(int iN, CGameMap *map)
	{
		bool bLEFT, bRIGHT;

		if (iN > 0)
		{
			bLEFT = map->checkCollisionLB((int)(fXPos - map->getXPos() + 2), (int)fYPos + iN, Height(), true);
			bRIGHT = map->checkCollisionRB((int)(fXPos - map->getXPos() - 2), (int)fYPos + iN, Width(), Height(), true);

			if (!bLEFT && !bRIGHT)
				fYPos += iN;
			else
			{
				if (jumpState == 2)
					jumpState = 0;

				updateYPos(iN - 1, map);
			}
		}
		else if (iN < 0)
		{
			bLEFT = map->checkCollisionLT((int)(fXPos - map->getXPos() + 2), (int)fYPos + iN, false);
			bRIGHT = map->checkCollisionRT((int)(fXPos - map->getXPos() - 2), (int)fYPos + iN, Width(), false);

			/*if (map->checkCollisionWithPlatform((int)fXPos, (int)fYPos, 0, 0) >= 0 || map->checkCollisionWithPlatform((int)fXPos, (int)fYPos, Width(), 0) >= 0)
			{
				jumpState = 2;
			}
			else */
			if (!bLEFT && !bRIGHT)
				fYPos += iN;
			else
			{
				if (jumpState == 1)
				{
					if (!bLEFT && bRIGHT)
					{
						//if (fXPos - map->getXPos() / BLOCK_WIDTH_HEIGHT < map->getMapWidth())
							Vector2* vRT = getBlockRT(fXPos - map->getXPos(), fYPos + iN, map);

							if (!map->getBlock(map->getMapBlock(vRT->getX(), vRT->getY())->getBlockID())->IsVisible())
							{
								if (map->blockUse(vRT->getX(), vRT->getY(), map->getMapBlock(vRT->getX(), vRT->getY())->getBlockID(), 0))
									jumpState = 2;
								else
									fYPos += iN;
							}
							else if ((int)(fXPos + Width() - map->getXPos()) % 32 <= 8)
								updateXPos((int)-((int)(fXPos + Width() - map->getXPos()) % 32), map);
							else if (map->getBlock(map->getMapBlock(vRT->getX(), vRT->getY())->getBlockID())->IsUse())
							{
								if (map->blockUse(vRT->getX(), vRT->getY(), map->getMapBlock(vRT->getX(), vRT->getY())->getBlockID(), 0))
									jumpState = 2;
								else
									fYPos += iN;
							}
							else
								jumpState = 2;

							delete vRT;
					}
					else if (bLEFT && !bRIGHT)
					{
						Vector2* vLT = getBlockLT(fXPos - map->getXPos(), fYPos + iN, map);

						if (!map->getBlock(map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->IsVisible())
						{
							if (map->blockUse(vLT->getX(), vLT->getY(), map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 0))
								jumpState = 2;
							else
								fYPos += iN;
						}
						else if ((int)(fXPos - map->getXPos()) % 32 >= 24)
							updateXPos((int)(32 - (int)(fXPos - map->getXPos()) % 32), map);
						else if (map->getBlock(map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->IsUse())
						{
							if (map->blockUse(vLT->getX(), vLT->getY(), map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 0))
								jumpState = 2;
							else
								fYPos += iN;
						}
						else
							jumpState = 2;

						delete vLT;
					}
					else
					{
						if ((int)(fXPos + Width() - map->getXPos()) % BLOCK_WIDTH_HEIGHT > BLOCK_WIDTH_HEIGHT - (int)(fXPos - map->getXPos()) % BLOCK_WIDTH_HEIGHT)
						{
							//if (fXPos - map->getXPos() / BLOCK_WIDTH_HEIGHT < map->getMapWidth())
								Vector2* vRT = getBlockRT(fXPos - map->getXPos(), fYPos + iN, map);

								if (map->getBlock(map->getMapBlock(vRT->getX(), vRT->getY())->getBlockID())->IsUse())
								{
									if (map->blockUse(vRT->getX(), vRT->getY(), map->getMapBlock(vRT->getX(), vRT->getY())->getBlockID(), 0))
										jumpState = 2;
								}
								else
									jumpState = 2;

								delete vRT;
						}
						else
						{
							Vector2* vLT = getBlockLT(fXPos - map->getXPos(), fYPos + iN, map);

							if (map->getBlock(map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->IsUse())
							{
								if (map->blockUse(vLT->getX(), vLT->getY(), map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 0))
									jumpState = 2;
							}
							else
								jumpState = 2;

							delete vLT;
						}
					}
				}

				updateYPos(iN + 1, map);
			}
		}

		if ((int)fYPos % 2 == 1)
		{
			fYPos += 1;
		}

		if (/*!map->getInEvent() &&*/ fYPos > SIZE_Y)
		{
			map->playerDeath(false);
			fYPos = -80;
		}
	}

	void CGamePlayer::startMove()
	{
		moveSpeed = 1;
		bMove = true;
	}

	void CGamePlayer::resetMove()
	{
		--moveSpeed;
		bMove = false;
	}

	void CGamePlayer::stopMove()
	{
		moveSpeed = 0;
		bMove = false;
		changeMoveDirection = false;
	}

	void CGamePlayer::setMoveDirection(bool moveDirection)
	{
		this->moveDirection = moveDirection;
	}

	bool CGamePlayer::getChangeMoveDirection()
	{
		return changeMoveDirection;
	}

	void CGamePlayer::setChangeMoveDirection()
	{
		this->changeMoveDirection = true;
		this->newMoveDirection = !moveDirection;
	}

	void CGamePlayer::setIsUpPressed(bool isUpPressed)
	{
		this->isUpPressed = isUpPressed;
	}

	bool CGamePlayer::IsUpPressed()
	{
		return isUpPressed;
	}

	void CGamePlayer::jump()
	{
		if (jumpState == 0)
			startJump(4);
	}

	void CGamePlayer::startJump(int iH)
	{
		currentJumpSpeed = startJumpSpeed;
		jumpDistance = BLOCK_WIDTH_HEIGHT * iH + 24.0f;
		currentJumpDistance = 0;

		//animation and music

		jumpState = 1;
	}

	void CGamePlayer::resetJump()
	{
		jumpState = 0;
		jumpDistance = 0;
		currentJumpDistance = 0;
		currentFallingSpeed = 2.7f;
		nextFallFrameID = 0;
	}

	int CGamePlayer::getMoveSpeed()
	{
		return moveSpeed;
	}

	bool CGamePlayer::getMove()
	{
		return bMove;
	}

	bool CGamePlayer::getMoveDirection()
	{
		return moveDirection;
	}

	void CGamePlayer::setCurrentJumpSpeed(float currentJumpSpeed)
	{
		currentJumpSpeed = currentJumpSpeed;
	}

	void CGamePlayer::setMoveSpeed(int moveSpeed)
	{
		moveSpeed = moveSpeed;
	}

	void CGamePlayer::setNextFallFrameID(int nextFallFrameID)
	{
		this->nextFallFrameID = nextFallFrameID;
	}

	int CGamePlayer::getJumpState()
	{
		return jumpState;
	}

	void CGamePlayer::TimeTick()
	{
		iTimeTick++;
	}

	int CGamePlayer::GetTimeTick()
	{
		return iTimeTick;
	}

	void CGamePlayer::movePlayer(CGameMap *map)
	{
		if (bMove && !changeMoveDirection) 
		{
			if (moveSpeed > currentMaxMove)
				--moveSpeed;
			else if (iTimeTick - (6 + moveSpeed) >= iTimePassed && moveSpeed < currentMaxMove) 
			{
				++moveSpeed;
				iTimePassed = iTimeTick;
			}
			else if (moveSpeed == 0)
				moveSpeed = 1;
		}
		else 
		{
			if (iTimeTick - 2 + (currentMaxMove - moveSpeed) > iTimePassed && moveSpeed != 0)
			{
				--moveSpeed;
				iTimePassed = iTimeTick;
				/*if (jumpState == 0 ) 
					setMarioSpriteID(6);*/
			}

			if (changeMoveDirection && moveSpeed <= 1) 
			{
				moveDirection = newMoveDirection;
				changeMoveDirection = false;
				bMove = true;
			}
		}

		if (moveSpeed > 0)
		{
			if (moveDirection)
				updateXPos(moveSpeed, map);
			else
				updateXPos(-moveSpeed, map);
		}
	}

	bool CGamePlayer::checkCollisionBot(int nX, int nY, CGameMap *map)
	{
		Vector2* vLT = getBlockLB(fXPos - map->getXPos() + nX, fYPos + nY, map);

		if (map->getBlock(map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->IsUse()) 
			map->blockUse(vLT->getX(), vLT->getY(), map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 1);

		delete vLT;

		if (fXPos - map->getXPos() + nX / BLOCK_WIDTH_HEIGHT < map->getMapWidth())
		{
			vLT = getBlockRB(fXPos - map->getXPos() + nX, fYPos + nY, map);

			if (map->getBlock(map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->IsUse())
				map->blockUse(vLT->getX(), vLT->getY(), map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 1);

			delete vLT;
		}
		return true;
	}

	bool CGamePlayer::checkCollisionCenter(int nX, int nY, CGameMap *map)
	{
		Vector2* vLT = getBlockLC(fXPos - map->getXPos() + nX, fYPos + nY, map);

		if (map->getBlock(map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->IsUse())
			map->blockUse(vLT->getX(), vLT->getY(), map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);

		delete vLT;

		vLT = getBlockRC(fXPos - map->getXPos() + nX, fYPos + nY, map);

		if (map->getBlock(map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID())->IsUse())
			map->blockUse(vLT->getX(), vLT->getY(), map->getMapBlock(vLT->getX(), vLT->getY())->getBlockID(), 2);

		delete vLT;

		return true;
	}

	Vector2 * CGamePlayer::getBlockLB(float nX, float nY, CGameMap *map)
	{
		return map->getBlockID((int)nX + 1, (int)nY + Height() + 2);
	}

	Vector2 * CGamePlayer::getBlockRB(float nX, float nY, CGameMap *map)
	{
		return map->getBlockID((int)nX + Width() - 1, (int)nY + Height() + 2);
	}

	Vector2 * CGamePlayer::getBlockLC(float nX, float nY, CGameMap *map)
	{
		return map->getBlockID((int)nX - 1, (int)nY + Height() / 2);
	}

	Vector2 * CGamePlayer::getBlockRC(float nX, float nY, CGameMap *map)
	{
		return map->getBlockID((int)nX + Width() + 1, (int)nY + Height() / 2);
	}

	Vector2 * CGamePlayer::getBlockLT(float nX, float nY, CGameMap *map)
	{
		return map->getBlockID((int)nX + 1, (int)nY);
	}

	Vector2 * CGamePlayer::getBlockRT(float nX, float nY, CGameMap *map)
	{
		return map->getBlockID((int)nX + Width() - 1, (int)nY);
	}
}