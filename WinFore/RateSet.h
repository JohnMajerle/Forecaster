// RateSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RateSet DAO recordset

class RateSet : public CDaoRecordset
{
public:
	RateSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(RateSet)

// Field/Param Data
	//{{AFX_FIELD(RateSet, CDaoRecordset)
	CString	m_UNIT;
	CString	m_TYPE;
	double	m_FT_P_MS;
	double	m_TNS_P_MS;
	double	m_TNS_P_FT;
	double	m_MS_P_DAY;
	CString	m_COAL;
	double	m_MAX_TPMS;
	double	m_MAX_FTPMS;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RateSet)
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
