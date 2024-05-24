// CDlgStylize.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgStylize.h"
#include "CDlgPreview.h"
#include "function.h"


// CDlgStylize 对话框

IMPLEMENT_DYNAMIC(CDlgStylize, CDialogEx)

CDlgStylize::CDlgStylize(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgStylize, pParent)
	, m_choice(0)
{

}

CDlgStylize::~CDlgStylize()
{
}

void CDlgStylize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_choice);
}


BEGIN_MESSAGE_MAP(CDlgStylize, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgStylize::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgStylize::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgStylize::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_PREW, &CDlgStylize::OnBnClickedPrew)
	ON_BN_CLICKED(IDC_RECOVER, &CDlgStylize::OnBnClickedRecover)
	ON_BN_CLICKED(IDOK, &CDlgStylize::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgStylize 消息处理程序
BOOL CDlgStylize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//复制图像到对话框
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	lpImg = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStylize::OnBnClickedRadio1()
{
	// 效果1
	m_choice = 0;
	UpdateData(TRUE);
}


void CDlgStylize::OnBnClickedRadio2()
{
	// 效果2
	m_choice = 1;
	UpdateData(TRUE);
}


void CDlgStylize::OnBnClickedRadio3()
{
	// 效果3
	m_choice = 2;
	UpdateData(TRUE);
}


void CDlgStylize::OnBnClickedPrew()
{
	// 更改并预览
	UpdateData(TRUE);
	if (m_choice == 0) flag = Stylize1(pBmpBuf, lSrcWidth, lSrcHeight, lpSrcBitCount);
	else if (m_choice == 1) flag = Stylize2(pBmpBuf, lSrcWidth, lSrcHeight, lpSrcBitCount);
	else flag = Stylize3(pBmpBuf, lSrcWidth, lSrcHeight, lpSrcBitCount);

	// 创建对话框
	CDlgPreview dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpImg;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = pBmpBuf;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;
}


void CDlgStylize::OnBnClickedRecover()
{
	// 恢复
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
}


void CDlgStylize::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (flag) {
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		memcpy(lpSrcDib, lpImg, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	CDialogEx::OnOK();
}



