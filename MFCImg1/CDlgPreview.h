#pragma once
#include "afxdialogex.h"


// CDlgPreview 对话框

class CDlgPreview : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPreview)

public:
	CDlgPreview(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgPreview();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PREVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	unsigned char*	lpSrcDib;		//指向被复制图象的指针	
	unsigned char*	lpSrcStartBits;	//指向被复制像素的指针

	unsigned char* pBmpBuf;		//指向对话框中副本像素的指针
	unsigned char* lpImg;	//指向对话框中副本图像的指针
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
