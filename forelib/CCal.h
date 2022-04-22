//*****************************************************************************
//
// Module:  CCal.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Calendar class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CCal_H
#define __CCal_H

//#include <iostream.h>
#include <iostream>
#include "..\classlib\str.h"
#include "..\classlib\date.h"
#include "basetype.h"

using std::ostream;

// class declaration

class CCal {

public:

	// backbone members
	CCal();
	CCal(const char *, const Date4&, const Date4&, int, UChar Days);
	virtual ~CCal();
	friend ostream& operator<< (ostream& os, CCal& cl);

	// accessors
	String ID() { return m_sID; }
	Date4& Begin() { return m_Begin; }
	Date4& End() { return m_End; }
	int    Alternate() { return m_nCal; }
	UChar  Days() const { return m_bytDays; }
	int    DayActive(int day) { return _getBitMask(m_bytDays,day); }

	// modifiers
	void AttachAlternate(Array<CCal>* Alternate) { m_Alternate = Alternate; }
	void Alternate(Date4& Monday);

	// operators
	int operator== (const CCal& rhs);

private:

	String       m_sID;			// section or unit where calendar to be used
	Date4        m_Begin;		// monday calendar to start
	Date4        m_End;			// monday calendar to end (can be blank)
	int          m_nCal;		// alternate calendar id (can be zero)
	Array<CCal>* m_Alternate;	// rereference to alternate calendar
	UChar        m_bytDays;		// working days bit-mask
};

#endif

//*****************************************************************************

// end of CCal.h
