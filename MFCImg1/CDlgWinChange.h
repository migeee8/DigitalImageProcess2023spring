#pragma once
#include "afxdialogex.h"


// CDlgWinChange 对话框

class CDlgWinChange : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWinChange)

public:
	CDlgWinChange(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgWinChange();
	//响应鼠标的区域
	CRect m_MouseRect;
	//标识是否拖动
	BOOL m_bIsDrawing;
	int m_draggedControlID;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgWinChange };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_lowerLimit;//下限
	int m_higherLimit;//上限
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

	afx_msg void OnEnKillfocusHLimit();
	afx_msg void OnEnKillfocusLLimit();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedPrevbt();
	afx_msg void OnBnClickedRstbt();
	afx_msg void OnBnClickedOk();
};
