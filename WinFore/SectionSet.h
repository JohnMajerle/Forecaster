#if !defined(AFX_SECTIONSET_H__FA417501_C25A_11D1_8321_0001FA449CDF__INCLUDED_)
#define AFX_SECTIONSET_H__FA417501_C25A_11D1_8321_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SectionSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SectionSet DAO recordset

class SectionSet : public CDaoRecordset
{
public:
	SectionSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(SectionSet)

// Field/Param Data
	//{{AFX_FIELD(SectionSet, CDaoRecordset)
	long	m_ORDERING;
	CString	m_SECT;
	double	m_START;
	double	m_END;
	CString	m_UNIT;
	COleDateTime	m_BEGIN;
	CString	m_REF1;
	double	m_STN1;
	short	m_D1;
	CString	m_REF2;
	double	m_STN2;
	short	m_D2;
	BOOL	m_DIRTY_FLAG;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SectionSet)
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

#endif // !defined(AFX_SECTIONSET_H__FA417501_C25A_11D1_8321_0001FA449CDF__INCLUDED_)
