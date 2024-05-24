// CDlgAnalysis.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgAnalysis.h"
#include "function.h"
#include "CDlgShowRes.h"
#include "DIBAPI.H"

// CDlgAnalysis 对话框

IMPLEMENT_DYNAMIC(CDlgAnalysis, CDialogEx)

CDlgAnalysis::CDlgAnalysis(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgAnalysis, pParent)
	, m_color(1)
	, m_connection(1)
{

}

CDlgAnalysis::~CDlgAnalysis()
{
}

void CDlgAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_color);
	DDX_Radio(pDX, IDC_RADIO4, m_connection);
}


BEGIN_MESSAGE_MAP(CDlgAnalysis, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAnalysis::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgAnalysis::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgAnalysis::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgAnalysis::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgAnalysis::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO10, &CDlgAnalysis::OnBnClickedRadio10)
END_MESSAGE_MAP()


// CDlgAnalysis 消息处理程序


BOOL CDlgAnalysis::OnInitDialog()
{
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


void CDlgAnalysis::OnBnClickedOk()
{
	unsigned char* lpDstDib;
	size_t dstImageSize = sizeof(BITMAPINFOHEADER) + WIDTHBYTES(lSrcWidth * 24) * lSrcHeight;
	lpDstDib = new unsigned char[dstImageSize];
	// 创建图像信息头
	BITMAPINFOHEADER* pHead = (BITMAPINFOHEADER*)lpDstDib;
	pHead->biBitCount = 24;
	pHead->biWidth = lSrcWidth;
	pHead->biHeight = lSrcHeight;
	pHead->biPlanes = 1;
	pHead->biSize = 40;
	pHead->biSizeImage = WIDTHBYTES(lSrcWidth * 24) * lSrcHeight;
	pHead->biCompression = 0;
	pHead->biClrUsed = 0;
	pHead->biClrImportant = 0;

	// 拷贝图像信息头
	memcpy(lpDstDib, lpSrcDib, sizeof(BITMAPINFOHEADER));

	// 拷贝图像数据
	unsigned char* lpDstImageData = lpDstDib + sizeof(BITMAPINFOHEADER);
	size_t lLineBytes = WIDTHBYTES(lSrcWidth * 24);
	for (size_t i = 0; i < lSrcHeight; i++)
	{
		memset(lpDstImageData + lLineBytes * i, 0, lLineBytes);
	}

	//区域计数
	int count = countRegions(pBmpBuf, lSrcWidth, lSrcHeight, m_connection, m_color);
	ObjectProperties obj;
	obj = calculateObjectProperties(pBmpBuf, lSrcWidth, lSrcHeight, m_connection, m_color, lpDstImageData);
	CDlgShowRes dlgPara;
	dlgPara.m_area = obj.area;
	dlgPara.m_perimeter = obj.perimeter;
	dlgPara.m_circle = obj.circularity;
	dlgPara.m_rectangle = obj.rectangularity;
	dlgPara.m_CorX = obj.centerX;
	dlgPara.m_CorY = obj.centerY;

	dlgPara.lSrcLineBytes = lLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = 24;       //图像的位深
	dlgPara.lpSrcDib = (char*)lpDstDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = (char*)lpDstImageData;	//指向源像素的指针
	dlgPara.m_count = count;
	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;

	CDialogEx::OnOK();
}


void CDlgAnalysis::OnBnClickedRadio1()
{
	//前景为黑色
	m_color = 0;
	UpdateData(TRUE);
}


void CDlgAnalysis::OnBnClickedRadio2()
{
	//前景为白色
	m_color = 1;
	UpdateData(TRUE);
}


void CDlgAnalysis::OnBnClickedRadio3()
{
	//自动分辨前景
	m_color = AnalysisForeground(pBmpBuf, lSrcWidth, lSrcHeight);
	UpdateData(TRUE);
}


void CDlgAnalysis::OnBnClickedRadio4()
{
	//8连通
	m_connection = 0;
	UpdateData(TRUE);
}


void CDlgAnalysis::OnBnClickedRadio10()
{
	//4连通
	m_connection = 1;
	UpdateData(TRUE);
}
