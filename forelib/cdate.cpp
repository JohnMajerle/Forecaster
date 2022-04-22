//*****************************************************************************
//
// Module:  CDateEx.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Date Exception class
//
// Notes:
//
//*****************************************************************************

#include "CDate.h"

//*****************************************************************************
//
// CDateEx - backbone members
//
//*****************************************************************************

CDateEx::CDateEx()
{
   return;
}

CDateEx::CDateEx(const char *id, const Date4& Start, const Date4& End, const char *unit,
				 const Double& rate, double incrs) :
	m_Start(Start),
	m_End(End),
	CExcept(id,unit,rate,incrs)
{
   return;
}

CDateEx::~CDateEx()
{
   return;
}

ostream& operator<< (ostream& os, CDateEx& ex)
{
	os << ex.Start().format("MM/DD/YY") << ' ';
	os << ex.End().format("MM/DD/YY") << ' ';
	os << CExcept(ex);
	return os;
}

//*****************************************************************************
//
// CDateEx - operators
//
//*****************************************************************************

int CDateEx::operator== (const CDateEx& rhs)
{
	return ( (m_Start == rhs.m_Start) &&
			 (m_End == rhs.m_End));
}

//*****************************************************************************

// end of CDateEx.CPP
