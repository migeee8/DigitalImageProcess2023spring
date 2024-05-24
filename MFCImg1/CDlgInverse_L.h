#pragma once
#include "afxdialogex.h"


// CDlgInverse_L 对话框

class CDlgInverse_L : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInverse_L)

public:
	CDlgInverse_L(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgInverse_L();
	int temp;
	//响应鼠标的区域
	CRect m_MouseRect;
	//标识是否拖动
	BOOL m_bIsDrawing;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgInverse_L };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_threshold;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusThreshold();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
