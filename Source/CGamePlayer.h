#pragma once
#include "Vector2.h"

namespace game_framework
{
	class CGameMap;
	class CGameSprite;

	class CGamePlayer
	{
	public:
		CGamePlayer(float fXPos, float fYPos);
		~CGamePlayer();
		//void LoadBitmap();
		int GetHitBoxX();
		int GetHitBoxY();
		void SetX(float);
		int GetX();
		void SetY(float);
		int GetY();
		
		int getPowerLVL();
		void setPowerLVL(int powerLVL);
		void resetPowerLVL();

		void Draw();
		void Update(CGameMap *map);

		void playerPhysics(CGameMap *map);
		void updateXPos(int iN, CGameMap *map);
		void updateYPos(int iN, CGameMap *map);

		void destroyBlock(CGameMap* map);

		void startMove();
		void resetMove();
		void stopMove();
		void setMoveDirection(bool moveDirection);
		bool getChangeMoveDirection();
		void setChangeMoveDirection();

		void setIsUpPressed(bool IsUpPressed);
		bool IsUpPressed();
		void jump();
		void startJump(int iH);
		void resetJump();

		void setMarioSpriteID(int iID);
		int getMarioSpriteID();

		void moveAnimation();
		int getMoveSpeed();
		bool getMove();
		bool getMoveDirection();
		void setCurrentJumpSpeed(float currentJumpSpeed);
		void setMoveSpeed(int moveSpeed);

		void setNextFallFrameID(int nextFallFrameID);
		int getJumpState();

		CGameSprite* getMarioSprite();

		void Death(bool animation);
		void setIsPlayerDeath(bool isPlayerDeath);
		bool IsPlayerDeath();


	private:
		std::vector<CGameSprite*> sMario;
		int iSpriteID;
		unsigned int iMoveAnimationTime;
		int powerLVL;
		bool isPlayerDeath;

		//CMovingBitmap player_bmp;
		float fXPos, fYPos;   //玩家在地图上的点坐标

		bool moveDirection; // true = LEFT, false = RIGHT
		bool bMove;
		bool changeMoveDirection;
		bool newMoveDirection;

		static const int maxMove = 4;
		int currentMaxMove;
		int moveSpeed;
		unsigned int iTimePassed;

		bool isUpPressed;
		int jumpState;
		float startJumpSpeed;
		float currentJumpSpeed;
		float jumpDistance;
		float currentJumpDistance;
		float currentFallingSpeed;

		int nextFallFrameID;

		void movePlayer(CGameMap *map);

		bool checkCollisionBot(int nX, int nY, CGameMap *map);
		bool checkCollisionCenter(int nX, int nY, CGameMap *map);

		Vector2* getBlockLB(float nX, float nY, CGameMap *map);
		Vector2* getBlockRB(float nX, float nY, CGameMap *map);

		Vector2* getBlockLC(float nX, float nY, CGameMap *map);
		Vector2* getBlockRC(float nX, float nY, CGameMap *map);

		Vector2* getBlockLT(float nX, float nY, CGameMap *map);
		Vector2* getBlockRT(float nX, float nY, CGameMap *map);
	};
}