#pragma once
#include "afxdialogex.h"


// CDlgGammaTrans 对话框

class CDlgGammaTrans : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGammaTrans)

public:
	CDlgGammaTrans(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgGammaTrans();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgGammaTrans };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_gamma;
};
