#pragma once

#include "Vector2.h"

namespace game_framework
{
	class CGameMap;

	class CGamePlayer
	{
	public:
		CGamePlayer(float fXPos, float fYPos);
		void LoadBitmap();
		/*void OnMove(CGameMap *map);
		void OnShow(CGameMap *map);*/
		int Width();
		int Height();
		void SetMovingLeft(bool);
		void SetMovingRight(bool);
		void SetMovingUp(bool);
		void SetX(float);
		int GetX();
		void SetY(float);
		int GetY();
		void SetIsOnland(bool);
		bool IsFalling();

		void Draw();
		void Update(CGameMap *map);

		void playerPhysics(CGameMap *map);
		void updateXPos(int iN, CGameMap *map);
		void updateYPos(int iN, CGameMap *map);

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

		int getMoveSpeed();
		bool getMove();
		bool getMoveDirection();
		void setCurrentJumpSpeed(float currentJumpSpeed);
		void setMoveSpeed(int moveSpeed);

		void setNextFallFrameID(int nextFallFrameID);
		int getJumpState();

		void TimeTick();
		int GetTimeTick();

	private:
		CMovingBitmap player_bmp;
		float fXPos, fYPos;   //玩家在地图上的点坐标
		bool isMovingLeft = false;
		bool isMovingRight = false;
		bool isMovingUp = false;
		int jumpingCount = 0;
		//int fallSpeed = 0;
		bool isFalling = false;
		bool isOnland = true;

		bool moveDirection; // true = LEFT, false = RIGHT
		bool bMove;
		bool changeMoveDirection;
		bool newMoveDirection;

		static const int maxMove = 4;
		int currentMaxMove;
		int moveSpeed;
		int iTimePassed;
		int iTimeTick;

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