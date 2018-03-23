//=========================================================
//程序名称：我的魔塔
//作者：方寸
//说明：无
//日期：2012-12-14
//=========================================================
#include <iostream>
#include "Globals.h"//全局定义

using namespace std;

//=========================================================
//全局变量
//=========================================================
HINSTANCE		g_hInstance;				//程序句柄
HWND			g_hMainWnd;					//主窗口句柄
DWORD			g_GameControlThreadID;		//游戏控制线程ID
HDC				g_hMainWndDC;				//主窗口设备上下文
bool			g_bKeyDown[256];			//按键是否按下
SPaintPage		g_GamePage;					//游戏画面
SPaintPage		g_TempPage;					//临时页面
SPaintPage		g_MapPage[2];				//游戏地图
char			g_strPicDir[MAX_PATH];		//图片文件夹路径
char			g_strSoundDir[MAX_PATH];	//音乐文件夹路径
char			g_strMapDir[MAX_PATH];		//地图文件夹路径
SBMP			g_MapBmp[2];				//地图位图资源
SBMP			g_RoleBmp;					//角色位图资源
int				g_Map[MAP_COUNT][TITLE_X_COUNT][TITLE_Y_COUNT];//地图数据
RoleInfo		g_RoleInfo;					//角色信息
EnemyInfo		g_EnemyInfo[ENEMY_COUNT];	//敌人信息
bool			g_bShowMap2;				//用地图2显示
int				g_x,g_y;					//全局坐标传递
int				g_Result;					//全局游戏结果传递
MagicList		g_MagicList;				//魔法表
GoodsList		g_GoodsList;				//物品表
POINT			g_UpStair[MAP_COUNT+1];		//上楼梯坐标
POINT			g_DownStair[MAP_COUNT+1];	//下楼梯坐标

//=========================================================
//函数名称：WinMain
//说明：主函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-12-14
//修改记录：
//=========================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	WNDCLASS wc;
	MSG Msg;
	char strFileName[MAX_PATH];
	string strFile;

	g_hInstance=hInstance;

	//注册窗口类
	wc.lpszClassName=WND_CLASSNAME;
	wc.hInstance=hInstance;
	wc.lpszMenuName=NULL;
	wc.lpfnWndProc=MainWndProc;
	wc.style=CS_OWNDC|CS_SAVEBITS;
	wc.hIcon=LoadIcon(hInstance,"ICON");
	wc.hCursor=LoadCursor(hInstance,"ARROW");
	wc.hbrBackground=(HBRUSH)COLOR_WINDOW;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	
	if(!RegisterClass(&wc))
	{
		MessageBox(NULL,"注册窗口类失败！",WND_CAPTION,MB_OK);

		return 1;
	}

	g_hMainWnd=CreateWindow(WND_CLASSNAME,WND_CAPTION,WS_VISIBLE|WS_POPUP,
								0,0,WND_W,WND_H,
								NULL,NULL,hInstance,NULL);
	if(!g_hMainWnd)
	{
		MessageBox(NULL,"创建主窗口失败！",WND_CAPTION,MB_OK);

		return 1;
	}

	srand(GetTickCount());//初始化随机数种子

	g_hMainWndDC=GetDC(g_hMainWnd);
	g_GamePage.SetPage(WND_W,WND_H);//建立游戏绘制缓冲页面
	g_TempPage.SetPage(WND_W,WND_H);//建立临时页面
	//建立地图缓冲页面
	g_MapPage[0].SetPage(TITLE_W*TITLE_X_COUNT,TITLE_H*TITLE_Y_COUNT);
	g_MapPage[1].SetPage(TITLE_W*TITLE_X_COUNT,TITLE_H*TITLE_Y_COUNT);

	SetWindowPos(g_hMainWnd,HWND_TOPMOST,0,0,WND_W,WND_H,SWP_SHOWWINDOW);

	//获取资源路径
	GetModuleFileName(NULL,strFileName,MAX_PATH);
	GetFileDir(strFileName,g_strPicDir);//图片文件夹路径
	GetFileDir(strFileName,g_strSoundDir);//音乐文件夹路径
	GetFileDir(strFileName,g_strMapDir);//音乐文件夹路径
	strcat(g_strPicDir,DIR_PIC);
	strcat(g_strSoundDir,DIR_SOUND);
	strcat(g_strMapDir,DIR_MAP);

	//载入地图资源
	strFile="";
	strFile+=g_strPicDir;
	strFile+="Map1.bmp";
	g_MapBmp[0]=LoadBmp(strFile.c_str());
	
	strFile="";
	strFile+=g_strPicDir;
	strFile+="Map2.bmp";
	g_MapBmp[1]=LoadBmp(strFile.c_str());

	//载入角色图片
	strFile="";
	strFile+=g_strPicDir;
	strFile+="Role.bmp";
	g_RoleBmp=LoadBmp(strFile.c_str());

	CreateThread(NULL,0,GameControl,NULL,NULL,&g_GameControlThreadID);//创建游戏控制线程

	while(GetMessage(&Msg,NULL,0,0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	DeleteBmp(g_MapBmp[0]);
	DeleteBmp(g_MapBmp[1]);
	DeleteBmp(g_RoleBmp);

	ReleaseDC(g_hMainWnd,g_hMainWndDC);

	return 0;
}

//=========================================================
//函数名称：MainWndProc
//说明：主窗口消息循环
//输入：
//输出：
//返回：
//备注：
//日期：2011-12-14
//修改记录：
//=========================================================
LRESULT CALLBACK MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	//=========================================================
	//WM_PAINT
	//=========================================================
	case WM_PAINT:
		g_GamePage.CopyToDC(g_hMainWndDC);
		break;
	//=========================================================
	//WM_KEYDOWN
	//=========================================================
	case WM_KEYDOWN:
		g_bKeyDown[(int)wParam]=true;
		break;
	//=========================================================
	//WM_KEYUP
	//=========================================================
	case WM_KEYUP:
		g_bKeyDown[(int)wParam]=false;
		break;
	//=========================================================
	//WM_SETFOCUS
	//=========================================================
	case WM_SETFOCUS:
		SetDisplay(WND_W,WND_H);
		break;
	//=========================================================
	//WM_KILLFOCUS
	//=========================================================
	case WM_KILLFOCUS:
		ShowWindow(hWnd,SW_MINIMIZE);
		ChangeDisplaySettings(NULL,NULL);
		break;
	//=========================================================
	//WM_CLOSE
	//=========================================================
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
