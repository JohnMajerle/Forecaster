// ShutSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ShutSet DAO recordset

class ShutSet : public CDaoRecordset
{
public:
	ShutSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(ShutSet)

// Field/Param Data
	//{{AFX_FIELD(ShutSet, CDaoRecordset)
	CString	m_ID;
	CString	m_UNIT;
	COleDateTime	m_START_DATE;
	COleDateTime	m_END_DATE;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ShutSet)
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
