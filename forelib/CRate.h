//*****************************************************************************
//
// Module:  CRate.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CRate class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CRate_H
#define __CRate_H

//#include <iostream.h>
#include <iostream>
#include "..\classlib\str.h"
#include "advance.h"

// error types

enum {
   CRate_NO_ERROR
};

// class declaration

class CRate {

public:

    // backbone members
    CRate();
    CRate(const char *, const char *, const Double&, char, const Double&);
    virtual ~CRate();
    friend ostream& operator<< (ostream& os, CRate& wk);

	// accessors
	String ID()        { return m_sID;}
	String Type()      { return m_sType; }
	Advance& Rate()    { return m_Advance; }
	double MaxTpms()   { return ma_dGovernor[0]; }
	double MaxFtpms()  { return ma_dGovernor[1]; }
    long Error()       { return m_wError; }

	// implementors
	int match(const String& rhs) const {return (rhs.match(m_sID)); }

private:

	String  m_sID;		    // unit
	String  m_sType;        // section type
	Advance m_Advance;      // advance rate
	Double  ma_dGovernor;	// advance rate governors
    long    m_wError;       // error value
};

#endif

//*****************************************************************************

// end of CRate.h
