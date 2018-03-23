//=========================================================
//�������ƣ��ҵ�ħ��
//���ߣ�����
//˵������
//���ڣ�2012-12-14
//=========================================================
#include <iostream>
#include "Globals.h"//ȫ�ֶ���

using namespace std;

//=========================================================
//ȫ�ֱ���
//=========================================================
HINSTANCE		g_hInstance;				//������
HWND			g_hMainWnd;					//�����ھ��
DWORD			g_GameControlThreadID;		//��Ϸ�����߳�ID
HDC				g_hMainWndDC;				//�������豸������
bool			g_bKeyDown[256];			//�����Ƿ���
SPaintPage		g_GamePage;					//��Ϸ����
SPaintPage		g_TempPage;					//��ʱҳ��
SPaintPage		g_MapPage[2];				//��Ϸ��ͼ
char			g_strPicDir[MAX_PATH];		//ͼƬ�ļ���·��
char			g_strSoundDir[MAX_PATH];	//�����ļ���·��
char			g_strMapDir[MAX_PATH];		//��ͼ�ļ���·��
SBMP			g_MapBmp[2];				//��ͼλͼ��Դ
SBMP			g_RoleBmp;					//��ɫλͼ��Դ
int				g_Map[MAP_COUNT][TITLE_X_COUNT][TITLE_Y_COUNT];//��ͼ����
RoleInfo		g_RoleInfo;					//��ɫ��Ϣ
EnemyInfo		g_EnemyInfo[ENEMY_COUNT];	//������Ϣ
bool			g_bShowMap2;				//�õ�ͼ2��ʾ
int				g_x,g_y;					//ȫ�����괫��
int				g_Result;					//ȫ����Ϸ�������
MagicList		g_MagicList;				//ħ����
GoodsList		g_GoodsList;				//��Ʒ��
POINT			g_UpStair[MAP_COUNT+1];		//��¥������
POINT			g_DownStair[MAP_COUNT+1];	//��¥������

//=========================================================
//�������ƣ�WinMain
//˵����������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-12-14
//�޸ļ�¼��
//=========================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	WNDCLASS wc;
	MSG Msg;
	char strFileName[MAX_PATH];
	string strFile;

	g_hInstance=hInstance;

	//ע�ᴰ����
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
		MessageBox(NULL,"ע�ᴰ����ʧ�ܣ�",WND_CAPTION,MB_OK);

		return 1;
	}

	g_hMainWnd=CreateWindow(WND_CLASSNAME,WND_CAPTION,WS_VISIBLE|WS_POPUP,
								0,0,WND_W,WND_H,
								NULL,NULL,hInstance,NULL);
	if(!g_hMainWnd)
	{
		MessageBox(NULL,"����������ʧ�ܣ�",WND_CAPTION,MB_OK);

		return 1;
	}

	srand(GetTickCount());//��ʼ�����������

	g_hMainWndDC=GetDC(g_hMainWnd);
	g_GamePage.SetPage(WND_W,WND_H);//������Ϸ���ƻ���ҳ��
	g_TempPage.SetPage(WND_W,WND_H);//������ʱҳ��
	//������ͼ����ҳ��
	g_MapPage[0].SetPage(TITLE_W*TITLE_X_COUNT,TITLE_H*TITLE_Y_COUNT);
	g_MapPage[1].SetPage(TITLE_W*TITLE_X_COUNT,TITLE_H*TITLE_Y_COUNT);

	SetWindowPos(g_hMainWnd,HWND_TOPMOST,0,0,WND_W,WND_H,SWP_SHOWWINDOW);

	//��ȡ��Դ·��
	GetModuleFileName(NULL,strFileName,MAX_PATH);
	GetFileDir(strFileName,g_strPicDir);//ͼƬ�ļ���·��
	GetFileDir(strFileName,g_strSoundDir);//�����ļ���·��
	GetFileDir(strFileName,g_strMapDir);//�����ļ���·��
	strcat(g_strPicDir,DIR_PIC);
	strcat(g_strSoundDir,DIR_SOUND);
	strcat(g_strMapDir,DIR_MAP);

	//�����ͼ��Դ
	strFile="";
	strFile+=g_strPicDir;
	strFile+="Map1.bmp";
	g_MapBmp[0]=LoadBmp(strFile.c_str());
	
	strFile="";
	strFile+=g_strPicDir;
	strFile+="Map2.bmp";
	g_MapBmp[1]=LoadBmp(strFile.c_str());

	//�����ɫͼƬ
	strFile="";
	strFile+=g_strPicDir;
	strFile+="Role.bmp";
	g_RoleBmp=LoadBmp(strFile.c_str());

	CreateThread(NULL,0,GameControl,NULL,NULL,&g_GameControlThreadID);//������Ϸ�����߳�

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
//�������ƣ�MainWndProc
//˵������������Ϣѭ��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-12-14
//�޸ļ�¼��
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
