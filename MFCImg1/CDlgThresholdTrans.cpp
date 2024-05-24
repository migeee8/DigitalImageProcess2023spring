// CDlgThresholdTrans.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgThresholdTrans.h"
#include "function.h"
#include "CDlgPreview.h"


// CDlgThresholdTrans 对话框

IMPLEMENT_DYNAMIC(CDlgThresholdTrans, CDialogEx)

CDlgThresholdTrans::CDlgThresholdTrans(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgThresholdTrans, pParent)
	, m_threshold(127)
{

}

CDlgThresholdTrans::~CDlgThresholdTrans()
{
}

void CDlgThresholdTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Threshold, m_threshold);
	DDX_Control(pDX, IDC_AlgBt1, m_radioBt1);
	DDX_Control(pDX, IDC_AlgBt2, m_radioBt2);
	DDX_Control(pDX, IDC_AlgBt3, m_radioBt3);
}


BEGIN_MESSAGE_MAP(CDlgThresholdTrans, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ThresholdCalBt, &CDlgThresholdTrans::OnBnClickedThresholdcalbt)
	ON_BN_CLICKED(IDC_PrevBt, &CDlgThresholdTrans::OnBnClickedPrevbt)
	ON_BN_CLICKED(IDC_RestBt, &CDlgThresholdTrans::OnBnClickedRestbt)
	ON_BN_CLICKED(IDOK, &CDlgThresholdTrans::OnBnClickedOk)
	ON_BN_CLICKED(IDC_AlgBt1, &CDlgThresholdTrans::OnBnClickedAlgbt1)
	ON_BN_CLICKED(IDC_AlgBt2, &CDlgThresholdTrans::OnBnClickedAlgbt2)
	ON_BN_CLICKED(IDC_AlgBt3, &CDlgThresholdTrans::OnBnClickedAlgbt3)
	ON_EN_KILLFOCUS(IDC_Threshold, &CDlgThresholdTrans::OnEnKillfocusThreshold)
	ON_EN_SETFOCUS(IDC_Threshold, &CDlgThresholdTrans::OnEnSetfocusThreshold)
	ON_BN_CLICKED(IDC_AdaptiveThre, &CDlgThresholdTrans::OnBnClickedAdaptivethre)
END_MESSAGE_MAP()


// CDlgThresholdTrans 消息处理程序


BOOL CDlgThresholdTrans::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//复制图像到对话框
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	lpImg = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;

	//在IDC_COORD上绘制
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	//计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	//设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;

	//初始化拖动状态
	m_bIsDrawing = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgThresholdTrans::OnEnKillfocusThreshold()
{
	UpdateData(TRUE);
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgThresholdTrans::setRadioUncheck() {
	m_radioBt1.SetCheck(BST_UNCHECKED);
	m_radioBt2.SetCheck(BST_UNCHECKED);
	m_radioBt3.SetCheck(BST_UNCHECKED);
}

void CDlgThresholdTrans::OnEnSetfocusThreshold()
{
	setRadioUncheck();
}

void CDlgThresholdTrans::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	//获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	CString str;
	CPoint pLeft, pRight, pCenterTop, pCenterBottom;
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	pDC->Rectangle(0, 0, 330, 300);
	//创建画笔对象
	CPen* pPenRed = new CPen;
	//红色画笔，红绿蓝
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	//创建画笔对象
	CPen* pPenBlue = new CPen;
	//蓝色画笔，红绿蓝
	pPenBlue->CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	//创建画笔对象
	CPen* pPenGreen = new CPen;
	//绿色画笔，红绿蓝
	pPenGreen->CreatePen(PS_DOT, 1, RGB(0, 255, 0));
	//选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	//绘制坐标轴
	pDC->MoveTo(10, 10);
	//垂直轴
	pDC->LineTo(10, 280);
	//水平轴
	pDC->LineTo(320, 280);
	//写坐标
	str.Format(L"0");
	pDC->TextOut(10, 281, str);
	str.Format(L"255");
	pDC->TextOut(265, 281, str);
	pDC->TextOutW(11, 25, str);
	//绘制X箭头
	pDC->LineTo(315, 275);
	pDC->MoveTo(320, 280);
	pDC->LineTo(315, 285);
	//绘制Y箭头
	pDC->MoveTo(10, 10);
	pDC->LineTo(5, 15);
	pDC->MoveTo(10, 10);
	pDC->LineTo(15, 15);

	//蓝色画笔
	pDC->SelectObject(pPenBlue);
	pCenterTop.x = 10 + m_threshold;//128;
	pCenterTop.y = 25;

	pCenterBottom.x = 10 + m_threshold;//128;
	pCenterBottom.y = 280;

	pLeft.x = 10;
	pLeft.y = 280;

	pRight.x = 265;
	pRight.y = 25;

	pDC->MoveTo(pCenterTop);
	pDC->LineTo(pCenterBottom);
	
	pDC->MoveTo(pLeft);
	pDC->LineTo(pCenterBottom);

	pDC->MoveTo(pCenterTop);
	pDC->LineTo(pRight);
}


void CDlgThresholdTrans::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 当用户单击鼠标左键开始拖动
	//判断是否在有效区域
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_threshold))
		{
			//设置拖动状态
			m_bIsDrawing = TRUE;
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	//默认
	CDialogEx::OnLButtonDown(nFlags, point);

}


void CDlgThresholdTrans::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 当用户释放鼠标左键停止拖动
	if (m_bIsDrawing)
	{
		//重新设置拖动状态
		m_bIsDrawing = FALSE;
	}
	//默认
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgThresholdTrans::OnMouseMove(UINT nFlags, CPoint point)
{
	//  判断当前光标是否在绘制区域
	if (m_MouseRect.PtInRect(point))
	{
		//判断是否在拖动
		if (m_bIsDrawing)
		{
			setRadioUncheck();
			//更改阈值
			m_threshold = (BYTE)(point.x - m_MouseRect.left);
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			//更新
			UpdateData(FALSE);
			//重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_threshold))
		{
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgThresholdTrans::OnBnClickedThresholdcalbt()
{
	// 根据算法计算阈值
	switch (m_alg) {
	case(0):
		m_threshold = Iteration(pBmpBuf, lSrcWidth, lSrcHeight, 0.02);
		break;
	case(1):
		m_threshold = Ostu(pBmpBuf, lSrcWidth, lSrcHeight);
		break;
	case(2):
		m_threshold = HisValley(pBmpBuf, lSrcWidth, lSrcHeight);
		break;
	}
	InvalidateRect(m_MouseRect, TRUE);
	UpdateData(FALSE);
}


void CDlgThresholdTrans::OnBnClickedPrevbt()
{
	// 更改并预览
	UpdateData(TRUE);
	flag = Binarization(pBmpBuf,lSrcWidth, lSrcHeight,m_threshold);
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

void CDlgThresholdTrans::OnBnClickedAdaptivethre()
{
	// 自适应
	UpdateData(TRUE);
	flag = adaptiveThresholding(pBmpBuf, lSrcWidth, lSrcHeight,100);
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


void CDlgThresholdTrans::OnBnClickedRestbt()
{
	// 恢复
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;

}


void CDlgThresholdTrans::OnBnClickedOk()
{
	// 应用更改
	//修改位为1，将对话框中修改过的图像副本复制给源图像
	if (flag) {
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		memcpy( lpSrcDib, lpImg , sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	CDialogEx::OnOK();
}


void CDlgThresholdTrans::OnBnClickedAlgbt1()
{
	m_alg = 0;
}


void CDlgThresholdTrans::OnBnClickedAlgbt2()
{
	m_alg = 1;
}


void CDlgThresholdTrans::OnBnClickedAlgbt3()
{
	m_alg = 2;
}



