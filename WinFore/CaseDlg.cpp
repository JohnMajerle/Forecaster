// CaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "CaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaseDlg dialog


CCaseDlg::CCaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaseDlg)
	m_pSet = NULL;
	//}}AFX_DATA_INIT
}

CCaseDlg::CCaseDlg(CaseSet *pSet,CWnd* pParent)
	: CDialog(CCaseDlg::IDD, pParent)
{
	m_pSet = pSet;
}


void CCaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaseDlg)
	DDX_Control(pDX, IDC_CALENDAR, m_ctlCalendarList);
	DDX_Control(pDX, IDC_PERIOD, m_ctlPeriodList);
	DDX_FieldCBString(pDX, IDC_CALENDAR, m_pSet->m_CAL_ID, m_pSet);
	DDX_FieldCBString(pDX, IDC_PERIOD, m_pSet->m_PERIOD, m_pSet);
	DDX_FieldText(pDX, IDC_QUALITY, m_pSet->m_QUALITY_DB, m_pSet);
	DDX_FieldText(pDX, IDC_WEEKS, m_pSet->m_NUM_WEEKS, m_pSet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCaseDlg, CDialog)
	//{{AFX_MSG_MAP(CCaseDlg)
	ON_BN_CLICKED(IDC_QUALITY_BROWSE, OnQualityBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaseDlg message handlers

BOOL CCaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctlPeriodList.ResetContent();
	m_ctlPeriodList.AddString(CString("MONTHLY"));
	m_ctlPeriodList.AddString(CString("WEEKLY"));
	m_ctlPeriodList.AddString(CString("DAILY"));
	m_ctlPeriodList.SelectString(-1,LPCSTR(m_pSet->m_PERIOD));

	CDaoRecordset rs(m_pSet->m_pDatabase);
 	try
 	{
 		rs.Open(dbOpenDynaset,"select Distinct CAL_NO from CALENDAR");	
 	}
 	catch(CDaoException* e)
 	{
 		AfxMessageBox(e->
			m_pErrorInfo->m_strDescription);
 		e->Delete();
 		return FALSE;
 	}
    m_ctlCalendarList.ResetContent();
    while(!rs.IsEOF()) {
        COleVariant var = rs.GetFieldValue(0);
        m_ctlCalendarList.AddString(CCrack::strVARIANT(var));
        rs.MoveNext( );
    }
	m_ctlCalendarList.SelectString(-1,LPCSTR(m_pSet->m_CAL_ID));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCaseDlg::OnQualityBrowse() 
{
	CFileDialog dlg(TRUE,"mdb","*.mdb");
	if (dlg.DoModal() == IDOK) {
		HWND hWndCtrl = GetDlgItem(IDC_QUALITY)->m_hWnd;
		AfxSetWindowText(hWndCtrl,dlg.GetPathName());
	}
}

void CCaseDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
