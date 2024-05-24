
// MFCImg1View.h: CMFCImg1View 类的接口
//

#pragma once


class CMFCImg1View : public CView
{
protected: // 仅从序列化创建
	CMFCImg1View() noexcept;
	DECLARE_DYNCREATE(CMFCImg1View)

// 特性
public:
	CMFCImg1Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCImg1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDispimginfo();
	afx_msg void OnAdjbright();
	afx_msg void OnRedo();
	afx_msg void On24colorto8color();
	afx_msg void On8colorto8gray();
	afx_msg void OnHalftone();
	afx_msg void OnPointinvert();
	afx_msg void OnVtrans();
	afx_msg void OnInvertedvtrans();
	afx_msg void OnLtrans();
	afx_msg void OnReverltrans();
	afx_msg void OnLineartrans();
	afx_msg void OnBitplane();
	afx_msg void OnGammatrans();
	afx_msg void OnLogtrans();
	afx_msg void OnGray2binary();
	afx_msg void OnDisphist();
	afx_msg void OnSmooth();
	afx_msg void OnEnhansmooth();
	afx_msg void OnAddnoise();
	afx_msg void OnLaplacian();
	afx_msg void OnSobel();
	afx_msg void OnAddgnoise();
	afx_msg void OnMedian();
	afx_msg void OnMorphology();
	afx_msg void OnThresholdtrans();
	afx_msg void OnWnchange();
	afx_msg void OnHisstretch();
	afx_msg void OnHisbalance();
	afx_msg void OnTemplate();
//	afx_msg void OnGeometric();
	afx_msg void OnTranslation();
	afx_msg void OnZoom();
	afx_msg void OnRotate();
	afx_msg void OnMirror();
	afx_msg void OnImgcount();
	afx_msg void OnStylize();
	afx_msg void OnCanny();
	afx_msg void OnUsm();
};

#ifndef _DEBUG  // MFCImg1View.cpp 中的调试版本
inline CMFCImg1Doc* CMFCImg1View::GetDocument() const
   { return reinterpret_cast<CMFCImg1Doc*>(m_pDocument); }
#endif

