#pragma once
#include "afxdialogex.h"


// CDlgBitPlane 对话框

class CDlgBitPlane : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBitPlane)

public:
	CDlgBitPlane(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgBitPlane();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgBitPlane };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_bit;
	int flag;

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针

	unsigned char* pBmpBuf;
	unsigned char* lpImgPlane;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
