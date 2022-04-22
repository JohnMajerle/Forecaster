// SampleSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SampleSet DAO recordset

class SampleSet : public CDaoRecordset
{
public:
	SampleSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(SampleSet)

// Field/Param Data
	//{{AFX_FIELD(SampleSet, CDaoRecordset)
	CString	m_SAMPLE;
	CString	m_TYPE;
	double	m_X_COORD;
	double	m_Y_COORD;
	double	m_HT;
	double	m_SG;
	double	m_RW_ASH;
	double	m_RW_SUL;
	double	m_RW_BTU;
	double	m_RW_VOL;
	double	m_RW_AFT_ID;
	double	m_RW_AFT_HW;
	double	m_RW_AFT_2HW;
	double	m_RW_AFT_FLD;
	double	m_YIELD;
	double	m_CL_ASH;
	double	m_CL_SUL;
	double	m_CL_BTU;
	double	m_CL_VOL;
	double	m_CL_FSI;
	double	m_CL_INERTS;
	double	m_CL_REFLECT;
	double	m_CL_FLUID;
	double	m_CL_AFT_ID;
	double	m_CL_AFT_HW;
	double	m_CL_AFT_2HW;
	double	m_CL_AFT_FLD;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SampleSet)
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
