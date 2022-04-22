// CalSet.cpp : implementation file
//

#include "stdafx.h"
#include "..\forelib\basedef.h"
#include "WinFore.h"
#include "CalSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CalSet

IMPLEMENT_DYNAMIC(CalSet, CDaoRecordset)

CalSet::CalSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CalSet)
	m_CAL_NO = 0;
	m_MON = _T("");
	m_TUE = _T("");
	m_WED = _T("");
	m_THU = _T("");
	m_FRI = _T("");
	m_SAT = _T("");
	m_SUN = _T("");
	m_COMMENTS = _T("");
	m_nFields = 10;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CalSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\fore.mdb");
}

CString CalSet::GetDefaultSQL()
{
	return _T("[CALENDAR]");
}

void CalSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CalSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[CAL_NO]"), m_CAL_NO);
	DFX_DateTime(pFX, _T("[MON_DATE]"), m_MON_DATE);
	DFX_Text(pFX, _T("[MON]"), m_MON);
	DFX_Text(pFX, _T("[TUE]"), m_TUE);
	DFX_Text(pFX, _T("[WED]"), m_WED);
	DFX_Text(pFX, _T("[THU]"), m_THU);
	DFX_Text(pFX, _T("[FRI]"), m_FRI);
	DFX_Text(pFX, _T("[SAT]"), m_SAT);
	DFX_Text(pFX, _T("[SUN]"), m_SUN);
	DFX_Text(pFX, _T("[COMMENTS]"), m_COMMENTS);
	//}}AFX_FIELD_MAP
}

// [NEEDSWORK] Phocaz: WIP
/*UChar CalSet::Days() const
{
      UChar days;
      
      days = 0;
	  if (m_MON == 'X') _setBitMask(&days,0);
	  if (m_TUE == 'X') _setBitMask(&days,1);
	  if (m_WED == 'X') _setBitMask(&days,2);
	  if (m_THU == 'X') _setBitMask(&days,3);
	  if (m_FRI == 'X') _setBitMask(&days,4);
	  if (m_SAT == 'X') _setBitMask(&days,5);
	  if (m_SUN == 'X') _setBitMask(&days,6);
      return days;
}   */      

unsigned char CalSet::Days () const
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
// CalSet diagnostics

#ifdef _DEBUG
void CalSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CalSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
