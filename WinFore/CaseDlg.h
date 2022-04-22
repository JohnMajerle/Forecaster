#if !defined(AFX_CASEDLG_H__16E4C222_A7A5_11D1_8309_0001FA449CDF__INCLUDED_)
#define AFX_CASEDLG_H__16E4C222_A7A5_11D1_8309_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CaseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCaseDlg dialog

class CCaseDlg : public CDialog
{
// Construction
public:
	CCaseDlg(CWnd* pParent = NULL);   // standard constructor
	CCaseDlg(CaseSet *pSet, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCaseDlg)
	enum { IDD = IDD_DLGCASE };
	CComboBox	m_ctlCalendarList;
	CComboBox	m_ctlPeriodList;
	CaseSet*	m_pSet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnQualityBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASEDLG_H__16E4C222_A7A5_11D1_8309_0001FA449CDF__INCLUDED_)
