#if !defined(AFX_RUNDLG_H__E86F3511_B52F_11D1_8317_0001FA449CDF__INCLUDED_)
#define AFX_RUNDLG_H__E86F3511_B52F_11D1_8317_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RunDlg.h : header file
//

#define WM_RUN_GOODBYE	WM_USER + 6

class CaseSet;

/////////////////////////////////////////////////////////////////////////////
// CRunDlg dialog

class CRunDlg : public CDialog
{
// Construction
public:
	CRunDlg(CWnd* pParent = NULL);   // standard constructor
	CRunDlg(CView* pView);   
	BOOL Create();

// Operations
public:
	void AddCase(int nIndex);
	void UpdateCase(int nIndex);
	void EraseList();
	void InitList();
	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT pwp);
	void SaveWindowPlacement(LPWINDOWPLACEMENT pwp);

// Dialog Data
	//{{AFX_DATA(CRunDlg)
	enum { IDD = IDD_DLGRUN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CaseSet* m_pSet;

	// Generated message map functions
	virtual void OnCancel();
	//{{AFX_MSG(CRunDlg)
	virtual void OnOK();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnNewCase();
	afx_msg void OnEditCase();
	afx_msg void OnDeleteCase();
	afx_msg void OnRunCase();
	afx_msg void OnDblclkCaseList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CView* m_pView;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNDLG_H__E86F3511_B52F_11D1_8317_0001FA449CDF__INCLUDED_)
