#pragma once
#include "afxdialogex.h"


// CDlgTemplate 对话框

class CDlgTemplate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTemplate)

public:
	CDlgTemplate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgTemplate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DlgTemplate };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int flag;//修改位
	int edge;//0为边缘检测关闭，1为sobel算子选中，2为prewitt算子选中

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针

	unsigned char* pBmpBuf;		//指向对话框中副本像素的指针
	unsigned char* lpImg;	//指向对话框中副本图像的指针

	int m_t1;
	int m_t2;
	CEdit m_c1;
	CEdit m_c2;
	CEdit m_c3;
	int m_t3;
	CEdit m_c4;
	int m_t4;
	CEdit m_c5;
	int m_t5;
	CEdit m_c6;
	int m_t6;
	CEdit m_c7;
	int m_t7;
	CEdit m_c8;
	int m_t8;
	CEdit m_c9;
	int m_t9;
	CEdit m_cCoff;
	float m_coff;
	CEdit m_cthre;
	int m_thre;

	void CDlgTemplate::editOnlyRead();
	void CDlgTemplate::editEditable();


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedSavefile();
	afx_msg void OnBnClickedLoadfile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedInverse();
	afx_msg void OnBnClickedAvsm();
	afx_msg void OnBnClickedGssm();
	afx_msg void OnBnClickedShap();
	afx_msg void OnBnClickedReli();
	afx_msg void OnBnClickedSobel();
	afx_msg void OnBnClickedPrewitt();
	afx_msg void OnBnClickedVert();
	afx_msg void OnBnClickedHori();

	void CDlgTemplate::edgeDireShow();
	void CDlgTemplate::edgeDireHide();
	CStatic m_cDireFrame;
	CButton m_cVERT;
	CButton m_cHORI;
	int m_edgeDire;
	afx_msg void OnBnClickedInputbt();
	CButton m_cInverse;
	CButton m_cAvsm;
	CButton m_cGssm;
	CButton m_cShap;
	CButton m_cReli;
	CButton m_cSobel;
	CButton m_cPrewitt;
	afx_msg void OnBnClickedBribt();
};
