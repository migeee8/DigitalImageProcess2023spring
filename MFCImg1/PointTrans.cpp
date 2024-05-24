
/**************************************************************************
 *  �ļ�����PointTrans.cpp
 *
 *  ͼ�������API�����⣺
 *
 *  AddBrightness()		-�ı�ͼ������
 *	Trans24cTo8c()      -ת��24λ��ɫͼ��8λ��ɫͼ��
 *	Trans8cTo8g()		-ת��8λ��ɫͼ��8λ�Ҷ�ͼ��
 *	Trans8gToBin()		-ת��8λ�Ҷ�ͼ�񵽶�ֵͼ��
 *	HalfTone()			-��ͼ����а�Ӱ������
 *	LinerTrans()		-����ͼ������Ա任
 *	Pw2LinerTrans()		-�ú�����������ͼ��ķֶ����Ա任��������������
 *	logTrans()			-����ͼ���log�任
 *	GammaTrans()		-����ͼ���gamma�任
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

//����Ϊ0-255֮�������
unsigned char LimitPixelValue(int value)
{
	return value > 255 ? 255 : value < 0 ? 0 : static_cast<unsigned char>(value + 0.5);
}
//�������㷵��б�ʺͽؾ�
std::pair<float, float> calculateLineEquation(float x1, float y1, float x2, float y2) {
	float slope = (y2 - y1) / (x2 - x1);
	float y_intercept = y1 - slope * x1;
	return std::make_pair(slope, y_intercept);
}

 /*************************************************************************
  *
  * �������ƣ�
  *   AddBrightness()
  *
  * ����:
  *   LPSTR lpDIBBits    - ָ��DIBԴ����ָ��
  *   int BitCount       - ͼ��λ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *   int add            - �仯ֵ
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
  *
  * ˵��:
  *   �ú��������ı�ͼ������
  *
  ************************************************************************/

BOOL WINAPI AddBrightness(LPSTR lpDIBBits, int BitCount, LONG lWidth, LONG lHeight, int add)
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * BitCount);
	//ÿ������ռ�ݵ��ֽ���
	const int page = BitCount == 24 ? 3 : 1;
	//����ɨ��
	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ���i�У���j�����ص�ָ��
			// ����������ÿ���ֽ�
			for (int k = 0; k < page; k++)
			{
				//ָ��ƫ����
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j * page + k;
				//�����µ����Ȳ���ֵ
				*lpSrc = LimitPixelValue(*lpSrc + add);
			}
		}
	}
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   Trans24cTo8c()
 *
 * ����:
 *   LPSTR lpSrcDib		- ָ��Դͼ��ָ��
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������ת��24λ��ɫͼ��8λ��ɫͼ��
 *
 ************************************************************************/
BOOL WINAPI Trans24cTo8c(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight) 
{
	// ָ��Դͼ���ָ��
    unsigned char* lpSrc;
    // ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 24);

	//����8λ��ɫ��ͼ���Ԫ��
    BITMAPINFOHEADER* pHead;
	int lineByteNew = WIDTHBYTES(lWidth * 8);
    RGBQUAD* pColorTable = NULL;    //������ɫ��
    pColorTable = new RGBQUAD[256];
    memset(pColorTable, 0, sizeof(RGBQUAD) * 256);
    BYTE* Index = new BYTE[lineByteNew * lHeight]; //ͼ��������������
    WORD* shortColor = new WORD[lineByteNew * lHeight]; //��ɫ�ĸ�4λ
	memset(shortColor, 0, sizeof(WORD) * lineByteNew * lHeight); //����

	int iRed, iGreen, iBlue;
    for (size_t i = 0; i < lHeight; i++)
    {//ȡRGB��ɫ�ĸ�4λ  
        for (size_t j = 0; j < lWidth; j++)
        {
            lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j * 3;
            iBlue = (*lpSrc) >> 4;
            iGreen = (*(++lpSrc)) >> 4;
            iRed = (*(++lpSrc)) >> 4;
            shortColor[lineByteNew * i + j] = (iBlue << 8) + (iGreen << 4) + iRed;
        }
    }

    //24λת8λ
    Transfer(shortColor, lineByteNew * lHeight, Index, pColorTable);

    //����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
    unsigned char* pBmpBufNew;
    pBmpBufNew = new unsigned char[sizeof(BITMAPINFOHEADER) + 256 * 4];//������ͼ��Ŀռ�
    memcpy(pBmpBufNew, (unsigned char*)lpSrcDib, sizeof(BITMAPINFOHEADER));//��Ϣͷ����
    pHead = (BITMAPINFOHEADER*)pBmpBufNew;
    pHead->biBitCount = 8;//�ı�λ��,
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

    //����
    memcpy(lpSrcDib, pBmpBufNew, sizeof(BITMAPINFOHEADER));
    memcpy(lpSrcDib + sizeof(BITMAPINFOHEADER), pColorTable, sizeof(RGBQUAD) * 256);
    memcpy(lpSrcDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256, Index, lineByteNew * lHeight);

    delete[]Index;
    delete[]shortColor;

    return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   Trans8cTo8g()
 *
 * ����:
 *   LPSTR lpSrcDib		- ָ��Դͼ��ָ��
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������ת��8λ��ɫͼ��8λ�Ҷ�ͼ��
 *
 ************************************************************************/

BOOL WINAPI Trans8cTo8g(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);

	// �Ҷ�ӳ���
	BYTE bMap[256];
	RGBQUAD* pTable;//��ɫ��ָ��

	pTable = (RGBQUAD*)(lpSrcDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)//�����µĵ�ɫ��,��ת���Ҷ�
	{
		// �������ɫ��Ӧ�ĻҶ�ֵg=0.299*r+0.587*g+0.114*b	
		bMap[i] = (BYTE)(0.299 * pTable->rgbRed + 0.587 * pTable->rgbGreen + 0.114 * pTable->rgbBlue + 0.5);
		pTable->rgbRed = (BYTE)i;
		pTable->rgbGreen = (BYTE)i;
		pTable->rgbBlue = (BYTE)i;
		pTable->rgbReserved = (BYTE)0;
		pTable++;
	}
	// ����ÿ�����ص���ɫ�����������ջҶ�ӳ����ɻҶ�ֵ��
	//����ɨ��
	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			// ���µ�����ֵ
			*lpSrc = bMap[*lpSrc];
		}
	}
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   Trans8gToBin()
 *
 * ����:
 *   LPSTR lpSrcDib		- ָ��Դͼ��ָ��
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
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
BOOL WINAPI Trans8gToBin(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int threshold)
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
 *   HalfTone()
 *
 * ����:
 *   LPSTR lpSrcDib		- ָ��Դͼ��ָ��
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ����а�Ӱ������
 *
 ************************************************************************/
BOOL WINAPI HalfTone(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lWidth, LONG lHeight )
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ͼ��ÿ�е��ֽ���
	const size_t lLineBytes = WIDTHBYTES(lWidth * 8);
	//��ֵ
	int threshold=180;
	//����������
	unsigned char** err;
	err = new unsigned char* [lHeight];
	// 
	for (int i = 0; i < lHeight; i++) {
		err[i] = new unsigned char[lWidth];
		for (int j = 0; j < lWidth; j++)
			err[i][j] = 0;
	}
	//����ɨ��
	for (size_t i = 0; i < lHeight; i++)
	{
		//����ɨ��
		for (size_t j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			// ���㵱ǰ�������������ֵ�����ֵ
			int err_val = *lpSrc + err[i][j] - threshold;
			if (err_val > 0) {
				*lpSrc = 255;
			}
			else {
				*lpSrc = 0;
			}
			// �����ֵ��ɢ����Χ��������
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
 * �������ƣ�
 *   LinerTrans()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   LONG  fA			- ���Ա任������б��
 *   LONG  fB			- ���Ա任�����Ľؾ�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����������ͼ������Ա任
 *
 ************************************************************************/
BOOL WINAPI LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT fA, FLOAT fB)
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
			// ָ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j ;
			//�������Ա任��ĺ���ֵ����ֵ
			*lpSrc = LimitPixelValue(fA*(*lpSrc)+fB);	
		}
	}
	return true;
}


/*************************************************************************
 *
 * �������ƣ�
 *   Pw2LinerTrans()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   LONG  slope1		- ���Ա任��һ�κ�����б��
 *   LONG  intercept1	- ���Ա任��һ�κ����Ľؾ�
 *   LONG  slope1		- ���Էֶκ���ת�۵��x������
 *   LONG  slope2		- ���Ա任�ڶ��κ�����б��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����������ͼ��ķֶ����Ա任��������������
 *
 ************************************************************************/
BOOL WINAPI Pw2LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, Imgpoint A, Imgpoint B, Imgpoint C)
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
			// ָ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
		    //�ֱ�������εĺ����ؾ��б��
			std::pair<float, float> line_equation1 = calculateLineEquation(A.getX(), A.getY(), B.getX(), B.getY());
			std::pair<float, float> line_equation2 = calculateLineEquation(B.getX(), B.getY(), C.getX(), C.getY());
			//���صĻҶ�ֵ�ڵ�һ�κ�����
			if(*lpSrc < B.getX())
				*lpSrc = LimitPixelValue(line_equation1.first * (*lpSrc)+ line_equation1.second);
			//���صĻҶ�ֵ�ڵڶ��κ�����
			else *lpSrc = LimitPixelValue(line_equation2.first * (*lpSrc) + line_equation2.second);
		}
	}
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   logTrans()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����������ͼ���log�任
 *
 ************************************************************************/
BOOL WINAPI logTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight) 
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
			// ָ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			//������������Ľ������ֵ
			*lpSrc = (unsigned char)(255 * log((*lpSrc + 1.0)) / log(255));
		}
	}
	return true;
}

/*************************************************************************
 *
 * �������ƣ�
 *   GammaTrans()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��Դ����ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *	 float gamma        - ٤��任�Ĳ���ֵ
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����������ͼ���gamma�任
 *
 ************************************************************************/
BOOL WINAPI GammaTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,float gamma)
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
			// ָ���i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			//����٤��任��ĺ���ֵ����ֵ
			*lpSrc = unsigned char(pow(*lpSrc / 255.0, gamma) * 255);
		}
	}
	return true;
}