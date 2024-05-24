// ************************************************************************
//  文件名：morph.cpp
//
//  图像形态学变换API函数库：
//
//  ErosionDIB()    - 图像腐蚀
//  DilationDIB()	- 图像膨胀
//  OpenDIB()		- 图像开运算
//  CloseDIB()		- 图像闭运算
//  O_BorderDIB()	- 图像求外边界
//	I_BorderDIB()   - 图像求内边界
// ************************************************************************


#include "stdafx.h"
#include "morpho.h"
#include "DIBAPI.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
 *
 * 函数名称：
 *   ErosiontionDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 腐蚀方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *	 int	color       - 前景颜色（0为黑色，1为白色）
 *
 * 返回值:
 *   BOOL               - 腐蚀成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行腐蚀运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 *
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//像素值
	unsigned char pixel;
	
	//前景颜色设置
	//color == 0 前景为黑色
	//color == 1 前景为白色
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//使用水平方向的结构元素进行腐蚀
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成前景色
				*lpDst = (unsigned char)object;

				//如果源图像中当前点自身或者左右有一个点不是前景色，
				//则将目标图像中的当前点赋成背景色
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
		//使用垂直方向的结构元素进行腐蚀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成前景色
				*lpDst = (unsigned char)object;

				//如果源图像中当前点自身或者上下有一个点不是前景色，
				//则将目标图像中的当前点赋成背景色
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
		//使用自定义的结构元素进行腐蚀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成前景色
				*lpDst = (unsigned char)object;

				//如果原图像中对应结构元素中为黑色的那些点中有一个不是前景色，
				//则将目标图像中的当前点赋成背景色
				//注意在DIB图像中内容是上下倒置的
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
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}




/*************************************************************************
 *
 * 函数名称：
 *   DilationDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 膨胀方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *	 int	color       - 前景颜色（0为黑色，1为白色）
 *
 * 返回值:
 *   BOOL               - 膨胀成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行膨胀运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 *
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/


BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//像素值
	unsigned char pixel;

	//前景颜色设置
	//color == 0 前景为黑色
	//color == 1 前景为白色
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;


	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//使用水平方向的结构元素进行膨胀
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && pixel != 0)
					return FALSE;

				//目标图像中的当前点先赋成背景色
				*lpDst = (unsigned char)backgd;

				//源图像中当前点自身或者左右只要有一个点是前景色，
				//则将目标图像中的当前点赋成前景色
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
		//使用垂直方向的结构元素进行膨胀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成背景色
				*lpDst = (unsigned char)backgd;

				//源图像中当前点自身或者上下只要有一个点是前景色，
				//则将目标图像中的当前点赋成前景色
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
		//使用自定义的结构元素进行膨胀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成背景色
				*lpDst = (unsigned char)backgd;

				//原图像中对应结构元素中为黑色的那些点中只要有一个是前景色，
				//则将目标图像中的当前点赋成前景色
				//注意在DIB图像中内容是上下倒置的
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
	// 复制膨胀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   OpenDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 开运算方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *	 int	color       - 前景颜色（0为黑色，1为白色）
 *
 * 返回值:
 *   BOOL               - 开运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行开运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 *
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/

BOOL WINAPI OpenDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color)
{
	if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, structure, color))
	{

		if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode, structure, color))
		{
			// 返回
			return TRUE;

		}
	}
	return FALSE;

}


/*************************************************************************
 *
 * 函数名称：
 *   CloseDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 闭运算方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *	 int	color       - 前景颜色（0为黑色，1为白色）
 *
 * 返回值:
 *   BOOL               - 闭运算成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像进行闭运算。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 *
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/


BOOL WINAPI CloseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3],int color)
{
	if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode, structure, color))
	{

		if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, structure,color))
		{
			// 返回
			return TRUE;

		}
	}
	return FALSE;

}

/*************************************************************************
 *
 * 函数名称：
 *   O_BorderDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 闭运算方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *	 int	color       - 前景颜色（0为黑色，1为白色）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像求外边界。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 *
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL WINAPI O_BorderDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color) {
	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//像素值
	unsigned char pixel;

	//前景颜色设置
	//color == 0 前景为黑色
	//color == 1 前景为白色
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;


	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//使用水平方向的结构元素进行膨胀
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && pixel != 0)
					return FALSE;

				//目标图像中的当前点先赋成背景色
				*lpDst = (unsigned char)backgd;

				//源图像中当前点自身或者左右只要有一个点是前景色，
				//则将目标图像中的当前点赋成前景色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == object)
					{
						*lpDst = (unsigned char)object;
						break;
					}
				}
				//获得边界
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else if (nMode == 1)
	{
		//使用垂直方向的结构元素进行膨胀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成背景色
				*lpDst = (unsigned char)backgd;

				//源图像中当前点自身或者上下只要有一个点是前景色，
				//则将目标图像中的当前点赋成前景色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1) * lWidth);
					if (pixel == object)
					{
						*lpDst = (unsigned char)object;
						break;
					}
				}
				//获得边界
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else
	{
		//使用自定义的结构元素进行膨胀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成背景色
				*lpDst = (unsigned char)backgd;

				//原图像中对应结构元素中为黑色的那些点中只要有一个是前景色，
				//则将目标图像中的当前点赋成前景色
				//注意在DIB图像中内容是上下倒置的
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
				//获得边界
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	// 复制边界图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}



/*************************************************************************
 *
 * 函数名称：
 *   I_BorderDIB()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数，必须是4的倍数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *   int   nMode		- 闭运算方式，0表示水平方向，1表示垂直方向，2表示自定义结构元素。
 *	 int   structure[3][3]
						- 自定义的3×3结构元素。
 *	 int	color       - 前景颜色（0为黑色，1为白色）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 * 该函数用于对图像求内边界。结构元素为水平方向或垂直方向的三个点，中间点位于原点；
 * 或者由用户自己定义3×3的结构元素。
 *
 * 要求目标图像为只有0和255两个灰度值的灰度图像。
 ************************************************************************/
BOOL WINAPI I_BorderDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color) {
	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//像素值
	unsigned char pixel;

	//前景颜色设置
	//color == 0 前景为黑色
	//color == 1 前景为白色
	unsigned char object = color == 0 ? (unsigned char)0 : (unsigned char)255;
	unsigned char backgd = color == 0 ? (unsigned char)255 : (unsigned char)0;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	if (nMode == 0)
	{
		//使用水平方向的结构元素进行腐蚀
		for (j = 0; j < lHeight; j++)
		{
			for (i = 1; i < lWidth - 1; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成前景色
				*lpDst = (unsigned char)object;

				//如果源图像中当前点自身或者左右有一个点不是前景色，
				//则将目标图像中的当前点赋成背景色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + n - 1);
					if (pixel == backgd)
					{
						*lpDst = (unsigned char)backgd;
						break;
					}
				}
				//获得边界
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else if (nMode == 1)
	{
		//使用垂直方向的结构元素进行腐蚀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成前景色
				*lpDst = (unsigned char)object;

				//如果源图像中当前点自身或者上下有一个点不是前景色，
				//则将目标图像中的当前点赋成背景色
				for (n = 0; n < 3; n++)
				{
					pixel = *(lpSrc + (n - 1) * lWidth);
					if (pixel == backgd)
					{
						*lpDst = (unsigned char)backgd;
						break;
					}
				}
				//获得边界
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	else
	{
		//使用自定义的结构元素进行腐蚀
		for (j = 1; j < lHeight - 1; j++)
		{
			for (i = 0; i < lWidth; i++)
			{
				//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
				//和最上边和最下边的两列像素
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					return FALSE;

				//目标图像中的当前点先赋成前景色
				*lpDst = (unsigned char)object;

				//如果原图像中对应结构元素中为黑色的那些点中有一个不是前景色，
				//则将目标图像中的当前点赋成背景色
				//注意在DIB图像中内容是上下倒置的
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
				//获得边界
				if (*lpSrc == *lpDst) {
					*lpDst = (unsigned char)backgd;
				}
				else *lpDst = (unsigned char)object;
			}
		}

	}
	// 复制边界图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}