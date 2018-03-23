//�ļ����ƣ�Game
//���ߣ�����
//˵������Ϸʵ��
//���ڣ�2012-12-14
#include "Globals.h"
#include <iostream>

using namespace std;

//=========================================================
//����
//=========================================================
#define FLIPPAGE				g_GamePage.CopyToDC(g_hMainWndDC)
//������ʱ
#define KEY_DELAY				200

//=========================================================
//�ӳ�ʽ����
//=========================================================
void StartLogo();//��ʼLogo����
int GameMenu(int nCho);//��Ϸ���˵�
int GetKey();//��ȡ��Ϸ����
void PlayStart();//��ʼ����
void PlayLeave();//ƮҶ����
void Foreword();//��Ϸ����
void TrueHeart();//������
void ReadMap(int ID);//��ȡ1���ͼ
void ReadMap();//��ȡ��ͼ����
void ShowScene();//��ʾ��������
void DataInit();//���ݳ�ʼ��
int MyRoleMagic();//ħ��ѡ��
int MyRoleGoods();//��Ʒѡ��
void Gut();//��Ϸ����
int EscMenu();
void SaveGame();//�洢��Ϸ
bool ReadGame();//��ȡ��Ϸ
void GameOver();//��Ϸ����
void GameEnd();//��Ϸ����
void Shop(int ID);//�̵�
void GotoFloor(int ID);//¥����ת
void ClearTitle(int x,int y);//��IDΪ0�ĵ�ͼ�����ָ��λ�õĵ�ͼ�飬������
int DriveMap();//���ݽ�ɫ�ƶ�
void DrawFloor(int nFloor);//����һ���ͼ
bool RoleHaveGoods(int ID);//��ɫ�Ƿ��и���ID��Ʒ
void DelRoleGoods(int ID);//ɾ����ɫ��Ʒ
void AddRoleGoods(int ID);//���ӽ�ɫ��Ʒ
void AddRoleMagic(int ID);//���ӽ�ɫħ��
void SetEnemyInfo(int ID,int HP,int MP,int Attack,int Defend,int Money,int Exp);//���õ�����Ϣ
void ArtText(const char *strText,int x,int y,int msDelay,bool bVertical);//������
void SetMapZero(int x,int y);//���õ�ͼ����Ϊ0
void StartFlash();//��ʼ��ͼ����˸
void CloseFlash();//�رյ�ͼ����˸
void AddGoods(const char *strName,const char *strDescript,int Attack,int Defend,int HP,int MP,bool bFight,int ID);//������Ʒ
void AddMagic(const char *strName,const char *strDescript,int HP,int MP,int e,int ID);//����ħ��
void ArtBack(SPaintPage &Page,COLORREF cr,bool bUpright);//��������
VOID CALLBACK FlashTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);//��˸��ʱ��
void Say(const char *strText,const char *strHeadFile,bool bUp);//˵������
void ShowMsg(const char *strMsg);//��ʾ��Ϣ
void PlayMagic(const char *strName,int x,int y,int e,int msDelay);//����ħ������
int Fight(int EnemyID,bool bGut,bool bMustLoss);//��ɫս��
void DrawAllMap(SPaintPage &Pg,bool bMap2,int nFloor);//��ָ��ҳ�����һ���ͼ
void DrawMap(SPaintPage &Pg,bool bMap2,int Data,int x,int y);//��ָ��ҳ��ָ��λ�û��Ƶ�ͼ
void HollowWords(SPaintPage &Pg,const char *strText,const char *strFont,int nSize,bool bItalic,int x,int y,COLORREF crFront,COLORREF crBack);//������
SPaintPage *CreatePageByText(const char *strText,const char *strFont,int nFontSize,int nSpace);//ͨ���ı���������ҳ��

//=========================================================
//�������ƣ�GameControl
//˵������Ϸ�����߳�
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-14
//�޸ļ�¼��
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

	if(nChoose==0)//�µ���Ϸ
	{
		PlayLeave();
		Foreword();
		PlayLeave();
Start:
		DataInit();//��Ϸ���ݳ�ʼ��
		ReadMap();
		StartFlash();
		DrawAllMap(g_MapPage[0],false,0);
   		DrawAllMap(g_MapPage[1],true,0);

		Gut();
		CloseFlash();
		goto LabelGameMenu;
	}
	else if(nChoose==1)//�ɵĻ���
	{
		DataInit();//��Ϸ���ݳ�ʼ��
		bRet=ReadGame();
		if(bRet)//��ȡ��Ϸ�ɹ�
		{
			StartFlash();
			Gut();
			CloseFlash();
			goto LabelGameMenu;
		}
		else//��ȡ��Ϸʧ��
		{
			goto LabelGameMenu;
		}
	}
	if(nChoose==2)//������
	{
		PlayLeave();
		TrueHeart();
		PlayLeave();
		goto LabelGameMenu;
	}
	else if(nChoose==3)//�˳���Ϸ
		SendMessage(g_hMainWnd,WM_CLOSE,NULL,NULL);

	return 0;
}

//=========================================================
//�������ƣ�Gut
//˵��������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
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

		if(ID==46 && g_RoleInfo.Floor==0)//�ڵ�0����������
		{
			Say("ų�򣺰����������������ҵ����ˣ�����","ų��",false);
			Say("�����������ų�������𣡣�����������ܺã�����","����",true);
			Say("ų�򣺰���������������ģ�����֤���Ҳ���ų�򣡣���","ų��",false);
			Say("��������ţ����ҿɺ��ˣ����������ȣ�����","����",true);
			Say("ų������˵�Ǹ�ħ��������","ų��",false);
			Say("��ţ����������ȥ���ˣ�","��ţ",true);
			Say("��������ţ������أ�����ų�������ɣ�����","����",true);
			Say("ų�򣺰�������һ�����͵Ļ���ɣ�����","ų��",false);
			SetMapZero(8,15);
			strFile=g_strSoundDir;
			strFile+="����.mp3";
			MusicID=LoadMusic(strFile.c_str());
			PlayMusic(MusicID,false);
			ArtBack(g_GamePage,0,true);
			g_TempPage.CopyPage(g_GamePage);
			ArtText("������һ����ϵİ����������ǰ",500,100,300,true);
			ArtText("��û����ϧ",450,100,300,true);
			ArtText("����ʧȥ��ʱ��",400,100,300,true);
			ArtText("�Ҳź��Ī��",350,100,300,true);
			ArtText("��������ʹ�����Ī���ڴ�",300,100,300,true);
			g_GamePage.CopyPage(g_TempPage);

			Sleep(3000);
			CloseMusic(MusicID);

			Say("ų�����Ҹ�����ͣ�����","ų��",false);
		}
		else if(ID==46 && g_RoleInfo.Floor==21)//�ڵ�21���ҵ�����
		{
			if(!g_RoleInfo.Event[10])
			{
				Say("���������������~!","����",true);
				Say("���ߣ�����������ͣ�����","����",false);
				
				strFile=g_strSoundDir;
				strFile+="һ����.mp3";
				MusicID=LoadMusic(strFile.c_str());
				PlayMusic(MusicID,false);
				ArtBack(g_GamePage,0,true);
				g_TempPage.CopyPage(g_GamePage);
				ArtText("������һ����ϵİ����������ǰ",500,100,300,true);
				ArtText("��û����ϧ",450,100,300,true);
				ArtText("����ʧȥ��ʱ��",400,100,300,true);
				ArtText("�Ҳź��Ī��",350,100,300,true);
				ArtText("������������ʹ�����Ī���ڴ�",300,100,300,true);
				ArtText("�����������һ��",250,100,300,true);
				ArtText("�һ����˵",200,100,300,true);
				ArtText("�Ұ���",150,100,300,true);
				ArtText("һ����",100,100,300,true);
				g_GamePage.CopyPage(g_TempPage);
				
				Sleep(10000);
				CloseMusic(MusicID);
				
				Say("��������磡~ ���Ǻ�����������������","����2",true);
				Say("���ߣ����һ�������Լ�����˶��������ˣ����Ż���ʲô��˼������","����",false);
				Say("��������磡~ ����ˣ�����","����2",true);
				Say("���ߣ��ǵģ���Щ���������˺ܶ࣡���ھ����������е�ɱ��ħ����","����",false);

				g_RoleInfo.Event[10]=true;
			}
			else
			{
				Say("������ȥ�ɣ���֧���㣡~ ","����2",true);
			}
		}
		else if(ID==50 && RoleHaveGoods(28-14) && RoleHaveGoods(29-14) && RoleHaveGoods(30-14))
		{
			Say("ų���ǲ�����Щ��","ų��",false);
			Say("���ӣ�������Щ��лл�㣡","����",true);
			ShowMsg("��������");
			g_RoleInfo.Attack+=50;
			g_RoleInfo.Defend+=50;
			DelRoleGoods(28-14);
			DelRoleGoods(29-14);
			DelRoleGoods(30-14);
			g_RoleInfo.Event[20]=true;
		}
		else if(ID==50 && !g_RoleInfo.Event[20])
		{
			Say("���ӣ��ҵķ�����ף����ʧ�ˣ�������ܰ����һ������Ҿ��ܸ���һЩ��������","����",true);
			Say("ų����ȥ���ң�","ų��",false);
		}
		else if(ID==50 && g_RoleInfo.Event[20])
		{
			Say("���ӣ�ȥ������ʹ���ɣ�","����",true);
		}
		else if(ID==48 && g_RoleInfo.Floor==1)//1¥���ʵ�����
		{
			Say("�ʵ����ˣ������Ǻܿ��µģ���Ҫ��������ǡ�����������ͣ��������� ESC�������Լ�����Ʒ����û�к�Ѫƿ�����á�","�ʵ�����",true);
		}
		else if(ID==48 && g_RoleInfo.Floor==2 && g_RoleInfo.Event[0]==false)//2¥���ʵ�����
		{
			Say("�ʵ����ˣ���¥���ֿɲ���ô�����ˣ��ٺ٣����������𣿺ߺߣ�����35����ң��Ҿͽ���һ�С�","�ʵ�����",true);
			if(g_RoleInfo.Money>=35)
			{
				AddRoleMagic(MAGIC_STORM);
				ShowMsg("ѧ�᣺�籩");
				g_RoleInfo.Money-=35;
				g_RoleInfo.Event[0]=true;
			}
			else
			{
				Say("ų���ҵĽ�һ��������ɲ�������Ƿ�š�����","ų��",false);
				Say("�ʵ����ˣ��������Ϥ�������̤Τ��","�ʵ�����",true);
			}
		}
		else if(ID==48 && g_RoleInfo.Floor==2 && g_RoleInfo.Event[0]==true)//2¥���ʵ�����
		{
			Say("�ʵ����ˣ������������л��аɣ��ٺ٣�","�ʵ�����",true);
		}
		else if(ID==48 && g_RoleInfo.Floor==3)//3¥���ʵ�����
		{
			Say("�ʵ����ˣ�û�������ң�����100���飬��������1����","�ʵ�����",true);
			if(g_RoleInfo.Experience<100)
			{
				ShowMsg("���鲻��");
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
				ShowMsg("��ɫ����");
				if(g_RoleInfo.Level==5)
				{
					ShowMsg("ѧ�� �콵��ʯ");
					AddRoleMagic(MAGIC_STONEFALL);
				}
				else if(g_RoleInfo.Level==10)
				{
					ShowMsg("ѧ�� ����");
					AddRoleMagic(MAGIC_AURORA);
				}
				else if(g_RoleInfo.Level==15)
				{
					ShowMsg("ѧ�� ����ٻ�");
					AddRoleMagic(MAGIC_SOULOUT);
				}
				else if(g_RoleInfo.Level==20)
				{
					ShowMsg("ѧ�� ��������");
					AddRoleMagic(MAGIC_THUNDER);
				}
			}
		}
		else if(ID==43 && g_RoleInfo.Floor==2)//2¥���̵�
		{
			Say("���ˣ�����25����ң��ҿ�����ǿ���������","����",true);
			if(g_RoleInfo.Money<25)
			{
				ShowMsg("��Ҳ���");
			}
			else
			{
				Shop(0);
			}
		}
		else if(ID==49 && g_RoleInfo.Floor==5)//5¥�����Ƿ�ʦ
		{
			if(!g_RoleInfo.Event[1])
			{
				Say("���Ƿ�ʦ���ұ���������������ħ��Ҳ�����ˣ������������ҵ��ź����Ҿͽ��㼸��������������ɣ�","���Ƿ�ʦ",true);
				ShowMsg("ѧ�᣺�ָ���");
				AddRoleMagic(MAGIC_RECOVER);
				ShowMsg("ѧ�᣺��������");
				AddRoleMagic(MAGIC_ZONEFIRE);
				ShowMsg("ѧ�᣺�һ�");
				AddRoleMagic(MAGIC_HEATFIRE);
				Say("ų���һ���������Ը�ģ�����","ų��",false);
				g_RoleInfo.Event[1]=true;
				ClearTitle(1,16);
			}
		}
		else if(ID==47 && g_RoleInfo.Floor==8)//8¥������
		{
			if(!g_RoleInfo.Event[2])
			{
				Say("���ʣ�лл�����ң���ʿ������","����", true);
				Say("ų����ʿ��������˵���𣿣�������ֻ��һ��ų��ɣ���~ ","ų��",false);
				Say("���ʣ�û�������������������¸ҵģ���������Ҫ�ػ����ܵ��˺�ʱ��������������¸ң�����","����", true);
				Say("ų��лл�㣡���Һ���������ʲô��~ ","ų��",false);
				SetMapZero(14,1);
				g_RoleInfo.Event[2]=true;
			}
		}
		else if(ID==43 && g_RoleInfo.Floor==9)//9¥���̵�
		{
			Say("���ˣ�����50����ң��ҿ�����ǿ���������","����",true);
			if(g_RoleInfo.Money<50)
			{
				ShowMsg("��Ҳ���");
			}
			else
			{
				Shop(1);
			}
		}
		else if(ID==5 && !(g_RoleInfo.Event[11] && g_RoleInfo.Event[12]) && g_RoleInfo.Floor==21)
		{
			Say("���ߣ��ȴ��ħ��������","����",false);
		}
		else if(ID==5 && g_RoleInfo.Event[11] && g_RoleInfo.Event[12] && g_RoleInfo.Floor==21)
		{
			g_RoleInfo.Floor=22;
			DrawAllMap(g_MapPage[0],false,22);
			DrawAllMap(g_MapPage[1],true,22);
			g_RoleInfo.x=8;
			g_RoleInfo.y=11;
		}
		else if(ID==5)//��¥
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
		else if(ID==4)//��¥
		{
			g_RoleInfo.x=g_DownStair[g_RoleInfo.Floor].x;
			g_RoleInfo.y=g_DownStair[g_RoleInfo.Floor].y;
			
			g_RoleInfo.Floor--;
			DrawFloor(g_RoleInfo.Floor);
		}
		else if(ID==80)
		{
			Say("��ţ���������ģ��������Ҵ򣡣���","��ţ",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//ս��ʧ��
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//ս��ʤ��
			{
				SetMapZero(g_x,g_y);
				g_RoleInfo.Event[11]=true;
			}
			else if(nRet==2)//���ܳɹ�
				;
		}
		else if(ID==81)
		{
			Say("��Ѫ���������μ�ţħ���Ļ���ģ�˳���������Ѫ�ɣ�","��Ѫ��",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//ս��ʧ��
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//ս��ʤ��
			{
				SetMapZero(g_x,g_y);
				g_RoleInfo.Event[12]=true;
			}
			else if(nRet==2)//���ܳɹ�
				;
		}
		else if(ID==98)//ţħ������
		{
			Say("ţħ�������Գ������㻹�Ƕ����𣿣�","��ţ",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//ս��ʧ��
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//ս��ʤ��
			{
				ClearTitle(g_x,g_y);
				g_RoleInfo.Event[13]=true;
			}
			else if(nRet==2)//���ܳɹ�
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
		else if(ID==89)//��Ѫ������
		{
			Say("��Ѫ�����Գ������㻹�Ƕ����𣿣�","��ţ",true);
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//ս��ʧ��
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_END;
				return;
			}
			else if(nRet==1)//ս��ʤ��
			{
				ClearTitle(g_x,g_y);
				g_RoleInfo.Event[14]=true;
			}
			else if(nRet==2)//���ܳɹ�
				;
			if(g_RoleInfo.Event[13] && g_RoleInfo.Event[14])
			{
				PlayLeave();
				GameEnd();
				PlayLeave();
				return;
			}
		}
		else if(ID>=51 && ID<=79)//��������
		{
			nRet=Fight(g_Map[g_RoleInfo.Floor][g_x][g_y],false,false);
			if(nRet==0)//ս��ʧ��
			{
				PlayLeave();
				GameOver();
				PlayLeave();
				g_Result=RESULT_FAIL;
				return;
			}
			else if(nRet==1)//ս��ʤ��
				SetMapZero(g_x,g_y);
			else if(nRet==2)//���ܳɹ�
				;
		}
		else if(ID>=14 && ID<=41)
		{
			sprintf(text,"��� %s",g_GoodsList.Info[g_Map[g_RoleInfo.Face][g_x][g_y]].strName);
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
//�������ƣ�RoleHaveGoods
//˵����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-1-12
//�޸ļ�¼��
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
//�������ƣ�GameEnd
//˵����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-1-12
//�޸ļ�¼��
//=========================================================
void GameEnd()
{
	char str[1024];
	SPaintPage *pPg=NULL;
	SBMP Bmp;
	string strFile;
	int y;

	strFile=g_strPicDir;
	strFile+="���뽥���ɰ�.bmp";
	Bmp=LoadBmp(strFile.c_str());

	LoadString(g_hInstance,5,str,1024);
	pPg=CreatePageByText(str,"����",16,5);
	
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
//�������ƣ�GotoFloor
//˵������ת¥��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-1-12
//�޸ļ�¼��
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
	
	//���Ʊ���
	g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
	g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
	
	si=0;

	i=s;

	for(;i<=e;i++,si++)
	{
		sprintf(text,"��%d��",i);
		HollowWords(g_GamePage,text,"����",16,false,x+60,y+3+si*20,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}

	
	//���ƹ��
	g_GamePage.ShowPic(ArrowBmp,x+20,y+3+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);
	
	FLIPPAGE;
	
	
	while(1)
	{
		if(Key)
		{
			//���Ʊ���
			g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
			g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
	
	si=0;

	i=s;

	for(;i<=e;i++,si++)
	{
		sprintf(text,"��%d��",i);
		HollowWords(g_GamePage,text,"����",16,false,x+60,y+3+si*20,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}
			//���ƹ��
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
//�������ƣ�SaveGame
//˵����������Ϸ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2013-1-11
//�޸ļ�¼��
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
		MessageBox(NULL,"�����浵�ļ�ʧ�ܣ�",WND_CAPTION,MB_OK|MB_ICONSTOP);
		return;
	}

	fwrite(&g_RoleInfo,sizeof(RoleInfo),1,fp);
	fwrite(&g_Map,sizeof(g_Map),1,fp);

	ShowMsg("����ɹ���");

	fclose(fp);
}

//=========================================================
//�������ƣ�ReadGame
//˵������ȡ��Ϸ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2013-1-11
//�޸ļ�¼��
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
		ShowMsg("��δ�浵��");
		return false;
	}

	fread(&g_RoleInfo,sizeof(RoleInfo),1,fp);
	fread(&g_Map,sizeof(g_Map),1,fp);

	ShowMsg("��ȡ�ɹ���");

	//����¥���ͼ
	DrawAllMap(g_MapPage[0],false,g_RoleInfo.Floor);
	DrawAllMap(g_MapPage[1],true,g_RoleInfo.Floor);

	fclose(fp);

	return true;
}

//=========================================================
//�������ƣ�MyRoleMagic
//˵����ѡ��һ��ħ��
//���룺
//�����
//���أ�ѡ���ħ��������-1Ϊδѡ��
//��ע��
//���ڣ�2013-1-1
//�޸ļ�¼��
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
		ShowMsg("û��ħ��");
		return -1;
	}

	ChoMax=g_RoleInfo.Magic.Count-1;//����ѡ��

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

	pstr=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript;//�ı�������Ϣ
	//��ʾ������Ϣ
	PageCount=strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)/(24*3);
	if(strlen(g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript)%(32*3))
		PageCount++;

	i=0;
	
	while(1)
	{
		g_GamePage.ShowPic(BackBmp,x,y,0,0,w,h,RGB(255,0,255),true);//��ʾ����
		strName1=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho-Chox]].strName;
		
		//��ʾ�ı�
		HollowWords(g_GamePage,strName1.c_str(),"����",16,false,x+80,y+10,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		
		if(ChoMax>=1)
		{
			strName2=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho-Chox+1]].strName;
			//��ʾ�ı�
			HollowWords(g_GamePage,strName2.c_str(),"����",16,false,x+80,y+26,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		}
	
		pstr=&g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript[i*24*3];
		while(*pstr)
		{
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
		}
		
		
		g_GamePage.ShowPic(ArrowBmp,x+40,y+10+Chox*16-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);//��ʾ��ͷ

		FLIPPAGE;

		Key=GetKey();

		if(Key==VK_UP)
		{
			Chox=0;
			Cho--;
			if(Cho<0) Cho=0;
			i=0;
			pstr=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript;//�ı�������Ϣ
			//��ʾ������Ϣ
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
			pstr=g_MagicList.Info[g_RoleInfo.Magic.ID[Cho]].strDescript;//�ı�������Ϣ
			//��ʾ������Ϣ
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
//�������ƣ�MyRoleGoods
//˵����ѡ��һ����Ʒ
//���룺
//�����
//���أ�ѡ�����Ʒ������-1Ϊδѡ��
//��ע��
//���ڣ�2013-1-1
//�޸ļ�¼��
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
		ShowMsg("û����Ʒ");
		return -1;
	}

	ChoMax=g_RoleInfo.Goods.Count-1;//����ѡ��

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

	pstr=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript;//�ı�������Ϣ
	//��ʾ������Ϣ
	PageCount=strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)/(24*3);
	if(strlen(g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript)%(32*3))
		PageCount++;

	i=0;
	
	while(1)
	{
		g_GamePage.ShowPic(BackBmp,x,y,0,0,w,h,RGB(255,0,255),true);//��ʾ����
		strName1=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho-Chox]].strName;
		strName1+="x";
		sprintf(text,"%d",g_RoleInfo.Goods.GoodsCount[Cho-Chox]);
		strName1+=text;
		
		//��ʾ�ı�
		HollowWords(g_GamePage,strName1.c_str(),"����",16,false,x+80,y+10,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		
		if(ChoMax>=1)
		{
			strName2=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho-Chox+1]].strName;
			strName2+="x";
			sprintf(text,"%d",g_RoleInfo.Goods.GoodsCount[Cho-Chox+1]);
			strName2+=text;

			//��ʾ�ı�
			HollowWords(g_GamePage,strName2.c_str(),"����",16,false,x+80,y+26,COLOR_CHO_FRONT,COLOR_CHO_BACK);
		}

		//��ʾ��ƷͼƬ
		DrawMap(g_GamePage,false,g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].ID+14,x+170,y+30);
	
		pstr=&g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript[i*24*3];
		while(*pstr)
		{
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+70,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+86,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			
			
			if(strlen(pstr)>24)
			{
				memcpy(text,pstr,24);
				text[24]='\0';
				pstr+=24;
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
			else
			{
				strcpy(text,pstr);
				pstr+=strlen(pstr);
				HollowWords(g_GamePage,text,"����",16,false,x+25,y+102,COLOR_CHO_FRONT,COLOR_CHO_BACK);
			}
		}
		
		
		g_GamePage.ShowPic(ArrowBmp,x+40,y+10+Chox*16-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);//��ʾ��ͷ

		FLIPPAGE;

		Key=GetKey();

		if(Key==VK_UP)
		{
			Chox=0;
			Cho--;
			if(Cho<0) Cho=0;
			i=0;
			pstr=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript;//�ı�������Ϣ
			//��ʾ������Ϣ
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
			pstr=g_GoodsList.Info[g_RoleInfo.Goods.ID[Cho]].strDescript;//�ı�������Ϣ
			//��ʾ������Ϣ
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
//�������ƣ�AddRoleMagic
//˵�������ӽ�ɫ��Ʒ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2013-1-1
//�޸ļ�¼��
//=========================================================
void AddRoleMagic(int ID)
{
	g_RoleInfo.Magic.ID[g_RoleInfo.Magic.Count]=ID;
	g_RoleInfo.Magic.Count++;
}

//=========================================================
//�������ƣ�AddRoleGoods
//˵�������ӽ�ɫ��Ʒ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2013-1-1
//�޸ļ�¼��
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
//�������ƣ�DelRoleGoods
//˵����ɾ����ɫ��Ʒ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2013-1-1
//�޸ļ�¼��
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
//�������ƣ�EscMenu
//˵�����˳��˵�
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
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

	//���Ʊ���
	g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
	g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
	HollowWords(g_GamePage,"�ҵ�ħ��","����",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"�ҵ���Ʒ","����",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"������Ϸ","����",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"��½��׷�����ͽ���ף��","����",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	HollowWords(g_GamePage,"�˳���Ϸ","����",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);

	//���ƹ��
	g_GamePage.ShowPic(ArrowBmp,x+20,y+40+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);

	FLIPPAGE;

	
	while(1)
	{
		if(Key)
		{
			//���Ʊ���
			g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
			g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
			HollowWords(g_GamePage,"�ҵ�ħ��","����",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"�ҵ���Ʒ","����",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"������Ϸ","����",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"��½��׷�����ͽ���ף��","����",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			HollowWords(g_GamePage,"�˳���Ϸ","����",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			
			//���ƹ��
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

	if(Cho==2)//������Ϸ
		SaveGame();
	else if(Cho==3)//��½����ף��
		ShellExecute(NULL,"OPEN","http://blog.sina.com.cn/s/blog_b2bbe82101016jum.html",NULL,NULL,SW_SHOW);


	return Cho;
}

//=========================================================
//�������ƣ�DrawFloor
//˵��������һ���ͼ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
//=========================================================
void DrawFloor(int nFloor)
{
	DrawAllMap(g_MapPage[0],false,nFloor);
	DrawAllMap(g_MapPage[1],true,nFloor);
}

//=========================================================
//�������ƣ�ArtText
//˵����������
//���룺bVertical:�Ƿ�Ϊ����
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
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
//�������ƣ�SetMapZero
//˵�������õ�ͼ����Ϊ0
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
//=========================================================
void SetMapZero(int x,int y)
{
	g_Map[g_RoleInfo.Floor][x][y]=0;
	DrawMap(g_MapPage[0],false,0,x*TITLE_W,y*TITLE_H);
	DrawMap(g_MapPage[1],true,0,x*TITLE_W,y*TITLE_H);
}

//=========================================================
//�������ƣ�GameMenu
//˵������Ϸ���˵�
//���룺nCho����ʼѡ��
//�����
//���أ�
//��ע��
//���ڣ�2012-12-15
//�޸ļ�¼��
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
	strFile+="���˵�����.bmp";
	BackBmp=LoadBmp(strFile.c_str());
	strFile=g_strPicDir;
	strFile+="���˵�.bmp";
	MenuBmp=LoadBmp(strFile.c_str());
	strFile=g_strPicDir;
	strFile+="ѡ��.bmp";
	SelectBmp=LoadBmp(strFile.c_str());

	strFile=g_strSoundDir;
	strFile+="����.mp3";
	MusicID=LoadMusic(strFile.c_str());

	//������ɫ����
	g_GamePage.FillRect(0,0,WND_W,WND_H,0x000000);

	//��ʾ���˵�����
	g_GamePage.ShowPic(BackBmp,(WND_W-BackBmp.w)/2,50,0,0,BackBmp.w,BackBmp.h,RGB(255,0,255),true);

	//��ʾ����
	g_GamePage.SetBkMode(TRANSPARENT);
	g_GamePage.SetTextColor(0x001000);
	g_GamePage.SetFont("�����п�",50,SPaintPage::FE_NORMAL,false,false,false);
	g_GamePage.TextOut((WND_W-50*4)/2,100,"�ҵ�ħ��");
	
	//��ʾ�˵�
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
//�������ƣ�Fight
//˵������ɫս��
//���룺bGut:�Ƿ�Ϊ����ս����ս��ʧ����Ϸ�Լ�����
//		bMustLoss:�Ƿ�������˲Ž���ս��
//�����0:ս��ʧ�� 1:ս��ʤ�� 2:���ܳɹ�
//���أ�true:ս��ʤ��
//��ע��
//���ڣ�2012-12-14
//�޸ļ�¼��
//=========================================================
int Fight(int EnemyID,bool bGut,bool bMustLoss)
{
	EnemyInfo EInfo;//��������
	SBMP FightBackBmp;//ս������
	SBMP FightMenuBmp;//ս���˵�
	SBMP HPMPBmp;//����ħ����
	string strFile;
	UINT AttackMusicID;
	int i;//ѭ������
	int Hurt;//�˺�ֵ
	int x,y;//���ڼ��������ʾλ��
	int px,py;
	int pw,ph;
	int ix,iy;
	int ex,ey;//����λ��
	int xadd,yadd;//x��y��������
	int MenuCho=0;//ս���˵���ǰѡ��
	int Key=0;
	int Step;//��ɫ����
	char text[256];
	int Ret=0;

	//�������Ը���
	memcpy(&EInfo,&g_EnemyInfo[EnemyID],sizeof(EnemyInfo));
	
	strFile=g_strPicDir;
	strFile+="FightBack.bmp";
	//����ս������
	FightBackBmp=LoadBmp(strFile.c_str());
	
	strFile=g_strPicDir;
	strFile+="FightMenu.bmp";
	//����ս���˵�
	FightMenuBmp=LoadBmp(strFile.c_str());

	strFile=g_strPicDir;
	strFile+="HPMP.bmp";
	//��������ħ����
	HPMPBmp=LoadBmp(strFile.c_str());

	//��������Ч
	strFile=g_strSoundDir;
	strFile+="���.mp3";
	AttackMusicID=LoadMusic(strFile.c_str());
	
	while(1)
	{
		//���˹���
		Hurt=EInfo.Attack-g_RoleInfo.Defend;
		
		if(Hurt<1) Hurt=1;
		//���˳�ʼλ��
		ex=(WND_W-FightBackBmp.w)/2+50;
		ey=(WND_H-FightBackBmp.h)/2+10;
		//��ɫ��ʼλ��
		ix=(WND_W-FightBackBmp.w)/2+110;
		iy=(WND_H-FightBackBmp.h)/2+70;
		xadd=(ix-ex)/10;
		yadd=(iy-ey)/10;
		
		while(ex<ix || ey<iy)
		{
			//����ս������
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightBackBmp.w;
			ph=FightBackBmp.h;
			g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
			g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
			
			//���Ƶ��˽�ɫ
			DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,ex,ey);
			
			//���ƽ�ɫ
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
		//����ս������
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		pw=FightBackBmp.w;
		ph=FightBackBmp.h;
		g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
		g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
		
		//���Ƶ��˽�ɫ
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
		
		//���ƽ�ɫ
		pw=g_RoleBmp.w/3;
		ph=g_RoleBmp.h/4;
		g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
		
		//����ս���˵�
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		pw=FightMenuBmp.w;
		ph=FightMenuBmp.h/4;
		g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
		
		//��������ħ����
		x=(WND_W-FightBackBmp.w)/2;
		y=(WND_H-FightBackBmp.h)/2;
		pw=HPMPBmp.w;
		ph=HPMPBmp.h;
		g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
		//��������ħ����ֵ
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
		
		//�˺�ֵ����
		Hurt=EInfo.Attack-g_RoleInfo.Defend;
		if(Hurt<=0) Hurt=1;
		//�ڽ�ɫ��Ʈ���˺�ֵ
		i=0;
		while(i<5)
		{
			//����ս������
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightBackBmp.w;
			ph=FightBackBmp.h;
			g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
			g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
			
			//���Ƶ��˽�ɫ
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
			
			//���ƽ�ɫ
			pw=g_RoleBmp.w/3;
			ph=g_RoleBmp.h/4;
			g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
			
			//����ս���˵�
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightMenuBmp.w;
			ph=FightMenuBmp.h/4;
			g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
			
			//��������ħ����
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=HPMPBmp.w;
			ph=HPMPBmp.h;
			g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
			//��������ħ����ֵ
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
			
			//�����˺�ֵ
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
		if(g_RoleInfo.HP<=0)//ս��ʧ��
		{
			ShowMsg("ս��ʧ�ܣ�");
			Ret=0;
			goto EndFight;
		}
EndEnemyAttack:

		while(Key!=VK_RETURN)
		{
			if(Key==0)
			{
				//����ս������
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//���Ƶ��˽�ɫ
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
				
				//���ƽ�ɫ
				pw=g_RoleBmp.w/3;
				ph=g_RoleBmp.h/4;
				g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
				
				//����ս���˵�
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightMenuBmp.w;
				ph=FightMenuBmp.h/4;
				g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
				
				//��������ħ����
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=HPMPBmp.w;
				ph=HPMPBmp.h;
				g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
				//��������ħ����ֵ
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
			
			//����ս���˵�
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightMenuBmp.w;
			ph=FightMenuBmp.h/4;
			g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
			
			FLIPPAGE;
		}
		if(MenuCho==FIGHT_ATTACK)//��ͨ����
		{
			//���˳�ʼλ��
			ex=(WND_W-FightBackBmp.w)/2+50;
			ey=(WND_H-FightBackBmp.h)/2+10;
			//��ɫ��ʼλ��
			ix=(WND_W-FightBackBmp.w)/2+110;
			iy=(WND_H-FightBackBmp.h)/2+70;
			xadd=(ix-x)/10;
			yadd=(iy-y)/10;
			Step=0;
			while(ix>ex || iy>ey)
			{
				//����ս������
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//���Ƶ��˽�ɫ
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,ex,ey);
				
				//���ƽ�ɫ
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
			//����ս������
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightBackBmp.w;
			ph=FightBackBmp.h;
			g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
			g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
			
			//���Ƶ��˽�ɫ
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
			
			//���ƽ�ɫ
			pw=g_RoleBmp.w/3;
			ph=g_RoleBmp.h/4;
			g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
			
			//����ս���˵�
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=FightMenuBmp.w;
			ph=FightMenuBmp.h/4;
			g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
			
			//��������ħ����
			x=(WND_W-FightBackBmp.w)/2;
			y=(WND_H-FightBackBmp.h)/2;
			pw=HPMPBmp.w;
			ph=HPMPBmp.h;
			g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
			//��������ħ����ֵ
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
			
			//�˺�ֵ����
			Hurt=g_RoleInfo.Attack-EInfo.Defend;
			if(Hurt<=0) Hurt=1;
			//�ڵ��˽�ɫ��Ʈ���˺�ֵ
			i=0;
			while(i<5)
			{
				//����ս������
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//���Ƶ��˽�ɫ
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
				
				//���ƽ�ɫ
				pw=g_RoleBmp.w/3;
				ph=g_RoleBmp.h/4;
				g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
				
				//����ս���˵�
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightMenuBmp.w;
				ph=FightMenuBmp.h/4;
				g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
				
				//��������ħ����
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=HPMPBmp.w;
				ph=HPMPBmp.h;
				g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
				//��������ħ����ֵ
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

				//�����˺�ֵ
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
			if(EInfo.HP<=0)//ս��ʤ��
			{
				ShowMsg("ս��ʤ����");
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
				ShowMsg("ħ������");
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
				//�ڽ�ɫ��Ʈ���˺�ֵ
				i=0;
				while(i<5)
				{
					//����ս������
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightBackBmp.w;
					ph=FightBackBmp.h;
					g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
					g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
					
					//���Ƶ��˽�ɫ
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
					
					//���ƽ�ɫ
					pw=g_RoleBmp.w/3;
					ph=g_RoleBmp.h/4;
					g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
					
					//����ս���˵�
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightMenuBmp.w;
					ph=FightMenuBmp.h/4;
					g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
					
					//��������ħ����
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=HPMPBmp.w;
					ph=HPMPBmp.h;
					g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
					//��������ħ����ֵ
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
					
					//�����˺�ֵ
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
				//�ڵ��˽�ɫ��Ʈ���˺�ֵ
				i=0;
				while(i<5)
				{
					//����ս������
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightBackBmp.w;
					ph=FightBackBmp.h;
					g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
					g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
					
					//���Ƶ��˽�ɫ
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
					
					//���ƽ�ɫ
					pw=g_RoleBmp.w/3;
					ph=g_RoleBmp.h/4;
					g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
					
					//����ս���˵�
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=FightMenuBmp.w;
					ph=FightMenuBmp.h/4;
					g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
					
					//��������ħ����
					x=(WND_W-FightBackBmp.w)/2;
					y=(WND_H-FightBackBmp.h)/2;
					pw=HPMPBmp.w;
					ph=HPMPBmp.h;
					g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
					//��������ħ����ֵ
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
					
					//�����˺�ֵ
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
				if(EInfo.HP<=0)//ս��ʤ��
				{
					ShowMsg("ս��ʤ����");
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
					PlayMagic("�ָ���",x,y,17,100);
					g_RoleInfo.HP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].HP;
					g_RoleInfo.MP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].MP;
					if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
					if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
					DelRoleGoods(g_RoleInfo.Goods.ID[nRet]);
					HP=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].HP;
					MP=g_GoodsList.Info[g_RoleInfo.Goods.ID[nRet]].MP;
					if(HP>0)
					{
						//�ڽ�ɫ��Ʈ���˺�ֵ
						i=0;
						while(i<5)
						{
							//����ս������
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							pw=FightBackBmp.w;
							ph=FightBackBmp.h;
							g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
							g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
							
							//���Ƶ��˽�ɫ
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
							
							//���ƽ�ɫ
							pw=g_RoleBmp.w/3;
							ph=g_RoleBmp.h/4;
							g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
							
							//����ս���˵�
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							pw=FightMenuBmp.w;
							ph=FightMenuBmp.h/4;
							g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
							
							//��������ħ����
							x=(WND_W-FightBackBmp.w)/2;
							y=(WND_H-FightBackBmp.h)/2;
							pw=HPMPBmp.w;
							ph=HPMPBmp.h;
							g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
							//��������ħ����ֵ
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
							
							//�����˺�ֵ
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
							//�ڽ�ɫ��Ʈ���˺�ֵ
							i=0;
							while(i<5)
							{
								//����ս������
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								pw=FightBackBmp.w;
								ph=FightBackBmp.h;
								g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
								g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
								
								//���Ƶ��˽�ɫ
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
								
								//���ƽ�ɫ
								pw=g_RoleBmp.w/3;
								ph=g_RoleBmp.h/4;
								g_GamePage.ShowPic(g_RoleBmp,x+110,y+70,0,0,pw,ph,RGB(255,0,255),true);
								
								//����ս���˵�
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								pw=FightMenuBmp.w;
								ph=FightMenuBmp.h/4;
								g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
								
								//��������ħ����
								x=(WND_W-FightBackBmp.w)/2;
								y=(WND_H-FightBackBmp.h)/2;
								pw=HPMPBmp.w;
								ph=HPMPBmp.h;
								g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
								//��������ħ����ֵ
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
								
								//�����˺�ֵ
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
					ShowMsg("ս��������");
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
		else if(MenuCho==FIGHT_ESCAPE)//����
		{
			//��ɫ����
			i=0;
			Step=0;
			while(i<5)
			{
				//����ս������
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightBackBmp.w;
				ph=FightBackBmp.h;
				g_GamePage.FillRect(x-1,y-1,pw+2,ph+2,0xFFFFFF);
				g_GamePage.ShowPic(FightBackBmp,x,y,0,0,pw,ph,0,false);
				
				//���Ƶ��˽�ɫ
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				DrawMap(g_GamePage,false,g_EnemyInfo[EnemyID].PicID,x+50,y+10);
				
				//���ƽ�ɫ
				pw=g_RoleBmp.w/3;
				ph=g_RoleBmp.h/4;
				g_GamePage.ShowPic(g_RoleBmp,x+110,y+70+i*5,Step*TITLE_W,0,pw,ph,RGB(255,0,255),true);

				Step++;
				if(Step>2) Step=0;
				
				//����ս���˵�
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=FightMenuBmp.w;
				ph=FightMenuBmp.h/4;
				g_GamePage.ShowPic(FightMenuBmp,x+5,y+90,0,MenuCho*ph,pw,ph,RGB(255,0,255),true);
				
				//��������ħ����
				x=(WND_W-FightBackBmp.w)/2;
				y=(WND_H-FightBackBmp.h)/2;
				pw=HPMPBmp.w;
				ph=HPMPBmp.h;
				g_GamePage.ShowPic(HPMPBmp,x+110,y+110,0,0,pw,ph,0,false);
				//��������ħ����ֵ
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
			if(bGut)//���Ϊ����ģʽ��ֱ������ʧ��
			{
				ShowMsg("����ʧ�ܣ�");
			}
			else//�ٷ�֮��ʮ������ܻ���
			{
				if(rand()%4<1)
				{
					ShowMsg("���ܳɹ���");
					Ret=2;
					goto EndFight;
				}
				else
				{
					ShowMsg("����ʧ�ܣ�");
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
		sprintf(text,"��ý�ң�%d",EInfo.Money);
		ShowMsg(text);
		sprintf(text,"��þ��飺%d",EInfo.Experience);
		ShowMsg(text);
	}

	CloseMusic(AttackMusicID);
	DeleteBmp(FightBackBmp);
	DeleteBmp(FightMenuBmp);
	DeleteBmp(HPMPBmp);

	return Ret;
}

//=========================================================
//�������ƣ�StartFlash
//˵������ʼ��ͼ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
//=========================================================
void StartFlash()
{
	SetTimer(g_hMainWnd,TIMER_FLASH,TIMER_FLASH_DELAY,FlashTimerProc);
}


//=========================================================
//�������ƣ�CloseFlash
//˵�����رյ�ͼ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
//=========================================================
void CloseFlash()
{
	KillTimer(g_hMainWnd,TIMER_FLASH);
}

//=========================================================
//�������ƣ�FlashTimerProc
//˵������ͼ����˸��ʱ��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
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
//�������ƣ�GameOver
//˵������Ϸʧ��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
//=========================================================
void GameOver()
{
	g_TempPage.CopyPage(g_GamePage);
	ArtBack(g_GamePage,0,true);
	ArtText("��",500,100,300,true);
	ArtText("��Ҫ����",450,100,300,true);
	ArtText("��",400,100,300,true);
	ArtText("�α�ִ��",350,100,300,true);
	ArtText("һ����������",300,100,300,true);
	g_GamePage.CopyPage(g_TempPage);
	Sleep(2000);
}

//=========================================================
//�������ƣ�DriveMap
//˵�������ݵ�ͼ
//���룺
//�����
//���أ������������ͼ��ID
//��ע��
//���ڣ�2012-12-30
//�޸ļ�¼��
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
	strFile+="����2.mp3";
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
			if(Ret==0)//�ҵ�ħ��
			{
				Ret=MyRoleMagic();

				if(Ret>=0)
				{
					if(g_MagicList.Info[g_RoleInfo.Magic.ID[Ret]].HP>0)
					{
						ShowMsg("ս��ʹ��");
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
							ShowMsg("ħ������");
						}
					}
				}
			}
			else if(Ret==1)//�ҵ���Ʒ
			{
				Ret=MyRoleGoods();

				if(Ret>=0)
				{
					if(g_RoleInfo.Goods.ID[Ret]==21)//���
					{
						ShowMsg("0�㱦ʯ�Ŵ�");
						DelRoleGoods(21);//ɾ�����
						g_Map[0][3][8]=0;
						FLIPPAGE;
					}
					else if(g_RoleInfo.Goods.ID[Ret]==24)//����Կ��
					{
						if(g_Map[g_RoleInfo.Floor][g_RoleInfo.x][g_RoleInfo.y-1]==10)//�Ƿ��ڼ������Ա�
						{
							ClearTitle(g_RoleInfo.x,g_RoleInfo.y-1);
						}
						DelRoleGoods(24);
					}
					else if(g_RoleInfo.Goods.ID[Ret]==25)//����ͷ
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
						if(g_Map[g_RoleInfo.Floor][tx][ty]==1)//�����ǽ
						{
							ClearTitle(tx,ty);
							DelRoleGoods(25);
						}
						else
						{
							ShowMsg("�����ǽʹ��");
						}
					}
					else if(g_RoleInfo.Goods.ID[Ret]==40-14)//��ʯ��ͷ
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
						if(g_Map[g_RoleInfo.Floor][tx][ty]==9 && g_RoleInfo.Floor==4)//�����¥��ʯ��
						{
							ClearTitle(tx,ty);
							DelRoleGoods(40-14);
						}
						else
						{
							ShowMsg("��Ա�ʯ��ʹ��");
						}
					}
					else if(g_RoleInfo.Goods.ID[Ret]==22-14)//���콣
					{
						ShowMsg("ѧ�� ���콣");
						AddRoleMagic(MAGIC_SWORD);
						g_RoleInfo.HP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].HP;
						g_RoleInfo.MP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].MP;
						if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
						if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
						g_RoleInfo.Attack+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Attack;
						g_RoleInfo.Defend+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Defend;
						DelRoleGoods(g_RoleInfo.Goods.ID[Ret]);
					}
					else if(g_RoleInfo.Goods.ID[Ret]==36-14)//����ʯ
					{
						Say("�ʵ����ˣ�һ���˵��������ޣ����������ޣ�ֻҪ�Ĵ����Ϊ������ս���ض��������ң�","�ʵ�����",true);
						g_RoleInfo.HP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].HP;
						g_RoleInfo.MP+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].MP;
						if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
						if(g_RoleInfo.MP>g_RoleInfo.MPMax) g_RoleInfo.MP=g_RoleInfo.MPMax;
						g_RoleInfo.Attack+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Attack;
						g_RoleInfo.Defend+=g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Defend;
						DelRoleGoods(g_RoleInfo.Goods.ID[Ret]);
					}
					else if(g_RoleInfo.Goods.ID[Ret]==37-14)//��֮����
					{
						Say("���ߣ��о��غ�����Ϸ�����֮���̵����ò������ԡ�������ʵ���е�͵���ˡ���������ǿ���������Ϊ������","����",true);
						g_RoleInfo.HPMax+=100;
						g_RoleInfo.MPMax+=100;
						ShowMsg("��������");
						DelRoleGoods(37-14);
					}
					else if(g_RoleInfo.Goods.ID[Ret]>=31-14 && g_RoleInfo.Goods.ID[Ret]<=33-14)//����
					{
						GotoFloor(g_RoleInfo.Goods.ID[Ret]-31+14);
						Ret=9999;
						goto EndDriveMap;
					}
					else if(g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].bFight==false &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].HP==0 &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].MP==0 &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Attack==0 &&
							g_GoodsList.Info[g_RoleInfo.Goods.ID[Ret]].Defend==0)//������Ʒ
					{
						ShowMsg("������Ʒ");
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
			else if(Ret==4)//�˳���Ϸ
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
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==6)//�ۺ���
		{
			if(g_RoleInfo.PowderKeyCount>0)
			{
				g_RoleInfo.PowderKeyCount--;
				ClearTitle(x,y);
			}
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==7)//��ɫ��
		{
			if(g_RoleInfo.BlueKeyCount>0)
			{
				g_RoleInfo.BlueKeyCount--;
				ClearTitle(x,y);
			}
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==8)//��ɫ��
		{
			if(g_RoleInfo.RedKeyCount>0)
			{
				g_RoleInfo.RedKeyCount--;
				ClearTitle(x,y);
			}
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==11)//�ۺ�Կ��
		{
			ShowMsg("��� �ۺ�Կ��");
			g_RoleInfo.PowderKeyCount++;
			SetMapZero(x,y);
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==12)//��ɫԿ��
		{
			ShowMsg("��� ��ɫԿ��");
			g_RoleInfo.BlueKeyCount++;
			SetMapZero(x,y);
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]==13)//��ɫԿ��
		{
			ShowMsg("��� ��ɫԿ��");
			g_RoleInfo.RedKeyCount++;
			SetMapZero(x,y);
		}
		else if(x>=0 && y>=0 && x<18 && y<18 && g_Map[g_RoleInfo.Floor][x][y]>=14
						&& g_Map[g_RoleInfo.Floor][x][y]<=41)
		{
			sprintf(text,"��� %s",g_GoodsList.Info[g_Map[g_RoleInfo.Floor][x][y]-14].strName);
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
//�������ƣ�Shop
//˵�����̵�
//���룺ID:�̵�ID
//�����
//���أ�
//��ע��
//���ڣ�2012-1-12
//�޸ļ�¼��
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

	//���Ʊ���
	g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
	g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);

	if(ID==0)
	{
		HollowWords(g_GamePage,"����+250","����",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"��������+20","����",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"����+3","����",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"����+3","����",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"�˳�","����",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}
	else if(ID==1)
	{
		HollowWords(g_GamePage,"����+1000","����",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"��������+50","����",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"����+10","����",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"����+10","����",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
		HollowWords(g_GamePage,"�˳�","����",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
	}

	//���ƹ��
	g_GamePage.ShowPic(ArrowBmp,x+20,y+40+Cho*20-3,0,0,ArrowBmp.w,ArrowBmp.h,RGB(255,0,255),true);

	FLIPPAGE;

	
	while(1)
	{
		if(Key)
		{
			//���Ʊ���
			g_GamePage.FillRect(x-1,y-1,BackBmp.w+2,BackBmp.h+2,0xFFFFFF);
			g_GamePage.ShowPic(BackBmp,x,y,0,0,BackBmp.w,BackBmp.h,0,false);
			if(ID==0)
			{
				HollowWords(g_GamePage,"����+250","����",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"��������+20","����",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"����+3","����",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"����+3","����",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"�˳�","����",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			}
			else if(ID==1)
			{
				HollowWords(g_GamePage,"����+1000","����",16,false,x+60,y+40,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"��������+50","����",16,false,x+60,y+60,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"����+10","����",16,false,x+60,y+80,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"����+10","����",16,false,x+60,y+100,COLOR_ESC_FRONT,COLOR_ESC_BACK);
				HollowWords(g_GamePage,"�˳�","����",16,false,x+60,y+120,COLOR_ESC_FRONT,COLOR_ESC_BACK);
			}
			
			//���ƹ��
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
		if(Cho==0)//����+250
		{
			g_RoleInfo.HP+=250;
			if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
		}
		else if(Cho==1)//��������
		{
			g_RoleInfo.HPMax+=20;
		}
		else if(Cho==2)//����+3
		{
			g_RoleInfo.Attack+=3;
		}
		else if(Cho==3)//����+3
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
		if(Cho==0)//����+1000
		{
			g_RoleInfo.HP+=1000;
			if(g_RoleInfo.HP>g_RoleInfo.HPMax) g_RoleInfo.HP=g_RoleInfo.HPMax;
		}
		else if(Cho==1)//��������
		{
			g_RoleInfo.HPMax+=50;
		}
		else if(Cho==2)//����+10
		{
			g_RoleInfo.Attack+=10;
		}
		else if(Cho==3)//����+10
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
//�������ƣ�AddGoods
//˵����������Ʒ��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-30
//�޸ļ�¼��
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
//�������ƣ�AddMagic
//˵��������ħ����
//���룺
//�����
//���أ�
//��ע��HPΪ������ɫ������
//���ڣ�2012-12-31
//�޸ļ�¼��
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
//�������ƣ�DataInit
//˵�������ݳ�ʼ����ħ������Ʒ��Ҫ�������¼
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-30
//�޸ļ�¼��
//=========================================================
void DataInit()
{
	//��¥������
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

	//��¥������
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


	//ħ��
	memset(&g_MagicList,0,sizeof(g_MagicList));//��0ħ����
	AddMagic("�ָ���","�ָ�����150",-150,25,17,MAGIC_RECOVER);//�ָ���
	AddMagic("�籩","�ٻ��籩��������",20,20,8,MAGIC_STORM);//�籩
	AddMagic("�һ�","�ٻ��һ𹥻�����",70,30,15,MAGIC_HEATFIRE);//�һ�
	AddMagic("�콵��ʯ","��������ľ�ʯ�����ж���أ�",80,40,5,MAGIC_STONEFALL);//�콵��ʯ
	AddMagic("����","���صĹ�â",85,40,6,MAGIC_AURORA);//����
	AddMagic("��������","�ӵ����ٻ��������һ�",100,40,7,MAGIC_ZONEFIRE);//��������
	AddMagic("����ٻ�","�ӵ������ٻ�������",150,50,9,MAGIC_SOULOUT);//����ٻ�
	AddMagic("��������","����֮���ʹ���ħ",200,60,7,MAGIC_THUNDER);//��������
	AddMagic("���콣","�ഫ����һ������",250,70,8,MAGIC_SWORD);//���콣

	//��Ʒ
	g_GoodsList.Count=0;
	AddGoods("�챦ʯ","����+1",1,0,0,0,false,0);//�챦ʯ
	AddGoods("����ʯ","����+1",0,1,0,0,false,1);//����ʯ
	AddGoods("��ҩƿ","����+150",0,0,150,0,true,2);//��ҩƿ
	AddGoods("��ҩƿ","ħ��+150",0,0,0,150,true,3);//��ҩƿ
	AddGoods("����","����ͨ��һ�ѽ�������+10",10,0,0,0,false,4);//����
	AddGoods("���ֽ�","���ִ���Ľ�������+20",20,0,0,0,false,5);//���ֽ�
	AddGoods("���߽�","���������������µı���������+30",30,0,0,0,false,6);//���߽�
	AddGoods("նħ��","Ϊնħ������Ľ�������+40",40,0,0,0,false,7);//նħ��
	AddGoods("���콣","�ഫ���ϴ˽����ʹ���ռ��������콣������+50",50,0,0,0,false,8);//���콣
	AddGoods("����","��������Ķ��ƣ�����+10",0,10,0,0,false,9);//����
	AddGoods("���ֶ�","���ִ���Ķ��ƣ�����+20",0,20,0,0,false,10);//���ֶ�
	AddGoods("����","��������һ���ķ���Ч��������+30",0,30,0,0,false,11);//����
	AddGoods("���߶�","�����������������Ķ��ƣ�����+40",0,40,0,0,false,12);//���߶�
	AddGoods("���߶�","�������ߵĶ��ƣ�����+50",0,50,0,0,false,13);//���߶�
	AddGoods("��ɫ�ػ�","������ػ���Ʒ֮һ",0,0,0,0,false,14);//��ɫ�ػ�
	AddGoods("��ɫ�ػ�","������ػ���Ʒ֮һ",0,0,0,0,false,15);//��ɫ�ػ�
	AddGoods("�����ػ�","������ػ���Ʒ֮һ",0,0,0,0,false,16);//�����ػ�
	AddGoods("��������","������ħ��1~7��䴩��",0,0,0,0,false,17);//��������
	AddGoods("��ɫ����","������ħ��8~15��䴩��",0,0,0,0,false,18);//��ɫ����
	AddGoods("��ɫ����","������ħ��16~20��䴩��",0,0,0,0,false,19);//��ɫ����
	AddGoods("ʮ��","����һ�����µ�ף�������Դ����������������+100,����+100",100,100,100,100,false,20);//ʮ��
	AddGoods("���","��˵����ʵ�����Ը��",0,0,0,0,false,21);//���������
	AddGoods("����ʯ","��˵�õ�����ӵ��������",100,100,0,0,false,22);//����ʯ
	AddGoods("��֮����","��֪��ʲô������",0,0,0,0,false,23);//��֮����
	AddGoods("����Կ��","�򿪱����ŵ�Կ��",0,0,0,0,false,24);//����Կ��
	AddGoods("����ͷ","���Դ���Ե���ǽ",0,0,0,0,false,25);//����ͷ
	AddGoods("��ʯ��ͷ","���Դ򿪱���",0,0,0,0,false,26);//��ʯ��ͷ
	AddGoods("ͭ����","���Դ����ǿ�������",100,100,100,100,false,27);//ͭ����

	//��ɫ
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
	//����
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
//�������ƣ�SetEnemyInfo
//˵�������õ�����Ϣ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2013-1-1
//�޸ļ�¼��
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
//�������ƣ�ClearTitle
//˵������IDΪ0�ĵ�ͼ�����ָ��λ�õĵ�ͼ�飬������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-31
//�޸ļ�¼��
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
//�������ƣ�PlayMagic
//˵��������ħ������
//���룺msDelay:ÿ֡�������ʱ��
//�����
//���أ�
//��ע��
//���ڣ�2012-12-30
//�޸ļ�¼��
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
	strFile+="ħ��.mp3";
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
//�������ƣ�Say
//˵����˵������
//���룺strHeadFile:ͷ���ļ�·��
//�����
//���أ�
//��ע��
//���ڣ�2012-12-30
//�޸ļ�¼��
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
	int PageCount;//ҳ��
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

		HollowWords(g_GamePage,text,"����",16,false,x+10,y+5,COLOR_SAY_FRONT,COLOR_SAY_BACK);
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
			HollowWords(g_GamePage,text,"����",16,false,x+10,y+5+16,COLOR_SAY_FRONT,COLOR_SAY_BACK);
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
			HollowWords(g_GamePage,text,"����",16,false,x+10,y+5+32,COLOR_SAY_FRONT,COLOR_SAY_BACK);
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
//�������ƣ�StartLogo
//˵������ʼLogo����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-15
//�޸ļ�¼��
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
	strFile+="Ϊ׷������Logo.bmp";
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
//�������ƣ�ShowMsg
//˵������ʾ��Ϣ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-30
//�޸ļ�¼��
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
	HollowWords(g_GamePage,strMsg,"����",16,false,(WND_W-16*strlen(strMsg))/2+25,(WND_H-16)/2,COLOR_MSG_FRONT,COLOR_MSG_BACK);
	FLIPPAGE;

	while(!GetKey());

	DeleteBmp(Bmp);
}

//=========================================================
//�������ƣ�PlayStart
//˵������ʼ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-29
//�޸ļ�¼��
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
	strFile+="��.wav";

	MusicID=LoadMusic(strFile.c_str());
	PlayMusic(MusicID,false);

	i=1;

	while(i<=76)
	{
		strFile=g_strPicDir;
		itoa(i,str,10);
		strFile+="��ʼ\\";
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
//�������ƣ�PlayLeave
//˵����ƮҶ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-16
//�޸ļ�¼��
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
	strFile+="��.wav";

	MusicID=LoadMusic(strFile.c_str());
	PlayMusic(MusicID,false);

	i=1;

	while(i<=36)
	{
		strFile=g_strPicDir;
		itoa(i,str,10);
		strFile+="ƮҶ\\";
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
//�������ƣ�GetKey
//˵������ȡ��Ϸ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-15
//�޸ļ�¼��
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
//�������ƣ�ReadMap
//˵������ȡһ���ͼ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-1-12
//�޸ļ�¼��
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
		MessageBox(g_hMainWnd,"�����ͼ�ļ�ʧ�ܣ�","�ҵ�ħ��",MB_OK|MB_ICONSTOP);
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
//�������ƣ�ReadMap
//˵������ȡ��ͼ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-24
//�޸ļ�¼��
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
			MessageBox(g_hMainWnd,"�����ͼ�ļ�ʧ�ܣ�","�ҵ�ħ��",MB_OK|MB_ICONSTOP);
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
//�������ƣ�ShowScene
//˵������ʾ��������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-24
//�޸ļ�¼��
//=========================================================
void ShowScene()
{
	int x,y;
	char text[256];
	string str;

	//��ɫ����
	g_GamePage.FillRect(0,0,WND_W,WND_H,0x000000);

	//���Ƶ�ͼ
	g_GamePage.CopyPage(g_MapPage[g_bShowMap2],(WND_W-TITLE_W*TITLE_X_COUNT)/2,
												(WND_H-TITLE_H*TITLE_Y_COUNT)/2,
												TITLE_W*TITLE_X_COUNT,TITLE_H*TITLE_Y_COUNT,0,0);
	//��ʾ��ɫ
	g_GamePage.ShowPic(g_RoleBmp,(WND_W-TITLE_W*TITLE_X_COUNT)/2+g_RoleInfo.x*TITLE_W,(WND_H-TITLE_H*TITLE_Y_COUNT)/2+g_RoleInfo.y*TITLE_H,
						g_RoleInfo.Step*TITLE_W,g_RoleInfo.Face*TITLE_H,TITLE_W,TITLE_H,
						RGB(255,0,255),true);


	//��߽�ɫ����
	for(y=0;y<=17;y++)
		for(x=0;x<=2;x++)
		{
			DrawMap(g_GamePage,false,0,(WND_W-TITLE_W*TITLE_X_COUNT)/2-(x+1)*TITLE_W-3,(WND_H-TITLE_H*TITLE_Y_COUNT)/2+y*TITLE_H);
		}
	HollowWords(g_GamePage,"�ȼ�","����",20,false,20,20,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Level,text,10);

	HollowWords(g_GamePage,text,"����",16,true,60,50,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"����","����",16,true,20,80,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.HP,text,10);
	str="";
	str+=text;
	str+='/';
	itoa(g_RoleInfo.HPMax,text,10);
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"����",12,true,20,100,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"ħ��","����",16,true,20,120,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.MP,text,10);
	str="";
	str+=text;
	str+='/';
	itoa(g_RoleInfo.MPMax,text,10);
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"����",12,true,20,140,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"����","����",16,true,20,160,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Attack,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"����",12,true,20,180,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"����","����",16,true,20,200,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Defend,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"����",12,true,20,220,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	HollowWords(g_GamePage,"���","����",16,true,20,240,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Money,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"����",12,true,20,260,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);


	HollowWords(g_GamePage,"����","����",16,true,20,280,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	itoa(g_RoleInfo.Experience,text,10);
	str="";
	str+=text;
	HollowWords(g_GamePage,str.c_str(),"����",12,true,20,300,COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	//����Կ����Ϣ
	DrawMap(g_GamePage,false,11,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3,11*TITLE_H);//��ʾ�ۺ�Կ��
	DrawMap(g_GamePage,false,12,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3,12*TITLE_H);//��ʾ��ɫԿ��
	DrawMap(g_GamePage,false,13,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3,13*TITLE_H);//��ʾ��ɫԿ��
	
	//��ʾ�ۺ�Կ�׸���
	itoa(g_RoleInfo.PowderKeyCount,text,10);
	str="";
	str+=text;
	str+="  ";
	str+="��";
	HollowWords(g_GamePage,str.c_str(),"����",16,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3+TITLE_W+5,11*TITLE_H+7,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	//��ʾ��ɫԿ�׸���
	itoa(g_RoleInfo.BlueKeyCount,text,10);
	str="";
	str+=text;
	str+="  ";
	str+="��";
	HollowWords(g_GamePage,str.c_str(),"����",16,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3+TITLE_W+5,12*TITLE_H+7,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
	//��ʾ��ɫԿ�׸���
	itoa(g_RoleInfo.RedKeyCount,text,10);
	str="";
	str+=text;
	str+="  ";
	str+="��";
	HollowWords(g_GamePage,str.c_str(),"����",16,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-3*TITLE_W-3+TITLE_W+5,13*TITLE_H+7,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);

	//���Ƶڲ���
	itoa(g_RoleInfo.Floor,text,10);
	str="�� ";
	str+=text;
	str+=" ";
	str+="��";
	HollowWords(g_GamePage,str.c_str(),"����",18,false,(WND_W-TITLE_W*TITLE_X_COUNT)/2-4*TITLE_W-3+TITLE_W+10,15*TITLE_H,
					COLOR_PROPERTY_FRONT,COLOR_PROPERTY_BACK);
}

//=========================================================
//�������ƣ�DrawAllMap
//˵������ָ��ҳ�����һ���ͼ
//���룺bMap2:ʹ��Map1���Ƶ�ͼ��
//�����
//���أ�
//��ע��
//���ڣ�2012-12-24
//�޸ļ�¼��
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
//�������ƣ�DrawMap
//˵������ָ��ҳ��ָ��λ�û��Ƶ�ͼ
//���룺bMap2:ʹ��Map1���Ƶ�ͼ��
//�����
//���أ�
//��ע��
//���ڣ�2012-12-24
//�޸ļ�¼��
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
//�������ƣ�Ever
//˵���������ξ���̨�ʣ������С���
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-23
//�޸ļ�¼��
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
	strFile+="���뽥���ɰ�.bmp";
	Bmp=LoadBmp(strFile.c_str());

	LoadString(g_hInstance,1,str,1024);
	pPg=CreatePageByText(str,"����",16,5);
	
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
//�������ƣ���Ϸ����
//˵����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-23
//�޸ļ�¼��
//=========================================================
void Foreword()
{
	char str[1024];
	SPaintPage *pPg=NULL;
	SBMP Bmp;
	string strFile;
	int y;

	strFile=g_strPicDir;
	strFile+="���뽥���ɰ�.bmp";
	Bmp=LoadBmp(strFile.c_str());

	LoadString(g_hInstance,1,str,1024);
	pPg=CreatePageByText(str,"����",16,5);
	
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
//�������ƣ�������
//˵����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-23
//�޸ļ�¼��
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
	strFile+="���뽥���ɰ�.bmp";
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
	pPg=CreatePageByText(text.c_str(),"����",16,5);

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
//�������ƣ�CreatePageByText
//˵����ͨ���ı���������ҳ��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-23
//�޸ļ�¼��
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
	pPg->SetBkMode(TRANSPARENT);//��������͸��
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
//�������ƣ�HollowWords
//˵����������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-29
//�޸ļ�¼��
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
//�������ƣ�ArtBack
//˵������������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2012-12-15
//�޸ļ�¼��
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
