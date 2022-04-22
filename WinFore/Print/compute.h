// ComputeDlg.h : header file
//

#include <d:\projects\forelib\CFore.h>
#include "cforedb.h"

/////////////////////////////////////////////////////////////////////////////
// CComputeDlg dialog

class CComputeDlg : public CDialog
{

public:
    int      m_nTimer;
	int		 m_nCal;
	int		 m_nWeeks;
	char     m_cPeriod;
	CForeDB *m_pDB;
	CFore   *m_pFore;
	CString  m_strDBName;
	CString  m_strQuality;

// Construction
public:
	CComputeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComputeDlg)
	enum { IDD = IDD_COMPUTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComputeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComputeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
