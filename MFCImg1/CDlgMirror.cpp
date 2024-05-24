// CDlgMirror.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgMirror.h"
#include "function.h"
#include "CDlgPreview.h"


// CDlgMirror 对话框

IMPLEMENT_DYNAMIC(CDlgMirror, CDialogEx)

CDlgMirror::CDlgMirror(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgMirror1, pParent)
	, m_dire(0)
{

}

CDlgMirror::~CDlgMirror()
{
}

void CDlgMirror::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_X_MIRR, m_dire);
}


BEGIN_MESSAGE_MAP(CDlgMirror, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMirror::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RECOVER, &CDlgMirror::OnBnClickedRecover)
	ON_BN_CLICKED(IDOK, &CDlgMirror::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgMirror 消息处理程序


BOOL CDlgMirror::OnInitDialog()
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


void CDlgMirror::OnBnClickedButton1()
{
	// 更改并预览
	UpdateData(TRUE);
	flag = Mirror(pBmpBuf, lSrcWidth,lSrcHeight, m_dire);
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


void CDlgMirror::OnBnClickedRecover()
{
	// 恢复
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
}


void CDlgMirror::OnBnClickedOk()
{
	if (flag) {
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		memcpy(lpSrcDib, lpImg, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	CDialogEx::OnOK();
}
