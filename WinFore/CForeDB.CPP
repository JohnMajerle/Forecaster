//*****************************************************************************
//
// Module:  CForeDB.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CForeDB class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "stdafx.h"

#include "CForeDB.h"
#include "CError.h"
#include "CalSet.h"
#include "GeometrySet.h"
#include "RateSet.h"
#include "SectionSet.h"
#include "CalExSet.h"
#include "DateSet.h"
#include "AreaSet.h"
#include "RefSet.h"
#include "ShutSet.h"
#include "SampleSet.h"
#include "AdjustSet.h"

//*****************************************************************************
//
// CForeDB - backbone members
//
//*****************************************************************************

CForeDB::CForeDB()
{
   return;
}

CForeDB::~CForeDB()
{
   return;
}

//*****************************************************************************
//
// CForeDB - database members
//
//*****************************************************************************


void CForeDB::Load(CFore& aFore, int nCal, int nWeeks) 
{
	LoadGeometry(aFore.SectionGeometry());
	LoadRates(aFore.AdvanceRates());
	LoadSections(aFore.SectionSequence(),aFore.SectionGeometry(),aFore.AdvanceRates());
	int dow = m_StartDate.dow();
	if (dow == 1) dow = 8;
	Date4 MonDate(m_StartDate - (long)(dow-2));
	Date4 EndDate(MonDate + (long)((nWeeks-1)*7));
	LoadCalendar(aFore.WorkCalendar(),nCal,MonDate,EndDate);
	LoadCalendarExceptions(aFore.CalendarExceptions());
	LoadAlternateCalendars(aFore.CalendarExceptions());
	LoadDateExceptions(aFore.DateExceptions(),0);
	LoadDateExceptions(aFore.PeriodExceptions(),1);
	LoadAreaExceptions(aFore.AreaExceptions());
	LoadRefExceptions(aFore.ReferenceExceptions());
	LoadShutdowns(aFore.ShutdownExceptions());
}


void CForeDB::LoadCalendar(Calendar &a_Calendar, int nCal, Date4 StartDate, Date4 EndDate)
{
	char strStart[9],strEnd[9];
	CalSet CS;
	CString str;

	ASSERT(IsOpen());

	StartDate.format(strStart,"MM/DD/CCYY");
	EndDate.format(strEnd,"MM/DD/CCYY");
	str.Format(_T("[CAL_NO] = %i And [MON_DATE] >= DateValue('%10.10s') And "
			   "[MON_DATE] <= DateValue('%10.10s')"),nCal,strStart,strEnd);
	//str.Format(_T("[MON_DATE] >= DateValue('%8.8s') And "
	//		   "[MON_DATE] <= DateValue('%8.8s')"),strStart,strEnd);
	CS.m_pDatabase = this;
	CS.m_strFilter = str;
	CS.m_strSort = "[MON_DATE]";

	CS.Open();
	if (CS.IsEOF() && CS.IsBOF())
	  throw ErrorInfo(CForeDB_NO_RECORDS,str);
	CS.MoveLast();
	int nRecords = CS.GetAbsolutePosition()+1;
	a_Calendar.resize(nRecords);
	CS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  Date4 CurrentDate(CS.m_MON_DATE.Format("%Y%m%d"));
	  Date4 Blank;
	  CCal tmp("*",CurrentDate,Blank,0,CS.Days());
	  a_Calendar[nCount] = tmp;
	  CS.MoveNext();
	}
	CS.Close();
}


void CForeDB::LoadAlternateCalendars(Calendar &a_CalendarExceptions)
{
	CalendarPtr start(a_CalendarExceptions,0);
	CalendarPtr beyond(a_CalendarExceptions,a_CalendarExceptions.size());
	CCal blank;
	if ((*start) == blank)
		return;		// no calendar exceptions
	while (start != beyond) {
		int cal_no = (*start).Alternate();
		if (cal_no) {
			Calendar *pa = new Calendar(1);
			(*start).AttachAlternate(pa);
			LoadCalendar(*pa,cal_no,(*start).Begin(),(*start).End());
		}
		++start;
	}
}


void CForeDB::LoadGeometry(Geometry &a_Geometry)
{
	Double geodata(NUMGEO);
	GeometrySet GS;

	ASSERT(IsOpen());

	GS.m_pDatabase = this;
	GS.m_strSort = "[SECT]";

	GS.Open();
	if (GS.IsEOF() && GS.IsBOF())
	  throw ErrorInfo(CForeDB_NO_RECORDS,"Geometry");
	GS.MoveLast();
	int nRecords = GS.GetAbsolutePosition()+1;
	a_Geometry.resize(nRecords);
	GS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  geodata[0] = GS.m_INIT_STN;
	  geodata[1] = GS.m_X_COORD;
	  geodata[2] = GS.m_Y_COORD;
	  geodata[3] = GS.m_AZIMUTH;
	  geodata[4] = GS.m_LENGTH;
	  geodata[5] = GS.m_WIDTH;
	  geodata[6] = GS.m_LINEAR_FACTOR;
	  geodata[7] = GS.m_RECOV;
	  for (int i=0; i<NUMGEO; i++)
		  if (geodata[i] == AFX_RFX_DOUBLE_PSEUDO_NULL)
			  geodata[i] = 0.0;
	  CGeometry tmp(LPCTSTR(GS.m_SECT),LPCTSTR(GS.m_TYPE),geodata);
	  a_Geometry[nCount] = tmp;
	  GS.MoveNext();
	}
	GS.Close();
}


void CForeDB::LoadRates(Rates &a_Rates)
{
	Double ratedata(NUMADV),govdata(2);
	RateSet RS;

	ASSERT(IsOpen());

	RS.m_pDatabase = this;
	RS.Open();
	if (RS.IsEOF() && RS.IsBOF())
	  throw ErrorInfo(CForeDB_NO_RECORDS,"Rates");
	RS.MoveLast();
	int nRecords = RS.GetAbsolutePosition()+1;
	a_Rates.resize(nRecords);
	RS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  ratedata[0] = RS.m_FT_P_MS;
	  ratedata[1] = RS.m_TNS_P_MS;
	  ratedata[2] = RS.m_TNS_P_FT;
	  ratedata[3] = RS.m_MS_P_DAY;
	  govdata[0]  = RS.m_MAX_TPMS;
	  govdata[1]  = RS.m_MAX_FTPMS;
	  for (int i=0; i<NUMADV; i++)
		  if (ratedata[i] == AFX_RFX_DOUBLE_PSEUDO_NULL)
			  ratedata[i] = 0.0;
	  for (int j=0; j<2; j++)
		  if (govdata[j] == AFX_RFX_DOUBLE_PSEUDO_NULL)
			  govdata[j] = 0.0;
	  CRate tmp(LPCTSTR(RS.m_UNIT),LPCTSTR(RS.m_TYPE),ratedata,
				*LPCTSTR(RS.m_COAL),govdata);
	  a_Rates[nCount] = tmp;
	  RS.MoveNext();
	}
	RS.Close();
}


void CForeDB::LoadSections(Sections &a_Sections, Geometry &a_Geometry, Rates &a_Rates)
{
	SectionSet SS;
	SectionPtr pZero;

	ASSERT(IsOpen());

	SS.m_pDatabase = this;
	SS.m_strSort = "[BEGIN]";
	SS.Open();
	if (SS.IsEOF() && SS.IsBOF())
	  throw ErrorInfo(CForeDB_NO_RECORDS,"Sequence");
	SS.MoveLast();
	int nRecords = SS.GetAbsolutePosition()+1;
	a_Sections.resize(nRecords);
	SS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  CString strBegin;
	  strBegin = SS.m_BEGIN.GetStatus()?
				 "        " :
				 SS.m_BEGIN.Format("%Y%m%d");
	  Date4 Begin(strBegin);
	  if (SS.m_STN1 == AFX_RFX_DOUBLE_PSEUDO_NULL) SS.m_STN1 = 0.0;
	  if (SS.m_D1 == AFX_RFX_INT_PSEUDO_NULL) SS.m_D1 = 0;
	  Reference Primary(LPCTSTR(SS.m_REF1),SS.m_STN1,(UInt)SS.m_D1);	
	  if (SS.m_STN2 == AFX_RFX_DOUBLE_PSEUDO_NULL) SS.m_STN2 = 0.0;
	  if (SS.m_D2 == AFX_RFX_INT_PSEUDO_NULL) SS.m_D2 = 0;
	  Reference Secondary(LPCTSTR(SS.m_REF2),SS.m_STN2,(UInt)SS.m_D2);	
	  if (SS.m_START == AFX_RFX_DOUBLE_PSEUDO_NULL) SS.m_START = 0.0;
	  if (SS.m_END == AFX_RFX_DOUBLE_PSEUDO_NULL) SS.m_END = 0.0;
	  CSection tmp(LPCTSTR(SS.m_SECT),SS.m_START,SS.m_END,LPCTSTR(SS.m_UNIT),
				   Begin,Primary,Secondary);
	  tmp.AttachGeometry(a_Geometry);
	  tmp.AttachRate(a_Rates);
	  a_Sections[nCount] = tmp;

	  Date4 blank;
	  if ((tmp.State() == WAIT_DATE)  &&  ((long)m_StartDate == (long)blank)) {
		  // start of date list
		  m_StartDate = tmp.Begin();
	  }
	  SS.MoveNext();
	}
	SS.Close();
}


void CForeDB::LoadCalendarExceptions(Calendar &a_CalExceptions)
{
	CalExSet CES;

	ASSERT(IsOpen());

	CES.m_pDatabase = this;
	CES.m_strSort = "[START_MON]";
	CES.Open();
	if (CES.IsEOF() && CES.IsBOF()) 
	  return;
	CES.MoveLast();
	int nRecords = CES.GetAbsolutePosition()+1;
	a_CalExceptions.resize(nRecords);
	CES.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  Date4 Begin(CES.m_START_MON.Format("%Y%m%d"));
	  Date4 End(CES.m_END_MON.Format("%Y%m%d"));
	  CCal tmp(LPCTSTR(CES.m_ID),Begin,End,(int)CES.m_CAL_NO,
			   CES.Days());
	  a_CalExceptions[nCount] = tmp;
	  CES.MoveNext();
	}
	CES.Close();
}


void CForeDB::LoadDateExceptions(Date &a_DateExceptions, int period)
{
	Double ratedata(NUMADV);
	DateSet DS;

	ASSERT(IsOpen());

	DS.m_pDatabase = this;
	DS.m_strFilter = period? "[END_DATE] IS NOT NULL" : "[END_DATE] IS NULL";
	DS.m_strSort = "[START_DATE]";

	DS.Open();
	if (DS.IsEOF() && DS.IsBOF())
	  return;
	DS.MoveLast();
	int nRecords = DS.GetAbsolutePosition()+1;
	a_DateExceptions.resize(nRecords);
	DS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  Date4 Start(DS.m_START_DATE.Format("%Y%m%d"));
	  Date4 End(DS.m_END_DATE.Format("%Y%m%d"));
	  ratedata[0] = DS.m_FT_P_MS;
	  ratedata[1] = DS.m_TNS_P_MS;
	  ratedata[2] = DS.m_TNS_P_FT;
	  ratedata[3] = DS.m_MS_P_DAY;
	  for (int i=0; i<NUMADV; i++)
		  if (ratedata[i] == AFX_RFX_DOUBLE_PSEUDO_NULL)
			  ratedata[i] = 0.0;
	  if (DS.m_INCRS == AFX_RFX_DOUBLE_PSEUDO_NULL) DS.m_INCRS = 0.0;
	  CDateEx tmp(LPCTSTR(DS.m_ID),Start,End,LPCTSTR(DS.m_UNIT),
				  ratedata,DS.m_INCRS);
	  a_DateExceptions[nCount] = tmp;
	  DS.MoveNext();
	}
	DS.Close();
}


void CForeDB::LoadAreaExceptions(Area &a_AreaExceptions)
{
	Double ratedata(NUMADV);
	AreaSet AS;

	ASSERT(IsOpen());

	AS.m_pDatabase = this;
	AS.m_strSort = "[ID],[START_STN]";

	AS.Open();
	if (AS.IsEOF() && AS.IsBOF()) 
	  return;
	AS.MoveLast();
	int nRecords = AS.GetAbsolutePosition()+1;
	a_AreaExceptions.resize(nRecords);
	AS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  if (AS.m_START_STN == AFX_RFX_DOUBLE_PSEUDO_NULL) AS.m_START_STN = 0.0;
	  if (AS.m_END_STN == AFX_RFX_DOUBLE_PSEUDO_NULL) AS.m_END_STN = 0.0;
	  ratedata[0] = AS.m_FT_P_MS;
	  ratedata[1] = AS.m_TNS_P_MS;
	  ratedata[2] = AS.m_TNS_P_FT;
	  ratedata[3] = AS.m_MS_P_DAY;
	  for (int i=0; i<NUMADV; i++)
		  if (ratedata[i] == AFX_RFX_DOUBLE_PSEUDO_NULL)
			  ratedata[i] = 0.0;
	  if (AS.m_INCRS == AFX_RFX_DOUBLE_PSEUDO_NULL) AS.m_INCRS = 0.0;
	  CAreaEx tmp(LPCTSTR(AS.m_ID),AS.m_START_STN,AS.m_END_STN,
				  ratedata,AS.m_INCRS);
	  a_AreaExceptions[nCount] = tmp;
	  AS.MoveNext();
	}
	AS.Close();
}


void CForeDB::LoadRefExceptions(Ref &a_RefExceptions)
{
	Double ratedata(NUMADV);
	RefSet RS;

	ASSERT(IsOpen());

	RS.m_pDatabase = this;
	RS.m_strSort = "[REF_ID],[REF_STN]";

	RS.Open();
	if (RS.IsEOF() && RS.IsBOF()) 
	  return;
	RS.MoveLast();
	int nRecords = RS.GetAbsolutePosition()+1;
	a_RefExceptions.resize(nRecords);
	RS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  if (RS.m_REF_STN == AFX_RFX_DOUBLE_PSEUDO_NULL) RS.m_REF_STN = 0.0;
	  ratedata[0] = RS.m_FT_P_MS;
	  ratedata[1] = RS.m_TNS_P_MS;
	  ratedata[2] = RS.m_TNS_P_FT;
	  ratedata[3] = RS.m_MS_P_DAY;
	  for (int i=0; i<NUMADV; i++)
		  if (ratedata[i] == AFX_RFX_DOUBLE_PSEUDO_NULL)
			  ratedata[i] = 0.0;
	  if (RS.m_INCRS == AFX_RFX_DOUBLE_PSEUDO_NULL) RS.m_INCRS = 0.0;
	  CRefEx tmp(LPCTSTR(RS.m_ID),LPCTSTR(RS.m_REF_ID),RS.m_REF_STN,
				 LPCTSTR(RS.m_UNIT),ratedata,RS.m_INCRS);
	  a_RefExceptions[nCount] = tmp;
	  RS.MoveNext();
	}
	RS.Close();
}


void CForeDB::LoadShutdowns(Shutdowns &a_Shutdowns)
{
	ShutSet SS;

	ASSERT(IsOpen());

	SS.m_pDatabase = this;
	SS.m_strSort = "[START_DATE]";

	SS.Open();
	if (SS.IsEOF() && SS.IsBOF()) 
	  return;
	SS.MoveLast();
	int nRecords = SS.GetAbsolutePosition()+1;
	a_Shutdowns.resize(nRecords);
	SS.MoveFirst();
	for (int nCount=0; nCount<nRecords; nCount++) {
	  Date4 Start(SS.m_START_DATE.Format("%Y%m%d"));
	  Date4 End(SS.m_END_DATE.Format("%Y%m%d"));
	  CShutdown tmp(LPCTSTR(SS.m_ID),LPCTSTR(SS.m_UNIT),Start,End);
	  a_Shutdowns[nCount] = tmp;
	  SS.MoveNext();
	}
	SS.Close();
}


long CForeDB::SampleNumber()
{
	long SampleNumber;
	SampleSet SS;

	ASSERT(IsOpen());

	SS.m_pDatabase = this;
	SS.Open();
	if (SS.IsEOF() && SS.IsBOF())
		throw ErrorInfo(CForeDB_NO_RECORDS,"Samples");

	SS.MoveLast();
	SampleNumber = SS.GetAbsolutePosition()+1;
	SS.Close();

	return SampleNumber;
}


int CForeDB::LoadSamples(char *type, CSample *pSample, long *pXCoord, long *pYCoord,
						 int  *pQuality)
{
	double quality[NUMQUAL];
	SampleSet SS;

	ASSERT(IsOpen());

	SS.m_pDatabase = this;
	CString str;
	str.Format(_T("[TYPE] = '%s'"),type);
	SS.m_strFilter = str;
	SS.m_strSort = "[Y_COORD]";

	SS.Open();
	if (SS.IsEOF() && SS.IsBOF()) 
		throw ErrorInfo(CForeDB_NO_RECORDS,"Samples");
	SS.MoveLast();
	int nRecords = SS.GetAbsolutePosition()+1;
	int nCount = 0;

	while ( !SS.IsBOF( ) ) {
		if (SS.m_X_COORD == AFX_RFX_INT_PSEUDO_NULL) SS.m_X_COORD = 0L;
		if (SS.m_Y_COORD == AFX_RFX_INT_PSEUDO_NULL) SS.m_Y_COORD = 0L;
		if (SS.m_HT == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_HT = 0.0;
		if (SS.m_SG == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_SG = 0.0;
		if (SS.m_RW_ASH == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_RW_ASH = 0.0;
		if (SS.m_RW_SUL == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_RW_SUL = 0.0;
		if (SS.m_RW_BTU == AFX_RFX_INT_PSEUDO_NULL) SS.m_RW_BTU = 0;
		if (SS.m_RW_VOL == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_RW_VOL = 0.0;
		if (SS.m_RW_AFT_ID == AFX_RFX_INT_PSEUDO_NULL) SS.m_RW_AFT_ID = 0;
		if (SS.m_RW_AFT_HW == AFX_RFX_INT_PSEUDO_NULL) SS.m_RW_AFT_HW = 0;
		if (SS.m_RW_AFT_2HW == AFX_RFX_INT_PSEUDO_NULL) SS.m_RW_AFT_2HW = 0;
		if (SS.m_RW_AFT_FLD == AFX_RFX_INT_PSEUDO_NULL) SS.m_RW_AFT_FLD = 0;
		if (SS.m_YIELD == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_YIELD = 0.0;
		if (SS.m_CL_ASH == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_CL_ASH = 0.0;
		if (SS.m_CL_SUL == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_CL_SUL = 0.0;
		if (SS.m_CL_BTU == AFX_RFX_INT_PSEUDO_NULL) SS.m_CL_BTU = 0;
		if (SS.m_CL_VOL == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_CL_VOL = 0.0;
		if (SS.m_CL_FSI == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_CL_FSI = 0.0;
		if (SS.m_CL_INERTS == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_CL_INERTS = 0.0;
		if (SS.m_CL_REFLECT == AFX_RFX_SINGLE_PSEUDO_NULL) SS.m_CL_REFLECT = 0.0;
		if (SS.m_CL_FLUID == AFX_RFX_INT_PSEUDO_NULL) SS.m_CL_FLUID = 0;
		if (SS.m_CL_AFT_ID == AFX_RFX_INT_PSEUDO_NULL) SS.m_CL_AFT_ID = 0;
		if (SS.m_CL_AFT_HW == AFX_RFX_INT_PSEUDO_NULL) SS.m_CL_AFT_HW = 0;
		if (SS.m_CL_AFT_2HW == AFX_RFX_INT_PSEUDO_NULL) SS.m_CL_AFT_2HW = 0;
		if (SS.m_CL_AFT_FLD == AFX_RFX_INT_PSEUDO_NULL) SS.m_CL_AFT_FLD = 0;
		quality[0] = SS.m_HT;
		quality[1] = SS.m_SG;
		quality[2] = SS.m_RW_ASH;
		quality[3] = SS.m_RW_SUL;
		quality[4] = SS.m_RW_BTU;
		quality[5] = SS.m_RW_VOL;
		quality[6] = SS.m_RW_AFT_ID;
		quality[7] = SS.m_RW_AFT_HW;
		quality[8] = SS.m_RW_AFT_2HW;
		quality[9] = SS.m_RW_AFT_FLD;
		quality[10] = SS.m_YIELD;
		quality[11] = SS.m_CL_ASH;
		quality[12] = SS.m_CL_SUL;
		quality[13] = SS.m_CL_BTU;
		quality[14] = SS.m_CL_VOL;
		quality[15] = SS.m_CL_FSI;
		quality[16] = SS.m_CL_INERTS;
		quality[17] = SS.m_CL_REFLECT;
		quality[18] = SS.m_CL_FLUID;
		quality[19] = SS.m_CL_AFT_ID;
		quality[20] = SS.m_CL_AFT_HW;
		quality[21] = SS.m_CL_AFT_2HW;
		quality[22] = SS.m_CL_AFT_FLD;
		CSample tmp(LPCTSTR(SS.m_SAMPLE),
				    LPCTSTR(SS.m_TYPE),
				    long(SS.m_X_COORD),
				    long(SS.m_Y_COORD),
				    quality);
		pSample[nCount] = tmp;
		pXCoord[nCount] = long(SS.m_X_COORD);
		pYCoord[nCount] = long(SS.m_Y_COORD);
		pQuality[nCount++] = (SS.m_SG > .001);
		SS.MovePrev();
	}

	SS.Close();
	return nRecords;
}


void CForeDB::LoadAdjustments(char *type, double *pAdj)
{
	AdjustSet AS;

	ASSERT(IsOpen());

	AS.m_pDatabase = this;
	CString str;
	str.Format(_T("[TYPE] = '%s'"),type);
	AS.m_strFilter = str;
    AS.Open();
	if (AS.IsEOF() && AS.IsBOF())
		throw ErrorInfo(CForeDB_NO_RECORDS,str+"Adjustments");

	AS.MoveFirst();
	pAdj[0] = AS.m_DLN_THICK;
	pAdj[1] = AS.m_DLN_SG;
	pAdj[2] = AS.m_DLN_ASH;
	pAdj[3] = AS.m_DLN_SUL;
	pAdj[4] = AS.m_DLN_BTU;
	pAdj[5] = AS.m_RW_PRD_H2O;
	pAdj[6] = AS.m_ASH_ADJ;
	pAdj[7] = AS.m_SUL_ADJ;
	pAdj[8] = AS.m_YLD_ADJ;
	pAdj[9] = AS.m_CL_PRD_H2O;
	pAdj[10] = AS.m_MIN_HEIGHT;
	pAdj[11] = AS.m_PCT_WASHED;
	pAdj[12] = AS.m_RW_DLN_THK;
	pAdj[13] = AS.m_CL_DLN_THK;
    for (int i=0; i<14; i++)
		if (pAdj[i] == AFX_RFX_SINGLE_PSEUDO_NULL) pAdj[i] = 0.0;
	AS.Close();
}


void CForeDB::OpenResults()
{
	ASSERT(IsOpen());

	m_RS.m_pDatabase = this;
	m_RS.m_strSort = "[BEGIN]";
	m_RS.Open();
}


void CForeDB::FirstResult()
{
	m_RS.MoveFirst();
}


void CForeDB::NextResult()
{
	m_RS.MoveNext();
}

int CForeDB::NumberResults()
{
	m_RS.MoveLast();
	return m_RS.GetAbsolutePosition()+1;
}


void CForeDB::CloseResults()
{
	m_RS.Close();
}


void CForeDB::ClearResults()
{
	this->Execute("delete from RESULTS",dbFailOnError);
}


// [NEEDSWORK] Phocaz: WIP
void CForeDB::SaveResults(ResultsList &l_Results, char prd, int prdDays, double pctWashed,
						  double rawMoisture, double clnMoisture)
{
	char *blank="XXXXX";
	//for (CResults *p = l_Results.First(); p!=0; p = l_Results.Next()) 
    for (auto p = l_Results.begin (); p!=l_Results.end (); ++p)

        {
		m_RS.AddNew();
		COleDateTime tmp(p->Date().year(),p->Date().month(),p->Date().day(),0,0,0);
		m_RS.m_BEGIN = tmp;
		m_RS.m_SECT = p->ID();
		m_RS.m_TYPE = p->Type();
		m_RS.m_UNIT = p->Unit();
		if (p->SuperUnit().length())
			m_RS.m_SUPER = p->SuperUnit();
		else
			m_RS.m_SUPER = blank;
		m_RS.m_WORK_DAYS = _32BitCount(p->Days());
		m_RS.m_SHIFTS = p->Shifts();
		m_RS.m_START = p->Start();
		m_RS.m_END = p->End();
		m_RS.m_AREA = p->Area();
		m_RS.m_DAYS = p->Days();
		m_RS.m_LATE = p->Late();
		m_RS.m_LEAD = p->Lead();
		m_RS.m_MAX_USED = p->GovUsed();
		CString strPeriod(prd);
		m_RS.m_PERIOD = strPeriod;
		m_RS.m_PERIOD_DAYS = prdDays;
		m_RS.m_MINING_HT = *(p->Quality() + 0);
		m_RS.m_SG = *(p->Quality() + 1);
		m_RS.m_RW_ASH = *(p->Quality() + 2);
		m_RS.m_RW_SUL = *(p->Quality() + 3);
		m_RS.m_RW_BTU = *(p->Quality() + 4);
		m_RS.m_RW_VOL = *(p->Quality() + 5);
		m_RS.m_RW_AFT_ID = *(p->Quality() + 6);
		m_RS.m_RW_AFT_HW = *(p->Quality() + 7);
		m_RS.m_RW_AFT_2HW = *(p->Quality() + 8);
		m_RS.m_RW_AFT_FLD = *(p->Quality() + 9);
		m_RS.m_YIELD = *(p->Quality() + 10);
		m_RS.m_CL_ASH = *(p->Quality() + 11);
		m_RS.m_CL_SUL = *(p->Quality() + 12);
		m_RS.m_CL_BTU = *(p->Quality() + 13);
		m_RS.m_CL_VOL = *(p->Quality() + 14);
		m_RS.m_CL_FSI = *(p->Quality() + 15);
		m_RS.m_CL_INERTS = *(p->Quality() + 16);
		m_RS.m_CL_REFLECT = *(p->Quality() + 17);
		m_RS.m_CL_FLUID = *(p->Quality() + 18);
		m_RS.m_CL_AFT_ID = *(p->Quality() + 19);
		m_RS.m_CL_AFT_HW = *(p->Quality() + 20);
		m_RS.m_CL_AFT_2HW = *(p->Quality() + 21);
		m_RS.m_CL_AFT_FLD = *(p->Quality() + 22);
		m_RS.m_RAW_TONS = *(p->Quality() + 23);
		m_RS.m_CLEAN_TONS = *(p->Quality() + 24);
		m_RS.m_SEAM_HT = *(p->Quality() + 25);
		m_RS.m_PCT_WASHED = pctWashed;
		m_RS.m_RAW_MOISTURE = rawMoisture;
		m_RS.m_CLEAN_MOISTURE = clnMoisture;
		m_RS.Update();
	}
}


void CForeDB::LoadResultParameters(char &prd, double &pctWashed,
					double &rawMoisture, double &clnMoisture)
{
	m_RS.MoveFirst();
	prd = *LPCTSTR(m_RS.m_PERIOD);
	pctWashed = m_RS.m_PCT_WASHED;
	rawMoisture = m_RS.m_RAW_MOISTURE;
	clnMoisture = m_RS.m_CLEAN_MOISTURE;
}


void CForeDB::LoadResults(CResults &results)
{
	CString strBegin;
	strBegin = m_RS.m_BEGIN.Format("%Y%m%d");
	Date4 Begin(strBegin);
	CResults tmp(LPCTSTR(m_RS.m_SECT),
				 LPCTSTR(m_RS.m_TYPE),
				 m_RS.m_START,
				 m_RS.m_END,
				 LPCTSTR(m_RS.m_UNIT),
				 LPCTSTR(m_RS.m_SUPER),
				 Begin,
				 m_RS.m_DAYS,
				 m_RS.m_LATE,
				 m_RS.m_LEAD,
				 m_RS.m_MAX_USED,
				 m_RS.m_SHIFTS,
				 m_RS.m_AREA,
				 0.0,0.0);
	*(tmp.Quality() + 0) = m_RS.m_MINING_HT;
	*(tmp.Quality() + 1) = m_RS.m_SG;
	*(tmp.Quality() + 2) = m_RS.m_RW_ASH;
	*(tmp.Quality() + 3) = m_RS.m_RW_SUL;
	*(tmp.Quality() + 4) = m_RS.m_RW_BTU;
	*(tmp.Quality() + 5) = m_RS.m_RW_VOL;
	*(tmp.Quality() + 6) = m_RS.m_RW_AFT_ID;
	*(tmp.Quality() + 7) = m_RS.m_RW_AFT_HW;
	*(tmp.Quality() + 8) = m_RS.m_RW_AFT_2HW; 
	*(tmp.Quality() + 9) = m_RS.m_RW_AFT_FLD; 
	*(tmp.Quality() + 10) = m_RS.m_YIELD;
	*(tmp.Quality() + 11) = m_RS.m_CL_ASH;
	*(tmp.Quality() + 12) = m_RS.m_CL_SUL;
	*(tmp.Quality() + 13) = m_RS.m_CL_BTU;
	*(tmp.Quality() + 14) = m_RS.m_CL_VOL;
	*(tmp.Quality() + 15) = m_RS.m_CL_FSI;
	*(tmp.Quality() + 16) = m_RS.m_CL_INERTS;
	*(tmp.Quality() + 17) = m_RS.m_CL_REFLECT; 
	*(tmp.Quality() + 18) = m_RS.m_CL_FLUID;
	*(tmp.Quality() + 19) = m_RS.m_CL_AFT_ID;
	*(tmp.Quality() + 20) = m_RS.m_CL_AFT_HW;
	*(tmp.Quality() + 21) = m_RS.m_CL_AFT_2HW; 
	*(tmp.Quality() + 22) = m_RS.m_CL_AFT_FLD; 
	*(tmp.Quality() + 23) = m_RS.m_RAW_TONS;
	*(tmp.Quality() + 24) = m_RS.m_CLEAN_TONS;
	*(tmp.Quality() + 25) = m_RS.m_SEAM_HT;
	results = tmp;
}

void CForeDB::CreateCalendar()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("CALENDAR");
	daoTblDef.CreateField("CAL_NO",dbInteger,0);
	daoTblDef.CreateField("MON_DATE",dbDate,0);
	daoTblDef.CreateField("MON",dbText,1);
	daoTblDef.CreateField("TUE",dbText,1);
	daoTblDef.CreateField("WED",dbText,1);
	daoTblDef.CreateField("THU",dbText,1);
	daoTblDef.CreateField("FRI",dbText,1);
	daoTblDef.CreateField("SAT",dbText,1);
	daoTblDef.CreateField("SUN",dbText,1);
	daoTblDef.CreateField("COMMENTS",dbText,20);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateRates()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("RATES");
	daoTblDef.CreateField("UNIT",dbText,5);
	daoTblDef.CreateField("TYPE",dbText,5);
	daoTblDef.CreateField("FT_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_FT",dbDouble,0);
	daoTblDef.CreateField("MS_P_DAY",dbDouble,0);
	daoTblDef.CreateField("COAL",dbText,1);
	daoTblDef.CreateField("MAX_TPMS",dbDouble,0);
	daoTblDef.CreateField("MAX_FTPMS",dbDouble,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateGeometry()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("GEOMETRY");
	daoTblDef.CreateField("SECT",dbText,6);
	daoTblDef.CreateField("TYPE",dbText,5);
	daoTblDef.CreateField("INIT_STN",dbDouble,0);
	daoTblDef.CreateField("X_COORD",dbDouble,0);
	daoTblDef.CreateField("Y_COORD",dbDouble,0);
	daoTblDef.CreateField("AZIMUTH",dbDouble,0);
	daoTblDef.CreateField("LENGTH",dbDouble,0);
	daoTblDef.CreateField("WIDTH",dbDouble,0);
	daoTblDef.CreateField("LINEAR_FACTOR",dbDouble,0);
	daoTblDef.CreateField("RECOV",dbDouble,0);
	daoTblDef.CreateField("BARRIER",dbDouble,0);
	daoTblDef.CreateField("PANEL_CO",dbInteger,0);
	daoTblDef.CreateField("PANEL_LV",dbInteger,0);
	daoTblDef.CreateField("PANEL_WT",dbInteger,0);
	daoTblDef.CreateField("TEXT_CO",dbInteger,0);
	daoTblDef.CreateField("TEXT_LV",dbInteger,0);
	daoTblDef.CreateField("TEXT_WT",dbInteger,0);
	daoTblDef.CreateField("TEXT_FONT",dbInteger,0);
	daoTblDef.CreateField("KIND",dbLong,0);
	daoTblDef.CreateField("PREVIOUS",dbText,6);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateSequence()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("SEQUENCE");
	daoTblDef.CreateField("ORDERING",dbLong,0);
	daoTblDef.CreateField("SECT",dbText,6);
	daoTblDef.CreateField("START",dbDouble,0);
	daoTblDef.CreateField("END",dbDouble,0);
	daoTblDef.CreateField("UNIT",dbText,5);
	daoTblDef.CreateField("BEGIN",dbDate,0);
	daoTblDef.CreateField("REF1",dbText,6);
	daoTblDef.CreateField("STN1",dbDouble,0);
	daoTblDef.CreateField("D1",dbInteger,0);
	daoTblDef.CreateField("REF2",dbText,6);
	daoTblDef.CreateField("STN2",dbDouble,0);
	daoTblDef.CreateField("D2",dbInteger,0);
	daoTblDef.CreateField("DIRTY_FLAG",dbBoolean,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateCalExceptions()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("CAL_EXCEPTIONS");
	daoTblDef.CreateField("ID",dbText,6);
	daoTblDef.CreateField("START_MON",dbDate,0);
	daoTblDef.CreateField("END_MON",dbDate,0);
	daoTblDef.CreateField("CAL_NO",dbInteger,0);
	daoTblDef.CreateField("MON",dbText,1);
	daoTblDef.CreateField("TUE",dbText,1);
	daoTblDef.CreateField("WED",dbText,1);
	daoTblDef.CreateField("THU",dbText,1);
	daoTblDef.CreateField("FRI",dbText,1);
	daoTblDef.CreateField("SAT",dbText,1);
	daoTblDef.CreateField("SUN",dbText,1);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateDateExceptions()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("DATE_EXCEPTIONS");
	daoTblDef.CreateField("ID",dbText,6);
	daoTblDef.CreateField("START_DATE",dbDate,0);
	daoTblDef.CreateField("END_DATE",dbDate,0);
	daoTblDef.CreateField("UNIT",dbText,5);
	daoTblDef.CreateField("FT_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_FT",dbDouble,0);
	daoTblDef.CreateField("MS_P_DAY",dbDouble,0);
	daoTblDef.CreateField("INCRS",dbDouble,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateAreaExceptions()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("AREA_EXCEPTIONS");
	daoTblDef.CreateField("ID",dbText,6);
	daoTblDef.CreateField("START_STN",dbDouble,0);
	daoTblDef.CreateField("END_STN",dbDouble,0);
	daoTblDef.CreateField("FT_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_FT",dbDouble,0);
	daoTblDef.CreateField("MS_P_DAY",dbDouble,0);
	daoTblDef.CreateField("INCRS",dbDouble,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateRefExceptions()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("REF_EXCEPTIONS");
	daoTblDef.CreateField("ID",dbText,6);
	daoTblDef.CreateField("REF_ID",dbText,6);
	daoTblDef.CreateField("REF_STN",dbDouble,0);
	daoTblDef.CreateField("UNIT",dbText,5);
	daoTblDef.CreateField("FT_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_MS",dbDouble,0);
	daoTblDef.CreateField("TNS_P_FT",dbDouble,0);
	daoTblDef.CreateField("MS_P_DAY",dbDouble,0);
	daoTblDef.CreateField("INCRS",dbDouble,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateShutdowns()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("SHUTDOWNS");
	daoTblDef.CreateField("ID",dbText,6);
	daoTblDef.CreateField("UNIT",dbText,5);
	daoTblDef.CreateField("START_DATE",dbDate,0);
	daoTblDef.CreateField("END_DATE",dbDate,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateCase()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("CASE");
	daoTblDef.CreateField("CAL_ID",dbText,5);
	daoTblDef.CreateField("NUM_WEEKS",dbInteger,0);
	daoTblDef.CreateField("PERIOD",dbText,7);
	daoTblDef.CreateField("QUALITY_DB",dbText,50);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateGrouping()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("GROUPING");
	daoTblDef.CreateField("FIRST_SECT",dbText,6);
	daoTblDef.CreateField("MAINS",dbText,6);
	daoTblDef.CreateField("ANGLE",dbDouble,0);
	daoTblDef.CreateField("START_DIST",dbDouble,0);
	daoTblDef.CreateField("LW_UNIT",dbText,5);
	daoTblDef.CreateField("LW_MOVEDELAY",dbLong,0);
	daoTblDef.CreateField("LW_SETUPDELAY",dbLong,0);
	daoTblDef.CreateField("LW_BEGIN",dbDate,0);
	daoTblDef.CreateField("CM_UNIT1",dbText,5);
	daoTblDef.CreateField("CM_UNIT2",dbText,5);
	daoTblDef.CreateField("CM_DELAY",dbLong,0);
	daoTblDef.CreateField("CM_BEGIN",dbDate,0);
	daoTblDef.CreateField("JIT_MAINS",dbBoolean,0);
	daoTblDef.CreateField("REVERSED",dbBoolean,0);
	daoTblDef.CreateField("ON_LEFT",dbBoolean,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateResults()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("RESULTS");
	daoTblDef.CreateField("BEGIN",dbDate,0);
	daoTblDef.CreateField("SECT",dbText,6);
	daoTblDef.CreateField("TYPE",dbText,5);
	daoTblDef.CreateField("UNIT",dbText,5);
	daoTblDef.CreateField("SUPER",dbText,5);
	daoTblDef.CreateField("WORK_DAYS",dbInteger,0);
	daoTblDef.CreateField("SHIFTS",dbDouble,0);
	daoTblDef.CreateField("START",dbDouble,0);
	daoTblDef.CreateField("END",dbDouble,0);
	daoTblDef.CreateField("AREA",dbDouble,0);
	daoTblDef.CreateField("DAYS",dbLong,0);
	daoTblDef.CreateField("LATE",dbInteger,0);
	daoTblDef.CreateField("LEAD",dbInteger,0);
	daoTblDef.CreateField("MAX_USED",dbByte,0);
	daoTblDef.CreateField("PERIOD",dbText,1);
	daoTblDef.CreateField("PERIOD_DAYS",dbInteger,0);
	daoTblDef.CreateField("MINING_HT",dbSingle,0);
	daoTblDef.CreateField("SG",dbSingle,0);
	daoTblDef.CreateField("RW_ASH",dbSingle,0);
	daoTblDef.CreateField("RW_SUL",dbSingle,0);
	daoTblDef.CreateField("RW_BTU",dbInteger,0);
	daoTblDef.CreateField("RW_VOL",dbSingle,0);
	daoTblDef.CreateField("RW_AFT_ID",dbInteger,0);
	daoTblDef.CreateField("RW_AFT_HW",dbInteger,0);
	daoTblDef.CreateField("RW_AFT_2HW",dbInteger,0);
	daoTblDef.CreateField("RW_AFT_FLD",dbInteger,0);
	daoTblDef.CreateField("YIELD",dbSingle,0);
	daoTblDef.CreateField("CL_ASH",dbSingle,0);
	daoTblDef.CreateField("CL_SUL",dbSingle,0);
	daoTblDef.CreateField("CL_BTU",dbInteger,0);
	daoTblDef.CreateField("CL_VOL",dbSingle,0);
	daoTblDef.CreateField("CL_FSI",dbSingle,0);
	daoTblDef.CreateField("CL_INERTS",dbSingle,0);
	daoTblDef.CreateField("CL_REFLECT",dbSingle,0);
	daoTblDef.CreateField("CL_FLUID",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_ID",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_HW",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_2HW",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_FLD",dbInteger,0);
	daoTblDef.CreateField("RAW_TONS",dbDouble,0);
	daoTblDef.CreateField("CLEAN_TONS",dbDouble,0);
	daoTblDef.CreateField("SEAM_HT",dbSingle,0);
	daoTblDef.CreateField("PCT_WASHED",dbDouble,0);
	daoTblDef.CreateField("RAW_MOISTURE",dbDouble,0);
	daoTblDef.CreateField("CLEAN_MOISTURE",dbDouble,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateSamples()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("SAMPLES");
	daoTblDef.CreateField("SAMPLE",dbText,8);
	daoTblDef.CreateField("TYPE",dbText,2);
	daoTblDef.CreateField("X_COORD",dbLong,0);
	daoTblDef.CreateField("Y_COORD",dbLong,0);
	daoTblDef.CreateField("HT",dbSingle,0);
	daoTblDef.CreateField("SG",dbSingle,0);
	daoTblDef.CreateField("RW_ASH",dbSingle,0);
	daoTblDef.CreateField("RW_SUL",dbSingle,0);
	daoTblDef.CreateField("RW_BTU",dbInteger,0);
	daoTblDef.CreateField("RW_VOL",dbSingle,0);
	daoTblDef.CreateField("RW_AFT_ID",dbInteger,0);
	daoTblDef.CreateField("RW_AFT_HW",dbInteger,0);
	daoTblDef.CreateField("RW_AFT_2HW",dbInteger,0);
	daoTblDef.CreateField("RW_AFT_FLD",dbInteger,0);
	daoTblDef.CreateField("YIELD",dbSingle,0);
	daoTblDef.CreateField("CL_ASH",dbSingle,0);
	daoTblDef.CreateField("CL_SUL",dbSingle,0);
	daoTblDef.CreateField("CL_BTU",dbInteger,0);
	daoTblDef.CreateField("CL_VOL",dbSingle,0);
	daoTblDef.CreateField("CL_FSI",dbSingle,0);
	daoTblDef.CreateField("CL_INERTS",dbSingle,0);
	daoTblDef.CreateField("CL_REFLECT",dbSingle,0);
	daoTblDef.CreateField("CL_FLUID",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_ID",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_HW",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_2HW",dbInteger,0);
	daoTblDef.CreateField("CL_AFT_FLD",dbInteger,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

void CForeDB::CreateAdjustments()
{
	CDaoTableDef daoTblDef(this);
	daoTblDef.Create("ADJUSTMENTS");
	daoTblDef.CreateField("TYPE",dbText,2);
	daoTblDef.CreateField("DLN_THICK",dbSingle,0);
	daoTblDef.CreateField("DLN_SG",dbSingle,0);
	daoTblDef.CreateField("DLN_ASH",dbSingle,0);
	daoTblDef.CreateField("DLN_SUL",dbSingle,0);
	daoTblDef.CreateField("DLN_BTU",dbInteger,0);
	daoTblDef.CreateField("RW_PRD_H2O",dbSingle,0);
	daoTblDef.CreateField("ASH_ADJ",dbSingle,0);
	daoTblDef.CreateField("SUL_ADJ",dbSingle,0);
	daoTblDef.CreateField("YLD_ADJ",dbSingle,0);
	daoTblDef.CreateField("CL_PRD_H2O",dbSingle,0);
	daoTblDef.CreateField("MIN_HEIGHT",dbSingle,0);
	daoTblDef.CreateField("PCT_WASHED",dbSingle,0);
	daoTblDef.CreateField("RW_DLN_THK",dbSingle,0);
	daoTblDef.CreateField("CL_DLN_THK",dbSingle,0);
	daoTblDef.Append();
	daoTblDef.Close();
}

//*****************************************************************************

// end of CForeDB.CPP
