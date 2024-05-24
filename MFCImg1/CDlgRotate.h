#pragma once
#include "afxdialogex.h"


// CDlgRotate 对话框

class CDlgRotate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRotate)

public:
	CDlgRotate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgRotate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgRotate };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_angle;//旋转角度
	HDIB hNewDIB;
	int flag;//修改位

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPrew();
	afx_msg void OnBnClickedOk();
};
