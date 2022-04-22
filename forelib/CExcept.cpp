//*****************************************************************************
//
// Module:  CExcept.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Exception class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include <math.h>
#include "CExcept.h"

//*****************************************************************************
//
// CExcept - backbone members
//
//*****************************************************************************

CExcept::CExcept() : m_dIncrease(0.0)
{
   return;
}

CExcept::CExcept(const char *id, const char *unit, const Double& rate, double incrs) :
	m_sID(id),
	m_sUnit(unit),
	m_Advance(' ',rate),
	m_dIncrease(incrs)
{
   return;
}

CExcept::~CExcept()
{
   return;
}

ostream& operator<< (ostream& os, CExcept& ex)
{
	os << ex.ID() << ' ';
	os << ex.Unit() << ' ';
	os << ex.Rate() << ' ';
	os << ex.Increase();
	return os;
}

//*****************************************************************************
//
// CExcept - implementors
//
//*****************************************************************************

void CExcept::Apply(Advance& advance)
{
	if (this->HasUnit() && !this->HasAdvance() && !this->HasIncrease())
		// unit exception cannot set rate to zero				
		return;

	if (!this->HasAdvance() && !this->HasIncrease()) {
		// set rate to zero
		advance.Reset();
		return;
	}

	if (this->HasAdvance())					// update rate
		advance.Rate(m_Advance.Rate());
	if (this->HasIncrease())				// increase rate
		advance *= m_dIncrease;
}

//*****************************************************************************

// end of CExcept.CPP
