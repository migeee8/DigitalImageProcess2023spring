// CDlgSmooth.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgSmooth.h"


// CDlgSmooth 对话框

IMPLEMENT_DYNAMIC(CDlgSmooth, CDialogEx)

CDlgSmooth::CDlgSmooth(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgSmooth, pParent)
	, m_fcoef(1.0/25)
	, m_fheight(3)
	, m_fwidth(3)
	, m_fcorx(1)
	, m_fcory(1)
	, m_f1(1)
	, m_f2(1)
	, m_f3(1)
	, m_f4(1)
	, m_f5(1)
	, m_f6(1)
	, m_f7(1)
	, m_f8(1)
	, m_f9(1)
	, m_f10(1)
	, m_f11(1)
	, m_f12(1)
	, m_f13(1)
	, m_f14(1)
	, m_f15(1)
	, m_f16(1)
	, m_f17(1)
	, m_f18(1)
	, m_f19(1)
	, m_f20(1)
	, m_f21(1)
	, m_f22(1)
	, m_f23(1)
	, m_f24(1)
	, m_f25(1)
	, m_bn1(0)
{

}

CDlgSmooth::~CDlgSmooth()
{
}


void CDlgSmooth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Coefficient, m_fcoef);
	DDX_Text(pDX, IDC_fHeight, m_fheight);
	DDX_Text(pDX, IDC_fWidth, m_fwidth);
	DDX_Text(pDX, IDC_fcorX, m_fcorx);
	DDX_Text(pDX, IDC_fcorY, m_fcory);
	DDX_Text(pDX, IDC_f1, m_f1);
	DDX_Text(pDX, IDC_f2, m_f2);
	DDX_Text(pDX, IDC_f3, m_f3);
	DDX_Text(pDX, IDC_f4, m_f4);
	DDX_Text(pDX, IDC_f5, m_f5);
	DDX_Text(pDX, IDC_f6, m_f6);
	DDX_Text(pDX, IDC_f7, m_f7);
	DDX_Text(pDX, IDC_f8, m_f8);
	DDX_Text(pDX, IDC_f9, m_f9);
	DDX_Text(pDX, IDC_f10, m_f10);
	DDX_Text(pDX, IDC_f11, m_f11);
	DDX_Text(pDX, IDC_f12, m_f12);
	DDX_Text(pDX, IDC_f13, m_f13);
	DDX_Text(pDX, IDC_f14, m_f14);
	DDX_Text(pDX, IDC_f15, m_f15);
	DDX_Text(pDX, IDC_f16, m_f16);
	DDX_Text(pDX, IDC_f17, m_f17);
	DDX_Text(pDX, IDC_f18, m_f18);
	DDX_Text(pDX, IDC_f19, m_f19);
	DDX_Text(pDX, IDC_f20, m_f20);
	DDX_Text(pDX, IDC_f21, m_f21);
	DDX_Text(pDX, IDC_f22, m_f22);
	DDX_Text(pDX, IDC_f23, m_f23);
	DDX_Text(pDX, IDC_f24, m_f24);
	DDX_Text(pDX, IDC_f25, m_f25);
	DDX_Radio(pDX, IDC_RADIO1, m_bn1);
	DDX_Control(pDX, IDC_f1, m_e1);
	DDX_Control(pDX, IDC_f2, m_e2);
	DDX_Control(pDX, IDC_f3, m_e3);
	DDX_Control(pDX, IDC_f4, m_e4);
	DDX_Control(pDX, IDC_f5, m_e5);
	DDX_Control(pDX, IDC_f6, m_e6);
	DDX_Control(pDX, IDC_f10, m_e10);
	DDX_Control(pDX, IDC_f11, m_e11);
	DDX_Control(pDX, IDC_f15, m_e15);
	DDX_Control(pDX, IDC_f16, m_e16);
	DDX_Control(pDX, IDC_f20, m_e20);
	DDX_Control(pDX, IDC_f21, m_e21);
	DDX_Control(pDX, IDC_f22, m_e22);
	DDX_Control(pDX, IDC_f23, m_e23);
	DDX_Control(pDX, IDC_f24, m_e24);
	DDX_Control(pDX, IDC_f25, m_e25);
	DDX_Control(pDX, IDC_f7, m_e7);
	DDX_Control(pDX, IDC_f8, m_e8);
	DDX_Control(pDX, IDC_f9, m_e9);
	DDX_Control(pDX, IDC_f12, m_e12);
	DDX_Control(pDX, IDC_f13, m_e13);
	DDX_Control(pDX, IDC_f14, m_e14);
	DDX_Control(pDX, IDC_f17, m_e17);
	DDX_Control(pDX, IDC_f18, m_e18);
	DDX_Control(pDX, IDC_f19, m_e19);
}


BEGIN_MESSAGE_MAP(CDlgSmooth, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgSmooth::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgSmooth::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgSmooth::OnBnClickedRadio3)
END_MESSAGE_MAP()

BOOL CDlgSmooth::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	return TRUE;
}


// CDlgSmooth 消息处理程序

void CDlgSmooth::editcrtlShow()
{
	m_e1.ShowWindow(SW_SHOW);
	m_e2.ShowWindow(SW_SHOW);
	m_e3.ShowWindow(SW_SHOW);
	m_e4.ShowWindow(SW_SHOW);
	m_e5.ShowWindow(SW_SHOW);
	m_e6.ShowWindow(SW_SHOW);
	m_e10.ShowWindow(SW_SHOW);
	m_e11.ShowWindow(SW_SHOW);
	m_e15.ShowWindow(SW_SHOW);
	m_e16.ShowWindow(SW_SHOW);
	m_e20.ShowWindow(SW_SHOW);
	m_e21.ShowWindow(SW_SHOW);
	m_e22.ShowWindow(SW_SHOW);
	m_e23.ShowWindow(SW_SHOW);
	m_e24.ShowWindow(SW_SHOW);
	m_e25.ShowWindow(SW_SHOW);
}

void CDlgSmooth::editcrtlHide()
{
	m_e1.ShowWindow(SW_HIDE);
	m_e2.ShowWindow(SW_HIDE);
	m_e3.ShowWindow(SW_HIDE);
	m_e4.ShowWindow(SW_HIDE);
	m_e5.ShowWindow(SW_HIDE);
	m_e6.ShowWindow(SW_HIDE);
	m_e10.ShowWindow(SW_HIDE);
	m_e11.ShowWindow(SW_HIDE);
	m_e15.ShowWindow(SW_HIDE);
	m_e16.ShowWindow(SW_HIDE);
	m_e20.ShowWindow(SW_HIDE);
	m_e21.ShowWindow(SW_HIDE);
	m_e22.ShowWindow(SW_HIDE);
	m_e23.ShowWindow(SW_HIDE);
	m_e24.ShowWindow(SW_HIDE);
	m_e25.ShowWindow(SW_HIDE);
}

void CDlgSmooth::editcrtlOnlyRead()
{
	m_e7.SetReadOnly(TRUE);
	m_e8.SetReadOnly(TRUE);
	m_e9.SetReadOnly(TRUE);
	m_e12.SetReadOnly(TRUE);
	m_e13.SetReadOnly(TRUE);
	m_e14.SetReadOnly(TRUE);
	m_e17.SetReadOnly(TRUE);
	m_e18.SetReadOnly(TRUE);
	m_e19.SetReadOnly(TRUE);

}

void CDlgSmooth::editcrtlEditable()
{
	m_e7.SetReadOnly(FALSE);
	m_e8.SetReadOnly(FALSE);
	m_e9.SetReadOnly(FALSE);
	m_e12.SetReadOnly(FALSE);
	m_e13.SetReadOnly(FALSE);
	m_e14.SetReadOnly(FALSE);
	m_e17.SetReadOnly(FALSE);
	m_e18.SetReadOnly(FALSE);
	m_e19.SetReadOnly(FALSE);

}
void CDlgSmooth::OnBnClickedRadio1()
{
	UpdateData(TRUE);
	editcrtlHide();
	editcrtlOnlyRead();
	// 均值滤波模板
	m_fcoef = 1.0 / 9;
	m_fheight = 3;
	m_fwidth = 3;
	m_f7 = 1;
	m_f8 = 1;
	m_f9 = 1;
	m_f12 = 1;
	m_f13 = 1;
	m_f14 = 1;
	m_f17 = 1;
	m_f18 = 1;
	m_f19 = 1;
	m_fcorx = 1;
	m_fcory = 1;
	UpdateData(FALSE);
}


void CDlgSmooth::OnBnClickedRadio2()
{
	UpdateData(TRUE);
	editcrtlHide();
	editcrtlOnlyRead();
	// 高斯滤波模板
	m_fcoef = 1.0 / 16;
	m_fheight = 3;
	m_fwidth = 3;
	m_f7 = 1;
	m_f8 = 2;
	m_f9 = 1;
	m_f12 = 2;
	m_f13 = 4;
	m_f14 = 2;
	m_f17 = 1;
	m_f18 = 2;
	m_f19 = 1;
	m_fcorx = 1;
	m_fcory = 1;
	UpdateData(FALSE);
}

void CDlgSmooth::OnBnClickedRadio3()
{
	UpdateData(TRUE);
	editcrtlShow();
	editcrtlEditable();
	// 自定义平滑模板
	m_fheight = 5;
	m_fwidth = 5;
	m_f7 = 1;
	m_f8 = 1;
	m_f9 = 1;
	m_f12 = 1;
	m_f13 = 1;
	m_f14 = 1;
	m_f17 = 1;
	m_f18 = 1;
	m_f19 = 1;
	m_fcorx = 2;
	m_fcory = 2;
	UpdateData(FALSE);
	//m_fcoef = 1.0 / (m_f1 + m_f2 + m_f3 + m_f4 + m_f5 + m_f6 + m_f7 + m_f8 + m_f9 + m_f10 + m_f11 + m_f12 + m_f13 + m_f14 + m_f15 + m_f16 + m_f17 + m_f18 + m_f19 + m_f20 + m_f21 + m_f22 + m_f23 + m_f24 + m_f25);
}



