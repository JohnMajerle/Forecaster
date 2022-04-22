// WinForeView.h : interface of the CWinForeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINFOREVIEW_H__7A48B7BD_A324_11D1_8304_0001FA449CDF__INCLUDED_)
#define AFX_WINFOREVIEW_H__7A48B7BD_A324_11D1_8304_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDataDlg;
class CRunDlg;

class CWinForeView : public CFormView
{
public: // change from protected
	CWinForeView();
	DECLARE_DYNCREATE(CWinForeView)

public:
	//{{AFX_DATA(CWinForeView)
	enum { IDD = IDD_WINFORE_FORM };
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CWinForeDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinForeView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinForeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL m_bGraphics;
	BOOL m_bResults;
	CDataDlg* m_pDataDlg;
	CRunDlg* m_pRunDlg;
	void DestroyDialogs();

// Generated message map functions
protected:
	LONG OnDataGoodbye(UINT wParam, LONG lParam) ;
	LONG OnRunGoodbye(UINT wParam, LONG lParam) ;
	//{{AFX_MSG(CWinForeView)
	afx_msg void OnClickNewButton();
	afx_msg void OnClickOpenButton();
	afx_msg void OnClickQueryButton();
	afx_msg void OnClickRunButton();
	afx_msg void OnClickGraphicsButton();
	afx_msg void OnClickResultsButton();
	afx_msg void OnClickDataButton();
	afx_msg void OnViewData();
	afx_msg void OnViewRun();
	afx_msg void OnViewGraphics();
	afx_msg void OnViewResults();
	afx_msg void OnClickEditButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WinForeView.cpp
inline CWinForeDoc* CWinForeView::GetDocument()
   { return (CWinForeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINFOREVIEW_H__7A48B7BD_A324_11D1_8304_0001FA449CDF__INCLUDED_)
