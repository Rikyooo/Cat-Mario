/*
 * mygame.h: ����������Ϸ�����class��interface
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
// ���classΪ��Ϸ����Ϸ��ͷ�������
// ÿ��Member function��Implementation��ҪŪ��
/////////////////////////////////////////////////////////////////////////////

class CGameStateInit : public CGameState
{
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// ��Ϸ�ĳ�ֵ��ͼ���趨
        void OnBeginState();							// �趨ÿ����������ı���
        void OnKeyUp(UINT, UINT, UINT); 				// �������Up�Ķ���
    protected:
        void OnShow();									// ��ʾ���״̬����Ϸ����
    private:
        //CMovingBitmap logo;								// csie��logo
		CMovingBitmap block_floor_1_ground_bmp;
		CMovingBitmap block_floor_2_ground_bmp;
		CMovingBitmap player_bmp;
		CMovingBitmap bush_top_bmp, bush_center_1_bmp, bush_center_2_bmp, bush_left_bmp, bush_right_bmp;
		CMovingBitmap grass_left_bmp, grass_right_bmp;
		CMovingBitmap title_bmp;
};

/////////////////////////////////////////////////////////////////////////////
// ���classΪ��Ϸ����Ϸִ���������Ҫ����Ϸ��ʽ��������
// ÿ��Member function��Implementation��ҪŪ��
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState
{
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �趨ÿ����������ı���
        void OnInit();  								// ��Ϸ�ĳ�ֵ��ͼ���趨
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // ������Ķ���
        void OnLButtonUp(UINT nFlags, CPoint point);	// ������Ķ���
        void OnMouseMove(UINT nFlags, CPoint point);	// ������Ķ���
        void OnRButtonDown(UINT nFlags, CPoint point);  // ������Ķ���
        void OnRButtonUp(UINT nFlags, CPoint point);	// ������Ķ���
    protected:
        void OnMove();									// �ƶ���ϷԪ��
        void OnShow();									// ��ʾ���״̬����Ϸ����
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
// ���classΪ��Ϸ�Ľ���״̬(Game Over)
// ÿ��Member function��Implementation��ҪŪ��
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState
{
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// �趨ÿ����������ı���
        void OnInit();
    protected:
        void OnMove();									// �ƶ���ϷԪ��
        void OnShow();									// ��ʾ���״̬����Ϸ����
    private:
        int counter;	// ����֮������
		int lives = 3;
};
}