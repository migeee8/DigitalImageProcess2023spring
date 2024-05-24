#pragma once
#include "afxdialogex.h"


// CDlgAdjBright 对话框

class CDlgAdjBright : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAdjBright)

public:
	CDlgAdjBright(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgAdjBright();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgAdjBright };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_brightness;
};
