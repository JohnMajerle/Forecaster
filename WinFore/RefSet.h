// RefSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RefSet DAO recordset

class RefSet : public CDaoRecordset
{
public:
	RefSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(RefSet)

// Field/Param Data
	//{{AFX_FIELD(RefSet, CDaoRecordset)
	CString	m_ID;
	CString	m_REF_ID;
	double	m_REF_STN;
	CString	m_UNIT;
	double	m_FT_P_MS;
	double	m_TNS_P_MS;
	double	m_TNS_P_FT;
	double	m_MS_P_DAY;
	double	m_INCRS;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RefSet)
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
