// DateSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DateSet DAO recordset

class DateSet : public CDaoRecordset
{
public:
	DateSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(DateSet)

// Field/Param Data
	//{{AFX_FIELD(DateSet, CDaoRecordset)
	CString	m_ID;
	COleDateTime	m_START_DATE;
	COleDateTime	m_END_DATE;
	CString	m_UNIT;
	double	m_FT_P_MS;
	double	m_TNS_P_MS;
	double	m_TNS_P_FT;
	double	m_MS_P_DAY;
	double	m_INCRS;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DateSet)
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
