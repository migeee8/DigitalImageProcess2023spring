#pragma once
#include "afxdialogex.h"


// CDlgLaplacian 对话框

class CDlgLaplacian : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLaplacian)

public:
	CDlgLaplacian(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgLaplacian();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgLaplacian };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_radio1;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	FLOAT m_Value[9];
	virtual BOOL OnInitDialog();
};
