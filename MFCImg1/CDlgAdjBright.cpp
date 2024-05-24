// CDlgAdjBright.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgAdjBright.h"


// CDlgAdjBright 对话框

IMPLEMENT_DYNAMIC(CDlgAdjBright, CDialogEx)

CDlgAdjBright::CDlgAdjBright(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgAdjBright, pParent)
	, m_brightness(0)
{

}

CDlgAdjBright::~CDlgAdjBright()
{
}

void CDlgAdjBright::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDBRIGHTNESS, m_brightness);
}


BEGIN_MESSAGE_MAP(CDlgAdjBright, CDialogEx)
END_MESSAGE_MAP()


// CDlgAdjBright 消息处理程序


BOOL CDlgAdjBright::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(true);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
