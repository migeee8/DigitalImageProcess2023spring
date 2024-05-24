// CDlgHisStretch.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgHisStretch.h"
#include "CDlgPreview.h"
#include "function.h"


// CDlgHisStretch 对话框

IMPLEMENT_DYNAMIC(CDlgHisStretch, CDialogEx)

CDlgHisStretch::CDlgHisStretch(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgHisStretch, pParent)
	, m_x1(50)
	, m_y1(30)
	, m_x2(200)
	, m_y2(220)
{

}

CDlgHisStretch::~CDlgHisStretch()
{
}

void CDlgHisStretch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITX1, m_x1);
	DDX_Text(pDX, IDC_EDITY1, m_y1);
	DDX_Text(pDX, IDC_EDITX2, m_x2);
	DDX_Text(pDX, IDC_EDITY2, m_y2);
}


BEGIN_MESSAGE_MAP(CDlgHisStretch, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDITX1, &CDlgHisStretch::OnEnKillfocusEditx1)
	ON_EN_KILLFOCUS(IDC_EDITX2, &CDlgHisStretch::OnEnKillfocusEditx2)
	ON_EN_KILLFOCUS(IDC_EDITY1, &CDlgHisStretch::OnEnKillfocusEdity1)
	ON_EN_KILLFOCUS(IDC_EDITY2, &CDlgHisStretch::OnEnKillfocusEdity2)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
//	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_PrevBt, &CDlgHisStretch::OnBnClickedPrevbt)
	ON_BN_CLICKED(IDC_RstBt, &CDlgHisStretch::OnBnClickedRstbt)
	ON_BN_CLICKED(IDOK, &CDlgHisStretch::OnBnClickedOk)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgHisStretch 消息处理程序


BOOL CDlgHisStretch::OnInitDialog()
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
	CWnd* pWnd = GetDlgItem(IDC_COORD1);
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
	m_iIsDraging = 0;

	return TRUE; 
}


void CDlgHisStretch::OnEnKillfocusEditx1()
{
	// 更新值
	UpdateData(TRUE);
	//判断是否下限超过上限
	if (m_x1 > m_x2) {
		//交换
		int bTemp = m_x1;
		m_x1 = m_x2;
		m_x2 = bTemp;
		bTemp = m_y1;
		m_y1 = m_y2;
		m_y2 = bTemp;
		//更新到控件
		UpdateData(FALSE);
	}
	if (m_x1 <= 0 ) {
		m_x1 = 1;
		UpdateData(FALSE);
	}
	//重绘
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgHisStretch::OnEnKillfocusEditx2()
{
	// 更新值
	UpdateData(TRUE);
	//判断是否下限超过上限
	if (m_x1 > m_x2) {
		//交换
		int bTemp = m_x1;
		m_x1 = m_x2;
		m_x2 = bTemp;
		bTemp = m_y1;
		m_y1 = m_y2;
		m_y2 = bTemp;
		//更新到控件
		UpdateData(FALSE);
	}
	if (m_x2 >= 255) {
		m_x2 = 254;
		UpdateData(FALSE);
	}
	//重绘
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgHisStretch::OnEnKillfocusEdity1()
{
	// 更新值
	UpdateData(TRUE);
	if (m_y1 <= 0) {
		m_y1 = 1;
		UpdateData(FALSE);
	}
	if (m_y1 >= 255) {
		m_y1 = 254;
		UpdateData(FALSE);
	}
	//重绘
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgHisStretch::OnEnKillfocusEdity2()
{
	// 更新值
	UpdateData(TRUE);
	if (m_y2 <= 0) {
		m_y2 = 1;
		UpdateData(FALSE);
	}
	if (m_y2 >= 255) {
		m_y2 = 254;
		UpdateData(FALSE);
	}
	//重绘
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgHisStretch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD1);
	CString str;
	CPoint pLower,pHigher;
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
	pLower.x = 10 + m_x1;
	pLower.y = 280 - m_y1;

	pHigher.x = 10 + m_x2;
	pHigher.y = 280 - m_y2;

	pDC->MoveTo(10,280);
	pDC->LineTo(pLower);
	pDC->LineTo(pHigher);
	pDC->LineTo(265,25);
	
	str.Format(L"(%d, %d)", m_x1, m_y1);
	pDC->TextOut(m_x1+10, 281 - m_y1, str);
	str.Format(L"(%d, %d)", m_x2, m_y2);
	pDC->TextOut(m_x2 + 10, 281 - m_y2, str);

	//绘制点边缘的小矩形
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 255, 0));

	//选中刷子
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(m_x1 + 10 - 2, 280 - m_y1 - 2, m_x1 + 12, 280 - m_y1 + 2);
	pDC->Rectangle(m_x2 + 10 - 2, 280 - m_y2 - 2, m_x2 + 12, 280 - m_y2 + 2);

	//恢复以前的画笔
	pDC->SelectObject(pOldPen);

	//绘制边缘
	pDC->MoveTo(10, 25);
	pDC->LineTo(265, 25);
	pDC->LineTo(265, 280);
}


void CDlgHisStretch::OnLButtonDown(UINT nFlags, CPoint point)
{
	//判断鼠标单击是否在区域内
	if (m_MouseRect.PtInRect(point))
	{
		CRect rectTemp;

		//计算点1临近区域
		rectTemp.left = m_MouseRect.left + m_x1 - 2;
		rectTemp.right = m_MouseRect.left + m_x1 + 2;
		rectTemp.top = 255 + m_MouseRect.top - m_y1 - 2;
		rectTemp.bottom = 255 + m_MouseRect.top - m_y1 + 2;
		//如果在1邻近区域
		if (rectTemp.PtInRect(point))
		{
			m_iIsDraging = 1;
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			//计算点2临近区域
			rectTemp.left = m_MouseRect.left + m_x2 - 2;
			rectTemp.right = m_MouseRect.left + m_x2 + 2;
			rectTemp.top = 255 + m_MouseRect.top - m_y2 - 2;
			rectTemp.bottom = 255 + m_MouseRect.top - m_y2 + 2;
			//如果在1邻近区域
			if (rectTemp.PtInRect(point))
			{
				m_iIsDraging = 2;
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgHisStretch::OnLButtonUp(UINT nFlags, CPoint point)
{
	//当用户释放鼠标左键停止拖动
		if (m_iIsDraging != 0)
		{
			//重新设置拖动状态
				m_iIsDraging = 0;
		}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgHisStretch::OnMouseMove(UINT nFlags, CPoint point)
{
	//  判断当前光标是否在绘制区域
	if (m_MouseRect.PtInRect(point)) {
		if (m_iIsDraging != 0 )
		{
			if (m_iIsDraging == 1)
			{
				//拖动点1
				//判断下限小于上限
				if (point.x - m_MouseRect.left < m_x2 && point.x - m_MouseRect.left > 0 )
				{
					m_x1 = (BYTE)(point.x - m_MouseRect.left);
				}
				else if (point.x - m_MouseRect.left <= 0)
				{
					m_x1 = 1;
				}
				else
				{
					m_x1 = m_x2 - 1;
				}
				m_y1 = (BYTE)(255 + m_MouseRect.top - point.y);
			}
			else 
			{
				//拖动点2
				if (point.x - m_MouseRect.left > m_x1 && point.x - m_MouseRect.left < 255)
				{
					m_x2 = (BYTE)(point.x - m_MouseRect.left);
				}
				else if (point.x - m_MouseRect.left >= 255)
				{
					m_x2 = 254;
				}
				else
				{
					m_x2 = m_x1 + 1;
				}
				m_y2 = (BYTE)(255 + m_MouseRect.top - point.y);
			}
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			UpdateData(FALSE);
			InvalidateRect(m_MouseRect, TRUE);
		}
		else 
		{
			CRect rectTemp1;
			CRect rectTemp2;
			//计算点1临近区域
			rectTemp1.left = m_MouseRect.left + m_x1 - 2;
			rectTemp1.right = m_MouseRect.left + m_x1 + 2;
			rectTemp1.top = 255 + m_MouseRect.top - m_y1 - 2;
			rectTemp1.bottom = 255 + m_MouseRect.top - m_y1 + 2;
			//计算点2临近区域
			rectTemp2.left = m_MouseRect.left + m_x2 - 2;
			rectTemp2.right = m_MouseRect.left + m_x2 + 2;
			rectTemp2.top = 255 + m_MouseRect.top - m_y2 - 2;
			rectTemp2.bottom = 255 + m_MouseRect.top - m_y2 + 2;

			//鼠标移动到点1或点2旁边
			if (rectTemp1.PtInRect(point) || rectTemp2.PtInRect(point)) 
			{
				//更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgHisStretch::OnBnClickedPrevbt()
{
	UpdateData(TRUE);
	flag = HisStretch(pBmpBuf, lSrcWidth, lSrcHeight,m_x1, m_y1, m_x2, m_y2);
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


void CDlgHisStretch::OnBnClickedRstbt()
{
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
}


void CDlgHisStretch::OnBnClickedOk()
{
	if (flag) {
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		memcpy(lpSrcDib, lpImg, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	CDialogEx::OnOK();
}


