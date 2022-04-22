#if !defined(AFX_RESULTSSET_H__D16A9A20_4BD6_11D2_8394_0001FA449CDF__INCLUDED_)
#define AFX_RESULTSSET_H__D16A9A20_4BD6_11D2_8394_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ResultsSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ResultsSet DAO recordset

class ResultsSet : public CDaoRecordset
{
public:
	ResultsSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(ResultsSet)

// Field/Param Data
	//{{AFX_FIELD(ResultsSet, CDaoRecordset)
	COleDateTime	m_BEGIN;
	CString	m_SECT;
	CString	m_TYPE;
	CString	m_UNIT;
	CString	m_SUPER;
	short	m_WORK_DAYS;
	double	m_SHIFTS;
	double	m_START;
	double	m_END;
	double	m_AREA;
	long	m_DAYS;
	short	m_LATE;
	short	m_LEAD;
	BYTE	m_MAX_USED;
	CString	m_PERIOD;
	short	m_PERIOD_DAYS;
	float	m_MINING_HT;
	float	m_SG;
	float	m_RW_ASH;
	float	m_RW_SUL;
	short	m_RW_BTU;
	float	m_RW_VOL;
	short	m_RW_AFT_ID;
	short	m_RW_AFT_HW;
	short	m_RW_AFT_2HW;
	short	m_RW_AFT_FLD;
	float	m_YIELD;
	float	m_CL_ASH;
	float	m_CL_SUL;
	short	m_CL_BTU;
	float	m_CL_VOL;
	float	m_CL_FSI;
	float	m_CL_INERTS;
	float	m_CL_REFLECT;
	short	m_CL_FLUID;
	short	m_CL_AFT_ID;
	short	m_CL_AFT_HW;
	short	m_CL_AFT_2HW;
	short	m_CL_AFT_FLD;
	double	m_RAW_TONS;
	double	m_CLEAN_TONS;
	float	m_SEAM_HT;
	double	m_PCT_WASHED;
	double	m_RAW_MOISTURE;
	double	m_CLEAN_MOISTURE;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResultsSet)
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

#endif // !defined(AFX_RESULTSSET_H__D16A9A20_4BD6_11D2_8394_0001FA449CDF__INCLUDED_)
