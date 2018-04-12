/*
 * gamelib.h: ����������Ϸ��ص�class��interface
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
 *		1. Add ENABLE_AUDIO flag.
 *   2004-03-02 V4.0
 *		1. Add CInteger class.
 *      2. Add CGameState abstract class and adjust CGame to apply
 *         state pattern for switching game states.
 *   2004-03-08 V4.1
 *      1. Add OPEN_AS_FULLSCREEN flag.
 *      2. The Game Engine now becomes a framework.
 *   2005-07-28
 *      1. Add GAME_ASSERT macro to display run time errors gracefully.
 *      2. Remove CDDarw::CheckSystem(). It is useless.
 *   2005-09-08
 *      1. Eliminate CSpecialEffect::Abort. It is useless now.
 *      2. Add SHOW_GAME_CYCLE_TIME flag.
 *      3. Add DEFAULT_BG_COLOR flag;
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add MOUSEMOVE Handler for CGame and CGameState.
 *      2. Add _TRACE preprocessor flag for VC++.net.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Remove constantness of CAnimation::DELAY_COUNT.
 *      3. Enhance CAnimation to support SetDelayCount(), Reset(), and IsFinalBitmap().
 *      4. Remove CAnimation::GetLocation() and CMovingBitmap::GetLocation().
 *      5. Bitmap coordinate can no longer be set by CMovingBitmap::LoadBitmap().
 *         Defauts to (0,0).
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *      2. Fix bug: audio is now correctly recovered after a sleep or suspension of windows.
 *      3. Support ENABLE_GAME_PAUSE.
 *   2008-02-15 V4.4
 *      1. Add setup project for Visual studio 2005.
 *      2. Support bitmap scaling when ShowBitmap(scale) is called.
 *      3. Add namespace game_framework.
 *      4. Make the class CGame a singleton so that MFC can access it easily.
 *      5. Support loading of bitmap from bmp file.
 *      6. Support ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Rewrite CAudio with MCI commands to eliminate dependency with DirectMusic.
*/

/////////////////////////////////////////////////////////////////////////////
// ������Ϸ���趨�Ļ���������
/////////////////////////////////////////////////////////////////////////////

#define SIZE_X				 (33 * 16 + 16)   // �趨��Ϸ����Ľ�����Ϊ640x480
#define SIZE_Y				 (33 * 15)		// ע������ʹ�ñ�׼�Ľ����ȣ������л���ȫӫĻ
#define PLAYER_HEIGHT        54
#define PLAYER_WIDTH         33
#define BLOCK_WIDTH_HEIGHT   33
#define OPEN_AS_FULLSCREEN	 false		// �Ƿ���ȫӫĻ��ʽ������Ϸ
#define SHOW_LOAD_PROGRESS   true		// �Ƿ���ʾloading(OnInit)�Ľ���
#define DEFAULT_BG_COLOR	 RGB(160,180,250)	// ��Ϸ����Ԥ��ı�����ɫ(��ɫ)
#define GAME_CYCLE_TIME		 33		    // ÿ33ms��һ��Move��Show(ÿ��30��)
#define SHOW_GAME_CYCLE_TIME false		// �Ƿ���debug mode��ʾcycle time
#define ENABLE_GAME_PAUSE	 false		// �Ƿ������� Ctrl-Q ��ͣ��Ϸ
#define ENABLE_AUDIO		 false		// ������Ч����

/////////////////////////////////////////////////////////////////////////////
// ����CGame��CGameState��ʹ�õ�����״̬����
/////////////////////////////////////////////////////////////////////////////

enum GAME_STATES {
	GAME_STATE_INIT,
	GAME_STATE_RUN,
	GAME_STATE_OVER
};

/////////////////////////////////////////////////////////////////////////////
// Header for STL (Standard Template Library)
/////////////////////////////////////////////////////////////////////////////

#include <list>
#include <vector>
#include <map>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// �ر���ʽ�����õ�macro
// ��ע������ʹ��macro�Ա㱣���������Ƽ��кţ�����debug��
/////////////////////////////////////////////////////////////////////////////

#define GAME_ASSERT(boolexp,str)											\
		if (!(boolexp)) {													\
			int id;															\
			char s[300]="";													\
			sprintf(s,"Game fatal error:\n\n%s\n\nFile: %s\n\nLine: %d"		\
				"\n\n(Press Retry to debug the application, "				\
				"if it is executed in debug mode.)"							\
				"\n(Press Cancel otherwise.)",								\
				 str , __FILE__,__LINE__);									\
			id = AfxMessageBox(s, MB_RETRYCANCEL);							\
			if (id == IDCANCEL)												\
				exit(1);													\
			AfxDebugBreak();												\
		}

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// ���class�ṩʱ�䡢����ȿ���
// һ�����Ϸ������ֱ�Ӳ�������������˿��Բ������class��ʹ�÷���
/////////////////////////////////////////////////////////////////////////////

class CSpecialEffect {
public:
	static void  SetCurrentTime();					// ����Ŀǰ��ʱ����ctime
	static DWORD GetEllipseTime();					// ��ȡĿǰ��ʱ�� - ctime
	static int   GetCurrentTimeCount();				// ��ȡ����ctime�Ĵ���
	static void  Delay(DWORD ms);					// �ӳ� x ms
	static void  DelayFromSetCurrentTime(DWORD ms);	// ��ctime���㣬�ӳ� x ms
private:
	static DWORD ctime;
	static int	 ctimeCount;
};

/////////////////////////////////////////////////////////////////////////////
// ���class�Ὠ��DirectDraw��������ṩ����classʹ��
// һ�����Ϸ������ֱ�Ӳ�������������˿��Բ������class��ʹ�÷���
/////////////////////////////////////////////////////////////////////////////

class CDDraw {
	friend class CMovingBitmap;
public:
	~CDDraw();
	static void  BltBackColor(DWORD);		// ��Back plainȫ������ָ������ɫ
	static void  BltBackToPrimary();		// ��Back plain����Primary plain
	static CDC*  GetBackCDC();				// ȡ��Back Plain��DC (device context)
	static void  GetClientRect(CRect &r);	// ȡ���趨�Ľ�����
	static void  Init(int, int);			// Initialize direct draw
	static void  ReleaseBackCDC();			// �ŵ�Back Plain��DC (device context)
	static bool  SetFullScreen(bool);		// �趨ΪȫӫĻģʽ/�Ӵ�ģʽ
	static bool  IsFullScreen();			// �ش��Ƿ�ΪȫӫĻģʽ/�Ӵ�ģʽ
private:
	CDDraw();								// private constructor
	static void  BltBitmapToBack(unsigned SurfaceID, int x, int y);
	static void  BltBitmapToBack(unsigned SurfaceID, int x, int y, double factor);
	static void  BltBitmapToBitmap(unsigned SourceID, unsigned TargetID, int x, int y);
	static void	 CheckDDFail(char *s);
	static bool  CreateSurface();
	static bool  CreateSurfaceFullScreen();
	static bool  CreateSurfaceWindowed();
	static void  LoadBitmap(int i, int IDB_BITMAP);
	static void  LoadBitmap(int i, char *filename);
	static DWORD MatchColorKey(LPDIRECTDRAWSURFACE lpDDSurface, COLORREF color);
	static int   RegisterBitmap(int IDB_BITMAP, COLORREF ColorKey);
	static int   RegisterBitmap(char *filename, COLORREF ColorKey);
	static void  ReleaseSurface();
	static void  RestoreSurface();
	static void  SetColorKey(unsigned SurfaceID, COLORREF color);
    static HDC					hdc;
	static CDC					cdc;
	static CView				*pCView;
    static LPDIRECTDRAW2		lpDD;
	static LPDIRECTDRAWCLIPPER	lpClipperPrimary;   
	static LPDIRECTDRAWCLIPPER	lpClipperBack;   
	static LPDIRECTDRAWSURFACE	lpDDSPrimary;
	static LPDIRECTDRAWSURFACE	lpDDSBack;
	static vector<LPDIRECTDRAWSURFACE>	lpDDS;
    static HRESULT				ddrval;
	static vector<int>			BitmapID;
	static vector<string>		BitmapName;
	static vector<CRect>		BitmapRect;
	static vector<COLORREF>		BitmapColorKey;
	static bool					fullscreen;
	static CDDraw				ddraw;
	static int					size_x, size_y;
};

/////////////////////////////////////////////////////////////////////////////
// ���class�ṩ��̬(�����ƶ�)��ͼ��
// ÿ��Public Interface���÷���Ҫ����Implementation���Բ���
/////////////////////////////////////////////////////////////////////////////

class CMovingBitmap {
public:
	CMovingBitmap();
	int   Height();						// ȡ��ͼ�εĸ߶�
	int   Left();						// ȡ��ͼ�ε����Ͻǵ� x ����
	void  LoadBitmap(int,COLORREF=CLR_INVALID);		// ����ͼ��ָ��ͼ�ı��(resource)��͸��ɫ
	void  LoadBitmap(char *,COLORREF=CLR_INVALID);	// ����ͼ��ָ��ͼ�ĵ�����͸��ɫ
	void  SetTopLeft(int,int);			// ��ͼ�����Ͻ��������� (x,y)
	void  ShowBitmap();					// ��ͼ����ӫĻ
	void  ShowBitmap(double factor);	// ��ͼ����ӫĻ factor < 1ʱ��С��>1ʱ�Ŵ�ע�⣺��ҪVGA��Ӳ���֧Ԯ����������
	void  ShowBitmap(CMovingBitmap &);	// ��ͼ��������һ��ͼ�� (����������;)
	int   Top();						// ȡ��ͼ�ε����Ͻǵ� y ����
	int   Width();						// ȡ��ͼ�εĿ��
protected:
	CRect    location;			// location of the bitmap
	bool     isBitmapLoaded;	// whether a bitmap has been loaded
	unsigned SurfaceID;			// the surface id of this bitmap
};

/////////////////////////////////////////////////////////////////////////////
// ���class�ṩ�����ƶ��Ķ���
// ÿ��Public Interface���÷���Ҫ����Implementation���Բ���
/////////////////////////////////////////////////////////////////////////////

class CAnimation {
public:
	CAnimation(int=10);				// Constructor (Ԥ�趯������Ƶ��ÿ1/3�뻻һ��ͼ)
	void  AddBitmap(int,COLORREF=CLR_INVALID);
									// ����һ��ͼ��������(ͼ�ı�ż�͸��ɫ)
	void  AddBitmap(char *,COLORREF=CLR_INVALID);
									// ����һ��ͼ��������(ͼ�ı�ż�͸��ɫ)
	int   GetCurrentBitmapNumber();	// ȡ�����ڲ��ŵ�bitmap�ǵڼ���bitmap
	int   Height();					// ȡ�ö����ĸ߶�
	bool  IsFinalBitmap();			// �ش����ڲ��ŵ�bitmap�Ƿ�Ϊ���һ��bitmap
	int   Left();					// ȡ�ö��������Ͻǵ� x ����
	void  OnMove();					// ��Ƶ�ʸ���bitmap
	void  OnShow();					// ����������ӫĻ
	void  Reset();					// ���貥��˳��ص���һ��ͼ��
	void  SetDelayCount(int);		// �趨���������ٶȵĳ���(Խ��Խ��)
	void  SetTopLeft(int,int);		// �����������Ͻ��������� (x,y)
	int   Top();					// ȡ�ö��������Ͻǵ� y ����
	int   Width();					// ȡ�ö����Ŀ��
private:
	list<CMovingBitmap>				bmp;			// list of CMovingBitmap
	list<CMovingBitmap>::iterator	bmp_iter;		// list iterator
	int								bmp_counter;	// ����bmp_iterΪ��n��bmp
	int								delay_counter;	// �ӻ����������ٶȵļ�����
	int								delay_count;	// ���������ٶȵĳ���
	int								x, y;			// ����������
};

/////////////////////////////////////////////////////////////////////////////
// ���class�ṩ��ʾ����ͼ�ε�����
// ÿ��Public Interface���÷���Ҫ����Implementation���Բ���
/////////////////////////////////////////////////////////////////////////////

class CInteger {
public:
	CInteger(int=5);			// default 5 digits
	void Add(int n);			// ��������ֵ
	int  GetInteger();			// �ش�����ֵ
	void LoadBitmap();			// ����0..9������֮ͼ��
	void SetInteger(int);		// �趨����ֵ
	void SetTopLeft(int,int);	// �����������Ͻ��������� (x,y)
	void ShowBitmap();			// ����������ӫĻ
private:
	const int NUMDIGITS;			// ����ʾNUMDIGITS��λ��
	static CMovingBitmap digit[11]; // ����0..9������֮ͼ��(bitmap)
	int x, y;						// ��ʾ������
	int n;							// ����ֵ
	bool isBmpLoaded;				// �Ƿ��Ѿ�����ͼ��
};

/////////////////////////////////////////////////////////////////////////////
// ������δ�����class
/////////////////////////////////////////////////////////////////////////////

class CGame;
class CGameStateInit;
class CGameStateRun;
class CGameStateOver;

/////////////////////////////////////////////////////////////////////////////
// ���classΪ��Ϸ�ĸ���״̬֮Base class(��һ��abstract class)
// ÿ��Public Interface���÷���Ҫ����Implementation���Բ���
/////////////////////////////////////////////////////////////////////////////

class CGameState {
public:
	CGameState(CGame *g);
	void OnDraw();			// Template Method
	void OnCycle();			// Template Method
	//
	// virtual functions, �ɼ̳����ṩimplementation
	//
	virtual ~CGameState() {}								// virtual destructor
	virtual void OnBeginState() {}							// �趨ÿ�ν������״̬ʱ����ĳ�ֵ
	virtual void OnInit() {}								// ״̬�ĳ�ֵ��ͼ���趨
	virtual void OnKeyDown(UINT, UINT, UINT) {}				// �������Down�Ķ���
	virtual void OnKeyUp(UINT, UINT, UINT) {}				// �������Up�Ķ���
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {}// ������Ķ���
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {}	// ������Ķ���
	virtual void OnMouseMove(UINT nFlags, CPoint point) {}  // ������Ķ��� 
	virtual void OnRButtonDown(UINT nFlags, CPoint point) {}// ������Ķ���
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {}	// ������Ķ���
protected:
	void GotoGameState(int state);							// ��Ծ��ָ����state
	void ShowInitProgress(int percent);						// ��ʾ��ʼ���Ľ���
	//
	// virtual functions, �ɼ̳����ṩimplementation
	//
	virtual void OnMove() {}								// �ƶ����״̬����ϷԪ��
	virtual void OnShow() = 0;								// ��ʾ���״̬����Ϸ����
	CGame *game;
};

/////////////////////////////////////////////////////////////////////////////
// ���class����Ϸ�ĺ��ģ�������Ϸ�Ľ���
// һ�����Ϸ������ֱ�Ӳ�������������˿��Բ������class��ʹ�÷���
/////////////////////////////////////////////////////////////////////////////

class CGame {
public:
	CGame();										// Constructor
	~CGame();										// Destructor
	bool IsRunning();								// ��ȡ��Ϸ�Ƿ����ڽ�����
	void OnDraw();									// ��ӦCGameView��OnDraw()
	void OnFilePause();								// ��Ϸ��ͣ
	void OnInit();									// ��Ϸ��ͼ����Ч�ĳ�ʼ��
	void OnInitStates();							// ��Ϸ��״̬�ĳ�ֵ��ͼ���趨
	bool OnIdle();									// ��Ϸ������Ȧ
	void OnKeyDown(UINT, UINT, UINT);				// �������Down�Ķ���
	void OnKeyUp(UINT, UINT, UINT);					// �������Up�Ķ���
	void OnKillFocus();								// ��Ϸ������ͣ
	void OnLButtonDown(UINT nFlags, CPoint point);	// ������Ķ���
	void OnLButtonUp(UINT nFlags, CPoint point);	// ������Ķ���
	void OnMouseMove(UINT nFlags, CPoint point);    // ������Ķ��� 
	void OnRButtonDown(UINT nFlags, CPoint point);	// ������Ķ���
	void OnRButtonUp(UINT nFlags, CPoint point);	// ������Ķ���
	void OnResume();								// �����ԡ���������ԭ�Ķ���
	void OnSetFocus();								// ����Focus
	void OnSuspend();								// �����������Ķ���
	void SetGameState(int);
	static CGame *Instance();
private:
	bool			running;			// ��Ϸ�Ƿ����ڽ�����(δ��Pause)
	bool            suspended;			// ��Ϸ�Ƿ�suspended
	const int		NUM_GAME_STATES;	// ��Ϸ��״̬��(3��״̬)
	CGameState		*gameState;			// pointerָ��Ŀǰ����Ϸ״̬
	CGameState		*gameStateTable[3];	// ��Ϸ״̬�����pointer
	static CGame	instance;			// ��ϷΨһ��instance
};

}