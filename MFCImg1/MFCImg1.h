
// MFCImg1.h: MFCImg1 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CMFCImg1App:
// 有关此类的实现，请参阅 MFCImg1.cpp
//

class CMFCImg1App : public CWinApp
{
public:
	CMFCImg1App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();//初始化应用程序并创建主窗口
	virtual int ExitInstance();//释放资源并清理应用程序的状态

// 实现
	afx_msg void OnAppAbout();//消息处理函数,用来处理关于对话框的消息
	DECLARE_MESSAGE_MAP()//声明该类使用了MFC框架的消息映射机制
};

extern CMFCImg1App theApp;//当前应用程序的实例
