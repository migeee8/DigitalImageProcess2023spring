#pragma once
#include "afxdialogex.h"


// CDlgHisStretch 对话框

class CDlgHisStretch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHisStretch)

public:
	//鼠标拖动状态，0为未拖动，1为拖动第一点，2为第二点
	int m_iIsDraging;

	//相应鼠标事件的区域
	CRect m_MouseRect;

	//标识是否已经绘制橡皮线
	BOOL m_bDrawed;

	//保存鼠标左键单击位置
	CPoint m_p1;

	//保存鼠标拖动的位置
	CPoint m_p2;

	CDlgHisStretch(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgHisStretch();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgHisStretch };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_x1;
	int m_y1;
	int m_x2;
	int m_y2;
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
	afx_msg void OnEnKillfocusEditx1();
	afx_msg void OnEnKillfocusEditx2();
	afx_msg void OnEnKillfocusEdity1();
	afx_msg void OnEnKillfocusEdity2();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedPrevbt();
	afx_msg void OnBnClickedRstbt();
	afx_msg void OnBnClickedOk();

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
