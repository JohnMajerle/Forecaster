// DlgSQL.h : header file
//

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

class CWinForeDoc;

/////////////////////////////////////////////////////////////////////////////
// CDlgSQL dialog

class CDlgSQL : public CDialog
{
// Construction
public:
	CDlgSQL(CWnd* pParent = NULL);   // standard constructor
	CDlgSQL(CWinForeDoc* pDoc, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgSQL)
	enum { IDD = IDD_DLGSQL };
	CString	m_strName;
	CString	m_strSQL;
	//}}AFX_DATA
	BOOL m_bEditMode;
	CWinForeDoc* m_pDoc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSQL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSQL)
	virtual BOOL OnInitDialog();
	afx_msg void OnSqlQuery();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
