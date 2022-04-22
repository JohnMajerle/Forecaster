// ShutSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "ShutSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ShutSet

IMPLEMENT_DYNAMIC(ShutSet, CDaoRecordset)

ShutSet::ShutSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(ShutSet)
	m_ID = _T("");
	m_UNIT = _T("");
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString ShutSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\ef_new\\noex\\fore.mdb");
}

CString ShutSet::GetDefaultSQL()
{
	return _T("[SHUTDOWNS]");
}

void ShutSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(ShutSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[ID]"), m_ID);
	DFX_Text(pFX, _T("[UNIT]"), m_UNIT);
	DFX_DateTime(pFX, _T("[START_DATE]"), m_START_DATE);
	DFX_DateTime(pFX, _T("[END_DATE]"), m_END_DATE);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// ShutSet diagnostics

#ifdef _DEBUG
void ShutSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void ShutSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
