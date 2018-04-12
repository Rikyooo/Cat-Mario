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

namespace game_framework
{
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID  				// 定义各种音效的编号
{
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT				// 2
};

//map class
class CGameMap
{
public:
	CGameMap();
	void LoadBitmap();
	void OnShow();
	void OnDraw();
	bool IsEmpty(int, int);
	void SetScreenX(int);
	int GetScreenX();
	int GetScreenY();

protected:
	static const int BLOCK_ROW = 15;
	static const int BLOCK_COLUMN = 54;
	CMovingBitmap block_floor_1_ground,
		block_floor_2_ground,
		block_fragile_ground,
		block_sturdy_ground,
		block_item_1_common,
		block_item_2_ground,
		mountain,
		cloud_1,
		brush;
	int map_index[BLOCK_ROW][BLOCK_COLUMN];
	int screen_x, screen_y;   //屏幕左上角在地图上的点坐标
};

class CGamePlayer
{
public:
	CGamePlayer();
	void LoadBitmap();
	void OnMove(CGameMap *map);
	void OnShow(CGameMap *map);
	void SetMovingLeft(bool);
	void SetMovingRight(bool);
	void SetMovingUp(bool);
	void SetXY(int, int);
	int GetX();
	int GetY();
	void SetJump(bool);
	bool GetJump();
private:
	CMovingBitmap player;
	int x, y;   //玩家在地图上的点坐标
	bool isMovingLeft = false;
	bool isMovingRight = false;
	bool isMovingUp = false;
	int jumpingCount = 0;
	//int fallSpeed = 0;
	bool isFalling = false;
	bool isJumping = true;
};

class CGameEnemy
{
public:
	CGameEnemy(int);
	void LoadBitmap();
	void OnMove(CGameMap *map);
	void OnShow(CGameMap *map);
	void SetXY(int, int);
	int GetX();
	int GetY();
	bool IsAlive();
	void SetIsAlive(bool);
	bool HitPlayer(CGamePlayer *player);
private:
	CMovingBitmap enemy;
	int x, y; //怪在地图上的点坐标
	int movingSpeed;
	bool isAlive;
};

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
        void OnLButtonDown(UINT nFlags, CPoint point);  // 处理滑鼠的动作
    protected:
        void OnShow();									// 显示这个状态的游戏画面
    private:
        //CMovingBitmap logo;								// csie的logo
		CMovingBitmap block_floor_1_ground;
		CMovingBitmap block_floor_2_ground;
		CMovingBitmap player;
		CMovingBitmap title;
		CMovingBitmap mountain;
		CMovingBitmap brush;
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
        //const int		NUMBALLS;	// 球的总数
        CMovingBitmap	background;	// 背景图
        CMovingBitmap	help;		// 说明图
        //CBall			*ball;		// 球的阵列
        CMovingBitmap	corner;		// 角落图
        //CEraser			eraser;		// 拍子
        CInteger		hits_left;	// 剩下的撞击数
        //CBouncingBall   bball;		// 反覆弹跳的球
        CGameMap map_stage_1;
		CGamePlayer player;
		vector<CGameEnemy*> enemy;
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