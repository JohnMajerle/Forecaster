//*****************************************************************************
//
// Module:  CDateEx.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Date Exception class
//
// Notes:   
//
//*****************************************************************************

#ifndef __CDateEx_H
#define __CDateEx_H

#include <iostream>
#include "..\classlib\str.h"
#include "..\classlib\date.h"
#include "basetype.h"
#include "advance.h"
#include "cexcept.h"

// class declaration

class CDateEx : public CExcept {

public:

	// backbone members
	CDateEx();
	CDateEx(const char *, const Date4&, const Date4&,
		    const char *, const Double&, double);
	virtual ~CDateEx();
	friend ostream& operator<< (ostream& os, CDateEx& dt);

	// accessors
	Date4& Start() { return m_Start; }
	Date4& End() { return m_End; }

	// operators
	int operator== (const CDateEx& rhs);

private:

	Date4 m_Start;		// date exception to start
	Date4 m_End;		// date exception to end (can be blank)
};

#endif

//*****************************************************************************

// end of CDateEx.h
