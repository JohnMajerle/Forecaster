#if !defined(AFX_DATADLG_H__1CA50AE1_B435_11D1_8316_0001FA449CDF__INCLUDED_)
#define AFX_DATADLG_H__1CA50AE1_B435_11D1_8316_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DataDlg.h : header file
//

#define WM_DATA_GOODBYE		WM_USER + 5

/////////////////////////////////////////////////////////////////////////////
// CDataDlg dialog

class CDataDlg : public CDialog
{
// Construction
public:
	CDataDlg(CWnd* pParent = NULL);   // standard constructor
	CDataDlg(CView* pView);   
	BOOL Create();
	void EraseList();
	void InitList();
	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT pwp);
	void SaveWindowPlacement(LPWINDOWPLACEMENT pwp);

// Attributes
public:
	BOOL m_bTable;
	CString m_strItemName;

// Dialog Data
	//{{AFX_DATA(CDataDlg)
	enum { IDD = IDD_DLGDATA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnCopy();
	void OnDelete();
	void OnEditQuery();
	void OnNewQuery();
	BOOL CopyQuery(CDaoDatabase*,CDaoDatabase*);
	void RenameQuery();
	void RemoveRecords();

	// Generated message map functions
	virtual void OnCancel();
	void DoPopupMenu(UINT nMenuID);
	afx_msg void OnPopupCommand(UINT nMenuID);
	//{{AFX_MSG(CDataDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnOpen();
	afx_msg void OnRclickTablelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownTablelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTablelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CView* m_pView;
	CImageList m_SmallImageList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADLG_H__1CA50AE1_B435_11D1_8316_0001FA449CDF__INCLUDED_)
