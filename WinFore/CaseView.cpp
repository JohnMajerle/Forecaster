// CaseView.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "WinForeDoc.h"
#include "CaseDlg.h"
#include "CaseView.h"
#include "MainFrm.h"
#include "ComputeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaseView

IMPLEMENT_DYNCREATE(CCaseView, CFormView)

CCaseView::CCaseView()
	: CFormView(CCaseView::IDD)
{
	//{{AFX_DATA_INIT(CCaseView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCaseView::~CCaseView()
{
}

void CCaseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaseView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCaseView, CFormView)
	//{{AFX_MSG_MAP(CCaseView)
	ON_BN_CLICKED(IDOK, OnDone)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EDIT_CASE, OnEditCase)
	ON_BN_CLICKED(IDC_NEW_CASE, OnNewCase)
	ON_BN_CLICKED(IDC_DELETE_CASE, OnDeleteCase)
	ON_BN_CLICKED(IDC_RUN_CASE, OnRunCase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaseView diagnostics

#ifdef _DEBUG
void CCaseView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCaseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWinForeDoc* CCaseView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinForeDoc)));
	return (CWinForeDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCaseView operations

void CCaseView::AddCase(int nIndex)
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

void CCaseView::UpdateCase(int nIndex)
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	COleVariant var;
	for (int i=0; i < m_pSet->m_nFields; i++) {
		var = m_pSet->GetFieldValue(i);
		pList->UpdateItem(nIndex,i,CCrack::strVARIANT(var));
	}
}

void CCaseView::EraseList()
{
	CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_CASE_LIST);
	pList->DeleteAllItems();
	while(pList->DeleteColumn(0));
	UpdateWindow();
}

void CCaseView::InitList()
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);

	CDaoFieldInfo fieldInfo;
	int nFields;
	// Attempt to open the table (which may fail with a security violation)

	EraseList();

	CDaoTableDef td(GetDocument()->GetDatabase());
	try
	{
		td.Open("CASE");
		nFields = td.GetFieldCount();
		for (int j=0; j < nFields; j++)
		{
			td.GetFieldInfo(j,fieldInfo);
			pList->AddColumn(fieldInfo.m_strName,j);
		}
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}
	td.Close();

	int nItem = 0;
	try
	{
		m_pSet->MoveFirst();
		while (!m_pSet->IsEOF()) {
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

/////////////////////////////////////////////////////////////////////////////
// CCaseView message handlers

void CCaseView::OnDone() 
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SwitchToView(FORE);
}

void CCaseView::OnInitialUpdate() 
{
	CWinForeDoc* pDoc = GetDocument();
	m_pSet = new CaseSet();
	m_pSet->m_pDatabase = pDoc->GetDatabase();
	if (!m_pSet->m_pDatabase->IsOpen())
		return;
	m_pSet->Open();

	CFormView::OnInitialUpdate();
	
	InitList();	
}

void CCaseView::OnDestroy() 
{
	CFormView::OnDestroy();

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
	}
}

void CCaseView::OnEditCase() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;
	COleVariant* pMarker = (COleVariant*)pList->GetParam(nSelected);
	m_pSet->SetBookmark(*pMarker);

	CCaseDlg dlg(m_pSet);                        	
	m_pSet->Edit();
	if (dlg.DoModal() != IDOK) 
		m_pSet->CancelUpdate();
	else {
		m_pSet->Update();
		UpdateCase(nSelected);
	}
}

void CCaseView::OnNewCase() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);

	CCaseDlg dlg(m_pSet);                        	
	m_pSet->AddNew();
	if (dlg.DoModal() != IDOK)
		m_pSet->CancelUpdate();
	else {
		m_pSet->Update();
		m_pSet->MoveLast();
		AddCase(pList->GetItemCount());
	}
}

void CCaseView::OnDeleteCase() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_CASE_LIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;
	COleVariant* pMarker = (COleVariant*)pList->GetParam(nSelected);
	m_pSet->SetBookmark(*pMarker);
	m_pSet->Delete();
	pList->DeleteItem(nSelected);
	delete pMarker;
}

void CCaseView::OnRunCase() 
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
		CWinForeDoc* pDoc = GetDocument();
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
