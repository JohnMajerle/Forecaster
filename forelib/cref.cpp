//*****************************************************************************
//
// Module:  CRefEx.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Reference Exception class
//
// Notes:
//
//*****************************************************************************

#include "CRef.h"

//*****************************************************************************
//
// CRefEx - backbone members
//
//*****************************************************************************

CRefEx::CRefEx() : 	m_dRefStation(0.0)
{
   return;
}

CRefEx::CRefEx(const char *id, const char *ref, double station, const char *unit,
			   const Double& rate, double incrs) :
	m_sRefID(ref),
	m_dRefStation(station),
	CExcept(id,unit,rate,incrs)
{
   return;
}

CRefEx::~CRefEx()
{
   return;
}

ostream& operator<< (ostream& os, CRefEx& ex)
{
	os << ex.RefID() << ' ';
	os << ex.RefStation() << ' ';
	os << CExcept(ex);
	return os;
}

//*****************************************************************************

// end of CRefEx.CPP
