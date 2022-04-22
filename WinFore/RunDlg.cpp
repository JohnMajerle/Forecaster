// RunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "WinForeDoc.h"
#include "RunDlg.h"
#include "CaseDlg.h"
#include "ComputeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunDlg dialog


CRunDlg::CRunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pView = NULL;
	m_pSet = NULL;
}

CRunDlg::CRunDlg(CView* pView)
{
	m_pView = pView;
	m_pSet = NULL;
}

BOOL CRunDlg::Create()
{
	if (!CDialog::Create(CRunDlg::IDD))
		return 0;

	WINDOWPLACEMENT wp;
	if (LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);
	return 1;
}

void CRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CRunDlg operations

void CRunDlg::AddCase(int nIndex)
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	COleVariant var;
	COleVariant* pMarker = new COleVariant(m_pSet->GetBookmark());
	for (int i=0; i < m_pSet->m_nFields; i++) {
		var = m_pSet->GetFieldValue(i);
		if (i == 0)
			pList->AddItem(nIndex,i,CCrack::strVARIANT(var),-1,(LPARAM)pMarker);
		else
			pList->AddItem(nIndex,i,CCrack::strVARIANT(var));
	}
}

void CRunDlg::UpdateCase(int nIndex)
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	COleVariant var;
	for (int i=0; i < m_pSet->m_nFields; i++) {
		var = m_pSet->GetFieldValue(i);
		pList->UpdateItem(nIndex,i,CCrack::strVARIANT(var));
	}
}

void CRunDlg::EraseList()
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	int nCount = pList->GetItemCount();
	COleVariant* pMarker;
	for (int nIndex=0; nIndex < nCount; nIndex++) {
		pMarker = (COleVariant*)pList->GetParam(nIndex);
		delete pMarker;
		pList->SetParam(nIndex,NULL);
	}
	pList->DeleteAllItems();
	while(pList->DeleteColumn(0));
	UpdateWindow();
}

void CRunDlg::InitList()
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);

	// get size of control to help set the column widths
	CRect controlRect;
	pList->GetClientRect(controlRect);
	int controlWidth = controlRect.Width();
	//
	// set up columns
	int strWidth = pList->GetStringWidth(_T("Calendar")) + 15;
	pList->InsertColumn(0, _T("Calendar"),LVCFMT_LEFT,strWidth);
	int colWidth = pList->GetColumnWidth(0);
	strWidth = pList->GetStringWidth(_T("Num Weeks")) + 15;
	pList->InsertColumn(1, _T("Num Weeks"),LVCFMT_LEFT,strWidth);
	colWidth += pList->GetColumnWidth(1);
	strWidth = pList->GetStringWidth(_T("MONTHLY")) + 15;
	pList->InsertColumn(2, _T("Period"),LVCFMT_LEFT,strWidth);
	colWidth += pList->GetColumnWidth(2);
	pList->InsertColumn(3, _T("Quality DB"), LVCFMT_LEFT, controlWidth-colWidth);

	try
	{
		CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
		int nItem = 0;
		if (m_pSet != NULL)	delete m_pSet;
		m_pSet = new CaseSet();
		m_pSet->m_pDatabase = pDoc->m_pDB;
		m_pSet->Open();
		m_pSet->MoveFirst();
		while (!m_pSet->IsEOF()) {
			m_pSet->LockCStrings();
			AddCase(nItem++);
			m_pSet->MoveNext();
		}
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return;
	}
	UpdateWindow();
}

BOOL CRunDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = AfxGetApp()->GetProfileString("Settings","RunWindowPos");
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

void CRunDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer;
	strBuffer.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
				pwp->flags,pwp->showCmd,
				pwp->ptMinPosition.x,pwp->ptMinPosition.y,
				pwp->ptMaxPosition.x,pwp->ptMaxPosition.y,
				pwp->rcNormalPosition.left,pwp->rcNormalPosition.top,
				pwp->rcNormalPosition.right,pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString("Settings","RunWindowPos",strBuffer);
}

BEGIN_MESSAGE_MAP(CRunDlg, CDialog)
	//{{AFX_MSG_MAP(CRunDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_NEW_CASE, OnNewCase)
	ON_BN_CLICKED(IDC_EDIT_CASE, OnEditCase)
	ON_BN_CLICKED(IDC_DELETE_CASE, OnDeleteCase)
	ON_BN_CLICKED(IDC_RUN_CASE, OnRunCase)
	ON_NOTIFY(NM_DBLCLK, IDC_CASE_LIST, OnDblclkCaseList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunDlg message handlers

BOOL CRunDlg::DestroyWindow() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	if (m_pSet) {
		int nCount = pList->GetItemCount();
		COleVariant* pMarker;
		for (int nIndex=0; nIndex < nCount; nIndex++) {
			pMarker = (COleVariant*)pList->GetParam(nIndex);
			delete pMarker;
			pList->SetParam(nIndex,NULL);
		}
		delete m_pSet;	
		m_pSet = NULL;
	}

	WINDOWPLACEMENT wp;
	if (GetWindowPlacement(&wp))
		SaveWindowPlacement(&wp);
	
	return CDialog::DestroyWindow();
}

void CRunDlg::OnOK() 
{
	if (m_pView != NULL)
		m_pView->PostMessage(WM_RUN_GOODBYE,IDOK);
}

void CRunDlg::OnClose() 
{
	if (m_pView != NULL)
		m_pView->PostMessage(WM_RUN_GOODBYE,IDOK);
}

void CRunDlg::OnCancel() 
{
	if (m_pView != NULL)
		m_pView->PostMessage(WM_RUN_GOODBYE,IDOK);
}

BOOL CRunDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// set style so that list view will show its selections even when the
	// control loses focus
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	LONG style = ::GetWindowLong(pList->m_hWnd,GWL_STYLE);
	::SetWindowLong(pList->m_hWnd, GWL_STYLE,style | LVS_SHOWSELALWAYS);
//	pList->SetBkColor(RGB(0,0,0));
//	pList->SetTextColor(RGB(0,175,255));
//	pList->SetTextBkColor(RGB(0,0,0));
	EraseList();
	InitList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRunDlg::OnNewCase() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);

	CCaseDlg dlg(m_pSet);                        	
	m_pSet->AddNew();
	if (dlg.DoModal() != IDOK)
		m_pSet->CancelUpdate();
	else {
		m_pSet->Update();
		m_pSet->MoveLast();
		m_pSet->LockCStrings();
		AddCase(pList->GetItemCount());
	}
}

void CRunDlg::OnEditCase() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;
	COleVariant* pMarker = (COleVariant*)pList->GetParam(nSelected);
	m_pSet->SetBookmark(*pMarker);
	m_pSet->LockCStrings();

	CCaseDlg dlg(m_pSet);                        	
	m_pSet->Edit();
	if (dlg.DoModal() != IDOK) 
		m_pSet->CancelUpdate();
	else {
		m_pSet->Update();
		UpdateCase(nSelected);
	}
}

void CRunDlg::OnDeleteCase() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;

	int retCode = IDYES;
	retCode = MessageBox(
		_T("Are you sure you want to delete this item ?"),
		_T("WinFore - Warning"),MB_YESNO);
	if (retCode != IDYES)
		return;

	COleVariant* pMarker = (COleVariant*)pList->GetParam(nSelected);
	m_pSet->SetBookmark(*pMarker);
	m_pSet->LockCStrings();
	m_pSet->Delete();
	pList->DeleteItem(nSelected);
	delete pMarker;
}

void CRunDlg::OnRunCase() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;

	CString strCal = pList->GetItemText(nSelected,0);
	CString strWeeks = pList->GetItemText(nSelected,1);
	CString strPeriod = pList->GetItemText(nSelected,2);
	CString strQuality = pList->GetItemText(nSelected,3);
	char Msg[301];
	wsprintf(Msg, "CALENDAR: %s\n WEEKS: %s\n PERIOD: %s\n QUALITY: %s",
		(const char*) strCal,
		(const char*) strWeeks,
		(const char*) strPeriod,
		(const char*) strQuality);
   	if (AfxMessageBox(Msg,MB_YESNO|MB_ICONQUESTION) == IDYES) {
		CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
		CComputeDlg dlg;
		dlg.m_nCal = atoi(LPCTSTR(strCal));
		dlg.m_nWeeks = atoi(LPCTSTR(strWeeks));
		dlg.m_cPeriod = *LPCSTR(strPeriod);
		dlg.m_pDB = pDoc->m_pDB;
		pDoc->GetDBName(dlg.m_strDBName);
		dlg.m_strQuality = strQuality;
		dlg.DoModal();
	}
}

void CRunDlg::OnDblclkCaseList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnRunCase();	
	*pResult = 0;
}
