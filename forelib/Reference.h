//*****************************************************************************
//
// Module:  Reference.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Reference class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __Reference_H
#define __Reference_H

#include <iostream>
using std::iostream;
#include <ostream>
using std::ostream;

#include "..\classlib\str.h"
#include "basetype.h"

class CSection;

// class declaration

class Reference {

public:

	// backbone members
	Reference();
	Reference(const char *, double, UInt);
	virtual ~Reference();
	friend ostream& operator<< (ostream& os, Reference& rf);

	// accessors
	String ID()          { return m_sID; }
	double Station()     { return m_dStation; }
	CSection* pSection() { return m_pSection; }
	UInt Delay()         { return m_wDelay; }

	// modifiers
	void Attach(CSection* p) { m_pSection = p; }
	void Delay(int n) { m_wDelay += n; }

	// implementors
	int operator== (const String& rhs) const { return (m_sID == rhs); }

private:

	String    m_sID;
	double    m_dStation;
	CSection* m_pSection;
	UInt      m_wDelay;

};

#endif

//*****************************************************************************

// end of Reference.h
