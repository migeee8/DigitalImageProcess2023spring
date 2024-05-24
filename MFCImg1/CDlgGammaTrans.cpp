// CDlgGammaTrans.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgGammaTrans.h"


// CDlgGammaTrans 对话框

IMPLEMENT_DYNAMIC(CDlgGammaTrans, CDialogEx)

CDlgGammaTrans::CDlgGammaTrans(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgGammaTrans, pParent)

	, m_gamma(0)
{

}

CDlgGammaTrans::~CDlgGammaTrans()
{
}

void CDlgGammaTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, m_gamma);
}


BEGIN_MESSAGE_MAP(CDlgGammaTrans, CDialogEx)
END_MESSAGE_MAP()


// CDlgGammaTrans 消息处理程序
