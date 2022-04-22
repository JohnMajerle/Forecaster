// CaseSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CaseSet DAO recordset

class CaseSet : public CDaoRecordset
{
public:
	CaseSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CaseSet)

	void LockCStrings();	//necessary because of DAO 3.6

// Field/Param Data
	//{{AFX_FIELD(CaseSet, CDaoRecordset)
	CString	m_CAL_ID;
	long	m_NUM_WEEKS;
	CString	m_PERIOD;
	CString	m_QUALITY_DB;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CaseSet)
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
