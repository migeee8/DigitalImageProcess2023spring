#pragma once
#include "afxdialogex.h"


// CDlgHistogram 对话框

class CDlgHistogram : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHistogram)

public:
	CDlgHistogram(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgHistogram();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgDisphist };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
		virtual void OnOK();
public:
	int m_iLowGray, m_iUpGray;
	LONG m_lCount[256];
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
