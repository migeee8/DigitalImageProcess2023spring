
/**************************************************************************
 *  文件名：ThresholdAlg.cpp
 *
 *  灰度阈值变换函数库：
 *
 *  Binarization()		-二值化
 *
 *************************************************************************/
#include <opencv2/opencv.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc.hpp>  
#include "stdafx.h"
#include "function.h"
#include "DIBAPI.h"
#include"transformula.h"
#include <math.h>
#include <utility>
#include <direct.h>
#include <cmath>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>

using namespace cv;

#define  PI 3.1415926
unsigned char Limit(int value)
{
	return value > 255 ? 255 : value < 0 ? 0 : static_cast<unsigned char>(value + 0.5);
}
//给定两点返回斜率和截距
std::pair<float, float> calculateLine(float x1, float y1, float x2, float y2) {
	float slope = (y2 - y1) / (x2 - x1);
	float y_intercept = y1 - slope * x1;
	return std::make_pair(slope, y_intercept);
}
/*************************************************************************
 *
 * 函数名称：
 *   Binarization()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   int threshold		- 二值图像的阈值参数
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来转换8位灰度图像到二值图像
 *
 ************************************************************************/
BOOL WINAPI Binarization(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int threshold)
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	//逐行扫描
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			if (*lpSrc < threshold) *lpSrc = (unsigned char)0;
			else *lpSrc = (unsigned char)255;
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   Iteration()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   float delta_T		- 迭代算法的参数
 *
 * 返回值:
 *   float               - 成功返回算出的阈值T
 *
 * 说明:
 *   该函数为灰度阈值变换的迭代法实现
 *
 ************************************************************************/
float Iteration(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float delta_T)
{
	//求最大灰度和最小灰度
	int sum_his = 0;
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	for (size_t j = 0; j < lHeight; j++)
	{
		for (size_t i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			sum_his += *lpSrc;		
		}
	}
	float old_T = sum_his * 1.0 / (lHeight * lWidth);
	float new_T = 0;
	float dT = new_T - old_T;
	while (abs(dT) > delta_T)
	{
		int G1 = 0;
		int G2 = 0;
		int timer_G1 = 0;
		int timer_G2 = 0;
		for (size_t j = 0; j < lHeight; j++)
		{
			for (size_t i = 0; i < lWidth; i++)
			{
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
				if(*lpSrc> old_T)
				{
					G1 += *lpSrc;
					timer_G1++;
				}
				else {
					G2 += *lpSrc;
					timer_G2++;
				}
			}
		}
		new_T = 0.5 * ((G1 * 1.0f / timer_G1) + (G2 * 1.0f / timer_G2));
		dT = new_T - old_T;
		old_T = new_T;
	}
	return new_T;
}

/*************************************************************************
 *
 * 函数名称：
 *   Ostu()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   float delta_T		- 迭代算法的参数
 *
 * 返回值:
 *   int               - 成功返回算出的阈值T
 *
 * 说明:
 *   该函数为灰度阈值变化的大津法实现
 *
 ************************************************************************/
int Ostu(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	int T = 0; //大津阈值
	int size = lHeight * lWidth;
	float variance;   //类间方差
	float maxVariance = 0, w1 = 0, w2 = 0, avgValue = 0;
	float u0 = 0, u1 = 0, u2 = 0;
	//生成灰度直方图
	int pixels[256];
	float histgram[256];
	for (int i = 0; i < 256; i++)
	{
		pixels[i] = 0;
	}
	for (size_t j = 0; j < lHeight; j++)
	{
		for (size_t i = 0; i < lWidth; i++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			pixels[*lpSrc]++;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		histgram[i] = pixels[i] * 1.0f / size;
	}
	//遍历找出类间方差最大（maxVariance）的阈值（Ostu_Threshold）
	for (int i = 0; i <= 255; i++)
	{
		w1 = 0;
		w2 = 0;
		u1 = 0;
		u2 = 0;
		//计算背景像素占比,平均灰度
		for (int j = 0; j <= i; j++)
		{
			w1 += histgram[j];
			u1 += histgram[j] * j;
		}
		u1 = u1 / w1;
		//计算前景像素占比,平均灰度
		w2 = 1 - w1;
		if (i == 255)
		{
			u2 = 0;
		}
		else
		{
			for (int j = i + 1; j <= 255; j++)
			{
				u2 += histgram[j] * j;
			}
		}
		u2 = u2 / w2;
		//计算类间方差
		variance = w1 * w2 * (u1 - u2) * (u1 - u2);
		if (variance > maxVariance)
		{ //找到使灰度差最大的值
			maxVariance = variance;
			T = i;            //那个值就是阈值
		}
	}
	return T;
}

/*************************************************************************
 *
 * 函数名称：
 *   HisValley()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *
 * 返回值:
 *   float               - 成功返回算出的阈值T
 *
 * 说明:
 *   该函数为灰度阈值变换的灰度图波谷算法
 *
 ************************************************************************/
float HisValley(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 计算灰度直方图
	int histogram[256] = { 0 };
	for (size_t j = 0; j < lHeight; j++)
	{
		for (size_t i = 0; i < lWidth; i++)
		{
			unsigned char* lpSrc = lpDIBBits + j * lWidth + i;
			histogram[*lpSrc]++;
		}
	}

	// 计算频率分布图
	float freqDistribution[256] = { 0 };
	int totalPixels = lHeight * lWidth;
	for (int i = 0; i < 256; i++)
	{
		freqDistribution[i] = (float)histogram[i] / totalPixels;
	}

	// 找到使得累计概率和为0.5的分割点
	int splitIndex = 0;
	float cumulativeSum = 0.0;
	while (cumulativeSum < 0.5)
	{
		cumulativeSum += freqDistribution[splitIndex];
		splitIndex++;
	}

	// 在两段分割后的直方图中分别寻找峰值
	int peak1 = 0;
	int peak2 = 0;
	int peak1_index = 0;
	int peak2_index = 0;

	// 在前半段寻找峰值
	for (int i = 0; i < splitIndex; i++)
	{
		if (histogram[i] > peak1)
		{
			peak1 = histogram[i];
			peak1_index = i;
		}
	}

	// 在后半段寻找峰值
	for (int i = splitIndex; i < 256; i++)
	{
		if (histogram[i] > peak2)
		{
			peak2 = histogram[i];
			peak2_index = i;
		}
	}

	// 返回两个峰值的平均值作为阈值
	float threshold = (peak1_index + peak2_index) / 2.0;
	return threshold;


}
/*************************************************************************
 *
 * 函数名称：
 *   adaptiveThresholding()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   int windowSize		- 窗口大小
 *
 *
 * 说明:
 *   该函数为自适应阈值分割的实现
 *
 ************************************************************************/
BOOL WINAPI adaptiveThresholding(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int windowSize)
{
	// 指向复制图像的指针
	unsigned char* lpNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	int halfSize = windowSize / 2;
	int n = 3;
	int m = 2;

	// 计算结果
	FLOAT	fResult;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	int regionWidth = lWidth / n;
	int regionHeight = lHeight / m;

	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			int regionStartX = j * regionWidth;
			int regionStartY = i * regionHeight;

			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - regionStartY) + regionStartX;

			int histogram[256] = { 0 };

			// 统计区域的灰度直方图
			for (int y = regionStartY; y < regionStartY + regionHeight; ++y)
			{
				for (int x = regionStartX; x < regionStartX + regionWidth; ++x)
				{
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - y) + x;
					++histogram[*lpSrc];
				}
			}

			int totalPixels = regionWidth * regionHeight;
			float sum = 0.0;
			for (int k = 0; k < 256; ++k)
				sum += k * histogram[k];

			float sumB = 0.0;
			int wB = 0;
			int wF = 0;

			float maxVariance = 0.0;
			int threshold = 0;

			for (int k = 0; k < 256; ++k)
			{
				wB += histogram[k];
				if (wB == 0)
					continue;

				wF = totalPixels - wB;
				if (wF == 0)
					break;

				sumB += k * histogram[k];

				float meanB = sumB / wB;
				float meanF = (sum - sumB) / wF;

				float varianceBetween = wB * wF * (meanB - meanF) * (meanB - meanF);
				if (varianceBetween > maxVariance)
				{
					maxVariance = varianceBetween;
					threshold = k;
				}
			}

			// 应用阈值
			for (int y = regionStartY; y < regionStartY + regionHeight; ++y)
			{
				for (int x = regionStartX; x < regionStartX + regionWidth; ++x)
				{
					lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - y) + x;

					if (*lpDst > threshold)
						*lpDst = 255;
					else
						*lpDst = 0;
				}
			}
		}
	}

	for (int y = halfSize; y < lHeight - halfSize; y++)
	{
		for (int x = halfSize; x < lWidth - halfSize; x++)
		{
			int sum = 0;
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - y) + x;
			for (int i = -halfSize; i <= halfSize; i++)
			{
				for (int j = -halfSize; j <= halfSize; j++)
				{
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - y - i) + x + j;
					sum += (*lpSrc);
				}
			}
			int mean = sum / (windowSize * windowSize);
			if (*lpDst > mean - 5)
				*lpDst = 255;
			else
				*lpDst = 0;
		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   Winchange()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   int lowerLimit	- 下限参数
 *   int higherLimit	- 上限参数
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来进行窗口变换
 *
 ************************************************************************/
BOOL WINAPI Winchange(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight,int lowerLimit, int higherLimit) {
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	//逐行扫描
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			if (*lpSrc < lowerLimit) *lpSrc = 0;
			else if (*lpSrc > higherLimit) *lpSrc = 255;
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   HisStretch()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   int x1				- 第一个点的x坐标
 *   int y1				- 第一个点的y坐标
 *   int x2				- 第二个点的x坐标
 *   int y2				- 第二个点的y坐标
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来进行灰度拉伸
 *
 ************************************************************************/
BOOL WINAPI HisStretch(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int x1, int y1, int x2, int y2) {
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	std::pair<float, float> line_equation1 = calculateLine(0, 0, x1, y1);
	std::pair<float, float> line_equation2 = calculateLine(x1, y1, x2, y2);
	std::pair<float, float> line_equation3 = calculateLine(x2, y2, 255, 255);
	//逐行扫描
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			if (*lpSrc < x1) *lpSrc = Limit(line_equation1.first * (*lpSrc) + line_equation1.second);
			else if (*lpSrc > x2) *lpSrc = Limit(line_equation3.first * (*lpSrc) + line_equation3.second);
			else *lpSrc = Limit(line_equation2.first * (*lpSrc) + line_equation2.second);
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   HisEquation()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来进行直方图均衡化
 *
 ************************************************************************/
BOOL WINAPI HisEquation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight) {

	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	//像素总数
	int sum = lWidth * lHeight;
	//灰度统计表
	int hist[256] = { 0 };
	//概率
	double pHist[256] = { 0 };
	//累积概率
	double disHist[256] = { 0 };
	//新的灰度映射表
	unsigned char newHist[256] = { 0 };

	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			hist[*lpSrc]++;
		}
	}
	for (int i = 0; i < 256; i++) {
		pHist[i] = ((double)hist[i] /sum);
	}
	for (int i = 1; i < 256; i++) {
		disHist[i] = disHist[i - 1] + pHist[i];
	}
	for (int i = 0; i < 256; i++) {
		newHist[i] = (unsigned char)(255 * disHist[i] + 0.5);
	}
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			*lpSrc = newHist[*lpSrc];
		}
	}
	return TRUE;
}

/*************************************************************************
  *
  * 函数名称：
  *   Template()
  *
  * 参数:
  *   unsigned char* lpDIBBits    - 指向源DIB图像指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *	  FLOAT * fpArray	- 指向模板数组的指针
  *	  FLOAT fCoef		- 模板系数
  *   int fThre			- 模板阈值/系数
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE，否则返回FALSE。
  *
  * 说明:
  *   该函数用指定的模板（任意大小）来对图像进行操作，参数fpArray指定模板元素，fCoef指定系数。
  *
  ************************************************************************/

BOOL WINAPI Template(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float* fpArray, float fCoef, int fThre)
{
	// 指向复制图像的指针
	unsigned char* lpNewDIBBits;
	
	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;

	// 计算结果
	FLOAT	fResult;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// 行(除去边缘几行)
	for (i = 1; i < lHeight - 1; i++)
	{
		// 列(除去边缘几列)
		for (j = 1; j < lWidth - 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			// 计算
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// 指向DIB第i - 1 + k行，第j - 1 + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - i - k)
						+ j - 1 + l;

					// 保存象素值
					fResult += (*lpSrc) * fpArray[k * 3 + l];
				}
			}

			// 乘上系数
			fResult *= fCoef;
			fResult += fThre;

			// 取绝对值
			fResult = (FLOAT)fabs(fResult);

			// 判断是否超过255
			if (fResult > 255)
			{
				// 直接赋值为255
				*lpDst = 255;
			}
			else if (fResult < 0 )
			{
				// 直接赋值为255
				*lpDst = 0;
			}
			else
			{
				// 赋值
				*lpDst = (unsigned char)(fResult + 0.5);
			}

		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 返回
	return TRUE;

}

/*************************************************************************
  *
  * 函数名称：
  *   Translation()
  *
  * 参数:
  *   unsigned char* lpDIBBits    - 指向源DIB图像指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *	  int lXOffst		 - X方向偏移
  *	  int lYOffset		 - Y方向偏移
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE，否则返回FALSE。
  *
  * 说明:
  *   该函数用于图像平移。
  *
  ************************************************************************/

BOOL WINAPI Translation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset)
{
	// 指向复制图像的指针
	unsigned char* lpNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	// 循环变量
	LONG	i;
	LONG	j;
	LONG	i0;
	LONG	j0;

	// 计算结果
	FLOAT	fResult;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// 行(除去边缘几行)
	for (i = 0; i < lHeight; i++)
	{
		// 列(除去边缘几列)
		for (j = 0; j < lWidth; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			j0 = j - lXOffset;
			i0 = i - lYOffset;

			if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				*lpDst = *lpSrc;
			}
			else {
				*lpDst = 255;
			}
		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 返回
	return TRUE;
}

/*************************************************************************
  *
  * 函数名称：
  *   Mirror()
  *
  * 参数:
  *   unsigned char* lpDIBBits    - 指向源DIB图像指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *	  int   Dir		     - 镜像方向
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE，否则返回FALSE。
  *
  * 说明:
  *   该函数用于图像镜像。
  *
  ************************************************************************/

BOOL WINAPI Mirror(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int Dire)
{
	// 指向复制图像的指针
	unsigned char* lpNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	// 循环变量
	LONG	i;
	LONG	j;

	// 计算结果
	FLOAT	fResult;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// 行(除去边缘几行)
	for (i = 0; i < lHeight; i++)
	{
		// 列(除去边缘几列)
		for (j = 0; j < lWidth; j++)
		{
			if (j == 129)
			{
				int t = 0;
			}
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			//如果是水平镜像
			if (Dire == 0) {
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i)  + lWidth - j;
			}
			//如果是垂直镜像
			else {
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			}
			
			*lpDst = *lpSrc;
		}
	}
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 返回
	return TRUE;
}

/*************************************************************************
  *
  * 函数名称：
  *   Rotate()
  *
  * 参数:
  *   LPSTR lpDIB		 - 指向源DIB图像指针
  *   LPSTR lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *	  int   angle	     - 旋转角度
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE，否则返回FALSE。
  *
  * 说明:
  *   该函数用于图像旋转。
  *
  ************************************************************************/

HGLOBAL WINAPI Rotate(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int angle)
{
	// 指向复制图像的指针
	LPSTR lpNewDIB;
	LPSTR lpNewDIBBits;

	// 指向源像素的指针
	LPSTR lpSrc;

	// 指向新像素的指针
	LPSTR lpDst;

	// 循环变量
	LONG	i;
	LONG	j;
	LONG	i0;
	LONG	j0;

	//旋转后的新宽度和高度
	LONG lNewWidth;
	LONG lNewHeight;

	// 图像每行的字节数
	LONG lLineBytes;
	LONG lNewLineBytes;

	// 计算原图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 原图四个角的坐标（以图像中心为坐标系原点）
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
	float fRotateAngle = (angle * PI * 1.0) / 180;//角度转换为弧度
	float fSina = (float)sin((double)fRotateAngle);
	float fCosa = (float)cos((double)fRotateAngle);

	//原图的四个点坐标
	fSrcX1 = (float)(-(lWidth - 1) / 2);
	fSrcY1 = (float)((lHeight - 1) / 2);
	fSrcX2 = (float)((lWidth - 1) / 2);
	fSrcY2 = (float)((lHeight - 1) / 2);
	fSrcX3 = (float)((lWidth - 1) / 2);
	fSrcY3 = (float)(-(lHeight - 1) / 2);
	fSrcX4 = (float)(-(lWidth - 1) / 2);
	fSrcY4 = (float)(-(lHeight - 1) / 2);

	//新图的四个点坐标
	fDstX1 = fSrcX1 * fCosa + fSrcY1 * fSina;
	fDstY1 = -fSrcX1 * fSina + fSrcY1 * fCosa;
	fDstX2 = fSrcX2 * fCosa + fSrcY2 * fSina;
	fDstY2 = -fSrcX2 * fSina + fSrcY2 * fCosa;
	fDstX3 = fSrcX3 * fCosa + fSrcY3 * fSina;
	fDstY3 = -fSrcX3 * fSina + fSrcY3 * fCosa;
	fDstX4 = fSrcX4 * fCosa + fSrcY4 * fSina;
	fDstY4 = -fSrcX4 * fSina + fSrcY4 * fCosa;

	//新图像的长度和宽度
	lNewWidth = (LONG)(max(fabs(fDstX1 - fDstX3), fabs(fDstX2 - fDstX4)) + 0.5);
	lNewHeight = (LONG)(max(fabs(fDstY1 - fDstY3), fabs(fDstY2 - fDstY4)) + 0.5);

	//映射公式中的两个常数
	float f1 = (float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina + 0.5 * (lWidth - 1));
	float f2 = (float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa + 0.5 * (lHeight - 1));

	//新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	HDIB hDIB;
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	if (hDIB == NULL) {
		return NULL;
	}
	lpNewDIB = (char*) ::GlobalLock((HGLOBAL)hDIB);
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);

	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	LPBITMAPCOREHEADER lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;
	lpbmi->biWidth = lNewWidth;
	lpbmi->biHeight = lNewHeight;

	for (i = 0; i < lNewHeight; i++)
	{
		for (j = 0; j < lNewWidth; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (char*)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			i0 = (LONG)(-((float)j) * fSina + ((float)i) * fCosa + f2 + 0.5);
			j0 = (LONG)( ((float)j) * fCosa + ((float)i) * fSina + f1 + 0.5);

			if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				*lpDst = *lpSrc;
			}
			else {
				*lpDst = 255;
			}
		}
	}

	// 返回
	return hDIB;
}

/*************************************************************************
 *
 * 函数名称：
 *   AnalysisForeground()
 *
 * 参数:
 *   unsigned char * lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *
 * 返回值:
 *   BOOL               - 前景为白色返回1，为黑色返回0
 *
 * 说明:
 *   该函数用来检查前景的颜色
 *
 ************************************************************************/
int WINAPI AnalysisForeground(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	int FOREGROUND;
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	LONG white = 0;
	LONG black = 0;

	//逐行扫描
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			if (*lpSrc == 0) black++;
			else white++;
		}
	}
	if (black > white) return 1;
	else return 0;
}
/*************************************************************************
  *
  * 区域计数函数组
  *
  ************************************************************************/

// 创建一个标记图像并初始化为全零
std::vector<std::vector<bool>> createVisitedImage(int width, int height) {
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
	return visited;
}

// 使用传引用方式更新图像的访问标记
void markVisited(std::vector<std::vector<bool>>& visited, int x, int y) {
	visited[y][x] = true;
}

// 使用传值方式检查图像的访问状态
bool isVisited(const std::vector<std::vector<bool>>& visited, int x, int y) {
	return visited[y][x];
}

//边界追踪算法
int traceBoundary(const unsigned char* image, int width, int height, int startX, int startY, int FOREGROUND, int connection, unsigned char* lpDstImageData) {
	// 指向像素的指针
	unsigned char* lpSrc;
	unsigned char* lpDst;
	// 图像每行的字节数
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES( width * 8);
	// 定义方向
	int d4x[] = { 1, 0, -1, 0 };
	int d4y[] = { 0, -1, 0, 1 };

	int d8x[] = { 1, 1, 0, -1, -1,-1, 0, 1};
	int d8y[] = { 0, -1,-1,-1,  0, 1, 1, 1};

	// 记录起始点和当前点
	int currentX = startX;
	int currentY = startY;
	startX = currentX;
	startY = currentY;

	// 初始化周长
	int perimeter = 0;

	// 开始边界跟踪
	int direction = 0;
	do {
		int nextX = startX;
		int nextY = startY;
		if (connection == 4) {
			nextX = currentX + d4x[direction];
			nextY = currentY + d4y[direction];
		}
		else {
			nextX = currentX + d8x[direction];
			nextY = currentY + d8y[direction];
		}
		lpSrc = (unsigned char*)image + lLineBytes * (height - 1 - nextY) + nextX;
		// 检查下一个点是否在图像范围内且为前景像素
		if (nextX >= 0 && nextX < width && nextY >= 0 && nextY < height && *lpSrc == FOREGROUND) {
			// 更新当前点为下一个点
			currentX = nextX;
			currentY = nextY;
			lpDst = (unsigned char*)lpDstImageData + (WIDTHBYTES(width * 24) * (height - 1 - currentY)) + currentX * 3;
			lpDst[2] = 0;
			lpDst[1] = 0;
			lpDst[0] = 255;
			// 更新周长
			perimeter++;
			if (connection == 4) {
				// 重置方向为逆时针旋转90度
				direction = (direction + 1) % 4;
			}
			else {
				// 重置方向为逆时针旋转90度
				direction = (direction + 1) % 8;
			}
		}
		else {
			if (connection == 4) {
				// 顺时针旋转90度
				direction = (direction - 1 + 4) % 4;
			}
			else {
				// 顺时针旋转45度
				direction = (direction - 1 + 8) % 8;
			}
		}
	} while (currentX != startX || currentY != startY);  // 回到起始点时停止

	return perimeter;
}
/*************************************************************************
  *
  * 函数名称：
  *   countRegions()
  *
  * 参数:
  *   unsigned char *  lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *	  int connection	 - 连通数
  *	  int color			 - 前景颜色
  *
  * 返回值:
  *   BOOL               - 成功返回区域计数结果
  *
  * 说明:
  *   该函数用于区域计数。
  *
  ************************************************************************/
int WINAPI countRegions(unsigned char * lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color)
{
	int count = 0;

	// 指向源像素的指针
	unsigned char* lpSrc;
	unsigned char* lpNew;
	// 图像每行的字节数
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//给前景赋值
	int FOREGROUND = 0;
	if (color == 0)  FOREGROUND = 0;//黑
	else  FOREGROUND = 255;//白

	//邻域相对坐标
	int dx[] = { -1, 1, 0, 0 , -1, -1, 1, 1};
	int dy[] = { 0, 0, -1, 1 , -1, 1 ,1, -1};
	int NEIGHBORNUM = 0;
	if (connection == 0) {
		NEIGHBORNUM = 8;
	}
	else {
		NEIGHBORNUM = 4;
	}

	// 创建一个标记图像的数组并初始化为全零
	std::vector<std::vector<bool>> visited = createVisitedImage(lWidth, lHeight);

	// 计算图像中的物体数量
	for (int y = 0; y < lHeight; y++) {
		for (int x = 0; x < lWidth; x++) {
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - y) + x;
			// 如果是前景像素且未被访问过，则表示找到了一个新的物体
			if (*lpSrc == FOREGROUND && !isVisited(visited, x, y)) {
				count++;
				int area = 0;
				// 使用广度优先搜索来标记与当前像素相连的所有前景像素
				std::queue<std::pair<int, int>> q;
				q.push(std::make_pair(x, y));
				markVisited(visited, x, y);

				while (!q.empty()) {
					area++;
					int currentX = q.front().first;
					int currentY = q.front().second;
					q.pop();

					// 检查相邻的前景像素
					for (int i = 0; i < NEIGHBORNUM; i++) {
						int newX = currentX + dx[i];
						int newY = currentY + dy[i];

						// 检查像素是否在图像范围内且未被访问过
						lpNew = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - newY) + newX;
						if (newX >= 0 && newX < lWidth && newY >= 0 && newY < lHeight && *lpNew == FOREGROUND && !isVisited(visited, newX, newY)) {
							q.push(std::make_pair(newX, newY));
							markVisited(visited, newX, newY);
						}
					}
				}
				//太小的物体不计入
				if (area < 5) {
					count--;
				}
			}
		}
	}
	return count;
}

/*************************************************************************
  *
  * 函数名称：
  *   calculateObjectProperties()
  *
  * 参数:
  *   unsigned char *  lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *	  int connection	 - 连通数
  *	  int color			 - 前景颜色
  *
  * 返回值:
  *   BOOL               - 成功返回区域计数结果
  *
  * 说明:
  *   该函数用于区域计数。
  *
  ************************************************************************/
ObjectProperties WINAPI calculateObjectProperties(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color, unsigned char* lpDstImageData) {
	ObjectProperties properties = {0, 0, 0, 0, 0, 0, 0, 0};

	int count = 0;

	// 指向源像素的指针
	unsigned char* lpSrc;
	unsigned char* lpNew;
	unsigned char* lpDst;
	// 图像每行的字节数
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//给前景赋值
	int FOREGROUND = 0;
	if (color == 0)  FOREGROUND = 0;//黑
	else  FOREGROUND = 255;//白

	//邻域相对坐标
	int dx[] = { -1, 1, 0, 0 , -1, -1, 1, 1 };
	int dy[] = { 0, 0, -1, 1 , -1, 1 ,1, -1 };
	int NEIGHBORNUM = 0;
	if (connection == 0) {
		NEIGHBORNUM = 8;
	}
	else {
		NEIGHBORNUM = 4;
	}

	// 创建一个标记图像的数组并初始化为全零
	std::vector<std::vector<bool>> visited = createVisitedImage(lWidth, lHeight);

	for (int y = 0; y < lHeight; y++) {
		for (int x = 0; x < lWidth; x++) {
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - y) + x;
			// 如果是前景像素且未被访问过，则表示找到了一个新的物体
			if (*lpSrc == FOREGROUND && !isVisited(visited, x, y)) {
				// 使用广度优先搜索来计算物体的属性
				int startX = x;
				int startY = y;
				int area = 0;
				int minX = lWidth - 1;
				int maxX = 0;
				int minY = lHeight - 1;
				int maxY = 0;

				std::queue<std::pair<int, int>> q;
				q.push(std::make_pair(x, y));
				markVisited(visited, x, y);

				while (!q.empty()) {
					int currentX = q.front().first;
					int currentY = q.front().second;
					q.pop();

					// 更新物体的边界框
					minX = min(minX, currentX);
					maxX = max(maxX, currentX);
					minY = min(minY, currentY);
					maxY = max(maxY, currentY);

					// 更新物体的面积
					area++;
					// 检查相邻的前景像素
					for (int i = 0; i < NEIGHBORNUM; i++) {
						int newX = currentX + dx[i];
						int newY = currentY + dy[i];

						// 检查像素是否在图像范围内且未被访问过
						lpNew = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - newY) + newX;
						if (newX >= 0 && newX < lWidth && newY >= 0 && newY < lHeight && *lpNew == FOREGROUND && !isVisited(visited, newX, newY)) {
							q.push(std::make_pair(newX, newY));
							markVisited(visited, newX, newY);
							lpDst = (unsigned char*)lpDstImageData + (WIDTHBYTES(lWidth * 24) * (lHeight - 1 - newY)) + newX * 3;
							lpDst[2] = 0;
							lpDst[1] = 100;
							lpDst[0] = 100;
						}
					}
				}
				// 更新最大物体的属性
				if (area > properties.area) {
					properties.area = area;
					//矩形度和重心坐标
					properties.startX = startX;
					properties.startY = startY;
					int centerX = (minX + maxX) / 2;
					int centerY = (minY + maxY) / 2;
					properties.centerX = centerX;
					properties.centerY = centerY;
					double rectangularity = area * 1.0 / ((maxX - minX + 1) * (maxY - minY + 1));
					properties.rectangularity = rectangularity;
				}
			}
		}
	}
	//边界追踪求周长
	properties.perimeter = traceBoundary(lpDIBBits, lWidth, lHeight, properties.startX, properties.startY, FOREGROUND, connection, lpDstImageData);
	// 计算物体的圆形度
	properties.circularity = (4 * 3.14159 * properties.area) / (properties.perimeter * properties.perimeter);
	return properties;
}

/*************************************************************************
  *
  * 函数名称：
  *   Stylize1()
  *
  * 参数:
  *   unsigned char *  lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *   int lpSrcBitCount  - 位数
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE
  *
  * 说明:
  *   该函数用于第一种风格化。
  *
  ************************************************************************/
BOOL WINAPI Stylize1(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount) {
	unsigned char* lpSrc;
	// 图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// 调整颜色通道，添加暗角和光晕效果，并应用老化纹理和噪点
	for (int i = 0; i < lHeight; i++) {
		for (int j = 0; j < lWidth; j++) {
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j * 3;

			int red = lpSrc[2];
			int green = lpSrc[1];
			int blue = lpSrc[0];

			// 调整颜色通道，偏向黄色和绿色
			int newRed = static_cast<int>(0.7 * red + 0.3 * green);
			int newGreen = static_cast<int>(0.6 * green + 0.4 * red);
			int newBlue = static_cast<int>(0.6 * blue);

			// 添加暗角效果
			double distance = std::sqrt((j - lWidth / 2.0) * (j - lWidth / 2.0) + (i - lHeight / 2.0) * (i - lHeight / 2.0));
			double vignette = 1.0 - 0.8 * (distance / (lWidth / 2.0));
			newRed = static_cast<int>(newRed * vignette);
			newGreen = static_cast<int>(newGreen * vignette);
			newBlue = static_cast<int>(newBlue * vignette);

			// 添加光晕效果
			double glow = 1.0 - 0.5 * (distance / (lWidth / 2.0));
			newRed += static_cast<int>(100 * glow);
			newGreen += static_cast<int>(80 * glow);
			newBlue += static_cast<int>(60 * glow);

			// 确保像素值在 0-255 范围内
			if (newRed < 0) {
				newRed = 0;
			}
			else if (newRed > 255) {
				newRed = 255;
			}

			if (newGreen < 0) {
				newGreen = 0;
			}
			else if (newGreen > 255) {
				newGreen = 255;
			}

			if (newBlue < 0) {
				newBlue = 0;
			}
			else if (newBlue > 255) {
				newBlue = 255;
			}

			// 应用老化纹理
			if (rand() % 100 < 10) {
				// 30% 的概率添加纹理效果
				int texture = rand() % 10 - 25;
				newRed += texture;
				newGreen += texture;
				
			}

			// 添加噪点
			if (rand() % 100 < 5) {
				// 5% 的概率添加噪点
				int noise = rand() % 50 - 25;
				newRed += noise;
				newGreen += noise;
				newBlue += noise;
			}

			lpSrc[2] = static_cast<unsigned char>(newRed);
			lpSrc[1] = static_cast<unsigned char>(newGreen);
			lpSrc[0] = static_cast<unsigned char>(newBlue);
		}
	}

	return TRUE;
}


/*************************************************************************
  *
  * 函数名称：
  *   Stylize2()
  *
  * 参数:
  *   unsigned char *  lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *   int lpSrcBitCount  - 位数
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE
  *
  * 说明:
  *   该函数用于第二种风格化。
  *
  ************************************************************************/
BOOL WINAPI Stylize2(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount) {
	unsigned char* lpSrc;
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	int brushSize = 3;
	int intensityLevels = 7;
	double textureScale = 0.8;

	unsigned char* lpTempBits = new unsigned char[lLineBytes * lHeight];
	memcpy(lpTempBits, lpDIBBits, lLineBytes * lHeight);

	unsigned char* lpTextureBits = new unsigned char[lLineBytes * lHeight];
	// 填充纹理图像数据

	for (int y = 0; y < lHeight; y++) {
		for (int x = 0; x < lWidth; x++) {
			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;
			int count = 0;

			for (int i = -brushSize; i <= brushSize; i++) {
				for (int j = -brushSize; j <= brushSize; j++) {
					int nx = x + j;
					int ny = y + i;

					if (nx >= 0 && nx < lWidth && ny >= 0 && ny < lHeight) {
						lpSrc = lpTempBits + ny * lLineBytes + nx * 3;
						redTotal += lpSrc[2];
						greenTotal += lpSrc[1];
						blueTotal += lpSrc[0];
						count++;
					}
				}
			}

			int avgRed = redTotal / count;
			int avgGreen = greenTotal / count;
			int avgBlue = blueTotal / count;

			int intensityStep = 255 / (intensityLevels - 1);
			int newRed = (avgRed / intensityStep) * intensityStep;
			int newGreen = (avgGreen / intensityStep) * intensityStep;
			int newBlue = (avgBlue / intensityStep) * intensityStep;

			int textureX = x * textureScale;
			int textureY = y * textureScale;
			lpSrc = lpTextureBits + textureY * lLineBytes + textureX * 3;
			newRed = (newRed * lpSrc[2]) / 255;
			newGreen = (newGreen * lpSrc[1]) / 255;
			newBlue = (newBlue * lpSrc[0]) / 255;

			lpSrc = lpDIBBits + y * lLineBytes + x * 3;
			lpSrc[2] = newRed;
			lpSrc[1] = newGreen;
			lpSrc[0] = newBlue;
		}
	}

	delete[] lpTempBits;
	delete[] lpTextureBits;

	return TRUE;
}


/*************************************************************************
  *
  * 函数名称：
  *   Stylize3()
  *
  * 参数:
  *   unsigned char *  lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *   int lpSrcBitCount  - 位数
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE
  *
  * 说明:
  *   该函数用于第三种风格化。
  *
  ************************************************************************/
BOOL WINAPI Stylize3(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount) {
	// 定义每行像素所占的字节数
	int LineBytes = WIDTHBYTES(lWidth * lpSrcBitCount);

	// 创建OpenCV Mat对象来处理图像
	unsigned char* tempBits = lpDIBBits;
	cv::Mat frame(lHeight, lWidth, CV_8UC3, tempBits);

	// 转换为灰度图像
	cv::Mat gray;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

	// 边缘提取
	cv::Mat edges;
	cv::Canny(gray, edges, 120, 240);

	// 生成边缘掩膜
	cv::Mat mask;
	cv::threshold(edges, mask, 120, 255, cv::THRESH_BINARY_INV);

	// 进行下采样
	cv::Mat downsampled;
	cv::pyrDown(frame, downsampled);
	cv::pyrDown(downsampled, downsampled);

	// 双边滤波
	cv::Mat filtered;
	cv::bilateralFilter(downsampled, filtered, 5, 30, 20);

	// 进行下采样
	cv::pyrDown(filtered, downsampled);
	cv::pyrDown(downsampled, downsampled);

	// 恢复图像大小
	cv::Mat result;
	cv::pyrUp(downsampled, result);
	cv::pyrUp(result, result);

	// 降低图像饱和度
	cv::Mat hsv;
	cv::cvtColor(result, hsv, cv::COLOR_BGR2HSV);
	cv::Mat hsvChannels[3];
	cv::split(hsv, hsvChannels);
	cv::Mat saturationReduced;
	cv::subtract(hsvChannels[1], cv::Scalar(20), saturationReduced);
	cv::threshold(saturationReduced, saturationReduced, 0, 255, cv::THRESH_TRUNC);
	cv::add(hsvChannels[1], saturationReduced, hsvChannels[1]);
	cv::merge(hsvChannels, 3, result);
	cv::cvtColor(result, result, cv::COLOR_HSV2BGR);

	// 掩膜叠加
	cv::Mat output;
	frame.copyTo(output, mask);

	// 将结果复制回lpDIBBits
	tempBits = lpDIBBits;
	std::memcpy(tempBits, output.data, LineBytes * lHeight);

	return TRUE;
}


/*************************************************************************
  *
  * canny边缘检测函数组
  *
  ************************************************************************/
// 计算梯度幅值和方向
void computeGradient(unsigned char* image, int width, int height, int* gradientMagnitude, int* gradientDirection)
{
	unsigned char* lpSrc;
	// 图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(width * 8);
	// 使用Sobel算子计算梯度
	int sobelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int sobelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			int gx = 0;
			int gy = 0;

			// 计算x方向梯度
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					lpSrc = (unsigned char*)image + lLineBytes * (y + i) + (x + j);
					int pixelValue = *lpSrc;
					gx += sobelX[i + 1][j + 1] * pixelValue;
				}
			}

			// 计算y方向梯度
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					lpSrc = (unsigned char*)image + lLineBytes * (y + i) + (x + j);
					int pixelValue = *lpSrc;
					gy += sobelY[i + 1][j + 1] * pixelValue;
				}
			}

			// 计算梯度幅值和方向
			gradientMagnitude[y * lLineBytes + x] = sqrt(gx * gx + gy * gy);
			gradientDirection[y * lLineBytes + x] = atan2(gy, gx) * 180 / PI;
		}
	}
}

// 非极大值抑制
void nonMaxSuppression(int* gradientMagnitude, int* gradientDirection, int width, int height, unsigned char* edges)
{
	LONG lLineBytes = WIDTHBYTES(width * 8);
	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			int angle = gradientDirection[y * lLineBytes + x];

			// 根据梯度方向进行非极大值抑制
			int pixelValue = gradientMagnitude[y * lLineBytes + x];
			int n1, n2;

			// 根据梯度方向确定抑制方向
			if ((angle <= 22.5 && angle >= -22.5) || (angle >= 157.5 && angle <= -157.5))
			{
				n1 = gradientMagnitude[y * lLineBytes + x + 1];
				n2 = gradientMagnitude[y * lLineBytes + x - 1];
			}
			else if ((angle > 22.5 && angle < 67.5) || (angle < -112.5 && angle > -157.5))
			{
				n1 = gradientMagnitude[(y + 1) * lLineBytes + x + 1];
				n2 = gradientMagnitude[(y - 1) * lLineBytes + x - 1];
			}
			else if ((angle >= 67.5 && angle <= 112.5) || (angle <= -67.5 && angle >= -112.5))
			{
				n1 = gradientMagnitude[(y + 1) * lLineBytes + x];
				n2 = gradientMagnitude[(y - 1) * lLineBytes + x];
			}
			else
			{
				n1 = gradientMagnitude[(y + 1) * lLineBytes + x - 1];
				n2 = gradientMagnitude[(y - 1) * lLineBytes + x + 1];
			}

			// 进行非极大值抑制
			if (pixelValue >= n1 && pixelValue >= n2)
			{
				edges[y * lLineBytes + x] = (unsigned char)(pixelValue);
			}
			else
			{
				edges[y * lLineBytes + x] = 0;
			}
		}
	}
}

// 双阈值处理
void doubleThreshold(unsigned char* edges, int width, int height, int lowThreshold, int highThreshold)
{
	LONG lLineBytes = WIDTHBYTES(width * 8);
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			unsigned char pixelValue = edges[y * lLineBytes + x];

			// 根据阈值进行分割
			if (pixelValue >= highThreshold)
			{
				edges[y * lLineBytes + x] = 255;  // 强边缘
			}
			else if (pixelValue >= lowThreshold)
			{
				edges[y * lLineBytes + x] = 128;  // 弱边缘
			}
			else
			{
				edges[y * lLineBytes + x] = 0;    // 背景
			}
		}
	}
}

// 边缘跟踪
void edgeTracking(unsigned char* edges, int width, int height)
{
	LONG lLineBytes = WIDTHBYTES(width * 8);
	unsigned char* tempEdges = new unsigned char[lLineBytes * height];
	std::memcpy(tempEdges, edges, lLineBytes * height * sizeof(unsigned char));

	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			unsigned char pixelValue = tempEdges[y * width + x];

			if (pixelValue == 128)
			{
				// 检查8邻域是否存在强边缘
				if (tempEdges[(y - 1) * lLineBytes + (x - 1)] == 255 || tempEdges[(y - 1) * lLineBytes + x] == 255 ||
					tempEdges[(y - 1) * lLineBytes + (x + 1)] == 255 || tempEdges[y * lLineBytes + (x - 1)] == 255 ||
					tempEdges[y * lLineBytes + (x + 1)] == 255 || tempEdges[(y + 1) * lLineBytes + (x - 1)] == 255 ||
					tempEdges[(y + 1) * lLineBytes + x] == 255 || tempEdges[(y + 1) * lLineBytes + (x + 1)] == 255)
				{
					edges[y * lLineBytes + x] = 255;
				}
				else
				{
					edges[y * lLineBytes + x] = 0;
				}
			}
		}
	}
	delete[] tempEdges;
}

/*************************************************************************
  *
  * 函数名称：
  *   cannyEdgeDetection()
  *
  * 参数:
  *   unsigned char *  lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE
  *
  * 说明:
  *   Canny边缘检测算法
  *
  ************************************************************************/
BOOL WINAPI cannyEdgeDetection(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 8);
	// 高斯平滑
	float coff = 1.0 / 16;
	float fpArray[9] = { 1,2,1,2,4,2,1,2,1 };
	Template(lpDIBBits, lWidth, lHeight, fpArray, coff, 0);

	// 计算梯度幅值和方向
	int* gradientMagnitude = new int[lLineBytes * lHeight];
	int* gradientDirection = new int[lLineBytes * lHeight];
	computeGradient(lpDIBBits, lWidth, lHeight, gradientMagnitude, gradientDirection);

	// 非极大值抑制
	unsigned char* edges = new unsigned char[lLineBytes * lHeight];
	nonMaxSuppression(gradientMagnitude, gradientDirection, lWidth, lHeight, edges);

	// 双阈值处理
	int lowThreshold = 50;
	int highThreshold = 150;
	doubleThreshold(edges, lWidth, lHeight, lowThreshold, highThreshold);

	// 边缘跟踪
	edgeTracking(edges, lWidth, lHeight);

	// 复制变换后的图像
	memcpy(lpDIBBits, edges, lLineBytes * lHeight);
	// 清理内存
	delete[] gradientMagnitude;
	delete[] gradientDirection;
	delete[] edges;
	return TRUE;
}

/*************************************************************************
  *
  * 函数名称：
  *   usmSharpening()
  *
  * 参数:
  *   unsigned char *  lpDIBBits    - 指向源DIB像素指针
  *   LONG  lWidth       - 源图像宽度（象素数）
  *   LONG  lHeight      - 源图像高度（象素数）
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE
  *
  * 说明:
  *   usm算法
  *
  ************************************************************************/
// USM锐化函数
BOOL WINAPI usmSharpening(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float amount, float threshold)
{
	// 图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 8);

	// 创建临时缓冲区
	unsigned char* blurred = new unsigned char[lLineBytes * lHeight];
	memcpy(blurred, lpDIBBits, lLineBytes * lHeight);

	unsigned char* mask = new unsigned char[lLineBytes * lHeight];
	unsigned char* sharpened = new unsigned char[lLineBytes * lHeight];

	// 计算模糊图像
	// 高斯平滑
	float coff = 1.0 / 16;
	float fpArray[9] = { 1,2,1,2,4,2,1,2,1 };
	Template(blurred, lWidth, lHeight, fpArray, coff, 0);

	// 计算掩膜和锐化图像
	for (int y = 0; y < lHeight; y++)
	{
		for (int x = 0; x < lWidth; x++)
		{
			int diff = lpDIBBits[y * lLineBytes + x] - blurred[y * lLineBytes + x];
			mask[y * lLineBytes + x] = (abs(diff) > threshold) ? 255 : 0;
			sharpened[y * lLineBytes + x] = lpDIBBits[y * lLineBytes + x] + amount * mask[y * lLineBytes + x] * diff / 255;
		}
	}

	// 更新原始图像
	memcpy(lpDIBBits, sharpened, lLineBytes * lHeight);

	// 释放临时缓冲区
	delete[] blurred;
	delete[] mask;
	delete[] sharpened;

	return TRUE;
}