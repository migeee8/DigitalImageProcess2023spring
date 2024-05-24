// CDlgZoom.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgZoom.h"


// CDlgZoom 对话框

IMPLEMENT_DYNAMIC(CDlgZoom, CDialogEx)

CDlgZoom::CDlgZoom(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgZoom, pParent)
	, m_xRatio(0)
	, m_yRatio(0)
{

}

CDlgZoom::~CDlgZoom()
{
}

void CDlgZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_X_RATIO, m_xRatio);
	DDX_Text(pDX, IDC_Y_RATIO, m_yRatio);
}

BEGIN_MESSAGE_MAP(CDlgZoom, CDialogEx)
END_MESSAGE_MAP()
// CDlgZoom 消息处理程序


BOOL CDlgZoom::OnInitDialog()
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


