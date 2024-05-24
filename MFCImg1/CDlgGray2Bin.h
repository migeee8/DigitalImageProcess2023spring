#pragma once
#include "afxdialogex.h"


// CDlgGray2Bin 对话框

class CDlgGray2Bin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGray2Bin)

public:
	CDlgGray2Bin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgGray2Bin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgGray2Bin };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_threshold;
};
