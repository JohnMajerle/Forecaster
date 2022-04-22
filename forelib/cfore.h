//*****************************************************************************
//
// Module:  CFore.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CFore class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CFore_H
#define __CFore_H

#include "basedef.h"

// class declaration

class CFore {

public:

	// backbone members
	CFore();
	virtual ~CFore();

	// accessors
	char *ToDay ()
	{ 
		Date4 ToDay(long((*mp_ForecastWeek).Begin())+long(m_nForecastDay));
		return ToDay.format("MM/DD/YY");
	}
	int          AvailableDays()       { return m_nAvailableDays; }
	Calendar&    WorkCalendar()        { return ma_Calendar; }
	Geometry&    SectionGeometry()     { return ma_Geometry; }
	Rates&       AdvanceRates()        { return ma_Rates; }
	Sections&    SectionSequence()     { return ma_Sections; }
	Calendar&    CalendarExceptions()  { return ma_CalendarExceptions; }
	Date&        DateExceptions()      { return ma_DateExceptions; }
	Date&        PeriodExceptions()    { return ma_PeriodExceptions; }
	Area&        AreaExceptions()      { return ma_AreaExceptions; }
	Ref&         ReferenceExceptions() { return ma_RefExceptions; }
	Shutdowns&   ShutdownExceptions()  { return ma_Shutdowns; }
	ResultsList& Results()	 		   { return m_lResults;}

	// methods
	void Start(char);
	int  NextPeriod();
	void PeriodDelete();
	void PeriodQuality();
	void End();

private:

	int  AvailableDay() ;
	void ForecastDay() ;
	void CalendarExceptions(Date4&) ;
	void DateExceptions(Date4&) ;
	void PeriodExceptions(Date4&) ;
	void ShutdownActivity(Date4&) ;
	void BoundActivities(Date4&) ;
	void WaitingDevelopmentActivities() ;
	void PrepareActivities() ;
	void DependentActivities() ;
	void MoveActivities() ;
	void MiningExclusions(SectionList&, int) ;
	void MiningInclusions(SectionList&, SectionList&, int) ;
	void MiningShutdowns(SectionList&) ;
	void MiningActivities(Date4&,SectionList&) ;
	void WaitingDevelopmentCondition() ;
	void PrepareCondition() ;
	void DependentCondition() ;
	void MoveCondition(Date4&) ;
	void RunningCondition() ;
	void ApplyException(CExcept& except);
	void ApplyException2Running(CExcept& except);
	void CheckDependencies(CSection&);
	void CheckReferences(CSection&);

	char         m_cPeriod;
	Calendar     ma_Calendar;
	CalendarPtr  mp_ForecastWeek;
	int			 m_nForecastDay;
	Calendar     ma_CalendarExceptions;
	CalendarPtr  mp_NextCalendarException;
	CalendarList ml_ActiveCalendarExceptions;
	Date         ma_DateExceptions;
	DatePtr      mp_NextDateException;
	Date         ma_PeriodExceptions;
	DatePtr      mp_NextPeriodException;
	DateList     ml_ActivePeriodExceptions;
	Area         ma_AreaExceptions;
	Ref          ma_RefExceptions;
	Shutdowns    ma_Shutdowns;
	ShutdownPtr  mp_NextShutdown;
	ShutdownList ml_ActiveShutdowns;
	Geometry     ma_Geometry;
	Rates        ma_Rates;
	Sections     ma_Sections;
	SectionPtr   mp_NextWaitDate;
	SectionList  ml_WaitDevelopment;
	SectionList  ml_Prepare;
	SectionList  ml_WaitDependency;
	SectionList  ml_Move;
	SectionList  ml_Running;
	ResultsList  m_lResults;
	int          m_nAvailableDays;
};

#endif

//*****************************************************************************

// end of CFore.h
