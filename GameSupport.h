//�ļ����ƣ�GameSupport
//���ߣ�����
//˵������Ϸ֧�ֿ�
//���ڣ�2012-12-14
#ifndef _S_GAMESUPPORT_H_
#define _S_GAMESUPPORT_H_

#include <windows.h>
#include <Digitalv.h>
//����

//ȫ�ֱ���

//�ӳ�ʽ����
//=========================================================
//��Ƶ֧��
//=========================================================
UINT LoadMusic(const char *strFileName);//���������ļ�
void PlayMusic(UINT wDeviceID,bool bRepeat);//��������
void CloseMusic(UINT wDeviceID);//�ر�����

void SetDisplay(int w,int h);//������ʾ���ֱ���
void GetFileDir(const char *strFileName,char *strDirName);//���ļ�·������ļ�����Ŀ¼

#endif
