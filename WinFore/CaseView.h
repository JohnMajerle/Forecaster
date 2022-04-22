#if !defined(AFX_CASEVIEW_H__E45CD301_A6EA_11D1_8308_0001FA449CDF__INCLUDED_)
#define AFX_CASEVIEW_H__E45CD301_A6EA_11D1_8308_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CaseView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCaseView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCaseView : public CFormView
{
public: // change from protected
	CCaseView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCaseView)

// Form Data
public:
	//{{AFX_DATA(CCaseView)
	enum { IDD = IDD_CASE_VIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CWinForeDoc* GetDocument();

// Operations
public:
	void AddCase(int nIndex);
	void UpdateCase(int nIndex);
	void EraseList();
	void InitList();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaseView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCaseView();
	CaseSet* m_pSet;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCaseView)
	afx_msg void OnDone();
	afx_msg void OnDestroy();
	afx_msg void OnEditCase();
	afx_msg void OnNewCase();
	afx_msg void OnDeleteCase();
	afx_msg void OnRunCase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WinForeView.cpp
inline CWinForeDoc* CCaseView::GetDocument()
   { return (CWinForeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASEVIEW_H__E45CD301_A6EA_11D1_8308_0001FA449CDF__INCLUDED_)
