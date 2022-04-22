//*****************************************************************************
//
// Module:  CResults.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CResults class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CResults_H
#define __CResults_H

#include <math.h>
#include "..\classlib\select.h"
#include "..\classlib\date.h"
#include "..\classlib\str.h"
#include "..\classlib\fm.h"
#include "..\classlib\bt.h"
#include "basetype.h"
#include "CGeometry.h"

#include <iostream>
using std::iostream;

#include <ostream>
using std::ostream;

using std::ofstream;
using std::ifstream;

// global constants

const int maxUint = 999;

// error types

enum {
	CResults_NO_ERROR,
	CResults_FILE_ERROR,
	CResults_EOF,
	CResults_RECORD_ERROR
};

// class declaration

class CResults
{
public:

	// backbone members
	CResults();
	CResults(const char*, const char*, double, double, const char *, const char *,
			 Date4&, ULong, UInt, UInt, UChar, double, double, double, double);
	virtual ~CResults();

	// accessors
	CGeometry* Geometry() { return m_pPanel; }
	String ID()       { return m_sID; }
	String Type()     { return m_sType; }
	int    CMType()   { return !m_sType.match(String("CM*")); }
	int    LWType()   { return !m_sType.match(String("LW*")); }
	int    Super()    { return (m_sSuper==String("XXXXX"))? 0 : 1; }
	String Unit()     { return m_sUnit; }
	String SuperUnit(){ return m_sSuper; }
	int    UseUnit(Select& Units)
		{ return (Units.selected(m_sUnit) || (Super() && Units.selected(m_sSuper))); }
	double Start()    { return m_dStart; }
	double End()      { return m_dEnd; }
	Date4& Date()     { return m_Begin; }
	Date4& Finish()   { return m_End; }
	ULong  Days()     { return m_lDays; }
	int    Late()     { return (int)m_wLate; }
	int    Lead()     { return (int)m_wLead; }
	UChar  GovUsed()  { return m_wMaxUsed; }
	double Shifts()   { return m_dShifts; }
	double Distance() { return fabs(m_dEnd - m_dStart); }
	double Linear()   { return Distance() * m_pPanel->LinearFactor(); }
	double Area()     { return m_dArea; }
	double Clean()    { return m_dClean; }
	double Raw()      { return m_dRaw; }
	double *Quality() { return m_dQuality; }
	void GetGeo(double *xcoord, double *ycoord)
				   { m_pPanel->Block(m_dStart,m_dEnd,xcoord,ycoord); }
	void UnitData(double, double&, double&,	double&, double&,
				  double&, double&,	double&, double&, double&,
				  double&, double&, double&);
	long Error()   { return m_wError; }

	// implementors
	int  WorkDays() { return (m_wDays = _32BitCount(m_lDays)); }
	void Print(fileMgr&);
	//void Scan(fileMgr&); // [NEEDSWORK] Phocaz: Removed because source in cresults.cpp doesn not compile
	void SetGeometry(Array<CGeometry>&);
	void SetQuality();
	char *SectionReport(Select&,char,char,double);
	char *UnitReport(int,char,char,double);
	char *SectionItem(Select&,double);
	char *SectionXML(double);
	char *SectionSummaryItem();
	char *UnitItem(int,double);
	char *UnitXML(double);
	char *UnitSummaryItem(int,double);

	// operators
	int operator== (CResults& rhs);
	CResults& operator+= (CResults& rhs);
	CResults& operator= (CResults& rhs);
	friend ofstream& operator<< (ofstream& os, CResults& rs);
	friend ifstream& operator>> (ifstream& is, CResults& rs);
	friend fstream& operator<< (fstream& os, CResults& rs) { return os; }

private:

	CGeometry* m_pPanel;		 // reference to Panel
	String	   m_sID;			 // Panel ID
	String	   m_sType;			 // Panel typpe ('CM' or 'LW')
	double     m_dStart;		 // start station
	double     m_dEnd;			 // end station
	String     m_sUnit;			 // mining unit (machine)
	String     m_sSuper;		 // additional unit used in super-section
	Date4      m_Begin;			 // start date
	Date4      m_End;			 // end date
	int		   m_wDays;			 // number of days worked
	ULong	   m_lDays;			 // actual days worked in period (bit-mask)
	UInt       m_wLate;			 // delay time in days
	UInt       m_wLead;			 // lead time in days
	UChar      m_wMaxUsed;		 // set if advance rate governors used	
	double	   m_dShifts;		 // shifts worked
	double	   m_dArea;		     // area mined
	double	   m_dClean;		 // estimated clean tons mined
	double	   m_dRaw;			 // estimated raw tons mined
	double	   m_dQuality[26];	 // coal quality forecast
	long       m_wError;

};

#endif

//*****************************************************************************

// end of CResults.h
