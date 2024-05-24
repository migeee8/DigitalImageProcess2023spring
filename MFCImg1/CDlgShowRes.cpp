// CDlgShowRes.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgShowRes.h"


// CDlgShowRes 对话框

IMPLEMENT_DYNAMIC(CDlgShowRes, CDialogEx)

CDlgShowRes::CDlgShowRes(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgShowRes, pParent)
	, m_count(0)
	, m_area(0)
	, m_perimeter(0)
	, m_circle(0)
	, m_rectangle(0)
	, m_CorX(0)
	, m_CorY(0)
{

}

CDlgShowRes::~CDlgShowRes()
{
}

void CDlgShowRes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDX_Text(pDX, IDC_AREA, m_area);
	DDX_Text(pDX, IDC_PERIMETER, m_perimeter);
	DDX_Text(pDX, IDC_CIRCLE, m_circle);
	DDX_Text(pDX, IDC_RECTANGLE, m_rectangle);
	DDX_Text(pDX, IDC_X_COR, m_CorX);
	DDX_Text(pDX, IDC_Y_COR, m_CorY);
}


BEGIN_MESSAGE_MAP(CDlgShowRes, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgShowRes 消息处理程序


void CDlgShowRes::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 不为绘图消息调用 CDialogEx::OnPaint()
	unsigned char* lpSrc;
	int r, b, g;
	for (int i = 0; i < lSrcHeight; i++) {
		for (int j = 0; j < lSrcWidth; j++) {
			lpSrc = (unsigned char*)lpSrcStartBits + (lSrcLineBytes * (lSrcHeight - 1 - i)) + (j * 3); // 指向当前像素的首字节

			// 读取红、绿、蓝通道的值
			b = *(lpSrc++);
			g = *(lpSrc++);
			r = *(lpSrc++);

			dc.SetPixel((j + 400), (i + 25), RGB(r, g, b)); // 绘制像素
		}
	}
}
