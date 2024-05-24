#pragma once


#ifndef _INC_FUNCTIONAPI
#define _INC_FUNCTIONAPI
//最大物体属性的结构体
struct ObjectProperties {
	int area;
	int perimeter;
	double circularity;
	double rectangularity;
	int centerX;
	int centerY;
	int startX;
	int startY;
};
// 函数原型
//灰度阈值变换
BOOL WINAPI Binarization(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int threshold);
float Iteration(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float delta_T);
int Ostu(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);
float HisValley(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

BOOL WINAPI adaptiveThresholding(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int windowSize);

//窗口变换
BOOL WINAPI Winchange(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int m_lowerLimit, int m_higherLimit);

//灰度拉伸
BOOL WINAPI HisStretch(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int x1, int y1, int x2, int y2);

//直方图均衡化
BOOL WINAPI HisEquation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

//模板操作
BOOL WINAPI Template(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float* fpArray, float fCoef, int fThre);

BOOL WINAPI cannyEdgeDetection(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

BOOL WINAPI usmSharpening(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float amount, float threshold);

//图像平移
BOOL WINAPI Translation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset);

//图像镜像
BOOL WINAPI Mirror(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int Dire);

//图像旋转
HGLOBAL WINAPI Rotate(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int angle);

//图像风格化
BOOL WINAPI Stylize1(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount);

BOOL WINAPI Stylize2(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount);

BOOL WINAPI Stylize3(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount);

//自动监测前景颜色
int WINAPI AnalysisForeground(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

//区域计数
int WINAPI countRegions(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color);

//最大物体特征
ObjectProperties WINAPI calculateObjectProperties(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color, unsigned char* lpDstImageData);
#endif 