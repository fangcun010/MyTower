//文件名称：GameSupport
//作者：方寸
//说明：游戏支持库
//日期：2012-12-14

#include "GameSupport.h"

//常量

//全局变量



//=========================================================
//音频支持
//=========================================================


//=========================================================
//函数名称：LoadMusic
//说明：载入音乐文件
//输入：strFileName:音乐文件名
//输出：
//返回：设备ID
//备注：
//日期：2011-12-14
//修改记录：
//=========================================================
 UINT LoadMusic(const char *strFileName)
 {
	MCI_OPEN_PARMS mciOpen;
	MCIERROR mciError;

	mciOpen.lpstrDeviceType="mpegvideo";
	mciOpen.lpstrElementName=strFileName;

	mciError=mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)&mciOpen);

	if(mciError)
	{
		MessageBox(NULL,"音乐载入失败！","GameSupport",MB_OK);
		return 0;
	}

	return mciOpen.wDeviceID;
 }

//=========================================================
//函数名称：PlayMusic
//说明：播放音乐
//输入：bRepeat:是否循环播放
//输出：
//返回：
//备注：
//日期：2011-12-14
//修改记录：
//=========================================================
void PlayMusic(UINT wDeviceID,bool bRepeat)
{
	MCI_SEEK_PARMS mciSeek;
	MCI_PLAY_PARMS mciPlay;
	MCIERROR mciError;

	mciError=mciSendCommand(wDeviceID,MCI_SEEK,MCI_SEEK_TO_START,(DWORD)&mciSeek);
	if(mciError)
	{
		MessageBox(NULL,"音乐播放失败！","GameSupport",MB_OK);
	}

	if(bRepeat)
		mciError=mciSendCommand(wDeviceID,MCI_PLAY,MCI_DGV_PLAY_REPEAT,(DWORD)&mciPlay);
	else
		mciError=mciSendCommand(wDeviceID,MCI_PLAY,0,(DWORD)&mciPlay);
	if(mciError)
	{
		MessageBox(NULL,"音乐播放失败！","GameSupport",MB_OK);
	}
}

//=========================================================
//函数名称：CloseMusic
//说明：关闭音乐
//输入：wDeviceID:要关闭的设备ID
//输出：
//返回：
//备注：
//日期：2011-12-14
//修改记录：
//=========================================================
void CloseMusic(UINT wDeviceID)
{
	MCI_CLOSE_PARMS mciClose;
	MCIERROR mciError;

	mciError=mciSendCommand(wDeviceID,MCI_CLOSE,MCI_NOTIFY,(DWORD)&mciClose);
}

//=========================================================
//函数名称：SetDisplay
//说明：设置显示器分辨率
//输入：
//输出：
//返回：
//备注：
//日期：2011-12-14
//修改记录：
//=========================================================
void SetDisplay(int w,int h)
{
	DEVMODE dm;

	dm.dmPelsWidth=w;
	dm.dmPelsHeight=h;
	dm.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT;

	ChangeDisplaySettings(&dm,CDS_FULLSCREEN);
}

//=========================================================
//函数名称：GetFileDir
//说明：由文件路径获得文件所在目录
//输入：
//输出：
//返回：
//备注：
//日期：2011-12-14
//修改记录：
//=========================================================
void GetFileDir(const char *strFileName,char *strDirName)
{
	char strFilePath[MAX_PATH];
	char *p=NULL;

	strcpy(strFilePath,strFileName);
	p=strFilePath+strlen(strFileName);

	while(*p!='\\')
		p--;
	*p='\0';
	
	strcpy(strDirName,strFilePath);
}
