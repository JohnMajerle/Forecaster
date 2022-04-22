//*****************************************************************************
//
// Module:  CFore.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CFore class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "..\classlib\date.h"
#include "CFore.h"

//*****************************************************************************
//
// CFore - backbone members
//
//*****************************************************************************

CFore::CFore() :
	ma_Calendar(1),
	ma_CalendarExceptions(1),
	ml_ActiveCalendarExceptions(0),
	ma_DateExceptions(1),
	ma_PeriodExceptions(1),
	ml_ActivePeriodExceptions(0),
	ma_AreaExceptions(1),
	ma_RefExceptions(1),
	ma_Shutdowns(1),
	ml_ActiveShutdowns(0),
	ma_Geometry(1),
	ma_Rates(1),
	ma_Sections(1),
	ml_WaitDevelopment(0),
	ml_Prepare(0),
	ml_WaitDependency(0),
	ml_Move(0),
	ml_Running(0),
	m_lResults(0)
{
	return;
}

CFore::~CFore()
{
	return;
}

//*****************************************************************************
//
// CFore - methods
//
//*****************************************************************************

void CFore::Start(char prd)
{
	CalendarPtr tmp(ma_Calendar,0);
	mp_ForecastWeek = tmp;
	m_nForecastDay = 0;
	m_cPeriod = prd;
	m_nAvailableDays = 0;
}

int CFore::NextPeriod()
{
	CalendarPtr beyond(ma_Calendar,ma_Calendar.size());

	PeriodDelete();
	m_nAvailableDays = 0;
	while(1) {

		ForecastDay();
		m_nAvailableDays += AvailableDay();
		//
		// update day
		//
		if (m_nForecastDay == 6) {	// end of week
			mp_ForecastWeek++;
			m_nForecastDay = 0;
		} else
			m_nForecastDay++;
		if (mp_ForecastWeek == beyond)
			return 0;	// end of forecast
		//
		// check for a new period
		//
		if (m_cPeriod == 'D')
			return 1;
		else if (m_cPeriod == 'W' && m_nForecastDay == 0)
			return 1;
		else if (m_cPeriod == 'M') {
			Date4 ToDay(long((*mp_ForecastWeek).Begin())+long(m_nForecastDay));
			if (ToDay.day() == 1)
				return 1;
		}
	}
}

int CFore::AvailableDay() 
{
	String general("*");

	//CCal *pE = ml_ActiveCalendarExceptions.First();
	//while (pE) {
	//	if (pE->ID() == general)
	//		return pE->DayActive(m_nForecastDay);
	//	pE = ml_ActiveCalendarExceptions.Next();
	//}
    for (auto p = ml_ActiveCalendarExceptions.begin (); p!=ml_ActiveCalendarExceptions.end (); ++p)
        {
        if (p->ID() == general)
		    return p->DayActive(m_nForecastDay);
        }

	return (*mp_ForecastWeek).DayActive(m_nForecastDay);
}

void CFore::PeriodDelete()
{
	//for (CResults *p=m_lResults.First(); p!=0; p=m_lResults.Next())
	//	delete p;
	//m_lResults.freeContents();

    for (auto p = m_lResults.begin (); p!=m_lResults.end (); ++p)
        {
        delete &p;
        }
    m_lResults.clear ();
}

void CFore::PeriodQuality()
{
	//for (CResults *p=m_lResults.First(); p!=0; p=m_lResults.Next()) {
	//	p->SetGeometry(ma_Geometry);
	//	p->SetQuality();
	//}
    for (auto p = m_lResults.begin (); p!=m_lResults.end (); ++p)
        {
        p->SetGeometry ( ma_Geometry );
        p->SetQuality ();
        }
}

void CFore::End()
{
	PeriodDelete();
}

void CFore::ForecastDay()
{
	if (m_nForecastDay == 0)	// start of week
		CalendarExceptions((*mp_ForecastWeek).Begin());

	Date4 ToDay(long((*mp_ForecastWeek).Begin())+long(m_nForecastDay));
	DateExceptions(ToDay);
	PeriodExceptions(ToDay);
	ShutdownActivity(ToDay);
	BoundActivities(ToDay);

	if ((*mp_ForecastWeek).DayActive(m_nForecastDay)) {

		WaitingDevelopmentActivities();
		PrepareActivities();
		DependentActivities();
		MoveActivities();

		SectionList Mining(ml_Running);
		MiningExclusions(Mining,m_nForecastDay);
		MiningShutdowns(Mining);
		MiningActivities(ToDay,Mining);

	} else {

		SectionList Mining(0);
		MiningInclusions(Mining,ml_Running,m_nForecastDay);
		MiningShutdowns(Mining);
		MiningActivities(ToDay,Mining);
	}

	WaitingDevelopmentCondition();
	PrepareCondition();
	DependentCondition();
	MoveCondition(ToDay);
	RunningCondition();
}

void CFore::CalendarExceptions(Date4& ToDay) 
{
	//
	// Check for exceptions
	//
	CalendarPtr zero;
	CalendarPtr start(ma_CalendarExceptions,0);
	if (mp_NextCalendarException == zero) {
		CCal blank;
		if ((*start) == blank)
			return;	// no exceptions
		mp_NextCalendarException = start;
	}
	//
	// Add calendar exceptions to active list
	//
	CalendarPtr beyond(ma_CalendarExceptions,ma_CalendarExceptions.size());
	while (mp_NextCalendarException != beyond &&
		   ToDay >= (*mp_NextCalendarException).Begin()) {
		//ml_ActiveCalendarExceptions.Enqueue(&(*mp_NextCalendarException));
        ml_ActiveCalendarExceptions.push_back ( *mp_NextCalendarException );
		++mp_NextCalendarException;
	}
	//
	// Remove calendar exceptions from active list or check for alternate calendars
	//
	//CCal *p = ml_ActiveCalendarExceptions.First();
	//while (p) {
	//	if (ToDay >= p->End()) {
	//		ml_ActiveCalendarExceptions.Remove();
	//	    p = ml_ActiveCalendarExceptions.Retrieve();
	//	} else {
	//		p->Alternate(ToDay);
	//		p = ml_ActiveCalendarExceptions.Next();
	//	}
	//}
    for (auto p = ml_ActiveCalendarExceptions.begin (); p!=ml_ActiveCalendarExceptions.end (); ++p)
        {
        if (ToDay>=p->End ()) {
            p = ml_ActiveCalendarExceptions.erase (p);
            --p;
            }
        else {
            p->Alternate ( ToDay );
            }
        }
}

void CFore::DateExceptions(Date4& ToDay) 
{
	//
	// Check for exceptions
	//
	DatePtr zero;
	DatePtr start(ma_DateExceptions,0);
	if (mp_NextDateException == zero) {
		CDateEx blank;
		if ((*start) == blank)
			return;	// no exceptions
		mp_NextDateException = start;
	}
	//
	// Apply exceptions
	//
	DatePtr beyond(ma_DateExceptions,ma_DateExceptions.size());
	while (mp_NextDateException != beyond &&
		   ToDay >= (*mp_NextDateException).Start()) {
		ApplyException(*mp_NextDateException);
		++mp_NextDateException;
	}
}

void CFore::ApplyException(CExcept& except)
{
	SectionPtr start(ma_Sections,0);
	SectionPtr beyond(ma_Sections,ma_Sections.size());
	while (start != beyond) {
		if ((*start).Match(except.ID())) {
			if (except.HasUnit()) {
				if ((*start).MatchID(except.ID()))
					(*start).Super(except.Unit());
				else
					(*start).Unit(except.Unit());
			}
			except.Apply((*start).Rate());
		}
		++start;
	}
}

void CFore::ApplyException2Running(CExcept& except)
{
	SectionList lRunning(ml_Running);
	//CSection *p = lRunning.First();
	//while (p) {
	//	if (p->Match(except.ID())) {
	//		if (except.HasUnit()) {
	//			if (p->MatchID(except.ID()))
	//				p->Super(except.Unit());
	//			else
	//				p->Unit(except.Unit());
	//		}
	//		except.Apply(p->Rate());
	//	}
	//	p = lRunning.Next();
	//}
    for (auto p = lRunning.begin (); p!=lRunning.end (); ++p)
        {
        if (p->Match(except.ID())) {
		    if (except.HasUnit()) {
			    if (p->MatchID(except.ID()))
				    p->Super(except.Unit());
			    else
				    p->Unit(except.Unit());
		    }
		    except.Apply(p->Rate());
	        }
        }
}

void CFore::PeriodExceptions(Date4& ToDay) 
{
	//
	// Check for exceptions
	//
	DatePtr zero;
	DatePtr start(ma_PeriodExceptions,0);
	if (mp_NextPeriodException == zero) {
		CDateEx blank;
		if ((*start) == blank)
			return;	// no exceptions
		mp_NextPeriodException = start;
	}
	//
	// Add period exceptions to active list
	//
	DatePtr beyond(ma_PeriodExceptions,ma_PeriodExceptions.size());
	while (mp_NextPeriodException != beyond &&
		   ToDay >= (*mp_NextPeriodException).Start()) {
		//ml_ActivePeriodExceptions.Enqueue(&(*mp_NextPeriodException));
        ml_ActivePeriodExceptions.push_back (*mp_NextPeriodException);
		++mp_NextPeriodException;
	}
	//
	// Remove period exceptions from active list
	//
	//CDateEx *p = ml_ActivePeriodExceptions.First();
	//while (p) {
	//	if (ToDay > p->End()) {
	//		ml_ActivePeriodExceptions.Remove();
	//	    p = ml_ActivePeriodExceptions.Retrieve();
	//	} else 
	//		p = ml_ActivePeriodExceptions.Next();
	//}

    for (auto p = ml_ActivePeriodExceptions.begin (); p!=ml_ActivePeriodExceptions.end (); ++p)
        {
        if (ToDay>p->End ()) {
            p = ml_ActivePeriodExceptions.erase ( p );
            --p;
            }
        }

}

void CFore::ShutdownActivity(Date4& ToDay) 
{
	//
	// Check for exceptions
	//
	ShutdownPtr zero;
	ShutdownPtr start(ma_Shutdowns,0);
	if (mp_NextShutdown == zero) {
		CShutdown blank;
		if ((*start) == blank)
			return;	// no exceptions
		mp_NextShutdown = start;
	}
	//
	// Add shutdowns to active list
	//
	ShutdownPtr beyond(ma_Shutdowns,ma_Shutdowns.size());
	while (mp_NextShutdown != beyond &&
		   ToDay >= (*mp_NextShutdown).Start()) {
		//ml_ActiveShutdowns.Enqueue(&(*mp_NextShutdown));
        ml_ActiveShutdowns.push_back (*mp_NextShutdown);
		++mp_NextShutdown;
	}
	//
	// Remove shutdowns from active list
	//
	//CShutdown *p = ml_ActiveShutdowns.First();
	//while (p) {
	//	if (ToDay > p->End()) {
	//		ml_ActiveShutdowns.Remove();
	//	    p = ml_ActiveShutdowns.Retrieve();
	//	} else 
	//		p = ml_ActiveShutdowns.Next();
	//}

    for (auto p = ml_ActiveShutdowns.begin (); p!=ml_ActiveShutdowns.end (); ++p)
        {
        if (ToDay>p->End ()) {
            p = ml_ActiveShutdowns.erase ( p );
            --p;
            }
        }
}

void CFore::BoundActivities(Date4& ToDay) 
{
	//
	// Check for first section waiting on a date
	//
	SectionPtr zero;
	SectionPtr start(ma_Sections,0);
	SectionPtr beyond(ma_Sections,ma_Sections.size());
	if (mp_NextWaitDate == zero) {
		while (start != beyond) {
			if ((*start).State() == WAIT_DATE) {
				mp_NextWaitDate = start;
				break;
			}
			++start;
		}
		if (start == beyond) return;	// this should not occur
	}
	//
	// Check all waiting sections
	//
	while (mp_NextWaitDate != beyond && ToDay >= (*mp_NextWaitDate).Begin()) {
		if ((*mp_NextWaitDate).State() == WAIT_DATE) {
			(*mp_NextWaitDate).AttachAreaExceptions(ma_AreaExceptions);
			(*mp_NextWaitDate).AttachRefExceptions(ma_RefExceptions);
			(*mp_NextWaitDate).Running();
			//ml_Running.Enqueue(&(*mp_NextWaitDate));
            ml_Running.push_back (*mp_NextWaitDate);
			CheckDependencies(*mp_NextWaitDate);
		}
		++mp_NextWaitDate;
	}
}

void CFore::WaitingDevelopmentActivities() 
{
	//CSection *p = ml_WaitDevelopment.First();
	//while (p) {
	//	Reference& primary = p->Primary();
	//	CSection*  pSection = primary.pSection();
	//	if (pSection && pSection->Reached(primary.Station()))
	//		p->Late(1);
	//	p = ml_WaitDevelopment.Next();
	//}

    for (auto p = ml_WaitDevelopment.begin (); p!=ml_WaitDevelopment.end (); ++p)
        {
        Reference& primary = p->Primary ();
        CSection*  pSection = primary.pSection ();
        if (pSection && pSection->Reached ( primary.Station () ))
            p->Late ( 1 );
        }
}

void CFore::PrepareActivities() 
{
	//CSection *p = ml_Prepare.First();
	//while (p) {
	//	Reference& primary = p->Primary();
	//	Reference& secondary = p->Secondary();
	//	CSection*  pSection = primary.pSection();
	//	if (pSection && pSection->Reached(primary.Station())) {
	//		p->Lead(-(int)p->Lead());
	//		p->Late(1);
	//	} else
	//		p->Lead(1);
	//	secondary.Delay(-1);
	//	p = ml_Prepare.Next();
	//}

    for (auto p = ml_Prepare.begin (); p!=ml_Prepare.end (); ++p)
        {
        Reference& primary = p->Primary ();
        Reference& secondary = p->Secondary ();
        CSection*  pSection = primary.pSection ();
        if (pSection && pSection->Reached ( primary.Station () )) {
            p->Lead ( -(int)p->Lead () );
            p->Late ( 1 );
            }
        else
            p->Lead ( 1 );
        secondary.Delay ( -1 );
        }
}

void CFore::DependentActivities() 
{
	//CSection *p = ml_WaitDependency.First();
	//while (p) {
	//	Reference& secondary = p->Secondary();
	//	String ID = secondary.ID();
	//	if (ID.length())
	//		p->Lead(1);
	//	p = ml_WaitDependency.Next();
	//}

    for (auto p = ml_WaitDependency.begin (); p!=ml_WaitDependency.end (); ++p)
        {
        Reference& secondary = p->Secondary ();
        String ID = secondary.ID ();
        if (ID.length ())
            p->Lead ( 1 );
        }
}

void CFore::MoveActivities() 
{
	//CSection *p = ml_Move.First();
	//while (p) {
	//	Reference& primary = p->Primary();
	//	primary.Delay(-1);
	//	p = ml_Move.Next();
	//}

    for (auto p = ml_Move.begin (); p!=ml_Move.end (); ++p)
        {
        Reference& primary = p->Primary ();
        primary.Delay ( -1 );
        }
}

void CFore::MiningExclusions(SectionList& Mining, int today) 
{
    // [NEEDSWORK] Phocaz: WIP
	//CCal *pE = ml_ActiveCalendarExceptions.First();
	//while (pE) {
	//	if (!pE->DayActive(today)) {
	//		CSection *pS = Mining.First();
	//		while (pS) {
	//			if (pS->Match(pE->ID())) {
	//				Mining.Remove();
	//				pS = Mining.Retrieve();
	//			} else
	//				pS = Mining.Next();
	//		}
	//	}
	//	pE = ml_ActiveCalendarExceptions.Next();
	//}
}

void CFore::MiningInclusions(SectionList& Mining, SectionList& Runing, int today) 
{
    // [NEEDSWORK] Phocaz: WIP
	//CCal *pE = ml_ActiveCalendarExceptions.First();
	//while (pE) {
	//	if (pE->DayActive(today)) {
	//		CSection *pS = Runing.First();
	//		while (pS) {
 //               if (pS->Match ( pE->ID () )&&!Mining.Member ( pS ))
 //                   //Mining.Enqueue(pS);
 //                   Mining.push_back ( *pS );
	//			pS = Runing.Next();
	//		}
	//	}
	//	pE = ml_ActiveCalendarExceptions.Next();
	//}
}

void CFore::MiningShutdowns(SectionList& Mining) 
{
    // [NEEDSWORK] Phocaz: WIP
	//CShutdown *pS = ml_ActiveShutdowns.First();
	//while (pS) {				// for each active shutdown
	//	CSection *pM = ml_Move.First();
	//	while(pM) {				// check each section waiting for move
	//		if (pM->Unit() == pS->Unit()) {	
	//			// unit being moved causes shutdowns
	//			CSection *pR = Mining.First();
	//			while (pR) {	// check if section should be removed from mining list
	//				if (pR->Match(pS->ID())) {
	//					Mining.Remove();
	//					pR = Mining.Retrieve();
	//				} else
	//					pR = Mining.Next();
	//			}
	//		}
	//		pM = ml_Move.Next();
	//	}
	//	pS = ml_ActiveShutdowns.Next();
	//}
}

void CFore::MiningActivities(Date4& ToDay, SectionList& Mining) 
{
    // [NEEDSWORK] Phocaz: WIP
	//CSection *p = Mining.First();
	//while (p) {
	//	double ShiftsUsed;
	//	UChar GovUsed=0;
	//	String UnitUsed,SuperUsed;
	//	double Previous = p->Heading();
	//	p->RePosition(ml_ActivePeriodExceptions,UnitUsed,SuperUsed,ShiftsUsed,
	//				GovUsed);
	//	if (fabs(p->Heading()-Previous) > .5) {		
	//		// section moved so store results
	//		//
	//		ULong days=0;
	//		_set32BitMask(&days,ToDay.day());
	//		CResults *pR = new CResults(p->ID(),
	//									p->Type(),
	//									Previous,
	//									p->Heading(),
	//									UnitUsed,
	//									SuperUsed,
	//									ToDay,
	//									days,
	//									p->Late(),
	//									p->Lead(),
	//									GovUsed,
	//									ShiftsUsed,
	//									p->Area(p->Heading(),Previous),
	//									0.0,0.0);
	//		if (p->Late()) p->Late(-int(p->Late()));		// reset late time
	//		if (p->Lead()) p->Lead(-int(p->Lead()));		// reset lead time
	//		for (CResults *p=m_lResults.First(); p!=0; p=m_lResults.Next()) {
	//			if (*p == *pR)
	//				break;
	//		}
	//		if (p) {		// add to existing results for a section
 //               // [NEEDSWORK] Phocaz: WIP
	//			//*p += *pR;
	//			delete pR;
 //               }
 //           else			// append new section results
 //            //m_lResults.Enqueue(pR);
 //               m_lResults.push_back ( *pR );
	//	}
	//	p = Mining.Next();
	//}
}

void CFore::WaitingDevelopmentCondition() 
{
    // [NEEDSWORK] Phocaz: WIP
	//CSection *p = ml_WaitDevelopment.First();
	//while (p) {
	//	Reference& secondary = p->Secondary();
	//	CSection*  pSection = secondary.pSection();
	//	if (pSection && pSection->Reached(secondary.Station())) {
	//		p->Prepare();
	//		//ml_Prepare.Enqueue(p);
 //           ml_Prepare.push_back ( *p );
	//		ml_WaitDevelopment.Remove();
	//		p = ml_WaitDevelopment.Retrieve();
	//    } else
	//		p = ml_WaitDevelopment.Next();
	//}
}

void CFore::PrepareCondition() 
{
    // [NEEDSWORK] Phocaz: WIP
	//CSection *p = ml_Prepare.First();
	//while (p) {
	//	Reference& secondary = p->Secondary();
	//	if (!secondary.Delay()) {
	//		p->Dependent();
	//		//ml_WaitDependency.Enqueue(p);
 //           ml_WaitDependency.push_back ( *p );
	//		ml_Prepare.Remove();
	//		p = ml_Prepare.Retrieve();
	//    } else
	//		p = ml_Prepare.Next();
	//}
}

void CFore::DependentCondition() 
{
    // [NEEDSWORK] Phocaz: WIP
	//CSection *p = ml_WaitDependency.First();
	//while (p) {
	//	Reference& primary = p->Primary();
	//	CSection*  pSection = primary.pSection();
	//	if (pSection && pSection->Reached(primary.Station())) {
	//		p->Move();
	//		//ml_Move.Enqueue(p);
 //           ml_Move.push_back ( *p );
	//		ml_WaitDependency.Remove();
	//		p = ml_WaitDependency.Retrieve();
	//    } else
	//		p = ml_WaitDependency.Next();
	//}
}

void CFore::MoveCondition(Date4& ToDay) 
{
    // [NEEDSWORK] Phocaz: WIP
	//CSection *p = ml_Move.First();
	//while (p) {
	//	Reference& primary = p->Primary();
	//	if (!primary.Delay()) {
	//		if ((long(p->Begin()) > 0)  &&  (p->Begin() > ToDay)) {
	//			// date constraint not met
	//			primary.Delay(1);
	//			p->Lead(-(int)p->Lead());
	//			p->Late(1);
	//			p = ml_Move.Next();
	//			continue;
	//		}
	//		p->AttachAreaExceptions(ma_AreaExceptions);
	//		p->AttachRefExceptions(ma_RefExceptions);
	//		p->Running();
	//		//ml_Running.Enqueue(p);
 //           ml_Running.push_back ( *p );
	//		CheckDependencies(*p);
	//		ml_Move.Remove();
	//		p = ml_Move.Retrieve();
	//    } else
	//		p = ml_Move.Next();
	//}
}

void CFore::RunningCondition() 
{
    // [NEEDSWORK] Phocaz: WIP
	//CSection *p = ml_Running.First();
	//while (p) {
	//	CheckReferences(*p);
	//	if (p->Finished()) {
	//		ml_Running.Remove();
	//		p = ml_Running.Retrieve();
	//	} else
	//		p = ml_Running.Next();
	//}
}

void CFore::CheckDependencies(CSection& section)
{
    // [NEEDSWORK] Phocaz: WIP
	//Pointer<CSection> start(ma_Sections,0);
	//Pointer<CSection> beyond(ma_Sections,ma_Sections.size());

	//while (start != beyond) {
	//	CSection& current = *start;
	//	Reference& primary = current.Primary();
	//	Reference& secondary = current.Secondary();

	//	if (current.State() == WAIT_DEVELOPMENT  &&
	//		(section == primary || section == secondary)) {

	//		if (section == primary)	  primary.Attach(&section);
	//		if (section == secondary) secondary.Attach(&section);
	//		if (!ml_WaitDevelopment.Member(&current))
	//			//ml_WaitDevelopment.Enqueue(&current);
 //               ml_WaitDevelopment.push_back ( current );

	//	} else if (current.State() == PREPARE_DEVELOPMENT  &&  section == primary) {

	//		primary.Attach(&section);

	//	} else if (current.State() == WAIT_DEPENDENCY  &&  section == primary) {

	//		primary.Attach(&section);
	//		if (!ml_WaitDependency.Member(&current))	// might have been put there after development
	//			//ml_WaitDependency.Enqueue(&(*start));
 //               ml_WaitDependency.push_back (*start);

	//	}

	//	++start;
	//}
}

void CFore::CheckReferences(CSection& section)
{
    // [NEEDSWORK] Phocaz: WIP
	//List<CRefEx>& lRefs = section.References();
	//CRefEx *p = lRefs.First();
	//while (p) {
	//	if (section.Reached(p->RefStation())) {
	//		ApplyException2Running(*p);
	//		lRefs.Remove();
	//		p = lRefs.Retrieve();
	//	} else
	//		p = lRefs.Next();
	//}
}

//*****************************************************************************

// end of CFore.CPP
