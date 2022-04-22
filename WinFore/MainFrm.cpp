// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WinFore.h"

#include "MainFrm.h"
#include "WinForeDoc.h"
#include "WinForeView.h"
#include "CaseView.h"
#include "Splash.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT WM_SHUTDOWN_VCMINE = RegisterWindowMessage("ShutVCMine");
UINT WM_SHUTDOWN_VCREPORT = RegisterWindowMessage("ShutVCReport");
UINT WM_SHUTDOWN_WINFORE = RegisterWindowMessage("ShutWinFore");
UINT WM_SIZE_WINFORE = RegisterWindowMessage("SizeWinFore");

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_REGISTERED_MESSAGE(WM_SHUTDOWN_VCMINE, OnGraphicsShutdown )
	ON_REGISTERED_MESSAGE(WM_SHUTDOWN_VCREPORT, OnResultsShutdown )
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame operations

void CMainFrame::SwitchToView(eView nView)
{
    CView* pOldActiveView = GetActiveView();
    CView* pNewActiveView = (CView*)GetDlgItem(nView);
    if (pNewActiveView == NULL)	{
	  switch(nView) {
      case FORE:
           pNewActiveView = (CView*)new CWinForeView;
           break;    
      case CASE:
           pNewActiveView = (CView*)new CCaseView;
           break;    
      }
      CCreateContext context;                   
      context.m_pCurrentDoc = pOldActiveView->GetDocument(); 
      pNewActiveView->Create(NULL, NULL, 0L, CFrameWnd::rectDefault,
                            this, nView, &context);	    
      pNewActiveView->OnInitialUpdate();
    }
    SetActiveView(pNewActiveView);                   
    pNewActiveView->ShowWindow(SW_SHOW); 
    pOldActiveView->ShowWindow(SW_HIDE);
    pOldActiveView->SetDlgCtrlID(
        pOldActiveView->GetRuntimeClass() ==
        RUNTIME_CLASS(CWinForeView) ? FORE : CASE);
    pNewActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
    RecalcLayout(); 
	CScrollView* pScrollView = (CScrollView *)pNewActiveView;
	pScrollView->ResizeParentToFit(FALSE);
}

BOOL CMainFrame::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = AfxGetApp()->GetProfileString("Settings","WindowPos");
	if (strBuffer.IsEmpty())
		return FALSE;

	int cRead = _stscanf(strBuffer,"%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
				&pwp->flags,&pwp->showCmd,
				&pwp->ptMinPosition.x,&pwp->ptMinPosition.y,
				&pwp->ptMaxPosition.x,&pwp->ptMaxPosition.y,
				&pwp->rcNormalPosition.left,&pwp->rcNormalPosition.top,
				&pwp->rcNormalPosition.right,&pwp->rcNormalPosition.bottom);
	if (cRead != 10)
		return FALSE;
	return TRUE;
}

void CMainFrame::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer;
	strBuffer.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
				pwp->flags,pwp->showCmd,
				pwp->ptMinPosition.x,pwp->ptMinPosition.y,
				pwp->ptMaxPosition.x,pwp->ptMaxPosition.y,
				pwp->rcNormalPosition.left,pwp->rcNormalPosition.top,
				pwp->rcNormalPosition.right,pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString("Settings","WindowPos",strBuffer);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnDestroy() 
{
	::RemoveProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName);

	CFrameWnd::OnDestroy();
}

void CMainFrame::OnClose() 
{
	CWinForeView* pActiveView = (CWinForeView*)GetActiveView();
	pActiveView->DestroyDialogs();
	if (pActiveView->m_bGraphics == TRUE  ||  pActiveView->m_bResults == TRUE)
		::SendMessage(HWND_BROADCAST,WM_SHUTDOWN_WINFORE,0,0);

	WINDOWPLACEMENT wp;
	if (GetWindowPlacement(&wp)) {
		if (IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		SaveWindowPlacement(&wp);
	}
	SaveBarState("ControlBars\\State");

	CWinForeApp* pApp = (CWinForeApp *)AfxGetApp();
	pApp->WriteProfileInt("Settings", "OpenOnStartup", pApp->m_bOpenOnStartup);
	pApp->WriteProfileInt("Settings", "BrowseOnStartup", pApp->m_bBrowseOnStartup);
	pApp->WriteProfileInt("Settings", "RunOnStartup", pApp->m_bRunOnStartup);
	pApp->WriteProfileInt("Settings", "GraphicsOnStartup", pApp->m_bGraphicsOnStartup);
	pApp->WriteProfileInt("Settings", "ResultsOnStartup", pApp->m_bResultsOnStartup);
	
	CFrameWnd::OnClose();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	WINDOWPLACEMENT wp;
	if (LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);
	return 0;
}

void CMainFrame::OnViewOptions() 
{
	CPropertySheet propSheet(_T("WinFore - Options"));
	COptionsDlg pageOptions;
	propSheet.AddPage(&pageOptions);

	CWinForeApp* pApp = (CWinForeApp *)AfxGetApp();

	pageOptions.m_bOpenOnStartup = pApp->m_bOpenOnStartup;
	pageOptions.m_bBrowseOnStartup = pApp->m_bBrowseOnStartup;
	pageOptions.m_bRunOnStartup = pApp->m_bRunOnStartup;
	pageOptions.m_bGraphicsOnStartup = pApp->m_bGraphicsOnStartup;
	pageOptions.m_bResultsOnStartup = pApp->m_bResultsOnStartup;

	if (propSheet.DoModal() == IDOK) {
		pApp->m_bOpenOnStartup = pageOptions.m_bOpenOnStartup;
		pApp->m_bBrowseOnStartup = pageOptions.m_bBrowseOnStartup;
		pApp->m_bRunOnStartup = pageOptions.m_bRunOnStartup;
		pApp->m_bGraphicsOnStartup = pageOptions.m_bGraphicsOnStartup;
		pApp->m_bResultsOnStartup = pageOptions.m_bResultsOnStartup;
	}
}

LRESULT CMainFrame::OnGraphicsShutdown(WPARAM, LPARAM)
{
	CWinForeView* pActive = (CWinForeView*)GetActiveView();
	pActive->m_bGraphics = FALSE;
	return 0;
}

LRESULT CMainFrame::OnResultsShutdown(WPARAM, LPARAM)
{
	CWinForeView* pActive = (CWinForeView*)GetActiveView();
	pActive->m_bResults = FALSE;
	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED  ||  nType == SIZE_RESTORED) {
		CWinForeView* pActiveView = (CWinForeView*)GetActiveView();
		if (pActiveView == NULL) return;
		if (pActiveView->m_bGraphics == TRUE  ||  pActiveView->m_bResults == TRUE)
			::SendMessage(HWND_BROADCAST,WM_SIZE_WINFORE,nType,0);

		CWinForeDoc *pDoc = pActiveView->GetDocument();
		if (pDoc->m_browse.m_lpDispatch != NULL) {
			short state = (nType == SIZE_MINIMIZED) ? 1 : 0 ;
			pDoc->m_browse.VBForeWindowState(&state);
		}
	}
}
