// WinFore.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WinFore.h"

#include "MainFrm.h"
#include "WinForeDoc.h"
#include "WinForeView.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinForeApp

BEGIN_MESSAGE_MAP(CWinForeApp, CWinApp)
	//{{AFX_MSG_MAP(CWinForeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinForeApp construction

CWinForeApp::CWinForeApp()
{
	m_bOpenOnStartup = FALSE;
	m_bBrowseOnStartup = FALSE;
	m_bRunOnStartup = FALSE;
	m_bGraphicsOnStartup = FALSE;
	m_bResultsOnStartup = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinForeApp object

CWinForeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinForeApp initialization

BOOL CWinForeApp::InitInstance()
{
	AfxGetModuleState()->m_dwVersion = 0x0601;
	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}

	if(!AfxOleInit()){
		AfxMessageBox(_T("Ole Initialization failed"));
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Create a mutext object. If it already exists, then this is the
	// second instance of the application.
	::CreateMutex(NULL,TRUE,m_pszExeName);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CWnd* pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while (pPrevWnd) {
			if (::GetProp(pPrevWnd->GetSafeHwnd(), m_pszExeName)) {
				if (pPrevWnd->IsIconic())
					pPrevWnd->ShowWindow(SW_RESTORE);
				pPrevWnd->SetForegroundWindow();
				pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
				return FALSE;
			}
			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);
		}
		return FALSE;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey("CONSOL");
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWinForeDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CWinForeView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Default Open
	m_nCmdShow = SW_HIDE;
	CWinApp::OnFileNew();
	CSplashWnd::ShowSplashScreen(m_pMainWnd);

	m_bOpenOnStartup = GetProfileInt("Settings", "OpenOnStartup", 0);
	m_bBrowseOnStartup = GetProfileInt("Settings", "BrowseOnStartup", 0);
	m_bRunOnStartup = GetProfileInt("Settings", "RunOnStartup", 0);
	m_bGraphicsOnStartup = GetProfileInt("Settings", "GraphicsOnStartup", 0);
	m_bResultsOnStartup = GetProfileInt("Settings", "ResultsOnStartup", 0);
	if (m_lpCmdLine[0] == '\0')	{
		if (m_bOpenOnStartup) {
			// Open file name saved in private INI file.
			CString strDocPath = GetDocPathFromReg();
			if (!strDocPath.IsEmpty())
				OpenDocumentFile(strDocPath);
		}
	} else
		OpenDocumentFile(m_lpCmdLine);

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->DragAcceptFiles();
	m_pMainWnd->ShowWindow(SW_SHOW);

	// Associate a 'tag' with the window so it can be located later
	::SetProp(m_pMainWnd->GetSafeHwnd(),m_pszExeName,(HANDLE)1);

	HWND hWnd = ::FindWindow(VCMINE_CLASSNAME,NULL);
	if (hWnd) {	// check if VCMINE already running
		CMainFrame* pMain = (CMainFrame*)m_pMainWnd;
		CWinForeView* pActive = (CWinForeView*)pMain->GetActiveView();
		pActive->m_bGraphics = TRUE;
	}
	hWnd = ::FindWindow(VCREPORT_CLASSNAME,NULL);
	if (hWnd) {	// check if VCREPORT already running
		CMainFrame* pMain = (CMainFrame*)m_pMainWnd;
		CWinForeView* pActive = (CWinForeView*)pMain->GetActiveView();
		pActive->m_bResults = TRUE;
	}
	if (m_bBrowseOnStartup)
		m_pMainWnd->SendMessage(WM_COMMAND,MAKEWPARAM(ID_VIEW_DATA,0));
	if (m_bRunOnStartup)
		m_pMainWnd->SendMessage(WM_COMMAND,MAKEWPARAM(ID_VIEW_RUN,0));
	if (m_bGraphicsOnStartup)
		m_pMainWnd->SendMessage(WM_COMMAND,MAKEWPARAM(ID_VIEW_GRAPHICS,0));
	if (m_bResultsOnStartup)
		m_pMainWnd->SendMessage(WM_COMMAND,MAKEWPARAM(ID_VIEW_RESULTS,0));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWinForeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

static TCHAR BASED_CODE szRegSection[] = _T("Most Recent Forecaster Database");
static TCHAR BASED_CODE szRegEntry[] = _T("DB");

void CWinForeApp::UpdateRegWithDocPath(LPCTSTR lpszPathName)
{
	WriteProfileString(szRegSection, szRegEntry, lpszPathName);
}

CString CWinForeApp::GetDocPathFromReg()
{
	return GetProfileString(szRegSection, szRegEntry, NULL);
}

BOOL CWinForeApp::CreateFile(LPCTSTR fileName) 
{
	CWaitCursor waiter;
	CForeDB m_daoDB;
	try	{
		m_daoDB.Create(fileName);
		m_daoDB.CreateCalendar();
		m_daoDB.CreateRates();
		m_daoDB.CreateGeometry();
		m_daoDB.CreateSequence();
		m_daoDB.CreateCalExceptions();
		m_daoDB.CreateDateExceptions();
		m_daoDB.CreateAreaExceptions();
		m_daoDB.CreateRefExceptions();
		m_daoDB.CreateShutdowns();
		m_daoDB.CreateCase();
		m_daoDB.CreateGrouping();
		m_daoDB.CreateResults();
		m_daoDB.CreateSamples();
		m_daoDB.CreateAdjustments();
		m_daoDB.Close();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Global Routines

void DisplayDaoException(CDaoException* e)
{
	CString strMsg;
	if (e->m_pErrorInfo!=NULL)
	{
		strMsg.Format(
			_T("%s   (%d)\n\n")
			_T("Would you like to see help?"),
			(LPCTSTR)e->m_pErrorInfo->m_strDescription,
			e->m_pErrorInfo->m_lErrorCode);

		if (AfxMessageBox(strMsg, MB_YESNO) == IDYES)
		{
			WinHelp(GetDesktopWindow(),
					e->m_pErrorInfo->m_strHelpFile,
					HELP_CONTEXT,
					e->m_pErrorInfo->m_lHelpContext);
		}
	}
	else
	{
		strMsg.Format(
			_T("ERROR:CDaoException\n\n")
			_T("SCODE_CODE		=%d\n")	
			_T("SCODE_FACILITY	=%d\n")	
			_T("SCODE_SEVERITY	=%d\n")	
			_T("ResultFromScode	=%d\n"),
			SCODE_CODE		(e->m_scode),
			SCODE_FACILITY	(e->m_scode),
			SCODE_SEVERITY	(e->m_scode),
			ResultFromScode (e->m_scode));
		AfxMessageBox(strMsg);
	}
}

void GetLaunchedDir(CString& strLaunched)
{
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];

	::GetModuleFileName(NULL,szFullPath,MAX_PATH);
	_splitpath(szFullPath,szDrive,szDir,NULL,NULL);
	strLaunched.Format(_T("%s%s"),szDrive,szDir);
}

/////////////////////////////////////////////////////////////////////////////
// CWinForeApp commands

int CWinForeApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}

BOOL CWinForeApp::PreTranslateMessage(MSG* pMsg) 
{

	// CG: The following line was added by the Splash Screen component.

	CSplashWnd::PreTranslateAppMessage(pMsg);
	return CWinApp::PreTranslateMessage(pMsg);
}

void CWinForeApp::OnFileNew() 
{
	const char fileDialogFilter[]="Access files (*.mdb)|*.mdb||";
	CFileDialog dlg(FALSE,"mdb","fore.mdb",
		OFN_CREATEPROMPT|OFN_HIDEREADONLY,
		fileDialogFilter);
	dlg.m_ofn.lpstrTitle = "Create New Forecast Database";
	if (dlg.DoModal() == IDOK) {
		if (CreateFile(dlg.GetPathName())) {
			AfxMessageBox("Empty database created.");
			OpenDocumentFile(dlg.GetPathName());
		}
	}
}

void CWinForeApp::OnFileOpen() 
{
	CWinApp::OnFileOpen();
}
