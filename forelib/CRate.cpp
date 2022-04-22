//*****************************************************************************
//
// Module:  CRate.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CRate class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "CRate.h"

//*****************************************************************************
//
// CRate - backbone members
//
//*****************************************************************************

CRate::CRate() : ma_dGovernor(2), m_wError(CRate_NO_ERROR)
{
    return;
}

CRate::CRate(const char *id, const char *type, const Double& rate, char coal,
			 const Double& governor) :
			m_sID(id),
			m_sType(type),
			m_Advance(coal,rate),
			ma_dGovernor(governor),
			m_wError(CRate_NO_ERROR)
{
	return;
}

CRate::~CRate()
{
    return;
}

ostream& operator<< (ostream& os, CRate& rt)
{
	os << rt.ID() << ' ';
	os << rt.Type() << ' ';
	os << rt.Rate() << ' ';
	os << rt.MaxTpms() << ' ';
	os << rt.MaxFtpms();
	return os;
}

//*****************************************************************************

// end of CRate.CPP
