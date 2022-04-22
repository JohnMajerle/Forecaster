// RateSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "RateSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RateSet

IMPLEMENT_DYNAMIC(RateSet, CDaoRecordset)

RateSet::RateSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(RateSet)
	m_UNIT = _T("");
	m_TYPE = _T("");
	m_FT_P_MS = 0.0;
	m_TNS_P_MS = 0.0;
	m_TNS_P_FT = 0.0;
	m_MS_P_DAY = 0.0;
	m_COAL = _T("");
	m_MAX_TPMS = 0.0;
	m_MAX_FTPMS = 0.0;
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString RateSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\Browse\\VB\\fore.mdb");
}

CString RateSet::GetDefaultSQL()
{
	return _T("[RATES]");
}

void RateSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(RateSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[UNIT]"), m_UNIT);
	DFX_Text(pFX, _T("[TYPE]"), m_TYPE);
	DFX_Double(pFX, _T("[FT_P_MS]"), m_FT_P_MS);
	DFX_Double(pFX, _T("[TNS_P_MS]"), m_TNS_P_MS);
	DFX_Double(pFX, _T("[TNS_P_FT]"), m_TNS_P_FT);
	DFX_Double(pFX, _T("[MS_P_DAY]"), m_MS_P_DAY);
	DFX_Text(pFX, _T("[COAL]"), m_COAL);
	DFX_Double(pFX, _T("[MAX_TPMS]"), m_MAX_TPMS);
	DFX_Double(pFX, _T("[MAX_FTPMS]"), m_MAX_FTPMS);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// RateSet diagnostics

#ifdef _DEBUG
void RateSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void RateSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
