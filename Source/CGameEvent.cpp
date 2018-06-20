#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CGameEvent.h"
#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
	Event::Event(void)
	{
		this->iDelay = 0;
		this->newUnderWater = false;
		this->endGame = false;
		this->iTime = 0;
		this->bState = true;
		this->stepID = 0;
	}

	Event::~Event(void)
	{
	}

	void Event::Normal(CGameMap* map)
	{
		if (bState) {
			if (vOLDDir.size() > stepID) {
				if (vOLDLength[stepID] > 0) {
					switch (vOLDDir[stepID]) {
					case eTOP: // TOP
						map->getPlayer()->setYPos((float)map->getPlayer()->getYPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						break;
					case eBOT:
						map->getPlayer()->setYPos((float)map->getPlayer()->getYPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						break;
					case eRIGHT:
						map->getPlayer()->setXPos((float)map->getPlayer()->getXPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->moveAnimation();
						map->getPlayer()->setMoveDirection(true);
						break;
					case eRIGHTEND:
						map->setXPos((float)map->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->moveAnimation();
						map->getPlayer()->setMoveDirection(true);
						break;
					case eLEFT:
						map->getPlayer()->setXPos((float)map->getPlayer()->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->moveAnimation();
						map->getPlayer()->setMoveDirection(false);
						break;
					case eBOTRIGHTEND: // BOT & RIGHT
						map->getPlayer()->setYPos((float)map->getPlayer()->getYPos() + iSpeed);
						map->setXPos((float)map->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->moveAnimation();
						break;
					case eENDBOT1:
						map->getPlayer()->setYPos((float)map->getPlayer()->getYPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(10);
						break;
					case eENDBOT2:
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->setMoveDirection(false);
						break;
					case eENDPOINTS:
						if (map->getMapTime() > 0) {
							map->setMapTime(map->getMapTime() - 1);
							map->getPlayer()->setScore(map->getPlayer()->getScore() + 50);
							if (CCFG::getMusic()->musicStopped) {
								CCFG::getMusic()->PlayMusic(CCFG::getMusic()->mSCORERING);
							}
						}
						else {
							vOLDLength[stepID] = 0;
							CCFG::getMusic()->StopMusic();
						}
						map->getFlag()->UpdateCastleFlag();
						break;
					case eDEATHNOTHING:
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(0);
						break;
					case eDEATHTOP: // DEATH TOP
						map->getPlayer()->setYPos((float)map->getPlayer()->getYPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(0);
						break;
					case eDEATHBOT: // DEATH BOT
						map->getPlayer()->setYPos((float)map->getPlayer()->getYPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(0);
						break;
					case eNOTHING: // NOTHING YAY
						vOLDLength[stepID] -= 1;
						break;
					case ePLAYPIPERIGHT:
						map->getPlayer()->setXPos((float)map->getPlayer()->getXPos() + iSpeed);
						vOLDLength[stepID] -= 1;
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPIPE);
						break;
					case eLOADINGMENU:
						vOLDLength[stepID] -= 1;

						if (vOLDLength[stepID] < 2) {
							map->setInEvent(false);
							inEvent = false;
							map->getPlayer()->stopMove();

							CCFG::getMM()->getLoadingMenu()->loadingType = true;
							CCFG::getMM()->getLoadingMenu()->updateTime();
							CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
						}
						break;
					case eGAMEOVER:
						vOLDLength[stepID] -= 1;

						if (vOLDLength[stepID] < 2) {
							map->setInEvent(false);
							inEvent = false;
							map->getPlayer()->stopMove();

							CCFG::getMM()->getLoadingMenu()->loadingType = false;
							CCFG::getMM()->getLoadingMenu()->updateTime();
							CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);

							CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cGAMEOVER);
						}
						break;
					case eBOSSEND1:
						for (int i = map->getMapWidth() - 1; i > 0; i--) {
							if (map->getMapBlock(i, 6)->getBlockID() == 82) {
								map->getMapBlock(i, 6)->setBlockID(0);
								break;
							}
						}
						//map->getMapBlock(map->getBlockIDX((int)(map->getPlayer()->getXPos() + map->getPlayer()->getHitBoxX()/2 - map->getXPos()) + vOLDLength[stepID] - 1), 6)->setBlockID(0);
						map->clearPlatforms();
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBRIDGEBREAK);
						vOLDLength[stepID] = 0;
						map->getPlayer()->setMoveDirection(false);
						break;
					case eBOSSEND2:
						//map->getMapBlock(map->getBlockIDX((int)(map->getPlayer()->getXPos() + map->getPlayer()->getHitBoxX()/2 - map->getXPos())) - 1, 5)->setBlockID(0);
						//map->getMapBlock(map->getBlockIDX((int)(map->getPlayer()->getXPos() + map->getPlayer()->getHitBoxX()/2 - map->getXPos())) - 1, 4)->setBlockID(0);
						for (int i = map->getMapWidth() - 1; i > 0; i--) {
							if (map->getMapBlock(i, 5)->getBlockID() == 79) {
								map->getMapBlock(i, 5)->setBlockID(0);
								break;
							}
						}
						for (int i = map->getMapWidth() - 1; i > 0; i--) {
							if (map->getMapBlock(i, 4)->getBlockID() == 76) {
								map->getMapBlock(i, 4)->setBlockID(0);
								break;
							}
						}
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBRIDGEBREAK);
						vOLDLength[stepID] = 0;
						break;
					case eBOSSEND3:
						for (int i = map->getMapWidth() - 1; i > 0; i--) {
							if (map->getMapBlock(i, 4)->getBlockID() == 76) {
								map->getMapBlock(i, 4)->setBlockID(0);
								break;
							}
						}
						//map->getMapBlock(map->getBlockIDX((int)(map->getPlayer()->getXPos() + map->getPlayer()->getHitBoxX()/2 - map->getXPos())) - vOLDLength[stepID], 4)->setBlockID(0);
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBRIDGEBREAK);
						map->getPlayer()->setMoveDirection(true);
						vOLDLength[stepID] = 0;
						break;
					case eBOSSEND4:
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBOWSERFALL);
						vOLDLength[stepID] = 0;
						break;
					case eBOTRIGHTBOSS: // BOT & RIGHT
						map->getPlayer()->moveAnimation();
						map->getPlayer()->playerPhysics();
						map->setXPos((float)map->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						break;
					case eBOSSTEXT1:
						map->addText(vOLDLength[stepID], CCFG::GAME_HEIGHT - 16 - 9 * 32, "THANK YOU MARIOz");
						vOLDLength[stepID] = 0;
						break;
					case eBOSSTEXT2:
						map->addText(vOLDLength[stepID] + 16, CCFG::GAME_HEIGHT - 16 - 7 * 32, "BUT OUR PRINCESS IS IN");
						map->addText(vOLDLength[stepID] + 16, CCFG::GAME_HEIGHT - 16 - 6 * 32, "ANOTHER CASTLEz");
						vOLDLength[stepID] = 0;
						break;
					case eENDGAMEBOSSTEXT1:
						map->addText(vOLDLength[stepID], CCFG::GAME_HEIGHT - 16 - 9 * 32, "THANK YOU MARIOz");
						vOLDLength[stepID] = 0;
						break;
					case eENDGAMEBOSSTEXT2:
						map->addText(vOLDLength[stepID] + 16, CCFG::GAME_HEIGHT - 16 - 7 * 32, "YOUR QUEST IS OVER.");
						vOLDLength[stepID] = 0;
						break;
					case eMARIOSPRITE1:
						map->getPlayer()->setMarioSpriteID(1);
						vOLDLength[stepID] = 0;
						break;
					case eVINE1:
						map->getPlayer()->SetY((float)map->getPlayer()->GetY() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(10);
						break;
					case eVINE2:
						map->getPlayer()->SetY((float)map->getPlayer()->GetY() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(11);
						break;
					case eVINESPAWN:
						map->addVine(vOLDLength[stepID], 0, 1, newLevelType == 0 || newLevelType == 4 ? 34 : 36);
						vOLDLength[stepID] = 0;
						break;
					}
				}
				else {
					++stepID;
					iTime = timeGetTime();
				}
			}
			else {
				if (!endGame) {
					if (timeGetTime() >= iTime + iDelay) {
						bState = false;
						stepID = 0;
						newLevel(map);
						map->getPlayer()->stopMove();
						if (inEvent) {
							CCFG::getMM()->getLoadingMenu()->updateTime();
							CCFG::getMM()->getLoadingMenu()->loadingType = true;
							CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
							map->startLevelAnimation();
						}

						CCFG::keySpace = false;
					}
				}
				else {
					map->resetGameData();
					CCFG::getMM()->setViewID(CCFG::getMM()->eMainMenu);
					map->setInEvent(false);
					map->getPlayer()->stopMove();
					inEvent = false;
					CCFG::keySpace = false;
					endGame = false;
					stepID = 0;
				}
			}
		}
		else {
			if (vNEWDir.size() > stepID) {
				if (vNEWLength[stepID] > 0) {
					switch (vNEWDir[stepID]) {
					case eTOP: // TOP
						map->getPlayer()->SetY((float)map->getPlayer()->GetY() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						break;
					case eBOT:
						map->getPlayer()->SetY((float)map->getPlayer()->GetY() + iSpeed);
						vNEWLength[stepID] -= iSpeed;
						break;
					case eRIGHT:
						map->getPlayer()->SetX((float)map->getPlayer()->GetX() + iSpeed);
						vNEWLength[stepID] -= iSpeed;
						map->getPlayer()->moveAnimation();
						break;
					case eLEFT:
						map->getPlayer()->SetX((float)map->getPlayer()->GetX() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						map->getPlayer()->moveAnimation();
						break;
					case ePLAYPIPETOP:
						vNEWLength[stepID] -= 1;
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPIPE);
						break;
					case eNOTHING: // NOTHING YAY
						vNEWLength[stepID] -= 1;
						break;
					case eVINE1:
						map->getPlayer()->SetY((float)map->getPlayer()->GetY() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(10);
						break;
					case eVINE2:
						map->getPlayer()->SetY((float)map->getPlayer()->GetY() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						map->getPlayer()->setMarioSpriteID(11);
						break;
					}
				}
				else {
					++stepID;
				}
			}
			else {
				map->setInEvent(false);
				map->getPlayer()->stopMove();
				//CCFG::getMusic()->changeMusic(true, true);
				inEvent = false;
				CCFG::keySpace = false;
				CCore::resetKeys();
			}
		}
	}

	void Event::end(CGameMap* map)
	{
		if (map->getFlag() != NULL && map->getFlag()->iYPos < CCFG::GAME_HEIGHT - 16 - 3 * 32 - 4) {
			map->getFlag()->Update();
		}
	}

	void Event::resetData()
	{
		vNEWDir.clear();
		vNEWLength.clear();
		vOLDDir.clear();
		vOLDLength.clear();
		resetRedraw();

		this->eventTypeID = eNormal;

		this->bState = true;
		this->stepID = 0;
		this->inEvent = false;
		this->endGame = false;
		this->newUnderWater = false;
	}

	void Event::Draw(CGameMap* map)
	{
		for (unsigned int i = 0; i < reDrawX.size(); i++) {
			if (reDrawX[i] < map->getMapWidth())
				map->getBlock(map->getMapBlock(reDrawX[i], reDrawY[i])->getBlockID())->OnDraw(32 * reDrawX[i] + (int)map->getXPos(), SIZE_Y - 32 * reDrawY[i] - 16, false);
		}
	}

	void Event::Animation(CGameMap* map)
	{
		switch (eventTypeID) 
		{
		case eNormal: 
			Normal(map);
			break;
		case eEnd:
			Normal(map);
			end(map);
			break;
		case eBossEnd:
			Normal(map);
			break;
		default:
			Normal(map);
			break;
		}
	}
	void Event::newLevel(CGameMap* map)
	{
		map->setXPos((float)newMapXPos);
		map->getPlayer()->SetX((float)newPlayerXPos);
		map->getPlayer()->SetY((float)newPlayerYPos);
		map->setMoveMap(newMoveMap);
		if (map->getCurrentLevelID() != newCurrentLevel) {
			/*CCFG::getMM()->getLoadingMenu()->updateTime();
			CCFG::getMM()->getLoadingMenu()->loadingType = true;
			CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
			map->getPlayer()->setCoins(0);*/
		}
		map->setCurrentLevelID(newCurrentLevel);
		map->setLevelType(newLevelType);
		/*if (newUnderWater) {
			map->getPlayer()->resetRun();
		}
		map->setUnderWater(newUnderWater);*/

		map->lockMinions();
	}

	void Event::resetRedraw()
	{
		reDrawX.clear();
		reDrawY.clear();
	}
}