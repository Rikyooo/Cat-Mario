#pragma once

namespace game_framework
{
	class CGameMap;
	class CGamePlayer;

	class Event
	{
	public:
		Event(void);
		~Event(void);

		enum animationType {
			eTOP,
			eRIGHT,
			eRIGHTEND,
			eBOT,
			eLEFT,
			eBOTRIGHTEND,
			eENDBOT1,
			eENDBOT2,
			eENDPOINTS,
			eDEATHNOTHING,
			eDEATHTOP,
			eDEATHBOT,
			eNOTHING,
			ePLAYPIPERIGHT,
			ePLAYPIPETOP,
			eLOADINGMENU,
			eGAMEOVER,
			eBOSSEND1,
			eBOSSEND2,
			eBOSSEND3,
			eBOSSEND4,
			eBOTRIGHTBOSS,
			eBOSSTEXT1,
			eBOSSTEXT2,
			eENDGAMEBOSSTEXT1,
			eENDGAMEBOSSTEXT2,
			eMARIOSPRITE1,
			eVINE1,
			eVINE2,
			eVINESPAWN,
		};

		vector<animationType> vOLDDir;
		vector<int> vOLDLength;

		vector<animationType> vNEWDir;
		vector<int> vNEWLength;

		vector<int> reDrawX;
		vector<int> reDrawY;

		enum eventType {
			eNormal,
			eEnd,
			eBossEnd,
		};

		eventType eventTypeID;

		void Normal(CGameMap* map);
		void end(CGameMap* map);

		int iSpeed;

		void resetData();

		int newLevelType;
		int newMapXPos;
		int newPlayerXPos;
		int newPlayerYPos;
		bool newMoveMap;

		unsigned int iTime;
		int iDelay;

		int newCurrentLevel;
		bool inEvent;
		bool newUnderWater;

		bool endGame;

		// ----- Methods

		void Draw(CGameMap* map);

		void Animation(CGameMap* map);
		void newLevel(CGameMap* map);

		void resetRedraw();

	private:
		bool bState; // true = OLD, false = NEW
		unsigned int stepID;
	};
}