// WinForeView.cpp : implementation of the CWinForeView class
//

#include "stdafx.h"
#include "WinFore.h"

#include "WinForeDoc.h"
#include "WinForeView.h"
#include "DataDlg.h"
#include "RunDlg.h"
#include "MainFrm.h"
#include "Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT WM_SETFOCUS_VCMINE = RegisterWindowMessage("SetFocusVCMine");
UINT WM_SETFOCUS_VCREPORT = RegisterWindowMessage("SetFocusVCReport");

/////////////////////////////////////////////////////////////////////////////
// CWinForeView

IMPLEMENT_DYNCREATE(CWinForeView, CFormView)

BEGIN_MESSAGE_MAP(CWinForeView, CFormView)
	ON_MESSAGE(WM_DATA_GOODBYE,OnDataGoodbye)
	ON_MESSAGE(WM_RUN_GOODBYE,OnRunGoodbye)
	//{{AFX_MSG_MAP(CWinForeView)
	ON_COMMAND(ID_VIEW_DATA, OnViewData)
	ON_COMMAND(ID_VIEW_RUN, OnViewRun)
	ON_COMMAND(ID_VIEW_GRAPHICS, OnViewGraphics)
	ON_COMMAND(ID_VIEW_RESULTS, OnViewResults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinForeView construction/destruction

CWinForeView::CWinForeView()
	: CFormView(CWinForeView::IDD)
{
	//{{AFX_DATA_INIT(CWinForeView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_bGraphics = FALSE;
	m_bResults = FALSE;
	m_pDataDlg = new CDataDlg(this);
	m_pRunDlg = new CRunDlg(this);
}

CWinForeView::~CWinForeView()
{
	delete m_pDataDlg;
	delete m_pRunDlg;
}

void CWinForeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinForeView)
	//}}AFX_DATA_MAP
}

BOOL CWinForeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWinForeView diagnostics

#ifdef _DEBUG
void CWinForeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWinForeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWinForeDoc* CWinForeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinForeDoc)));
	return (CWinForeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinForeView message handlers

LONG CWinForeView::OnDataGoodbye(UINT wParam, LONG lParam) 
{
	m_pDataDlg->DestroyWindow();
	return 0L;
}

LONG CWinForeView::OnRunGoodbye(UINT wParam, LONG lParam) 
{
	m_pRunDlg->DestroyWindow();
	return 0L;
}

BEGIN_EVENTSINK_MAP(CWinForeView, CFormView)
    //{{AFX_EVENTSINK_MAP(CWinForeView)
	ON_EVENT(CWinForeView, IDC_NEW_BUTTON, -600 /* Click */, OnClickNewButton, VTS_NONE)
	ON_EVENT(CWinForeView, IDC_OPEN_BUTTON, -600 /* Click */, OnClickOpenButton, VTS_NONE)
	ON_EVENT(CWinForeView, IDC_QUERY_BUTTON, -600 /* Click */, OnClickQueryButton, VTS_NONE)
	ON_EVENT(CWinForeView, IDC_RUN_BUTTON, -600 /* Click */, OnClickRunButton, VTS_NONE)
	ON_EVENT(CWinForeView, IDC_GRAPHICS_BUTTON, -600 /* Click */, OnClickGraphicsButton, VTS_NONE)
	ON_EVENT(CWinForeView, IDC_RESULTS_BUTTON, -600 /* Click */, OnClickResultsButton, VTS_NONE)
	ON_EVENT(CWinForeView, IDC_DATA_BUTTON, -600 /* Click */, OnClickDataButton, VTS_NONE)
	ON_EVENT(CWinForeView, IDC_EDIT_BUTTON, -600 /* Click */, OnClickEditButton, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CWinForeView::OnClickNewButton() 
{
	CWinForeApp* pApp = (CWinForeApp *)AfxGetApp();
	pApp->OnFileNew();
}

void CWinForeView::OnClickOpenButton() 
{
	CWinForeApp* pApp = (CWinForeApp *)AfxGetApp();
	pApp->OnFileOpen();
}

void CWinForeView::OnClickDataButton() 
{
	if (!GetDocument()->GetDatabase()) {
		AfxMessageBox("No database open.");
		return;
	}
	m_pDataDlg->m_bTable = TRUE;
	if (m_pDataDlg->GetSafeHwnd() == 0)
		m_pDataDlg->Create();
	else {
		m_pDataDlg->EraseList();
		m_pDataDlg->InitList();
		if (m_pDataDlg->IsIconic())
			m_pDataDlg->ShowWindow(SW_RESTORE);
		m_pDataDlg->SetForegroundWindow();
	}
}

void CWinForeView::OnClickQueryButton() 
{
	if (!GetDocument()->GetDatabase()) {
		AfxMessageBox("No database open.");
		return;
	}
	m_pDataDlg->m_bTable = FALSE;
	if (m_pDataDlg->GetSafeHwnd() == 0)
		m_pDataDlg->Create();
	else {
		m_pDataDlg->EraseList();
		m_pDataDlg->InitList();
		if (m_pDataDlg->IsIconic())
			m_pDataDlg->ShowWindow(SW_RESTORE);
		m_pDataDlg->SetForegroundWindow();
	}
}

void CWinForeView::OnClickRunButton() 
{
	if (!GetDocument()->GetDatabase()) {
		AfxMessageBox("No database open.");
		return;
	}
	if (m_pRunDlg->GetSafeHwnd() == 0)
		m_pRunDlg->Create();
	else {
		m_pRunDlg->EraseList();
		m_pRunDlg->InitList();
		if (m_pRunDlg->IsIconic())
			m_pRunDlg->ShowWindow(SW_RESTORE);
		m_pRunDlg->SetForegroundWindow();
	}
}

void CWinForeView::OnClickGraphicsButton() 
{
	if (!GetDocument()->GetDatabase()) {
		AfxMessageBox("No database open.");
		return;
	}

	if (!m_bGraphics) {
		PROCESS_INFORMATION stProcessInfo;
#ifdef _DEBUG
		CString cmdLine = "d:\\projects\\vcmine\\debug\\vcmine " +
			GetDocument()->GetDatabase()->GetName();
#else
		CString cmdLine = "vcmine " + GetDocument()->GetDatabase()->GetName();
#endif
		m_bGraphics = LaunchApplication(cmdLine,&stProcessInfo);
	} else
		::SendMessage(HWND_BROADCAST,WM_SETFOCUS_VCMINE,0,0);
}

void CWinForeView::OnClickResultsButton() 
{
	if (!GetDocument()->GetDatabase()) {
		AfxMessageBox("No database open.");
		return;
	}

	OnClickGraphicsButton();
	if (!m_bResults) {
		PROCESS_INFORMATION stProcessInfo;
#ifdef _DEBUG
		CString cmdLine = "d:\\projects\\vcreport\\debug\\vcreport " + 
			GetDocument()->GetDatabase()->GetName();
#else
		CString cmdLine = "vcreport " + GetDocument()->GetDatabase()->GetName();
#endif
		m_bResults = LaunchApplication(cmdLine,&stProcessInfo);
	} else
		::SendMessage(HWND_BROADCAST,WM_SETFOCUS_VCREPORT,0,0);
}

void CWinForeView::OnInitialUpdate() 
{
    CFrameWnd * pFrame;
    CRect rectFrame, rectView;

	CFormView::OnInitialUpdate();

    // Resize parent to fit dialog template exactly    
    // while not resizing beyond size of screen
    VERIFY( pFrame = GetParentFrame() );
    pFrame->GetClientRect( rectFrame );
    GetClientRect( rectView );
    if ( rectFrame.Width() < rectView.Width()
         || rectFrame.Height() < rectView.Height() )
    {        
        // Resize so can be refit to any template that fits the screen
        pFrame->MoveWindow( 0, 0, GetSystemMetrics(SM_CXSCREEN), 
                        GetSystemMetrics(SM_CYSCREEN), FALSE ); 
    }
    pFrame->RecalcLayout();
    ResizeParentToFit(TRUE);    // Shrink to fit template

	if (m_pDataDlg->GetSafeHwnd() != 0) {
		m_pDataDlg->EraseList();
		m_pDataDlg->InitList();
	}
	if (m_pRunDlg->GetSafeHwnd() != 0) {
		m_pRunDlg->EraseList();
		m_pRunDlg->InitList();
	}
}

void CWinForeView::DestroyDialogs() 
{
	if (m_pDataDlg->GetSafeHwnd() != 0)
		m_pDataDlg->DestroyWindow();
	if (m_pRunDlg->GetSafeHwnd() != 0)
		m_pRunDlg->DestroyWindow();
}

void CWinForeView::OnViewData()
{
	OnClickDataButton();
}

void CWinForeView::OnViewRun() 
{
	OnClickRunButton();
}

void CWinForeView::OnViewGraphics() 
{
	OnClickGraphicsButton();
}

void CWinForeView::OnViewResults() 
{
	OnClickResultsButton();
}

void CWinForeView::OnClickEditButton() 
{
	if (GetDocument()->m_browse.m_lpDispatch != NULL) {
		short state = 0;	// restore
		GetDocument()->m_browse.VBForeWindowState(&state);
		BOOL visible = TRUE;
		GetDocument()->m_browse.VBForeWindowVisible(&visible);
		GetDocument()->m_browse.VBForeWindowFocus();
	}
}
