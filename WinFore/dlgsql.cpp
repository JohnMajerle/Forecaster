// DlgSQL.cpp : implementation file
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

#include "stdafx.h"
#include "WinFore.h"
#include "WinForeDoc.h"
#include "DlgSQL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSQL dialog


CDlgSQL::CDlgSQL(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSQL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSQL)
	m_strName = _T("");
	m_strSQL = _T("");
	//}}AFX_DATA_INIT
	m_bEditMode = FALSE;
	m_pDoc = NULL;
}


CDlgSQL::CDlgSQL(CWinForeDoc* pDoc, CWnd* pParent)
	: CDialog(CDlgSQL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSQL)
	m_strName = _T("");
	m_strSQL = _T("");
	//}}AFX_DATA_INIT
	m_bEditMode = FALSE;
	m_pDoc = pDoc;
}

void CDlgSQL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSQL)
	DDX_Text(pDX, IDC_EDITNAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 45);
	DDX_Text(pDX, IDC_EDITSQL, m_strSQL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSQL, CDialog)
	//{{AFX_MSG_MAP(CDlgSQL)
	ON_BN_CLICKED(IDC_SQL_QUERY, OnSqlQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSQL message handlers

BOOL CDlgSQL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_bEditMode)
	{
		((CEdit *) GetDlgItem(IDC_EDITNAME))->SetReadOnly();
		((CEdit *) GetDlgItem(IDC_EDITSQL))->SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSQL::OnSqlQuery() 
{
	CString strQuery;
	short state = 0;
	m_pDoc->m_browse.VBForeWindowState(&state);
	strQuery = m_pDoc->m_browse.VBForeQuery();
	if (strQuery.GetLength()) {
		UpdateData(TRUE);
		m_strSQL = strQuery;
		UpdateData(FALSE);
	}
}
