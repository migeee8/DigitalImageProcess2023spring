#pragma once
#include "afxdialogex.h"


// CDlgStylize 对话框

class CDlgStylize : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStylize)

public:
	CDlgStylize(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgStylize();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgStylize };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int flag;//修改位

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* pBmpBuf;		//指向对话框中副本像素的指针
	unsigned char* lpImg;	//指向对话框中副本图像的指针

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedPrew();
	afx_msg void OnBnClickedRecover();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	int m_choice;
};
