//*****************************************************************************
//
// Module:  Basedef.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: header file for VCFore base definitions
//
// Notes:
//
//*****************************************************************************

#ifndef   __BASEDEF_H
#define   __BASEDEF_H

#include "..\classlib\array.h"
//#include "..\classlib\list.h"  
#define List std::list				// Changed from List to std::list
#include "basetype.h"
#include "CCal.h"
#include "CGeometry.h"
#include "CRate.h"
#include "CSection.h"
#include "CDate.h"
#include "CArea.h"
#include "CRef.h"
#include "CShut.h"
#include "CResults.h"

//
// array sizes 
//
//const int NUMQUAL = 23;		// quality parameters
//const int NUMADJ  = 14;		// quality adjustment factors
//                                                    
// typedef's for aliasing class names
//
typedef Array<CCal> Calendar;
typedef Pointer<CCal> CalendarPtr;
typedef List<CCal> CalendarList;
typedef Array<CGeometry> Geometry;
typedef Array<CRate> Rates;
typedef Array<CSection> Sections;
typedef Pointer<CSection> SectionPtr;
typedef List<CSection> SectionList;
typedef Array<CDateEx> Date;
typedef Pointer<CDateEx> DatePtr;
typedef List<CDateEx> DateList;
typedef Array<CAreaEx> Area;
typedef Pointer<CAreaEx> AreaPtr;
typedef Array<CRefEx> Ref;
typedef Pointer<CRefEx> RefPtr;
typedef Array<CShutdown> Shutdowns;
typedef Pointer<CShutdown> ShutdownPtr;
typedef List<CShutdown> ShutdownList;
typedef List<CResults> ResultsList;

#endif
