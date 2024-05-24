// CDlgLaplacian.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgLaplacian.h"


// CDlgLaplacian 对话框

IMPLEMENT_DYNAMIC(CDlgLaplacian, CDialogEx)

CDlgLaplacian::CDlgLaplacian(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgLaplacian, pParent)
	, m_radio1(0)
{

}

CDlgLaplacian::~CDlgLaplacian()
{
}

void CDlgLaplacian::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_radio1);
}


BEGIN_MESSAGE_MAP(CDlgLaplacian, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgLaplacian::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgLaplacian::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CDlgLaplacian 消息处理程序


void CDlgLaplacian::OnBnClickedRadio1()
{
	//四邻域
	UpdateData(TRUE);
	FLOAT aValue[9] = { 0,-1,0,-1,5,-1,0,-1,0 };
	for (int i = 0; i < 9; i++)
		m_Value[i] = aValue[i];
}


void CDlgLaplacian::OnBnClickedRadio2()
{
	//八邻域
	 UpdateData(TRUE);
	 FLOAT aValue[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
	 for (int i = 0; i < 9; i++)
		 m_Value[i] = aValue[i];
}


BOOL CDlgLaplacian::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	FLOAT aValue[9] = { 0,-1,0,-1,5,-1,0,-1,0 };
	for (int i = 0; i < 9; i++)
		m_Value[i] = aValue[i];
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
