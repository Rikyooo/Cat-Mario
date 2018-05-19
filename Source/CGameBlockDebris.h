#pragma once

#include "Vector2.h"

namespace game_framework
{
	class CGameMap;

	class CGameBlockDebris
	{
	public:
		CGameBlockDebris(int iXPos, int iYPos);
		~CGameBlockDebris(void);

		void Update();
		void Draw(CGameMap* map);

		int getDebrisState();
	private:
		// ----- 0 = Animation, -1 = Delete
		int debrisState;

		Vector2* vPositionL;
		Vector2* vPositionR;
		Vector2* vPositionL2;
		Vector2* vPositionR2;

		int iFrameID;

		float fSpeedX, fSpeedY;

		bool bRotate;
	};
}