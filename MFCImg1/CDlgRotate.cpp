// CDlgRotate.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "DIBAPI.h"
#include "CDlgRotate.h"
#include "CDlgPreview.h"
#include "function.h"


// CDlgRotate 对话框

IMPLEMENT_DYNAMIC(CDlgRotate, CDialogEx)

CDlgRotate::CDlgRotate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgRotate, pParent)
	, m_angle(0)
{

}

CDlgRotate::~CDlgRotate()
{
}

void CDlgRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ANGLE, m_angle);
}


BEGIN_MESSAGE_MAP(CDlgRotate, CDialogEx)
	ON_BN_CLICKED(IDC_PREW, &CDlgRotate::OnBnClickedPrew)
	ON_BN_CLICKED(IDOK, &CDlgRotate::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgRotate 消息处理程序


BOOL CDlgRotate::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	flag = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgRotate::OnBnClickedPrew()
{
	// 更改并预览
	UpdateData(TRUE);
	//调用旋转函数返回新图像句柄
	hNewDIB = (HDIB)::Rotate(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, m_angle);
	//锁定全局内存句柄hNewDIB以获取指向新图像数据的指针lpNewDIB
	LPSTR lpNewDIB = (LPSTR)::GlobalLock((HGLOBAL)hNewDIB);
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	LPBITMAPCOREHEADER lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;
	LONG lNewWidth = lpbmi->biWidth;
	LONG lNewHeight = lpbmi->biHeight;
	LONG lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	LPSTR lpNewDIBBits = ::FindDIBBits(lpNewDIB);

	CDlgPreview dlgPara;
	dlgPara.lSrcLineBytes = lNewLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lNewWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lNewHeight;
	dlgPara.lpSrcBitCount = 8;       //图像的位深
	dlgPara.lpSrcDib = (unsigned char *)lpNewDIB;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = (unsigned char *)lpNewDIBBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;
	::GlobalUnlock((HGLOBAL)hNewDIB);
	::GlobalFree((HGLOBAL)hNewDIB);

}


void CDlgRotate::OnBnClickedOk()
{
	UpdateData(TRUE);
	hNewDIB = (HDIB)::Rotate(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, m_angle);
	CDialogEx::OnOK();
}
