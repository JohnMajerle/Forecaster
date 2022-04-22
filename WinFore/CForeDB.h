//*****************************************************************************
//
// Module:  CForeDB.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CForeDB class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CForeDB_H
#define __CForeDB_H

#include "StdAfx.h"
#include "..\classlib\date.h"
#include "..\forelib\basedef.h"
#include "..\forelib\cfore.h"
#include "ResultsSet.h"

// class declaration

class CForeDB : public CDaoDatabase
{
public:

    // constructor and destructor
	CForeDB();
    virtual ~CForeDB();

	// database methods
	void Load(CFore&, int, int);
	void LoadCalendar(Calendar &a_Calendar, int nCal, Date4 StartDate, Date4 EndDate);
	void LoadAlternateCalendars(Calendar &a_Calendar);
	void LoadGeometry(Geometry &a_Geometry);
	void LoadRates(Rates &a_Rates);
	void LoadSections(Sections &a_Sections, Geometry &a_Geometry, Rates &a_Rates);
	void LoadCalendarExceptions(Calendar &a_CalExceptions);
	void LoadDateExceptions(Date &a_DateExceptions, int period);
	void LoadAreaExceptions(Area &a_AreaExceptions);
	void LoadRefExceptions(Ref &a_RefExceptions);
	void LoadShutdowns(Shutdowns &a_Shutdowns);
	long SampleNumber();
	int  LoadSamples(char*, CSample*, long*, long*, int*);
	void LoadAdjustments(char*, double*);
	
	// results methods
	void OpenResults();
	void FirstResult();
	int  NumberResults();
	void NextResult();
	void ClearResults();
	void SaveResults(ResultsList&, char, int, double, double, double);
	void LoadResultParameters(char&, double&, double&, double&);
	void LoadResults(CResults&);
	void CloseResults();

	// database creation
	void CreateCalendar();
	void CreateRates();
	void CreateGeometry();
	void CreateSequence();
	void CreateCalExceptions();
	void CreateDateExceptions();
	void CreateAreaExceptions();
	void CreateRefExceptions();
	void CreateShutdowns();
	void CreateCase();
	void CreateGrouping();
	void CreateResults();
	void CreateSamples();
	void CreateAdjustments();

private:
	Date4 m_StartDate;
	ResultsSet m_RS;
};

#endif

//*****************************************************************************

// end of CForeDB.h
