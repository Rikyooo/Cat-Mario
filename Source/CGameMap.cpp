#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameMap.h"
#include "CGamePlayer.h"
#include "CGameMinion.h"
#include "CGameGoombas.h"
#include "CGameKoppa.h"
#include "CGameMushroom.h"
#include "CGameStar.h"
#include "CGameCoin.h"
#include "CGameBlockDebris.h"

namespace game_framework
{
CGameMap::CGameMap()
{
    oPlayer = new CGamePlayer(6, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 37);
    this->currentLevelID = 0;
    this->iMapWidth = 0;
    this->iMapHeight = 0;
    this->iLevelType = 0;
    this->fXPos = 0;
    this->fYPos = 0;
    this->iSpawnPointID = 0;
    this->bMoveMap = true;
	this->isPlayerDeath = false;
    loadGameData();
    loadLVL();
}

CGameMap::~CGameMap(void)
{
    for (vector<CGameBlock*>::iterator i = vBlock.begin(); i != vBlock.end(); i++)
        delete (*i);

    for (vector<CGameBlock*>::iterator i = vMinion.begin(); i != vMinion.end(); i++)
        delete (*i);
}

void CGameMap::LoadBitmap()
{
    for (int i = 0; i < iBlockSize; i++)
        vBlock[i]->LoadBitmap();

	for (int i = 0; i < iMinionSize; i++)
		vMinion[i]->LoadBitmap();

    //oPlayer->LoadBitmap();
}

void CGameMap::Update()
{
    /*if (!oPlayer->getInLevelAnimation())
    {
    	*/
    //UpdateMinionBlokcs();
    UpdateMinions();
    UpdatePlayer();

    for (unsigned int i = 0; i < lBlockDebris.size(); i++)
        if (lBlockDebris[i]->getDebrisState() != -1)
            lBlockDebris[i]->Update();
        else
        {
            delete lBlockDebris[i];
            lBlockDebris.erase(lBlockDebris.begin() + i);
        }

    for (unsigned int i = 0; i < lCoin.size(); i++)
        if (!lCoin[i]->getDelete())
            lCoin[i]->Update();
        else
        {
            delete lCoin[i];
            lCoin.erase(lCoin.begin() + i);
        }
}

void CGameMap::UpdatePlayer()
{
    oPlayer->Update(this);
    //checkSpawnPoint();
}

void CGameMap::UpdateMinions()
{
    for (int i = 0; i < iMinionListSize; i++)
        for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
            if (lMinion[i][j]->updateMinion(this))
                lMinion[i][j]->Update(this);

    // ----- UPDATE MINION LIST ID
    for (int i = 0; i < iMinionListSize; i++)
    {
        for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
            if (lMinion[i][j]->getMinionSpawned())
            {
                if (lMinion[i][j]->getMinionState() == -1)
                {
                    delete lMinion[i][j];
                    lMinion[i].erase(lMinion[i].begin() + j);
                    jSize = lMinion[i].size();
                    continue;
                }

                if (floor(lMinion[i][j]->getXPos() / 160) != i)
                {
                    lMinion[(int)floor((int)lMinion[i][j]->getXPos() / 160)].push_back(lMinion[i][j]);
                    lMinion[i].erase(lMinion[i].begin() + j);
                    jSize = lMinion[i].size();
                }
            }
    }
}

void CGameMap::UpdateMinionsCollisions()
{  
    // ----- COLLISIONS
    for (int i = 0; i < iMinionListSize; i++)
    {
        for (unsigned int j = 0; j < lMinion[i].size(); j++)
        {
            if (!lMinion[i][j]->getCollisionOnlyWithPlayer()&& lMinion[i][j]->getDeadTime() < 0)
            {
                // ----- WITH MINIONS IN SAME LIST
                for (unsigned int k = j + 1; k < lMinion[i].size(); k++)
                    if (!lMinion[i][k]->getCollisionOnlyWithPlayer() && lMinion[i][k]->getDeadTime() < 0)
                        if (lMinion[i][j]->getXPos() < lMinion[i][k]->getXPos())
                            if (lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() >= lMinion[i][k]->getXPos() && lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() <= lMinion[i][k]->getXPos() + lMinion[i][k]->getHitBoxX() && ((lMinion[i][j]->getYPos() <= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY() && lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() >= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY()) || (lMinion[i][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY() >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())))
                            {
                                if (lMinion[i][j]->getKillOtherUnits() && lMinion[i][j]->getMoveSpeed() > 0 && lMinion[i][k]->getMinionSpawned())
                                {
                                    lMinion[i][k]->setMinionState(-2);
                                    lMinion[i][j]->collisionWithAnotherUnit();
                                }

                                if (lMinion[i][k]->getKillOtherUnits() && lMinion[i][k]->getMoveSpeed() > 0 && lMinion[i][j]->getMinionSpawned())
                                {
                                    lMinion[i][j]->setMinionState(-2);
                                    lMinion[i][k]->collisionWithAnotherUnit();
                                }

                                if (lMinion[i][j]->getYPos() - 4 <= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY() && lMinion[i][j]->getYPos() + 4 >= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY())
                                    lMinion[i][k]->setOnAnotherMinion(true);
                                else if (lMinion[i][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())
                                    lMinion[i][j]->setOnAnotherMinion(true);
                                else
                                {
                                    lMinion[i][j]->collisionEffect();
                                    lMinion[i][k]->collisionEffect();
                                }
                            }
                            else if (lMinion[i][k]->getXPos() + lMinion[i][k]->getHitBoxX() >= lMinion[i][j]->getXPos() && lMinion[i][k]->getXPos() + lMinion[i][k]->getHitBoxX() <= lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() && ((lMinion[i][j]->getYPos() <= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY() && lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() >= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY()) || (lMinion[i][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY() >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())))
                            {
                                if (lMinion[i][j]->getKillOtherUnits() && lMinion[i][j]->getMoveSpeed() > 0 && lMinion[i][k]->getMinionSpawned())
                                {
                                    lMinion[i][k]->setMinionState(-2);
                                    lMinion[i][j]->collisionWithAnotherUnit();
                                }

                                if (lMinion[i][k]->getKillOtherUnits() && lMinion[i][k]->getMoveSpeed() > 0 && lMinion[i][j]->getMinionSpawned())
                                {
                                    lMinion[i][j]->setMinionState(-2);
                                    lMinion[i][k]->collisionWithAnotherUnit();
                                }

                                if (lMinion[i][j]->getYPos() - 4 <= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY() && lMinion[i][j]->getYPos() + 4 >= lMinion[i][k]->getYPos() + lMinion[i][k]->getHitBoxY())
                                    lMinion[i][k]->setOnAnotherMinion(true);
                                else if (lMinion[i][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())
                                    lMinion[i][j]->setOnAnotherMinion(true);
                                else
                                {
                                    lMinion[i][j]->collisionEffect();
                                    lMinion[i][k]->collisionEffect();
                                }
                            }

                // ----- WITH MINIONS IN OTHER LIST
                if (i + 1 < iMinionListSize)
                {
                    for (unsigned int k = 0; k < lMinion[i + 1].size(); k++)
                    {
                        if (!lMinion[i + 1][k]->getCollisionOnlyWithPlayer() /*&& lMinion[i + 1][k]->minionSpawned*/ && lMinion[i + 1][k]->getDeadTime() < 0)
                        {
                            if (lMinion[i][j]->getXPos() < lMinion[i + 1][k]->getXPos())
                            {
                                if (lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() >= lMinion[i + 1][k]->getXPos() && lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() <= lMinion[i + 1][k]->getXPos() + lMinion[i + 1][k]->getHitBoxX() && ((lMinion[i][j]->getYPos() <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY() && lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY()) || (lMinion[i + 1][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY() >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())))
                                {
                                    if (lMinion[i][j]->getKillOtherUnits() && lMinion[i][j]->getMoveSpeed() > 0 && lMinion[i + 1][k]->getMinionSpawned())
                                    {
                                        lMinion[i + 1][k]->setMinionState(-2);
                                        lMinion[i][j]->collisionWithAnotherUnit();
                                    }

                                    if (lMinion[i + 1][k]->getKillOtherUnits() && lMinion[i + 1][k]->getMoveSpeed() > 0 && lMinion[i][j]->getMinionSpawned())
                                    {
                                        lMinion[i][j]->setMinionState(-2);
                                        lMinion[i + 1][k]->collisionWithAnotherUnit();
                                    }

                                    if (lMinion[i][j]->getYPos() - 4 <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY() && lMinion[i][j]->getYPos() + 4 >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY())
                                    {
                                        lMinion[i + 1][k]->setOnAnotherMinion(true);
                                    }
                                    else if (lMinion[i + 1][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i + 1][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())
                                    {
                                        lMinion[i][j]->setOnAnotherMinion(true);
                                    }
                                    else
                                    {
                                        lMinion[i][j]->collisionEffect();
                                        lMinion[i + 1][k]->collisionEffect();
                                    }
                                }
                            }
                            else
                            {
                                if (lMinion[i + 1][k]->getXPos() + lMinion[i + 1][k]->getHitBoxX() >= lMinion[i][j]->getXPos() && lMinion[i + 1][k]->getXPos() + lMinion[i + 1][k]->getHitBoxX() < lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() && ((lMinion[i][j]->getYPos() <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY() && lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY()) || (lMinion[i + 1][k]->getYPos() <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY() >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())))
                                {
                                    if (lMinion[i][j]->getKillOtherUnits() && lMinion[i][j]->getMoveSpeed() > 0 && lMinion[i + 1][k]->getMinionSpawned())
                                    {
                                        lMinion[i + 1][k]->setMinionState(-2);
                                        lMinion[i][j]->collisionWithAnotherUnit();
                                    }

                                    if (lMinion[i + 1][k]->getKillOtherUnits() && lMinion[i + 1][k]->getMoveSpeed() > 0 && lMinion[i][j]->getMinionSpawned())
                                    {
                                        lMinion[i][j]->setMinionState(-2);
                                        lMinion[i + 1][k]->collisionWithAnotherUnit();
                                    }

                                    if (lMinion[i][j]->getYPos() - 4 <= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY() && lMinion[i][j]->getYPos() + 4 >= lMinion[i + 1][k]->getYPos() + lMinion[i + 1][k]->getHitBoxY())
                                    {
                                        lMinion[i + 1][k]->setOnAnotherMinion(true);
                                    }
                                    else if (lMinion[i + 1][k]->getYPos() - 4 <= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() && lMinion[i + 1][k]->getYPos() + 4 >= lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY())
                                    {
                                        lMinion[i][j]->setOnAnotherMinion(true);
                                    }
                                    else
                                    {
                                        lMinion[i][j]->collisionEffect();
                                        lMinion[i + 1][k]->collisionEffect();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //if (!inEvent && !oPlayer->getInLevelAnimation())
    // ----- COLLISION WITH PLAYER
    for (int i = getListID(-(int)fXPos + oPlayer->GetX()) - (getListID(-(int)fXPos + oPlayer->GetX()) > 0 ? 1 : 0), iSize = i + 2; i < iSize; i++) 
	{
		for (unsigned int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
		{
			if (lMinion[i][j]->getDeadTime() < 0)
			{
				if ((oPlayer->GetX() - fXPos >= lMinion[i][j]->getXPos() && oPlayer->GetX() - fXPos <= lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX()) || (oPlayer->GetX() - fXPos + oPlayer->GetHitBoxX() >= lMinion[i][j]->getXPos() && oPlayer->GetX() - fXPos + oPlayer->GetHitBoxX() <= lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX())) 
				{
					if (lMinion[i][j]->getYPos() - 2 <= oPlayer->GetY() + oPlayer->GetHitBoxY() && lMinion[i][j]->getYPos() + 16 >= oPlayer->GetY() + oPlayer->GetHitBoxY()) {
						lMinion[i][j]->collisionWithPlayer(true, this);
					}
					else if ((lMinion[i][j]->getYPos() <= oPlayer->GetY() + oPlayer->GetHitBoxY() && lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() >= oPlayer->GetY() + oPlayer->GetHitBoxY()) || (lMinion[i][j]->getYPos() - (oPlayer->GetHitBoxY() - lMinion[i][j]->getHitBoxY()) <= oPlayer->GetY() && lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() >= oPlayer->GetY())) {
						lMinion[i][j]->collisionWithPlayer(false, this);
					}
				}
			}
		}
	}
}

//void CGameMap::UpdateMinionBlokcs()
//{
//	vMinion[0]->getSprite()->Update();
//	vMinion[4]->getSprite()->Update();
//	vMinion[6]->getSprite()->Update();
//	vMinion[7]->getSprite()->Update();
//	vMinion[8]->getSprite()->Update();
//	vMinion[10]->getSprite()->Update();
//	vMinion[12]->getSprite()->Update();
//	vMinion[14]->getSprite()->Update();
//	vMinion[15]->getSprite()->Update();
//	vMinion[17]->getSprite()->Update();
//	vMinion[18]->getSprite()->Update();
//	vMinion[19]->getSprite()->Update();
//	vMinion[20]->getSprite()->Update();
//	vMinion[21]->getSprite()->Update();
//	vMinion[22]->getSprite()->Update();
//	vMinion[23]->getSprite()->Update();
//	vMinion[24]->getSprite()->Update();
//	vMinion[30]->getSprite()->Update();
//	vMinion[31]->getSprite()->Update();
//	vMinion[43]->getSprite()->Update();
//	vMinion[44]->getSprite()->Update();
//	vMinion[45]->getSprite()->Update();
//	vMinion[46]->getSprite()->Update();
//	vMinion[47]->getSprite()->Update();
//	vMinion[48]->getSprite()->Update();
//	vMinion[51]->getSprite()->Update();
//	vMinion[52]->getSprite()->Update();
//	vMinion[53]->getSprite()->Update();
//	vMinion[55]->getSprite()->Update();
//	vMinion[57]->getSprite()->Update();
//	vMinion[62]->getSprite()->Update();
//}

void CGameMap::Draw()
{
	DrawMap();
	DrawMinions();
    
	for (unsigned int i = 0; i < lCoin.size(); i++)
		lCoin[i]->Draw(this);

	for (unsigned int i = 0; i < lBlockDebris.size(); i++)
		lBlockDebris[i]->Draw(this);

    oPlayer->Draw();
}

void CGameMap::DrawMap()
{
    for (int i = getStartBlock(), iEnd = getEndBlock(); i < iEnd && i < iMapWidth; i++)
        for (int j = 0; j < iMapHeight; j++)
            if (lMap[i][j]->getBlockID() != 0)
            {
                //TRACE("i:%d j:%d ID:%d\n", i, j, lMap[i][j]->getBlockID());
                vBlock[lMap[i][j]->getBlockID()]->OnDraw(32 * i + (int)fXPos, SIZE_Y - BLOCK_WIDTH_HEIGHT * (j + 1) - lMap[i][j]->updateYPos(), false);
            }
}

void CGameMap::DrawMinions()
{
    for (int i = 0; i < iMinionListSize; i++)
    {
        for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
        {
            lMinion[i][j]->Draw(this, lMinion[i][j]->getBlockID());
            //CCFG::getText()->DrawWS(rR, std::to_string(i), lMinion[i][j]->getXPos() + (int)fXPos, lMinion[i][j]->getYPos(), 0, 0, 0, 8);
        }
    }
}

void CGameMap::moveMap(int nX, int nY)
{
    if (fXPos + nX > 0)
    {
        oPlayer->updateXPos((int)(nX - fXPos), this);
        fXPos = 0;
    }
    else
    {
        this->fXPos += nX;
    }

    if (SIZE_X - fXPos > iMapWidth * BLOCK_WIDTH_HEIGHT)
        fXPos = (float)(SIZE_X - iMapWidth * BLOCK_WIDTH_HEIGHT);
}

//void CGameMap::setSpawnPoint()
//{
//    float X = (float)getSpawnPointXPos(iSpawnPointID);
//
//    if (X > 6 * 32)
//    {
//        fXPos = -(X - 6 * 32);
//        oPlayer->SetX(6 * 32);
//        oPlayer->SetY((float)getSpawnPointYPos(iSpawnPointID));
//    }
//    else
//    {
//        fXPos = 0;
//        oPlayer->SetX(X);
//        oPlayer->SetY((float)getSpawnPointYPos(iSpawnPointID));
//    }
//
//    oPlayer->setMoveDirection(true);
//}

bool CGameMap::blockUse(int nX, int nY, int iBlockID, int POS)
{
    if (POS == 0)
        switch (iBlockID)
        {
        case 9: case 55: // ----- BlockQ
            if (lMap[nX][nY]->getSpawnMushroom())
            {
                if (lMap[nX][nY]->getPowerUP())
                    lMinion[getListID(32 * nX)].push_back(new CGameMushroom(32 * nX, SIZE_Y - 32 * (nY + 1), true, nX, nY));
                else
                    lMinion[getListID(32 * nX)].push_back(new CGameMushroom(32 * nX, SIZE_Y - 32 - 32 * (nY + 1), false, nX, nY));

				CAudio::Instance()->Play(AUDIO_BLOCK_USE);
            }
            else
            {
                lCoin.push_back(new CGameCoin(nX * 32 + 7, SIZE_Y - nY * 32 - 48));
				CAudio::Instance()->Play(AUDIO_BLOCK_COIN);
            }

            if (lMap[nX][nY]->getNumOfUse() > 1)
                lMap[nX][nY]->setNumOfUse(lMap[nX][nY]->getNumOfUse() - 1);
            else
            {
                lMap[nX][nY]->setNumOfUse(0);
                lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 10 : iLevelType == 1 ? 56 : 56);
            }

            //lMap[nX][nY]->startBlockAnimation();
            checkCollisionOnTopOfTheBlock(nX, nY);
            break;

        case 13:
        case 28:
        case 81: // ----- Brick
            if (lMap[nX][nY]->getSpawnStar())
            {
                lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 10 : iLevelType == 1 ? 56 : 80);
                lMinion[getListID(32 * nX)].push_back(new CGameStar(32 * nX, SIZE_Y - 32 * (nY + 1), nX, nY));
                //lMap[nX][nY]->startBlockAnimation();
				CAudio::Instance()->Play(AUDIO_BLOCK_USE);
            }
            else if (lMap[nX][nY]->getSpawnMushroom())
            {
                lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 10 : iLevelType == 1 ? 56 : 80);

				if (lMap[nX][nY]->getPowerUP())
					lMinion[getListID(32 * nX)].push_back(new CGameMushroom(32 * nX, SIZE_Y - 32 * (nY + 1), true, nX, nY));
				else
					lMinion[getListID(32 * nX)].push_back(new CGameMushroom(32 * nX, SIZE_Y - 32 - 32 * (nY + 1), false, nX, nY));

                //lMap[nX][nY]->startBlockAnimation();
				CAudio::Instance()->Play(AUDIO_BLOCK_USE);
            }
            else if (lMap[nX][nY]->getNumOfUse() > 0)
            {
                lCoin.push_back(new CGameCoin(nX * 32 + 7, SIZE_Y - nY * 32 - 48));
                lMap[nX][nY]->setNumOfUse(lMap[nX][nY]->getNumOfUse() - 1);

                if (lMap[nX][nY]->getNumOfUse() == 0)
                    lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 10 : iLevelType == 1 ? 56 : 80);

				CAudio::Instance()->Play(AUDIO_BLOCK_COIN);
                //lMap[nX][nY]->startBlockAnimation();
            }
            else
            {
                lMap[nX][nY]->setBlockID(0);
                lBlockDebris.push_back(new CGameBlockDebris(nX * 32, SIZE_Y - 48 - nY * 32));
				CAudio::Instance()->Play(AUDIO_BLOCK_BREAK);
            }

            //else
            //{
            //	lMap[nX][nY]->startBlockAnimation();
            //	//CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBLOCKHIT);
            //}
            checkCollisionOnTopOfTheBlock(nX, nY);
            break;

        case 21: // ----- BlockQ2
            if (lMap[nX][nY]->getSpawnMushroom())
            {
				if (lMap[nX][nY]->getPowerUP())
					lMinion[getListID(32 * nX)].push_back(new CGameMushroom(32 * nX, SIZE_Y - 32 * (nY + 1), true, nX, nY));
				else
					lMinion[getListID(32 * nX)].push_back(new CGameMushroom(32 * nX, SIZE_Y - 32 - 32 * (nY + 1), false, nX, nY));

				CAudio::Instance()->Play(AUDIO_BLOCK_USE);
            }
            else
            {
                lCoin.push_back(new CGameCoin(nX * 32 + 7, SIZE_Y - nY * 32 - 48));
				CAudio::Instance()->Play(AUDIO_BLOCK_COIN);
                //lMap[nX][nY]->startBlockAnimation();
            }

            lMap[nX][nY]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 10 : iLevelType == 1 ? 56 : 80);
            checkCollisionOnTopOfTheBlock(nX, nY);
            break;

        /*case 128: case 129:
        	spawnVine(nX, nY, iBlockID);
        	lMap[nX][nY]->setBlockID(iBlockID == 128 ? 9 : 56);
        	lMap[nX][nY]->startBlockAnimation();
        	break;*/
        default:
            break;
        }

    //else if (POS == 1)
    //	switch (iBlockID)
    //	{
    //	case 21: case 23: case 31: case 33: case 98: case 100: case 113: case 115: case 137: case 139: case 177: case 179: // Pipe
    //		pipeUse();
    //		break;
    //	case 40: case 41: case 123: case 124: case 182: // End
    //		EndUse();
    //		break;
    //	case 82:
    //		EndBoss();
    //		break;
    //	default:
    //		break;
    //	}

    switch (iBlockID)
    {
    case 29:
    case 71:
    case 72:
    case 73:// COIN
        lMap[nX][nY]->setBlockID(iLevelType == 2 ? 94 : 0);
		CAudio::Instance()->Play(AUDIO_BLOCK_COIN);
        return false;
        break;

    //case 36: case 38: case 60: case 62: case 103: case 105: case 118: case 120: // Pipe
    //	pipeUse();
    //	break;
    //case 127: // BONUS END
    //	EndBonus();
    //	break;
    //case 169:
    //	TPUse();
    //	break;
    //case 170:
    //	TPUse2();
    //	break;
    //case 171:
    //	TPUse3();
    //	break;
    default:
        break;
    }

    return true;
}

void CGameMap::addGoombas(int iX, int iY, bool moveDirection)
{
	lMinion[getListID(iX)].push_back(new CGameGoombas(iX, iY, iLevelType == 0 || iLevelType == 4 ? 0 : iLevelType == 1 ? 8 : 10, moveDirection));
}

void CGameMap::addKoppa(int iX, int iY, int minionState, bool moveDirection)
{
	int tempBlock;

	switch (minionState) {
	/*case 0: case 3:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 7 : iLevelType == 1 ? 14 : 17;
		break;*/
	case 1:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 7 : iLevelType == 1 ? 12 : 15;
		break;
	case 2:
		tempBlock = iLevelType == 0 || iLevelType == 4 ? 6 : iLevelType == 1 ? 13 : 16;
		break;
	}

	lMinion[getListID(iX)].push_back(new CGameKoppa(iX, iY, minionState, moveDirection, tempBlock));
}

Vector2* CGameMap::getBlockID(int nX, int nY)
{
    //return new Vector2((int)(nX < 0 ? 0 : nX + BLOCK_WIDTH_HEIGHT) / BLOCK_WIDTH_HEIGHT, (int)(nY > SIZE_Y ? 0 : (SIZE_Y - nY + BLOCK_WIDTH_HEIGHT) / BLOCK_WIDTH_HEIGHT));
    return new Vector2((int)(nX < 0 ? 0 : nX) / BLOCK_WIDTH_HEIGHT, (int)(nY > SIZE_Y ? 0 : (SIZE_Y - nY)) / BLOCK_WIDTH_HEIGHT);
}

int CGameMap::getBlockIDX(int nX)
{
    return (int)(nX < 0 ? 0 : nX) / BLOCK_WIDTH_HEIGHT;
}

int CGameMap::getBlockIDY(int nY)
{
    return (int)(nY > SIZE_Y ? 0 : (SIZE_Y - nY)) / BLOCK_WIDTH_HEIGHT;
}

bool CGameMap::checkCollisionLB(int nX, int nY, int nHitBoxY, bool checkVisible)
{
    return checkCollision(getBlockID(nX, nY + nHitBoxY), checkVisible);
}

bool CGameMap::checkCollisionLT(int nX, int nY, bool checkVisible)
{
    return checkCollision(getBlockID(nX, nY), checkVisible);
}

bool CGameMap::checkCollisionLC(int nX, int nY, int nHitBoxY, bool checkVisible)
{
    return checkCollision(getBlockID(nX, nY + nHitBoxY), checkVisible);
}

bool CGameMap::checkCollisionRC(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible)
{
    return checkCollision(getBlockID(nX + nHitBoxX, nY + nHitBoxY), checkVisible);
}

bool CGameMap::checkCollisionRB(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible)
{
    return checkCollision(getBlockID(nX + nHitBoxX, nY + nHitBoxY), checkVisible);
}

bool CGameMap::checkCollisionRT(int nX, int nY, int nHitBoxX, bool checkVisible)
{
    return checkCollision(getBlockID(nX + nHitBoxX, nY), checkVisible);
}

bool CGameMap::checkCollision(Vector2* nV, bool checkVisible)
{
    bool output = vBlock[lMap[nV->getX()][nV->getY()]->getBlockID()]->IsCollision() && (checkVisible ? vBlock[lMap[nV->getX()][nV->getY()]->getBlockID()]->IsVisible() : true);
    delete nV;
    return output;
}

void CGameMap::checkCollisionOnTopOfTheBlock(int nX, int nY)
{
    switch (lMap[nX][nY + 1]->getBlockID())
    {
    case 29:
    case 71:
    case 72:
    case 73:// COIN
        lMap[nX][nY + 1]->setBlockID(0);
        lCoin.push_back(new CGameCoin(nX * 32 + 7, SIZE_Y - nY * 32 - 48));
        //CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cCOIN);
        return;
        break;
    }

    for (int i = (nX - nX % 5) / 5, iEnd = i + 3; i < iEnd && i < iMinionListSize; i++)
        for (unsigned int j = 0; j < lMinion[i].size(); j++)
            if (!lMinion[i][j]->getCollisionOnlyWithPlayer() && lMinion[i][j]->getMinionState() >= 0 && ((lMinion[i][j]->getXPos() >= nX * 32 && lMinion[i][j]->getXPos() <= nX * 32 + 32) || (lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() >= nX * 32 && lMinion[i][j]->getXPos() + lMinion[i][j]->getHitBoxX() <= nX * 32 + 32)))
                if (lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() >= SIZE_Y - 24 - nY * 32 && lMinion[i][j]->getYPos() + lMinion[i][j]->getHitBoxY() <= SIZE_Y - nY * 32 + 16)
                {
                    lMinion[i][j]->setMoveDirection(!lMinion[i][j]->getMoveDirection());
                    lMinion[i][j]->setMinionState(-2);
                }
}

void CGameMap::loadLVL_1_1()
{
    clearMap();
    this->iMapWidth = 141;
    this->iMapHeight = 17;
    // ---------- LOAD LISTS ----------
    createMap();
    // ----- MINIONS
    loadMinionsLVL_1_1();
    // ----- PIPEEVENT
    //loadPipeEventsLVL_1_1();
    // ----- Bush -----
    structBush(1, 2, 2);
    structBush(33, 2, 2);
    // ----- Clouds -----
    structCloud(6, 12);
    structCloud(23, 12);
    structCloud2(65, 13);
    structCloud2(103, 9);
    // ----- Grass -----
    structGrass(19, 2, 1);
    structGrass(26, 2, 1);
    structGrass(46, 2, 1);
    structGrass(123, 2, 1);
    // ----- GND -----
    structGround(0, 0, 40, 2);
    structGround(43, 0, 11, 2);
    structGround(57, 0, 28, 2);
    structGround(88, 0, 6, 2);
    structGround(95, 0, 42, 2);
    structGround(138, 0, 3, 2);
    // ----- GND 2 -----
    structGND2(82, 2, 3, true);
    structGND2(88, 2, 1, 3);
    structGND2(89, 2, 1, 2);
    structGND2(107, 2, 8, true);
    structGND2(115, 2, 1, 8);
    structGND2(122, 2, 1, 1);
	structGND2(140, 2, 1, 13);
    // ----- BRICK -----
    struckBlockQ(8, 5, 1);
    structBrick(12, 5, 1, 1);
    struckBlockQ(13, 5, 1);
	lMap[13][5]->setSpawnMushroom(true);
	struckBlockQ2(13, 4, 1);
    structBrick(14, 5, 1, 1);
    struckBlockQ(14, 9, 1);
    lMap[14][9]->setSpawnMushroom(true);
    struckBlockQ(15, 5, 1);
    structBrick(16, 5, 1, 1);
    struckBlockQ2(40, 4, 1);
    structBrick(46, 5, 1, 1);
    struckBlockQ(47, 5, 1);
    lMap[47][5]->setSpawnMushroom(true);
    structBrick(48, 5, 1, 1);
    structBrick(49, 9, 5, 1);
    structBrick(57, 9, 3, 1);
    structBrick(59, 5, 1, 1);
	lMap[59][5]->setNumOfUse(6);
    structBrick(66, 5, 2, 1);
	lMap[67][5]->setSpawnStar(true);
    struckBlockQ(71, 5, 1);
    struckBlockQ(74, 5, 1);
    struckBlockQ(74, 9, 1);
    struckBlockQ(77, 5, 1);
    struckBlockQ2(89, 8, 1);
	struckBlockQ2(90, 4, 5);
    structBrick(98, 5, 2, 1);
    struckBlockQ(100, 5, 1);
    structBrick(101, 5, 1, 1);
    // ----- PIPES -----
    //structPipe(28, 2, 1);
    // ----- END
    //structEnd(127, 3, 9);
    //structCastleSmall(202, 2);
    this->iLevelType = 0;
}

void CGameMap::loadMinionsLVL_1_1()
{
	clearMinions();

	addGoombas(12 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(17 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(20 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(48 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 6 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(53 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 10 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(60 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(62 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(100 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 29, true);
	addGoombas(102 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 29, true);

	addKoppa(67 * BLOCK_WIDTH_HEIGHT, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 41, 1, true);

}

void CGameMap::clearMap()
{
    for (int i = 0; i < iMapWidth; i++)
    {
        for (int j = 0; j < iMapHeight; j++)
            delete lMap[i][j];

        lMap[i].clear();
    }

    lMap.clear();
    iMapWidth = iMapHeight = 0;
    /*if (oFlag != NULL) {
    	delete oFlag;
    	oFlag = NULL;
    }*/
    /*oEvent->eventTypeID = oEvent->eNormal;*/
}

void CGameMap::clearMinions()
{
    for (int i = 0; i < iMinionListSize; i++)
    {
        for (int j = 0, jSize = lMinion[i].size(); j < jSize; j++)
        {
            delete lMinion[i][j];
            jSize = lMinion[i].size();
        }

        lMinion[i].clear();
    }

    //clearPlatforms();
}

void CGameMap::resetGameData()
{
    this->currentLevelID = 0;
    this->iSpawnPointID = 0;
    /*oPlayer->resetPowerLVL();*/
    /*setSpawnPoint();*/
    loadLVL();
}

void CGameMap::loadLVL()
{
    switch (currentLevelID)
    {
    case 0:
        loadLVL_1_1();
        break;

    default:
        break;
    }
}

void CGameMap::setBackgroundColor()
{
    /*switch (iLevelType) {
    case 0: case 2:
    	SDL_SetRenderDrawColor(rR, 93, 148, 252, 255);
    	break;
    case 1: case 3: case 4:
    	SDL_SetRenderDrawColor(rR, 0, 0, 0, 255);
    	break;
    default:
    	SDL_SetRenderDrawColor(rR, 93, 148, 252, 255);
    	break;
    }*/
}

void CGameMap::structBush(int X, int Y, int iSize)
{
    // ----- LEFT & RIGHT
    for (int i = 0; i < iSize; i++)
    {
        lMap[X + i][Y + i]->setBlockID(4);
        lMap[X + iSize + 1 + i][Y + iSize - 1 - i]->setBlockID(5);
    }

    // ----- CENTER LEFT & RIGHT
    for (int i = 0, k = 1; i < iSize - 1; i++)
    {
        for (int j = 0; j < k; j++)
        {
            lMap[X + 1 + i][Y + j]->setBlockID((i % 2 == 0 ? 8 : 7));
            lMap[X + iSize * 2 - 1 - i][Y + j]->setBlockID((i % 2 == 0 ? 8 : 7));
        }

        ++k;
    }

    // ----- CENTER
    for (int i = 0; i < iSize; i++)
    {
        lMap[X + iSize][Y + i]->setBlockID((i % 2 == 0 && iSize != 1 ? 7 : 8));
    }

    // ----- TOP
    lMap[X + iSize][Y + iSize]->setBlockID(6);
}

void CGameMap::structGrass(int X, int Y, int iSize)
{
    for (int i = 0; i < iSize; i++)
    {
        lMap[X + 2 * i][Y]->setBlockID(11);
        lMap[X + 2 * i + 1][Y]->setBlockID(12);
    }
}

void CGameMap::structCloud(int X, int Y)
{
    lMap[X][Y]->setBlockID(18);
    lMap[X - 1][Y]->setBlockID(14);
    lMap[X + 1][Y]->setBlockID(15);
    lMap[X][Y + 1]->setBlockID(16);
    lMap[X][Y - 1]->setBlockID(17);
}

void CGameMap::structCloud2(int X, int Y)
{
    lMap[X][Y]->setBlockID(19);
    lMap[X - 1][Y]->setBlockID(14);
    lMap[X + 1][Y]->setBlockID(15);
    lMap[X][Y + 1]->setBlockID(16);
    lMap[X][Y - 1]->setBlockID(17);
}

void CGameMap::structGround(int X, int Y, int iWidth, int iHeight)
{
    for (int i = 0; i < iWidth; i++)
        for (int j = 0; j < iHeight; j++)
        {
            if (j == iHeight - 1)
                lMap[X + i][Y + j]->setBlockID(1);
            //lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 1 : iLevelType == 1 ? 26 : iLevelType == 2 ? 92 : iLevelType == 6 ? 166 : iLevelType == 7 ? 181 : 75);
            else
                lMap[X + i][Y + j]->setBlockID(2);
        }
}

void CGameMap::structGND2(int X, int Y, int iSize, bool bDir)
{
    if (bDir)
        for (int i = 0, k = 1; i < iSize; i++)
        {
            for (int j = 0; j < k; j++)
                lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 20 : iLevelType == 3 ? 167 : 27);

            ++k;
        }
    else
        for (int i = 0, k = 1; i < iSize; i++)
        {
            for (int j = 0; j < k; j++)
                lMap[X + iSize - 1 - i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 20 : iLevelType == 3 ? 167 : 27);

            ++k;
        }
}

void CGameMap::structGND2(int X, int Y, int iWidth, int iHeight)
{
    for (int i = 0; i < iWidth; i++)
        for (int j = 0; j < iHeight; j++)
            lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 20 : iLevelType == 3 ? 167 : 27);
}

void CGameMap::structCoins(int X, int Y, int iWidth, int iHeight)
{
    for (int i = 0; i < iWidth; i++)
        for (int j = 0; j < iHeight; j++)
            lMap[X + i][Y + j]->setBlockID(3);

    //lMap[X + i][Y + j]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 71 : iLevelType == 1 ? 29 : iLevelType == 2 ? 73 : 29);
}

void CGameMap::structBrick(int X, int Y, int iWidth, int iHeight)
{
    for (int i = 0; i < iWidth; i++)
        for (int j = 0; j < iHeight; j++)
            lMap[X + i][Y + j]->setBlockID(13);
}

void CGameMap::struckBlockQ(int X, int Y, int iWidth)
{
    for (int i = 0; i < iWidth; i++)
        lMap[X + i][Y]->setBlockID(9);

    //lMap[X + i][Y]->setBlockID(iLevelType == 0 || iLevelType == 4 ? 8 : 55);
}

void CGameMap::struckBlockQ2(int X, int Y, int iWidth)
{
    for (int i = 0; i < iWidth; i++)
        lMap[X + i][Y]->setBlockID(21); //transp
}

void CGameMap::setBlockID(int X, int Y, int iBlockID)
{
    if (X >= 0 && X < iMapWidth)
        lMap[X][Y]->setBlockID(iBlockID);
}

float CGameMap::getXPos()
{
    return fXPos;
}

void CGameMap::setXPos(float iXPos)
{
    this->fXPos = iXPos;
}

float CGameMap::getYPos()
{
    return fYPos;
}

void CGameMap::setYPos(float iYPos)
{
    this->fYPos = iYPos;
}

int CGameMap::getLevelType()
{
    return iLevelType;
}

void CGameMap::setLevelType(int iLevelType)
{
    this->iLevelType = iLevelType;
}

int CGameMap::getCurrentLevelID()
{
    return currentLevelID;
}

void CGameMap::setCurrentLevelID(int currentLevelID)
{
    if (this->currentLevelID != currentLevelID)
    {
        this->currentLevelID = currentLevelID;
        /*oEvent->resetRedraw();*/
        loadLVL();
        iSpawnPointID = 0;
    }
}

void CGameMap::setSpawnPointID(int iSpawnPointID)
{
    this->iSpawnPointID = iSpawnPointID;
}

int CGameMap::getMapWidth()
{
    return iMapWidth;
}

CGameBlock* CGameMap::getBlock(int iID)
{
    return vBlock[iID];
}

CGameBlock* CGameMap::getMinionBlock(int iID)
{
    return vMinion[iID];
}

CGameMapLevel* CGameMap::getMapBlock(int iX, int iY)
{
    return lMap[iX][iY];
}

CGamePlayer* CGameMap::getPlayer()
{
    return oPlayer;
}

bool CGameMap::getMoveMap()
{
    return bMoveMap;
}

void CGameMap::setMoveMap(bool bMoveMap)
{
    this->bMoveMap = bMoveMap;
}

int CGameMap::getListID(int nX)
{
    return (int)(nX / 160);
}

int CGameMap::getNumOfMinions()
{
    int iOutput = 0;

    for (int i = 0, size = lMinion.size(); i < size; i++)
    {
        iOutput += lMinion[i].size();
    }

    return iOutput;
}

int CGameMap::getStartBlock()
{
    return (int)(-fXPos - (-(int)fXPos) % 32) / 32;
}

int CGameMap::getEndBlock()
{
    return (int)(-fXPos - (-(int)fXPos) % 32 + SIZE_X) / 32 + 2; //多往前加载两格
}

void CGameMap::loadGameData()
{
    // ----- 0 Transparent -----
    vBlock.push_back(new CGameBlock(0, IDB_TRANSP, false, false, false, false));
    //block_floor_1_ground
    vBlock.push_back(new CGameBlock(1, IDB_BLOCK_FLOOR_1_GROUND, true, false, true, true));
    //block_floor_2_ground
    vBlock.push_back(new CGameBlock(2, IDB_BLOCK_FLOOR_2_GROUND, true, false, true, true));
    //coin
    vBlock.push_back(new CGameBlock(3, IDB_COIN, false, false, true, true));
    //bush
    vBlock.push_back(new CGameBlock(4, IDB_BUSH_LEFT, false, false, false, true));
    vBlock.push_back(new CGameBlock(5, IDB_BUSH_RIGHT, false, false, false, true));
    vBlock.push_back(new CGameBlock(6, IDB_BUSH_TOP, false, false, false, true));
    vBlock.push_back(new CGameBlock(7, IDB_BUSH_CENTER_1, false, false, false, true));
    vBlock.push_back(new CGameBlock(8, IDB_BUSH_CENTER_2, false, false, false, true));
    //block_item_1
    vBlock.push_back(new CGameBlock(9, IDB_BLOCK_ITEM_1_COMMON, true, false, true, true));
    //block_item_2
    vBlock.push_back(new CGameBlock(10, IDB_BLOCK_ITEM_2_GROUND, true, false, false, true));
    //grass
    vBlock.push_back(new CGameBlock(11, IDB_GRASS_LEFT, false, false, false, true));
    vBlock.push_back(new CGameBlock(12, IDB_GRASS_RIGHT, false, false, false, true));
    //block_fragile_ground
    vBlock.push_back(new CGameBlock(13, IDB_BLOCK_FRAGILE_GROUND, true, false, true, true));
    //cloud
    vBlock.push_back(new CGameBlock(14, IDB_CLOUD_LEFT, false, false, false, true));
    vBlock.push_back(new CGameBlock(15, IDB_CLOUD_RIGHT, false, false, false, true));
    vBlock.push_back(new CGameBlock(16, IDB_CLOUD_TOP, false, false, false, true));
    vBlock.push_back(new CGameBlock(17, IDB_CLOUD_BOTTOM, false, false, false, true));
    vBlock.push_back(new CGameBlock(18, IDB_CLOUD_CENTER_1, false, false, false, true));
    vBlock.push_back(new CGameBlock(19, IDB_CLOUD_CENTER_2, false, false, false, true));
    //sturdy block
    vBlock.push_back(new CGameBlock(20, IDB_BLOCK_STURDY_GROUND, true, false, false, true));
    //blockQ2
    vBlock.push_back(new CGameBlock(21, IDB_TRANSP, true, false, true, false));
    //coin
    vBlock.push_back(new CGameBlock(22, IDB_COIN, false, false, true, true));
    //pipe
    //debris
	vBlock.push_back(new CGameBlock(23, IDB_BLOCK_DEBRIS_LEFT, false, false, false, true));
	//vBlock.push_back(new CGameBlock(24, IDB_BLOCK_DEBRIS_RIGHT, false, false, false, true));
    // --------------- Enemy ---------------
    //enemy_normal
    vMinion.push_back(new CGameBlock(0, IDB_ENEMY_NORMAL, true, false, true, true));
	//mushroom
	vMinion.push_back(new CGameBlock(1, IDB_MUSHROOM_GOOD, true, false, true, true));
	vMinion.push_back(new CGameBlock(2, IDB_MUSHROOM_GOOD, true, false, true, true));
	vMinion.push_back(new CGameBlock(3, IDB_MUSHROOM_GOOD, true, false, true, true));
	//flower
	vMinion.push_back(new CGameBlock(4, IDB_FLOWER, true, false, true, true));
	//star
	vMinion.push_back(new CGameBlock(5, IDB_STAR_1, true, false, false, true));
	//enemy_turtle
	vMinion.push_back(new CGameBlock(6, IDB_ENEMY_TURTLE_1, true, false, true, true));
	vMinion.push_back(new CGameBlock(7, IDB_ENEMY_TURTLE_2, true, false, true, true));

    iBlockSize = vBlock.size();
    iMinionSize = vMinion.size();

	CAudio::Instance()->Load(AUDIO_FIELD, "sounds\\BGM\\field.wav");
	CAudio::Instance()->Load(AUDIO_BLOCK_BREAK, "sounds\\SE\\blockbreak.wav");
	CAudio::Instance()->Load(AUDIO_BLOCK_COIN, "sounds\\SE\\blockcoin.wav");
	CAudio::Instance()->Load(AUDIO_BLOCK_USE, "sounds\\SE\\blockkinoko.wav");
	CAudio::Instance()->Load(AUDIO_JUMP, "sounds\\SE\\jump.wav");
	CAudio::Instance()->Load(AUDIO_DEATH, "sounds\\SE\\death.wav");
	CAudio::Instance()->Load(AUDIO_HUMI, "sounds\\SE\\humi.wav");
}

void CGameMap::createMap()
{
    // ----- MIONION LIST -----
    for (int i = 0; i < iMapWidth; i += 5)
    {
        std::vector<CGameMinion*> temp;
        lMinion.push_back(temp);
    }

    iMinionListSize = lMinion.size();

    // ----- CREATE MAP -----
    for (int i = 0; i < iMapWidth; i++)
    {
        std::vector<CGameMapLevel*> temp;

        for (int j = 0; j < iMapHeight; j++)
            temp.push_back(new CGameMapLevel(0));

        lMap.push_back(temp);
    }
}

//void CGameMap::checkSpawnPoint()
//{
//    if (getNumOfSpawnPoints() > 1)
//        for (int i = iSpawnPointID + 1; i < getNumOfSpawnPoints(); i++)
//            if (getSpawnPointXPos(i) > oPlayer->getXPos() - fXPos && getSpawnPointXPos(i) < oPlayer->getXPos() - fXPos + 128)
//                iSpawnPointID = i;
//}

//void CGameMap::checkSpawnPoint()
//{
//}

int CGameMap::getNumOfSpawnPoints()
{
    switch (currentLevelID)
    {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
    case 8:
    case 9:
    case 10:
    case 13:
    case 14:
    case 16:
    case 17:
    case 18:
    case 20:
    case 21:
    case 22:
    case 24:
    case 25:
    case 26:
        return 2;
    }

    return 1;
}

int CGameMap::getSpawnPointXPos(int iID)
{
    switch (currentLevelID)
    {
    case 0:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 82 * 32;
        }

    case 1:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 89 * 32;
        }

    case 2:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 66 * 32;
        }

    case 3:
        return 64;

    case 4:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 98 * 32;
        }

    case 5:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 86 * 32;
        }

    case 6:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 114 * 32;
        }

    case 7:
        return 64;

    case 8:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 90 * 32;
        }

    case 9:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 98 * 32;
        }

    case 10:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 66 * 32;
        }

    case 13:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 95 * 32;
        }

    case 14:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 65 * 32;
        }

    case 16:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 97 * 32;
        }

    case 17:
        switch (iID)
        {
        case 0:
            return 84 + 80 * 32;

        case 1:
            return 177 * 32;
        }

    case 18:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 66 * 32;
        }

    case 20:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 98 * 32;
        }

    case 21:
        switch (iID)
        {
        case 0:
            return 84 + 85 * 32;

        case 1:
            return 183 * 32;
        }

    case 22:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 98 * 32;
        }

    case 24:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 98 * 32;
        }

    case 25:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 86 * 32;
        }

    case 26:
        switch (iID)
        {
        case 0:
            return 84;

        case 1:
            return 113 * 32;
        }
    }

    return 84;
}

//int CGameMap::getSpawnPointYPos(int iID)
//{
//    switch (currentLevelID)
//    {
//    case 1:
//        switch (iID)
//        {
//        case 0:
//            return 64;
//        }
//
//    case 5:
//    case 25:
//        switch (iID)
//        {
//        case 0:
//            return 64;
//
//        case 1:
//            return GAME_HEIGHT - 48 - oPlayer->getHitBoxY();
//        }
//
//    case 3:
//    case 7:
//    case 11:
//    case 15:
//    case 19:
//    case 23:
//    case 27:
//    case 31:
//        return 150;
//    }
//
//    return SIZE_Y - 48 - oPlayer->getHitBoxY();
//}
}