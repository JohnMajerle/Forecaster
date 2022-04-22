// SectionSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "SectionSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SectionSet

IMPLEMENT_DYNAMIC(SectionSet, CDaoRecordset)

SectionSet::SectionSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(SectionSet)
	m_ORDERING = 0;
	m_SECT = _T("");
	m_START = 0.0;
	m_END = 0.0;
	m_UNIT = _T("");
	m_REF1 = _T("");
	m_STN1 = 0.0;
	m_D1 = 0;
	m_REF2 = _T("");
	m_STN2 = 0.0;
	m_D2 = 0;
	m_DIRTY_FLAG = FALSE;
	m_nFields = 13;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString SectionSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\WinFore\\fore.mdb");
}

CString SectionSet::GetDefaultSQL()
{
	return _T("[SEQUENCE]");
}

void SectionSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(SectionSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[ORDERING]"), m_ORDERING);
	DFX_Text(pFX, _T("[SECT]"), m_SECT);
	DFX_Double(pFX, _T("[START]"), m_START);
	DFX_Double(pFX, _T("[END]"), m_END);
	DFX_Text(pFX, _T("[UNIT]"), m_UNIT);
	DFX_DateTime(pFX, _T("[BEGIN]"), m_BEGIN);
	DFX_Text(pFX, _T("[REF1]"), m_REF1);
	DFX_Double(pFX, _T("[STN1]"), m_STN1);
	DFX_Short(pFX, _T("[D1]"), m_D1);
	DFX_Text(pFX, _T("[REF2]"), m_REF2);
	DFX_Double(pFX, _T("[STN2]"), m_STN2);
	DFX_Short(pFX, _T("[D2]"), m_D2);
	DFX_Bool(pFX, _T("[DIRTY_FLAG]"), m_DIRTY_FLAG);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// SectionSet diagnostics

#ifdef _DEBUG
void SectionSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void SectionSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
