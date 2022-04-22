// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg property page

IMPLEMENT_DYNCREATE(COptionsDlg, CPropertyPage)

COptionsDlg::COptionsDlg() : CPropertyPage(COptionsDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_bBrowseOnStartup = FALSE;
	m_bGraphicsOnStartup = FALSE;
	m_bOpenOnStartup = FALSE;
	m_bResultsOnStartup = FALSE;
	m_bRunOnStartup = FALSE;
	//}}AFX_DATA_INIT
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Check(pDX, IDC_BBROWSEONSTARTUP, m_bBrowseOnStartup);
	DDX_Check(pDX, IDC_BGRAPHICSONSTARTUP, m_bGraphicsOnStartup);
	DDX_Check(pDX, IDC_BOPENONSTARTUP, m_bOpenOnStartup);
	DDX_Check(pDX, IDC_BRESULTSONSTARTUP, m_bResultsOnStartup);
	DDX_Check(pDX, IDC_BRUNONSTARTUP, m_bRunOnStartup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers
