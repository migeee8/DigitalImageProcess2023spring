#pragma once
#include "afxdialogex.h"


// CDlgThresholdTrans 对话框

class CDlgThresholdTrans : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThresholdTrans)

public:
	CDlgThresholdTrans(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgThresholdTrans();
	//响应鼠标的区域
	CRect m_MouseRect;
	//标识是否拖动
	BOOL m_bIsDrawing;

// 对话框数据
#ifdef AFX_DESIGN_TIMEs
	enum { IDD = IDD_DlgThresholdTrans };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_threshold;//阈值
	int flag;//修改位
	int m_alg;

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针

	unsigned char* pBmpBuf;		//指向对话框中副本像素的指针
	unsigned char* lpImg;	//指向对话框中副本图像的指针

	void CDlgThresholdTrans::setRadioUncheck();//取消radio button的选择

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedThresholdcalbt();
	afx_msg void OnBnClickedPrevbt();
	afx_msg void OnBnClickedRestbt();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAlgbt1();
	afx_msg void OnBnClickedAlgbt2();
	afx_msg void OnBnClickedAlgbt3();
	afx_msg void OnEnKillfocusThreshold();
	afx_msg void OnEnSetfocusThreshold();

	CButton m_radioBt1;
	CButton m_radioBt2;
	CButton m_radioBt3;
	afx_msg void OnBnClickedAdaptivethre();
	afx_msg void OnBnClickedButton2();
};
