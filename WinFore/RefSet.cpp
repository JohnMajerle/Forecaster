// RefSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "RefSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RefSet

IMPLEMENT_DYNAMIC(RefSet, CDaoRecordset)

RefSet::RefSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(RefSet)
	m_ID = _T("");
	m_REF_ID = _T("");
	m_REF_STN = 0.0;
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


CString RefSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\ef_new\\noex\\fore.mdb");
}

CString RefSet::GetDefaultSQL()
{
	return _T("[REF_EXCEPTIONS]");
}

void RefSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(RefSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[ID]"), m_ID);
	DFX_Text(pFX, _T("[REF_ID]"), m_REF_ID);
	DFX_Double(pFX, _T("[REF_STN]"), m_REF_STN);
	DFX_Text(pFX, _T("[UNIT]"), m_UNIT);
	DFX_Double(pFX, _T("[FT_P_MS]"), m_FT_P_MS);
	DFX_Double(pFX, _T("[TNS_P_MS]"), m_TNS_P_MS);
	DFX_Double(pFX, _T("[TNS_P_FT]"), m_TNS_P_FT);
	DFX_Double(pFX, _T("[MS_P_DAY]"), m_MS_P_DAY);
	DFX_Double(pFX, _T("[INCRS]"), m_INCRS);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// RefSet diagnostics

#ifdef _DEBUG
void RefSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void RefSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
