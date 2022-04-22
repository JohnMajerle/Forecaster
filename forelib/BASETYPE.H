//*****************************************************************************
//
// Module:  Basetype.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: header file for VCFore base type definitions
//
// Notes:
//
//*****************************************************************************

#ifndef   __BASETYPE_H
#define   __BASETYPE_H

#include "..\classlib\array.h"
//                                                    
// standard typedef's
//
typedef unsigned long  ULong;
typedef unsigned short UShort;
typedef unsigned int   UInt;
typedef unsigned char  UChar;
//                                                    
// typedef's for arrays
//
typedef Array<double> Double;
//                                                    
// global constants
//
const double eps = 1e-5;
const int sqft2acres = 43560;
//
// error handling 
//
#define ExcCheck(ExcCondition,Label)  if(ExcCondition) goto Label
//                                                    
// inline functions
//
inline void _setBitMask(UChar *mask, int bitNum) { *mask=*mask|((UChar)1<<bitNum); }
inline int  _getBitMask(UChar  mask, int bitNum) { return((mask>>bitNum)&(UChar)1); }
inline void _set32BitMask(ULong *mask, int bitNum) { *mask=*mask|((ULong)1<<bitNum); }
inline int  _32BitCount(ULong mask)
{
	  int count=0;
      for (int i=0; i<32; ++i)
         count += (int)((mask>>i)&(ULong)1);
      return count;
}

//                                                    
// generic functions
//
// [NEEDSWORK] Phocaz: Redundant with C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Tools\MSVC\14.16.27023\include\xutility

//template<class T, class X>
//T find(T start, T beyond, const X& x)
//{
//	while (start != beyond && *start != x)
//		++start;
//	return start;
//}
//
//template<class T, class X>
//T match(T start, T beyond, const X& x)
//{
//	while (start != beyond && (*start).match(x))
//		++start;
//	return start;
//}

#endif
