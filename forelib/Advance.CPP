//*****************************************************************************
//
// Module:  Advance.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the Advance class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include <math.h>
#include "Advance.h"

//*****************************************************************************
//
// Advance - backbone members
//
//*****************************************************************************

Advance::Advance() : m_bytCoal(' '),ma_dRate(NUMADV)
{
	return;
}

Advance::Advance(char coal, const Double& rate) :
	m_bytCoal(coal),
	ma_dRate(rate)
{
	return;
}

Advance::~Advance()
{
	return;
}

ostream& operator<< (ostream& os, Advance& rt)
{
	os << rt.Coal() << ' ';
	os << rt.FtPerMs() << ' ';
	os << rt.TnsPerMs() << ' ';
	os << rt.TnsPerFt() << ' ';
	os << rt.MsPerDay();
	return os;
}

//*****************************************************************************
//
// Advance - modifiers
//
//*****************************************************************************

Advance &Advance::Rate(const Double& rhs)
{
	for (int i=0; i<NUMADV; ++i)			// replace with parameters that
		if (fabs(rhs[i]) > eps)				// are non-zero
			ma_dRate[i] = rhs[i];	

	if (fabs(rhs[1]) > eps  &&  fabs(rhs[0]) <= eps)	// make sure tons/ms is used
		ma_dRate[0] = 0.0;

	return *this;
}

//*****************************************************************************
//
// Advance - implementors
//
//*****************************************************************************

int Advance::operator!= (const Advance& rhs)
{
	for (int i=0; i<NUMADV; ++i)
		if (fabs(ma_dRate[i] - rhs.ma_dRate[i]) > eps)
			return 1;
	return 0;
}

Advance &Advance::operator*= (const double percent)
{
	double  increase = 1.0 + (percent/100.);

	if (ma_dRate[0] > eps)            // feet/ms
		ma_dRate[0] *= increase;
	else                              // tons/ms
		ma_dRate[1] *= increase;

	return *this;
}

//*****************************************************************************

// end of Advance.CPP
