// CDlgTemplate.cpp: 实现文件
//

#include "pch.h"
#include "MFCImg1.h"
#include "afxdialogex.h"
#include "CDlgTemplate.h"
#include "function.h"
#include "CDlgPreview.h"
#include <fstream>

// CDlgTemplate 对话框

IMPLEMENT_DYNAMIC(CDlgTemplate, CDialogEx)

CDlgTemplate::CDlgTemplate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DlgTemplate, pParent)
	, m_t1(1)
	, m_t2(1)
	, m_t3(1)
	, m_t4(1)
	, m_t5(1)
	, m_t6(1)
	, m_t7(1)
	, m_t8(1)
	, m_t9(1)
	, m_coff(1)
	, m_thre(0)
	, m_edgeDire(0)
{

}

CDlgTemplate::~CDlgTemplate()
{
}

void CDlgTemplate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_T1, m_t1);
	DDX_Text(pDX, IDC_T2, m_t2);
	DDX_Control(pDX, IDC_T1, m_c1);
	DDX_Control(pDX, IDC_T2, m_c2);
	DDX_Control(pDX, IDC_T3, m_c3);
	DDX_Text(pDX, IDC_T3, m_t3);
	DDX_Control(pDX, IDC_T4, m_c4);
	DDX_Text(pDX, IDC_T4, m_t4);
	DDX_Control(pDX, IDC_T5, m_c5);
	DDX_Text(pDX, IDC_T5, m_t5);
	DDX_Control(pDX, IDC_T6, m_c6);
	DDX_Text(pDX, IDC_T6, m_t6);
	DDX_Control(pDX, IDC_T7, m_c7);
	DDX_Text(pDX, IDC_T7, m_t7);
	DDX_Control(pDX, IDC_T8, m_c8);
	DDX_Text(pDX, IDC_T8, m_t8);
	DDX_Control(pDX, IDC_T9, m_c9);
	DDX_Text(pDX, IDC_T9, m_t9);
	DDX_Control(pDX, IDC_COFF, m_cCoff);
	DDX_Text(pDX, IDC_COFF, m_coff);
	DDX_Control(pDX, IDC_THRE, m_cthre);
	DDX_Text(pDX, IDC_THRE, m_thre);
	DDX_Control(pDX, IDC_DIREFRAME, m_cDireFrame);
	DDX_Control(pDX, IDC_VERT, m_cVERT);
	DDX_Control(pDX, IDC_HORI, m_cHORI);
	DDX_Radio(pDX, IDC_VERT, m_edgeDire);
	DDX_Control(pDX, IDC_INVERSE, m_cInverse);
	DDX_Control(pDX, IDC_AVSM, m_cAvsm);
	DDX_Control(pDX, IDC_GSSM, m_cGssm);
	DDX_Control(pDX, IDC_SHAP, m_cShap);
	DDX_Control(pDX, IDC_RELI, m_cReli);
	DDX_Control(pDX, IDC_SOBEL, m_cSobel);
	DDX_Control(pDX, IDC_PREWITT, m_cPrewitt);
}


BEGIN_MESSAGE_MAP(CDlgTemplate, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgTemplate::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgTemplate::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_SAVEFILE, &CDlgTemplate::OnBnClickedSavefile)
	ON_BN_CLICKED(IDC_LOADFILE, &CDlgTemplate::OnBnClickedLoadfile)
	ON_BN_CLICKED(IDOK, &CDlgTemplate::OnBnClickedOk)
	ON_BN_CLICKED(IDC_INVERSE, &CDlgTemplate::OnBnClickedInverse)
	ON_BN_CLICKED(IDC_AVSM, &CDlgTemplate::OnBnClickedAvsm)
	ON_BN_CLICKED(IDC_GSSM, &CDlgTemplate::OnBnClickedGssm)
	ON_BN_CLICKED(IDC_SHAP, &CDlgTemplate::OnBnClickedShap)
	ON_BN_CLICKED(IDC_RELI, &CDlgTemplate::OnBnClickedReli)
	ON_BN_CLICKED(IDC_SOBEL, &CDlgTemplate::OnBnClickedSobel)
	ON_BN_CLICKED(IDC_PREWITT, &CDlgTemplate::OnBnClickedPrewitt)
	ON_BN_CLICKED(IDC_VERT, &CDlgTemplate::OnBnClickedVert)
	ON_BN_CLICKED(IDC_HORI, &CDlgTemplate::OnBnClickedHori)
	ON_BN_CLICKED(IDC_INPUTBT, &CDlgTemplate::OnBnClickedInputbt)
	ON_BN_CLICKED(IDC_BriBT, &CDlgTemplate::OnBnClickedBribt)
END_MESSAGE_MAP()


// CDlgTemplate 消息处理程序


BOOL CDlgTemplate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//复制图像到对话框
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	lpImg = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;

	edge = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTemplate::edgeDireShow()
{
	m_cDireFrame.ShowWindow(SW_SHOW);
	m_cVERT.ShowWindow(SW_SHOW);
	m_cHORI.ShowWindow(SW_SHOW);
}

void CDlgTemplate::edgeDireHide()
{
	m_cDireFrame.ShowWindow(SW_HIDE);
	m_cVERT.ShowWindow(SW_HIDE);
	m_cHORI.ShowWindow(SW_HIDE);

}
void CDlgTemplate::editOnlyRead()
{
	m_c1.SetReadOnly(TRUE);
	m_c2.SetReadOnly(TRUE);
	m_c3.SetReadOnly(TRUE);
	m_c4.SetReadOnly(TRUE);
	m_c5.SetReadOnly(TRUE);
	m_c6.SetReadOnly(TRUE);
	m_c7.SetReadOnly(TRUE);
	m_c8.SetReadOnly(TRUE);
	m_c9.SetReadOnly(TRUE);
	m_cCoff.SetReadOnly(TRUE);
	m_cthre.SetReadOnly(TRUE);
}

void CDlgTemplate::editEditable()
{
	m_c1.SetReadOnly(FALSE);
	m_c2.SetReadOnly(FALSE);
	m_c3.SetReadOnly(FALSE);
	m_c4.SetReadOnly(FALSE);
	m_c5.SetReadOnly(FALSE);
	m_c6.SetReadOnly(FALSE);
	m_c7.SetReadOnly(FALSE);
	m_c8.SetReadOnly(FALSE);
	m_c9.SetReadOnly(FALSE);
	m_cCoff.SetReadOnly(FALSE);
	m_cthre.SetReadOnly(FALSE);

}
void CDlgTemplate::OnBnClickedButton3()
{
	// 预览
	UpdateData(TRUE);
	float* fpArray = new float[9];
	fpArray[0] = m_t1;
	fpArray[1] = m_t2;
	fpArray[2] = m_t3;
	fpArray[3] = m_t4;
	fpArray[4] = m_t5;
	fpArray[5] = m_t6;
	fpArray[6] = m_t7;
	fpArray[7] = m_t8;
	fpArray[8] = m_t9;
	flag = Template(pBmpBuf, lSrcWidth, lSrcHeight, fpArray,m_coff , m_thre);
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


void CDlgTemplate::OnBnClickedButton4()
{
	// 恢复
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	memcpy(lpImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
}


void CDlgTemplate::OnBnClickedSavefile()
{
	// 保存算子
	// 创建保存文件对话框
	UpdateData(TRUE);
	CFileDialog fileDlg(FALSE); // FALSE表示保存文件对话框
	CString filter = _T("txt files (*.txt)|*.txt|All files (*.*)|*.*");
	fileDlg.m_ofn.lpstrFilter = filter;
	fileDlg.m_ofn.lpstrDefExt = _T("txt"); // 默认的文件扩展名
	INT_PTR result = fileDlg.DoModal();

	if (result == IDOK)
	{
		CString filePath = fileDlg.GetPathName(); // 获取用户选择的文件路径

		// 将内容保存到文件
		std::ofstream file(filePath);
		if (file.is_open())
		{
			file << m_t1 << " " << m_t2 << " " << m_t3 << "\n";
			file << m_t4 << " " << m_t5 << " " << m_t6 << "\n";
			file << m_t7 << " " << m_t8 << " " << m_t9 << "\n";
			file << m_coff << " " << m_thre << "\n";
			file.close();
		}
	}
}


void CDlgTemplate::OnBnClickedLoadfile()
{
	// 载入算子
	// 创建打开文件对话框
	CFileDialog fileDlg(TRUE); // TRUE表示打开文件对话框
	INT_PTR result = fileDlg.DoModal();

	if (result == IDOK)
	{
		CString filePath = fileDlg.GetPathName(); // 获取用户选择的文件路径

		// 读取文件内容并将值赋给变量
		std::ifstream file(filePath);
		if (file.is_open())
		{
			int value1, value2, value3;
			float coff;
			int thre;
			file >> value1 >> value2 >> value3;
			// 将读取的值赋给对应的变量，假设m_t1、m_t2、m_t3为对应的成员变量
			m_t1 = value1;
			m_t2 = value2;
			m_t3 = value3;

			// 继续读取和赋值其他变量的值
			file >> value1 >> value2 >> value3;
			m_t4 = value1;
			m_t5 = value2;
			m_t6 = value3;

			file >> value1 >> value2 >> value3;
			m_t7 = value1;
			m_t8 = value2;
			m_t9 = value3;

			file >> coff >> thre;
			m_coff = coff;
			m_thre = thre;
			file.close();
		}
	}

	UpdateData(FALSE);
}

void CDlgTemplate::OnBnClickedInputbt()
{
	// 自定义
	edgeDireHide();
	editEditable();
	m_cInverse.SetCheck(FALSE);
	m_cAvsm.SetCheck(FALSE);
	m_cGssm.SetCheck(FALSE);
	m_cShap.SetCheck(FALSE);
	m_cReli.SetCheck(FALSE);
	m_cSobel.SetCheck(FALSE);
	m_cPrewitt.SetCheck(FALSE);
	
}

void CDlgTemplate::OnBnClickedOk()
{
	// 应用更改
	if (flag) {
		int palettesize = 0;
		if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
		memcpy(lpSrcDib, lpImg, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
		pBmpBuf = lpImg + sizeof(BITMAPINFOHEADER) + palettesize;
	}
	CDialogEx::OnOK();
}


void CDlgTemplate::OnBnClickedInverse()
{
	// 反色
	editOnlyRead();
	edgeDireHide();//边缘方向选择隐藏
	edge = 0;//边缘检测关闭
	//载入模板
	m_t1 = 0;
	m_t2 = 0;
	m_t3 = 0;
	m_t4 = 0;
	m_t5 = -1;
	m_t6 = 0;
	m_t7 = 0;
	m_t8 = 0;
	m_t9 = 0;
	m_coff = 1;
	m_thre = 255;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedAvsm()
{
	// 平均模板
	editOnlyRead();
	edgeDireHide();
	edge = 0;//边缘检测关闭
	//载入模板
	m_t1 = 1;
	m_t2 = 1;
	m_t3 = 1;
	m_t4 = 1;
	m_t5 = 1;
	m_t6 = 1;
	m_t7 = 1;
	m_t8 = 1;
	m_t9 = 1;
	m_coff = 1.0/9;
	m_thre = 0;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedGssm()
{
	// 高斯模板
	editOnlyRead();
	edgeDireHide();
	edge = 0;//边缘检测关闭
	m_t1 = 1;
	m_t2 = 2;
	m_t3 = 1;
	m_t4 = 2;
	m_t5 = 4;
	m_t6 = 2;
	m_t7 = 1;
	m_t8 = 2;
	m_t9 = 1;
	m_coff = 1.0/ 16;
	m_thre = 0;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedShap()
{
	// 锐化
	editOnlyRead();
	edgeDireHide();
	edge = 0;//边缘检测关闭
	m_t1 = 0;
	m_t2 = -1;
	m_t3 = 0;
	m_t4 = -1;
	m_t5 = 5;
	m_t6 = -1;
	m_t7 = 0;
	m_t8 = -1;
	m_t9 = 0;
	m_coff = 1 ;
	m_thre = 0;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedReli()
{
	// 浮雕
	editOnlyRead();
	edgeDireHide();
	edge = 0;//边缘检测关闭
	m_t1 = -1;
	m_t2 = 0;
	m_t3 = 0;
	m_t4 = 0;
	m_t5 = 1;
	m_t6 = 0;
	m_t7 = 0;
	m_t8 = 0;
	m_t9 = 0;
	m_coff = 1;
	m_thre = 128;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedSobel()
{
	// sobel
	edgeDireShow();
	editOnlyRead();
	edge = 1;//边缘检测开启
	if (m_edgeDire == 0) {
		//sobel x
		m_t1 = -1;
		m_t2 = -2;
		m_t3 = -1;
		m_t4 = 0;
		m_t5 = 0;
		m_t6 = 0;
		m_t7 = 1;
		m_t8 = 2;
		m_t9 = 1;
	}
	else if (m_edgeDire == 1) 
	{
		//sobel x
		m_t1 = -1;
		m_t2 = 0;
		m_t3 = 1;
		m_t4 = -2;
		m_t5 = 0;
		m_t6 = 2;
		m_t7 = -1;
		m_t8 = 0;
		m_t9 = 1;
	}
	m_coff = 1;
	m_thre = 0;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedPrewitt()
{
	// prewitt
	edgeDireShow();
	editOnlyRead();
	edge = 2;//边缘检测开启
	if (m_edgeDire == 0) {
		//prewitt y
		m_t1 = -1;
		m_t2 = -1;
		m_t3 = -1;
		m_t4 = 0;
		m_t5 = 0;
		m_t6 = 0;
		m_t7 = 1;
		m_t8 = 1;
		m_t9 = 1;
	}
	else if (m_edgeDire == 1)
	{
		//prewitt x
		m_t1 = -1;
		m_t2 = 0;
		m_t3 = 1;
		m_t4 = -1;
		m_t5 = 0;
		m_t6 = 1;
		m_t7 = -1;
		m_t8 = 0;
		m_t9 = 1;
	}
	m_coff = 1;
	m_thre = 0;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedVert()
{
	m_edgeDire = 0;
	// 垂直方向
	if (edge == 1) 
		//sobel y
	{
		m_t1 = -1;
		m_t2 = -2;
		m_t3 = -1;
		m_t4 = 0;
		m_t5 = 0;
		m_t6 = 0;
		m_t7 = 1;
		m_t8 = 2;
		m_t9 = 1;
	}
	else if (edge == 2) 
		//prewitt y
	{
		m_t1 = -1;
		m_t2 = -1;
		m_t3 = -1;
		m_t4 = 0;
		m_t5 = 0;
		m_t6 = 0;
		m_t7 = 1;
		m_t8 = 1;
		m_t9 = 1;
	}
	m_coff = 1;
	m_thre = 0;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedHori()
{
	// 水平方向
	m_edgeDire = 1;
	if (edge == 1) 
		//sobel x
	{
		m_t1 = -1;
		m_t2 = 0;
		m_t3 = 1;
		m_t4 = -2;
		m_t5 = 0;
		m_t6 = 2;
		m_t7 = -1;
		m_t8 = 0;
		m_t9 = 1;
	}
	else if (edge == 2) 
		//prewitt x
	{
		m_t1 = -1;
		m_t2 = 0;
		m_t3 = 1;
		m_t4 = -1;
		m_t5 = 0;
		m_t6 = 1;
		m_t7 = -1;
		m_t8 = 0;
		m_t9 = 1;
	}
	m_coff = 1;
	m_thre = 0;
	UpdateData(FALSE);
}


void CDlgTemplate::OnBnClickedBribt()
{
	// 改变亮度
	edgeDireHide();
	m_c1.SetReadOnly(TRUE);
	m_c2.SetReadOnly(TRUE);
	m_c3.SetReadOnly(TRUE);
	m_c4.SetReadOnly(TRUE);
	m_c5.SetReadOnly(TRUE);
	m_c6.SetReadOnly(TRUE);
	m_c7.SetReadOnly(TRUE);
	m_c8.SetReadOnly(TRUE);
	m_c9.SetReadOnly(TRUE);
	m_cCoff.SetReadOnly(FALSE);
	m_cthre.SetReadOnly(FALSE);
	edge = 0;
	m_t1 = 0;
	m_t2 = 0;
	m_t3 = 0;
	m_t4 = 0;
	m_t5 = 1;
	m_t6 = 0;
	m_t7 = 0;
	m_t8 = 0;
	m_t9 = 0;
	m_coff = 1;
	m_thre = 50;
	UpdateData(FALSE);
}
