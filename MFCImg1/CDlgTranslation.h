#pragma once
#include "afxdialogex.h"


// CDlgTranslation 对话框

class CDlgTranslation : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTranslation)

public:
	CDlgTranslation(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgTranslation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgTranslation };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_xoffset;//x偏移量
	int m_yoffset;//y偏移量
	int flag;//修改位

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针

	unsigned char* pBmpBuf;		//指向对话框中副本像素的指针
	unsigned char* lpImg;	//指向对话框中副本图像的指针

	CSliderCtrl m_sliderCtrl1;
	CSliderCtrl m_sliderCtrl2;
//	afx_msg void OnNMCustomdrawSliderX(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusXOffset();
	afx_msg void OnEnKillfocusYOffset();
	afx_msg void OnBnClickedPrew();
	afx_msg void OnBnClickedRecover();
	afx_msg void OnBnClickedOk();
};
