//*****************************************************************************
//
// Module:  CRef.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Reference Exception class
//
// Notes:   
//
//*****************************************************************************

#ifndef __CRefEx_H
#define __CRefEx_H

#include <iostream>
#include "..\classlib\str.h"
#include "cexcept.h"

// class declaration

class CRefEx : public CExcept {

public:

	// backbone members
	CRefEx();
	CRefEx(const char *, const char *, double, const char *, const Double&, double);
	virtual ~CRefEx();
	friend ostream& operator<< (ostream& os, CRefEx& dt);

	// accessors
	String RefID()      { return m_sRefID; }
	double RefStation() { return m_dRefStation; }

private:

	String m_sRefID;		// section id of reference
	double m_dRefStation;	// station to be reached before exception to start
};

#endif

//*****************************************************************************

// end of CRefEx.h
