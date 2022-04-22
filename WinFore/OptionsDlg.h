#if !defined(AFX_OPTIONSDLG_H__109345D0_B76F_11D1_8318_0001FA449CDF__INCLUDED_)
#define AFX_OPTIONSDLG_H__109345D0_B76F_11D1_8318_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsDlg)

// Construction
public:
	COptionsDlg();
	~COptionsDlg();

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_DLGOPTIONS };
	BOOL	m_bBrowseOnStartup;
	BOOL	m_bGraphicsOnStartup;
	BOOL	m_bOpenOnStartup;
	BOOL	m_bResultsOnStartup;
	BOOL	m_bRunOnStartup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__109345D0_B76F_11D1_8318_0001FA449CDF__INCLUDED_)
