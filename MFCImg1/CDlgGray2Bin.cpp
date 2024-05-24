// CDlgGray2Bin.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgGray2Bin.h"


// CDlgGray2Bin 对话框

IMPLEMENT_DYNAMIC(CDlgGray2Bin, CDialogEx)

CDlgGray2Bin::CDlgGray2Bin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgGray2Bin, pParent)
	, m_threshold(127)
{

}

CDlgGray2Bin::~CDlgGray2Bin()
{
}

void CDlgGray2Bin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_threshold);
}


BEGIN_MESSAGE_MAP(CDlgGray2Bin, CDialogEx)
END_MESSAGE_MAP()


// CDlgGray2Bin 消息处理程序
