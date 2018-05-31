/*
 * mygame.h: 本档案储游戏本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

#include "CGameMap.h"
#include "CGamePlayer.h"

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// 这个class为游戏的游戏开头画面物件
// 每个Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// 游戏的初值及图形设定
        void OnBeginState();							// 设定每次重玩所需的变数
        void OnKeyUp(UINT, UINT, UINT); 				// 处理键盘Up的动作
    protected:
        void OnShow();									// 显示这个状态的游戏画面
    private:
        //CMovingBitmap logo;								// csie的logo
		CMovingBitmap block_floor_1_ground_bmp;
		CMovingBitmap block_floor_2_ground_bmp;
		CMovingBitmap player_bmp;
		CMovingBitmap bush_top_bmp, bush_center_1_bmp, bush_center_2_bmp, bush_left_bmp, bush_right_bmp;
		CMovingBitmap grass_left_bmp, grass_right_bmp;
		CMovingBitmap title_bmp;
};

/////////////////////////////////////////////////////////////////////////////
// 这个class为游戏的游戏执行物件，主要的游戏程式都在这里
// 每个Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 设定每次重玩所需的变数
        void OnInit();  								// 游戏的初值及图形设定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 处理滑鼠的动作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 处理滑鼠的动作
        void OnMouseMove(UINT nFlags, CPoint point);	// 处理滑鼠的动作
        void OnRButtonDown(UINT nFlags, CPoint point);  // 处理滑鼠的动作
        void OnRButtonUp(UINT nFlags, CPoint point);	// 处理滑鼠的动作
    protected:
        void OnMove();									// 移动游戏元素
        void OnShow();									// 显示这个状态的游戏画面
    private:
        CGameMap *map;
		bool isLeftPressed = false;
		bool isRightPressed = false;
		bool firstDir; // ----- true = RIGHT, false = LEFT
		/*CGamePlayer bmp;
		vector<CGameEnemy*> bmp;*/
		int UpClickCount = 0;
};

/////////////////////////////////////////////////////////////////////////////
// 这个class为游戏的结束状态(Game Over)
// 每个Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 设定每次重玩所需的变数
        void OnInit();
    protected:
        void OnMove();									// 移动游戏元素
        void OnShow();									// 显示这个状态的游戏画面
    private:
        int counter;	// 倒数之计数器
		int lives = 3;
};
}