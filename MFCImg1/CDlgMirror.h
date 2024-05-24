#pragma once
#include "afxdialogex.h"


// CDlgMirror 对话框

class CDlgMirror : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMirror)

public:
	CDlgMirror(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgMirror();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgMirror1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_dire;//镜像方向，0为水平，1为垂直

	int flag;//修改位

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* pBmpBuf;		//指向对话框中副本像素的指针
	unsigned char* lpImg;	//指向对话框中副本图像的指针

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRecover();
	afx_msg void OnBnClickedOk();
};
