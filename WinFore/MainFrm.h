// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__7A48B7B9_A324_11D1_8304_0001FA449CDF__INCLUDED_)
#define AFX_MAINFRM_H__7A48B7B9_A324_11D1_8304_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

enum eView {FORE = 1, CASE = 2};

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void SwitchToView(eView nView);
	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT pwp);
	void SaveWindowPlacement(LPWINDOWPLACEMENT pwp);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	LRESULT OnGraphicsShutdown(WPARAM wParam, LPARAM lParam);
	LRESULT OnResultsShutdown(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewOptions();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7A48B7B9_A324_11D1_8304_0001FA449CDF__INCLUDED_)
