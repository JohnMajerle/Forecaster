// SampleSet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "SampleSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SampleSet

IMPLEMENT_DYNAMIC(SampleSet, CDaoRecordset)

SampleSet::SampleSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(SampleSet)
	m_SAMPLE = _T("");
	m_TYPE = _T("");
	m_X_COORD = 0.0;
	m_Y_COORD = 0.0;
	m_HT = 0.0;
	m_SG = 0.0;
	m_RW_ASH = 0.0;
	m_RW_SUL = 0.0;
	m_RW_BTU = 0.0;
	m_RW_VOL = 0.0;
	m_RW_AFT_ID = 0.0;
	m_RW_AFT_HW = 0.0;
	m_RW_AFT_2HW = 0.0;
	m_RW_AFT_FLD = 0.0;
	m_YIELD = 0.0;
	m_CL_ASH = 0.0;
	m_CL_SUL = 0.0;
	m_CL_BTU = 0.0;
	m_CL_VOL = 0.0;
	m_CL_FSI = 0.0;
	m_CL_INERTS = 0.0;
	m_CL_REFLECT = 0.0;
	m_CL_FLUID = 0.0;
	m_CL_AFT_ID = 0.0;
	m_CL_AFT_HW = 0.0;
	m_CL_AFT_2HW = 0.0;
	m_CL_AFT_FLD = 0.0;
	m_nFields = 27;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString SampleSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\FORE\\qual.mdb");
}

CString SampleSet::GetDefaultSQL()
{
	return _T("[SAMPLES]");
}

void SampleSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(SampleSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[SAMPLE]"), m_SAMPLE);
	DFX_Text(pFX, _T("[TYPE]"), m_TYPE);
	DFX_Double(pFX, _T("[X_COORD]"), m_X_COORD);
	DFX_Double(pFX, _T("[Y_COORD]"), m_Y_COORD);
	DFX_Double(pFX, _T("[HT]"), m_HT);
	DFX_Double(pFX, _T("[SG]"), m_SG);
	DFX_Double(pFX, _T("[RW_ASH]"), m_RW_ASH);
	DFX_Double(pFX, _T("[RW_SUL]"), m_RW_SUL);
	DFX_Double(pFX, _T("[RW_BTU]"), m_RW_BTU);
	DFX_Double(pFX, _T("[RW_VOL]"), m_RW_VOL);
	DFX_Double(pFX, _T("[RW_AFT_ID]"), m_RW_AFT_ID);
	DFX_Double(pFX, _T("[RW_AFT_HW]"), m_RW_AFT_HW);
	DFX_Double(pFX, _T("[RW_AFT_2HW]"), m_RW_AFT_2HW);
	DFX_Double(pFX, _T("[RW_AFT_FLD]"), m_RW_AFT_FLD);
	DFX_Double(pFX, _T("[YIELD]"), m_YIELD);
	DFX_Double(pFX, _T("[CL_ASH]"), m_CL_ASH);
	DFX_Double(pFX, _T("[CL_SUL]"), m_CL_SUL);
	DFX_Double(pFX, _T("[CL_BTU]"), m_CL_BTU);
	DFX_Double(pFX, _T("[CL_VOL]"), m_CL_VOL);
	DFX_Double(pFX, _T("[CL_FSI]"), m_CL_FSI);
	DFX_Double(pFX, _T("[CL_INERTS]"), m_CL_INERTS);
	DFX_Double(pFX, _T("[CL_REFLECT]"), m_CL_REFLECT);
	DFX_Double(pFX, _T("[CL_FLUID]"), m_CL_FLUID);
	DFX_Double(pFX, _T("[CL_AFT_ID]"), m_CL_AFT_ID);
	DFX_Double(pFX, _T("[CL_AFT_HW]"), m_CL_AFT_HW);
	DFX_Double(pFX, _T("[CL_AFT_2HW]"), m_CL_AFT_2HW);
	DFX_Double(pFX, _T("[CL_AFT_FLD]"), m_CL_AFT_FLD);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// SampleSet diagnostics

#ifdef _DEBUG
void SampleSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void SampleSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
