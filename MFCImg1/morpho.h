// morph.h


#ifndef _INC_MorphAPI
#define _INC_MorphAPI


// º¯ÊýÔ­ÐÍ
BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color);
BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color);
BOOL WINAPI OpenDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color);
BOOL WINAPI CloseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color);
BOOL WINAPI O_BorderDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color);
BOOL WINAPI I_BorderDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], int color);

#endif //!_INC_MorphAPI#pragma once
