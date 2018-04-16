/*
 * mygame.cpp: 本档案储游戏本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

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
	// 这个class为游戏的游戏开头画面物件
	/////////////////////////////////////////////////////////////////////////////
	CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// 当图很多时，OnInit载入所有的图要花很多时间。为避免玩游戏的人
		//     等的不耐烦，游戏会出现“Loading ...”，显示Loading的进度。
		//
		CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// 将 Back Plain 涂上预设的颜色
		ShowInitProgress(0);	// 一开始的loading进度为0%
								//
								// 开始载入资料
								//
								/*logo.LoadBitmap(IDB_BACKGROUND);*/
		block_floor_1_ground.LoadBitmap(IDB_BLOCK_FLOOR_1_GROUND);
		block_floor_2_ground.LoadBitmap(IDB_BLOCK_FLOOR_2_GROUND);
		player.LoadBitmap(IDB_PLAYER_1, RGB(160, 180, 250));
		title.LoadBitmap(IDB_TITLE);
		mountain.LoadBitmap(IDB_MOUNTAIN, RGB(160, 180, 250));
		brush.LoadBitmap(IDB_BRUSH, RGB(160, 180, 250));
		//Sleep(300);				// 放慢，以便看清楚进度，实际游戏请删除此Sleep
								//
								// 此OnInit动作会接到CGameStaterRun::OnInit()，所以进度还没到100%
								//
	}

	void CGameStateInit::OnBeginState()
	{
	}

	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';

		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// 切换至GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo 关闭游戏的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 关闭游戏
	}

	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// 切换至GAME_STATE_RUN
	}

	void CGameStateInit::OnShow()
	{
		//
		// 贴上logo
		//
		/*logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
		logo.ShowBitmap();*/
		//
		// 贴上floor
		//
		for (int i = 0; i < 17; i++)
		{
			block_floor_1_ground.SetTopLeft(BLOCK_WIDTH_HEIGHT * i, BLOCK_WIDTH_HEIGHT * 13);
			block_floor_1_ground.ShowBitmap();
			block_floor_2_ground.SetTopLeft(BLOCK_WIDTH_HEIGHT * i, BLOCK_WIDTH_HEIGHT * 14);
			block_floor_2_ground.ShowBitmap();
		}
		//
		// 贴上player
		//


		player.SetTopLeft(33 * 2, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - player.Height());
		player.ShowBitmap();
		//
		// 贴上title
		//
		title.SetTopLeft((SIZE_X - title.Width()) / 2, SIZE_Y / 7);
		title.ShowBitmap();
		//
		// 贴上brush
		//
		brush.SetTopLeft(BLOCK_WIDTH_HEIGHT * 6 + 5, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - 28);
		brush.ShowBitmap();
		//
		// 贴上mountain
		//
		mountain.SetTopLeft(BLOCK_WIDTH_HEIGHT * 12 + 20, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - 74);
		mountain.ShowBitmap();
		//
		// Demo荧幕字型的使用，不过开发时请尽量避免直接使用字型，改用CMovingBitmap比较好
		//
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
		CFont f, *fp;
		f.CreatePointFont(160, "Yu Gothic UI");	// 产生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 选用 font f
		pDC->SetBkColor(RGB(160, 180, 250));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(170, 240, "Press SPACE Key");
		/*pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");*/

		if (ENABLE_GAME_PAUSE)
			pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

		/*pDC->TextOut(5,455,"Press Alt-F4 or ESC to Quit.");*/
		pDC->SelectObject(fp);						// 放掉 font f (千万不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 这个class为游戏的结束状态(Game Over)
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
		// 当图很多时，OnInit载入所有的图要花很多时间。为避免玩游戏的人
		//     等的不耐烦，游戏会出现“Loading ...”，显示Loading的进度。
		//
		ShowInitProgress(66);	// 接个前一个状态的进度，此处进度视为66%
								//
								// 开始载入资料
								//
		//Sleep(300);				// 放慢，以便看清楚进度，实际游戏请删除此Sleep
								//
								// 最终进度为100%
								//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnShow()
	{
		CDDraw::BltBackColor(RGB(0, 0, 0));
		CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
		CFont f, *fp;
		f.CreatePointFont(160, "Yu Gothic UI");	// 产生 font f; 160表示16 point的字
		fp = pDC->SelectObject(&f);					// 选用 font f
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 255, 255));
		char str[80];								// Demo 数字对字串的转换
		sprintf(str, "Lives x %d", lives);
		pDC->TextOut(230, 210, str);
		pDC->SelectObject(fp);						// 放掉 font f (千万不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}

	/////////////////////////////////////////////////////////////////////////////
	// 这个class为游戏的游戏执行物件，主要的游戏程式都在这里
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
	{
		enemy.push_back(new CGameEnemy(1));
		enemy.push_back(new CGameEnemy(1));
		/*ball = new CBall [NUMBALLS];*/
	}

	CGameStateRun::~CGameStateRun()
	{
		for (unsigned int i = 0; i < enemy.size(); i++)
			delete enemy[i];
		/*delete [] ball;*/
	}

	void CGameStateRun::OnBeginState()
	{
		/*const int BALL_GAP = 90;
		const int BALL_XY_OFFSET = 45;
		const int BALL_PER_ROW = 7;
		const int HITS_LEFT = 10;
		const int HITS_LEFT_X = 590;
		const int HITS_LEFT_Y = 0;
		const int BACKGROUND_X = 60;
		const int ANIMATION_SPEED = 15;*/
		player.SetXY(0, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - player.Height());              //设定玩家的起始坐标
		enemy[0]->SetXY(11 * BLOCK_WIDTH_HEIGHT, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - 30); //设定怪物的起始坐标 enemy_normal_height = 30
		enemy[1]->SetXY(15 * BLOCK_WIDTH_HEIGHT, SIZE_Y - BLOCK_WIDTH_HEIGHT * 2 - 30);
		map_stage_1.SetScreenX(0);                          //设置地图的起始坐标

		for (unsigned int i = 0; i < enemy.size(); i++)
			enemy[i]->SetIsAlive(true);
		//for (int i = 0; i < NUMBALLS; i++) {				// 设定球的起始座标
		//	int x_pos = i % BALL_PER_ROW;
		//	int y_pos = i / BALL_PER_ROW;
		//	ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		//	ball[i].SetDelay(x_pos);
		//	ball[i].SetIsAlive(true);
		//}
		/*eraser.Initialize();*/
		//background.SetTopLeft(BACKGROUND_X,0);				// 设定背景的起始座标
		//help.SetTopLeft(0, SIZE_Y - help.Height());			// 设定说明图的起始座标
		//hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞击数
		//hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// 指定剩下撞击数的座标
		//CAudio::Instance()->Play(AUDIO_LAKE, true);			// 拨放 WAVE
		//CAudio::Instance()->Play(AUDIO_DING, false);		// 拨放 WAVE
		//CAudio::Instance()->Play(AUDIO_NTUT, true);			// 拨放 MIDI
	}

	void CGameStateRun::OnMove()							// 移动游戏元素
	{
		//
		// 如果希望修改cursor的样式，则将下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		// 移动背景图的座标
		//
		/*if (background.Top() > SIZE_Y)
		background.SetTopLeft(60 ,-background.Height());
		background.SetTopLeft(background.Left(),background.Top()+1);*/
		//
		// 移动球
		//
		/*for (int i=0; i < NUMBALLS; i++)
		ball[i].OnMove();*/
		//
		// 移动擦子
		//
		/*eraser.OnMove();*/
		//
		// 判断擦子是否碰到球
		//
		//for (int i=0; i < NUMBALLS; i++)
		//	if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
		//		ball[i].SetIsAlive(false);
		//		CAudio::Instance()->Play(AUDIO_DING);
		//		hits_left.Add(-1);
		//		//
		//		// 若剩余碰撞次数为0，则跳到Game Over状态
		//		//
		//		if (hits_left.GetInteger() <= 0) {
		//			CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
		//			CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
		//			GotoGameState(GAME_STATE_OVER);
		//		}
		//	}
		//
		// 移动弹跳的球
		//
		/*bball.OnMove();*/
		player.OnMove(&map_stage_1);
		for (unsigned int i = 0; i < enemy.size(); i++)
		{
			enemy[i]->OnMove(&map_stage_1);

			if (enemy[i]->IsAlive() && enemy[i]->HitPlayer(&player))
			{
				if (player.GetY() + player.Height() <= enemy[i]->GetY() + 11)
				{
					player.SetMovingUp(true);
					enemy[i]->SetIsAlive(false);
				}
				else
				{
					player.SetMovingLeft(false);
					player.SetMovingRight(false);
					GotoGameState(GAME_STATE_OVER);
				}

			}
		}

		enemy[0]->OnMove(&map_stage_1);
		enemy[1]->OnMove(&map_stage_1);

		if (player.GetY() - map_stage_1.GetScreenY() > SIZE_Y)
		{
			player.SetMovingLeft(false);
			player.SetMovingRight(false);
			GotoGameState(GAME_STATE_OVER);
		}
	}

	void CGameStateRun::OnInit()  								// 游戏的初值及图形设定
	{
		//
		// 当图很多时，OnInit载入所有的图要花很多时间。为避免玩游戏的人
		//     等的不耐烦，游戏会出现“Loading ...”，显示Loading的进度。
		//
		CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// 将 Back Plain 涂上预设的颜色
		ShowInitProgress(33);	// 接个前一个状态的进度，此处进度视为33%
								//
								// 开始载入资料
								//
								//for (int i = 0; i < NUMBALLS; i++)
								//	ball[i].LoadBitmap();								// 载入第i个球的图形
								//eraser.LoadBitmap();
		map_stage_1.LoadBitmap(); //载入地图
		player.LoadBitmap();
		for (unsigned int i = 0; i < enemy.size(); i++)
			enemy[i]->LoadBitmap();
		/*background.LoadBitmap(IDB_BACKGROUND);*/					// 载入背景的图形
																	//
																	// 完成部分Loading动作，提高进度
																	//
		ShowInitProgress(50);
		//Sleep(300); // 放慢，以便看清楚进度，实际游戏请删除此Sleep
					//
					// 继续载入其他资料
					//
					//help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// 载入说明的图形
					//corner.LoadBitmap(IDB_CORNER);							// 载入角落图形
					//corner.ShowBitmap(background);							// 将corner贴到background
					//bball.LoadBitmap();										// 载入图形
		hits_left.LoadBitmap();
		//CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// 载入编号0的声音ding.wav
		//CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// 载入编号1的声音lake.mp3
		//CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// 载入编号2的声音ntut.mid
		//
		// 此OnInit动作会接到CGameStaterOver::OnInit()，所以进度还没到100%
		//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭头
		const char KEY_UP = 0x26; // keyboard上箭头
		const char KEY_RIGHT = 0x27; // keyboard右箭头
		const char KEY_DOWN = 0x28; // keyboard下箭头

		if (nChar == KEY_LEFT)
			player.SetMovingLeft(true);

		/*eraser.SetMovingLeft(true);*/
		if (nChar == KEY_RIGHT)
			player.SetMovingRight(true);

		/*eraser.SetMovingRight(true);*/
		if (nChar == KEY_UP && !player.IsFalling() && UpClickCount < 1)
		{
			UpClickCount++;
			player.SetMovingUp(true);
		}
		/*eraser.SetMovingUp(true);*/
		/*if (nChar == KEY_DOWN)
		eraser.SetMovingDown(true);*/
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard左箭头
		const char KEY_UP = 0x26; // keyboard上箭头
		const char KEY_RIGHT = 0x27; // keyboard右箭头
		const char KEY_DOWN = 0x28; // keyboard下箭头

		if (nChar == KEY_LEFT)
			player.SetMovingLeft(false);

		/*eraser.SetMovingLeft(false);*/
		if (nChar == KEY_RIGHT)
			player.SetMovingRight(false);

		/*eraser.SetMovingRight(false);*/
		if (nChar == KEY_UP)
		{
			UpClickCount = 0;
			player.SetMovingUp(false);
		}
		/*eraser.SetMovingUp(false);
		if (nChar == KEY_DOWN)
		eraser.SetMovingDown(false);*/
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 处理滑鼠的动作
	{
		/*eraser.SetMovingLeft(true);*/
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 处理滑鼠的动作
	{
		/*eraser.SetMovingLeft(false);*/
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 处理滑鼠的动作
	{
		// 没事。如果需要处理滑鼠移动的话，写code在这里
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 处理滑鼠的动作
	{
		/*eraser.SetMovingRight(true);*/
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 处理滑鼠的动作
	{
		/*eraser.SetMovingRight(false);*/
	}

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show里面千万不要移动任何物件的座标，移动座标的工作应由Move做才对，
		//        否则当视窗重新绘图时(OnDraw)，物件就会移动，看起来会很怪。换个术语
		//        说，Move负责MVC中的Model，Show负责View，而View不应更动Model。
		//
		//
		//  贴上背景图、撞击数、球、擦子、弹跳的球
		//
		map_stage_1.OnShow();
		for (unsigned int i = 0; i < enemy.size(); i++)
			enemy[i]->OnShow(&map_stage_1);
		player.OnShow(&map_stage_1);
		/*background.ShowBitmap();*/			// 贴上背景图
												/*help.ShowBitmap();*/					// 贴上说明图
		hits_left.ShowBitmap();
		//for (int i=0; i < NUMBALLS; i++)
		//	ball[i].OnShow();				// 贴上第i号球
		//bball.OnShow();						// 贴上弹跳的球
		//eraser.OnShow();					// 贴上擦子
		//
		//  贴上左上及右下角落的图
		//
		/*corner.SetTopLeft(0,0);
		corner.ShowBitmap();
		corner.SetTopLeft(SIZE_X-corner.Width(), SIZE_Y-corner.Height());
		corner.ShowBitmap();*/
	}
}