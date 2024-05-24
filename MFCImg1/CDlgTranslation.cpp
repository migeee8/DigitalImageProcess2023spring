// CDlgTranslation.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgTranslation.h"
#include "function.h"
#include "CDlgPreview.h"


// CDlgTranslation 对话框

IMPLEMENT_DYNAMIC(CDlgTranslation, CDialogEx)

CDlgTranslation::CDlgTranslation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgTranslation, pParent)
	, m_xoffset(10)
	, m_yoffset(10)
{

}

CDlgTranslation::~CDlgTranslation()
{
}

void CDlgTranslation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X_OFFSET, m_xoffset);
	DDX_Text(pDX, IDC_Y_OFFSET, m_yoffset);
	DDX_Control(pDX, IDC_SLIDER_X, m_sliderCtrl1);
	DDX_Control(pDX, IDC_SLIDER_Y, m_sliderCtrl2);
}


BEGIN_MESSAGE_MAP(CDlgTranslation, CDialogEx)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_X, &CDlgTranslation::OnNMCustomdrawSliderX)
ON_WM_HSCROLL()
ON_EN_KILLFOCUS(IDC_X_OFFSET, &CDlgTranslation::OnEnKillfocusXOffset)
ON_EN_KILLFOCUS(IDC_Y_OFFSET, &CDlgTranslation::OnEnKillfocusYOffset)
ON_BN_CLICKED(IDC_PREW, &CDlgTranslation::OnBnClickedPrew)
ON_BN_CLICKED(IDC_RECOVER, &CDlgTranslation::OnBnClickedRecover)
ON_BN_CLICKED(IDOK, &CDlgTranslation::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgTranslation 消息处理程序

void CDlgTranslation::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 滑动时
	// 获取滑动条数据更新到变量中
	m_xoffset = m_sliderCtrl1.GetPos();
	m_yoffset = m_sliderCtrl2.GetPos();
	UpdateData(FALSE);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CDlgTranslation::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//复制图像到对话框
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	lpImg = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;

	//设置滑动条范围
	m_sliderCtrl1.SetRange(-lSrcWidth, lSrcWidth);
	m_sliderCtrl2.SetRange(-lSrcHeight, lSrcHeight);
	//设置互动条初始位置
	m_sliderCtrl1.SetPos(10);
	m_sliderCtrl2.SetPos(10);
	Invalidate();
	m_xoffset = m_sliderCtrl1.GetPos();
	m_yoffset = m_sliderCtrl2.GetPos();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTranslation::OnEnKillfocusXOffset()
{
	UpdateData(TRUE);
	m_sliderCtrl1.SetPos(m_xoffset);
}


void CDlgTranslation::OnEnKillfocusYOffset()
{
	UpdateData(TRUE);
	m_sliderCtrl2.SetPos(m_yoffset);
}


void CDlgTranslation::OnBnClickedPrew()
{
	// 更改并预览
	UpdateData(TRUE);
	flag = Translation(pBmpBuf, lSrcWidth, lSrcHeight, m_xoffset, m_yoffset);
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

void CDlgTranslation::OnBnClickedRecover()
{
	// 恢复图像
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
}


void CDlgTranslation::OnBnClickedOk()
{
	if (flag) {
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		memcpy(lpSrcDib, lpImg, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	CDialogEx::OnOK();
}
