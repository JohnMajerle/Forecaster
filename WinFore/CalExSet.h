// CalExSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CalExSet DAO recordset

class CalExSet : public CDaoRecordset
{
public:
	CalExSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CalExSet)

// Field/Param Data
	//{{AFX_FIELD(CalExSet, CDaoRecordset)
	CString	m_ID;
	COleDateTime	m_START_MON;
	COleDateTime	m_END_MON;
	double	m_CAL_NO;
	CString	m_MON;
	CString	m_TUE;
	CString	m_WED;
	CString	m_THU;
	CString	m_FRI;
	CString	m_SAT;
	CString	m_SUN;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CalExSet)
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
