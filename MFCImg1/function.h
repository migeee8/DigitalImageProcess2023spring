#pragma once


#ifndef _INC_FUNCTIONAPI
#define _INC_FUNCTIONAPI
//����������ԵĽṹ��
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
// ����ԭ��
//�Ҷ���ֵ�任
BOOL WINAPI Binarization(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int threshold);
float Iteration(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float delta_T);
int Ostu(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);
float HisValley(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

BOOL WINAPI adaptiveThresholding(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int windowSize);

//���ڱ任
BOOL WINAPI Winchange(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int m_lowerLimit, int m_higherLimit);

//�Ҷ�����
BOOL WINAPI HisStretch(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int x1, int y1, int x2, int y2);

//ֱ��ͼ���⻯
BOOL WINAPI HisEquation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

//ģ�����
BOOL WINAPI Template(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float* fpArray, float fCoef, int fThre);

BOOL WINAPI cannyEdgeDetection(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

BOOL WINAPI usmSharpening(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float amount, float threshold);

//ͼ��ƽ��
BOOL WINAPI Translation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset);

//ͼ����
BOOL WINAPI Mirror(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int Dire);

//ͼ����ת
HGLOBAL WINAPI Rotate(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int angle);

//ͼ����
BOOL WINAPI Stylize1(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount);

BOOL WINAPI Stylize2(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount);

BOOL WINAPI Stylize3(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount);

//�Զ����ǰ����ɫ
int WINAPI AnalysisForeground(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);

//�������
int WINAPI countRegions(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color);

//�����������
ObjectProperties WINAPI calculateObjectProperties(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color, unsigned char* lpDstImageData);
#endif 