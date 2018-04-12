/*
 * gamelib.h: 本档案储游戏相关的class的interface
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
// 定义游戏可设定的环境与条件
/////////////////////////////////////////////////////////////////////////////

#define SIZE_X				 (33 * 16 + 16)   // 设定游戏画面的解析度为640x480
#define SIZE_Y				 (33 * 15)		// 注：若不使用标准的解析度，则不能切换到全荧幕
#define PLAYER_HEIGHT        54
#define PLAYER_WIDTH         33
#define BLOCK_WIDTH_HEIGHT   33
#define OPEN_AS_FULLSCREEN	 false		// 是否以全荧幕方式开启游戏
#define SHOW_LOAD_PROGRESS   true		// 是否显示loading(OnInit)的进度
#define DEFAULT_BG_COLOR	 RGB(160,180,250)	// 游戏画面预设的背景颜色(黑色)
#define GAME_CYCLE_TIME		 33		    // 每33ms跑一次Move及Show(每秒30次)
#define SHOW_GAME_CYCLE_TIME false		// 是否在debug mode显示cycle time
#define ENABLE_GAME_PAUSE	 false		// 是否允许以 Ctrl-Q 暂停游戏
#define ENABLE_AUDIO		 false		// 启动音效介面

/////////////////////////////////////////////////////////////////////////////
// 定义CGame及CGameState所使用的三个状态常数
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
// 回报程式错误用的macro
// 备注：这里使用macro以便保留档案名称及行号，利于debug。
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
// 这个class提供时间、错误等控制
// 一般的游戏并不需直接操作这个物件，因此可以不管这个class的使用方法
/////////////////////////////////////////////////////////////////////////////

class CSpecialEffect {
public:
	static void  SetCurrentTime();					// 储存目前的时间至ctime
	static DWORD GetEllipseTime();					// 读取目前的时间 - ctime
	static int   GetCurrentTimeCount();				// 读取储存ctime的次数
	static void  Delay(DWORD ms);					// 延迟 x ms
	static void  DelayFromSetCurrentTime(DWORD ms);	// 自ctime起算，延迟 x ms
private:
	static DWORD ctime;
	static int	 ctimeCount;
};

/////////////////////////////////////////////////////////////////////////////
// 这个class会建立DirectDraw物件，以提供其他class使用
// 一般的游戏并不需直接操作这个物件，因此可以不管这个class的使用方法
/////////////////////////////////////////////////////////////////////////////

class CDDraw {
	friend class CMovingBitmap;
public:
	~CDDraw();
	static void  BltBackColor(DWORD);		// 将Back plain全部着上指定的颜色
	static void  BltBackToPrimary();		// 将Back plain贴至Primary plain
	static CDC*  GetBackCDC();				// 取得Back Plain的DC (device context)
	static void  GetClientRect(CRect &r);	// 取得设定的解析度
	static void  Init(int, int);			// Initialize direct draw
	static void  ReleaseBackCDC();			// 放掉Back Plain的DC (device context)
	static bool  SetFullScreen(bool);		// 设定为全荧幕模式/视窗模式
	static bool  IsFullScreen();			// 回答是否为全荧幕模式/视窗模式
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
// 这个class提供动态(可以移动)的图形
// 每个Public Interface的用法都要懂，Implementation可以不懂
/////////////////////////////////////////////////////////////////////////////

class CMovingBitmap {
public:
	CMovingBitmap();
	int   Height();						// 取得图形的高度
	int   Left();						// 取得图形的左上角的 x 座标
	void  LoadBitmap(int,COLORREF=CLR_INVALID);		// 载入图，指定图的编号(resource)及透明色
	void  LoadBitmap(char *,COLORREF=CLR_INVALID);	// 载入图，指定图的档名及透明色
	void  SetTopLeft(int,int);			// 将图的左上角座标移至 (x,y)
	void  ShowBitmap();					// 将图贴到荧幕
	void  ShowBitmap(double factor);	// 将图贴到荧幕 factor < 1时缩小，>1时放大。注意：需要VGA卡硬体的支援，否则会很慢
	void  ShowBitmap(CMovingBitmap &);	// 将图贴到到另一张图上 (仅供特殊用途)
	int   Top();						// 取得图形的左上角的 y 座标
	int   Width();						// 取得图形的宽度
protected:
	CRect    location;			// location of the bitmap
	bool     isBitmapLoaded;	// whether a bitmap has been loaded
	unsigned SurfaceID;			// the surface id of this bitmap
};

/////////////////////////////////////////////////////////////////////////////
// 这个class提供可以移动的动画
// 每个Public Interface的用法都要懂，Implementation可以不懂
/////////////////////////////////////////////////////////////////////////////

class CAnimation {
public:
	CAnimation(int=10);				// Constructor (预设动画播放频率每1/3秒换一张图)
	void  AddBitmap(int,COLORREF=CLR_INVALID);
									// 增加一张图形至动画(图的编号及透明色)
	void  AddBitmap(char *,COLORREF=CLR_INVALID);
									// 增加一张图形至动画(图的编号及透明色)
	int   GetCurrentBitmapNumber();	// 取得正在拨放的bitmap是第几个bitmap
	int   Height();					// 取得动画的高度
	bool  IsFinalBitmap();			// 回传正在拨放的bitmap是否为最后一个bitmap
	int   Left();					// 取得动画的左上角的 x 座标
	void  OnMove();					// 依频率更换bitmap
	void  OnShow();					// 将动画贴到荧幕
	void  Reset();					// 重设播放顺序回到第一张图形
	void  SetDelayCount(int);		// 设定动画播放速度的常数(越大越慢)
	void  SetTopLeft(int,int);		// 将动画的左上角座标移至 (x,y)
	int   Top();					// 取得动画的左上角的 y 座标
	int   Width();					// 取得动画的宽度
private:
	list<CMovingBitmap>				bmp;			// list of CMovingBitmap
	list<CMovingBitmap>::iterator	bmp_iter;		// list iterator
	int								bmp_counter;	// 储存bmp_iter为第n个bmp
	int								delay_counter;	// 延缓动画播放速度的计数器
	int								delay_count;	// 动画播放速度的常数
	int								x, y;			// 动画的座标
};

/////////////////////////////////////////////////////////////////////////////
// 这个class提供显示整数图形的能力
// 每个Public Interface的用法都要懂，Implementation可以不懂
/////////////////////////////////////////////////////////////////////////////

class CInteger {
public:
	CInteger(int=5);			// default 5 digits
	void Add(int n);			// 增加整数值
	int  GetInteger();			// 回传整数值
	void LoadBitmap();			// 载入0..9及负号之图形
	void SetInteger(int);		// 设定整数值
	void SetTopLeft(int,int);	// 将动画的左上角座标移至 (x,y)
	void ShowBitmap();			// 将动画贴到荧幕
private:
	const int NUMDIGITS;			// 共显示NUMDIGITS个位数
	static CMovingBitmap digit[11]; // 储存0..9及负号之图形(bitmap)
	int x, y;						// 显示的座标
	int n;							// 整数值
	bool isBmpLoaded;				// 是否已经载入图形
};

/////////////////////////////////////////////////////////////////////////////
// 宣告尚未定义的class
/////////////////////////////////////////////////////////////////////////////

class CGame;
class CGameStateInit;
class CGameStateRun;
class CGameStateOver;

/////////////////////////////////////////////////////////////////////////////
// 这个class为游戏的各种状态之Base class(是一个abstract class)
// 每个Public Interface的用法都要懂，Implementation可以不懂
/////////////////////////////////////////////////////////////////////////////

class CGameState {
public:
	CGameState(CGame *g);
	void OnDraw();			// Template Method
	void OnCycle();			// Template Method
	//
	// virtual functions, 由继承者提供implementation
	//
	virtual ~CGameState() {}								// virtual destructor
	virtual void OnBeginState() {}							// 设定每次进入这个状态时所需的初值
	virtual void OnInit() {}								// 状态的初值及图形设定
	virtual void OnKeyDown(UINT, UINT, UINT) {}				// 处理键盘Down的动作
	virtual void OnKeyUp(UINT, UINT, UINT) {}				// 处理键盘Up的动作
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {}// 处理滑鼠的动作
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {}	// 处理滑鼠的动作
	virtual void OnMouseMove(UINT nFlags, CPoint point) {}  // 处理滑鼠的动作 
	virtual void OnRButtonDown(UINT nFlags, CPoint point) {}// 处理滑鼠的动作
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {}	// 处理滑鼠的动作
protected:
	void GotoGameState(int state);							// 跳跃至指定的state
	void ShowInitProgress(int percent);						// 显示初始化的进度
	//
	// virtual functions, 由继承者提供implementation
	//
	virtual void OnMove() {}								// 移动这个状态的游戏元素
	virtual void OnShow() = 0;								// 显示这个状态的游戏画面
	CGame *game;
};

/////////////////////////////////////////////////////////////////////////////
// 这个class是游戏的核心，控制游戏的进行
// 一般的游戏并不需直接操作这个物件，因此可以不管这个class的使用方法
/////////////////////////////////////////////////////////////////////////////

class CGame {
public:
	CGame();										// Constructor
	~CGame();										// Destructor
	bool IsRunning();								// 读取游戏是否正在进行中
	void OnDraw();									// 对应CGameView的OnDraw()
	void OnFilePause();								// 游戏暂停
	void OnInit();									// 游戏绘图及音效的初始化
	void OnInitStates();							// 游戏各状态的初值及图形设定
	bool OnIdle();									// 游戏的主回圈
	void OnKeyDown(UINT, UINT, UINT);				// 处理键盘Down的动作
	void OnKeyUp(UINT, UINT, UINT);					// 处理键盘Up的动作
	void OnKillFocus();								// 游戏被迫暂停
	void OnLButtonDown(UINT nFlags, CPoint point);	// 处理滑鼠的动作
	void OnLButtonUp(UINT nFlags, CPoint point);	// 处理滑鼠的动作
	void OnMouseMove(UINT nFlags, CPoint point);    // 处理滑鼠的动作 
	void OnRButtonDown(UINT nFlags, CPoint point);	// 处理滑鼠的动作
	void OnRButtonUp(UINT nFlags, CPoint point);	// 处理滑鼠的动作
	void OnResume();								// 处理自“待命”还原的动作
	void OnSetFocus();								// 处理Focus
	void OnSuspend();								// 处理“待命”的动作
	void SetGameState(int);
	static CGame *Instance();
private:
	bool			running;			// 游戏是否正在进行中(未被Pause)
	bool            suspended;			// 游戏是否被suspended
	const int		NUM_GAME_STATES;	// 游戏的状态数(3个状态)
	CGameState		*gameState;			// pointer指向目前的游戏状态
	CGameState		*gameStateTable[3];	// 游戏状态物件的pointer
	static CGame	instance;			// 游戏唯一的instance
};

}