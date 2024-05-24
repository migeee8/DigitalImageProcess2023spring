// TemplateTrans.h
#include "WinDef.h"

#ifndef _INC_TemplateTransAPI
#define _INC_TemplateTransAPI

BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,
	int iTempH, int iTempW,
	int iTempMX, int iTempMY,
	FLOAT* fpArray, FLOAT fCoef);
BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int iMaskSize);
#endif //!_INC_TemplateTransAPI
