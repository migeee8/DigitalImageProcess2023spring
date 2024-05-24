#pragma once
#include "afxdialogex.h"


// CDlgShowRes 对话框

class CDlgShowRes : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowRes)

public:
	CDlgShowRes(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgShowRes();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgShowRes };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_count;
	float m_area;
	float m_perimeter;
	float m_circle;
	float m_rectangle;
	int m_CorX;
	int m_CorY;

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向图象的指针	
	LPSTR	lpSrcStartBits;	//指向像素的指针

	afx_msg void OnPaint();
};
