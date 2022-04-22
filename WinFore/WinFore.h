// WinFore.h : main header file for the WINFORE application
//

#if !defined(AFX_WINFORE_H__7A48B7B5_A324_11D1_8304_0001FA449CDF__INCLUDED_)
#define AFX_WINFORE_H__7A48B7B5_A324_11D1_8304_0001FA449CDF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Crack.h"			// Type conversion helpers
#include "CaseSet.h"

#define VCMINE_CLASSNAME "WinForeVCMine"
#define VCREPORT_CLASSNAME "WinForeVCReport"
#define ID_VCMINE_OPEN_DATABASE 3L
#define ID_VCREPORT_OPEN_RESULTS 4L

/////////////////////////////////////////////////////////////////////////////
// CWinForeApp:
// See WinFore.cpp for the implementation of this class
//

class CWinForeApp : public CWinApp
{
public:
	CWinForeApp();

	BOOL m_bOpenOnStartup;
	BOOL m_bBrowseOnStartup;
	BOOL m_bRunOnStartup;
	BOOL m_bGraphicsOnStartup;
	BOOL m_bResultsOnStartup;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinForeApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	void UpdateRegWithDocPath(LPCTSTR lpszPathName);
	CString GetDocPathFromReg();
	BOOL CreateFile(LPCTSTR fileName);

	//{{AFX_MSG(CWinForeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

// structure to hold thread information
typedef struct {
	CString strPathName;			// path to executable
	CEvent* pTermThreadEvent;		// terminate thread event
	HWND hWndList;					// window handle to list box
	CCriticalSection* pcsList;		// critical section for list box
} THREADINFO, *FAR LPTHREADINFO;

void DisplayDaoException(CDaoException* e);
void GetLaunchedDir(CString& strLaunched);

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINFORE_H__7A48B7B5_A324_11D1_8304_0001FA449CDF__INCLUDED_)
