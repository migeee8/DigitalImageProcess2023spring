// CDlgBitPlane.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgBitPlane.h"

#define DIB_MARKER   ((WORD) ('M' << 8) | 'B')
// CDlgBitPlane 对话框

IMPLEMENT_DYNAMIC(CDlgBitPlane, CDialogEx)

CDlgBitPlane::CDlgBitPlane(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgBitPlane, pParent)
	, m_bit(0)
{

}

CDlgBitPlane::~CDlgBitPlane()
{
}

void CDlgBitPlane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_InputBit, m_bit);
}


BEGIN_MESSAGE_MAP(CDlgBitPlane, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgBitPlane::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgBitPlane::OnBnClickedButton2)
	ON_WM_PAINT()
END_MESSAGE_MAP()



BOOL CDlgBitPlane::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	lpImgPlane = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	memcpy(lpImgPlane, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImgPlane + sizeof(BITMAPINFOHEADER) + palettesize;
	return TRUE;
}

// CDlgBitPlaneSlicing 消息处理程序
/////////////////////////////////////////
//------------------位平面切分函数
/*************************************************************************
* 函数名称：
*   BitPlaneSlicing1()
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
*   LONG  lHeight      - 源图像高度（象素数）
*   LONG  lLineBytes   -每行字节数
*   int   Bit    		- 分割 位 （0---7）
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
* 说明:
* 该函数用于对图像进行位平面分离运算。8位
************************************************************************/
BOOL BitPlaneSlicing1(LPSTR lpDIBBits, LONG lWidth, LONG  lLineBytes, LONG lHeight, int Bit)
{
	//遍历图像中每一个像素的指针
	unsigned char* lpSrc;
	//创建andValue，根据输入位数来与像素上的灰度值做与运算
	//将1左移Bit位，例如，若Bit为3，andValue=1000
	int andValue = 1 << Bit;
	for (int t = 0; t < 8; t++) {
		for (int i = 0; i < lHeight; i++) {
			for (int j = 0; j < lWidth; j++) {
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
				*lpSrc = *lpSrc & andValue;
			}
		}
	}
	return true;
}

void CDlgBitPlane::OnBnClickedButton1()
{
	// 分解button
	// 如果图片已经经过位分解操作，需要重新读取源图像
	if (flag == 1) {
		flag = 0;
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		lpImgPlane = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
		memcpy(lpImgPlane, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImgPlane + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	//读入要进行操作的位数
	int Bit;
	UpdateData(TRUE);
	Bit = m_bit;
	//位分解
	if((Bit>7)||(Bit<0)) AfxMessageBox(L"请输入0-7中的数字！");// 警告	
	else if (BitPlaneSlicing1((LPSTR)pBmpBuf, lSrcWidth, lSrcLineBytes, lSrcHeight, Bit)) flag = 1;
	//显示图像
	Invalidate();
}


void CDlgBitPlane::OnBnClickedButton2()
{
	// 保存button
	if (flag == 0) return;

	BOOL isOpen = FALSE;//是否打开(否则为保存)
	CString defaultDir = L" ";//默认打开的文件路径
	CString fileName = L"test.bmp";//默认打开的文件名
	CString filter = L"文件 (*.bmp;  )|*.bmp;*.ppt;*.xls||";//文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L" ";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}

	//如果位图数据指针为，则没有数据传入，函数返回
	errno_t err;
	if (!pBmpBuf)return;
	//颜色表大小，以字节为单位，灰度图像颜色表为字节，彩色图像颜色表大小为
	int colorTablesize = 0;
	if (lpSrcBitCount == 8)colorTablesize = 1024;
	//待存储图像数据每行字节数为的倍数
	//lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
	//lineByte = lpSrcStartBits;
	//以二进制写的方式打开文件

	CFile file;
	if (!file.Open(filePath, CFile::modeCreate |// 打开文件
		CFile::modeReadWrite | CFile::shareExclusive))
	{
		return;// 返回FALSE
	}


	//申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = DIB_MARKER;	// 填充文件头
	//bmp类型//bfSize是图像文件个组成部分之和
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lSrcHeight * lSrcLineBytes;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	//bfOffBits是图像文件前个部分所需空间之和
	fileHead.bfOffBits = 54 + colorTablesize;
	//写文件头进文件
	file.Write(&fileHead, sizeof(BITMAPFILEHEADER));// 写文件头		
	/*
	//申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER head;
	head.biBitCount = lpSrcBitCount;
	head.biClrImportant = 0;
	if (lpSrcBitCount == 8)
		head.biClrUsed = 256;
	else
		head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = lSrcHeight;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lSrcLineBytes*lSrcHeight;
	head.biWidth = lSrcWidth;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;
	//写位图信息头进内存
	file.Write(&head, sizeof(BITMAPFILEHEADER));// 写信息头

												//如果灰度图像，有颜色表，写入文件
	if (lpSrcBitCount == 8)
		file.Write(lpImgPlane, sizeof(RGBQUAD) * 256);


	file.Write(pBmpBuf, lSrcHeight*lSrcLineBytes);// 写像素
		*/

	file.Write(lpImgPlane, sizeof(BITMAPINFOHEADER) + colorTablesize + lSrcHeight * lSrcLineBytes);
	//关闭文件
	file.Close();
}


void CDlgBitPlane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// 不为绘图消息调用 CDialogEx::OnPaint()
	unsigned char* lpSrc;
	int r, b, g;
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


