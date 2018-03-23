//文件名称：Game
//作者：方寸
//说明：游戏实现
//日期：2012-12-14
#include "Globals.h"
#include <iostream>

using namespace std;

//=========================================================
//常量
//=========================================================
#define FLIPPAGE				g_GamePage.CopyToDC(g_hMainWndDC)
//按键延时
#define KEY_DELAY				200

//=========================================================
//子程式声明
//=========================================================
void StartLogo();//开始Logo动画
int GameMenu(int nCho);//游戏主菜单
int GetKey();//获取游戏按键
void PlayStart();//开始动画
void PlayLeave();//飘叶动画
void Foreword();//游戏序言
void TrueHeart();//真情告白
void ReadMap(int ID);//读取1层地图
void ReadMap();//读取地图数据
void ShowScene();//显示整个场景
void DataInit();//数据初始化
int MyRoleMagic();//魔法选择
int MyRoleGoods();//物品选择
void Gut();//游戏剧情
int EscMenu();
void SaveGame();//存储游戏
bool ReadGame();//读取游戏
void GameOver();//游戏结束
void GameEnd();//游戏结束
void Shop(int ID);//商店
void GotoFloor(int ID);//楼层跳转
void ClearTitle(int x,int y);//用ID为0的地图块清除指定位置的地图块，带动画
int DriveMap();//操纵角色移动
void DrawFloor(int nFloor);//绘制一层地图
bool RoleHaveGoods(int ID);//角色是否有给定ID物品
void DelRoleGoods(int ID);//删除角色物品
void AddRoleGoods(int ID);//增加角色物品
void AddRoleMagic(int ID);//增加角色魔法
void SetEnemyInfo(int ID,int HP,int MP,int Attack,int Defend,int Money,int Exp);//设置敌人信息
void ArtText(const char *strText,int x,int y,int msDelay,bool bVertical);//艺术字
void SetMapZero(int x,int y);//设置地图数据为0
void StartFlash();//开始地图块闪烁
void CloseFlash();//关闭地图块闪烁
void AddGoods(const char *strName,const char *strDescript,int Attack,int Defend,int HP,int MP,bool bFight,int ID);//加入物品
void AddMagic(const char *strName,const char *strDescript,int HP,int MP,int e,int ID);//加入魔法
void ArtBack(SPaintPage &Page,COLORREF cr,bool bUpright);//艺术清屏
VOID CALLBACK FlashTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);//闪烁计时器
void Say(const char *strText,const char *strHeadFile,bool bUp);//说话函数
void ShowMsg(const char *strMsg);//显示消息
void PlayMagic(const char *strName,int x,int y,int e,int msDelay);//播放魔法动画
int Fight(int EnemyID,bool bGut,bool bMustLoss);//角色战斗
void DrawAllMap(SPaintPage &Pg,bool bMap2,int nFloor);//在指定页面绘制一层地图
void DrawMap(SPaintPage &Pg,bool bMap2,int Data,int x,int y);//在指定页面指定位置绘制地图
void HollowWords(SPaintPage &Pg,const char *strText,const char *strFont,int nSize,bool bItalic,int x,int y,COLORREF crFront,COLORREF crBack);//空心字
SPaintPage *CreatePageByText(const char *strText,const char *strFont,int nFontSize,int nSpace);//通过文本创建缓冲页面

//=========================================================
//函数名称：GameControl
//说明：游戏控制线程
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-14
//修改记录：
//=========================================================
DWORD WINAPI GameControl(LPVOID lpParam)
{
	int nChoose=0;
	bool bRet;

	StartLogo();
	PlayLeave();
	PlayStart();
	PlayLeave();
LabelGameMenu:

	nChoose=GameMenu(nChoose);

	if(nChoose==0)//新的游戏
	{
		PlayLeave();
		Foreword();
		PlayLeave();
Start:
		DataInit();//游戏数据初始化
		ReadMap();
		StartFlash();
		DrawAllMap(g_MapPage[0],false,0);
   		DrawAllMap(g_MapPage[1],true,0);

		Gut();
		CloseFlash();
		goto LabelGameMenu;
	}
	else if(nChoose==1)//旧的回忆
	{
		DataInit();//游戏数据初始化
		bRet=ReadGame();
		if(bRet)//读取游戏成功
		{
			StartFlash();
			Gut();
			CloseFlash();
			goto LabelGameMenu;
		}
		else//读取游戏失败
		{
			goto LabelGameMenu;
		}
	}
	if(nChoose==2)//真情告白
	{
		PlayLeave();
		TrueHeart();
		PlayLeave();
		goto LabelGameMenu;
	}
	else if(nChoose==3)//退出游戏
		SendMessage(g_hMainWnd,WM_CLOSE,NULL,NULL);

	return 0;
}

//=========================================================
//函数名称：Gut
//说明：剧情
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void Gut()
{
	string strFile;
	char text[256];
	UINT MusicID;
	int ID;
	int nRet;

	while(1)
	{
		ID=DriveMap();

		if(ID==46 && g_RoleInfo.Floor==0)//在第0层遇到阿兰
		{
			Say("懦夫：阿兰！！！我终于找到你了！！！","懦夫",false);
			Say("阿兰：你这个懦夫来干吗！！！我在这里很好！！！","阿兰",true);
			Say("懦夫：阿兰，我是来救你的，向你证明我不是懦夫！！！","懦夫",false);
			Say("阿兰：阿牛哥对我可好了，不用你来救！！！","阿兰",true);
			Say("懦夫：你是说那个魔王！！！","懦夫",false);
			Say("阿牛：阿兰，你去哪了？","阿牛",true);
			Say("阿兰：阿牛哥叫我呢！！！懦夫，你快滚吧！！！","阿兰",true);
			Say("懦夫：阿兰给我一个解释的机会吧！！！","懦夫",false);
			SetMapZero(8,15);
			strFile=g_strSoundDir;
			strFile+="曾经.mp3";
			MusicID=LoadMusic(strFile.c_str());
			PlayMusic(MusicID,false);
			ArtBack(g_GamePage,0,true);
			g_TempPage.CopyPage(g_GamePage);
			ArtText("曾经有一份真诚的爱情放在我面前",500,100,300,true);
			ArtText("我没有珍惜",450,100,300,true);
			ArtText("等我失去的时候",400,100,300,true);
			ArtText("我才后悔莫及",350,100,300,true);
			ArtText("人世间最痛苦的事莫过于此",300,100,300,true);
			g_GamePage.CopyPage(g_TempPage);

			Sleep(3000);
			CloseMusic(MusicID);

			Say("懦夫：让我给你解释！！！","懦夫",false);
		}
		else if(ID==46 && g_RoleInfo.Floor==21)//在第21层找到阿兰
		{
			if(!g_RoleInfo.Event[10])
			{
				Say("阿兰：你真的来了~!","阿兰",true);
				Say("勇者：我来跟你解释！！！","勇者",false);
				
				strFile=g_strSoundDir;
				strFile+="一万年.mp3";
				MusicID=LoadMusic(strFile.c_str());
				PlayMusic(MusicID,false);
				ArtBack(g_GamePage,0,true);
				g_TempPage.CopyPage(g_GamePage);
				ArtText("曾经有一份真诚的爱情放在我面前",500,100,300,true);
				ArtText("我没有珍惜",450,100,300,true);
				ArtText("等我失去的时候",400,100,300,true);
				ArtText("我才后悔莫及",350,100,300,true);
				ArtText("尘世间世间最痛苦的事莫过于此",300,100,300,true);
				ArtText("如果可以再来一次",250,100,300,true);
				ArtText("我会对她说",200,100,300,true);
				ArtText("我爱她",150,100,300,true);
				ArtText("一万年",100,100,300,true);
				g_GamePage.CopyPage(g_TempPage);
				
				Sleep(10000);
				CloseMusic(MusicID);
				
				Say("阿兰：哥哥！~ 我是害怕你来送死啊！！","阿兰2",true);
				Say("勇者：如果一个人连自己最爱的人都保护不了，活着还有什么意思！！！","勇者",false);
				Say("阿兰：哥哥！~ 你变了！！！","阿兰2",true);
				Say("勇者：是的，这些天我明白了很多！现在就让我替天行道杀了魔王！","勇者",false);

				g_RoleInfo.Event[10]=true;
			}
			else
			{
				Say("阿兰：去吧！我支持你！~ ","阿兰2",true);
			}
		}
		else if(ID==50 && RoleHaveGoods(28-14) && RoleHaveGoods(29-14) && RoleHaveGoods(30-14))
		{
			Say("懦夫：是不是这些？","懦夫",false);
			Say("仙子：就是这些，谢谢你！","仙子",true);
			ShowMsg("能力提升");
			g_RoleInfo.Attack+=50;
			g_RoleInfo.Defend+=50;
			DelRoleGoods(28-14);
			DelRoleGoods(29-14);
			DelRoleGoods(30-14);
			g_RoleInfo.Event[20]=true;
		}
		else if(ID==50 && !g_RoleInfo.Event[20])
		{
			Say("仙子：我的法器：祝福丢失了，如果你能帮我找回来，我就能给你一些帮助！！","仙子",true);
			Say("懦夫：我去找找！","懦夫",false);
		}
		else if(ID==50 && g_RoleInfo.Event[20])
		{
			Say("仙子：去完成你的使命吧！","仙子",true);
		}
		else if(ID==48 && g_RoleInfo.Floor==1)//1楼的问道仙人
		{
			Say("问道仙人：怪物是很可怕的，不要随便乱招惹。如果生命过低，不妨按下 ESC键看看自己的物品里有没有红血瓶可以用。","问道仙人",true);
		}
		else if(ID==48 && g_RoleInfo.Floor==2 && g_RoleInfo.Event[0]==false)//2楼的问道仙人
		{
			Say("问道仙人：此楼妖怪可不那么温柔了，嘿嘿，想打败他们吗？哼哼，给我35个金币，我就教你一招。","问道仙人",true);
			if(g_RoleInfo.Money>=35)
			{
				AddRoleMagic(MAGIC_STORM);
				ShowMsg("学会：风暴");
				g_RoleInfo.Money-=35;
				g_RoleInfo.Event[0]=true;
			}
			else
			{
				Say("懦夫：我的金币还不够！可不可以先欠着。。。","懦夫",false);
				Say("问道仙人：すそたはそそちゅぬのめの","问道仙人",true);
			}
		}
		else if(ID==48 && g_RoleInfo.Floor==2 && g_RoleInfo.Event[0]==true)//2楼的问道仙人
		{
			Say("问道仙人：哈哈，我这招还行吧！嘿嘿！","问道仙人",true);
		}
		else if(ID==48 && g_RoleInfo.Floor==3)//3楼的问道仙人
		{
			Say("问道仙人：没错又是我，给我100经验，我让你升1级。","问道仙人",true);
			if(g_RoleInfo.Experience<100)
			{
				ShowMsg("经验不足");
			}
			else
			{
				g_RoleInfo.Experience-=100;
				g_RoleInfo.HPMax+=10;
				g_RoleInfo.HP=g_RoleInfo.HPMax;
				g_RoleInfo.MPMax+=10;
				g_RoleInfo.MP=g_RoleInfo.MPMax;
				g_RoleInfo.Attack+=5;
				g_RoleInfo.Defend+=5;
				g_RoleInfo.Level++;
				ShowMsg("角色升级");
				if(g_RoleInfo.Level==5)
				{
					ShowMsg("学会 天降巨石");
					AddRoleMagic(MAGIC_STONEFALL);
				}
				else if(g_RoleInfo.Level==10)
				{
					ShowMsg("学会 极光");
					AddRoleMagic(MAGIC_AURORA);
				}
				else if(g_RoleInfo.Level==15)
				{
					ShowMsg("学会 鬼魂召唤");
					AddRoleMagic(MAGIC_SOULOUT);
				}
				else if(g_RoleInfo.Level==20)
				{
					ShowMsg("学会 电闪雷鸣");
					AddRoleMagic(MAGIC_THUNDER);
				}
			}
		}
		else if(ID==43 && g_RoleInfo.Floor==2)//2楼的商店
		{
			Say("商人：给我25个金币，我可以增强你的能力！","商人",true);
			if(g_RoleInfo.Money<25)
			{
				ShowMsg("金币不足");
			}
			else
			{
				Shop(0);
			}
		}
		else if(ID==49 && g_RoleInfo.Floor==5)//5楼的落魄法师
		{
			if(!g_RoleInfo.Event[1])
			{
				Say("落魄法师：我被怪物怪物困在这里，魔法也用完了，不能屠妖是我的遗憾，我就教你几招让你代我屠妖吧！","落魄法师",true);
				ShowMsg("学会：恢复术");
				AddRoleMagic(MAGIC_RECOVER);
				ShowMsg("学会：地域炼火");
				AddRoleMagic(MAGIC_ZONEFIRE);
				ShowMsg("学会：烈火");
				AddRoleMagic(MAGIC_HEATFIRE);
				Say("懦夫：我会完成你的遗愿的！！！","懦夫",false);
				g_RoleInfo.Event[1]=true;
				ClearTitle(1,16);
			}
		}
		else if(ID==47 && g_RoleInfo.Floor==8)//8楼的人质
		{
			if(!g_RoleInfo.Event[2])
			{
				Say("人质：谢谢救了我，勇士！！！","人质", true);
				Say("懦夫：勇士？！你是说我吗？，或许我只是一个懦夫吧！！~ ","懦夫",false);
				Say("人质：没有人是与生俱来就是勇敢的，但当他所要守护的受到伤害时，他会慢慢变得勇敢！！！","人质", true);
				Say("懦夫：谢谢你！！我好像明白了什么！~ ","懦夫",false);
				SetMapZero(14,1);
				g_RoleInfo.Event[2]=true;
			}
		}
		else if(ID==43 && g_RoleInfo.Floor==9)//9楼的商店
		{
			Say("商人：给我50个金币，我可以增强你的能力！","商人",true);
			if(g_RoleInfo.Money<50)
			{
				ShowMsg("金币不足");
			}
			else
			{
				Shop(1);
			}
		}
		else if(ID==5 && !(g_RoleInfo.Event[11] && g_RoleInfo.Event[12]) && g_RoleInfo.Floor==21)
		{
			Say("勇者：先打败魔王！！！","勇者",false);
		}
		else if(ID==5 && g_RoleInfo.Event[11] && g_RoleInfo.Event[12] && g_RoleInfo.Floor==21)
		{
			g_RoleInfo.Floor=22;
			DrawAllMap(g_MapPage[0],false,22);
			DrawAllMap(g_MapPage[1],true,22);
			g_RoleInfo.x=8;
			g_RoleInfo.y=11;
		}
		else if(ID==5)//上楼
		{
			g_RoleInfo.x=g_UpStair[g_RoleInfo.Floor].x;
			g_RoleInfo.y=g_UpStair[g_RoleInfo.Floor].y;
			
			g_RoleInfo.Floor++;
			DrawFloor(g_RoleInfo.Floor);
		}
		else if(ID==4 && g_RoleInfo.Floor==0)
		{
			PlayLeave();
			TrueHeart();
			PlayLeave();
		}
		else if(ID==4)//下楼
		{
			g_RoleInfo.x=g_DownStair[g_RoleInfo.Floor].x;
			g_RoleInfo.y=g_DownStair[g_RoleInfo.Floor].y;
			
			g_RoleInfo.Floor--;
			DrawFloor(g_RoleInfo.Floor);
		}
		else if(ID==80)
		{
			Say("阿牛：不怕死的！！来和我打！！！","阿牛",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//战斗失败
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//战斗胜利
			{
				SetMapZero(g_x,g_y);
				g_RoleInfo.Event[11]=true;
			}
			else if(nRet==2)//逃跑成功
				;
		}
		else if(ID==81)
		{
			Say("吸血鬼：我是来参加牛魔王的婚礼的，顺便吸干你的血吧！","吸血鬼",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//战斗失败
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//战斗胜利
			{
				SetMapZero(g_x,g_y);
				g_RoleInfo.Event[12]=true;
			}
			else if(nRet==2)//逃跑成功
				;
		}
		else if(ID==98)//牛魔王真身
		{
			Say("牛魔王：我显出真身，你还是对手吗？！","阿牛",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//战斗失败
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//战斗胜利
			{
				ClearTitle(g_x,g_y);
				g_RoleInfo.Event[13]=true;
			}
			else if(nRet==2)//逃跑成功
				;
			if(g_RoleInfo.Event[13] && g_RoleInfo.Event[14])
			{
				PlayLeave();
				GameEnd();
				PlayLeave();
				g_Result=RESULT_END;
				return;
			}
		}
		else if(ID==89)//吸血鬼真身
		{
			Say("吸血鬼：我显出真身，你还是对手吗？！","阿牛",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//战斗失败
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_END;
				return;
			}
			else if(nRet==1)//战斗胜利
			{
				ClearTitle(g_x,g_y);
				g_RoleInfo.Event[14]=true;
			}
			else if(nRet==2)//逃跑成功
				;
			if(g_RoleInfo.Event[13] && g_RoleInfo.Event[14])
			{
				PlayLeave();
				GameEnd();
				PlayLeave();
				return;
			}
		}
		else if(ID>=51 && ID<=79)//遇到怪物
		{
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//战斗失败
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//战斗胜利
				SetMapZero(g_x,g_y);
			else if(nRet==2)//逃跑成功
				;
		}
		else if(ID>=14 && ID<=41)
		{
			sprintf(text,"获得 %s",g_GoodsList.Info[g_Map[g_RoleInfo.Face][g_x][g_y]].strName);
			ShowMsg(text);
		}
		else if(ID==-1)
		{
			g_Result=RESULT_EXIT;

			return;
		}
	}
}

//=========================================================
//函数名称：RoleHaveGoods
//说明：
//输入：
//输出：
//返回：
//备注：
//日期：2012-1-12
//修改记录：
//=========================================================
bool RoleHaveGoods(int ID)
{
	int i;

	for(i=0;i<g_RoleInfo.Goods.Count;i++)
	{
		if(g_RoleInfo.Goods.ID[i]==ID)
			return true;
	}

	return false;
}

//=========================================================
//函数名称：GameEnd
//说明：
//输入：
//输出：
//返回：
//备注：
//日期：2012-1-12
//修改记录：
//=========================================================
void GameEnd()
{
	char str[1024];
	SPaintPage *pPg=NULL;
	SBMP Bmp;
	string strFile;
	int y;

	strFile=g_strPicDir;
	strFile+="渐入渐出蒙版.bmp";
	Bmp=LoadBmp(strFile.c_str());

	LoadString(g_hInstance,5,str,1024);
	pPg=CreatePageByText(str,"宋体",16,5);
	
	y=-50;
	while(-y<=pPg->GetHeight()-WND_H-50)
	{
		g_GamePage.ShowPic(Bmp,0,0,0,0,Bmp.w,Bmp.h,0,false);
		TransparentBlt(g_GamePage.GetDC(),0,0,WND_W,WND_H,pPg->GetDC(),0,-y,WND_W,WND_H,RGB(255,0,255));
		FLIPPAGE;
		y-=2;
		Sleep(100);
	}

	DeleteBmp(Bmp);
	delete pPg;
}

//=========================================================
//函数名称：GotoFloor
//说明：跳转楼层
//输入：
//输出：
//返回：
//备注：
//日期：2012-1-12
//修改记录：
//=========================================================
void GotoFloor(int ID)
{
	int s,e;
	int i,si;
	
	if(ID==0)
	{
		s=1;
		e=7;
	}
	else if(ID==1)
	{
		s=8;
		e=15;
	}
	else if(ID==2)
	{
		s=16;
		e=20;
	}
	SBMP BackBmp;
	SBMP ArrowBmp;
	string strFile;
	char text[256];
	int x,y;
	int Cho=0;
	int Key=0;
	
	strFile=g_strPicDir;
	strFile+="EscBack.bmp";
	
	BackBmp=LoadBmp(strFile.c_str());
	
	strFile=g_strPicDir;
	strFile+="Arrow.bmp";
	
	ArrowBmp=LoadBmp(strFile.c_str());
	
	x=(WND_W-BackBmp.w)/2;
	y=(WND_H-BackBmp.h)/2;
	
	g_TempPage.CopyPage(g_GamePage);
	
	//绘制背景
	g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
	g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
	
	si=0;

	i=s;

	for(;i<=e;i++,si++)
	{
		sprintf(text,"第%d层",i);
		HollowWords(g_GamePage,text,"宋体",16,false,x+60,y+3+si*20,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}

	
	//绘制光标
	g_GamePage.ShowPic(ArrowBmp,x+20,y+3+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);
	
	FLIPPAGE;
	
	
	while(1)
	{
		if(Key)
		{
			//绘制背景
			g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
			g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
	
	si=0;

	i=s;

	for(;i<=e;i++,si++)
	{
		sprintf(text,"第%d层",i);
		HollowWords(g_GamePage,text,"宋体",16,false,x+60,y+3+si*20,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}
			//绘制光标
			g_GamePage.ShowPic(ArrowBmp,x+20,y+3+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);
			FLIPPAGE;
		}
		Key=GetKey();
		
		if(Key==VK_UP)
		{
			Cho--;
		}
		else if(Key==VK_DOWN)
		{
			Cho++;
		}
		else if(Key==VK_RETURN)
		{
			goto EndGotoFloor;	
		}
		else if(Key==VK_ESCAPE)
		{
			Cho=-1;
			goto EndGotoFloor;
		}
		if(Cho<0) Cho=e-s;
		if(Cho>e-s) Cho=0;
	}
	
EndGotoFloor:
	g_GamePage.CopyPage(g_TempPage);
	
	DeleteBmp(BackBmp);
	DeleteBmp(ArrowBmp);
	
	if(Cho!=-1)
	{
		g_RoleInfo.x=g_UpStair[Cho+s-1].x;
		g_RoleInfo.y=g_UpStair[Cho+s-1].y;
		g_RoleInfo.Floor=Cho+s;
		DrawFloor(g_RoleInfo.Floor);	
	}
}

//=========================================================
//函数名称：SaveGame
//说明：保存游戏
//输入：
//输出：
//返回：
//备注：
//日期：2013-1-11
//修改记录：
//=========================================================
void SaveGame()
{
	FILE *fp;
	string strFile;
	char strFileName[MAX_PATH];
	char strDirName[MAX_PATH];

	GetModuleFileName(NULL,strFileName,MAX_PATH);
	
	GetFileDir(strFileName,strDirName);

	strFile="";
	strFile+=strDirName;
	strFile+="\\";
	strFile+=SAVE_FILENAME;

	fp=fopen(strFile.c_str(),"wb");

	if(!fp)
	{
		MessageBox(NULL,"创建存档文件失败！",WND_CAPTION,MB_OK|MB_ICONSTOP);
		return;
	}

	fwrite(&g_RoleInfo,sizeof(RoleInfo),1,fp);
	fwrite(&g_Map,sizeof(g_Map),1,fp);

	ShowMsg("保存成功！");

	fclose(fp);
}

//=========================================================
//函数名称：ReadGame
//说明：读取游戏
//输入：
//输出：
//返回：
//备注：
//日期：2013-1-11
//修改记录：
//=========================================================
bool ReadGame()
{
	FILE *fp;
	string strFile;
	char strFileName[MAX_PATH];
	char strDirName[MAX_PATH];

	GetModuleFileName(NULL,strFileName,MAX_PATH);
	
	GetFileDir(strFileName,strDirName);

	strFile="";
	strFile+=strDirName;
	strFile+="\\";
	strFile+=SAVE_FILENAME;

	fp=fopen(strFile.c_str(),"rb");

	if(!fp)
	{
		ShowMsg("尚未存档！");
		return false;
	}

	fread(&g_RoleInfo,sizeof(RoleInfo),1,fp);
	fread(&g_Map,sizeof(g_Map),1,fp);

	ShowMsg("读取成功！");

	//绘制楼层地图
	DrawAllMap(g_MapPage[0],false,g_RoleInfo.Floor);
	DrawAllMap(g_MapPage[1],true,g_RoleInfo.Floor);

	fclose(fp);

	return true;
}

//=========================================================
//函数名称：MyRoleMagic
//说明：选择一个魔法
//输入：
//输出：
//返回：选择的魔法索引，-1为未选择
//备注：
//日期：2013-1-1
//修改记录：
//=========================================================
int MyRoleMagic()
{
	int Cho=0;
	int ChoMax;
	int Chox=0;
	int PageCount;
	char text[256];
	const char *pstr;
	string strFile;
	string strName1,strName2;
	SBMP BackBmp;
	SBMP ArrowBmp;
	int x,y;
	int w,h;
	int i;
	int Key;
	
	if(!g_RoleInfo.Magic.Count)
	{
		ShowMsg("没有魔法");
		return -1;
	}

	ChoMax=g_RoleInfo.Magic.Count-1;//最大可选项

	g_TempPage.CopyPage(g_GamePage);

	strFile=g_strPicDir;
	strFile+="Cho.bmp";

	BackBmp=LoadBmp(strFile.c_str());
	
	x=(WND_W-BackBmp.w)/2;
	y=(WND_H-BackBmp.h)/2;
	w=BackBmp.w;
	h=BackBmp.h;
	
	strFile=g_strPicDir;
	strFile+="Arrow.bmp";
	
	ArrowBmp=LoadBmp(strFile.c_str());

	pstr=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript;//文本描述信息
	//显示描述信息
	PageCount=strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)/(24*3);
	if(strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)%(32*3))
		PageCount++;

	i=0;
	
	while(1)
	{
		g_GamePage.ShowPic(BackBmp,x,y,0,0,w,h,RGB(255,0,255),true);//显示背景
		strName1=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho-Chox]].strName;
		
		//显示文本
		HollowWords(g_GamePage,strName1.c_str(),"宋体",16,false,x+80,y+10,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		
		if(ChoMax>=1)
		{
			strName2=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho-Chox+1]].strName;
			//显示文本
			HollowWords(g_GamePage,strName2.c_str(),"宋体",16,false,x+80,y+26,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		}
	
		pstr=&g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript[i*24*3];
		while(*pstr)
		{
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
		}
		
		
		g_GamePage.ShowPic(ArrowBmp,x+40,y+10+Chox*16-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);//显示箭头

		FLIPPAGE;

		Key=GetKey();

		if(Key==VK_UP)
		{
			Chox=0;
			Cho--;
			if(Cho<0) Cho=0;
			i=0;
			pstr=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript;//文本描述信息
			//显示描述信息
			PageCount=strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)/(24*3);
			if(strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)%(24*3))
				PageCount++;
		}
		else if(Key==VK_DOWN)
		{
			Chox=1;
			Cho++;
			if(Cho>ChoMax) Cho=ChoMax;
			if(Cho==0) Chox=0;
			i=0;
			pstr=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript;//文本描述信息
			//显示描述信息
			PageCount=strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)/(24*3);
			if(strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)%(24*3))
				PageCount++;
		}
		else if(Key==VK_RETURN)
		{
			goto EndMyRoleMagic;
		}
		else if(Key==VK_ESCAPE)
		{
			Cho=-1;
			goto EndMyRoleMagic;
		}
		else if(Key==VK_SPACE)
		{
			i++;
			pstr=&g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript[i*24*3];
		}
	
	}

EndMyRoleMagic:
	g_GamePage.CopyPage(g_TempPage);

	DeleteBmp(BackBmp);

	return Cho;
}

//=========================================================
//函数名称：MyRoleGoods
//说明：选择一个物品
//输入：
//输出：
//返回：选择的物品索引，-1为未选择
//备注：
//日期：2013-1-1
//修改记录：
//=========================================================
int MyRoleGoods()
{
	int Cho=0;
	int ChoMax;
	int Chox=0;
	int PageCount;
	char text[256];
	const char *pstr;
	string strFile;
	string strName1,strName2;
	SBMP BackBmp;
	SBMP ArrowBmp;
	int x,y;
	int w,h;
	int i;
	int Key;
	
	if(!g_RoleInfo.Goods.Count)
	{
		ShowMsg("没有物品");
		return -1;
	}

	ChoMax=g_RoleInfo.Goods.Count-1;//最大可选项

	g_TempPage.CopyPage(g_GamePage);

	strFile=g_strPicDir;
	strFile+="Cho.bmp";

	BackBmp=LoadBmp(strFile.c_str());
	
	x=(WND_W-BackBmp.w)/2;
	y=(WND_H-BackBmp.h)/2;
	w=BackBmp.w;
	h=BackBmp.h;
	
	strFile=g_strPicDir;
	strFile+="Arrow.bmp";
	
	ArrowBmp=LoadBmp(strFile.c_str());

	pstr=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript;//文本描述信息
	//显示描述信息
	PageCount=strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)/(24*3);
	if(strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)%(32*3))
		PageCount++;

	i=0;
	
	while(1)
	{
		g_GamePage.ShowPic(BackBmp,x,y,0,0,w,h,RGB(255,0,255),true);//显示背景
		strName1=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho-Chox]].strName;
		strName1+="x";
		sprintf(text,"%d",g_RoleInfo.Goods.GoodsCount[Cho-Chox]);
		strName1+=text;
		
		//显示文本
		HollowWords(g_GamePage,strName1.c_str(),"宋体",16,false,x+80,y+10,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		
		if(ChoMax>=1)
		{
			strName2=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho-Chox+1]].strName;
			strName2+="x";
			sprintf(text,"%d",g_RoleInfo.Goods.GoodsCount[Cho-Chox+1]);
			strName2+=text;

			//显示文本
			HollowWords(g_GamePage,strName2.c_str(),"宋体",16,false,x+80,y+26,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		}

		//显示物品图片
		DrawMap(g_GamePage,false,g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].ID+14,x+170,y+30);
	
		pstr=&g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript[i*24*3];
		while(*pstr)
		{
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"宋体",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
		}
		
		
		g_GamePage.ShowPic(ArrowBmp,x+40,y+10+Chox*16-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);//显示箭头

		FLIPPAGE;

		Key=GetKey();

		if(Key==VK_UP)
		{
			Chox=0;
			Cho--;
			if(Cho<0) Cho=0;
			i=0;
			pstr=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript;//文本描述信息
			//显示描述信息
			PageCount=strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)/(24*3);
			if(strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)%(24*3))
				PageCount++;
		}
		else if(Key==VK_DOWN)
		{
			Chox=1;
			Cho++;
			if(Cho>ChoMax) Cho=ChoMax;
			if(Cho==0) Chox=0;
			i=0;
			pstr=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript;//文本描述信息
			//显示描述信息
			PageCount=strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)/(24*3);
			if(strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)%(24*3))
				PageCount++;
		}
		else if(Key==VK_RETURN)
		{
			goto EndMyRoleGoods;
		}
		else if(Key==VK_ESCAPE)
		{
			Cho=-1;
			goto EndMyRoleGoods;
		}
		else if(Key==VK_SPACE)
		{
			i++;
			pstr=&g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript[i*24*3];
		}
	
	}

EndMyRoleGoods:
	g_GamePage.CopyPage(g_TempPage);

	DeleteBmp(BackBmp);

	return Cho;}


//=========================================================
//函数名称：AddRoleMagic
//说明：增加角色物品
//输入：
//输出：
//返回：
//备注：
//日期：2013-1-1
//修改记录：
//=========================================================
void AddRoleMagic(int ID)
{
	g_RoleInfo.Magic.ID[g_RoleInfo.Magic.Count]=ID;
	g_RoleInfo.Magic.Count++;
}

//=========================================================
//函数名称：AddRoleGoods
//说明：增加角色物品
//输入：
//输出：
//返回：
//备注：
//日期：2013-1-1
//修改记录：
//=========================================================
void AddRoleGoods(int ID)
{
	int i;

	for(i=0;i<g_RoleInfo.Goods.Count;i++)
	{
		if(g_RoleInfo.Goods.ID[i]==ID)
		{
			g_RoleInfo.Goods.GoodsCount[i]++;
			return;
		}
	}
	
	g_RoleInfo.Goods.ID[g_RoleInfo.Goods.Count]=ID;
	g_RoleInfo.Goods.GoodsCount[g_RoleInfo.Goods.Count]=1;
	g_RoleInfo.Goods.Count++;

	return;
}

//=========================================================
//函数名称：DelRoleGoods
//说明：删除角色物品
//输入：
//输出：
//返回：
//备注：
//日期：2013-1-1
//修改记录：
//=========================================================
void DelRoleGoods(int ID)
{
	int i;
	int j;

	for(i=0;i<g_RoleInfo.Goods.Count;i++)
	{
		if(g_RoleInfo.Goods.ID[i]==ID)
		{
			g_RoleInfo.Goods.GoodsCount[i]--;
			if(g_RoleInfo.Goods.GoodsCount[i]==0)
			{
				for(;i<g_RoleInfo.Goods.Count-1;i++)
				{
					g_RoleInfo.Goods.ID[i]=g_RoleInfo.Goods.ID[i+1];
					g_RoleInfo.Goods.GoodsCount[i]=g_RoleInfo.Goods.GoodsCount[i+1];
				}
				g_RoleInfo.Goods.Count--;
			}
			return;
		}
	}
}

//=========================================================
//函数名称：EscMenu
//说明：退出菜单
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
int EscMenu()
{
	SBMP BackBmp;
	SBMP ArrowBmp;
	string strFile;
	int x,y;
	int Cho=0;
	int Key=0;

	strFile=g_strPicDir;
	strFile+="EscBack.bmp";

	BackBmp=LoadBmp(strFile.c_str());

	strFile=g_strPicDir;
	strFile+="Arrow.bmp";

	ArrowBmp=LoadBmp(strFile.c_str());

	x=(WND_W-BackBmp.w)/2;
	y=(WND_H-BackBmp.h)/2;

	g_TempPage.CopyPage(g_GamePage);

	//绘制背景
	g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
	g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
	HollowWords(g_GamePage,"我的魔法","宋体",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"我的物品","宋体",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"保存游戏","宋体",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"登陆到追爱博客进行祝福","宋体",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"退出游戏","宋体",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);

	//绘制光标
	g_GamePage.ShowPic(ArrowBmp,x+20,y+40+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);

	FLIPPAGE;

	
	while(1)
	{
		if(Key)
		{
			//绘制背景
			g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
			g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
			HollowWords(g_GamePage,"我的魔法","宋体",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"我的物品","宋体",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"保存游戏","宋体",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"登陆到追爱博客进行祝福","宋体",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"退出游戏","宋体",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			
			//绘制光标
			g_GamePage.ShowPic(ArrowBmp,x+20,y+40+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);
			FLIPPAGE;
		}
		Key=GetKey();

		if(Key==VK_UP)
		{
			Cho--;
		}
		else if(Key==VK_DOWN)
		{
			Cho++;
		}
		else if(Key==VK_RETURN)
		{
			goto EndEscMenu;	
		}
		else if(Key==VK_ESCAPE)
		{
			Cho=-1;
			goto EndEscMenu;
		}
		if(Cho<0) Cho=4;
		if(Cho>4) Cho=0;
	}

EndEscMenu:
	g_GamePage.CopyPage(g_TempPage);

	DeleteBmp(BackBmp);
	DeleteBmp(ArrowBmp);

	if(Cho==2)//保存游戏
		SaveGame();
	else if(Cho==3)//登陆博客祝福
		ShellExecute(NULL,"OPEN","http://blog.sina.com.cn/s/blog_b2bbe82101016jum.html",NULL,NULL,SW_SHOW);


	return Cho;
}

//=========================================================
//函数名称：DrawFloor
//说明：绘制一层地图
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void DrawFloor(int nFloor)
{
	DrawAllMap(g_MapPage[0],false,nFloor);
	DrawAllMap(g_MapPage[1],true,nFloor);
}

//=========================================================
//函数名称：ArtText
//说明：艺术字
//输入：bVertical:是否为竖向
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void ArtText(const char *strText,int x,int y,int msDelay,bool bVertical)
{
	char c[3];
	const char *pstr=strText;

	while(*pstr)
	{
		memcpy(c,pstr,2);
		c[2]='\0';

		HollowWords(g_GamePage,c,"",16,false,x,y,0x000000,0xFFF0f0);
		if(bVertical)
			y+=16;
		else
			x+=16;
		Sleep(msDelay);
		FLIPPAGE;
		pstr+=2;
	}
}

//=========================================================
//函数名称：SetMapZero
//说明：设置地图数据为0
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void SetMapZero(int x,int y)
{
	g_Map[g_RoleInfo.Floor][x][y]=0;
	DrawMap(g_MapPage[0],false,0,x*TITLE_W,y*TITLE_H);
	DrawMap(g_MapPage[1],true,0,x*TITLE_W,y*TITLE_H);
}

//=========================================================
//函数名称：GameMenu
//说明：游戏主菜单
//输入：nCho：初始选项
//输出：
//返回：
//备注：
//日期：2012-12-15
//修改记录：
//=========================================================
int GameMenu(int nCho)
{
	SBMP BackBmp;
	SBMP MenuBmp;
	SBMP SelectBmp;
	UINT MusicID;
	int nChoose=nCho;
	string strFile;
	int Key=0;

	strFile=g_strPicDir;
	strFile+="主菜单背景.bmp";
	BackBmp=LoadBmp(strFile.c_str());
	strFile=g_strPicDir;
	strFile+="主菜单.bmp";
	MenuBmp=LoadBmp(strFile.c_str());
	strFile=g_strPicDir;
	strFile+="选项.bmp";
	SelectBmp=LoadBmp(strFile.c_str());

	strFile=g_strSoundDir;
	strFile+="按键.mp3";
	MusicID=LoadMusic(strFile.c_str());

	//画出黑色背景
	g_GamePage.FillRect(0,0,WND_W,WND_H,0x000000);

	//显示主菜单背景
	g_GamePage.ShowPic(BackBmp,(WND_W-BackBmp.w)/2,50,0,0,BackBmp.w,BackBmp.h,RGB(255,0,255),true);

	//显示标题
	g_GamePage.SetBkMode(TRANSPARENT);
	g_GamePage.SetTextColor(0x001000);
	g_GamePage.SetFont("华文行楷",50,SPaintPage::FE_NORMAL,false,false,false);
	g_GamePage.TextOut((WND_W-50*4)/2,100,"我的魔塔");
	
	//显示菜单
	g_GamePage.FillRect((WND_W-MenuBmp.w)/2,230,MenuBmp.w+3,MenuBmp.h+3,RGB(0,0,0));
	g_GamePage.ShowPic(SelectBmp,(WND_W-MenuBmp.w)/2,240+nChoose*40,0,0,SelectBmp.w,SelectBmp.h,RGB(255,0,255),true);
	g_GamePage.ShowPic(MenuBmp,(WND_W-MenuBmp.w)/2,230,0,0,MenuBmp.w,MenuBmp.h,RGB(255,0,255),true);

	FLIPPAGE;

	while(Key!=VK_RETURN && Key!=VK_ESCAPE)
	{
		Key=GetKey();
		if(Key)
		{
			if(Key==VK_UP)
			{
				nChoose--;
				if(nChoose<0) nChoose=3;
			}
			else if(Key==VK_DOWN)
			{
				nChoose++;
				if(nChoose>3) nChoose=0;
			}
			g_GamePage.FillRect((WND_W-MenuBmp.w)/2,230,MenuBmp.w+3,MenuBmp.h+3,RGB(0,0,0));
			g_GamePage.ShowPic(SelectBmp,(WND_W-MenuBmp.w)/2,240+nChoose*40,0,0,SelectBmp.w,SelectBmp.h,RGB(255,0,255),true);
			g_GamePage.ShowPic(MenuBmp,(WND_W-MenuBmp.w)/2,230,0,0,MenuBmp.w,MenuBmp.h,RGB(255,0,255),true);

			FLIPPAGE;

			PlayMusic(MusicID,false);
		}
	}
		
	DeleteBmp(BackBmp);
	DeleteBmp(MenuBmp);
	DeleteBmp(SelectBmp);

	if(Key==VK_ESCAPE) nChoose=3;

	CloseMusic(MusicID);

	return nChoose;
}

//=========================================================
//函数名称：Fight
//说明：角色战斗
//输入：bGut:是否为剧情战斗（战斗失败游戏仍继续）
//		bMustLoss:是否必须输了才结束战斗
//输出：0:战斗失败 1:战斗胜利 2:逃跑成功
//返回：true:战斗胜利
//备注：
//日期：2012-12-14
//修改记录：
//=========================================================
int Fight(int EnemyID,bool bGut,bool bMustLoss)
{
	EnemyInfo EInfo;//敌人属性
	SBMP FightBackBmp;//战斗背景
	SBMP FightMenuBmp;//战斗菜单
	SBMP HPMPBmp;//生命魔法栏
	string strFile;
	UINT AttackMusicID;
	int i;//循环计数
	int Hurt;//伤害值
	int x,y;//用于计算对象显示位置
	int px,py;
	int pw,ph;
	int ix,iy;
	int ex,ey;//敌人位置
	int xadd,yadd;//x，y坐标增量
	int MenuCho=0;//战斗菜单当前选项
	int Key=0;
	int Step;//角色步数
	char text[256];
	int Ret=0;

	//敌人属性复制
	memcpy(&EInfo,&g_EnemyInfo[EnemyID],sizeof(EnemyInfo));
	
	strFile=g_strPicDir;
	strFile+="FightBack.bmp";
	//载入战斗背景
	FightBackBmp=LoadBmp(strFile.c_str());
	
	strFile=g_strPicDir;
	strFile+="FightMenu.bmp";
	//载入战斗菜单
	FightMenuBmp=LoadBmp(strFile.c_str());

	strFile=g_strPicDir;
	strFile+="HPMP.bmp";
	//载入生命魔法栏
	HPMPBmp=LoadBmp(strFile.c_str());

	//载入打击音效
	strFile=g_strSoundDir;
	strFile+="打击.mp3";
	AttackMusicID=LoadMusic(strFile.c_str());
	
	while(1)
	{
		//敌人攻击
		Hurt=EInfo.Attack-g_RoleInfo.Defend;
		
		if(Hurt<1) Hurt=1;
		//敌人初始位置
		ex=(WND_W-FightBackBmp.w)/2+50;
		ey=(WND_H-FightBackBmp.h)/2+10;
		//角色初始位置
		ix=(WND_W-FightBackBmp.w)/2+110;
		iy=(WND_H-FightBackBmp.h)/2+70;
		xadd=(ix-ex)/10;
		yadd=(iy-ey)/10;
		
		while(ex<ix || ey<iy)
		{
			//绘制战斗背景
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightBackBmp.w;
			ph=FightBackBmp.h;
			g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
			g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
			
			//绘制敌人角色
			DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,ex,ey);
			
			//绘制角色
			pw=g_RoleBmp.w/3;
			ph=g_RoleBmp.h/4;
			g_GamePage.ShowPic(g_RoleBmp,ix,iy,0,0,pw,ph,RGB(255,0,255),true);
			
			FLIPPAGE;
			
			Sleep(100);
			
			if(ex<ix) ex+=10;
			if(ey<iy) ey+=10;
		}
		Key=0;
		
		PlayMusic(AttackMusicID,false);
		//绘制战斗背景
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		pw=FightBackBmp.w;
		ph=FightBackBmp.h;
		g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
		g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
		
		//绘制敌人角色
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
		
		//绘制角色
		pw=g_RoleBmp.w/3;
		ph=g_RoleBmp.h/4;
		g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
		
		//绘制战斗菜单
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		pw=FightMenuBmp.w;
		ph=FightMenuBmp.h/4;
		g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
		
		//绘制生命魔法栏
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		pw=HPMPBmp.w;
		ph=HPMPBmp.h;
		g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
		//绘制生命魔法数值
		itoa(g_RoleInfo.HP,text,10);
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
		itoa(g_RoleInfo.HPMax,text,10);
		HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
		itoa(g_RoleInfo.MP,text,10);
		HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
		itoa(g_RoleInfo.MPMax,text,10);
		HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
		
		//伤害值计算
		Hurt=EInfo.Attack-g_RoleInfo.Defend;
		if(Hurt<=0) Hurt=1;
		//在角色上飘起伤害值
		i=0;
		while(i<5)
		{
			//绘制战斗背景
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightBackBmp.w;
			ph=FightBackBmp.h;
			g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
			g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
			
			//绘制敌人角色
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
			
			//绘制角色
			pw=g_RoleBmp.w/3;
			ph=g_RoleBmp.h/4;
			g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
			
			//绘制战斗菜单
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightMenuBmp.w;
			ph=FightMenuBmp.h/4;
			g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
			
			//绘制生命魔法栏
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=HPMPBmp.w;
			ph=HPMPBmp.h;
			g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
			//绘制生命魔法数值
			itoa(g_RoleInfo.HP,text,10);
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			itoa(g_RoleInfo.HPMax,text,10);
			HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			itoa(g_RoleInfo.MP,text,10);
			HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			itoa(g_RoleInfo.MPMax,text,10);
			HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			
			//绘制伤害值
			itoa(Hurt,text,10);
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
			HollowWords(g_GamePage,text,"",12,false,x+115,y+70-i,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			
			FLIPPAGE;
			Sleep(100);
			i++;
		}
		g_RoleInfo.HP-=Hurt;
		if(g_RoleInfo.HP<=0)//战斗失败
		{
			ShowMsg("战斗失败！");
			Ret=0;
			goto EndFight;
		}
EndEnemyAttack:

		while(Key!=VK_RETURN)
		{
			if(Key==0)
			{
				//绘制战斗背景
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//绘制敌人角色
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
				
				//绘制角色
				pw=g_RoleBmp.w/3;
				ph=g_RoleBmp.h/4;
				g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
				
				//绘制战斗菜单
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightMenuBmp.w;
				ph=FightMenuBmp.h/4;
				g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
				
				//绘制生命魔法栏
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=HPMPBmp.w;
				ph=HPMPBmp.h;
				g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
				//绘制生命魔法数值
				itoa(g_RoleInfo.HP,text,10);
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.HPMax,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.MP,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.MPMax,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			}
			Key=GetKey();
			
			if(Key==VK_UP) MenuCho=FIGHT_MAGIC;
			else if(Key==VK_DOWN) MenuCho=FIGHT_GOODS;
			else if(Key==VK_LEFT) MenuCho=FIGHT_ATTACK;
			else if(Key==VK_RIGHT) MenuCho=FIGHT_ESCAPE;
			
			//绘制战斗菜单
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightMenuBmp.w;
			ph=FightMenuBmp.h/4;
			g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
			
			FLIPPAGE;
		}
		if(MenuCho==FIGHT_ATTACK)//普通攻击
		{
			//敌人初始位置
			ex=(WND_W-FightBackBmp.w)/2+50;
			ey=(WND_H-FightBackBmp.h)/2+10;
			//角色初始位置
			ix=(WND_W-FightBackBmp.w)/2+110;
			iy=(WND_H-FightBackBmp.h)/2+70;
			xadd=(ix-x)/10;
			yadd=(iy-y)/10;
			Step=0;
			while(ix>ex || iy>ey)
			{
				//绘制战斗背景
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//绘制敌人角色
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,ex,ey);
				
				//绘制角色
				pw=g_RoleBmp.w/3;
				ph=g_RoleBmp.h/4;
				g_GamePage.ShowPic(g_RoleBmp,ix,iy,TITLE_W*Step,0,pw,ph,RGB(255,0,255),true);

				FLIPPAGE;

				Sleep(100);
				
				if(ix>ex) ix-=10;
				if(iy>ey) iy-=10;
				Step++;
				if(Step>2) Step=0;
			}
			Key=0;

			PlayMusic(AttackMusicID,false);
			//绘制战斗背景
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightBackBmp.w;
			ph=FightBackBmp.h;
			g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
			g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
			
			//绘制敌人角色
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
			
			//绘制角色
			pw=g_RoleBmp.w/3;
			ph=g_RoleBmp.h/4;
			g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
			
			//绘制战斗菜单
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightMenuBmp.w;
			ph=FightMenuBmp.h/4;
			g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
			
			//绘制生命魔法栏
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=HPMPBmp.w;
			ph=HPMPBmp.h;
			g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
			//绘制生命魔法数值
			itoa(g_RoleInfo.HP,text,10);
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			itoa(g_RoleInfo.HPMax,text,10);
			HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			itoa(g_RoleInfo.MP,text,10);
			HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			itoa(g_RoleInfo.MPMax,text,10);
			HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
			
			//伤害值计算
			Hurt=g_RoleInfo.Attack-EInfo.Defend;
			if(Hurt<=0) Hurt=1;
			//在敌人角色上飘起伤害值
			i=0;
			while(i<5)
			{
				//绘制战斗背景
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//绘制敌人角色
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
				
				//绘制角色
				pw=g_RoleBmp.w/3;
				ph=g_RoleBmp.h/4;
				g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
				
				//绘制战斗菜单
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightMenuBmp.w;
				ph=FightMenuBmp.h/4;
				g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
				
				//绘制生命魔法栏
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=HPMPBmp.w;
				ph=HPMPBmp.h;
				g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
				//绘制生命魔法数值
				itoa(g_RoleInfo.HP,text,10);
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.HPMax,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.MP,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.MPMax,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);

				//绘制伤害值
				itoa(Hurt,text,10);
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
				HollowWords(g_GamePage,text,"",12,false,x+60,y+10-i,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);

				FLIPPAGE;
				Sleep(100);
				i++;
			}
			EInfo.HP-=Hurt;
			if(EInfo.HP<=0)//战斗胜利
			{
				ShowMsg("战斗胜利！");
				Ret=1;
				goto EndFight;
			}
		}
		else if(MenuCho==FIGHT_MAGIC)
		{
			int Cho;
			int HP;
			int MP;

			Cho=MyRoleMagic();

			if(Cho==-1)
			{
				Key=0;
				goto EndEnemyAttack;
			}

			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			
			HP=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].HP;
			MP=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].MP;
			if(MP>g_RoleInfo.MP)
			{
				ShowMsg("魔法不足");
				FLIPPAGE;
				goto EndEnemyAttack;
			}
			if(HP<0)
			{
				x=x+100;
				y=y+70;
			}
			else
			{
				x=x+50;
				y=y+10;
			}
			PlayMagic(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strName,x,y,g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].e,100);
			g_RoleInfo.MP-=MP;
			if(HP<0)
			{
				//在角色上飘起伤害值
				i=0;
				while(i<5)
				{
					//绘制战斗背景
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightBackBmp.w;
					ph=FightBackBmp.h;
					g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
					g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
					
					//绘制敌人角色
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
					
					//绘制角色
					pw=g_RoleBmp.w/3;
					ph=g_RoleBmp.h/4;
					g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
					
					//绘制战斗菜单
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightMenuBmp.w;
					ph=FightMenuBmp.h/4;
					g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
					
					//绘制生命魔法栏
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=HPMPBmp.w;
					ph=HPMPBmp.h;
					g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
					//绘制生命魔法数值
					itoa(g_RoleInfo.HP,text,10);
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					itoa(g_RoleInfo.HPMax,text,10);
					HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					itoa(g_RoleInfo.MP,text,10);
					HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					itoa(g_RoleInfo.MPMax,text,10);
					HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					
					//绘制伤害值
					itoa(-HP,text,10);
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
					HollowWords(g_GamePage,text,"",12,false,x+115,y+70-i,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					
					FLIPPAGE;
					Sleep(100);
					i++;
				}
				g_RoleInfo.HP+=-HP;
				if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
			}
			else
			{	
				//在敌人角色上飘起伤害值
				i=0;
				while(i<5)
				{
					//绘制战斗背景
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightBackBmp.w;
					ph=FightBackBmp.h;
					g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
					g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
					
					//绘制敌人角色
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
					
					//绘制角色
					pw=g_RoleBmp.w/3;
					ph=g_RoleBmp.h/4;
					g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
					
					//绘制战斗菜单
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightMenuBmp.w;
					ph=FightMenuBmp.h/4;
					g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
					
					//绘制生命魔法栏
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=HPMPBmp.w;
					ph=HPMPBmp.h;
					g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
					//绘制生命魔法数值
					itoa(g_RoleInfo.HP,text,10);
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					itoa(g_RoleInfo.HPMax,text,10);
					HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					itoa(g_RoleInfo.MP,text,10);
					HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					itoa(g_RoleInfo.MPMax,text,10);
					HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					
					//绘制伤害值
					itoa(HP,text,10);
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
					HollowWords(g_GamePage,text,"",12,false,x+60,y+10-i,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
					
					FLIPPAGE;
					Sleep(100);
					i++;
				}
				EInfo.HP-=HP;
				if(EInfo.HP<=0)//战斗胜利
				{
					ShowMsg("战斗胜利！");
					Ret=1;
					goto EndFight;
				}
			}
		}
		else if(MenuCho==FIGHT_GOODS)
		{
			int nRet;
			int HP,MP;
			
			nRet=MyRoleGoods();
			
			if(nRet>=0)
			{
				if(g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].bFight)
				{
					x=(WND_W-FightBackBmp.w)/2+100;
					y=(WND_H-FightBackBmp.h)/2+70;
					PlayMagic("恢复术",x,y,17,100);
					g_RoleInfo.HP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].HP;
					g_RoleInfo.MP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].MP;
					if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
					if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
					DelRoleGoods(g_RoleInfo.Goods.ID[nRet]);
					HP=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].HP;
					MP=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].MP;
					if(HP>0)
					{
						//在角色上飘起伤害值
						i=0;
						while(i<5)
						{
							//绘制战斗背景
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							pw=FightBackBmp.w;
							ph=FightBackBmp.h;
							g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
							g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
							
							//绘制敌人角色
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
							
							//绘制角色
							pw=g_RoleBmp.w/3;
							ph=g_RoleBmp.h/4;
							g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
							
							//绘制战斗菜单
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							pw=FightMenuBmp.w;
							ph=FightMenuBmp.h/4;
							g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
							
							//绘制生命魔法栏
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							pw=HPMPBmp.w;
							ph=HPMPBmp.h;
							g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
							//绘制生命魔法数值
							itoa(g_RoleInfo.HP,text,10);
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
							itoa(g_RoleInfo.HPMax,text,10);
							HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
							itoa(g_RoleInfo.MP,text,10);
							HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
							itoa(g_RoleInfo.MPMax,text,10);
							HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
							
							//绘制伤害值
							itoa(HP,text,10);
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
							HollowWords(g_GamePage,text,"",12,false,x+115,y+70-i,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
							
							FLIPPAGE;
							Sleep(100);
							i++;
						}
						if(MP>0)
						{
							//在角色上飘起伤害值
							i=0;
							while(i<5)
							{
								//绘制战斗背景
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								pw=FightBackBmp.w;
								ph=FightBackBmp.h;
								g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
								g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
								
								//绘制敌人角色
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
								
								//绘制角色
								pw=g_RoleBmp.w/3;
								ph=g_RoleBmp.h/4;
								g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
								
								//绘制战斗菜单
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								pw=FightMenuBmp.w;
								ph=FightMenuBmp.h/4;
								g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
								
								//绘制生命魔法栏
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								pw=HPMPBmp.w;
								ph=HPMPBmp.h;
								g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
								//绘制生命魔法数值
								itoa(g_RoleInfo.HP,text,10);
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
								itoa(g_RoleInfo.HPMax,text,10);
								HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
								itoa(g_RoleInfo.MP,text,10);
								HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
								itoa(g_RoleInfo.MPMax,text,10);
								HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
								
								//绘制伤害值
								itoa(MP,text,10);
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
								HollowWords(g_GamePage,text,"",12,false,x+115,y+70-i,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
								
								FLIPPAGE;
								Sleep(100);
								i++;
							}
						}
					}
				}
				else
				{
					ShowMsg("战斗不可用");
					FLIPPAGE;
					Key=0;
					goto EndEnemyAttack;
				}
			}
			else
			{
				Key=0;
				goto EndEnemyAttack;
			}
		}
		else if(MenuCho==FIGHT_ESCAPE)//逃跑
		{
			//角色后退
			i=0;
			Step=0;
			while(i<5)
			{
				//绘制战斗背景
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//绘制敌人角色
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
				
				//绘制角色
				pw=g_RoleBmp.w/3;
				ph=g_RoleBmp.h/4;
				g_GamePage.ShowPic(g_RoleBmp,x+110,y+70+i*5,Step*TITLE_W,0,pw,ph,RGB(255,0,255),true);

				Step++;
				if(Step>2) Step=0;
				
				//绘制战斗菜单
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightMenuBmp.w;
				ph=FightMenuBmp.h/4;
				g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
				
				//绘制生命魔法栏
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=HPMPBmp.w;
				ph=HPMPBmp.h;
				g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
				//绘制生命魔法数值
				itoa(g_RoleInfo.HP,text,10);
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				HollowWords(g_GamePage,text,"",12,false,x+135,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.HPMax,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+165,y+114,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.MP,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+135,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);
				itoa(g_RoleInfo.MPMax,text,10);
				HollowWords(g_GamePage,text,"",12,false,x+165,y+126,COLOR_HPMP_FRONT,COLOR_HPMP_BACK);

				FLIPPAGE;
				Sleep(100);
				i++;
			}
			if(bGut)//如果为剧情模式，直接逃跑失败
			{
				ShowMsg("逃跑失败！");
			}
			else//百分之二十五的逃跑机会
			{
				if(rand()%4<1)
				{
					ShowMsg("逃跑成功！");
					Ret=2;
					goto EndFight;
				}
				else
				{
					ShowMsg("逃跑失败！");
				}
			}
			Key=0;
		}
	}

EndFight:

	if(EInfo.HP<=0)
	{
		char text[256];
		g_RoleInfo.Experience+=EInfo.Experience;
		g_RoleInfo.Money+=EInfo.Money;
		sprintf(text,"获得金币：%d",EInfo.Money);
		ShowMsg(text);
		sprintf(text,"获得经验：%d",EInfo.Experience);
		ShowMsg(text);
	}

	CloseMusic(AttackMusicID);
	DeleteBmp(FightBackBmp);
	DeleteBmp(FightMenuBmp);
	DeleteBmp(HPMPBmp);

	return Ret;
}

//=========================================================
//函数名称：StartFlash
//说明：开始地图块闪动
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void StartFlash()
{
	SetTimer(g_hMainWnd,TIMER_FLASH,TIMER_FLASH_DELAY,FlashTimerProc);
}


//=========================================================
//函数名称：CloseFlash
//说明：关闭地图块闪动
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void CloseFlash()
{
	KillTimer(g_hMainWnd,TIMER_FLASH);
}

//=========================================================
//函数名称：FlashTimerProc
//说明：地图块闪烁计时器
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
VOID CALLBACK FlashTimerProc(
  HWND hwnd,     // handle of window for timer messages
  UINT uMsg,     // WM_TIMER message
  UINT idEvent,  // timer identifier
  DWORD dwTime   // current system time
)
{
	if(idEvent==TIMER_FLASH)
	{
		g_bShowMap2=g_bShowMap2?0:1;
	}
}


//=========================================================
//函数名称：GameOver
//说明：游戏失败
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void GameOver()
{
	g_TempPage.CopyPage(g_GamePage);
	ArtBack(g_GamePage,0,true);
	ArtText("梦",500,100,300,true);
	ArtText("总要醒来",450,100,300,true);
	ArtText("爱",400,100,300,true);
	ArtText("何必执著",350,100,300,true);
	ArtText("一切重新来过",300,100,300,true);
	g_GamePage.CopyPage(g_TempPage);
	Sleep(2000);
}

//=========================================================
//函数名称：DriveMap
//说明：操纵地图
//输入：
//输出：
//返回：遇到的特殊地图块ID
//备注：
//日期：2012-12-30
//修改记录：
//=========================================================
int DriveMap()
{
	string strFile;
	char text[256];
	UINT MusicID;
	int Ret;
	int Key=0;
	int x,y;

	strFile+=g_strSoundDir;
	strFile+="按键2.mp3";
	MusicID=LoadMusic(strFile.c_str());

	while(1)
	{
		ShowScene();
		FLIPPAGE;

		Key=GetKey();
		
		if(Key!=VK_ESCAPE && Key)
		{
			g_RoleInfo.Step++;
	//		PlayMusic(MusicID,false);
		}
		if(g_RoleInfo.Step>2) g_RoleInfo.Step=0;
		x=g_RoleInfo.x;
		y=g_RoleInfo.y;
		if(Key==VK_UP)
		{
			g_RoleInfo.Face=FACE_UP;
			y--;
		}
		else if(Key==VK_DOWN)
		{
			g_RoleInfo.Face=FACE_DOWN;
			y++;
		}
		else if(Key==VK_LEFT)
		{
			g_RoleInfo.Face=FACE_LEFT;
			x--;
		}
		else if(Key==VK_RIGHT)
		{
			g_RoleInfo.Face=FACE_RIGHT;
			x++;
		}
		else if(Key==VK_ESCAPE)
		{
			int HP,MP;
			Ret=EscMenu();
			if(Ret==0)//我的魔法
			{
				Ret=MyRoleMagic();

				if(Ret>=0)
				{
					if(g_MagicList.Info[g_RoleInfo.Magic.ID[Ret]].HP>0)
					{
						ShowMsg("战斗使用");
						FLIPPAGE;
					}
					else
					{
						if(g_RoleInfo.MP>=g_MagicList.Info[g_RoleInfo.Magic.ID[Ret]].MP)
						{
							g_RoleInfo.HP+=-g_MagicList.Info[g_RoleInfo.Magic.ID[Ret]].HP;
							g_RoleInfo.MP-=g_MagicList.Info[g_RoleInfo.Magic.ID[Ret]].MP;
							if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
							if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
						}
						else
						{
							ShowMsg("魔法不足");
						}
					}
				}
			}
			else if(Ret==1)//我的物品
			{
				Ret=MyRoleGoods();

				if(Ret>=0)
				{
					if(g_RoleInfo.Goods.ID[Ret]==21)//神灯
					{
						ShowMsg("0层宝石门打开");
						DelRoleGoods(21);//删除神灯
						g_Map[0][3][8]=0;
						FLIPPAGE;
					}
					else if(g_RoleInfo.Goods.ID[Ret]==24)//监狱钥匙
					{
						if(g_Map[g_RoleInfo.Floor][g_RoleInfo.x][g_RoleInfo.y-1]==10)//是否在监狱门旁边
						{
							ClearTitle(g_RoleInfo.x,g_RoleInfo.y-1);
						}
						DelRoleGoods(24);
					}
					else if(g_RoleInfo.Goods.ID[Ret]==25)//铁榔头
					{
						int tx,ty;

						tx=g_RoleInfo.x;
						ty=g_RoleInfo.y;
						if(g_RoleInfo.Face==FACE_UP)
						{
							ty--;
						}
						else if(g_RoleInfo.Face==FACE_DOWN)
						{
							ty++;
						}
						else if(g_RoleInfo.Face==FACE_LEFT)
						{
							tx--;
						}
						else if(g_RoleInfo.Face==FACE_RIGHT)
						{
							tx++;
						}
						if(g_Map[g_RoleInfo.Floor][tx][ty]==1)//面对土墙
						{
							ClearTitle(tx,ty);
							DelRoleGoods(25);
						}
						else
						{
							ShowMsg("面对土墙使用");
						}
					}
					else if(g_RoleInfo.Goods.ID[Ret]==40-14)//宝石榔头
					{
						int tx,ty;

						tx=g_RoleInfo.x;
						ty=g_RoleInfo.y;
						if(g_RoleInfo.Face==FACE_UP)
						{
							ty--;
						}
						else if(g_RoleInfo.Face==FACE_DOWN)
						{
							ty++;
						}
						else if(g_RoleInfo.Face==FACE_LEFT)
						{
							tx--;
						}
						else if(g_RoleInfo.Face==FACE_RIGHT)
						{
							tx++;
						}
						if(g_Map[g_RoleInfo.Floor][tx][ty]==9 && g_RoleInfo.Floor==4)//面对四楼宝石门
						{
							ClearTitle(tx,ty);
							DelRoleGoods(40-14);
						}
						else
						{
							ShowMsg("面对宝石门使用");
						}
					}
					else if(g_RoleInfo.Goods.ID[Ret]==22-14)//破天剑
					{
						ShowMsg("学会 破天剑");
						AddRoleMagic(MAGIC_SWORD);
						g_RoleInfo.HP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].HP;
						g_RoleInfo.MP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].MP;
						if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
						if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
						g_RoleInfo.Attack+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Attack;
						g_RoleInfo.Defend+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Defend;
						DelRoleGoods(g_RoleInfo.Goods.ID[Ret]);
					}
					else if(g_RoleInfo.Goods.ID[Ret]==36-14)//信仰石
					{
						Say("问道仙人：一个人的能力有限，但信仰无限，只要心存善念，为苍生而战，必定所向披靡！","问道仙人",true);
						g_RoleInfo.HP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].HP;
						g_RoleInfo.MP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].MP;
						if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
						if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
						g_RoleInfo.Attack+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Attack;
						g_RoleInfo.Defend+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Defend;
						DelRoleGoods(g_RoleInfo.Goods.ID[Ret]);
					}
					else if(g_RoleInfo.Goods.ID[Ret]==37-14)//风之罗盘
					{
						Say("作者：感觉回合制游戏这个风之罗盘的作用不大，所以。。。其实是有点偷懒了。。。。增强你的能力作为补偿吧","作者",true);
						g_RoleInfo.HPMax+=100;
						g_RoleInfo.MPMax+=100;
						ShowMsg("能力提升");
						DelRoleGoods(37-14);
					}
					else if(g_RoleInfo.Goods.ID[Ret]>=31-14 && g_RoleInfo.Goods.ID[Ret]<=33-14)//飞梭
					{
						GotoFloor(g_RoleInfo.Goods.ID[Ret]-31+14);
						Ret=9999;
						goto EndDriveMap;
					}
					else if(g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].bFight==false &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].HP==0 &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].MP==0 &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Attack==0 &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Defend==0)//剧情物品
					{
						ShowMsg("剧情物品");
						FLIPPAGE;
					}
					else
					{
						g_RoleInfo.HP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].HP;
						g_RoleInfo.MP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].MP;
						if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
						if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
						g_RoleInfo.Attack+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Attack;
						g_RoleInfo.Defend+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Defend;
						DelRoleGoods(g_RoleInfo.Goods.ID[Ret]);
					}
				}
			}
			else if(Ret==4)//退出游戏
			{
				Ret=-1;
				goto EndDriveMap;
			}
		}
		if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==0)
		{
			g_RoleInfo.x=x;
			g_RoleInfo.y=y;
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==6)//粉红门
		{
			if(g_RoleInfo.PowderKeyCount>0)
			{
				g_RoleInfo.PowderKeyCount--;
				ClearTitle(x,y);
			}
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==7)//蓝色门
		{
			if(g_RoleInfo.BlueKeyCount>0)
			{
				g_RoleInfo.BlueKeyCount--;
				ClearTitle(x,y);
			}
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==8)//红色门
		{
			if(g_RoleInfo.RedKeyCount>0)
			{
				g_RoleInfo.RedKeyCount--;
				ClearTitle(x,y);
			}
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==11)//粉红钥匙
		{
			ShowMsg("获得 粉红钥匙");
			g_RoleInfo.PowderKeyCount++;
			SetMapZero(x,y);
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==12)//蓝色钥匙
		{
			ShowMsg("获得 蓝色钥匙");
			g_RoleInfo.BlueKeyCount++;
			SetMapZero(x,y);
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==13)//红色钥匙
		{
			ShowMsg("获得 红色钥匙");
			g_RoleInfo.RedKeyCount++;
			SetMapZero(x,y);
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]>=14
						&& g_Map[g_RoleInfo.Floor][x][y]<=41)
		{
			sprintf(text,"获得 %s",g_GoodsList.Info[g_Map[g_RoleInfo.Floor][x][y]-14].strName);
			ShowMsg(text);
			AddRoleGoods(g_Map[g_RoleInfo.Floor][x][y]-14);
			SetMapZero(x,y);
		}
		else if(g_Map[g_RoleInfo.Floor][x][y]>3)
		{
			Ret=g_Map[g_RoleInfo.Floor][x][y];
			goto EndDriveMap;
		}
	}
EndDriveMap:
	g_x=x;
	g_y=y;
	CloseMusic(MusicID);
	return Ret;
}

//=========================================================
//函数名称：Shop
//说明：商店
//输入：ID:商店ID
//输出：
//返回：
//备注：
//日期：2012-1-12
//修改记录：
//=========================================================
void Shop(int ID)
{

	SBMP BackBmp;
	SBMP ArrowBmp;
	string strFile;
	int x,y;
	int Cho=0;
	int Key=0;

	strFile=g_strPicDir;
	strFile+="EscBack.bmp";

	BackBmp=LoadBmp(strFile.c_str());

	strFile=g_strPicDir;
	strFile+="Arrow.bmp";

	ArrowBmp=LoadBmp(strFile.c_str());

	x=(WND_W-BackBmp.w)/2;
	y=(WND_H-BackBmp.h)/2;

	g_TempPage.CopyPage(g_GamePage);

	//绘制背景
	g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
	g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);

	if(ID==0)
	{
		HollowWords(g_GamePage,"生命+250","宋体",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"生命上限+20","宋体",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"攻击+3","宋体",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"防御+3","宋体",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"退出","宋体",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}
	else if(ID==1)
	{
		HollowWords(g_GamePage,"生命+1000","宋体",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"生命上限+50","宋体",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"攻击+10","宋体",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"防御+10","宋体",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"退出","宋体",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}

	//绘制光标
	g_GamePage.ShowPic(ArrowBmp,x+20,y+40+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);

	FLIPPAGE;

	
	while(1)
	{
		if(Key)
		{
			//绘制背景
			g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
			g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
			if(ID==0)
			{
				HollowWords(g_GamePage,"生命+250","宋体",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"生命上限+20","宋体",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"攻击+3","宋体",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"防御+3","宋体",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"退出","宋体",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			}
			else if(ID==1)
			{
				HollowWords(g_GamePage,"生命+1000","宋体",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"生命上限+50","宋体",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"攻击+10","宋体",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"防御+10","宋体",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"退出","宋体",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			}
			
			//绘制光标
			g_GamePage.ShowPic(ArrowBmp,x+20,y+40+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);
			FLIPPAGE;
		}
		Key=GetKey();

		if(Key==VK_UP)
		{
			Cho--;
		}
		else if(Key==VK_DOWN)
		{
			Cho++;
		}
		else if(Key==VK_RETURN)
		{
			goto EndShopMenu;	
		}
		else if(Key==VK_ESCAPE)
		{
			Cho=-1;
			goto EndShopMenu;
		}
		if(Cho<0) Cho=4;
		if(Cho>4) Cho=0;
	}

EndShopMenu:
	g_GamePage.CopyPage(g_TempPage);

	DeleteBmp(BackBmp);
	DeleteBmp(ArrowBmp);

	if(ID==0)
	{
		if(Cho==0)//生命+250
		{
			g_RoleInfo.HP+=250;
			if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
		}
		else if(Cho==1)//生命上限
		{
			g_RoleInfo.HPMax+=20;
		}
		else if(Cho==2)//攻击+3
		{
			g_RoleInfo.Attack+=3;
		}
		else if(Cho==3)//防御+3
		{
			g_RoleInfo.Defend+=3;
		}
		else
		{
			g_RoleInfo.Money+=25;
		}
		g_RoleInfo.Money-=25;
	}
	else if(ID==1)
	{
		if(Cho==0)//生命+1000
		{
			g_RoleInfo.HP+=1000;
			if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
		}
		else if(Cho==1)//生命上限
		{
			g_RoleInfo.HPMax+=50;
		}
		else if(Cho==2)//攻击+10
		{
			g_RoleInfo.Attack+=10;
		}
		else if(Cho==3)//防御+10
		{
			g_RoleInfo.Defend+=10;
		}
		else
		{
			g_RoleInfo.Money+=50;
		}
		g_RoleInfo.Money-=50;
	}
}

//=========================================================
//函数名称：AddGoods
//说明：加入物品表
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-30
//修改记录：
//=========================================================
void AddGoods(const char *strName,const char *strDescript,int Attack,int Defend,int HP,int MP,bool bFight,int ID)
{
	int i;

	i=g_GoodsList.Count;
	strcpy(g_GoodsList.Info[i].strName,strName);
	strcpy(g_GoodsList.Info[i].strDescript,strDescript);
	g_GoodsList.Info[i].Attack=Attack;
	g_GoodsList.Info[i].Defend=Defend;
	g_GoodsList.Info[i].HP=HP;
	g_GoodsList.Info[i].MP=MP;
	g_GoodsList.Info[i].bFight=bFight;
	g_GoodsList.Info[i].ID=ID;
	g_GoodsList.Count++;
}

//=========================================================
//函数名称：AddMagic
//说明：加入魔法表
//输入：
//输出：
//返回：
//备注：HP为负给角色加生命
//日期：2012-12-31
//修改记录：
//=========================================================
void AddMagic(const char *strName,const char *strDescript,int HP,int MP,int e,int ID)
{
	int i;

	i=g_MagicList.Count;

	strcpy(g_MagicList.Info[i].strName,strName);
	strcpy(g_MagicList.Info[i].strDescript,strDescript);
	g_MagicList.Info[i].HP=HP;
	g_MagicList.Info[i].MP=MP;
	g_MagicList.Info[i].e=e;
	g_MagicList.Info[i].ID=ID;
	
	g_MagicList.Count++;
}


//=========================================================
//函数名称：DataInit
//说明：数据初始化，魔法和物品都要在这里记录
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-30
//修改记录：
//=========================================================
void DataInit()
{
	//上楼梯坐标
	g_UpStair[0].x=9;
	g_UpStair[0].y=6;

	g_UpStair[1].x=1;
	g_UpStair[1].y=2;

	g_UpStair[2].x=10;
	g_UpStair[2].y=16;

	g_UpStair[3].x=1;
	g_UpStair[3].y=15;

	g_UpStair[4].x=2;
	g_UpStair[4].y=1;

	g_UpStair[5].x=15;
	g_UpStair[5].y=16;

	g_UpStair[6].x=10;
	g_UpStair[6].y=1;

	g_UpStair[7].x=16;
	g_UpStair[7].y=2;

	g_UpStair[8].x=16;
	g_UpStair[8].y=15;

	g_UpStair[9].x=15;
	g_UpStair[9].y=16;

	g_UpStair[10].x=1;
	g_UpStair[10].y=15;

	g_UpStair[11].x=16;
	g_UpStair[11].y=15;

	g_UpStair[12].x=2;
	g_UpStair[12].y=16;

	g_UpStair[13].x=2;
	g_UpStair[13].y=16;

	g_UpStair[14].x=2;
	g_UpStair[14].y=16;

	g_UpStair[15].x=16;
	g_UpStair[15].y=15;

	g_UpStair[16].x=2;
	g_UpStair[16].y=16;

	g_UpStair[17].x=2;
	g_UpStair[17].y=16;

	g_UpStair[18].x=15;
	g_UpStair[18].y=16;

	g_UpStair[19].x=1;
	g_UpStair[19].y=3;

	g_UpStair[20].x=8;
	g_UpStair[20].y=11;

	g_UpStair[21].x=8;
	g_UpStair[21].y=11;

	//下楼梯坐标
	g_DownStair[1].x=8;
	g_DownStair[1].y=8;

	g_DownStair[2].x=2;
	g_DownStair[2].y=1;

	g_DownStair[3].x=7;
	g_DownStair[3].y=15;

	g_DownStair[4].x=1;
	g_DownStair[4].y=15;

	g_DownStair[5].x=1;
	g_DownStair[5].y=2;

	g_DownStair[6].x=16;
	g_DownStair[6].y=15;

	g_DownStair[7].x=9;
	g_DownStair[7].y=2;

	g_DownStair[8].x=2;
	g_DownStair[8].y=11;

	g_DownStair[9].x=1;
	g_DownStair[9].y=2;

	g_DownStair[10].x=2;
	g_DownStair[10].y=16;

	g_DownStair[11].x=10;
	g_DownStair[11].y=1;

	g_DownStair[12].x=15;
	g_DownStair[12].y=1;

	g_DownStair[13].x=1;
	g_DownStair[13].y=2;

	g_DownStair[14].x=16;
	g_DownStair[14].y=2;

	g_DownStair[15].x=15;
	g_DownStair[15].y=1;

	g_DownStair[16].x=15;
	g_DownStair[16].y=1;

	g_DownStair[17].x=1;
	g_DownStair[17].y=2;

	g_DownStair[18].x=16;
	g_DownStair[18].y=2;

	g_DownStair[19].x=16;
	g_DownStair[19].y=3;

	g_DownStair[20].x=1;
	g_DownStair[20].y=3;

	g_DownStair[21].x=8;
	g_DownStair[21].y=8;

	g_DownStair[22].x=12;
	g_DownStair[22].y=6;


	//魔法
	memset(&g_MagicList,0,sizeof(g_MagicList));//置0魔法表
	AddMagic("恢复术","恢复生命150",-150,25,17,MAGIC_RECOVER);//恢复术
	AddMagic("风暴","召唤风暴攻击敌人",20,20,8,MAGIC_STORM);//风暴
	AddMagic("烈火","召唤烈火攻击敌人",70,30,15,MAGIC_HEATFIRE);//烈火
	AddMagic("天降巨石","从天而降的巨石威力有多大呢？",80,40,5,MAGIC_STONEFALL);//天降巨石
	AddMagic("极光","极地的光芒",85,40,6,MAGIC_AURORA);//极光
	AddMagic("地域炼火","从地域召唤出来的烈火",100,40,7,MAGIC_ZONEFIRE);//地域炼火
	AddMagic("鬼魂召唤","从地狱中召唤鬼魂出来",150,50,9,MAGIC_SOULOUT);//鬼魂召唤
	AddMagic("电闪雷鸣","以天之力惩处恶魔",200,60,7,MAGIC_THUNDER);//电闪雷鸣
	AddMagic("破天剑","相传可以一剑破天",250,70,8,MAGIC_SWORD);//破天剑

	//物品
	g_GoodsList.Count=0;
	AddGoods("红宝石","攻击+1",1,0,0,0,false,0);//红宝石
	AddGoods("蓝宝石","防御+1",0,1,0,0,false,1);//蓝宝石
	AddGoods("红药瓶","生命+150",0,0,150,0,true,2);//红药瓶
	AddGoods("蓝药瓶","魔法+150",0,0,0,150,true,3);//蓝药瓶
	AddGoods("铁剑","很普通的一把剑，攻击+10",10,0,0,0,false,4);//铁剑
	AddGoods("精钢剑","精钢打造的剑，攻击+20",20,0,0,0,false,5);//精钢剑
	AddGoods("勇者剑","以往人类勇者留下的宝剑，攻击+30",30,0,0,0,false,6);//勇者剑
	AddGoods("斩魔剑","为斩魔而铸造的剑，攻击+40",40,0,0,0,false,7);//斩魔剑
	AddGoods("破天剑","相传拿上此剑便可使出终极招术破天剑，攻击+50",50,0,0,0,false,8);//破天剑
	AddGoods("铁盾","用铁铸造的盾牌，防御+10",0,10,0,0,false,9);//铁盾
	AddGoods("精钢盾","精钢打造的盾牌，防御+20",0,20,0,0,false,10);//精钢盾
	AddGoods("铠甲","柔软且有一定的防御效果，防御+30",0,30,0,0,false,11);//铠甲
	AddGoods("勇者盾","人类勇者遗留下来的盾牌，防御+40",0,40,0,0,false,12);//勇者盾
	AddGoods("王者盾","人类王者的盾牌，防御+50",0,50,0,0,false,13);//王者盾
	AddGoods("红色守护","精灵的守护物品之一",0,0,0,0,false,14);//红色守护
	AddGoods("蓝色守护","精灵的守护物品之一",0,0,0,0,false,15);//蓝色守护
	AddGoods("淡兰守护","精灵的守护物品之一",0,0,0,0,false,16);//淡兰守护
	AddGoods("淡兰飞梭","可以在魔塔1~7层间穿梭",0,0,0,0,false,17);//淡兰飞梭
	AddGoods("红色飞梭","可以在魔塔8~15层间穿梭",0,0,0,0,false,18);//红色飞梭
	AddGoods("蓝色飞梭","可以在魔塔16~20层间穿梭",0,0,0,0,false,19);//蓝色飞梭
	AddGoods("十字","精灵一族留下的祝福，可以大大提升能力。攻击+100,防御+100",100,100,100,100,false,20);//十字
	AddGoods("神灯","据说可以实现你的愿望",0,0,0,0,false,21);//阿拉丁神灯
	AddGoods("信仰石","据说得到它便拥有了勇气",100,100,0,0,false,22);//信仰石
	AddGoods("风之罗盘","鬼知道什么东西！",0,0,0,0,false,23);//风之罗盘
	AddGoods("监狱钥匙","打开宝藏门的钥匙",0,0,0,0,false,24);//监狱钥匙
	AddGoods("铁榔头","可以打开面对的土墙",0,0,0,0,false,25);//铁榔头
	AddGoods("宝石榔头","可以打开宝藏",0,0,0,0,false,26);//宝石榔头
	AddGoods("铜锣烧","可以大大增强你的能力",100,100,100,100,false,27);//铜锣烧

	//角色
	memset(&g_RoleInfo,0,sizeof(RoleInfo));
	g_RoleInfo.Level=1;
	g_RoleInfo.HP=100;
	g_RoleInfo.HPMax=100;
	g_RoleInfo.MP=100;
	g_RoleInfo.MPMax=100;
	g_RoleInfo.Attack=5;
	g_RoleInfo.Defend=1;
	g_RoleInfo.Face=FACE_UP;
	g_RoleInfo.Step=0;
	g_RoleInfo.x=8;
	g_RoleInfo.y=17;
	g_RoleInfo.PowderKeyCount=0;
	g_RoleInfo.BlueKeyCount=0;
	g_RoleInfo.RedKeyCount=0;
	g_RoleInfo.Money=0;
	g_RoleInfo.Experience=0;
	memset(&g_RoleInfo.Goods,0,sizeof(g_RoleInfo.Goods));
	memset(&g_RoleInfo.Magic,0,sizeof(g_RoleInfo.Magic));
	//怪物
	memset(g_EnemyInfo,0,sizeof(g_EnemyInfo));

	SetEnemyInfo(51,10,0,4,2,3,3);
	SetEnemyInfo(52,15,0,5,3,4,4);
	SetEnemyInfo(53,20,0,10,6,5,5);
	SetEnemyInfo(54,30,0,15,8,4,6);
	SetEnemyInfo(55,40,0,50,2,4,7);
	SetEnemyInfo(56,50,0,60,2,5,8);
	SetEnemyInfo(57,60,0,65,2,3,9);
	SetEnemyInfo(58,70,0,80,2,8,10);
	SetEnemyInfo(59,80,0,85,2,9,11);
	SetEnemyInfo(60,90,0,100,2,5,12);
	SetEnemyInfo(61,100,100,110,2,8,13);
	SetEnemyInfo(62,110,110,120,200,7,14);
	SetEnemyInfo(63,120,120,130,210,4,15);
	SetEnemyInfo(64,130,130,150,220,5,16);
	SetEnemyInfo(65,140,140,180,280,3,17);
	SetEnemyInfo(66,150,150,200,290,3,18);
	SetEnemyInfo(67,160,160,280,300,5,19);
	SetEnemyInfo(68,170,170,300,420,5,20);
	SetEnemyInfo(69,180,180,450,450,6,21);
	SetEnemyInfo(70,190,190,450,500,5,22);
	SetEnemyInfo(71,200,200,500,550,7,23);
	SetEnemyInfo(72,210,210,550,580,4,24);
	SetEnemyInfo(73,220,220,580,600,5,25);
	SetEnemyInfo(74,230,230,590,620,5,26);
	SetEnemyInfo(75,240,240,600,650,4,27);
	SetEnemyInfo(76,250,250,620,680,6,28);
	SetEnemyInfo(77,260,260,650,700,5,29);
	SetEnemyInfo(78,270,70,680,720,4,30);
	SetEnemyInfo(79,280,380,720,720,6,31);
	SetEnemyInfo(80,500,800,750,750,100,50);
	SetEnemyInfo(81,5000,5000,800,800,200,100);
	SetEnemyInfo(98,5000,5000,1010,1500,200,500);
	SetEnemyInfo(89,6000,5000,1510,2000,300,1000);
}



//=========================================================
//函数名称：SetEnemyInfo
//说明：设置敌人信息
//输入：
//输出：
//返回：
//备注：
//日期：2013-1-1
//修改记录：
//=========================================================
void SetEnemyInfo(int ID,int HP,int MP,int Attack,int Defend,int Money,int Exp)
{
	g_EnemyInfo[ID].PicID=ID;
	g_EnemyInfo[ID].HP=HP;
	g_EnemyInfo[ID].MP=MP;
	g_EnemyInfo[ID].Attack=Attack;
	g_EnemyInfo[ID].Defend=Defend;
	g_EnemyInfo[ID].Money=Money;
	g_EnemyInfo[ID].Experience=Exp;
}
				 


//=========================================================
//函数名称：ClearTitle
//说明：用ID为0的地图块清除指定位置的地图块，带动画
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-31
//修改记录：
//=========================================================
void ClearTitle(int x,int y)
{
	int delx;
	int i;

	delx=TITLE_W/5;
	g_Map[g_RoleInfo.Floor][x][y]=0;

	for(i=0;i<delx;i++)
	{
		g_MapPage[0].ShowPic(g_MapBmp[0],x*TITLE_W+(TITLE_W-i*5)/2,y*TITLE_H,(TITLE_W-i*5)/2,0,i*5,TITLE_W,0,false);
		g_MapPage[1].ShowPic(g_MapBmp[0],x*TITLE_W+(TITLE_W-i*5)/2,y*TITLE_H,(TITLE_W-i*5)/2,0,i*5,TITLE_W,0,false);

		ShowScene();
		FLIPPAGE;
		Sleep(100);
	}
	DrawMap(g_MapPage[0],false,0,x*TITLE_W,y*TITLE_H);
	DrawMap(g_MapPage[1],true,0,x*TITLE_W,y*TITLE_H);
}

//=========================================================
//函数名称：PlayMagic
//说明：播放魔法动画
//输入：msDelay:每帧动画间隔时间
//输出：
//返回：
//备注：
//日期：2012-12-30
//修改记录：
//=========================================================
void PlayMagic(const char *strName,int x,int y,int e,int msDelay)
{
	string str;
	string strFile;
	int i;
	char text[256];
	SBMP Bmp;
	SPaintPage Pg;
	UINT MusicID;

	strFile=g_strSoundDir;
	strFile+="魔法.mp3";
	MusicID=LoadMusic(strFile.c_str());

	PlayMusic(MusicID,false);

	i=1;
	while(i<=e)
	{
		str=g_strPicDir;
		str+=strName;
		str+="\\";
		itoa(i,text,10);
		str+=text;
		str+=".bmp";
		
		Bmp=LoadBmp(str.c_str());


		if(i==1) Pg.SetPage(Bmp.w,Bmp.h);

		FLIPPAGE;

		Pg.ShowPic(Bmp,0,0,0,0,Bmp.w,Bmp.h,0,false);

		TransparentBlt(g_hMainWndDC,x,y,Bmp.w,Bmp.h,Pg.GetDC(),0,0,Bmp.w,Bmp.h,RGB(255,0,255));
		Sleep(msDelay);

		DeleteBmp(Bmp);
		i++;
	}

	CloseMusic(MusicID);
}


//=========================================================
//函数名称：Say
//说明：说话函数
//输入：strHeadFile:头像文件路径
//输出：
//返回：
//备注：
//日期：2012-12-30
//修改记录：
//=========================================================
void Say(const char *strText,const char *strHeadFile,bool bUp)
{
	string strFile;
	int x,y;
	SBMP Bmp;
	SPaintPage Pg;
	SPaintPage Pg2;
	const char *pstr=strText;
	char text[256];
	int PageCount;//页数
	int Key=0;
	int i;

	PageCount=strlen(strText)/(22*3);
	if(strlen(strText)%(22*3)!=0)
		PageCount++;

	g_TempPage.CopyPage(g_GamePage);

	if(strHeadFile)
	{
		
		strFile=g_strPicDir;
		strFile+=strHeadFile;
		strFile+=".Bmp";
		
		Bmp=LoadBmp(strFile.c_str());
		Pg.SetPage(Bmp.w,Bmp.h);
		
		x=(WND_W-TITLE_X_COUNT*TITLE_W)/2;
		y=(WND_H-TITLE_Y_COUNT*TITLE_H)/2;
		if(bUp)
		{
			x+=60;
			y+=100;
		}
		else
		{
			x+=300;
			y+=200;
		}
		
		g_GamePage.FillRect(x-1,y-1,Bmp.w+2,Bmp.h+2,0x000000);
		g_GamePage.ShowPic(Bmp,x,y,0,0,Bmp.w,Bmp.h,0,false);
		
		y+=Bmp.h+5;
		x-=10;
		
	}

	DeleteBmp(Bmp);

	strFile=g_strPicDir;
	strFile+="Say.bmp";

	Bmp=LoadBmp(strFile.c_str());

	i=1;
	while(i<=PageCount)
	{
		g_GamePage.ShowPic(Bmp,x,y,0,0,Bmp.w,Bmp.h,0,false);
		if(strlen(pstr)>=22)
		{
			memcpy(text,pstr,22);
			text[22]='\0';
			pstr+=22;
		}
		else
		{
			memcpy(text,pstr,strlen(pstr));
			text[strlen(pstr)]='\0';
			pstr+=strlen(pstr);
		}

		HollowWords(g_GamePage,text,"宋体",16,false,x+10,y+5,COLOR_SAY_FRONT,COLOR_SAY_BACK);
		if(strlen(pstr)>0)
		{
			if(strlen(pstr)>=22) 
			{
				memcpy(text,pstr,22);
				text[22]='\0';
				pstr+=22;
			}
			else
			{
				memcpy(text,pstr,strlen(pstr));
				text[strlen(pstr)]='\0';
				pstr+=strlen(pstr);
			}
			HollowWords(g_GamePage,text,"宋体",16,false,x+10,y+5+16,COLOR_SAY_FRONT,COLOR_SAY_BACK);
		}
		if(strlen(pstr)>0)
		{
			if(strlen(pstr)>=22) 
			{
				memcpy(text,pstr,22);
				text[22]='\0';
				pstr+=22;
			}
			else
			{
				memcpy(text,pstr,strlen(pstr));
				text[strlen(pstr)]='\0';
				pstr+=strlen(pstr);
			}
			HollowWords(g_GamePage,text,"宋体",16,false,x+10,y+5+32,COLOR_SAY_FRONT,COLOR_SAY_BACK);
		}

		FLIPPAGE;

		while(!GetKey());
		i++;
	}

	if(PageCount==1)
	{
	//	PlayMusic(MusicID,false);
	//	Sleep(500);
	}
	g_GamePage.CopyPage(g_TempPage);
	DeleteBmp(Bmp);
}

//=========================================================
//函数名称：StartLogo
//说明：开始Logo动画
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-15
//修改记录：
//=========================================================
void StartLogo()
{
	SBMP Bmp;
	string strFile;
	int R,G,B;

	strFile=g_strPicDir;
	strFile+="Logo.bmp";
	Bmp=LoadBmp(strFile.c_str());

	R=0,G=0,B=0;

	while(R<=255)
	{
		g_GamePage.FillRect((WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,Bmp.w,Bmp.h,RGB(R,G,B));
		g_GamePage.ShowPic(Bmp,(WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,0,0,Bmp.w,Bmp.h,RGB(255,0,255),true);
		FLIPPAGE;
		Sleep(200);
		R+=15;
		G+=15;
		B+=15;
	}

	while(R>0)
	{
		R-=15;
		G-=15;
		B-=15;
		g_GamePage.FillRect((WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,Bmp.w,Bmp.h,RGB(R,G,B));
		g_GamePage.ShowPic(Bmp,(WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,0,0,Bmp.w,Bmp.h,RGB(255,0,255),true);
		FLIPPAGE;
		Sleep(200);
	}

	DeleteBmp(Bmp);

	strFile=g_strPicDir;
	strFile+="为追爱而作Logo.bmp";
	Bmp=LoadBmp(strFile.c_str());

	R=0,G=0,B=0;

	while(R<=255)
	{
		g_GamePage.FillRect((WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,Bmp.w,Bmp.h,RGB(R,G,B));
		g_GamePage.ShowPic(Bmp,(WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,0,0,Bmp.w,Bmp.h,RGB(255,0,255),true);
		FLIPPAGE;
		Sleep(200);
		R+=15;
		G+=15;
		B+=15;
	}

	while(R>0)
	{
		R-=15;
		G-=15;
		B-=15;
		g_GamePage.FillRect((WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,Bmp.w,Bmp.h,RGB(R,G,B));
		g_GamePage.ShowPic(Bmp,(WND_W-Bmp.w)/2,(WND_H-Bmp.h)/2,0,0,Bmp.w,Bmp.h,RGB(255,0,255),true);
		FLIPPAGE;
		Sleep(200);
	}

	DeleteBmp(Bmp);
}

//=========================================================
//函数名称：ShowMsg
//说明：显示消息
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-30
//修改记录：
//=========================================================
void ShowMsg(const char *strMsg)
{
	string strFile;
	int x,y;
	SBMP Bmp;
	
	strFile=g_strPicDir;
	strFile+="Message.bmp";
	Bmp=LoadBmp(strFile.c_str());

	x=(WND_W-Bmp.w)/2;
	y=(WND_H-Bmp.h)/2;

	g_GamePage.ShowPic(Bmp,x,y,0,0,Bmp.w,Bmp.h,RGB(255,0,255),true);
	HollowWords(g_GamePage,strMsg,"宋体",16,false,(WND_W-16*strlen(strMsg))/2+25,(WND_H-16)/2,COLOR_MSG_FRONT,COLOR_MSG_BACK);
	FLIPPAGE;

	while(!GetKey());

	DeleteBmp(Bmp);
}

//=========================================================
//函数名称：PlayStart
//说明：开始动画
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-29
//修改记录：
//=========================================================
void PlayStart()
{
	SBMP Bmp;
	string strFile;
	SPaintPage Pg(WND_W,WND_H);
	char str[MAX_PATH];
	int i;
	UINT MusicID;

	
	strFile=g_strSoundDir;
	strFile+="鼓.wav";

	MusicID=LoadMusic(strFile.c_str());
	PlayMusic(MusicID,false);

	i=1;

	while(i<=76)
	{
		strFile=g_strPicDir;
		itoa(i,str,10);
		strFile+="开始\\";
		strFile+=str;
		strFile+=".bmp";
		Bmp=LoadBmp(strFile.c_str());
		Pg.ShowPic(Bmp,0,0,0,0,Bmp.w,Bmp.h,0,false);
		TransparentBlt(g_GamePage.GetDC(),0,0,WND_W,WND_H,Pg.GetDC(),0,0,Bmp.w,Bmp.h,RGB(255,0,255));
		DeleteBmp(Bmp);
		FLIPPAGE;
		Sleep(100);
		i++;
	}

	CloseMusic(MusicID);
}

//=========================================================
//函数名称：PlayLeave
//说明：飘叶动画
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-16
//修改记录：
//=========================================================
void PlayLeave()
{
	SBMP Bmp;
	string strFile;
	SPaintPage Pg(WND_W,WND_H);
	char str[MAX_PATH];
	int i;
	UINT MusicID;

	
	strFile=g_strSoundDir;
	strFile+="风.wav";

	MusicID=LoadMusic(strFile.c_str());
	PlayMusic(MusicID,false);

	i=1;

	while(i<=36)
	{
		strFile=g_strPicDir;
		itoa(i,str,10);
		strFile+="飘叶\\";
		strFile+=str;
		strFile+=".bmp";
		Bmp=LoadBmp(strFile.c_str());
		Pg.ShowPic(Bmp,0,0,0,0,Bmp.w,Bmp.h,0,false);
		TransparentBlt(g_GamePage.GetDC(),0,0,WND_W,WND_H,Pg.GetDC(),0,0,Bmp.w,Bmp.h,RGB(255,0,255));
		DeleteBmp(Bmp);
		FLIPPAGE;
		Sleep(100);
		i++;
	}

	CloseMusic(MusicID);
}

//=========================================================
//函数名称：GetKey
//说明：获取游戏按键
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-15
//修改记录：
//=========================================================
int GetKey()
{
	int Key;
	long Tick;

	Tick=GetTickCount();

	while(GetTickCount()-Tick<=KEY_DELAY)
	{
		if(g_bKeyDown[VK_UP])
		{
			Key=VK_UP;
		}
		else if(g_bKeyDown[VK_DOWN])
		{
			Key=VK_DOWN;
		}
		else if(g_bKeyDown[VK_LEFT])
		{
			Key=VK_LEFT;
		}
		else if(g_bKeyDown[VK_RIGHT])
		{
			Key=VK_RIGHT;
		}
		else if(g_bKeyDown[VK_ESCAPE])
		{
			Key=VK_ESCAPE;
		}
		else if(g_bKeyDown[VK_RETURN])
		{
			Key=VK_RETURN;
		}
		else if(g_bKeyDown[VK_SPACE])
		{
			Key=VK_SPACE;
		}
		else
		{
			Key=0;
		}
		if(Key!=0)
		{
			while(g_bKeyDown[Key]);
			break;
		}
	}

	return Key;
}


//=========================================================
//函数名称：ReadMap
//说明：读取一层地图数据
//输入：
//输出：
//返回：
//备注：
//日期：2012-1-12
//修改记录：
//=========================================================
void ReadMap(int ID)
{
	FILE *fp;
	char str[MAX_PATH];
	string strFile;
	int x,y;
	int t;


	itoa(ID,str,10);
	strFile=g_strMapDir;
	strFile+=str;
	strFile+=".map";
	fp=fopen(strFile.c_str(),"rb");
	
	if(!fp)
	{
		MessageBox(g_hMainWnd,"载入地图文件失败！","我的魔塔",MB_OK|MB_ICONSTOP);
		return;
	}
	
	fread(&t,4,1,fp);
	
	for(y=0;y<TITLE_Y_COUNT;y++)
	{
		for(x=0;x<TITLE_X_COUNT;x++)
		{
			fread(&g_Map[ID][x][y],4,1,fp);
		}
	}
	
	fclose(fp);
}

//=========================================================
//函数名称：ReadMap
//说明：读取地图数据
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-24
//修改记录：
//=========================================================
void ReadMap()
{
	FILE *fp;
	int i;
	char str[MAX_PATH];
	string strFile;
	int x,y;
	int t;

	for(i=0;i<MAP_COUNT;i++)
	{
		itoa(i,str,10);
		strFile=g_strMapDir;
		strFile+=str;
		strFile+=".map";
		fp=fopen(strFile.c_str(),"rb");

		if(!fp)
		{
			MessageBox(g_hMainWnd,"载入地图文件失败！","我的魔塔",MB_OK|MB_ICONSTOP);
			return;
		}

		fread(&t,4,1,fp);
		
		for(y=0;y<TITLE_Y_COUNT;y++)
		{
			for(x=0;x<TITLE_X_COUNT;x++)
			{
				fread(&g_Map[i][x][y],4,1,fp);
			}
		}

		fclose(fp);
	}
}

//=========================================================
//函数名称：ShowScene
//说明：显示整个场景
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-24
//修改记录：
//=========================================================
void ShowScene()
{
	int x,y;
	char text[256];
	string str;

	//黑色背景
	g_GamePage.FillRect(0,0,WND_W,WND_H,0x000000);

	//复制地图
	g_GamePage.CopyPage(g_MapPage[g_bShowMap2],(WND_W-TITLE_W*TITLE_X_COUNT)/2,
												(WND_H-TITLE_H*TITLE_Y_COUNT)/2,
												TITLE_W*TITLE_X_COUNT,TITLE_H*TITLE_Y_COUNT,0,0);
	//显示角色
	g_GamePage.ShowPic(g_RoleBmp,(WND_W-TITLE_W*TITLE_X_COUNT)/2+g_RoleInfo.x*TITLE_W,(WND_H-TITLE_H*TITLE_Y_COUNT)/2+g_RoleInfo.y*TITLE_H,
						g_RoleInfo.Step*TITLE_W,g_RoleInfo.Face*TITLE_H,TITLE_W,TITLE_H,
						RGB(255,0,255),true);


	//左边角色属性
	for(y=0;y<=17;y++)
		for(x=0;x<=2;x++)
		{
			DrawMap(g_GamePage,false,0,(WND_W-TITLE_W*TITLE_X_COUNT)/2-(x+1)*TITLE_W-3,(WND_H-TITLE_H*TITLE_Y_COUNT)/2+y*TITLE_H);
		}
	HollowWords(g_GamePage,"等级","宋体",20,false,20,20,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Level,text,10);

	HollowWords(g_GamePage,text,"宋体",16,true,60,50,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"生命","宋体",16,true,20,80,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.HP,text,10);
	str="";
	str+=text;
	str+='/';
	itoa(g_RoleInfo.HPMax,text,10);
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"宋体",12,true,20,100,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"魔法","宋体",16,true,20,120,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.MP,text,10);
	str="";
	str+=text;
	str+='/';
	itoa(g_RoleInfo.MPMax,text,10);
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"宋体",12,true,20,140,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"攻击","宋体",16,true,20,160,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Attack,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"宋体",12,true,20,180,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"防御","宋体",16,true,20,200,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Defend,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"宋体",12,true,20,220,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"金币","宋体",16,true,20,240,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Money,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"宋体",12,true,20,260,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);


	HollowWords(g_GamePage,"经验","宋体",16,true,20,280,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Experience,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"宋体",12,true,20,300,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	//绘制钥匙信息
	DrawMap(g_GamePage,false,11,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3,11*TITLE_H);//显示粉红钥匙
	DrawMap(g_GamePage,false,12,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3,12*TITLE_H);//显示蓝色钥匙
	DrawMap(g_GamePage,false,13,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3,13*TITLE_H);//显示红色钥匙
	
	//显示粉红钥匙个数
	itoa(g_RoleInfo.PowderKeyCount,text,10);
	str="";
	str+=text;
	str+="  ";
	str+="个";
	HollowWords(g_GamePage,str.c_str(),"黑体",16,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3+TITLE_W+5,11*TITLE_H+7,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	//显示蓝色钥匙个数
	itoa(g_RoleInfo.BlueKeyCount,text,10);
	str="";
	str+=text;
	str+="  ";
	str+="个";
	HollowWords(g_GamePage,str.c_str(),"黑体",16,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3+TITLE_W+5,12*TITLE_H+7,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	//显示红色钥匙个数
	itoa(g_RoleInfo.RedKeyCount,text,10);
	str="";
	str+=text;
	str+="  ";
	str+="个";
	HollowWords(g_GamePage,str.c_str(),"黑体",16,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3+TITLE_W+5,13*TITLE_H+7,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	//绘制第层数
	itoa(g_RoleInfo.Floor,text,10);
	str="第 ";
	str+=text;
	str+=" ";
	str+="层";
	HollowWords(g_GamePage,str.c_str(),"黑体",18,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-4*TITLE_W-3+TITLE_W+10,15*TITLE_H,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
}

//=========================================================
//函数名称：DrawAllMap
//说明：在指定页面绘制一层地图
//输入：bMap2:使用Map1绘制地图块
//输出：
//返回：
//备注：
//日期：2012-12-24
//修改记录：
//=========================================================
void DrawAllMap(SPaintPage &Pg,bool bMap2,int nFloor)
{
	int x,y;

	y=0;
	for(;y<TITLE_Y_COUNT;y++)
	{
		x=0;
		for(;x<TITLE_X_COUNT;x++)
		{
			DrawMap(Pg,bMap2,g_Map[nFloor][x][y],x*TITLE_W,y*TITLE_H);
		}
	}
}

//=========================================================
//函数名称：DrawMap
//说明：在指定页面指定位置绘制地图
//输入：bMap2:使用Map1绘制地图块
//输出：
//返回：
//备注：
//日期：2012-12-24
//修改记录：
//=========================================================
void DrawMap(SPaintPage &Pg,bool bMap2,int Data,int x,int y)
{
	int XCount;
	int px,py;

	XCount=g_MapBmp[bMap2].w/TITLE_W;
	px=(Data%XCount*TITLE_W);
	py=Data/XCount*TITLE_H;

	Pg.ShowPic(g_MapBmp[bMap2],x,y,px,py,TITLE_W,TITLE_H,0,false);
}

//=========================================================
//函数名称：Ever
//说明：大话西游经典台词：曾经有……
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-23
//修改记录：
//=========================================================
void Ever()
{
	char str[1024];
	SPaintPage *pPg=NULL;
	SBMP Bmp;
	string strFile;
	int y;

	strFile=g_strSoundDir;
	strFile+="";

	strFile=g_strPicDir;
	strFile+="渐入渐出蒙版.bmp";
	Bmp=LoadBmp(strFile.c_str());

	LoadString(g_hInstance,1,str,1024);
	pPg=CreatePageByText(str,"宋体",16,5);
	
	y=-50;
	while(-y<=pPg->GetHeight()-WND_H-50)
	{
		g_GamePage.ShowPic(Bmp,0,0,0,0,Bmp.w,Bmp.h,0,false);
		TransparentBlt(g_GamePage.GetDC(),0,0,WND_W,WND_H,pPg->GetDC(),0,-y,WND_W,WND_H,RGB(255,0,255));
		FLIPPAGE;
		y-=2;
		Sleep(100);
	}

	delete pPg;
}

//=========================================================
//函数名称：游戏序言
//说明：
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-23
//修改记录：
//=========================================================
void Foreword()
{
	char str[1024];
	SPaintPage *pPg=NULL;
	SBMP Bmp;
	string strFile;
	int y;

	strFile=g_strPicDir;
	strFile+="渐入渐出蒙版.bmp";
	Bmp=LoadBmp(strFile.c_str());

	LoadString(g_hInstance,1,str,1024);
	pPg=CreatePageByText(str,"宋体",16,5);
	
	y=-50;
	while(-y<=pPg->GetHeight()-WND_H-50)
	{
		g_GamePage.ShowPic(Bmp,0,0,0,0,Bmp.w,Bmp.h,0,false);
		TransparentBlt(g_GamePage.GetDC(),0,0,WND_W,WND_H,pPg->GetDC(),0,-y,WND_W,WND_H,RGB(255,0,255));
		FLIPPAGE;
		y-=2;
		Sleep(100);
	}

	DeleteBmp(Bmp);
	delete pPg;
}

//=========================================================
//函数名称：真情告白
//说明：
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-23
//修改记录：
//=========================================================
void TrueHeart()
{
	char str[1024];
	SPaintPage *pPg=NULL;
	SBMP Bmp;
	string strFile;
	string text;
	UINT MusicID;
	int y;

	strFile=g_strPicDir;
	strFile+="渐入渐出蒙版.bmp";
	Bmp=LoadBmp(strFile.c_str());

	strFile=g_strSoundDir;
	strFile+="Take Me to Your Heart.mp3";
	MusicID=LoadMusic(strFile.c_str());
	PlayMusic(MusicID,false);

	text="";
	LoadString(g_hInstance,2,str,1024);
	text+=str;
	LoadString(g_hInstance,3,str,1024);
	text+=str;
	LoadString(g_hInstance,4,str,1024);
	text+=str;
	pPg=CreatePageByText(text.c_str(),"宋体",16,5);

	y=-50;
	while(-y<=pPg->GetHeight()-WND_H-50)
	{
		g_GamePage.ShowPic(Bmp,0,0,0,0,Bmp.w,Bmp.h,0,false);
		TransparentBlt(g_GamePage.GetDC(),0,0,WND_W,WND_H,pPg->GetDC(),0,-y,WND_W,WND_H,RGB(255,0,255));
		FLIPPAGE;
		y-=1;
		Sleep(100);
	}

	DeleteBmp(Bmp);
	delete pPg;

	CloseMusic(MusicID);
}


//=========================================================
//函数名称：CreatePageByText
//说明：通过文本创建缓冲页面
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-23
//修改记录：
//=========================================================
SPaintPage *CreatePageByText(const char *strText,const char *strFont,int nFontSize,int nSpace)
{
	SPaintPage *pPg;
	int nLineCount;
	int i;
	int x;
	const char *p=NULL;
	string str;

	nLineCount=1;

	p=strText;

	while(*p!='\0')
	{
		if(*p=='\n')
			nLineCount++;
		
		p++;
	}

	pPg=new SPaintPage(WND_W,WND_H*2+nLineCount*(nFontSize+nSpace));

	pPg->FillRect(0,0,WND_W,WND_H*2+nLineCount*(nFontSize+nSpace),0x000000);
	pPg->SetBkMode(TRANSPARENT);//设置字体透明
	pPg->SetFont(strFont,nFontSize,SPaintPage::FE_BOLD,false,false,false);
	pPg->SetTextColor(RGB(255,0,255));

	i=0;
	p=strText;
	for(;i<nLineCount;i++)
	{
		str="";
		while(*p!='\n' && *p!='\0')
		{
			str+=*p;
			p++;
		}
		if(*p!='\0')
			p++;
		x=(WND_W-str.length()*nFontSize/2)/2;
		pPg->TextOut(x,WND_H+i*(nFontSize+nSpace),str.c_str());
	}

	return pPg;
}


//=========================================================
//函数名称：HollowWords
//说明：空心字
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-29
//修改记录：
//=========================================================
void HollowWords(SPaintPage &Pg,const char *strText,const char *strFont,int nSize,bool bItalic,int x,int y,COLORREF crFront,COLORREF crBack)
{
	Pg.SetFont(strFont,nSize,SPaintPage::FE_NORMAL,bItalic,false,false);

	Pg.SetBkMode(TRANSPARENT);
	Pg.SetTextColor(crBack);

	Pg.TextOut(x-1,y-1,strText);
	Pg.TextOut(x-1,y,strText);
	Pg.TextOut(x,y-1,strText);
	Pg.TextOut(x+1,y,strText);
	Pg.TextOut(x,y+1,strText);
	Pg.TextOut(x+1,y+1,strText);

	Pg.SetTextColor(crFront);

	Pg.TextOut(x,y,strText);
}

//=========================================================
//函数名称：ArtBack
//说明：艺术清屏
//输入：
//输出：
//返回：
//备注：
//日期：2012-12-15
//修改记录：
//=========================================================
void ArtBack(SPaintPage &Page,COLORREF cr,bool bUpright)
{
	int x,y;

	x=0;
	y=0;

	if(!bUpright)
		for(;x<=800;x+=10)
		{
			Page.FillRect(0,0,x,600,cr);
			Page.CopyToDC(g_hMainWndDC);
			Sleep(10);
		}
	else
		for(;y<=800;y+=10)
		{
			Page.FillRect(0,0,800,y,cr);
			Page.CopyToDC(g_hMainWndDC);
			Sleep(10);
		}
}
