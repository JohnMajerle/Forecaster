#if !defined(AFX_GEOMETRYSET_H__FA417500_C25A_11D1_8321_0001FA449CDF__INCLUDED_)
#define AFX_GEOMETRYSET_H__FA417500_C25A_11D1_8321_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GeometrySet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GeometrySet DAO recordset

class GeometrySet : public CDaoRecordset
{
public:
	GeometrySet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(GeometrySet)

// Field/Param Data
	//{{AFX_FIELD(GeometrySet, CDaoRecordset)
	CString	m_SECT;
	CString	m_TYPE;
	double	m_INIT_STN;
	double	m_X_COORD;
	double	m_Y_COORD;
	double	m_AZIMUTH;
	double	m_LENGTH;
	double	m_WIDTH;
	double	m_LINEAR_FACTOR;
	double	m_RECOV;
	double	m_BARRIER;
	short	m_PANEL_CO;
	short	m_PANEL_LV;
	short	m_PANEL_WT;
	short	m_TEXT_CO;
	short	m_TEXT_LV;
	short	m_TEXT_WT;
	short	m_TEXT_FONT;
	long	m_KIND;
	CString	m_PREVIOUS;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GeometrySet)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMETRYSET_H__FA417500_C25A_11D1_8321_0001FA449CDF__INCLUDED_)
