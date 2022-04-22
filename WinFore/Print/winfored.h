// WinForeDoc.h : interface of the CWinForeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINFOREDOC_H__7A48B7BB_A324_11D1_8304_0001FA449CDF__INCLUDED_)
#define AFX_WINFOREDOC_H__7A48B7BB_A324_11D1_8304_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CForeDB.h"
#include "VBFore.h"

class CWinForeDoc : public CDocument
{
protected: // create from serialization only
	CWinForeDoc();
	DECLARE_DYNCREATE(CWinForeDoc)

// Attributes
public:
	CForeDB*      m_pDB;
	CDaoDatabase* GetDatabase() { return m_pDB; }

	// an instance of the IDispatch enabled browser
	_VBForeClass m_browse;  

// Operations
public:
	void GetDBName(CString& strDBName);
	void OpenBrowser();
	BOOL OpenTable(CString& strTable);
	BOOL RunQuery(CString& strTable);
	BOOL VCMineOpenDatabase(LPCTSTR lpszPathName, UINT nCount);
	BOOL VCReportOpenResults(LPCTSTR lpszPathName, UINT nCount);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinForeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinForeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
	//{{AFX_MSG(CWinForeDoc)
	afx_msg void OnFileImport();
	//}}AFX_MSG
	void OnUpdateReportRun(CCmdUI* pCmdUI);
	void OnUpdateGraphicsRun(CCmdUI* pCmdUI);
	void OnUpdateFileImport(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINFOREDOC_H__7A48B7BB_A324_11D1_8304_0001FA449CDF__INCLUDED_)
