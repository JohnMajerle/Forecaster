// GeometrySet.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "GeometrySet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GeometrySet

IMPLEMENT_DYNAMIC(GeometrySet, CDaoRecordset)

GeometrySet::GeometrySet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(GeometrySet)
	m_SECT = _T("");
	m_TYPE = _T("");
	m_INIT_STN = 0.0;
	m_X_COORD = 0.0;
	m_Y_COORD = 0.0;
	m_AZIMUTH = 0.0;
	m_LENGTH = 0.0;
	m_WIDTH = 0.0;
	m_LINEAR_FACTOR = 0.0;
	m_RECOV = 0.0;
	m_BARRIER = 0.0;
	m_PANEL_CO = 0;
	m_PANEL_LV = 0;
	m_PANEL_WT = 0;
	m_TEXT_CO = 0;
	m_TEXT_LV = 0;
	m_TEXT_WT = 0;
	m_TEXT_FONT = 0;
	m_KIND = 0;
	m_PREVIOUS = _T("");
	m_nFields = 20;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString GeometrySet::GetDefaultDBName()
{
	return _T("C:\\Projects\\WinFore\\fore.mdb");
}

CString GeometrySet::GetDefaultSQL()
{
	return _T("[GEOMETRY]");
}

void GeometrySet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(GeometrySet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[SECT]"), m_SECT);
	DFX_Text(pFX, _T("[TYPE]"), m_TYPE);
	DFX_Double(pFX, _T("[INIT_STN]"), m_INIT_STN);
	DFX_Double(pFX, _T("[X_COORD]"), m_X_COORD);
	DFX_Double(pFX, _T("[Y_COORD]"), m_Y_COORD);
	DFX_Double(pFX, _T("[AZIMUTH]"), m_AZIMUTH);
	DFX_Double(pFX, _T("[LENGTH]"), m_LENGTH);
	DFX_Double(pFX, _T("[WIDTH]"), m_WIDTH);
	DFX_Double(pFX, _T("[LINEAR_FACTOR]"), m_LINEAR_FACTOR);
	DFX_Double(pFX, _T("[RECOV]"), m_RECOV);
	DFX_Double(pFX, _T("[BARRIER]"), m_BARRIER);
	DFX_Short(pFX, _T("[PANEL_CO]"), m_PANEL_CO);
	DFX_Short(pFX, _T("[PANEL_LV]"), m_PANEL_LV);
	DFX_Short(pFX, _T("[PANEL_WT]"), m_PANEL_WT);
	DFX_Short(pFX, _T("[TEXT_CO]"), m_TEXT_CO);
	DFX_Short(pFX, _T("[TEXT_LV]"), m_TEXT_LV);
	DFX_Short(pFX, _T("[TEXT_WT]"), m_TEXT_WT);
	DFX_Short(pFX, _T("[TEXT_FONT]"), m_TEXT_FONT);
	DFX_Long(pFX, _T("[KIND]"), m_KIND);
	DFX_Text(pFX, _T("[PREVIOUS]"), m_PREVIOUS);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// GeometrySet diagnostics

#ifdef _DEBUG
void GeometrySet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void GeometrySet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
