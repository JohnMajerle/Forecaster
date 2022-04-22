//*****************************************************************************
//
// Module:  Advance.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the Advance class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __Advance_H
#define __Advance_H

//#include <iostream.h>
#include <iostream> 
#include <ostream>
using std::iostream;
using std::ostream;
#include "basetype.h"

// constants and internal data types for class

const int NUMADV = 4;

// class declaration

class Advance {

public:

    // backbone members
    Advance();
	Advance(char, const Double&);
    virtual ~Advance();
	friend ostream& operator<< (ostream& os, Advance& wk);

    // accessors
	char    Coal()     { return m_bytCoal; }
	Double& Rate()     { return ma_dRate; }		
	double  FtPerMs()  { return ma_dRate[0]; }
	double  TnsPerMs() { return ma_dRate[1]; }
	double  TnsPerFt() { return ma_dRate[2]; }
	double  MsPerDay() { return ma_dRate[3]; }

    // modifiers
	void Reset() { for (int i=0; i<NUMADV; ++i) ma_dRate[i] = 0.0; }
	Advance &Rate(const Double& rhs);

    // implementors
	int operator!= (const Advance& rhs);
	Advance &operator*= (const double percent);

private:

    char   m_bytCoal;	// 'C' - clean tons, 'R' - raw tons
    Double ma_dRate;    // ft/ms, tons/ms, tons/ft, ms/day      

};

#endif

//*****************************************************************************

// end of Advance.h
