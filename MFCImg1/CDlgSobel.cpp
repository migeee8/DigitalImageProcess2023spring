// CDlgSobel.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgSobel.h"


// CDlgSobel 对话框

IMPLEMENT_DYNAMIC(CDlgSobel, CDialogEx)

CDlgSobel::CDlgSobel(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgSobel, pParent)
	, m_temp1(0)
	, m_dire1(0)
{

}

CDlgSobel::~CDlgSobel()
{
}

void CDlgSobel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TEM_Roberts, m_temp1);
	DDX_Radio(pDX, IDC_DIRE_X, m_dire1);
	DDX_Control(pDX, IDCANCEL, m_static);
	DDX_Control(pDX, IDC_DIRE_X, m_bnt1);
	DDX_Control(pDX, IDC_DIRE_Y, m_bnt2);
}


BEGIN_MESSAGE_MAP(CDlgSobel, CDialogEx)
	ON_BN_CLICKED(IDC_TEM_Roberts, &CDlgSobel::OnBnClickedTemRoberts)
	ON_BN_CLICKED(IDC_TEM_Sobel, &CDlgSobel::OnBnClickedTemSobel)
	ON_BN_CLICKED(IDC_DIRE_X, &CDlgSobel::OnBnClickedDireX)
	ON_BN_CLICKED(IDC_DIRE_Y, &CDlgSobel::OnBnClickedDireY)
END_MESSAGE_MAP()


// CDlgSobel 消息处理程序


void CDlgSobel::OnBnClickedTemRoberts()
{
	// roberts算子
	UpdateData(TRUE);
	tempchoice = 0;
}


void CDlgSobel::OnBnClickedTemSobel()
{
	// sobel算子
	UpdateData(TRUE);
	tempchoice = 1;
}


void CDlgSobel::OnBnClickedDireX()
{
	// x方向
	UpdateData(TRUE);
	direchoice = 0;
}


void CDlgSobel::OnBnClickedDireY()
{
	// y方向
	UpdateData(TRUE);
	direchoice = 1;
}


BOOL CDlgSobel::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	tempchoice = 0;
	direchoice = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
