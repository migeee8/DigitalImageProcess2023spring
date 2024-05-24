#pragma once
#include "afxdialogex.h"


// CDlgSobel 对话框

class CDlgSobel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSobel)

public:
	CDlgSobel(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgSobel();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgSobel };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_temp1;
	int m_dire1;
	afx_msg void OnBnClickedTemRoberts();
	afx_msg void OnBnClickedTemSobel();
	afx_msg void OnBnClickedDireX();
	afx_msg void OnBnClickedDireY();
	CStatic m_static;
	CButton m_bnt1;
	CButton m_bnt2;
	int tempchoice;
	int direchoice;
	virtual BOOL OnInitDialog();
};
