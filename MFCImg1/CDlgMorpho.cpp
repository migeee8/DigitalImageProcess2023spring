// CDlgMorpho.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgMorpho.h"


// CDlgMorpho 对话框

IMPLEMENT_DYNAMIC(CDlgMorpho, CDialogEx)

CDlgMorpho::CDlgMorpho(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgMorph, pParent)
	, m_command(0)
	, m_nmode(0)
	, m_object(0)

	, m_nStructure1(FALSE)
	, m_nStructure2(FALSE)
	, m_nStructure3(FALSE)
	, m_nStructure4(FALSE)
	, m_nStructure5(FALSE)
	, m_nStructure6(FALSE)
	, m_nStructure7(FALSE)
	, m_nStructure8(FALSE)
	, m_nStructure9(FALSE)
{

}

CDlgMorpho::~CDlgMorpho()
{
}

void CDlgMorpho::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_command);
	DDX_Radio(pDX, IDC_RADIO7, m_nmode);
	DDX_Radio(pDX, IDC_RADIO19, m_object);
	DDX_Control(pDX, IDC_CHECK1, m_control1);
	DDX_Control(pDX, IDC_CHECK2, m_control2);
	DDX_Control(pDX, IDC_CHECK3, m_control3);
	DDX_Control(pDX, IDC_CHECK4, m_control4);
	DDX_Control(pDX, IDC_CHECK5, m_control5);
	DDX_Control(pDX, IDC_CHECK6, m_control6);
	DDX_Control(pDX, IDC_CHECK7, m_control7);
	DDX_Control(pDX, IDC_CHECK8, m_control8);
	DDX_Control(pDX, IDC_CHECK9, m_control9);
	DDX_Check(pDX, IDC_CHECK1, m_nStructure1);
	DDX_Check(pDX, IDC_CHECK2, m_nStructure2);
	DDX_Check(pDX, IDC_CHECK3, m_nStructure3);
	DDX_Check(pDX, IDC_CHECK4, m_nStructure4);
	DDX_Check(pDX, IDC_CHECK5, m_nStructure5);
	DDX_Check(pDX, IDC_CHECK6, m_nStructure6);
	DDX_Check(pDX, IDC_CHECK7, m_nStructure7);
	DDX_Check(pDX, IDC_CHECK8, m_nStructure8);
	DDX_Check(pDX, IDC_CHECK9, m_nStructure9);
}


BEGIN_MESSAGE_MAP(CDlgMorpho, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO7, &CDlgMorpho::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CDlgMorpho::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CDlgMorpho::OnBnClickedRadio9)
	ON_BN_CLICKED(IDCANCEL, &CDlgMorpho::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgMorpho 消息处理程序


void CDlgMorpho::OnBnClickedRadio7()
{
	// 水平方向
	//用户选择“自定义”的时候3*3结构元素禁止输入
	m_control1.EnableWindow(FALSE);
	m_control2.EnableWindow(FALSE);
	m_control3.EnableWindow(FALSE);
	m_control4.EnableWindow(FALSE);
	m_control5.EnableWindow(FALSE);
	m_control6.EnableWindow(FALSE);
	m_control7.EnableWindow(FALSE);
	m_control8.EnableWindow(FALSE);
	m_control9.EnableWindow(FALSE);
}


void CDlgMorpho::OnBnClickedRadio8()
{
	// 垂直方向
	//用户选择“自定义”的时候3*3结构元素禁止输入
	m_control1.EnableWindow(FALSE);
	m_control2.EnableWindow(FALSE);
	m_control3.EnableWindow(FALSE);
	m_control4.EnableWindow(FALSE);
	m_control5.EnableWindow(FALSE);
	m_control6.EnableWindow(FALSE);
	m_control7.EnableWindow(FALSE);
	m_control8.EnableWindow(FALSE);
	m_control9.EnableWindow(FALSE);
}


void CDlgMorpho::OnBnClickedRadio9()
{
	// 自定义
	//用户选择“自定义”的时候3*3结构元素允许输入
	m_control1.EnableWindow(TRUE);
	m_control2.EnableWindow(TRUE);
	m_control3.EnableWindow(TRUE);
	m_control4.EnableWindow(TRUE);
	m_control5.EnableWindow(TRUE);
	m_control6.EnableWindow(TRUE);
	m_control7.EnableWindow(TRUE);
	m_control8.EnableWindow(TRUE);
	m_control9.EnableWindow(TRUE);
}



void CDlgMorpho::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
