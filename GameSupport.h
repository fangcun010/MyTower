//文件名称：GameSupport
//作者：方寸
//说明：游戏支持库
//日期：2012-12-14
#ifndef _S_GAMESUPPORT_H_
#define _S_GAMESUPPORT_H_

#include <windows.h>
#include <Digitalv.h>
//常量

//全局变量

//子程式声明
//=========================================================
//音频支持
//=========================================================
UINT LoadMusic(const char *strFileName);//载入音乐文件
void PlayMusic(UINT wDeviceID,bool bRepeat);//播放音乐
void CloseMusic(UINT wDeviceID);//关闭音乐

void SetDisplay(int w,int h);//设置显示器分辨率
void GetFileDir(const char *strFileName,char *strDirName);//由文件路径获得文件所在目录

#endif
