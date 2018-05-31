#pragma once

#include "Vector2.h"
#include "CGameBlock.h"
#include "CGameMapLevel.h"

namespace game_framework
{
	class CGamePlayer;
	class CGameMinion;
	class CGameGoombas;
	class CGameKoppa;
	class CGameMushroom;
	class CGameStar;
	class CGameCoin;
	class CGameBlockDebris;

	enum AUDIO_ID {				// 定义各种音效的编号
		AUDIO_FIELD,				// 0
		AUDIO_BLOCK_BREAK,			// 1
		AUDIO_BLOCK_COIN,			// 2
		AUDIO_BLOCK_USE,			// 3
		AUDIO_JUMP,			        // 4
		AUDIO_DEATH                 // 5
	};

	//map class
	class CGameMap
	{
	public:
		CGameMap(); 
		~CGameMap(void);
		void LoadBitmap();

/***************************************************/
		void Update();

		void UpdatePlayer();
		void UpdateMinions();
		void UpdateMinionsCollisions();
		//void UpdateBlocks();
		//void UpdateMinionBlokcs();

		void Draw();
		void DrawMap();
		void DrawMinions();

		void moveMap(int, int);
		//void setSpawnPoint();

		bool blockUse(int nX, int nY, int iBlockID, int POS);
		void addGoombas(int iX, int iY, bool moveDirection);
		void addKoppa(int iX, int iY, int minionState, bool moveDirection);

		//void clearPlatforms();

		// ********** COLLISION
		Vector2* getBlockID(int nX, int nY);
		int getBlockIDX(int nX);
		int getBlockIDY(int nY);

		//// ----- LEFT
		bool checkCollisionLB(int nX, int nY, int nHitBoxY, bool checkVisible);
		bool checkCollisionLT(int nX, int nY, bool checkVisible);
		// ----- CENTER
		bool checkCollisionLC(int nX, int nY, int nHitBoxY, bool checkVisible);
		bool checkCollisionRC(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible);
		// ----- RIGHT
		bool checkCollisionRB(int nX, int nY, int nHitBoxX, int nHitBoxY, bool checkVisible);
		bool checkCollisionRT(int nX, int nY, int nHitBoxX, bool checkVisible);

		int checkCollisionWithPlatform(int nX, int nY, int iHitBoxX, int iHitBoxY);

		bool checkCollision(Vector2* nV, bool checkVisible);

		void checkCollisionOnTopOfTheBlock(int nX, int nY);
		//// ********** COLLISION

		void playerDeath(bool animation);

		// ----- LOAD
		void resetGameData();
		void loadLVL();
		void setBackgroundColor();

		void structBush(int X, int Y, int iSze);
		void structGrass(int X, int Y, int iSize);
		void structCloud(int X, int Y);
		void structCloud2(int X, int Y);
		void structGround(int X, int Y, int iWidth, int iHeight);
		void structGND2(int X, int Y, int iSize, bool bDir); // true = LEFT, false = RIGHT
		void structGND2(int X, int Y, int iWidth, int iHeight);
		void structCoins(int X, int Y, int iWidth, int iHeight);
		void structBrick(int X, int Y, int iWidth, int iHeight);
		void struckBlockQ(int X, int Y, int iWidth);
		void struckBlockQ2(int X, int Y, int iWidth); //invisible

		void setBlockID(int X, int Y, int iBlockID);

		float getXPos();
		void setXPos(float iYPos);
		float getYPos();
		void setYPos(float iYPos);

		int getLevelType();
		void setLevelType(int iLevelType);

		int getCurrentLevelID();
		void setCurrentLevelID(int currentLevelID);

		void setSpawnPointID(int iSpawnPointID);

		int getMapWidth();

		CGameBlock* getBlock(int iID);
		CGameBlock* getMinionBlock(int iID);
		CGameMapLevel* getMapBlock(int iX, int iY);

		CGamePlayer* getPlayer();
		//Platform* getPlatform(int iID);

		bool getMoveMap();
		void setMoveMap(bool bMoveMap);

		void setIsPlayerDeath(bool isPlayerDeath);
		bool IsPlayerDeath();

	private:

/***************************************************/
		float fXPos, fYPos;
		vector<CGameBlock*> vBlock;
		int iBlockSize; // Size of vBlock
		vector<CGameBlock*> vMinion;
		int iMinionSize; // Size of vBlock
		vector<vector<CGameMapLevel*>> lMap;
		int iMapWidth, iMapHeight;
		vector<CGameBlockDebris*> lBlockDebris;

		int currentLevelID;
		int iLevelType; // 0, 1, 2;
		int iSpawnPointID;

		bool bMoveMap;

		CGamePlayer* oPlayer;
		vector<vector<CGameMinion*>> lMinion;
		int iMinionListSize;
		int getListID(int);
		int getNumOfMinions();

		vector<CGameCoin*> lCoin;

		int getStartBlock();
		int getEndBlock();
		void loadGameData();

		void createMap();

		//void checkSpawnPoint();
		int getNumOfSpawnPoints();
		int getSpawnPointXPos(int iID);
		//int getSpawnPointYPos(int iID);

		void loadLVL_1_1();
		void loadMinionsLVL_1_1();

		void clearMap();
		void clearMinions();

		bool isPlayerDeath;
	};
}