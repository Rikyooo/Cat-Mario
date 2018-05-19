#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework
{
	/////////////////////////////////////////////////////////////////////////////
	// ���classΪ��Ϸ����Ϸ��ͷ�������
	/////////////////////////////////////////////////////////////////////////////
	CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// ��ͼ�ܶ�ʱ��OnInit�������е�ͼҪ���ܶ�ʱ�䡣Ϊ��������Ϸ����
		//     �ȵĲ��ͷ�����Ϸ����֡�Loading ...������ʾLoading�Ľ��ȡ�
		//
		CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// �� Back Plain Ϳ��Ԥ�����ɫ
		ShowInitProgress(0);	// һ��ʼ��loading����Ϊ0%
								//
								// ��ʼ��������
								//
								/*logo.LoadBitmap(IDB_BACKGROUND);*/

		title_bmp.LoadBitmap(IDB_TITLE);
		block_floor_1_ground_bmp.LoadBitmap(IDB_BLOCK_FLOOR_1_GROUND);
		block_floor_2_ground_bmp.LoadBitmap(IDB_BLOCK_FLOOR_2_GROUND);
		player_bmp.LoadBitmap(IDB_PLAYER_1);
		grass_left_bmp.LoadBitmap(IDB_GRASS_LEFT);
		grass_right_bmp.LoadBitmap(IDB_GRASS_RIGHT);
		bush_top_bmp.LoadBitmap(IDB_BUSH_TOP);
		bush_center_1_bmp.LoadBitmap(IDB_BUSH_CENTER_1);
		bush_center_2_bmp.LoadBitmap(IDB_BUSH_CENTER_2);
		bush_left_bmp.LoadBitmap(IDB_BUSH_LEFT);
		bush_right_bmp.LoadBitmap(IDB_BUSH_RIGHT);
		
		//Sleep(300);				// �������Ա㿴������ȣ�ʵ����Ϸ��ɾ����Sleep
								//
								// ��OnInit������ӵ�CGameStaterRun::OnInit()�����Խ��Ȼ�û��100%
								//
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_ENTER = 13;

		if (nChar == KEY_ENTER)
			GotoGameState(GAME_STATE_OVER);						// �л���GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo �ر���Ϸ�ķ���
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �ر���Ϸ
	}

	void CGameStateInit::OnShow()
	{
		//
		// ����title
		//
		title_bmp.SetTopLeft((SIZE_X - title_bmp.Width()) / 2, SIZE_Y / 7);
		title_bmp.ShowBitmap();
		//
		// ����floor
		//
		for (int i = 0; i < 20; i++)
		{
			block_floor_1_ground_bmp.SetTopLeft(BLOCK_WIDTH_HEIGHT * i, BLOCK_WIDTH_HEIGHT * 13);
			block_floor_1_ground_bmp.ShowBitmap();
			block_floor_2_ground_bmp.SetTopLeft(BLOCK_WIDTH_HEIGHT * i, BLOCK_WIDTH_HEIGHT * 14);
			block_floor_2_ground_bmp.ShowBitmap();
		}
		//
		// ����player
		//
		player_bmp.SetTopLeft(2 * BLOCK_WIDTH_HEIGHT + 6, SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - player_bmp.Height());
		player_bmp.ShowBitmap();
		////
		//// ����grass
		////
		grass_left_bmp.SetTopLeft(6 * BLOCK_WIDTH_HEIGHT + 5, SIZE_Y - 3 * BLOCK_WIDTH_HEIGHT);
		grass_left_bmp.ShowBitmap();
		grass_right_bmp.SetTopLeft(7 * BLOCK_WIDTH_HEIGHT + 5, SIZE_Y - 3 * BLOCK_WIDTH_HEIGHT);
		grass_right_bmp.ShowBitmap();
		////
		//// ����bush
		////
		bush_left_bmp.SetTopLeft(12 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y -  3 * BLOCK_WIDTH_HEIGHT);
		bush_left_bmp.ShowBitmap();
		bush_left_bmp.SetTopLeft(13 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y -  4 * BLOCK_WIDTH_HEIGHT);
		bush_left_bmp.ShowBitmap();
		bush_right_bmp.SetTopLeft(15 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y - 4 * BLOCK_WIDTH_HEIGHT);
		bush_right_bmp.ShowBitmap();
		bush_right_bmp.SetTopLeft(16 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y - 3 * BLOCK_WIDTH_HEIGHT);
		bush_right_bmp.ShowBitmap();
		bush_center_1_bmp.SetTopLeft(14 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y - 3 * BLOCK_WIDTH_HEIGHT);
		bush_center_1_bmp.ShowBitmap();
		bush_center_2_bmp.SetTopLeft(13 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y - 3 * BLOCK_WIDTH_HEIGHT);
		bush_center_2_bmp.ShowBitmap();
		bush_center_2_bmp.SetTopLeft(15 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y - 3 * BLOCK_WIDTH_HEIGHT);
		bush_center_2_bmp.ShowBitmap();
		bush_center_2_bmp.SetTopLeft(14 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y - 4 * BLOCK_WIDTH_HEIGHT);
		bush_center_2_bmp.ShowBitmap();
		bush_top_bmp.SetTopLeft(14 * BLOCK_WIDTH_HEIGHT + 15, SIZE_Y - 5 * BLOCK_WIDTH_HEIGHT);
		bush_top_bmp.ShowBitmap();
		//
		// DemoӫĻ���͵�ʹ�ã���������ʱ�뾡������ֱ��ʹ�����ͣ�����CMovingBitmap�ȽϺ�
		//
		CDC* pDC = CDDraw::GetBackCDC();			// ȡ�� Back Plain �� CDC
		CFont f, *fp;
		f.CreatePointFont(160, "Yu Gothic UI");	// ���� font f; 160��ʾ16 point����
		fp = pDC->SelectObject(&f);					// ѡ�� font f
		pDC->SetBkColor(RGB(160, 180, 250));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(230, 240, "Press ENTER Key");
		/*pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");*/

		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

		/*pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");*/
		pDC->SelectObject(fp);						// �ŵ� font f (ǧ��Ҫ©�˷ŵ�)
		CDDraw::ReleaseBackCDC();					// �ŵ� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// ���classΪ��Ϸ�Ľ���״̬(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame* g) : CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		counter--;

		if (counter < 0)
			GotoGameState(GAME_STATE_RUN);
	}

	void CGameStateOver::OnBeginState()
	{
		counter = 30 * 2; // 2 seconds
		lives--;
	}

	void CGameStateOver::OnInit()
	{
		//
		// ��ͼ�ܶ�ʱ��OnInit�������е�ͼҪ���ܶ�ʱ�䡣Ϊ��������Ϸ����
		//     �ȵĲ��ͷ�����Ϸ����֡�Loading ...������ʾLoading�Ľ��ȡ�
		//
		ShowInitProgress(66);	// �Ӹ�ǰһ��״̬�Ľ��ȣ��˴�������Ϊ66%
								//
								// ��ʼ��������
								//
		//Sleep(300);				// �������Ա㿴������ȣ�ʵ����Ϸ��ɾ����Sleep
								//
								// ���ս���Ϊ100%
								//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDDraw::BltBackColor(RGB(0, 0, 0));
		CDC* pDC = CDDraw::GetBackCDC();			// ȡ�� Back Plain �� CDC
		CFont f, *fp;
		f.CreatePointFont(160, "Yu Gothic UI");	// ���� font f; 160��ʾ16 point����
		fp = pDC->SelectObject(&f);					// ѡ�� font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 255));
		char str[80];								// Demo ���ֶ��ִ���ת��
		sprintf(str, "Lives x %d", lives);
		pDC->TextOut(270, 210, str);
		pDC->SelectObject(fp);						// �ŵ� font f (ǧ��Ҫ©�˷ŵ�)
		CDDraw::ReleaseBackCDC();					// �ŵ� Back Plain �� CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// ���classΪ��Ϸ����Ϸִ���������Ҫ����Ϸ��ʽ��������
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
	{
	}

	CGameStateRun::~CGameStateRun()
	{
		/*for (unsigned int i = 0; i < bmp.size(); i++)
			delete bmp[i];*/
		/*delete [] ball;*/
	}

	void CGameStateRun::OnBeginState()
	{
		map->setIsPlayerDeath(false);
		map->loadLVL();
	    //�趨��ҵ���ʼ����
		//bmp[0]->SetXY(11 * BLOCK_WIDTH_HEIGHT, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - 30); //�趨�������ʼ���� enemy_normal_height = 30
		//bmp[1]->SetXY(15 * BLOCK_WIDTH_HEIGHT, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - 30);
		//map_stage_1.SetScreenX(0);                          //���õ�ͼ����ʼ����

		/*for (unsigned int i = 0; i < bmp.size(); i++)
			bmp[i]->SetIsAlive(true);*/
		
		//CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
		//CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
	}

	void CGameStateRun::OnMove()							// �ƶ���ϷԪ��
	{
		map->TimeTick();

		if (isLeftPressed) 
		{
			if (!map->getPlayer()->getMove() && firstDir == false && !map->getPlayer()->getChangeMoveDirection()) 
			{
				map->getPlayer()->startMove();
				map->getPlayer()->setMoveDirection(false);
			}
			else if (!isRightPressed && map->getPlayer()->getMoveSpeed() > 0 && firstDir != map->getPlayer()->getMoveDirection()) 
				map->getPlayer()->setChangeMoveDirection();
		}

		if (isRightPressed)  
		{
			if (!map->getPlayer()->getMove() && firstDir == true && !map->getPlayer()->getChangeMoveDirection()) 
			{
				map->getPlayer()->startMove();
				map->getPlayer()->setMoveDirection(true);
			}
			else if (!isLeftPressed && map->getPlayer()->getMoveSpeed() > 0 && firstDir != map->getPlayer()->getMoveDirection())
				map->getPlayer()->setChangeMoveDirection();
		}

		if (map->getPlayer()->getMove() && !isLeftPressed && !isRightPressed)
			map->getPlayer()->resetMove();

		map->Update();
		map->UpdateMinionsCollisions();

		if (map->IsPlayerDeath())
		{
			isLeftPressed = false;
			isRightPressed = false;
			map->setXPos(0);
			map->setYPos(0);
			map->getPlayer()->SetX(6);
			map->getPlayer()->SetY(SIZE_Y - 2 * BLOCK_WIDTH_HEIGHT - 37);	
			GotoGameState(GAME_STATE_OVER);
		}
	}

	void CGameStateRun::OnInit()  								// ��Ϸ�ĳ�ֵ��ͼ���趨
	{
		//
		// ��ͼ�ܶ�ʱ��OnInit�������е�ͼҪ���ܶ�ʱ�䡣Ϊ��������Ϸ����
		//     �ȵĲ��ͷ�����Ϸ����֡�Loading ...������ʾLoading�Ľ��ȡ�
		//
		CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// �� Back Plain Ϳ��Ԥ�����ɫ
		ShowInitProgress(33);	// �Ӹ�ǰһ��״̬�Ľ��ȣ��˴�������Ϊ33%
								//
								// ��ʼ��������
								//
								//for (int i = 0; i < NUMBALLS; i++)
								//	ball[i].LoadBitmap();								// �����i�����ͼ��
								//eraser.LoadBitmap();
		//map_stage_1.LoadBitmap(); //�����ͼ
		//bmp.LoadBitmap();
		//for (unsigned int i = 0; i < bmp.size(); i++)
		//	bmp[i]->LoadBitmap();
		/*map.setSpawnPoint();*/
		map = new CGameMap();
		map->LoadBitmap();
																	//
																	// ��ɲ���Loading��������߽���
																	//
		ShowInitProgress(50);
		//Sleep(300); // �������Ա㿴������ȣ�ʵ����Ϸ��ɾ����Sleep
					//
					// ����������������
					//
					//help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// ����˵����ͼ��
					//corner.LoadBitmap(IDB_CORNER);							// �������ͼ��
					//corner.ShowBitmap(background);							// ��corner����background
					//bball.LoadBitmap();										// ����ͼ��
		/*hits_left.LoadBitmap();*/
		//CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// ������0������ding.wav
		//CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// ������1������lake.mp3
		//CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// ������2������ntut.mid
		//
		// ��OnInit������ӵ�CGameStaterOver::OnInit()�����Խ��Ȼ�û��100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard���ͷ
		const char KEY_UP = 0x26; // keyboard�ϼ�ͷ
		const char KEY_RIGHT = 0x27; // keyboard�Ҽ�ͷ
		const char KEY_DOWN = 0x28; // keyboard�¼�ͷ

		if (nChar == KEY_RIGHT)
		{
			isRightPressed = true;

			if (!isLeftPressed)
				firstDir = true;
		}


		if (nChar == KEY_LEFT)
		{
			isLeftPressed = true;

			if (!isRightPressed)
				firstDir = false;
		}

		if (nChar == KEY_UP)
		{
			if (!map->getPlayer()->IsUpPressed()) 
			{
				map->getPlayer()->jump();
				map->getPlayer()->setIsUpPressed(true);
			}
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard���ͷ
		const char KEY_UP = 0x26; // keyboard�ϼ�ͷ
		const char KEY_RIGHT = 0x27; // keyboard�Ҽ�ͷ
		const char KEY_DOWN = 0x28; // keyboard�¼�ͷ

		if (nChar == KEY_RIGHT)
		{
			if (firstDir)
				firstDir = false;

			isRightPressed = false;
		}


		if (nChar == KEY_LEFT)
		{
			if (!firstDir)
				firstDir = true;

			isLeftPressed = false;
		}

		if (nChar == KEY_UP)
		{
			map->getPlayer()->setIsUpPressed(false);
		}
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // ������Ķ���
	{
		/*eraser.SetMovingLeft(true);*/
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// ������Ķ���
	{
		/*eraser.SetMovingLeft(false);*/
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// ������Ķ���
	{
		// û�¡������Ҫ�������ƶ��Ļ���дcode������
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // ������Ķ���
	{
		/*eraser.SetMovingRight(true);*/
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// ������Ķ���
	{
		/*eraser.SetMovingRight(false);*/
	}

	void CGameStateRun::OnShow()
	{
		/*map_stage_1.OnShow();
		for (unsigned int i = 0; i < bmp.size(); i++)
			bmp[i]->OnShow(&map_stage_1);
		bmp.OnShow(&map_stage_1);
		hits_left.ShowBitmap();*/
		
		map->Draw();
	}
}