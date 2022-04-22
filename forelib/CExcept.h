//*****************************************************************************
//
// Module:  CExcept.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Exception class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CExcept_H
#define __CExcept_H

#include <iostream>
#include <math.h>
#include "..\classlib\str.h"
#include "..\classlib\date.h"
#include "basetype.h"
#include "advance.h"

// class declaration

class CExcept {

public:

	// backbone members
	CExcept();
	CExcept(const char *, const char *, const Double&, double);
	virtual ~CExcept();
	friend ostream& operator<< (ostream& os, CExcept& ex);

	// accessors
	int HasUnit() { return (m_sUnit.length()?1:0); }
	int HasAdvance() { Advance zero; zero.Reset(); return (m_Advance!=zero); }
	int HasIncrease() { return (fabs(m_dIncrease)>eps); }
	String ID() { return m_sID; }
	String Unit() { return m_sUnit; }
	Advance& Rate() { return m_Advance; }
	double Increase() { return m_dIncrease; }

	// implementors
	void Apply(Advance& Advance);

private:

	String       m_sID;			// section or unit where exception to be used
	String       m_sUnit;		// additional or replacement unit
	Advance      m_Advance;		// advance rate exception
	double       m_dIncrease;	// percentage productivity increase
};

#endif

//*****************************************************************************

// end of CExcept.h
