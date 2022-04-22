//*****************************************************************************
//
// Module:  CAreaEx.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Area Exception class
//
// Notes:
//
//*****************************************************************************

#include "CArea.h"

//*****************************************************************************
//
// CAreaEx - backbone members
//
//*****************************************************************************

CAreaEx::CAreaEx() : m_dStart(0.0),m_dEnd(0.0)
{
   return;
}

CAreaEx::CAreaEx(const char *id, double start, double end,
				 const Double& rate, double incrs) :
	m_dStart(start),
	m_dEnd(end),
	CExcept(id," ",rate,incrs)
{
   return;
}

CAreaEx::~CAreaEx()
{
   return;
}

ostream& operator<< (ostream& os, CAreaEx& ex)
{
	os << ex.Start() << ' ';
	os << ex.End() << ' ';
	os << CExcept(ex);
	return os;
}

//*****************************************************************************

// end of CCAreaEx.CPP
