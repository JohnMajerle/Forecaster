//*****************************************************************************
//
// Module:  CCArea.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Area Exception class
//
// Notes:   
//
//*****************************************************************************

#ifndef __CAreaEx_H
#define __CAreaEx_H

#include <iostream>
#include "cexcept.h"

// class declaration

class CAreaEx : public CExcept {

public:

	// backbone members
	CAreaEx();
	CAreaEx(const char *, double, double, const Double&, double);
	virtual ~CAreaEx();
	friend ostream& operator<< (ostream& os, CAreaEx& dt);

	// accessors
	double Start() { return m_dStart; }
	double End()   { return m_dEnd; }

private:

	double m_dStart;		// station where exception to start
	double m_dEnd;			// station where exception to end
};

#endif

//*****************************************************************************

// end of CAreaEx.h
