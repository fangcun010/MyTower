//===============================================================================
//������SPaintPage(�����ļ�)
//���ߣ��ν�
//===============================================================================
#ifndef _S_PAINTPAGE_H_
#define _S_PAINTPAGE_H_

#include <stdio.h>
#include <windows.h>

struct SBMP
{
	HBITMAP hBmp;
	int w,h;
};

SBMP LoadBmp(const char *strFileName);
void DeleteBmp(SBMP SBmp);

class SPaintPage
{
//����
private:
	int m_nPgW,m_nPgH;
public:
	HPEN m_hPen;
	HBRUSH m_hBrush;
	HFONT m_hFont;
	HBITMAP m_hBmp;//ҳ��洢λͼ
	HDC m_hDC;//ҳ�����DC
	enum FontEffect{FE_THIN,FE_NORMAL,FE_BOLD};//����Ч�������壬����������
//�ӳ�ʽ
public:
	SPaintPage();//���캯��
	SPaintPage(int w,int h);
	~SPaintPage();
	//���ú���
	void SetPage(int w,int h);
	void SetPen(int nStyle,int nWidth,COLORREF cr);
	void SetBrush(unsigned int nStyle,COLORREF cr,long Hatch);
	void SetFont(const char *strFontName,int nSize,FontEffect fe,
					bool bItalic,bool bUnderline,bool bStrikeout);
	void SetTextColor(COLORREF cr);
	void SetBkColor(COLORREF cr);
	void SetBkMode(int nMode);
	//��ͼ����
	void TextOut(int x,int y,const char *str);
	void SetPixel(int x,int y,COLORREF cr);
	void FillRect(int x,int y,int w,int h,COLORREF cr);
	void MoveTo(int x,int y);
	void LineTo(int x,int y);
	void Rectangle(int x,int y,int w,int h);
	void ShowPic(SBMP SBmp,int dx,int dy,int x,int y,int w,int h,COLORREF crTransparent,bool bTransparent);
	//���ƺ���
	void CopyToDC(HDC hDC);
	void CopyToDC(HDC hDC,int x,int y);
	void CopyToDC(HDC hDC,int nDestX,int nDestY,int nWidth,int nHeight,int nSrcX,int nSrcY);
	void CopyPage(SPaintPage &Pg);
	void CopyPage(SPaintPage &Pg,int x,int y);
	void CopyPage(SPaintPage &Pg,int nDestX,int nDestY,int nWidth,int nHeight,int nSrcX,int nSrcY);
	//��ȡ����
	HDC GetDC();
	int GetWidth();
	int GetHeight();
	COLORREF GetPixel(int x,int y);
};

#endif