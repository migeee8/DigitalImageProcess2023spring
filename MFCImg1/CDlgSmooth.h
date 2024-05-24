#pragma once
#include "afxdialogex.h"


// CDlgSmooth 对话框

class CDlgSmooth : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSmooth)

public:
	CDlgSmooth(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgSmooth();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgSmooth };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	void editcrtlShow();
	void editcrtlHide();
	void editcrtlOnlyRead();
	void editcrtlEditable();
	float m_fcoef;
	int m_fheight;
	int m_fwidth;
	int m_fcorx;
	int m_fcory;
	int m_f1;
	int m_f2;
	int m_f3;
	int m_f4;
	int m_f5;
	int m_f6;
	int m_f7;
	int m_f8;
	int m_f9;
	int m_f10;
	int m_f11;
	int m_f12;
	int m_f13;
	int m_f14;
	int m_f15;
	int m_f16;
	int m_f17;
	int m_f18;
	int m_f19;
	int m_f20;
	int m_f21;
	int m_f22;
	int m_f23;
	int m_f24;
	int m_f25;
	virtual BOOL OnInitDialog();
	int m_bn1;
	CEdit m_e1;
	CEdit m_e2;
	CEdit m_e3;
	CEdit m_e4;
	CEdit m_e5;
	CEdit m_e6;
	CEdit m_e10;
	CEdit m_e11;
	CEdit m_e15;
	CEdit m_e16;
	CEdit m_e20;
	CEdit m_e21;
	CEdit m_e22;
	CEdit m_e23;
	CEdit m_e24;
	CEdit m_e25;
	CEdit m_e7;
	CEdit m_e8;
	CEdit m_e9;
	CEdit m_e12;
	CEdit m_e13;
	CEdit m_e14;
	CEdit m_e17;
	CEdit m_e18;
	CEdit m_e19;
};
