
/**************************************************************************
 *  �ļ�����TemplateTrans.cpp
 *
 *  ͼ��ģ��任API�����⣺
 *
 *  Template()			- ͼ��ģ��任��ͨ���ı�ģ�壬��������ʵ��
 *						  ͼ���ƽ�����񻯡���Եʶ��Ȳ�����
 *  MedianFilter()		- ͼ����ֵ�˲�����������ȥ����������
 *************************************************************************/

#include "stdafx.h"
#include "TemplateTrans.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>
#include <algorithm>

 /*************************************************************************
  *
  * �������ƣ�
  *   Template()
  *
  * ����:
  *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *   int   iTempH		- ģ��ĸ߶�
  *   int   iTempW		- ģ��Ŀ��
  *   int   iTempMX		- ģ�������Ԫ��X���� ( < iTempW - 1)
  *   int   iTempMY		- ģ�������Ԫ��Y���� ( < iTempH - 1)
  *	 FLOAT * fpArray	- ָ��ģ�������ָ��
  *	 FLOAT fCoef		- ģ��ϵ��
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
  *
  * ˵��:
  *   �ú�����ָ����ģ�壨�����С������ͼ����в���������iTempHָ��ģ��
  * �ĸ߶ȣ�����iTempWָ��ģ��Ŀ�ȣ�����iTempMX��iTempMYָ��ģ�������
  * Ԫ�����꣬����fpArrayָ��ģ��Ԫ�أ�fCoefָ��ϵ����
  *
  ************************************************************************/

BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,
	int iTempH, int iTempW,
	int iTempMX, int iTempMY,
	FLOAT* fpArray, FLOAT fCoef)
{
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

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

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��(��ȥ��Ե����)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			// ����
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					// ��������ֵ
					fResult += (*lpSrc) * fpArray[k * iTempW + l];
				}
			}

			// ����ϵ��
			fResult *= fCoef;

			// ȡ����ֵ
			fResult = (FLOAT)fabs(fResult);

			// �ж��Ƿ񳬹�255
			if (fResult > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				*lpDst = 255;
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

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;

}

/*************************************************************************
  *
  * �������ƣ�
  *   MedianFilter()
  *
  * ����:
  *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
  *   LONG  lWidth       - Դͼ���ȣ���������
  *   LONG  lHeight      - Դͼ��߶ȣ���������
  *   int   iMaskSize	- ģ��Ĵ�С
  *
  * ����ֵ:
  *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
  *
  * ˵��:
  *   �ú�����ͼ�������ֵ�˲�
  *
  ************************************************************************/

BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,int iMaskSize)
{
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

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

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��(��ȥ��Ե����)
	for (i = iMaskSize / 2; i < lHeight - iMaskSize / 2; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = iMaskSize / 2; j < lWidth - iMaskSize / 2; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			LPBYTE lpDst = (LPBYTE)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// ������ʱ���飬������Χ������ֵ
			int* piArray = new int[iMaskSize * iMaskSize];

			// ����Χ������ֵ������ʱ������
			int iIndex = 0;
			for (k = i - iMaskSize / 2; k <= i + iMaskSize / 2; k++)
			{
				for (l = j - iMaskSize / 2; l <= j + iMaskSize / 2; l++)
				{
					// ָ��DIB��k�У���l�����ص�ָ��
					LPBYTE lpSrc = (LPBYTE)lpDIBBits + lLineBytes * (lHeight - 1 - k) + l;
					piArray[iIndex++] = *lpSrc;
				}
			}

			// ����ʱ�����������
			std::sort(piArray, piArray + iMaskSize * iMaskSize);

			// ȡ��ֵ
			int iMedian = piArray[iMaskSize * iMaskSize / 2];

			// ��ֵ
			*lpDst = (BYTE)iMedian;

			// �ͷ���ʱ����
			delete[] piArray;
				
		}

	}
	
	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;

}