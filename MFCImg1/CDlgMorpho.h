#pragma once
#include "afxdialogex.h"


// CDlgMorpho 对话框

class CDlgMorpho : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMorpho)

public:
	CDlgMorpho(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgMorpho();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgMorph };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_command;
	int m_nmode;
	int m_object;
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedCancel();
	CButton m_control1;
	CButton m_control2;
	CButton m_control3;
	CButton m_control4;
	CButton m_control5;
	CButton m_control6;
	CButton m_control7;
	CButton m_control8;
	CButton m_control9;
	BOOL m_nStructure1;
	BOOL m_nStructure2;
	BOOL m_nStructure3;
	BOOL m_nStructure4;
	BOOL m_nStructure5;
	BOOL m_nStructure6;
	BOOL m_nStructure7;
	BOOL m_nStructure8;
	BOOL m_nStructure9;
};
