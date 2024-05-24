// PointTrans.h
#include "Point.h"

#ifndef _INC_PointTransAPI
#define _INC_PointTransAPI

// º¯ÊýÔ­ÐÍ
BOOL WINAPI AddBrightness(LPSTR lpDIBBits, int BitCount, LONG lWidth, LONG lHeight, int offset);
BOOL WINAPI Trans24cTo8c(LPSTR lpSrcDib, LPSTR lpDIBBits,LONG lWidth, LONG lHeight);
BOOL WINAPI Trans8cTo8g(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lSrcWidth, LONG lSrcHeight);
BOOL WINAPI Trans8gToBin(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lSrcWidth, LONG lSrcHeight, int threshold);
BOOL WINAPI HalfTone(LPSTR lpSrcDib, LPSTR lpDIBBits, LONG lSrcWidth, LONG lSrcHeight);
BOOL WINAPI LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT fA, FLOAT fB);
BOOL WINAPI Pw2LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, Imgpoint A, Imgpoint B, Imgpoint C);
BOOL WINAPI logTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI GammaTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, float gamma);

#endif //!_INC_PointTransAPI