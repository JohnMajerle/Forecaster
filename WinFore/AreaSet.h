// AreaSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AreaSet DAO recordset

class AreaSet : public CDaoRecordset
{
public:
	AreaSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(AreaSet)

// Field/Param Data
	//{{AFX_FIELD(AreaSet, CDaoRecordset)
	CString	m_ID;
	double	m_START_STN;
	double	m_END_STN;
	double	m_FT_P_MS;
	double	m_TNS_P_MS;
	double	m_TNS_P_FT;
	double	m_MS_P_DAY;
	double	m_INCRS;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AreaSet)
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
