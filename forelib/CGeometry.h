//*****************************************************************************
//
// Module:  CGeometry.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CGeometry class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CGeometry_H
#define __CGeometry_H

//#include <iostream.h>
#include <iostream>
using std::iostream;
#include <ostream>
using std::ostream;

#include <math.h>
#include "..\classlib\str.h"
#include "basetype.h"
#include "CQual.h"



// constants and internal data types for class

const int NUMGEO = 8;
const double PI = 3.1415926535;

// class declaration

class CGeometry {

public:

	// backbone members
	CGeometry();
	CGeometry(const char *, const char *, const Double&);
	virtual ~CGeometry();
	friend ostream& operator<< (ostream& os, CGeometry& wk);
	
	// accessors
	String ID()      { return m_sID;      }
	String Type()    { return m_sType;    }
	double Station() { return ma_dGeometry[0]; }
	double XCoord()  { return ma_dGeometry[1]; }
	double YCoord()  { return ma_dGeometry[2]; }
	double Azimuth() { return ma_dGeometry[3]; }
	double Length()  { return ma_dGeometry[4]; }
	double Width()   { return ma_dGeometry[5]; }
	double LinearFactor()   { return ma_dGeometry[6]; }
	double Recovery(){ return ma_dGeometry[7]; }
	double CleanTonsPerFt(double);
	double RawTonsPerFt(double);
	double Area(double start, double end)
		{ return fabs(start-end) * Width() * Recovery() / 100.; }

	// methods
	static void AttachQuality(CQual *pQual) { m_pQuality = pQual; }
	void Quality(double, double, double*);
	void Block(double, double, double *, double *);

	// implementors
	int operator!= (const String& rhs) const { return (m_sID != rhs); }

private:

	String m_sID;			// name
	String m_sType;			// type - begins with 'LW' or 'CM'
	Double ma_dGeometry;	// geometry data

	double m_dEstimateCleanTPF;
	double m_dEstimateRawTPF;
	double m_dEstimateHeading;

	static CQual *m_pQuality;
};

#endif

//*****************************************************************************

// end of CGeometry.h
