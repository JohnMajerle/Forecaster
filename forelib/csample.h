//*****************************************************************************
//
// Module:  CSample.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CSample class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CSample_H
#define __CSample_H

#include <iostream>
using std::iostream;
#include <ostream>
using std::ostream;

#include "..\classlib\str.h"
#include "basetype.h"

// constants and internal data types for class

const int NUMQUAL = 23;

// class declaration

class CSample {

public:

	// backbone members
	CSample();
	CSample(const char *, const char *, long, long, const double *);
	virtual ~CSample();
	friend ostream& operator<< (ostream& os, CSample& smpl);

	// accessors
	char*   ID()      { return m_sID;      }
	char*   Type()    { return m_sType;    }
	double  Quality(int index) { return ma_dQuality[index]; }

	// methods

	// implementors

private:

	char   m_sID[9];				// name
	char   m_sType[3];				// type - 'LW' or 'CM'
	long   m_lXcoord;				// x-coordinate
	long   m_lYcoord;				// y-coordinate
	double ma_dQuality[NUMQUAL];	// quality data
};

#endif

//*****************************************************************************

// end of CSample.h
