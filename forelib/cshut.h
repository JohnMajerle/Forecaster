//*****************************************************************************
//
// Module:  CShutdown.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Shutdown class
//
// Notes:   
//
//*****************************************************************************

#ifndef __CShutdown_H
#define __CShutdown_H

#include <iostream>
using std::iostream;
#include <ostream>
using std::ostream;


#include "..\classlib\str.h"
#include "..\classlib\date.h"

// class declaration

class CShutdown {

public:

	// backbone members
	CShutdown();
	CShutdown(const char *, const char *,
			  const Date4&, const Date4&);
	virtual ~CShutdown();
	friend ostream& operator<< (ostream& os, CShutdown& sd);

	// accessors
	String ID()    { return m_sID; }
	String Unit()  { return m_sUnit; }
	Date4& Start() { return m_Start; }
	Date4& End()   { return m_End; }

	// operators
	int operator== (const CShutdown& rhs);

private:

	String m_sID;		// section or unit where shutdown to occur
	String m_sUnit;		// unit who's moving causes shutdowns
	Date4  m_Start;		// date shutdowns can start
	Date4  m_End;		// date shutdowns can no longer occur
};

#endif

//*****************************************************************************

// end of CShutdown.h
