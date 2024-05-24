
// MFCImg1View.cpp: CMFCImg1View 类的实现
//
#define _USE_MATH_DEFINES
#include "math.h"
#include <random>
#include<time.h>
#include<stdlib.h>
#include "pch.h"
#include "framework.h"
#include "DIBAPI.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCImg1.h"
#endif

#include "MFCImg1Doc.h"
#include "MFCImg1View.h"

#include "CDlgImgInfo.h"
#include "CDlgHistogram.h"
#include "CDlgAdjBright.h"
#include "CDlgGray2Bin.h"
#include "CDlgLinerPara.h"
#include "CDlgInverse_V.h"
#include "CDlg_V.h"
#include "CDlgInverse_L.h"
#include "CDlg_L.h"
#include "CDlgBitPlane.h"
#include "CDlgGammaTrans.h"
#include "CDlgSmooth.h"
#include "CDlgLaplacian.h"
#include "CDlgSobel.h"
#include "CDlgMorpho.h"
#include "CDlgThresholdTrans.h"
#include "CDlgWinChange.h"
#include "CDlgHisStretch.h"
#include "CDlgTemplate.h"
#include "CDlgTranslation.h"
#include "CDlgMirror.h"
#include "CDlgRotate.h"
#include "CDlgZoom.h"
#include "CDlgAnalysis.h"
#include "CDlgStylize.h"

#include "PointTrans.h"
#include "TemplateTrans.h"
#include "morpho.h"
#include "function.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCImg1View

IMPLEMENT_DYNCREATE(CMFCImg1View, CView)

BEGIN_MESSAGE_MAP(CMFCImg1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DispImgInfo, &CMFCImg1View::OnDispimginfo)
	ON_COMMAND(ID_AdjBright, &CMFCImg1View::OnAdjbright)
	ON_COMMAND(ID_Redo, &CMFCImg1View::OnRedo)
	ON_COMMAND(ID_24colorTo8color, &CMFCImg1View::On24colorto8color)
	ON_COMMAND(ID_8colorTo8gray, &CMFCImg1View::On8colorto8gray)
	ON_COMMAND(ID_HalfTone, &CMFCImg1View::OnHalftone)
	ON_COMMAND(ID_PointInvert, &CMFCImg1View::OnPointinvert)
	ON_COMMAND(ID_Vtrans, &CMFCImg1View::OnVtrans)
	ON_COMMAND(ID_InvertedVTrans, &CMFCImg1View::OnInvertedvtrans)
	ON_COMMAND(ID_LTrans, &CMFCImg1View::OnLtrans)
	ON_COMMAND(ID_ReverLTrans, &CMFCImg1View::OnReverltrans)
	ON_COMMAND(ID_LinearTrans, &CMFCImg1View::OnLineartrans)
	ON_COMMAND(ID_BitPlane, &CMFCImg1View::OnBitplane)
	ON_COMMAND(ID_GammaTrans, &CMFCImg1View::OnGammatrans)
	ON_COMMAND(ID_LogTrans, &CMFCImg1View::OnLogtrans)
	ON_COMMAND(ID_Gray2Binary, &CMFCImg1View::OnGray2binary)
	ON_COMMAND(ID_DispHist, &CMFCImg1View::OnDisphist)
	ON_COMMAND(ID_EnhanSmooth, &CMFCImg1View::OnEnhansmooth)
	ON_COMMAND(ID_AddNoise, &CMFCImg1View::OnAddnoise)
	ON_COMMAND(ID_Laplacian, &CMFCImg1View::OnLaplacian)
	ON_COMMAND(ID_Sobel, &CMFCImg1View::OnSobel)
	ON_COMMAND(ID_AddGNoise, &CMFCImg1View::OnAddgnoise)
	ON_COMMAND(ID_Median, &CMFCImg1View::OnMedian)
	ON_COMMAND(ID_Morphology, &CMFCImg1View::OnMorphology)
	ON_COMMAND(ID_ThresholdTrans, &CMFCImg1View::OnThresholdtrans)
	ON_COMMAND(ID_WnChange, &CMFCImg1View::OnWnchange)
	ON_COMMAND(ID_HisStretch, &CMFCImg1View::OnHisstretch)
	ON_COMMAND(ID_HisBalance, &CMFCImg1View::OnHisbalance)
	ON_COMMAND(ID_Template, &CMFCImg1View::OnTemplate)
//	ON_COMMAND(ID_Geometric, &CMFCImg1View::OnGeometric)
ON_COMMAND(ID_Translation, &CMFCImg1View::OnTranslation)
ON_COMMAND(ID_Rotate, &CMFCImg1View::OnRotate)
ON_COMMAND(ID_Mirror, &CMFCImg1View::OnMirror)
ON_COMMAND(ID_ImgCount, &CMFCImg1View::OnImgcount)
ON_COMMAND(ID_Stylize, &CMFCImg1View::OnStylize)
ON_COMMAND(ID_Canny, &CMFCImg1View::OnCanny)
ON_COMMAND(ID_USM, &CMFCImg1View::OnUsm)
END_MESSAGE_MAP()

// CMFCImg1View 构造/析构

CMFCImg1View::CMFCImg1View() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCImg1View::~CMFCImg1View()
{
}

BOOL CMFCImg1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCImg1View 绘图

void CMFCImg1View::OnDraw(CDC* pDC)
{
	CMFCImg1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	HGLOBAL hDIB = pDoc->GetHObject();

	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDibSection = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

		// 获取DIB宽度
		int cxDIB = (int)pDoc->m_dib.GetWidth(lpDibSection);

		// 获取DIB高度
		int cyDIB = (int)pDoc->m_dib.GetHeight(lpDibSection);
		::GlobalUnlock((HGLOBAL)hDIB);
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest = rcDIB;
		// 输出DIB
		pDoc->m_dib.DrawDib(pDC->m_hDC, &rcDest, pDoc->GetHObject(),
			&rcDIB, pDoc->GetDocPal());
	}
}


// CMFCImg1View 打印

BOOL CMFCImg1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCImg1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCImg1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCImg1View 诊断

#ifdef _DEBUG
void CMFCImg1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCImg1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCImg1Doc* CMFCImg1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCImg1Doc)));
	return (CMFCImg1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCImg1View 消息处理程序


void CMFCImg1View::OnDispimginfo()
{
	
	CMFCImg1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		   AfxMessageBox(L"对不起，不是256色位图！");// 警告
		   ::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
		   return;									//返回
	 }										//判断是否是8-bpp位图,不是则返回
	 */
	
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	CDlgImgInfo dlgPara;// 创建对话框		
	dlgPara.m_Width = lSrcWidth;
	dlgPara.m_Height = lSrcHeight;
	dlgPara.m_bitCount = lpSrcBitCount;

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
	
}

void CMFCImg1View::OnDisphist()
{
	// 简单显示直方图

	CMFCImg1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	LONG	m_lCount[256];  //灰度统计表
	unsigned char* lpSrc;  //指向每个像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		   AfxMessageBox(L"对不起，不是256色位图！");// 警告
		   ::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
		   return;									//返回
	 }										//判断是否是8-bpp位图,不是则返回

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	BeginWaitCursor();// 更改光标形状

	//计算直方图
	for (int i = 0; i < 256; i++)
	{
		// 清零
		m_lCount[i] = 0;
	}
	// 计算各个灰度值的计数
	for (size_t i = 0; i < lSrcHeight; i++)
	{
		for (size_t j = 0; j < lSrcHeight; j++)
		{
			
			lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * i + j;
			// 计数加1
			m_lCount[*(lpSrc)]++;
		}
	}
	// 创建对话框
	CDlgHistogram dlgHist;
	// 初始化变量值
	dlgHist.m_iLowGray = 0;
	dlgHist.m_iUpGray = 255;
	for (int i = 0; i < 256; i++)
	{
		dlgHist.m_lCount[i] = m_lCount[i];
	}
	// 显示对话框
	if (dlgHist.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定

	// 恢复光标
	EndWaitCursor();
}


void CMFCImg1View::OnAdjbright()
{

	CMFCImg1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		   AfxMessageBox(L"对不起，不是256色位图！");// 警告
		   ::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
		   return;									//返回
	 }										//判断是否是8-bpp位图,不是则返回
	 */
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	CDlgAdjBright dlgBright;// 创建对话框	
	if (dlgBright.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	int nValue = dlgBright.m_brightness; 

	AddBrightness(lpSrcStartBits, lpSrcBitCount, lSrcWidth, lSrcHeight, nValue);//改变亮度
	//修改完成后关闭对话框
	delete dlgBright;
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定	
}


void CMFCImg1View::OnRedo()
{
	
	// 恢复原图像
	CMFCImg1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	LPSTR lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	LONG lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	LONG lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	int lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	LONG lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	int nHeaderSize = sizeof(BITMAPINFOHEADER) + pDoc->m_dib.GetPalSize(lpSrcDib);   //获取头部和调色板所占字节数
	int nImageSize = lSrcHeight * lSrcLineBytes;                           //获取图像信息所占字节数
	////////////////////////////////////////////////////////////////////////////////////////////////	
	memcpy(lpSrcDib, pDoc->lpImgCpy, nHeaderSize + nImageSize);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::On24colorto8color()
{
	// 24位彩色转8位彩色
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long    lSrcLineBytes;	//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 24)// 判断是否是24-bpp位图
	{
		AfxMessageBox(L"对不起，不是24位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

	Trans24cTo8c(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight);// 24位彩色转8位彩色	 

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::On8colorto8gray()
{
	// 8位彩色转8位灰度
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

	Trans8cTo8g(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight);// 8位彩色转8位灰度	 


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}



void CMFCImg1View::OnGray2binary()
{
	// 八位转二值图像
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgGray2Bin dlgPara;
	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	int threshold = dlgPara.m_threshold;
	Trans8gToBin(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, threshold);// 8位灰度转二值


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}



void CMFCImg1View::OnHalftone()
{
	// 半影调处理
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

	HalfTone(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight);// 半影调处理 


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnPointinvert()
{
	// 图像反色
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	float fa, fb;
	fa = -1;
	fb = 255;
	LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, fa, fb);// 线性变换（反色）

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnVtrans()
{
	// V字变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlg_V dlgPara;
	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	float tp = dlgPara.m_threshold;
	Imgpoint A(0, 255);
	Imgpoint B(tp, 0);
	Imgpoint C(255, 255);
	Pw2LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, A, B, C);// 分段线性函数（2段）--V字变换

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnInvertedvtrans()
{
	// 倒V字变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgInverse_V dlgPara;
	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	float tp = dlgPara.m_threshold;
	Imgpoint A(0, 0);
	Imgpoint B(tp, 255);
	Imgpoint C(255, 0);
	Pw2LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, A, B, C);// 分段线性函数（2段）--倒V字变换

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnLtrans()
{
	// L字变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlg_L dlgPara;
	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	float tp = dlgPara.m_threshold;
	Imgpoint A(0, 255);
	Imgpoint B(tp, 0);
	Imgpoint C(255, 0);
	Pw2LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, A, B, C);// 分段线性函数（2段）--L字变换

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnReverltrans()
{
	// 倒L字变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgInverse_L dlgPara;
	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	float tp = dlgPara.m_threshold;
	Imgpoint A(0, 0);
	Imgpoint B(tp, 255);
	Imgpoint C(255, 255);
	Pw2LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, A, B, C);// 分段线性函数（2段）--倒L字变换

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnLineartrans()
{
	// 线性变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgLinerPara dlgPara;// 创建对话框		

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	float fa = dlgPara.m_fA;
	float fb = dlgPara.m_fB;
	LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, fa, fb);// 线性变换

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}


void CMFCImg1View::OnBitplane()
{
	// 位平面分解
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgBitPlane dlgPara;// 创建对话框	

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

void CMFCImg1View::OnLogtrans()
{
	// log变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	logTrans(lpSrcStartBits, lSrcWidth, lSrcHeight);// log变换

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnGammatrans()
{
	// 伽马变换
	// log变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgGammaTrans dlgPara;// 创建对话框	

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	float gamma = dlgPara.m_gamma;
	GammaTrans(lpSrcStartBits, lSrcWidth, lSrcHeight,gamma);// Gamma变换

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnEnhansmooth()
{
	// 图像平滑
	CMFCImg1Doc* pDoc = GetDocument();
	LPSTR lpDIB;
	LPSTR lpDIBBits;
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	//模板高度
	int iTempH;
	//模板宽度
	int iTempW;
	//模板系数
	int iTempC;
	//模板中心元素X坐标
	int iTempMX;
	//模板中心元素Y坐标
	int iTempMY;
	//模板系数
	float fTempC;
	//锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) return;
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度
	CDlgSmooth dlgPara;// 创建对话框	

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	//获取模板高度宽度
	/*
	iTempH = 3;
	iTempW = 3;
	*/
	iTempH = dlgPara.m_fheight;
	iTempW = dlgPara.m_fwidth;
	//获取模板数组
	FLOAT aValue[25] = { 0 };
	FLOAT elSum = 0;
	//3*3
	if (iTempH == 3)
	{
		// 给模板数组赋值
		aValue[0] = dlgPara.m_f7;
		aValue[1] = dlgPara.m_f8;
		aValue[2] = dlgPara.m_f9;
		aValue[3] = dlgPara.m_f12;
		aValue[4] = dlgPara.m_f13;
		aValue[5] = dlgPara.m_f14;
		aValue[6] = dlgPara.m_f17;
		aValue[7] = dlgPara.m_f18;
		aValue[8] = dlgPara.m_f19;
	}
	//5*5
	else if (iTempH == 5)
	{
		// 给模板数组赋值
		aValue[0] = dlgPara.m_f1;
		aValue[1] = dlgPara.m_f2;
		aValue[2] = dlgPara.m_f3;
		aValue[3] = dlgPara.m_f4;
		aValue[4] = dlgPara.m_f5;
		aValue[5] = dlgPara.m_f6;
		aValue[6] = dlgPara.m_f7;
		aValue[7] = dlgPara.m_f8;
		aValue[8] = dlgPara.m_f9;
		aValue[9] = dlgPara.m_f10;
		aValue[10] = dlgPara.m_f11;
		aValue[11] = dlgPara.m_f12;
		aValue[12] = dlgPara.m_f13;
		aValue[13] = dlgPara.m_f14;
		aValue[14] = dlgPara.m_f15;
		aValue[15] = dlgPara.m_f16;
		aValue[16] = dlgPara.m_f17;
		aValue[17] = dlgPara.m_f18;
		aValue[18] = dlgPara.m_f19;
		aValue[19] = dlgPara.m_f20;
		aValue[20] = dlgPara.m_f21;
		aValue[21] = dlgPara.m_f22;
		aValue[22] = dlgPara.m_f23;
		aValue[23] = dlgPara.m_f24;
		aValue[24] = dlgPara.m_f25;
	}
	//获取元素和
	for (int t = 0; t < iTempH * iTempW; t++)
	{
		elSum += aValue[t];
	}
	// 模板系数等于元素和的倒数
	fTempC = 1.0 / elSum;
	// 设定平移量（3*3）
	/*
	iTempMX = 1;
	iTempMY = 1;
	*/
	iTempMX = dlgPara.m_fcorx;
	iTempMY = dlgPara.m_fcory;
	// 更改光标形状
	BeginWaitCursor();

	// 调用Template()函数平滑DIB
	if (::Template(lpDIBBits, lSrcWidth, lSrcHeight,
		iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败");// 警告	
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}

void CMFCImg1View::OnMedian()
{
	// 中值滤波
	CMFCImg1Doc* pDoc = GetDocument();
	LPSTR lpDIB;
	LPSTR lpDIBBits;
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	//锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) return;
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	// 更改光标形状
	BeginWaitCursor();

	// 调用MedianFilter()函数
	if (::MedianFilter(lpDIBBits, lSrcWidth, lSrcHeight, 3))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败");// 警告	
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}

void CMFCImg1View::OnAddnoise()
{
	// 添加椒盐噪声
	CMFCImg1Doc* pDoc = GetDocument();
	LPSTR lpDIB;
	LPSTR lpDIBBits;
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	//锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) return;
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	// 随机设置某些像素的灰度值为0或255
	for (int y = 0; y < lSrcHeight; y++) {
		for (int x = 0; x < lSrcWidth; x++) {
			int r = rand() % 100;
			if (r < 5) { // 5% chance of setting pixel to 0
				lpDIBBits[y * lSrcWidth + x] = 0;
			}
			else if (r > 95) { // 5% chance of setting pixel to 255
				lpDIBBits[y * lSrcWidth + x] = 255;
			}
		}
	}
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


double generateGaussianNoise(double mean, double variance) {
	static std::default_random_engine generator;
	std::normal_distribution<double> distribution(mean, std::sqrt(variance));
	return  100* distribution(generator);
}

void CMFCImg1View::OnAddgnoise()
{
	CMFCImg1Doc* pDoc = GetDocument();
	if (!pDoc) return;

	LPSTR lpDIB;
	LPSTR lpDIBBits;
	long lSrcWidth; // 图象的宽度和高度
	long lSrcHeight;

	// 锁定 DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) {
		AfxMessageBox(L"无法锁定 DIB 对象！");
		return;
	}

	if (pDoc->m_dib.GetColorNum(lpDIB) != 256) {
		AfxMessageBox(L"对不起，不是 256 色位图！");
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
		return;
	}

	// 获取位图信息
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);

	// 定义高斯分布的参数
	const double mean = 0.0; // 分布的均值
	const double var = 0.01; // 分布的方差
	srand((unsigned)(time(NULL)));//调用srand是每次产生的随机数不同 

	for (int y = 0; y < lSrcHeight; y++) {
		for (int x = 0; x < lSrcWidth; x++) {
			// 从高斯分布中生成随机值
			double a = (generateGaussianNoise(mean, var));
			lpDIBBits[y * lSrcWidth + x] += (unsigned char)(generateGaussianNoise(mean, var));
		}
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 设置文档修改标志，更新视图
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);

}

void CMFCImg1View::OnLaplacian()
{
	// 拉普拉斯锐化
	CMFCImg1Doc* pDoc = GetDocument();
	LPSTR lpDIB;
	LPSTR lpDIBBits;
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	//模板高度
	int iTempH;
	//模板宽度
	int iTempW;
	//模板中心元素X坐标
	int iTempMX;
	//模板中心元素Y坐标
	int iTempMY;

	//锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) return;
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度
	CDlgLaplacian dlgPara;// 创建对话框	

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}

	//模板元素数组
	FLOAT aValue[9];
	for (int i = 0; i < 9; i++)
		aValue[i] = dlgPara.m_Value[i];

	// 设定平移量（3*3）
	iTempH = 3;
	iTempW = 3;
	iTempMX = 1;
	iTempMY = 1;
	float fTempC = 1;
	// 更改光标形状
	BeginWaitCursor();

	// 调用Template()函数锐化DIB
	if (::Template(lpDIBBits, lSrcWidth, lSrcHeight,
		iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败");// 警告	
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImg1View::OnSobel()
{
	// 边缘算子
	CMFCImg1Doc* pDoc = GetDocument();
	LPSTR lpDIB;
	LPSTR lpDIBBits;
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	//模板高度
	int iTempH;
	//模板宽度
	int iTempW;
	//模板中心元素X坐标
	int iTempMX;
	//模板中心元素Y坐标
	int iTempMY;
	//模板元素数组
	FLOAT fValue[9];
	//锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) return;
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	CDlgSobel dlgPara;
	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	int tempchoice = dlgPara.tempchoice;
	int direchoice = dlgPara.direchoice;
	if (tempchoice == 0 && direchoice == 0)//roberts x
	{
		FLOAT aValue[4] = { 1,0,0,-1 };
		iTempH = 2;
		iTempW = 2;
		iTempMX = 0;
		iTempMY = 0;
		for (int i = 0; i < 4; i++)
			fValue[i] = aValue[i];
	}
	else if (tempchoice == 0 && direchoice == 1)//roberts y
	{
		FLOAT aValue[4] = { 0,1,-1,0 };
		iTempH = 2;
		iTempW = 2;
		iTempMX = 0;
		iTempMY = 0;
		for (int i = 0; i < 4; i++)
			fValue[i] = aValue[i];
	}
	else if (tempchoice == 1 && direchoice == 0)//sobel x
	{
		FLOAT aValue[9] = { -1,0,1,-2,0,2,-1,0,1 };
		iTempH = 3;
		iTempW = 3;
		iTempMX = 1;
		iTempMY = 1;
		for (int i = 0; i < 9; i++)
			fValue[i] = aValue[i];
	}
	else//sobel y
	{
		FLOAT aValue[9] = { 1,2,1,0,0,0,-1,-2,-1 };
		iTempH = 3;
		iTempW = 3;
		iTempMX = 1;
		iTempMY = 1;
		for (int i = 0; i < 9; i++)
			fValue[i] = aValue[i];
	}
	float fTempC = 1;
	// 更改光标形状
	BeginWaitCursor();

	// 调用Template()函数实现边缘检测
	if (::Template(lpDIBBits, lSrcWidth, lSrcHeight,
		iTempH, iTempW, iTempMX, iTempMY, fValue, fTempC))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败");// 警告	
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}



void CMFCImg1View::OnMorphology()
{
	// 形态学运算
	CMFCImg1Doc* pDoc = GetDocument();	// 指向DIB的指针
	LPSTR	lpDIB;				// 指向DIB象素指针
	LPSTR   lpDIBBits;
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (!lpDIB) return;
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					

	//获取处理方式、结构元素类型、前景颜色
	int command;
	int nMode;
	int object;

	// 创建对话框
	CDlgMorpho dlgPara;

	// 初始化变量值
	//dlgPara.m_nMode = 0;

	// 显示对话框，提示用户设定腐蚀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	command = dlgPara.m_command;//0、膨胀 1、腐蚀 2、开启 3、闭合 4、内边界 5、外边界
	nMode = dlgPara.m_nmode;//0、水平 1、垂直 2、自定义
	object = dlgPara.m_object;//0、前景为黑色 1、前景为白色

	int structure[3][3];
	if (nMode == 2)
	{
		structure[0][0] = dlgPara.m_nStructure1;
		structure[0][1] = dlgPara.m_nStructure2;
		structure[0][2] = dlgPara.m_nStructure3;
		structure[1][0] = dlgPara.m_nStructure4;
		structure[1][1] = dlgPara.m_nStructure5;
		structure[1][2] = dlgPara.m_nStructure6;
		structure[2][0] = dlgPara.m_nStructure7;
		structure[2][1] = dlgPara.m_nStructure8;
		structure[2][2] = dlgPara.m_nStructure9;
	}

	// 删除对话框
	delete dlgPara;

	// 更改光标形状
	BeginWaitCursor();

	//设置更改状态
	int state=0;

	//根据命令进行相应函数调用
	switch (command) {
	case 0://膨胀
		state = DilationDIB(lpDIBBits, lSrcWidth, lSrcHeight, nMode, structure, object);
		break;
	case 1://腐蚀
		state = ErosionDIB(lpDIBBits, lSrcWidth, lSrcHeight, nMode, structure, object);
		break;
	case 2://开启
		state = OpenDIB(lpDIBBits, lSrcWidth, lSrcHeight, nMode, structure, object);
		break;
	case 3://闭合
		state = CloseDIB(lpDIBBits, lSrcWidth, lSrcHeight, nMode, structure, object);
		break;
	case 4://内边界
		state = I_BorderDIB(lpDIBBits, lSrcWidth, lSrcHeight, nMode, structure, object);
		break;
	case 5://外边界
		state = O_BorderDIB(lpDIBBits, lSrcWidth, lSrcHeight, nMode, structure, object);
		break;
	}
	
	if (state)
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败");// 警告	
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImg1View::OnThresholdtrans()
{
	// 灰度阈值变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgThresholdTrans dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnWnchange()
{
	// 窗口变换
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgWinChange dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnHisstretch()
{
	// 灰度拉伸
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgHisStretch dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnHisbalance()
{
	// 灰度直方图均衡化
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lSrcWidth * 8);

	// 指向源图像的指针
	unsigned char* lpSrc;
	//像素总数
	int sum = lSrcWidth * lSrcHeight;
	//灰度统计表
	int hist[256] = { 0 };
	//概率
	double pHist[256] = { 0 };
	//累积概率
	double disHist[256] = { 0 };
	//新的灰度映射表
	unsigned char newHist[256] = { 0 };

	for (size_t i = 0; i < lSrcHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lSrcWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * i + j;
			hist[*lpSrc]++;
		}
	}
	for (int i = 0; i < 256; i++) {
		pHist[i] = ((double)hist[i] / sum);
	}
	for (int i = 1; i < 256; i++) {
		disHist[i] = disHist[i - 1] + pHist[i];
	}
	for (int i = 0; i < 256; i++) {
		newHist[i] = (unsigned char)(255 * disHist[i] + 0.5);
	}
	for (size_t i = 0; i < lSrcHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lSrcWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * i + j;
			*lpSrc = newHist[*lpSrc];
		}
	}

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnTemplate()
{
	// 模板
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgTemplate dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnTranslation()
{
	// 图像平移
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgTranslation dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnRotate()
{
	// 图像旋转
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgRotate dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	HDIB hNewDIB = NULL;
	hNewDIB = dlgPara.hNewDIB;
	// 删除对话框
	delete dlgPara;

	if (hNewDIB != NULL) {
		pDoc->UpdateObject(hNewDIB);
		//设置文档修改标志
		pDoc->SetModifiedFlag(true);
		//更新视图
		pDoc->UpdateAllViews(NULL);
	}
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

void CMFCImg1View::OnMirror()
{
	// 图像镜像
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgMirror dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnImgcount()
{
	// 区域计数
	CMFCImg1Doc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	// 创建对话框
	CDlgAnalysis dlgPara;
	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针
	// 显示对话框
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 删除对话框
	delete dlgPara;
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImg1View::OnStylize()
{
	CMFCImg1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 24)// 判断是否是24-bpp位图
	{
		AfxMessageBox(L"对不起，不是24位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	CDlgStylize dlgPara;// 创建对话框	
	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针
	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	//修改完成后关闭对话框
	delete dlgPara;
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定	
}


void CMFCImg1View::OnCanny()
{
	CMFCImg1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	// 调用Canny边缘检测算法
	cannyEdgeDetection((unsigned char *)lpSrcStartBits, lSrcWidth, lSrcHeight);
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定	
}


void CMFCImg1View::OnUsm()
{
	CMFCImg1Doc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	float amount = 1.5;     // 锐化强度
	float threshold = 8.0; // 阈值
	// 执行USM锐化
	usmSharpening((unsigned char*)lpSrcStartBits, lSrcWidth, lSrcHeight, amount, threshold);
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定	
}
