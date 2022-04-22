//*****************************************************************************
//
// Module:  CSection.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CSection class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CSection_H
#define __CSection_H


// [NEEDSWORK] Phocaz: WIP
//#include <iostream.h>
#include <iostream>
#include <list> 
//#include "..\classlib\list.h"     
#define List std::list				// Changed List to std::list
#include "..\classlib\array.h"      
#include "..\classlib\date.h"       
#include "..\classlib\str.h"        
#include "..\classlib\fm.h"         
#include "basetype.h"
#include "reference.h"
#include "CGeometry.h"
#include "CRate.h"
#include "CDate.h"
#include "CArea.h"
#include "CRef.h"

#include <stdio.h>  // [NEEDSWORK] Phocaz: WIP

enum StateCode {
	WAIT_DATE,
	WAIT_DEPENDENCY,
	WAIT_DEVELOPMENT,
	PREPARE_DEVELOPMENT,
	MOVE_EQUIPMENT,
	RUNNING,
	FINISHED
};

// class declaration

class CSection {

public:

	// backbone members
	CSection();
	CSection(const char *, double, double, const char *,
			 Date4&, Reference&, Reference&);
	virtual ~CSection();
	friend ostream& operator<< (ostream& os, CSection& sc);

	// accessors
	String ID()      { return m_sID; }
	String Type()    { return m_pPanel->Type(); }
	double Start()   { return m_dStart; }
	double End()     { return m_dEnd; }
	int  Advancing() { return (m_dEnd > m_dStart+.5); }
	double Heading() { return m_dHeading; }
	String Unit()    { return m_sUnit; }
	Advance& Rate()  { return m_Advance; }
	double TonsPerLinearFt ();
	double Area(double start, double end) 
					{ return m_pPanel->Area(start,end); }
	Date4& Begin()          { return m_Begin; }
	Reference& Primary()    { return m_Primary; }
	Reference& Secondary()  { return m_Secondary; }
	StateCode State() const { return m_State; }
	UInt Late()	{ return m_wLate; }
	UInt Lead()	{ return m_wLead; }

    // [NEEDSWORK] Phocaz: WIP
	List<CRefEx>& References() { return m_lRefExceptions; }
    

	// modifiers
	void AttachGeometry(Array<CGeometry>&);
	void AttachRate(Array<CRate>&);
	void AttachAreaExceptions(Array<CAreaEx>&);
	void AttachRefExceptions(Array<CRefEx>&);
	void Prepare()   { m_State = PREPARE_DEVELOPMENT;	}
	void Dependent() { m_State = WAIT_DEPENDENCY; }
	void Move()      { m_State = MOVE_EQUIPMENT;	}
	void Running()   { m_State = RUNNING;	}
	int  Finished();
	void Super(String unit);
	void Unit(String unit);
	void RePosition(List<CDateEx>&, String&, String&, double&, UChar&) ;
	void Late(int n) { m_wLate += n; }
	void Lead(int n) { m_wLead += n; }

	// implementors
	int operator== (Reference& rhs)
		{ return (m_sID == rhs.ID()  &&  Within(rhs.Station())); }
	int Within(double);
	int Reached(double);
	int Match(const String& rhs) const
		{ return (!m_sID.match(rhs) || !m_sUnit.match(rhs)); }
	int MatchID(const String& rhs) const { return (!m_sID.match(rhs)); }

private:

	void PeriodAdjustment(List<CDateEx>&);
	void AreaAdjustment();
	void Mine(UChar&);

	String        m_sID;			 // name in Geometry table
	CGeometry*    m_pPanel;			 // reference to Panel
	double        m_dStart;			 // starting station
	double        m_dEnd;			 // end station
	double        m_dHeading;		 // current position (station)
	String        m_sUnit;			 // name of mining unit (machine)
	String        m_sSuper;			 // additional unit used in super-section
	CRate*        m_pOriginalRate;	 // reference to original advance rate
	Advance       m_Advance;		 // default advance rate
	Date4         m_Begin;			 // start date
	Reference     m_Primary;		 // primary reference if dependent section	
	Reference     m_Secondary;		 // secondary reference if dependent section	
	StateCode     m_State;			 // status of section
	UInt          m_wLate;			 // delay time in days
	UInt          m_wLead;			 // lead time in days
	List<CAreaEx> m_lAreaExceptions; // area exceptions to advance rate
	List<CRefEx>  m_lRefExceptions;  // reference exceptions dependent on this section

};

#endif

//*****************************************************************************

// end of CSection.h
