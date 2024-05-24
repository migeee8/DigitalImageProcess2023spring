// CDlgPreview.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgPreview.h"


// CDlgPreview 对话框

IMPLEMENT_DYNAMIC(CDlgPreview, CDialogEx)

CDlgPreview::CDlgPreview(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PREVIEW, pParent)
{

}

CDlgPreview::~CDlgPreview()
{
}

void CDlgPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPreview, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgPreview 消息处理程序


BOOL CDlgPreview::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	lpImg = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgPreview::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 不为绘图消息调用 CDialogEx::OnPaint()
	unsigned char* lpSrc;
	int r, b, g;
	if (lpSrcBitCount == 8) {
		for (int i = 0; i <= lSrcHeight - 1; i++)
		{
			for (int j = 0; j <= lSrcWidth - 1; j++)
			{
				lpSrc = (unsigned char*)pBmpBuf + lSrcLineBytes * (lSrcHeight - 1 - i) + j;
				r = *lpSrc;
				g = *lpSrc;
				b = *lpSrc;
				dc.SetPixel((j + 50), (i + 50), RGB(b, g, r));
			}
		}
	}
	else {
		for (int i = 0; i < lSrcHeight; i++) {
			for (int j = 0; j < lSrcWidth; j++) {
				lpSrc = (unsigned char*)pBmpBuf + (lSrcLineBytes * (lSrcHeight - 1 - i)) + (j * 3); // 指向当前像素的首字节

				// 读取红、绿、蓝通道的值
				b = *(lpSrc++);
				g = *(lpSrc++);
				r = *(lpSrc++);

				dc.SetPixel((j ), (i), RGB(r, g, b)); // 绘制像素
			}
		}
	}
}
