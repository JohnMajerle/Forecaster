// CalSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CalSet DAO recordset

class CalSet : public CDaoRecordset
{
public:
	CalSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CalSet)

// Field/Param Data
	//{{AFX_FIELD(CalSet, CDaoRecordset)
	long	m_CAL_NO;
	COleDateTime	m_MON_DATE;
	CString	m_MON;
	CString	m_TUE;
	CString	m_WED;
	CString	m_THU;
	CString	m_FRI;
	CString	m_SAT;
	CString	m_SUN;
	CString	m_COMMENTS;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CalSet)
	public:
	virtual CString GetDefaultDBName();		// Default database name
	virtual CString GetDefaultSQL();		// Default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
	unsigned char Days() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};
