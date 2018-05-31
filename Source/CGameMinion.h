#pragma once

namespace game_framework
{
	class CGameMap;

	class CGameMinion
	{
	public:
		CGameMinion(void);
		~CGameMinion(void);

		// ---------- Methods
		virtual void Update(CGameMap* map);
		virtual void Draw(CGameMap* map, int iBlockID);

		virtual void updateYPos(int iN, CGameMap* map);
		virtual void updateXPos(CGameMap* map);

		virtual bool updateMinion(CGameMap* map);
		virtual void minionPhysics(CGameMap* map);

		virtual void collisionEffect();

		virtual void minionDeathAnimation();

		void physicsState1(CGameMap* map);
		void physicsState2(CGameMap* map);

		void Spawn(CGameMap* map);
		void startJump(int iH);
		void resetJump();

		// ----- COLLISON

		virtual void collisionWithPlayer(bool TOP, CGameMap* map);

		virtual void collisionWithAnotherUnit(); // -- PLAYERFIREBALL

		virtual void lockMinion();

		// ----- get & set -----
		int getBlockID();
		void setBlockID(int iBlockID);
		int getMinionState();

		virtual void setMinionState(int minionState);
		virtual bool getPowerUP();

		int getXPos();
		int getYPos();
		void setYPos(int iYPos);
		int getHitBoxX();
		int getHitBoxY();
		bool getMoveDirection();
		void setMoveDirection(bool moveDirection);
		bool getCollisionOnlyWithPlayer();
		bool getMinionSpawned();
		int getDeadTime();
		bool getKillOtherUnits();
		int getMoveSpeed();
		void setOnAnotherMinion(bool onAnotherMinion);

	protected:
		int minionState;

		bool killOtherUnits;

		int iBlockID;
		float fXPos, fYPos;
		int iHitBoxX, iHitBoxY;
		bool minionSpawned;
		bool collisionOnlyWithPlayer;

		int deadTime;

		bool onAnotherMinion;

		// ----- true = LEFT, false = RIGHT
		bool moveDirection;
		int moveSpeed;

		int jumpState;

		float startJumpSpeed;
		float currentJumpSpeed;
		float jumpDistance;
		float currentJumpDistance;
		float currentFallingSpeed;
	};
}