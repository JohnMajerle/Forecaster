//*****************************************************************************
//
// Module:  CCal.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Calendar class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "CCal.h"

#include <ios>
using std::ios;

//*****************************************************************************
//
// CCal - backbone members
//
//*****************************************************************************

CCal::CCal() : m_nCal(0),m_Alternate(0),m_bytDays(0)
{
   return;
}

CCal::CCal(const char *id, const Date4& Begin, const Date4& End,
		   int Cal, UChar bytDays) :
	m_sID(id),
	m_Begin(Begin),
	m_End(End),
	m_nCal(Cal),
	m_Alternate(0),
	m_bytDays(bytDays)
{
   return;
}

CCal::~CCal()
{
   if (m_Alternate) delete m_Alternate;
   return;
}

ostream& operator<< (ostream& os, CCal& cl)
{
	os << cl.ID() << ' ';
	os << cl.Begin().format("MM/DD/YY") << ' ';
	os << cl.End().format("MM/DD/YY") << ' ';
	os << cl.Alternate() << ' ';
	os.setf(ios::hex);
	os << int(cl.Days());
	os.unsetf(ios::hex);
	return os;
}

//*****************************************************************************
//
// CCal - modifiers
//
//*****************************************************************************

void CCal::Alternate(Date4& Monday)
{
	if (m_Alternate) {
		Pointer<CCal> Start(*m_Alternate,0);
		Pointer<CCal> Beyond(*m_Alternate,(*m_Alternate).size());
		while (Start != Beyond && (*Start).Begin() != Monday)
			++Start;
		if (Start == Beyond)
			m_bytDays = 0;
		else
			m_bytDays = (*Start).Days();
	}
}

//*****************************************************************************
//
// CCal - operators
//
//*****************************************************************************

int CCal::operator== (const CCal& rhs)
{
	return ( (m_sID == rhs.m_sID) &&
			 (m_Begin == rhs.m_Begin) &&
			 (m_End == rhs.m_End) &&
			 (m_nCal == rhs.m_nCal) &&
			 (m_Alternate == rhs.m_Alternate) &&
			 (m_bytDays == rhs.m_bytDays));
}

//*****************************************************************************

// end of CCal.CPP
