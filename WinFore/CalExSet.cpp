// CalExSet.cpp : implementation file
//

#include "stdafx.h"
#include "..\forelib\basetype.h"
#include "WinFore.h"
#include "CalExSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CalExSet

IMPLEMENT_DYNAMIC(CalExSet, CDaoRecordset)

CalExSet::CalExSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CalExSet)
	m_ID = _T("");
	m_CAL_NO = 0.0;
	m_MON = _T("");
	m_TUE = _T("");
	m_WED = _T("");
	m_THU = _T("");
	m_FRI = _T("");
	m_SAT = _T("");
	m_SUN = _T("");
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CalExSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\fore.mdb");
}

CString CalExSet::GetDefaultSQL()
{
	return _T("[CAL_EXCEPTIONS]");
}

void CalExSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CalExSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[ID]"), m_ID);
	DFX_DateTime(pFX, _T("[START_MON]"), m_START_MON);
	DFX_DateTime(pFX, _T("[END_MON]"), m_END_MON);
	DFX_Double(pFX, _T("[CAL_NO]"), m_CAL_NO);
	DFX_Text(pFX, _T("[MON]"), m_MON);
	DFX_Text(pFX, _T("[TUE]"), m_TUE);
	DFX_Text(pFX, _T("[WED]"), m_WED);
	DFX_Text(pFX, _T("[THU]"), m_THU);
	DFX_Text(pFX, _T("[FRI]"), m_FRI);
	DFX_Text(pFX, _T("[SAT]"), m_SAT);
	DFX_Text(pFX, _T("[SUN]"), m_SUN);
	//}}AFX_FIELD_MAP
}

// [NEEDSWORK] Phocaz: WIP
//PHOCAZ - NOT WORKING
//UChar CalExSet::Days() const
//{
//      UChar days;
//      
//      days = 0;
//	  if (m_MON == 'X') _setBitMask(&days,0);
//	  if (m_TUE == 'X') _setBitMask(&days,1);
//	  if (m_WED == 'X') _setBitMask(&days,2);
//	  if (m_THU == 'X') _setBitMask(&days,3);
//	  if (m_FRI == 'X') _setBitMask(&days,4);
//	  if (m_SAT == 'X') _setBitMask(&days,5);
//	  if (m_SUN == 'X') _setBitMask(&days,6);
//      return days;
//} 

unsigned char CalExSet::Days () const
    {
    unsigned char days = 0;

    if (m_MON=='X') _setBitMask ( &days,0 );
    if (m_TUE=='X') _setBitMask ( &days,1 );
    if (m_WED=='X') _setBitMask ( &days,2 );
    if (m_THU=='X') _setBitMask ( &days,3 );
    if (m_FRI=='X') _setBitMask ( &days,4 );
    if (m_SAT=='X') _setBitMask ( &days,5 );
    if (m_SUN=='X') _setBitMask ( &days,6 );

    return days;
    }

/////////////////////////////////////////////////////////////////////////////
// CalExSet diagnostics

#ifdef _DEBUG
void CalExSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CalExSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
