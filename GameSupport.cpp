//�ļ����ƣ�GameSupport
//���ߣ�����
//˵������Ϸ֧�ֿ�
//���ڣ�2012-12-14

#include "GameSupport.h"

//����

//ȫ�ֱ���



//=========================================================
//��Ƶ֧��
//=========================================================


//=========================================================
//�������ƣ�LoadMusic
//˵�������������ļ�
//���룺strFileName:�����ļ���
//�����
//���أ��豸ID
//��ע��
//���ڣ�2011-12-14
//�޸ļ�¼��
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
		MessageBox(NULL,"��������ʧ�ܣ�","GameSupport",MB_OK);
		return 0;
	}

	return mciOpen.wDeviceID;
 }

//=========================================================
//�������ƣ�PlayMusic
//˵������������
//���룺bRepeat:�Ƿ�ѭ������
//�����
//���أ�
//��ע��
//���ڣ�2011-12-14
//�޸ļ�¼��
//=========================================================
void PlayMusic(UINT wDeviceID,bool bRepeat)
{
	MCI_SEEK_PARMS mciSeek;
	MCI_PLAY_PARMS mciPlay;
	MCIERROR mciError;

	mciError=mciSendCommand(wDeviceID,MCI_SEEK,MCI_SEEK_TO_START,(DWORD)&mciSeek);
	if(mciError)
	{
		MessageBox(NULL,"���ֲ���ʧ�ܣ�","GameSupport",MB_OK);
	}

	if(bRepeat)
		mciError=mciSendCommand(wDeviceID,MCI_PLAY,MCI_DGV_PLAY_REPEAT,(DWORD)&mciPlay);
	else
		mciError=mciSendCommand(wDeviceID,MCI_PLAY,0,(DWORD)&mciPlay);
	if(mciError)
	{
		MessageBox(NULL,"���ֲ���ʧ�ܣ�","GameSupport",MB_OK);
	}
}

//=========================================================
//�������ƣ�CloseMusic
//˵�����ر�����
//���룺wDeviceID:Ҫ�رյ��豸ID
//�����
//���أ�
//��ע��
//���ڣ�2011-12-14
//�޸ļ�¼��
//=========================================================
void CloseMusic(UINT wDeviceID)
{
	MCI_CLOSE_PARMS mciClose;
	MCIERROR mciError;

	mciError=mciSendCommand(wDeviceID,MCI_CLOSE,MCI_NOTIFY,(DWORD)&mciClose);
}

//=========================================================
//�������ƣ�SetDisplay
//˵����������ʾ���ֱ���
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-12-14
//�޸ļ�¼��
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
//�������ƣ�GetFileDir
//˵�������ļ�·������ļ�����Ŀ¼
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-12-14
//�޸ļ�¼��
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
