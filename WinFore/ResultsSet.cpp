// ResultsSet.cpp : implementation file
//

#include "stdafx.h"
#include "winfore.h"
#include "ResultsSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ResultsSet

IMPLEMENT_DYNAMIC(ResultsSet, CDaoRecordset)

ResultsSet::ResultsSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(ResultsSet)
	m_SECT = _T("");
	m_TYPE = _T("");
	m_UNIT = _T("");
	m_SUPER = _T("");
	m_WORK_DAYS = 0;
	m_SHIFTS = 0.0;
	m_START = 0.0;
	m_END = 0.0;
	m_AREA = 0.0;
	m_DAYS = 0;
	m_LATE = 0;
	m_LEAD = 0;
	m_MAX_USED = 0;
	m_PERIOD = _T("");
	m_PERIOD_DAYS = 0;
	m_MINING_HT = 0.0f;
	m_SG = 0.0f;
	m_RW_ASH = 0.0f;
	m_RW_SUL = 0.0f;
	m_RW_BTU = 0;
	m_RW_VOL = 0.0f;
	m_RW_AFT_ID = 0;
	m_RW_AFT_HW = 0;
	m_RW_AFT_2HW = 0;
	m_RW_AFT_FLD = 0;
	m_YIELD = 0.0f;
	m_CL_ASH = 0.0f;
	m_CL_SUL = 0.0f;
	m_CL_BTU = 0;
	m_CL_VOL = 0.0f;
	m_CL_FSI = 0.0f;
	m_CL_INERTS = 0.0f;
	m_CL_REFLECT = 0.0f;
	m_CL_FLUID = 0;
	m_CL_AFT_ID = 0;
	m_CL_AFT_HW = 0;
	m_CL_AFT_2HW = 0;
	m_CL_AFT_FLD = 0;
	m_RAW_TONS = 0.0;
	m_CLEAN_TONS = 0.0;
	m_SEAM_HT = 0.0f;
	m_PCT_WASHED = 0.0;
	m_RAW_MOISTURE = 0.0;
	m_CLEAN_MOISTURE = 0.0;
	m_nFields = 45;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString ResultsSet::GetDefaultDBName()
{
	return _T("C:\\Projects\\WinFore\\fore.mdb");
}

CString ResultsSet::GetDefaultSQL()
{
	return _T("[RESULTS]");
}

void ResultsSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(ResultsSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_DateTime(pFX, _T("[BEGIN]"), m_BEGIN);
	DFX_Text(pFX, _T("[SECT]"), m_SECT);
	DFX_Text(pFX, _T("[TYPE]"), m_TYPE);
	DFX_Text(pFX, _T("[UNIT]"), m_UNIT);
	DFX_Text(pFX, _T("[SUPER]"), m_SUPER);
	DFX_Short(pFX, _T("[WORK_DAYS]"), m_WORK_DAYS);
	DFX_Double(pFX, _T("[SHIFTS]"), m_SHIFTS);
	DFX_Double(pFX, _T("[START]"), m_START);
	DFX_Double(pFX, _T("[END]"), m_END);
	DFX_Double(pFX, _T("[AREA]"), m_AREA);
	DFX_Long(pFX, _T("[DAYS]"), m_DAYS);
	DFX_Short(pFX, _T("[LATE]"), m_LATE);
	DFX_Short(pFX, _T("[LEAD]"), m_LEAD);
	DFX_Byte(pFX, _T("[MAX_USED]"), m_MAX_USED);
	DFX_Text(pFX, _T("[PERIOD]"), m_PERIOD);
	DFX_Short(pFX, _T("[PERIOD_DAYS]"), m_PERIOD_DAYS);
	DFX_Single(pFX, _T("[MINING_HT]"), m_MINING_HT);
	DFX_Single(pFX, _T("[SG]"), m_SG);
	DFX_Single(pFX, _T("[RW_ASH]"), m_RW_ASH);
	DFX_Single(pFX, _T("[RW_SUL]"), m_RW_SUL);
	DFX_Short(pFX, _T("[RW_BTU]"), m_RW_BTU);
	DFX_Single(pFX, _T("[RW_VOL]"), m_RW_VOL);
	DFX_Short(pFX, _T("[RW_AFT_ID]"), m_RW_AFT_ID);
	DFX_Short(pFX, _T("[RW_AFT_HW]"), m_RW_AFT_HW);
	DFX_Short(pFX, _T("[RW_AFT_2HW]"), m_RW_AFT_2HW);
	DFX_Short(pFX, _T("[RW_AFT_FLD]"), m_RW_AFT_FLD);
	DFX_Single(pFX, _T("[YIELD]"), m_YIELD);
	DFX_Single(pFX, _T("[CL_ASH]"), m_CL_ASH);
	DFX_Single(pFX, _T("[CL_SUL]"), m_CL_SUL);
	DFX_Short(pFX, _T("[CL_BTU]"), m_CL_BTU);
	DFX_Single(pFX, _T("[CL_VOL]"), m_CL_VOL);
	DFX_Single(pFX, _T("[CL_FSI]"), m_CL_FSI);
	DFX_Single(pFX, _T("[CL_INERTS]"), m_CL_INERTS);
	DFX_Single(pFX, _T("[CL_REFLECT]"), m_CL_REFLECT);
	DFX_Short(pFX, _T("[CL_FLUID]"), m_CL_FLUID);
	DFX_Short(pFX, _T("[CL_AFT_ID]"), m_CL_AFT_ID);
	DFX_Short(pFX, _T("[CL_AFT_HW]"), m_CL_AFT_HW);
	DFX_Short(pFX, _T("[CL_AFT_2HW]"), m_CL_AFT_2HW);
	DFX_Short(pFX, _T("[CL_AFT_FLD]"), m_CL_AFT_FLD);
	DFX_Double(pFX, _T("[RAW_TONS]"), m_RAW_TONS);
	DFX_Double(pFX, _T("[CLEAN_TONS]"), m_CLEAN_TONS);
	DFX_Single(pFX, _T("[SEAM_HT]"), m_SEAM_HT);
	DFX_Double(pFX, _T("[PCT_WASHED]"), m_PCT_WASHED);
	DFX_Double(pFX, _T("[RAW_MOISTURE]"), m_RAW_MOISTURE);
	DFX_Double(pFX, _T("[CLEAN_MOISTURE]"), m_CLEAN_MOISTURE);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// ResultsSet diagnostics

#ifdef _DEBUG
void ResultsSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void ResultsSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
