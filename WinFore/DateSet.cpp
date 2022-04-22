// DateSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "DateSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DateSet

IMPLEMENT_DYNAMIC(DateSet, CDaoRecordset)

DateSet::DateSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(DateSet)
	m_ID = _T("");
	m_UNIT = _T("");
	m_FT_P_MS = 0.0;
	m_TNS_P_MS = 0.0;
	m_TNS_P_FT = 0.0;
	m_MS_P_DAY = 0.0;
	m_INCRS = 0.0;
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString DateSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\fore.mdb");
}

CString DateSet::GetDefaultSQL()
{
	return _T("[DATE_EXCEPTIONS]");
}

void DateSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(DateSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[ID]"), m_ID);
	DFX_DateTime(pFX, _T("[START_DATE]"), m_START_DATE);
	DFX_DateTime(pFX, _T("[END_DATE]"), m_END_DATE);
	DFX_Text(pFX, _T("[UNIT]"), m_UNIT);
	DFX_Double(pFX, _T("[FT_P_MS]"), m_FT_P_MS);
	DFX_Double(pFX, _T("[TNS_P_MS]"), m_TNS_P_MS);
	DFX_Double(pFX, _T("[TNS_P_FT]"), m_TNS_P_FT);
	DFX_Double(pFX, _T("[MS_P_DAY]"), m_MS_P_DAY);
	DFX_Double(pFX, _T("[INCRS]"), m_INCRS);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// DateSet diagnostics

#ifdef _DEBUG
void DateSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void DateSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
