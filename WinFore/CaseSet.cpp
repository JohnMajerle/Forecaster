// CaseSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CaseSet

IMPLEMENT_DYNAMIC(CaseSet, CDaoRecordset)

CaseSet::CaseSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CaseSet)
	m_CAL_ID = _T("");
	m_NUM_WEEKS = 0;
	m_PERIOD = _T("");
	m_QUALITY_DB = _T("");
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


void CaseSet::LockCStrings()
{
	m_CAL_ID.LockBuffer();
	m_PERIOD.LockBuffer();
	m_QUALITY_DB.LockBuffer();
	return;
}


CString CaseSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\fore.mdb");
}

CString CaseSet::GetDefaultSQL()
{
	return _T("[CASE]");
}

void CaseSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CaseSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[CAL_ID]"), m_CAL_ID);
	DFX_Long(pFX, _T("[NUM_WEEKS]"), m_NUM_WEEKS);
	DFX_Text(pFX, _T("[PERIOD]"), m_PERIOD);
	DFX_Text(pFX, _T("[QUALITY_DB]"), m_QUALITY_DB);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CaseSet diagnostics

#ifdef _DEBUG
void CaseSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CaseSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
