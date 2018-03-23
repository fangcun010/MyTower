//头文件名称：我的魔塔
//作者：方寸
//说明：无
//日期：2012-12-14

#ifndef _S_MY_MAGIC_TOWER_H_
#define _S_MY_MAGIC_TOWER_H_

#include <windows.h>
#include "GameSupport.h"
#include "SPaintPage.h"

//=========================================================
//常量
//=========================================================
//主窗口类名称
#define			WND_CLASSNAME				"My Magic Tower"
//主窗口标题
#define			WND_CAPTION					"我的魔塔"
#define			WND_W						800
#define			WND_H						600
//计时器
#define			TIMER_FLASH					0
#define			TIMER_FLASH_DELAY			500
//游戏资源
#define			DIR_PIC						"\\Res\\Picture\\"
#define			DIR_SOUND					"\\Res\\Sound\\"
#define			DIR_MAP						"\\Res\\Map\\"
//游戏地图
#define			TITLE_W						32
#define			TITLE_H						32
#define			TITLE_X_COUNT				18
#define			TITLE_Y_COUNT				18
#define			MAP_COUNT					23
//颜色
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
//游戏结果
#define			RESULT_FAIL					0
#define			RESULT_END					1
#define			RESULT_EXIT					2
//游戏保存
#define			SAVE_FILENAME				"Game.dat"

//角色
//面向
#define			FACE_UP						0
#define			FACE_DOWN					1
#define			FACE_LEFT					2
#define			FACE_RIGHT					3
//角色最大魔法数
#define			ROLE_MAGIC_MAXCOUNT			255
//角色最大物品数
#define			ROLE_GOODS_MAXCOUNT			255
//角色最大事件数
#define			ROLE_EVENT_MAXCOUNT			255
//敌人
#define			ENEMY_COUNT					255
#define			ENEMY_MAGIC_MAXCOUNT		255
//角色战斗选项
#define			FIGHT_ATTACK				0
#define			FIGHT_MAGIC					1
#define			FIGHT_ESCAPE				2
#define			FIGHT_GOODS					3
//魔法ID
//恢复术
#define			MAGIC_RECOVER				0
//风暴
#define			MAGIC_STORM					1
//地域炼火
#define			MAGIC_ZONEFIRE				2
//烈火
#define			MAGIC_HEATFIRE				3
//鬼魂召唤
#define			MAGIC_SOULOUT				4
//电闪雷鸣
#define			MAGIC_THUNDER				5
//天降巨石
#define			MAGIC_STONEFALL				6
//极光
#define			MAGIC_AURORA				7
//破天剑
#define			MAGIC_SWORD					8
//降魔符法
#define			MAGIC_SYMBOL				9

//Debug
#define			DEBUG						while(1)

//=========================================================
//全局变量
//=========================================================
extern HINSTANCE	g_hInstance;				//程序句柄
extern HWND			g_hMainWnd;					//主窗口句柄
extern DWORD		g_GameControlThreadID;		//游戏控制线程ID
extern HDC			g_hMainWndDC;				//主窗口设备上下文
extern SPaintPage	g_GamePage;					//游戏画面
extern SPaintPage	g_TempPage;					//临时页面
extern SPaintPage	g_MapPage[2];				//游戏地图
extern bool			g_bKeyDown[256];			//按键是否按下
extern char			g_strPicDir[MAX_PATH];		//图片文件夹路径
extern char			g_strSoundDir[MAX_PATH];	//音乐文件夹路径
extern char			g_strMapDir[MAX_PATH];		//地图文件夹路径
extern SBMP			g_MapBmp[2];				//地图位图资源
extern SBMP			g_RoleBmp;					//角色位图资源
extern int			g_Map[MAP_COUNT][TITLE_X_COUNT][TITLE_Y_COUNT];//地图数据
extern bool			g_bShowMap2;				//用地图2显示
extern int			g_x,g_y;					//全局坐标传递
extern int			g_Result;					//全局游戏结果传递
extern POINT		g_UpStair[MAP_COUNT+1];		//上楼梯坐标
extern POINT		g_DownStair[MAP_COUNT+1];	//下楼梯坐标
struct Magic//魔法
{
	char strName[256];
	char strDescript[256];//描述
	int e;//动画结束帧
	int ID;//魔法ID
	int HP;//正值对敌伤害，负值恢复自身
	int MP;
};
extern struct MagicList
{
	Magic Info[ROLE_MAGIC_MAXCOUNT];
	int Count;
} g_MagicList;

struct Goods//物品
{
	char strName[256];
	char strDescript[256];//描述
	int ID;//物品ID
	int HP;
	int MP;
	int Attack;
	int Defend;
	bool bFight;//战斗时是否可用
};

extern struct GoodsList
{
	Goods Info[ROLE_GOODS_MAXCOUNT];
	int Count;
} g_GoodsList;

extern struct RoleInfo//角色信息
{
	int Level;//等级
	int x,y;//角色位置坐标
	int Face;//角色面向
	int Step;//角色脚步
	int HP;//生命
	int HPMax;//生命上限
	int MP;//魔法
	int MPMax;//魔法上限
	int Attack;//攻击
	int Defend;//防御
	int Money;//金钱
	int Experience;//经验
	int PowderKeyCount;//粉红钥匙数
	int BlueKeyCount;//蓝钥匙数
	int RedKeyCount;//红钥匙数
	int Floor;//当前楼层数
	struct
	{
		int ID[ROLE_MAGIC_MAXCOUNT];//魔法编号
		int Count;//魔法数量
	} Magic;
	struct
	{
		int ID[ROLE_GOODS_MAXCOUNT];//物品编号
		int GoodsCount[ROLE_GOODS_MAXCOUNT];//物品数量
		int Count;//物品顺序表长度
	} Goods;
	bool Event[ROLE_EVENT_MAXCOUNT];
} g_RoleInfo;
extern struct EnemyInfo//敌人信息
{
	char *strName[256];//敌人名称
	int PicID;//敌人图片ID：实际是地图块的ID
	int HP;//生命
	int MP;//魔法
	int Attack;//攻击
	int Defend;//防御
	int Money;//金币
	int Experience;//经验
	struct
	{
		int ID[ENEMY_MAGIC_MAXCOUNT];//魔法编号
		int Count;//魔法数量
	} Magic;
} g_EnemyInfo[ENEMY_COUNT];

//=========================================================
//子程式声明
//=========================================================
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd);//主函数
LRESULT CALLBACK MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//主窗口消息循环
DWORD WINAPI GameControl(LPVOID lpParam);//游戏控制线程

#endif
