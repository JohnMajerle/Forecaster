// AdjustSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AdjustSet DAO recordset

class AdjustSet : public CDaoRecordset
{
public:
	AdjustSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(AdjustSet)

// Field/Param Data
	//{{AFX_FIELD(AdjustSet, CDaoRecordset)
	CString	m_TYPE;
	double	m_DLN_THICK;
	double	m_DLN_SG;
	double	m_DLN_ASH;
	double	m_DLN_SUL;
	double	m_DLN_BTU;
	double	m_RW_PRD_H2O;
	double	m_ASH_ADJ;
	double	m_SUL_ADJ;
	double	m_YLD_ADJ;
	double	m_CL_PRD_H2O;
	double	m_MIN_HEIGHT;
	double	m_PCT_WASHED;
	double	m_RW_DLN_THK;
	double	m_CL_DLN_THK;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AdjustSet)
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
