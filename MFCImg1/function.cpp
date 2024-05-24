
/**************************************************************************
 *  �ļ�����ThresholdAlg.cpp
 *
 *  �Ҷ���ֵ�任�����⣺
 *
 *  Binarization()		-��ֵ��
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
//�������㷵��б�ʺͽؾ�
std::pair<float, float> calculateLine(float x1, float y1, float x2, float y2) {
	float slope = (y2 - y1) / (x2 - x1);
	float y_intercept = y1 - slope * x1;
	return std::make_pair(slope, y_intercept);
}
/*************************************************************************
 *
 * �������ƣ�
 *   Binarization()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int threshold		- ��ֵͼ�����ֵ����
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������ת��8λ�Ҷ�ͼ�񵽶�ֵͼ��
 *
 ************************************************************************/
BOOL WINAPI Binarization(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int threshold)
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	//����ɨ��
	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			if (*lpSrc < threshold) *lpSrc = (unsigned char)0;
			else *lpSrc = (unsigned char)255;
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   Iteration()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   float delta_T		- �����㷨�Ĳ���
 *
 * ����ֵ:
 *   float               - �ɹ������������ֵT
 *
 * ˵��:
 *   �ú���Ϊ�Ҷ���ֵ�任�ĵ�����ʵ��
 *
 ************************************************************************/
float Iteration(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float delta_T)
{
	//�����ҶȺ���С�Ҷ�
	int sum_his = 0;
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
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
 * �������ƣ�
 *   Ostu()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   float delta_T		- �����㷨�Ĳ���
 *
 * ����ֵ:
 *   int               - �ɹ������������ֵT
 *
 * ˵��:
 *   �ú���Ϊ�Ҷ���ֵ�仯�Ĵ��ʵ��
 *
 ************************************************************************/
int Ostu(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	int T = 0; //�����ֵ
	int size = lHeight * lWidth;
	float variance;   //��䷽��
	float maxVariance = 0, w1 = 0, w2 = 0, avgValue = 0;
	float u0 = 0, u1 = 0, u2 = 0;
	//���ɻҶ�ֱ��ͼ
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
	//�����ҳ���䷽�����maxVariance������ֵ��Ostu_Threshold��
	for (int i = 0; i <= 255; i++)
	{
		w1 = 0;
		w2 = 0;
		u1 = 0;
		u2 = 0;
		//���㱳������ռ��,ƽ���Ҷ�
		for (int j = 0; j <= i; j++)
		{
			w1 += histgram[j];
			u1 += histgram[j] * j;
		}
		u1 = u1 / w1;
		//����ǰ������ռ��,ƽ���Ҷ�
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
		//������䷽��
		variance = w1 * w2 * (u1 - u2) * (u1 - u2);
		if (variance > maxVariance)
		{ //�ҵ�ʹ�ҶȲ�����ֵ
			maxVariance = variance;
			T = i;            //�Ǹ�ֵ������ֵ
		}
	}
	return T;
}

/*************************************************************************
 *
 * �������ƣ�
 *   HisValley()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   float               - �ɹ������������ֵT
 *
 * ˵��:
 *   �ú���Ϊ�Ҷ���ֵ�任�ĻҶ�ͼ�����㷨
 *
 ************************************************************************/
float HisValley(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ����Ҷ�ֱ��ͼ
	int histogram[256] = { 0 };
	for (size_t j = 0; j < lHeight; j++)
	{
		for (size_t i = 0; i < lWidth; i++)
		{
			unsigned char* lpSrc = lpDIBBits + j * lWidth + i;
			histogram[*lpSrc]++;
		}
	}

	// ����Ƶ�ʷֲ�ͼ
	float freqDistribution[256] = { 0 };
	int totalPixels = lHeight * lWidth;
	for (int i = 0; i < 256; i++)
	{
		freqDistribution[i] = (float)histogram[i] / totalPixels;
	}

	// �ҵ�ʹ���ۼƸ��ʺ�Ϊ0.5�ķָ��
	int splitIndex = 0;
	float cumulativeSum = 0.0;
	while (cumulativeSum < 0.5)
	{
		cumulativeSum += freqDistribution[splitIndex];
		splitIndex++;
	}

	// �����ηָ���ֱ��ͼ�зֱ�Ѱ�ҷ�ֵ
	int peak1 = 0;
	int peak2 = 0;
	int peak1_index = 0;
	int peak2_index = 0;

	// ��ǰ���Ѱ�ҷ�ֵ
	for (int i = 0; i < splitIndex; i++)
	{
		if (histogram[i] > peak1)
		{
			peak1 = histogram[i];
			peak1_index = i;
		}
	}

	// �ں���Ѱ�ҷ�ֵ
	for (int i = splitIndex; i < 256; i++)
	{
		if (histogram[i] > peak2)
		{
			peak2 = histogram[i];
			peak2_index = i;
		}
	}

	// ����������ֵ��ƽ��ֵ��Ϊ��ֵ
	float threshold = (peak1_index + peak2_index) / 2.0;
	return threshold;


}
/*************************************************************************
 *
 * �������ƣ�
 *   adaptiveThresholding()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int windowSize		- ���ڴ�С
 *
 *
 * ˵��:
 *   �ú���Ϊ����Ӧ��ֵ�ָ��ʵ��
 *
 ************************************************************************/
BOOL WINAPI adaptiveThresholding(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int windowSize)
{
	// ָ����ͼ���ָ��
	unsigned char* lpNewDIBBits;

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;

	int halfSize = windowSize / 2;
	int n = 3;
	int m = 2;

	// ������
	FLOAT	fResult;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// ��ʼ��ͼ��Ϊԭʼͼ��
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

			// ͳ������ĻҶ�ֱ��ͼ
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

			// Ӧ����ֵ
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

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// ����
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   Winchange()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int lowerLimit	- ���޲���
 *   int higherLimit	- ���޲���
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú����������д��ڱ任
 *
 ************************************************************************/
BOOL WINAPI Winchange(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight,int lowerLimit, int higherLimit) {
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	//����ɨ��
	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			if (*lpSrc < lowerLimit) *lpSrc = 0;
			else if (*lpSrc > higherLimit) *lpSrc = 255;
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   HisStretch()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int x1				- ��һ�����x����
 *   int y1				- ��һ�����y����
 *   int x2				- �ڶ������x����
 *   int y2				- �ڶ������y����
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú����������лҶ�����
 *
 ************************************************************************/
BOOL WINAPI HisStretch(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int x1, int y1, int x2, int y2) {
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	std::pair<float, float> line_equation1 = calculateLine(0, 0, x1, y1);
	std::pair<float, float> line_equation2 = calculateLine(x1, y1, x2, y2);
	std::pair<float, float> line_equation3 = calculateLine(x2, y2, 255, 255);
	//����ɨ��
	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
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
 * �������ƣ�
 *   HisEquation()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����������ֱ��ͼ���⻯
 *
 ************************************************************************/
BOOL WINAPI HisEquation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight) {

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	//��������
	int sum = lWidth * lHeight;
	//�Ҷ�ͳ�Ʊ�
	int hist[256] = { 0 };
	//����
	double pHist[256] = { 0 };
	//�ۻ�����
	double disHist[256] = { 0 };
	//�µĻҶ�ӳ���
	unsigned char newHist[256] = { 0 };

	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
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
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			*lpSrc = newHist[*lpSrc];
		}
	}
	return TRUE;
}

/*************************************************************************
  *
  * �������ƣ�
  *   Template()
  *
  * ����:
  *   unsigned char* lpDIBBits    - ָ��ԴDIBͼ��ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *	  FLOAT * fpArray	- ָ��ģ�������ָ��
  *	  FLOAT fCoef		- ģ��ϵ��
  *   int fThre			- ģ����ֵ/ϵ��
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
  *
  * ˵��:
  *   �ú�����ָ����ģ�壨�����С������ͼ����в���������fpArrayָ��ģ��Ԫ�أ�fCoefָ��ϵ����
  *
  ************************************************************************/

BOOL WINAPI Template(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float* fpArray, float fCoef, int fThre)
{
	// ָ����ͼ���ָ��
	unsigned char* lpNewDIBBits;
	
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;

	// ѭ������
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;

	// ������
	FLOAT	fResult;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��(��ȥ��Ե����)
	for (i = 1; i < lHeight - 1; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = 1; j < lWidth - 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			// ����
			for (k = 0; k < 3; k++)
			{
				for (l = 0; l < 3; l++)
				{
					// ָ��DIB��i - 1 + k�У���j - 1 + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - i - k)
						+ j - 1 + l;

					// ��������ֵ
					fResult += (*lpSrc) * fpArray[k * 3 + l];
				}
			}

			// ����ϵ��
			fResult *= fCoef;
			fResult += fThre;

			// ȡ����ֵ
			fResult = (FLOAT)fabs(fResult);

			// �ж��Ƿ񳬹�255
			if (fResult > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				*lpDst = 255;
			}
			else if (fResult < 0 )
			{
				// ֱ�Ӹ�ֵΪ255
				*lpDst = 0;
			}
			else
			{
				// ��ֵ
				*lpDst = (unsigned char)(fResult + 0.5);
			}

		}
	}

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// ����
	return TRUE;

}

/*************************************************************************
  *
  * �������ƣ�
  *   Translation()
  *
  * ����:
  *   unsigned char* lpDIBBits    - ָ��ԴDIBͼ��ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *	  int lXOffst		 - X����ƫ��
  *	  int lYOffset		 - Y����ƫ��
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
  *
  * ˵��:
  *   �ú�������ͼ��ƽ�ơ�
  *
  ************************************************************************/

BOOL WINAPI Translation(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset)
{
	// ָ����ͼ���ָ��
	unsigned char* lpNewDIBBits;

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;

	// ѭ������
	LONG	i;
	LONG	j;
	LONG	i0;
	LONG	j0;

	// ������
	FLOAT	fResult;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��(��ȥ��Ե����)
	for (i = 0; i < lHeight; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = 0; j < lWidth; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
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

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// ����
	return TRUE;
}

/*************************************************************************
  *
  * �������ƣ�
  *   Mirror()
  *
  * ����:
  *   unsigned char* lpDIBBits    - ָ��ԴDIBͼ��ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *	  int   Dir		     - ������
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
  *
  * ˵��:
  *   �ú�������ͼ����
  *
  ************************************************************************/

BOOL WINAPI Mirror(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int Dire)
{
	// ָ����ͼ���ָ��
	unsigned char* lpNewDIBBits;

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;

	// ѭ������
	LONG	i;
	LONG	j;

	// ������
	FLOAT	fResult;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	lpNewDIBBits = new BYTE[lLineBytes * lHeight];
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��(��ȥ��Ե����)
	for (i = 0; i < lHeight; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = 0; j < lWidth; j++)
		{
			if (j == 129)
			{
				int t = 0;
			}
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			//�����ˮƽ����
			if (Dire == 0) {
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i)  + lWidth - j;
			}
			//����Ǵ�ֱ����
			else {
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			}
			
			*lpDst = *lpSrc;
		}
	}
	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// ����
	return TRUE;
}

/*************************************************************************
  *
  * �������ƣ�
  *   Rotate()
  *
  * ����:
  *   LPSTR lpDIB		 - ָ��ԴDIBͼ��ָ��
  *   LPSTR lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *	  int   angle	     - ��ת�Ƕ�
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
  *
  * ˵��:
  *   �ú�������ͼ����ת��
  *
  ************************************************************************/

HGLOBAL WINAPI Rotate(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int angle)
{
	// ָ����ͼ���ָ��
	LPSTR lpNewDIB;
	LPSTR lpNewDIBBits;

	// ָ��Դ���ص�ָ��
	LPSTR lpSrc;

	// ָ�������ص�ָ��
	LPSTR lpDst;

	// ѭ������
	LONG	i;
	LONG	j;
	LONG	i0;
	LONG	j0;

	//��ת����¿�Ⱥ͸߶�
	LONG lNewWidth;
	LONG lNewHeight;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	LONG lNewLineBytes;

	// ����ԭͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ԭͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
	float fRotateAngle = (angle * PI * 1.0) / 180;//�Ƕ�ת��Ϊ����
	float fSina = (float)sin((double)fRotateAngle);
	float fCosa = (float)cos((double)fRotateAngle);

	//ԭͼ���ĸ�������
	fSrcX1 = (float)(-(lWidth - 1) / 2);
	fSrcY1 = (float)((lHeight - 1) / 2);
	fSrcX2 = (float)((lWidth - 1) / 2);
	fSrcY2 = (float)((lHeight - 1) / 2);
	fSrcX3 = (float)((lWidth - 1) / 2);
	fSrcY3 = (float)(-(lHeight - 1) / 2);
	fSrcX4 = (float)(-(lWidth - 1) / 2);
	fSrcY4 = (float)(-(lHeight - 1) / 2);

	//��ͼ���ĸ�������
	fDstX1 = fSrcX1 * fCosa + fSrcY1 * fSina;
	fDstY1 = -fSrcX1 * fSina + fSrcY1 * fCosa;
	fDstX2 = fSrcX2 * fCosa + fSrcY2 * fSina;
	fDstY2 = -fSrcX2 * fSina + fSrcY2 * fCosa;
	fDstX3 = fSrcX3 * fCosa + fSrcY3 * fSina;
	fDstY3 = -fSrcX3 * fSina + fSrcY3 * fCosa;
	fDstX4 = fSrcX4 * fCosa + fSrcY4 * fSina;
	fDstY4 = -fSrcX4 * fSina + fSrcY4 * fCosa;

	//��ͼ��ĳ��ȺͿ��
	lNewWidth = (LONG)(max(fabs(fDstX1 - fDstX3), fabs(fDstX2 - fDstX4)) + 0.5);
	lNewHeight = (LONG)(max(fabs(fDstY1 - fDstY3), fabs(fDstY2 - fDstY4)) + 0.5);

	//ӳ�乫ʽ�е���������
	float f1 = (float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina + 0.5 * (lWidth - 1));
	float f2 = (float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa + 0.5 * (lHeight - 1));

	//��ͼ��ÿ�е��ֽ���
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
			// ָ����DIB��i�У���j�����ص�ָ��
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

	// ����
	return hDIB;
}

/*************************************************************************
 *
 * �������ƣ�
 *   AnalysisForeground()
 *
 * ����:
 *   unsigned char * lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - ǰ��Ϊ��ɫ����1��Ϊ��ɫ����0
 *
 * ˵��:
 *   �ú����������ǰ������ɫ
 *
 ************************************************************************/
int WINAPI AnalysisForeground(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	int FOREGROUND;
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	LONG white = 0;
	LONG black = 0;

	//����ɨ��
	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
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
  * �������������
  *
  ************************************************************************/

// ����һ�����ͼ�񲢳�ʼ��Ϊȫ��
std::vector<std::vector<bool>> createVisitedImage(int width, int height) {
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
	return visited;
}

// ʹ�ô����÷�ʽ����ͼ��ķ��ʱ��
void markVisited(std::vector<std::vector<bool>>& visited, int x, int y) {
	visited[y][x] = true;
}

// ʹ�ô�ֵ��ʽ���ͼ��ķ���״̬
bool isVisited(const std::vector<std::vector<bool>>& visited, int x, int y) {
	return visited[y][x];
}

//�߽�׷���㷨
int traceBoundary(const unsigned char* image, int width, int height, int startX, int startY, int FOREGROUND, int connection, unsigned char* lpDstImageData) {
	// ָ�����ص�ָ��
	unsigned char* lpSrc;
	unsigned char* lpDst;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES( width * 8);
	// ���巽��
	int d4x[] = { 1, 0, -1, 0 };
	int d4y[] = { 0, -1, 0, 1 };

	int d8x[] = { 1, 1, 0, -1, -1,-1, 0, 1};
	int d8y[] = { 0, -1,-1,-1,  0, 1, 1, 1};

	// ��¼��ʼ��͵�ǰ��
	int currentX = startX;
	int currentY = startY;
	startX = currentX;
	startY = currentY;

	// ��ʼ���ܳ�
	int perimeter = 0;

	// ��ʼ�߽����
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
		// �����һ�����Ƿ���ͼ��Χ����Ϊǰ������
		if (nextX >= 0 && nextX < width && nextY >= 0 && nextY < height && *lpSrc == FOREGROUND) {
			// ���µ�ǰ��Ϊ��һ����
			currentX = nextX;
			currentY = nextY;
			lpDst = (unsigned char*)lpDstImageData + (WIDTHBYTES(width * 24) * (height - 1 - currentY)) + currentX * 3;
			lpDst[2] = 0;
			lpDst[1] = 0;
			lpDst[0] = 255;
			// �����ܳ�
			perimeter++;
			if (connection == 4) {
				// ���÷���Ϊ��ʱ����ת90��
				direction = (direction + 1) % 4;
			}
			else {
				// ���÷���Ϊ��ʱ����ת90��
				direction = (direction + 1) % 8;
			}
		}
		else {
			if (connection == 4) {
				// ˳ʱ����ת90��
				direction = (direction - 1 + 4) % 4;
			}
			else {
				// ˳ʱ����ת45��
				direction = (direction - 1 + 8) % 8;
			}
		}
	} while (currentX != startX || currentY != startY);  // �ص���ʼ��ʱֹͣ

	return perimeter;
}
/*************************************************************************
  *
  * �������ƣ�
  *   countRegions()
  *
  * ����:
  *   unsigned char *  lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *	  int connection	 - ��ͨ��
  *	  int color			 - ǰ����ɫ
  *
  * ����ֵ:
  *   BOOL               - �ɹ���������������
  *
  * ˵��:
  *   �ú����������������
  *
  ************************************************************************/
int WINAPI countRegions(unsigned char * lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color)
{
	int count = 0;

	// ָ��Դ���ص�ָ��
	unsigned char* lpSrc;
	unsigned char* lpNew;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//��ǰ����ֵ
	int FOREGROUND = 0;
	if (color == 0)  FOREGROUND = 0;//��
	else  FOREGROUND = 255;//��

	//�����������
	int dx[] = { -1, 1, 0, 0 , -1, -1, 1, 1};
	int dy[] = { 0, 0, -1, 1 , -1, 1 ,1, -1};
	int NEIGHBORNUM = 0;
	if (connection == 0) {
		NEIGHBORNUM = 8;
	}
	else {
		NEIGHBORNUM = 4;
	}

	// ����һ�����ͼ������鲢��ʼ��Ϊȫ��
	std::vector<std::vector<bool>> visited = createVisitedImage(lWidth, lHeight);

	// ����ͼ���е���������
	for (int y = 0; y < lHeight; y++) {
		for (int x = 0; x < lWidth; x++) {
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (lHeight - 1 - y) + x;
			// �����ǰ��������δ�����ʹ������ʾ�ҵ���һ���µ�����
			if (*lpSrc == FOREGROUND && !isVisited(visited, x, y)) {
				count++;
				int area = 0;
				// ʹ�ù����������������뵱ǰ��������������ǰ������
				std::queue<std::pair<int, int>> q;
				q.push(std::make_pair(x, y));
				markVisited(visited, x, y);

				while (!q.empty()) {
					area++;
					int currentX = q.front().first;
					int currentY = q.front().second;
					q.pop();

					// ������ڵ�ǰ������
					for (int i = 0; i < NEIGHBORNUM; i++) {
						int newX = currentX + dx[i];
						int newY = currentY + dy[i];

						// ��������Ƿ���ͼ��Χ����δ�����ʹ�
						lpNew = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - newY) + newX;
						if (newX >= 0 && newX < lWidth && newY >= 0 && newY < lHeight && *lpNew == FOREGROUND && !isVisited(visited, newX, newY)) {
							q.push(std::make_pair(newX, newY));
							markVisited(visited, newX, newY);
						}
					}
				}
				//̫С�����岻����
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
  * �������ƣ�
  *   calculateObjectProperties()
  *
  * ����:
  *   unsigned char *  lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *	  int connection	 - ��ͨ��
  *	  int color			 - ǰ����ɫ
  *
  * ����ֵ:
  *   BOOL               - �ɹ���������������
  *
  * ˵��:
  *   �ú����������������
  *
  ************************************************************************/
ObjectProperties WINAPI calculateObjectProperties(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int connection, int color, unsigned char* lpDstImageData) {
	ObjectProperties properties = {0, 0, 0, 0, 0, 0, 0, 0};

	int count = 0;

	// ָ��Դ���ص�ָ��
	unsigned char* lpSrc;
	unsigned char* lpNew;
	unsigned char* lpDst;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//��ǰ����ֵ
	int FOREGROUND = 0;
	if (color == 0)  FOREGROUND = 0;//��
	else  FOREGROUND = 255;//��

	//�����������
	int dx[] = { -1, 1, 0, 0 , -1, -1, 1, 1 };
	int dy[] = { 0, 0, -1, 1 , -1, 1 ,1, -1 };
	int NEIGHBORNUM = 0;
	if (connection == 0) {
		NEIGHBORNUM = 8;
	}
	else {
		NEIGHBORNUM = 4;
	}

	// ����һ�����ͼ������鲢��ʼ��Ϊȫ��
	std::vector<std::vector<bool>> visited = createVisitedImage(lWidth, lHeight);

	for (int y = 0; y < lHeight; y++) {
		for (int x = 0; x < lWidth; x++) {
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - y) + x;
			// �����ǰ��������δ�����ʹ������ʾ�ҵ���һ���µ�����
			if (*lpSrc == FOREGROUND && !isVisited(visited, x, y)) {
				// ʹ�ù�������������������������
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

					// ��������ı߽��
					minX = min(minX, currentX);
					maxX = max(maxX, currentX);
					minY = min(minY, currentY);
					maxY = max(maxY, currentY);

					// ������������
					area++;
					// ������ڵ�ǰ������
					for (int i = 0; i < NEIGHBORNUM; i++) {
						int newX = currentX + dx[i];
						int newY = currentY + dy[i];

						// ��������Ƿ���ͼ��Χ����δ�����ʹ�
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
				// ����������������
				if (area > properties.area) {
					properties.area = area;
					//���ζȺ���������
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
	//�߽�׷�����ܳ�
	properties.perimeter = traceBoundary(lpDIBBits, lWidth, lHeight, properties.startX, properties.startY, FOREGROUND, connection, lpDstImageData);
	// ���������Բ�ζ�
	properties.circularity = (4 * 3.14159 * properties.area) / (properties.perimeter * properties.perimeter);
	return properties;
}

/*************************************************************************
  *
  * �������ƣ�
  *   Stylize1()
  *
  * ����:
  *   unsigned char *  lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *   int lpSrcBitCount  - λ��
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE
  *
  * ˵��:
  *   �ú������ڵ�һ�ַ�񻯡�
  *
  ************************************************************************/
BOOL WINAPI Stylize1(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount) {
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ������ɫͨ������Ӱ��Ǻ͹���Ч������Ӧ���ϻ���������
	for (int i = 0; i < lHeight; i++) {
		for (int j = 0; j < lWidth; j++) {
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j * 3;

			int red = lpSrc[2];
			int green = lpSrc[1];
			int blue = lpSrc[0];

			// ������ɫͨ����ƫ���ɫ����ɫ
			int newRed = static_cast<int>(0.7 * red + 0.3 * green);
			int newGreen = static_cast<int>(0.6 * green + 0.4 * red);
			int newBlue = static_cast<int>(0.6 * blue);

			// ��Ӱ���Ч��
			double distance = std::sqrt((j - lWidth / 2.0) * (j - lWidth / 2.0) + (i - lHeight / 2.0) * (i - lHeight / 2.0));
			double vignette = 1.0 - 0.8 * (distance / (lWidth / 2.0));
			newRed = static_cast<int>(newRed * vignette);
			newGreen = static_cast<int>(newGreen * vignette);
			newBlue = static_cast<int>(newBlue * vignette);

			// ��ӹ���Ч��
			double glow = 1.0 - 0.5 * (distance / (lWidth / 2.0));
			newRed += static_cast<int>(100 * glow);
			newGreen += static_cast<int>(80 * glow);
			newBlue += static_cast<int>(60 * glow);

			// ȷ������ֵ�� 0-255 ��Χ��
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

			// Ӧ���ϻ�����
			if (rand() % 100 < 10) {
				// 30% �ĸ����������Ч��
				int texture = rand() % 10 - 25;
				newRed += texture;
				newGreen += texture;
				
			}

			// ������
			if (rand() % 100 < 5) {
				// 5% �ĸ���������
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
  * �������ƣ�
  *   Stylize2()
  *
  * ����:
  *   unsigned char *  lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *   int lpSrcBitCount  - λ��
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE
  *
  * ˵��:
  *   �ú������ڵڶ��ַ�񻯡�
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
	// �������ͼ������

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
  * �������ƣ�
  *   Stylize3()
  *
  * ����:
  *   unsigned char *  lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *   int lpSrcBitCount  - λ��
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE
  *
  * ˵��:
  *   �ú������ڵ����ַ�񻯡�
  *
  ************************************************************************/
BOOL WINAPI Stylize3(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, int lpSrcBitCount) {
	// ����ÿ��������ռ���ֽ���
	int LineBytes = WIDTHBYTES(lWidth * lpSrcBitCount);

	// ����OpenCV Mat����������ͼ��
	unsigned char* tempBits = lpDIBBits;
	cv::Mat frame(lHeight, lWidth, CV_8UC3, tempBits);

	// ת��Ϊ�Ҷ�ͼ��
	cv::Mat gray;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

	// ��Ե��ȡ
	cv::Mat edges;
	cv::Canny(gray, edges, 120, 240);

	// ���ɱ�Ե��Ĥ
	cv::Mat mask;
	cv::threshold(edges, mask, 120, 255, cv::THRESH_BINARY_INV);

	// �����²���
	cv::Mat downsampled;
	cv::pyrDown(frame, downsampled);
	cv::pyrDown(downsampled, downsampled);

	// ˫���˲�
	cv::Mat filtered;
	cv::bilateralFilter(downsampled, filtered, 5, 30, 20);

	// �����²���
	cv::pyrDown(filtered, downsampled);
	cv::pyrDown(downsampled, downsampled);

	// �ָ�ͼ���С
	cv::Mat result;
	cv::pyrUp(downsampled, result);
	cv::pyrUp(result, result);

	// ����ͼ�񱥺Ͷ�
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

	// ��Ĥ����
	cv::Mat output;
	frame.copyTo(output, mask);

	// ��������ƻ�lpDIBBits
	tempBits = lpDIBBits;
	std::memcpy(tempBits, output.data, LineBytes * lHeight);

	return TRUE;
}


/*************************************************************************
  *
  * canny��Ե��⺯����
  *
  ************************************************************************/
// �����ݶȷ�ֵ�ͷ���
void computeGradient(unsigned char* image, int width, int height, int* gradientMagnitude, int* gradientDirection)
{
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(width * 8);
	// ʹ��Sobel���Ӽ����ݶ�
	int sobelX[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int sobelY[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			int gx = 0;
			int gy = 0;

			// ����x�����ݶ�
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					lpSrc = (unsigned char*)image + lLineBytes * (y + i) + (x + j);
					int pixelValue = *lpSrc;
					gx += sobelX[i + 1][j + 1] * pixelValue;
				}
			}

			// ����y�����ݶ�
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					lpSrc = (unsigned char*)image + lLineBytes * (y + i) + (x + j);
					int pixelValue = *lpSrc;
					gy += sobelY[i + 1][j + 1] * pixelValue;
				}
			}

			// �����ݶȷ�ֵ�ͷ���
			gradientMagnitude[y * lLineBytes + x] = sqrt(gx * gx + gy * gy);
			gradientDirection[y * lLineBytes + x] = atan2(gy, gx) * 180 / PI;
		}
	}
}

// �Ǽ���ֵ����
void nonMaxSuppression(int* gradientMagnitude, int* gradientDirection, int width, int height, unsigned char* edges)
{
	LONG lLineBytes = WIDTHBYTES(width * 8);
	for (int y = 1; y < height - 1; ++y)
	{
		for (int x = 1; x < width - 1; ++x)
		{
			int angle = gradientDirection[y * lLineBytes + x];

			// �����ݶȷ�����зǼ���ֵ����
			int pixelValue = gradientMagnitude[y * lLineBytes + x];
			int n1, n2;

			// �����ݶȷ���ȷ�����Ʒ���
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

			// ���зǼ���ֵ����
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

// ˫��ֵ����
void doubleThreshold(unsigned char* edges, int width, int height, int lowThreshold, int highThreshold)
{
	LONG lLineBytes = WIDTHBYTES(width * 8);
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			unsigned char pixelValue = edges[y * lLineBytes + x];

			// ������ֵ���зָ�
			if (pixelValue >= highThreshold)
			{
				edges[y * lLineBytes + x] = 255;  // ǿ��Ե
			}
			else if (pixelValue >= lowThreshold)
			{
				edges[y * lLineBytes + x] = 128;  // ����Ե
			}
			else
			{
				edges[y * lLineBytes + x] = 0;    // ����
			}
		}
	}
}

// ��Ե����
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
				// ���8�����Ƿ����ǿ��Ե
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
  * �������ƣ�
  *   cannyEdgeDetection()
  *
  * ����:
  *   unsigned char *  lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE
  *
  * ˵��:
  *   Canny��Ե����㷨
  *
  ************************************************************************/
BOOL WINAPI cannyEdgeDetection(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 8);
	// ��˹ƽ��
	float coff = 1.0 / 16;
	float fpArray[9] = { 1,2,1,2,4,2,1,2,1 };
	Template(lpDIBBits, lWidth, lHeight, fpArray, coff, 0);

	// �����ݶȷ�ֵ�ͷ���
	int* gradientMagnitude = new int[lLineBytes * lHeight];
	int* gradientDirection = new int[lLineBytes * lHeight];
	computeGradient(lpDIBBits, lWidth, lHeight, gradientMagnitude, gradientDirection);

	// �Ǽ���ֵ����
	unsigned char* edges = new unsigned char[lLineBytes * lHeight];
	nonMaxSuppression(gradientMagnitude, gradientDirection, lWidth, lHeight, edges);

	// ˫��ֵ����
	int lowThreshold = 50;
	int highThreshold = 150;
	doubleThreshold(edges, lWidth, lHeight, lowThreshold, highThreshold);

	// ��Ե����
	edgeTracking(edges, lWidth, lHeight);

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, edges, lLineBytes * lHeight);
	// �����ڴ�
	delete[] gradientMagnitude;
	delete[] gradientDirection;
	delete[] edges;
	return TRUE;
}

/*************************************************************************
  *
  * �������ƣ�
  *   usmSharpening()
  *
  * ����:
  *   unsigned char *  lpDIBBits    - ָ��ԴDIB����ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE
  *
  * ˵��:
  *   usm�㷨
  *
  ************************************************************************/
// USM�񻯺���
BOOL WINAPI usmSharpening(unsigned char* lpDIBBits, LONG lWidth, LONG lHeight, float amount, float threshold)
{
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 8);

	// ������ʱ������
	unsigned char* blurred = new unsigned char[lLineBytes * lHeight];
	memcpy(blurred, lpDIBBits, lLineBytes * lHeight);

	unsigned char* mask = new unsigned char[lLineBytes * lHeight];
	unsigned char* sharpened = new unsigned char[lLineBytes * lHeight];

	// ����ģ��ͼ��
	// ��˹ƽ��
	float coff = 1.0 / 16;
	float fpArray[9] = { 1,2,1,2,4,2,1,2,1 };
	Template(blurred, lWidth, lHeight, fpArray, coff, 0);

	// ������Ĥ����ͼ��
	for (int y = 0; y < lHeight; y++)
	{
		for (int x = 0; x < lWidth; x++)
		{
			int diff = lpDIBBits[y * lLineBytes + x] - blurred[y * lLineBytes + x];
			mask[y * lLineBytes + x] = (abs(diff) > threshold) ? 255 : 0;
			sharpened[y * lLineBytes + x] = lpDIBBits[y * lLineBytes + x] + amount * mask[y * lLineBytes + x] * diff / 255;
		}
	}

	// ����ԭʼͼ��
	memcpy(lpDIBBits, sharpened, lLineBytes * lHeight);

	// �ͷ���ʱ������
	delete[] blurred;
	delete[] mask;
	delete[] sharpened;

	return TRUE;
}