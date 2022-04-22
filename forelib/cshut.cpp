//*****************************************************************************
//
// Module:  CShutdown.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Shutdown class
//
// Notes:
//
//*****************************************************************************

#include "CShut.h"

//*****************************************************************************
//
// CShutdown - backbone members
//
//*****************************************************************************

CShutdown::CShutdown()
{
   return;
}

CShutdown::CShutdown(const char *id, const char *unit,
					 const Date4& Start, const Date4& End) :
	m_sID(id),
	m_sUnit(unit),
	m_Start(Start),
	m_End(End)
{
   return;
}

CShutdown::~CShutdown()
{
   return;
}

ostream& operator<< (ostream& os, CShutdown& sd)
{
	os << sd.ID() << ' ';
	os << sd.Unit() << ' ';
	os << sd.Start().format("MM/DD/YY") << ' ';
	os << sd.End().format("MM/DD/YY") << ' ';
	return os;
}

//*****************************************************************************
//
// CShutdown - operators
//
//*****************************************************************************

int CShutdown::operator== (const CShutdown& rhs)
{
	return ( (m_sID == rhs.m_sID) &&
			 (m_sUnit == rhs.m_sUnit) &&
			 (m_Start == rhs.m_Start) &&
			 (m_End == rhs.m_End));
}

//*****************************************************************************

// end of CShutdown.CPP
