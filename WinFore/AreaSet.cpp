// AreaSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "AreaSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AreaSet

IMPLEMENT_DYNAMIC(AreaSet, CDaoRecordset)

AreaSet::AreaSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(AreaSet)
	m_ID = _T("");
	m_START_STN = 0.0;
	m_END_STN = 0.0;
	m_FT_P_MS = 0.0;
	m_TNS_P_MS = 0.0;
	m_TNS_P_FT = 0.0;
	m_MS_P_DAY = 0.0;
	m_INCRS = 0.0;
	m_nFields = 8;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString AreaSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\ef_new\\noex\\fore.mdb");
}

CString AreaSet::GetDefaultSQL()
{
	return _T("[AREA_EXCEPTIONS]");
}

void AreaSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(AreaSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[ID]"), m_ID);
	DFX_Double(pFX, _T("[START_STN]"), m_START_STN);
	DFX_Double(pFX, _T("[END_STN]"), m_END_STN);
	DFX_Double(pFX, _T("[FT_P_MS]"), m_FT_P_MS);
	DFX_Double(pFX, _T("[TNS_P_MS]"), m_TNS_P_MS);
	DFX_Double(pFX, _T("[TNS_P_FT]"), m_TNS_P_FT);
	DFX_Double(pFX, _T("[MS_P_DAY]"), m_MS_P_DAY);
	DFX_Double(pFX, _T("[INCRS]"), m_INCRS);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// AreaSet diagnostics

#ifdef _DEBUG
void AreaSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void AreaSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
