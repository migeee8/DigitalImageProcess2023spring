
/**************************************************************************
 *  文件名：PointTrans.cpp
 *
 *  图像点运算API函数库：
 *
 *  AddBrightness()		-改变图像亮度
 *	Trans24cTo8c()      -转换24位彩色图像到8位彩色图像
 *	Trans8cTo8g()		-转换8位彩色图像到8位灰度图像
 *	Trans8gToBin()		-转换8位灰度图像到二值图像
 *	HalfTone()			-对图像进行半影调处理
 *	LinerTrans()		-进行图像的线性变换
 *	Pw2LinerTrans()		-该函数用来进行图像的分段线性变换（两段且连续）
 *	logTrans()			-进行图像的log变换
 *	GammaTrans()		-进行图像的gamma变换
 *
 *************************************************************************/

#include "stdafx.h"
#include "PointTrans.h"
#include "DIBAPI.h"
#include"transformula.h"
#include <math.h>
#include <utility>
#include <direct.h>
#include <cmath>
#include <algorithm>

extern int Transfer(WORD* color24bit, LONG len, BYTE* Index, RGBQUAD* mainColor);

//限制为0-255之间的整数
unsigned char LimitPixelValue(int value)
{
	return value > 255 ? 255 : value < 0 ? 0 : static_cast<unsigned char>(value + 0.5);
}
//给定两点返回斜率和截距
std::pair<float, float> calculateLineEquation(float x1, float y1, float x2, float y2) {
	float slope = (y2 - y1) / (x2 - x1);
	float y_intercept = y1 - slope * x1;
	return std::make_pair(slope, y_intercept);
}

 /*************************************************************************
  *
  * 函数名称：
  *   AddBrightness()
  *
  * 参数:
  *   LPSTR lpDIBBits    - 指向DIB源像素指针
  *   int BitCount       - 图像位数
  *   LONG  lWidth       - 源图像宽度（像素数）
  *   LONG  lHeight      - 源图像高度（像素数）
  *   int add            - 变化值
  *
  * 返回值:
  *   BOOL               - 成功返回TRUE，否则返回FALSE。
  *
  * 说明:
  *   该函数用来改变图像亮度
  *
  ************************************************************************/

BOOL WINAPI AddBrightness(LPSTR lpDIBBits, int BitCount, LONG lWidth, LONG lHeight, int add)
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * BitCount);
	//每个像素占据的字节数
	const int page = BitCount == 24 ? 3 : 1;
	//逐行扫描
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向第i行，第j个像素的指针
			// 遍历像素内每个字节
			for (int k = 0; k < page; k++)
			{
				//指针偏移量
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j * page + k;
				//计算新的亮度并赋值
				*lpSrc = LimitPixelValue(*lpSrc + add);
			}
		}
	}
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   Trans24cTo8c()
 *
 * 参数:
 *   LPSTR lpSrcDib		- 指向源图像指针
 *   LPSTR lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来转换24位彩色图像到8位彩色图像
 *
 ************************************************************************/
BOOL WINAPI Trans24cTo8c(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight) 
{
	// 指向源图像的指针
    unsigned char* lpSrc;
    // 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 24);

	//创建8位彩色新图像的元素
    BITMAPINFOHEADER* pHead;
	int lineByteNew = WIDTHBYTES(lWidth * 8);
    RGBQUAD* pColorTable = NULL;    //创建颜色表
    pColorTable = new RGBQUAD[256];
    memset(pColorTable, 0, sizeof(RGBQUAD) * 256);
    BYTE* Index = new BYTE[lineByteNew * lHeight]; //图像数据区的数据
    WORD* shortColor = new WORD[lineByteNew * lHeight]; //颜色的高4位
	memset(shortColor, 0, sizeof(WORD) * lineByteNew * lHeight); //清零

	int iRed, iGreen, iBlue;
    for (size_t i = 0; i < lHeight; i++)
    {//取RGB颜色的高4位  
        for (size_t j = 0; j < lWidth; j++)
        {
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j * 3;
            iBlue = (*lpSrc) >> 4;
            iGreen = (*(++lpSrc)) >> 4;
            iRed = (*(++lpSrc)) >> 4;
            shortColor[lineByteNew * i + j] = (iBlue << 8) + (iGreen << 4) + iRed;
        }
    }

    //24位转8位
    Transfer(shortColor, lineByteNew * lHeight, Index, pColorTable);

    //申请位图数据所需要的空间，读位图数据进内存
    unsigned char* pBmpBufNew;
    pBmpBufNew = new unsigned char[sizeof(BITMAPINFOHEADER) + 256 * 4];//申请新图像的空间
    memcpy(pBmpBufNew, (unsigned char*)lpSrcDib, sizeof(BITMAPINFOHEADER));//信息头拷贝
    pHead = (BITMAPINFOHEADER*)pBmpBufNew;
    pHead->biBitCount = 8;//改变位数,
    pHead->biHeight = lHeight;
    pHead->biWidth = lWidth;
    pHead->biClrUsed = 256;
    pHead->biClrImportant = 0;
    pHead->biCompression = 0;
    pHead->biPlanes = 1;
    pHead->biSize = 40;
    pHead->biSizeImage = lineByteNew * lHeight;
    pHead->biXPelsPerMeter = 0;
    pHead->biYPelsPerMeter = 0;

    //拷贝
    memcpy(lpSrcDib, pBmpBufNew, sizeof(BITMAPINFOHEADER));
    memcpy(lpSrcDib + sizeof(BITMAPINFOHEADER), pColorTable, sizeof(RGBQUAD) * 256);
    memcpy(lpSrcDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256, Index, lineByteNew * lHeight);

    delete[]Index;
    delete[]shortColor;

    return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   Trans8cTo8g()
 *
 * 参数:
 *   LPSTR lpSrcDib		- 指向源图像指针
 *   LPSTR lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来转换8位彩色图像到8位灰度图像
 *
 ************************************************************************/

BOOL WINAPI Trans8cTo8g(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	// 灰度映射表
	BYTE bMap[256];
	RGBQUAD* pTable;//颜色表指针

	pTable = (RGBQUAD*)(lpSrcDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)//生成新的调色板,并转换灰度
	{
		// 计算该颜色对应的灰度值g=0.299*r+0.587*g+0.114*b	
		bMap[i] = (BYTE)(0.299 * pTable->rgbRed + 0.587 * pTable->rgbGreen + 0.114 * pTable->rgbBlue + 0.5);
		pTable->rgbRed = (BYTE)i;
		pTable->rgbGreen = (BYTE)i;
		pTable->rgbBlue = (BYTE)i;
		pTable->rgbReserved = (BYTE)0;
		pTable++;
	}
	// 更换每个象素的颜色索引（即按照灰度映射表换成灰度值）
	//逐行扫描
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			// 赋新的索引值
			*lpSrc = bMap[*lpSrc];
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   Trans8gToBin()
 *
 * 参数:
 *   LPSTR lpSrcDib		- 指向源图像指针
 *   LPSTR lpDIBBits    - 指向源像素指针
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
BOOL WINAPI Trans8gToBin(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int threshold)
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
 *   HalfTone()
 *
 * 参数:
 *   LPSTR lpSrcDib		- 指向源图像指针
 *   LPSTR lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行半影调处理
 *
 ************************************************************************/
BOOL WINAPI HalfTone(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight )
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 图像每行的字节数
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	//阈值
	int threshold=180;
	//创建误差矩阵
	unsigned char** err;
	err = new unsigned char* [lHeight];
	// 
	for (int i = 0; i < lHeight; i++) {
		err[i] = new unsigned char[lWidth];
		for (int j = 0; j < lWidth; j++)
			err[i][j] = 0;
	}
	//逐行扫描
	for (size_t i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (size_t j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			// 计算当前像素与最近的阈值的误差值
			int err_val = *lpSrc + err[i][j] - threshold;
			if (err_val > 0) {
				*lpSrc = 255;
			}
			else {
				*lpSrc = 0;
			}
			// 将误差值扩散到周围的像素上
			if (j + 1 < lWidth) {
				err[i][j + 1] += err_val * 7 / 16;
			}
			if (i + 1 < lHeight && j - 1 >= 0) {
				err[i + 1][j - 1] += err_val * 3 / 16;
			}
			if (i + 1 < lHeight) {
				err[i + 1][j] += err_val * 5 / 16;
			}
			if (i + 1 < lHeight && j + 1 < lWidth) {
				err[i + 1][j + 1] += err_val * 1 / 16;
			}
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   LinerTrans()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   LONG  fA			- 线性变换函数的斜率
 *   LONG  fB			- 线性变换函数的截距
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来进行图像的线性变换
 *
 ************************************************************************/
BOOL WINAPI LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT fA, FLOAT fB)
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
			// 指向第i行，第j个像素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j ;
			//计算线性变换后的函数值并赋值
			*lpSrc = LimitPixelValue(fA*(*lpSrc)+fB);	
		}
	}
	return true;
}


/*************************************************************************
 *
 * 函数名称：
 *   Pw2LinerTrans()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *   LONG  slope1		- 线性变换第一段函数的斜率
 *   LONG  intercept1	- 线性变换第一段函数的截距
 *   LONG  slope1		- 线性分段函数转折点的x轴坐标
 *   LONG  slope2		- 线性变换第二段函数的斜率
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来进行图像的分段线性变换（两段且连续）
 *
 ************************************************************************/
BOOL WINAPI Pw2LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, Imgpoint A, Imgpoint B, Imgpoint C)
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
			// 指向第i行，第j个像素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
		    //分别计算两段的函数截距和斜率
			std::pair<float, float> line_equation1 = calculateLineEquation(A.getX(), A.getY(), B.getX(), B.getY());
			std::pair<float, float> line_equation2 = calculateLineEquation(B.getX(), B.getY(), C.getX(), C.getY());
			//像素的灰度值在第一段函数上
			if(*lpSrc < B.getX())
				*lpSrc = LimitPixelValue(line_equation1.first * (*lpSrc)+ line_equation1.second);
			//像素的灰度值在第二段函数上
			else *lpSrc = LimitPixelValue(line_equation2.first * (*lpSrc) + line_equation2.second);
		}
	}
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   logTrans()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来进行图像的log变换
 *
 ************************************************************************/
BOOL WINAPI logTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight) 
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
			// 指向第i行，第j个像素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			//计算对数运算后的结果并赋值
			*lpSrc = (unsigned char)(255 * log((*lpSrc + 1.0)) / log(255));
		}
	}
	return true;
}

/*************************************************************************
 *
 * 函数名称：
 *   GammaTrans()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源像素指针
 *   LONG  lWidth       - 源图像宽度（像素数）
 *   LONG  lHeight      - 源图像高度（像素数）
 *	 float gamma        - 伽马变换的参数值
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来进行图像的gamma变换
 *
 ************************************************************************/
BOOL WINAPI GammaTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,float gamma)
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
			// 指向第i行，第j个像素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			//计算伽马变换后的函数值并赋值
			*lpSrc = unsigned char(pow(*lpSrc / 255.0, gamma) * 255);
		}
	}
	return true;
}