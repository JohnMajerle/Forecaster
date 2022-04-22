//*****************************************************************************
//
// Module:  CQual.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CQual class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CQual_H
#define __CQual_H

#include "CSample.h"
#include "qual.h"

#define NUMADJ 14

// class declaration

class CQual
{
public:

    // constructor and destructor
    CQual(long);
    virtual ~CQual();

    // accessors
	void  SetLWNumber(int n)  { m_wLWNumber = n; }
	void  SetCMNumber(int n)  { m_wCMNumber = n; }
	long* XCoord()    { return m_pXCoord; }
	long* YCoord()    { return m_pYCoord; }
	int*  Quality()   { return m_pQuality; }
	CSample* Sample() { return m_pSample; }
	double*  LWAdjustments() { return m_dLWAdj; }
	double*  CMAdjustments() { return m_dCMAdj; }
    CSample& Sample(long index) { return m_pSample[index]; }
	double PercentWashed() { return m_dCMAdj[11]; }
	double RawMoisture() { return m_dCMAdj[5]; }
	double CleanMoisture() { return m_dCMAdj[9]; }

    // implementors
	void StartEstimator()
		{ init_blk_fcst(m_wLWNumber,m_wCMNumber,
						m_pXCoord,m_pYCoord,
						m_pQuality,m_pSample,
						m_dLWAdj,m_dCMAdj);		}
	void BlockEstimate(double *qual, double *x, double *y,
					   const char *type, double recovery)
		{ blk_fcst(qual,x,y,type,recovery); }

private:

	int		 m_wLWNumber;
	int		 m_wCMNumber;
	long*	 m_pYCoord;	  
	long*	 m_pXCoord;	  
	int*	 m_pQuality;
	CSample* m_pSample;
	double   m_dLWAdj[NUMADJ];
	double   m_dCMAdj[NUMADJ];

};

#endif

//*****************************************************************************

// end of CQual.h
