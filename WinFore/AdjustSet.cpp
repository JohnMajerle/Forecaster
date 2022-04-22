// AdjustSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "AdjustSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AdjustSet

IMPLEMENT_DYNAMIC(AdjustSet, CDaoRecordset)

AdjustSet::AdjustSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(AdjustSet)
	m_TYPE = _T("");
	m_DLN_THICK = 0.0;
	m_DLN_SG = 0.0;
	m_DLN_ASH = 0.0;
	m_DLN_SUL = 0.0;
	m_DLN_BTU = 0.0;
	m_RW_PRD_H2O = 0.0;
	m_ASH_ADJ = 0.0;
	m_SUL_ADJ = 0.0;
	m_YLD_ADJ = 0.0;
	m_CL_PRD_H2O = 0.0;
	m_MIN_HEIGHT = 0.0;
	m_PCT_WASHED = 0.0;
	m_RW_DLN_THK = 0.0;
	m_CL_DLN_THK = 0.0;
	m_nFields = 15;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString AdjustSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\qual.mdb");
}

CString AdjustSet::GetDefaultSQL()
{
	return _T("[ADJUSTMENTS]");
}

void AdjustSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(AdjustSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[TYPE]"), m_TYPE);
	DFX_Double(pFX, _T("[DLN_THICK]"), m_DLN_THICK);
	DFX_Double(pFX, _T("[DLN_SG]"), m_DLN_SG);
	DFX_Double(pFX, _T("[DLN_ASH]"), m_DLN_ASH);
	DFX_Double(pFX, _T("[DLN_SUL]"), m_DLN_SUL);
	DFX_Double(pFX, _T("[DLN_BTU]"), m_DLN_BTU);
	DFX_Double(pFX, _T("[RW_PRD_H2O]"), m_RW_PRD_H2O);
	DFX_Double(pFX, _T("[ASH_ADJ]"), m_ASH_ADJ);
	DFX_Double(pFX, _T("[SUL_ADJ]"), m_SUL_ADJ);
	DFX_Double(pFX, _T("[YLD_ADJ]"), m_YLD_ADJ);
	DFX_Double(pFX, _T("[CL_PRD_H2O]"), m_CL_PRD_H2O);
	DFX_Double(pFX, _T("[MIN_HEIGHT]"), m_MIN_HEIGHT);
	DFX_Double(pFX, _T("[PCT_WASHED]"), m_PCT_WASHED);
	DFX_Double(pFX, _T("[RW_DLN_THK]"), m_RW_DLN_THK);
	DFX_Double(pFX, _T("[CL_DLN_THK]"), m_CL_DLN_THK);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// AdjustSet diagnostics

#ifdef _DEBUG
void AdjustSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void AdjustSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
