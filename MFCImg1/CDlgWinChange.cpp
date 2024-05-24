// CDlgWinChange.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgWinChange.h"
#include "Point.h"
#include "CDlgPreview.h"
#include "function.h"


// CDlgWinChange 对话框

IMPLEMENT_DYNAMIC(CDlgWinChange, CDialogEx)

CDlgWinChange::CDlgWinChange(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgWinChange, pParent)
	, m_lowerLimit(100)
	, m_higherLimit(200)
{

}

CDlgWinChange::~CDlgWinChange()
{
}

void CDlgWinChange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_L_Limit, m_lowerLimit);
	DDX_Text(pDX, IDC_H_Limit, m_higherLimit);
}


BEGIN_MESSAGE_MAP(CDlgWinChange, CDialogEx)
	ON_EN_KILLFOCUS(IDC_H_Limit, &CDlgWinChange::OnEnKillfocusHLimit)
	ON_EN_KILLFOCUS(IDC_L_Limit, &CDlgWinChange::OnEnKillfocusLLimit)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PrevBt, &CDlgWinChange::OnBnClickedPrevbt)
	ON_BN_CLICKED(IDC_RstBt, &CDlgWinChange::OnBnClickedRstbt)
	ON_BN_CLICKED(IDOK, &CDlgWinChange::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgWinChange 消息处理程序


BOOL CDlgWinChange::OnInitDialog()
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
	m_draggedControlID = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgWinChange::OnEnKillfocusHLimit()
{
	UpdateData(TRUE);
	if (m_higherLimit >= 255) m_higherLimit = 254;
	if (m_higherLimit <= m_lowerLimit ) m_higherLimit = m_lowerLimit + 1;
	UpdateData(FALSE);
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgWinChange::OnEnKillfocusLLimit()
{
	UpdateData(TRUE);
	if (m_lowerLimit <= 0) m_lowerLimit = 1;
	if (m_lowerLimit >= m_higherLimit) m_lowerLimit = m_higherLimit - 1;
	UpdateData(FALSE);
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgWinChange::OnMouseMove(UINT nFlags, CPoint point)
{
	//  判断当前光标是否在绘制区域
	if (m_MouseRect.PtInRect(point)) {
		if (m_bIsDrawing)
		{
			if (m_draggedControlID == IDC_L_Limit)
			{
				// 更新 lowerlimit 的值
				m_lowerLimit = (BYTE)(point.x - m_MouseRect.left);
				// 限制 lowerlimit 始终小于 higherlimit
				if (m_lowerLimit >= m_higherLimit)
				{
					m_lowerLimit = m_higherLimit - 1;
				}
				if (m_lowerLimit <= 0)
				{
					m_lowerLimit = 1;
				}
			}
			else if (m_draggedControlID == IDC_H_Limit)
			{
				// 更新 higherlimit 的值
				m_higherLimit = (BYTE)(point.x - m_MouseRect.left);
				// 限制 lowerlimit 始终小于 higherlimit
				if (m_higherLimit <= m_lowerLimit)
				{
					m_higherLimit = m_lowerLimit + 1;
				}
				if (m_higherLimit >= 255)
				{
					m_higherLimit = 254;
				}
			}
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			//更新
			UpdateData(FALSE);
			//重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_lowerLimit))
		{
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_higherLimit))
		{
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgWinChange::OnLButtonDown(UINT nFlags, CPoint point)
{
	 //当用户单击鼠标左键
	//判断是否在有效区域
	if (m_MouseRect.PtInRect(point)) {
		if (point.x == (m_MouseRect.left + m_lowerLimit))
		{
			// 鼠标点击了 lowerlimit 
			m_bIsDrawing = TRUE;
			m_draggedControlID = IDC_L_Limit;
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_higherLimit))
		{
			// 鼠标点击了 higherlimit 
			m_bIsDrawing = TRUE;
			m_draggedControlID = IDC_H_Limit;
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgWinChange::OnLButtonUp(UINT nFlags, CPoint point)
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


void CDlgWinChange::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	CString str;
	CPoint pLeft, pRight, pLowerTop, pLowerBottom, pHigherTop, pHigherBottom;
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
	pLowerTop.x = 10 + m_lowerLimit;
	pLowerTop.y = 280 - m_lowerLimit;

	pLowerBottom.x = 10 + m_lowerLimit;
	pLowerBottom.y = 280;

	pHigherTop.x = 10 + m_higherLimit;
	pHigherTop.y = 25;

	pHigherBottom.x = 10 + m_higherLimit;
	pHigherBottom.y = 280 - m_higherLimit;

	pLeft.x = 10;
	pLeft.y = 280;

	pRight.x = 265;
	pRight.y = 25;

	pDC->MoveTo(pLeft);
	pDC->LineTo(pLowerBottom);
	pDC->LineTo(pLowerTop);
	pDC->LineTo(pHigherBottom);
	pDC->LineTo(pHigherTop);
	pDC->LineTo(pRight);
}


void CDlgWinChange::OnBnClickedPrevbt()
{
	UpdateData(TRUE);
	flag = Winchange(pBmpBuf, lSrcWidth, lSrcHeight, m_lowerLimit, m_higherLimit);
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


void CDlgWinChange::OnBnClickedRstbt()
{
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
}


void CDlgWinChange::OnBnClickedOk()
{
	if (flag) {
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		memcpy(lpSrcDib, lpImg, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	CDialogEx::OnOK();
}
