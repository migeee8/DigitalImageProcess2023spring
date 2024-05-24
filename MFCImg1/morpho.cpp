// ************************************************************************
//  �ļ�����morph.cpp
//
//  ͼ����̬ѧ�任API�����⣺
//
//  ErosionDIB()    - ͼ��ʴ
//  DilationDIB()	- ͼ������
//  OpenDIB()		- ͼ������
//  CloseDIB()		- ͼ�������
//  O_BorderDIB()	- ͼ������߽�
//	I_BorderDIB()   - ͼ�����ڱ߽�
// ************************************************************************


#include "stdafx.h"
#include "morpho.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
 *
 * �������ƣ�
 *   ErosiontionDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- ��ʴ��ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *	 int	color       - ǰ����ɫ��0Ϊ��ɫ��1Ϊ��ɫ��
 *
 * ����ֵ:
 *   BOOL               - ��ʴ�ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����и�ʴ���㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 *
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color)
{

	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;
	
	//ǰ����ɫ����
	//color == 0 ǰ��Ϊ��ɫ
	//color == 1 ǰ��Ϊ��ɫ
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ���ǰ��ɫ
				*lpDst = (unsigned char)object;

				//���Դͼ���е�ǰ���������������һ���㲻��ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɱ���ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == backgd)
					{
						*lpDst = (unsigned char)backgd;
						break;
					}
				}

			}
		}

	}
	else if (nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ���ǰ��ɫ
				*lpDst = (unsigned char)object;

				//���Դͼ���е�ǰ���������������һ���㲻��ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɱ���ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1) * lWidth);
					if (pixel == backgd)
					{
						*lpDst = (unsigned char)backgd;
						break;
					}
				}

			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ���ǰ��ɫ
				*lpDst = (unsigned char)object;

				//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ������ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɱ���ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (structure[m][n] == 0)
							continue;
						pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
						if (pixel == backgd)
						{
							*lpDst = (unsigned char)backgd;
							break;
						}
					}
				}

			}
		}

	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}




/*************************************************************************
 *
 * �������ƣ�
 *   DilationDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- ���ͷ�ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *	 int	color       - ǰ����ɫ��0Ϊ��ɫ��1Ϊ��ɫ��
 *
 * ����ֵ:
 *   BOOL               - ���ͳɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ������������㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 *
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/


BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color)
{

	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	//ǰ����ɫ����
	//color == 0 ǰ��Ϊ��ɫ
	//color == 1 ǰ��Ϊ��ɫ
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;


	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ�������
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɱ���ɫ
				*lpDst = (unsigned char)backgd;

				//Դͼ���е�ǰ�������������ֻҪ��һ������ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳��ǰ��ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == object)
					{
						*lpDst = (unsigned char)object;
						break;
					}
				}

			}
		}

	}
	else if (nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɱ���ɫ
				*lpDst = (unsigned char)backgd;

				//Դͼ���е�ǰ�������������ֻҪ��һ������ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳��ǰ��ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1) * lWidth);
					if (pixel == object)
					{
						*lpDst = (unsigned char)object;
						break;
					}
				}

			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ�������
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɱ���ɫ
				*lpDst = (unsigned char)backgd;

				//ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ����ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳��ǰ��ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (structure[m][n] == 0)
							continue;
						pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
						if (pixel == object)
						{
							*lpDst = (unsigned char)object;
							break;
						}
					}
				}

			}
		}

	}
	// �������ͺ��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   OpenDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- �����㷽ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *	 int	color       - ǰ����ɫ��0Ϊ��ɫ��1Ϊ��ɫ��
 *
 * ����ֵ:
 *   BOOL               - ������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����п����㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 *
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/

BOOL WINAPI OpenDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color)
{
	if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, structure, color))
	{

		if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode, structure, color))
		{
			// ����
			return TRUE;

		}
	}
	return FALSE;

}


/*************************************************************************
 *
 * �������ƣ�
 *   CloseDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- �����㷽ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *	 int	color       - ǰ����ɫ��0Ϊ��ɫ��1Ϊ��ɫ��
 *
 * ����ֵ:
 *   BOOL               - ������ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ����б����㡣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 *
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/


BOOL WINAPI CloseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3],int color)
{
	if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode, structure, color))
	{

		if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, structure,color))
		{
			// ����
			return TRUE;

		}
	}
	return FALSE;

}

/*************************************************************************
 *
 * �������ƣ�
 *   O_BorderDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- �����㷽ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *	 int	color       - ǰ����ɫ��0Ϊ��ɫ��1Ϊ��ɫ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ������߽硣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 *
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL WINAPI O_BorderDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color) {
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	//ǰ����ɫ����
	//color == 0 ǰ��Ϊ��ɫ
	//color == 1 ǰ��Ϊ��ɫ
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;


	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ�������
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && pixel != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɱ���ɫ
				*lpDst = (unsigned char)backgd;

				//Դͼ���е�ǰ�������������ֻҪ��һ������ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳��ǰ��ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == object)
					{
						*lpDst = (unsigned char)object;
						break;
					}
				}
				//��ñ߽�
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else if (nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɱ���ɫ
				*lpDst = (unsigned char)backgd;

				//Դͼ���е�ǰ�������������ֻҪ��һ������ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳��ǰ��ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1) * lWidth);
					if (pixel == object)
					{
						*lpDst = (unsigned char)object;
						break;
					}
				}
				//��ñ߽�
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ�������
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ��ɱ���ɫ
				*lpDst = (unsigned char)backgd;

				//ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ����ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳��ǰ��ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (structure[m][n] == 0)
							continue;
						pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
						if (pixel == object)
						{
							*lpDst = (unsigned char)object;
							break;
						}
					}
				}
				//��ñ߽�
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	// ���Ʊ߽�ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}



/*************************************************************************
 *
 * �������ƣ�
 *   I_BorderDIB()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������������4�ı�����
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *   int   nMode		- �����㷽ʽ��0��ʾˮƽ����1��ʾ��ֱ����2��ʾ�Զ���ṹԪ�ء�
 *	 int   structure[3][3]
						- �Զ����3��3�ṹԪ�ء�
 *	 int	color       - ǰ����ɫ��0Ϊ��ɫ��1Ϊ��ɫ��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 * �ú������ڶ�ͼ�����ڱ߽硣�ṹԪ��Ϊˮƽ�����ֱ����������㣬�м��λ��ԭ�㣻
 * �������û��Լ�����3��3�ĽṹԪ�ء�
 *
 * Ҫ��Ŀ��ͼ��Ϊֻ��0��255�����Ҷ�ֵ�ĻҶ�ͼ��
 ************************************************************************/
BOOL WINAPI I_BorderDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color) {
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	//ǰ����ɫ����
	//color == 0 ǰ��Ϊ��ɫ
	//color == 1 ǰ��Ϊ��ɫ
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ���ǰ��ɫ
				*lpDst = (unsigned char)object;

				//���Դͼ���е�ǰ���������������һ���㲻��ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɱ���ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == backgd)
					{
						*lpDst = (unsigned char)backgd;
						break;
					}
				}
				//��ñ߽�
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else if (nMode == 1)
	{
		//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ���ǰ��ɫ
				*lpDst = (unsigned char)object;

				//���Դͼ���е�ǰ���������������һ���㲻��ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɱ���ɫ
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1) * lWidth);
					if (pixel == backgd)
					{
						*lpDst = (unsigned char)backgd;
						break;
					}
				}
				//��ñ߽�
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else
	{
		//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
				//�����ϱߺ����±ߵ���������
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//Ŀ��ͼ���еĵ�ǰ���ȸ���ǰ��ɫ
				*lpDst = (unsigned char)object;

				//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ������ǰ��ɫ��
				//��Ŀ��ͼ���еĵ�ǰ�㸳�ɱ���ɫ
				//ע����DIBͼ�������������µ��õ�
				for (m = 0; m < 3; m++)
				{
					for (n = 0; n < 3; n++)
					{
						if (structure[m][n] == 0)
							continue;
						pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
						if (pixel == backgd)
						{
							*lpDst = (unsigned char)backgd;
							break;
						}
					}
				}
				//��ñ߽�
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	// ���Ʊ߽�ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}