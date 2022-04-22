//*****************************************************************************
//
// Module:  CSection.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CSection class
//
// Notes:   This is a base class.
//
//*****************************************************************************
// [NEEDSWORK] Phocaz: WIP


#include "CSection.h"
#include "Error.h"



//*****************************************************************************
//
// CSection - backbone members
//
//*****************************************************************************

CSection::CSection() :
	m_lAreaExceptions(0),
	m_lRefExceptions(0)
{
	return;
}

CSection::CSection(const char *id, double start, double end, const char *unit, 
				   Date4& begin, Reference& primary, Reference& secondary) : 
	m_sID(id),
	m_dStart(start),
	m_dEnd(end),
	m_dHeading(start),
	m_sUnit(unit),
	m_Begin(begin),
	m_Primary(primary),
	m_Secondary(secondary),
	m_wLate(0),
	m_wLead(0),
	m_lAreaExceptions(0),
	m_lRefExceptions(0)
{
	m_State = (long(m_Begin) > 0) ? WAIT_DATE : WAIT_DEPENDENCY;
	if (m_State == WAIT_DATE  &&  m_Primary.ID().length())
		m_State = WAIT_DEPENDENCY;
	if (m_State == WAIT_DEPENDENCY  &&  m_Secondary.ID().length())
		m_State = WAIT_DEVELOPMENT;
	return;
}

CSection::~CSection()
{
	return;
}

ostream& operator<< (ostream& os, CSection& sc)
{
	os << (int)sc.State() << ' ';
	os << sc.ID() << ' ';
	os << sc.Type() << ' ';
	os << sc.Start() << ' ';
	os << sc.End() << ' ';
	os << sc.Unit() << ' ';
	os << sc.Begin().format("MM/DD/YY") << ' ';
	os << sc.Primary() << ' ';
	os << sc.Secondary();
	return os;
}

//*****************************************************************************
//
// CSection - accessors
//
//*****************************************************************************

double CSection::TonsPerLinearFt()
{
	if (m_Advance.TnsPerFt() < eps) {
		if (m_Advance.Coal() == 'C')
			return m_pPanel->CleanTonsPerFt(m_dHeading) / m_pPanel->LinearFactor();
		else
			return m_pPanel->RawTonsPerFt(m_dHeading) / m_pPanel->LinearFactor();
	}
	return m_Advance.TnsPerFt();
}

//*****************************************************************************
//
// CSection - modifiers
//
//*****************************************************************************

void CSection::AttachGeometry(Array<CGeometry>& a_Geometry)
{
    // [NEEDSWORK] Phocaz: compiler error inside xutility

	//Pointer<CGeometry> firstG(a_Geometry,0);
	//Pointer<CGeometry> beyondG(a_Geometry,a_Geometry.size());
 //   // [NEEDSWORK] Phocaz: WIP
	////Pointer<CGeometry> pG = find(firstG,beyondG,m_sID);
 //   Pointer<CGeometry> pG = std::find ( firstG,beyondG,m_sID );
	//if (pG == beyondG) 
	//	throw ForeError(CSection_NO_GEOMETRY,m_sID);
	//m_pPanel = &(*pG);
}

// [NEEDSWORK] Phocaz: WIP
void CSection::AttachRate(Array<CRate>& a_Rates)
{
	//Pointer<CRate> firstR(a_Rates,0);
	//Pointer<CRate> beyondR(a_Rates,a_Rates.size());

	//Pointer<CRate> pR = match(firstR,beyondR,m_sUnit);
	//while (pR != beyondR && (*pR).Type() != m_pPanel->Type())
	//	pR = match(++pR,beyondR,m_sUnit);
	//if (pR == beyondR) 
	//	throw ForeError(CSection_NO_RATE,m_sID);
	//m_pOriginalRate = &(*pR);
	//m_Advance = (*pR).Rate();
}

void CSection::AttachAreaExceptions(Array<CAreaEx>& aAreaExceptions)
{
	//
	// find first exception 
	//
	Pointer<CAreaEx> start(aAreaExceptions,0);
	Pointer<CAreaEx> beyond(aAreaExceptions,aAreaExceptions.size());
	while (start != beyond  &&  this->m_sID != (*start).ID())
		++start;
	//
	// attach all exceptions for this section
	//
	while (start != beyond  &&  this->m_sID == (*start).ID()) {
        // [NEEDSWORK] Phocaz: WIP
		//m_lAreaExceptions.Enqueue(&(*start));
        m_lAreaExceptions.push_back ( *start );
		++start;
	}
}

void CSection::AttachRefExceptions(Array<CRefEx>& aRefExceptions)
{
	//
	// find first exception 
	//
	Pointer<CRefEx> start(aRefExceptions,0);
	Pointer<CRefEx> beyond(aRefExceptions,aRefExceptions.size());
	while (start != beyond  &&  this->m_sID != (*start).RefID())
		++start;
	//
	// attach all references that are within this section
	//
	while (start != beyond  &&  this->m_sID == (*start).RefID()) {
        if (this->Within ( (*start).RefStation () ))
            // [NEEDSWORK] Phocaz: WIP
            //m_lRefExceptions.Enqueue(&(*start));
            m_lRefExceptions.push_back ( *start );
		++start;
	}
}

int CSection::Finished()
{
	if (fabs(m_dHeading - m_dEnd) <= .5)
		m_State = FINISHED;
	return (m_State == FINISHED);
}

void CSection::Super(String unit)
{
	String blank;
	if (!m_sSuper.length()  ||  (m_sSuper != unit))
		m_sSuper = unit;
	else
		m_sSuper = blank;
}

void CSection::Unit(String unit)
{
	if (unit.length())	m_sUnit = unit;
}

void CSection::RePosition(List<CDateEx>& PeriodExceptions, String& UnitUsed,
						  String& SuperUsed, double& ShiftsUsed, UChar& GovUsed)
{
	//
	// use temporary rate and units if necessary
	//
	Advance tmpAdvance = m_Advance;
	String tmpUnit = m_sUnit;
	String tmpSuper = m_sSuper;
	PeriodAdjustment(PeriodExceptions);
	AreaAdjustment();
	//
	// update heading
	//
	Mine(GovUsed);
	//
	// return results 
	//
	UnitUsed = m_sUnit;
	SuperUsed = m_sSuper;
	ShiftsUsed = m_Advance.MsPerDay();
	//
	// restore defaults
	//
	m_Advance = tmpAdvance;
	m_sUnit = tmpUnit;
	m_sSuper = tmpSuper;
}

//*****************************************************************************
//
// CSection - implementors
//
//*****************************************************************************

int CSection::Within(double station)
{
      if (Advancing())
         return (station >= m_dStart-.5  &&  station <= m_dEnd+.5);
      else
         return (station <= m_dStart+.5  &&  station >= m_dEnd-.5);
}

int CSection::Reached(double station)
{
      if (Advancing())
         return (m_dHeading >= station-.5);
      else
         return (m_dHeading <= station+.5);
}

//*****************************************************************************
//
// CSection - private methods
//
//*****************************************************************************

void CSection::PeriodAdjustment(List<CDateEx>& PeriodExceptions)
{
    // [NEEDSWORK] Phocaz: WIP
	//CDateEx *p = PeriodExceptions.First();
	//while (p) {
	//	if (this->Match(p->ID())) {
	//		if (p->HasUnit()) {
	//			if (this->MatchID(p->ID()))
	//				this->Super(p->Unit());
	//			else
	//				this->Unit(p->Unit());
	//		}
	//		p->Apply(m_Advance);
	//	}
	//	p = PeriodExceptions.Next();
	//}

    for (auto p = PeriodExceptions.begin (); p != PeriodExceptions.end (); ++p)
        {
        if (this->Match(p->ID())) 
            {
		    if (p->HasUnit()) 
                {
			    if (this->MatchID(p->ID()))
				    this->Super(p->Unit());
			    else
				    this->Unit(p->Unit());
		        }
		    p->Apply(m_Advance);
	        }
        }
}

void CSection::AreaAdjustment()
{
    // [NEEDSWORK] Phocaz: Needs testing
	//CAreaEx *p = m_lAreaExceptions.First();
	//while (p) {
	//	if (this->Reached(p->End())) {
	//		m_lAreaExceptions.Remove();
	//		p = m_lAreaExceptions.Retrieve();
	//	} else {
	//		if (this->Reached(p->Start()))
	//			p->Apply(m_Advance);
	//		p = m_lAreaExceptions.Next();
	//	}
	//}

	for (auto p = m_lAreaExceptions.begin(); p != m_lAreaExceptions.end(); ++p)
		{
		if (this->Reached(p->End())) {
			p = m_lAreaExceptions.erase(p);
			--p;
			}
		else if (this->Reached(p->Start())) {
			p->Apply(m_Advance);
			}
		}

    }

void CSection::Mine(UChar& max_used)
{
	Double rate(m_Advance.Rate());
	if (rate[3] < eps)		// section is idled
		return;
	rate[2] = TonsPerLinearFt();
	if (rate[2] <= eps) 
		throw ForeError(CGeometry_NO_TPF,m_sID);

	//
	// check for advance rate governors
	//
	if (rate[0] > eps) {
		double max_tpms = m_pOriginalRate->MaxTpms();
		if (max_tpms > eps) {
			double tpms = rate[0] * rate[2];
			if (tpms > max_tpms) {
				rate[0] = 0.0;
				rate[1] = max_tpms;
				_setBitMask(&max_used,1);
			}
		}
	} else {
		double max_ftpms = m_pOriginalRate->MaxFtpms();
		if (max_ftpms > eps) {
			double ftpms = rate[1] / rate[2];
			if (ftpms > max_ftpms) {
				rate[0] = max_ftpms;
				rate[1] = 0.0;
				_setBitMask(&max_used,0);
			}
		}
	}
	//
	// calculate distance to advance
	//
//	ofstream tfile("test.dat",ios::app);
//	tfile << rate[1] << "/" << rate[2] << "/" << rate[3] << "\n";
	if (m_sSuper.length())		// double shifts if supersection
		rate[3] *= 2.;
	double advance;
	if (rate[0] > eps)			// use feet per machine shift
		advance = rate[0] * rate[3];
	else						// use tons per machine shift
		advance = (rate[1] * rate[3]) / rate[2];
	advance /= m_pPanel->LinearFactor();	// convert to section feet
	//
	// check for end of section and update heading
	//
	double end;
	if (Advancing()) {
		end = m_dHeading + advance;
		if (end > m_dEnd + .5)
			end = m_dEnd;
	} else {
		end = m_dHeading - advance;
		if (end < m_dEnd - .5)
			end = m_dEnd;
	}
	m_dHeading = end;
	m_Advance.Rate(rate);	// return advance rate used
}

//*****************************************************************************

// end of CSection.CPP
