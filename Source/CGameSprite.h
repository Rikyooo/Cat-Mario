#pragma once

namespace game_framework
{
	class CGameSprite
	{
	public:
		CGameSprite(void);
		CGameSprite(std::vector<int> iSprite, std::vector<unsigned int> iDelay, bool bRotate);
		~CGameSprite(void);

		void Update();

		/* ----- get & set ----- */
		CMovingBitmap* getTexture();
		CMovingBitmap* getTexture(int iID);

	private:
		std::vector<CMovingBitmap*> vSprite;
		std::vector<unsigned int> iDelay;

		bool bRotate;

		int iCurrentFrame;
		int iStartFrame;
		int iEndFrame;

		unsigned long lTimePassed;
	};
}