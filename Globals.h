//ͷ�ļ����ƣ��ҵ�ħ��
//���ߣ�����
//˵������
//���ڣ�2012-12-14

#ifndef _S_MY_MAGIC_TOWER_H_
#define _S_MY_MAGIC_TOWER_H_

#include <windows.h>
#include "GameSupport.h"
#include "SPaintPage.h"

//=========================================================
//����
//=========================================================
//������������
#define			WND_CLASSNAME				"My Magic Tower"
//�����ڱ���
#define			WND_CAPTION					"�ҵ�ħ��"
#define			WND_W						800
#define			WND_H						600
//��ʱ��
#define			TIMER_FLASH					0
#define			TIMER_FLASH_DELAY			500
//��Ϸ��Դ
#define			DIR_PIC						"\\Res\\Picture\\"
#define			DIR_SOUND					"\\Res\\Sound\\"
#define			DIR_MAP						"\\Res\\Map\\"
//��Ϸ��ͼ
#define			TITLE_W						32
#define			TITLE_H						32
#define			TITLE_X_COUNT				18
#define			TITLE_Y_COUNT				18
#define			MAP_COUNT					23
//��ɫ
#define			COLOR_SAY_FRONT				RGB(0,10,0)
#define			COLOR_SAY_BACK				RGB(255,255,255)
#define			COLOR_PROPERTY_FRONT		RGB(100,40,10)
#define			COLOR_PROPERTY_BACK			RGB(255,255,255)
#define			COLOR_MSG_FRONT				RGB(255,255,255)
#define			COLOR_MSG_BACK				RGB(23,47,35)
#define			COLOR_HPMP_FRONT			RGB(0,0,0)
#define			COLOR_HPMP_BACK				RGB(255,255,255)
#define			COLOR_ESC_FRONT				RGB(10,10,10)
#define			COLOR_ESC_BACK				RGB(255,0,0)
#define			COLOR_CHO_FRONT				RGB(0,0,0)
#define			COLOR_CHO_BACK				RGB(120,10,10)
//��Ϸ���
#define			RESULT_FAIL					0
#define			RESULT_END					1
#define			RESULT_EXIT					2
//��Ϸ����
#define			SAVE_FILENAME				"Game.dat"

//��ɫ
//����
#define			FACE_UP						0
#define			FACE_DOWN					1
#define			FACE_LEFT					2
#define			FACE_RIGHT					3
//��ɫ���ħ����
#define			ROLE_MAGIC_MAXCOUNT			255
//��ɫ�����Ʒ��
#define			ROLE_GOODS_MAXCOUNT			255
//��ɫ����¼���
#define			ROLE_EVENT_MAXCOUNT			255
//����
#define			ENEMY_COUNT					255
#define			ENEMY_MAGIC_MAXCOUNT		255
//��ɫս��ѡ��
#define			FIGHT_ATTACK				0
#define			FIGHT_MAGIC					1
#define			FIGHT_ESCAPE				2
#define			FIGHT_GOODS					3
//ħ��ID
//�ָ���
#define			MAGIC_RECOVER				0
//�籩
#define			MAGIC_STORM					1
//��������
#define			MAGIC_ZONEFIRE				2
//�һ�
#define			MAGIC_HEATFIRE				3
//����ٻ�
#define			MAGIC_SOULOUT				4
//��������
#define			MAGIC_THUNDER				5
//�콵��ʯ
#define			MAGIC_STONEFALL				6
//����
#define			MAGIC_AURORA				7
//���콣
#define			MAGIC_SWORD					8
//��ħ����
#define			MAGIC_SYMBOL				9

//Debug
#define			DEBUG						while(1)

//=========================================================
//ȫ�ֱ���
//=========================================================
extern HINSTANCE	g_hInstance;				//������
extern HWND			g_hMainWnd;					//�����ھ��
extern DWORD		g_GameControlThreadID;		//��Ϸ�����߳�ID
extern HDC			g_hMainWndDC;				//�������豸������
extern SPaintPage	g_GamePage;					//��Ϸ����
extern SPaintPage	g_TempPage;					//��ʱҳ��
extern SPaintPage	g_MapPage[2];				//��Ϸ��ͼ
extern bool			g_bKeyDown[256];			//�����Ƿ���
extern char			g_strPicDir[MAX_PATH];		//ͼƬ�ļ���·��
extern char			g_strSoundDir[MAX_PATH];	//�����ļ���·��
extern char			g_strMapDir[MAX_PATH];		//��ͼ�ļ���·��
extern SBMP			g_MapBmp[2];				//��ͼλͼ��Դ
extern SBMP			g_RoleBmp;					//��ɫλͼ��Դ
extern int			g_Map[MAP_COUNT][TITLE_X_COUNT][TITLE_Y_COUNT];//��ͼ����
extern bool			g_bShowMap2;				//�õ�ͼ2��ʾ
extern int			g_x,g_y;					//ȫ�����괫��
extern int			g_Result;					//ȫ����Ϸ�������
extern POINT		g_UpStair[MAP_COUNT+1];		//��¥������
extern POINT		g_DownStair[MAP_COUNT+1];	//��¥������
struct Magic//ħ��
{
	char strName[256];
	char strDescript[256];//����
	int e;//��������֡
	int ID;//ħ��ID
	int HP;//��ֵ�Ե��˺�����ֵ�ָ�����
	int MP;
};
extern struct MagicList
{
	Magic Info[ROLE_MAGIC_MAXCOUNT];
	int Count;
} g_MagicList;

struct Goods//��Ʒ
{
	char strName[256];
	char strDescript[256];//����
	int ID;//��ƷID
	int HP;
	int MP;
	int Attack;
	int Defend;
	bool bFight;//ս��ʱ�Ƿ����
};

extern struct GoodsList
{
	Goods Info[ROLE_GOODS_MAXCOUNT];
	int Count;
} g_GoodsList;

extern struct RoleInfo//��ɫ��Ϣ
{
	int Level;//�ȼ�
	int x,y;//��ɫλ������
	int Face;//��ɫ����
	int Step;//��ɫ�Ų�
	int HP;//����
	int HPMax;//��������
	int MP;//ħ��
	int MPMax;//ħ������
	int Attack;//����
	int Defend;//����
	int Money;//��Ǯ
	int Experience;//����
	int PowderKeyCount;//�ۺ�Կ����
	int BlueKeyCount;//��Կ����
	int RedKeyCount;//��Կ����
	int Floor;//��ǰ¥����
	struct
	{
		int ID[ROLE_MAGIC_MAXCOUNT];//ħ�����
		int Count;//ħ������
	} Magic;
	struct
	{
		int ID[ROLE_GOODS_MAXCOUNT];//��Ʒ���
		int GoodsCount[ROLE_GOODS_MAXCOUNT];//��Ʒ����
		int Count;//��Ʒ˳�����
	} Goods;
	bool Event[ROLE_EVENT_MAXCOUNT];
} g_RoleInfo;
extern struct EnemyInfo//������Ϣ
{
	char *strName[256];//��������
	int PicID;//����ͼƬID��ʵ���ǵ�ͼ���ID
	int HP;//����
	int MP;//ħ��
	int Attack;//����
	int Defend;//����
	int Money;//���
	int Experience;//����
	struct
	{
		int ID[ENEMY_MAGIC_MAXCOUNT];//ħ�����
		int Count;//ħ������
	} Magic;
} g_EnemyInfo[ENEMY_COUNT];

//=========================================================
//�ӳ�ʽ����
//=========================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd);//������
LRESULT CALLBACK MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//��������Ϣѭ��
DWORD WINAPI GameControl(LPVOID lpParam);//��Ϸ�����߳�

#endif
