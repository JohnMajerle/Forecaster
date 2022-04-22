// DataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinFore.h"
#include "WinForeDoc.h"
#include "DataDlg.h"
#include "renamedl.h"
#include "DlgSQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString ForeTables[12] = {
			"AREA_EXCEPTIONS",
			"CAL_EXCEPTIONS",
			"CALENDAR",
			"DATE_EXCEPTIONS",
			"GEOMETRY",
			"RATES",
			"REF_EXCEPTIONS",
			"RESULTS",
			"SEQUENCE",
			"SHUTDOWNS",
			"ADJUSTMENTS",
			"SAMPLES"};

/////////////////////////////////////////////////////////////////////////////
// CDataDlg dialog

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pView = NULL;
	m_bTable = TRUE;
}

CDataDlg::CDataDlg(CView* pView)
{
	m_pView = pView;
	m_bTable = TRUE;
}

BOOL CDataDlg::Create()
{
	if (!CDialog::Create(CDataDlg::IDD))
		return 0;

	WINDOWPLACEMENT wp;
	if (LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);
	return 1;
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CDataDlg::EraseList()
{
	CListCtrl* pList = (CListCtrl*) GetDlgItem(IDC_TABLELIST);
	pList->DeleteAllItems();
	while(pList->DeleteColumn(0));
	UpdateWindow();
}

void CDataDlg::InitList()
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);

	// get size of control to help set the column widths
	CRect controlRect;
	pList->GetClientRect(controlRect);

	// set up column--leave room for scroll bar
	if (m_bTable)
		pList->InsertColumn(0, _T("Table"), LVCFMT_LEFT,controlRect.Width(), 0);
	else
		pList->InsertColumn(0, _T("Query"), LVCFMT_LEFT,controlRect.Width(), 0);

	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	CDaoDatabase* pDB = pDoc->GetDatabase();
	try
	{
		if (m_bTable) {
			int nItem = 0;
			for (int i = 0; i < 12; i++) {
				if (i < 10)
					pList->AddItem(nItem++, 0,ForeTables[i],1);
				else
					pList->AddItem(nItem++, 0,ForeTables[i],0);
			}
		} else {
			int nItem = 0;
			int nQueryDefCount = pDB->GetQueryDefCount();
			CDaoQueryDefInfo qdInfo;
			for (int j=0; j < nQueryDefCount; j++)
			{
				pDB->GetQueryDefInfo(j,qdInfo,AFX_DAO_ALL_INFO);
				if (qdInfo.m_nType == 5)
					continue;
				pList->AddItem(nItem++, 0,qdInfo.m_strName,3);
			}
		}
	}
	catch (CDaoException* e)
	{
		e->Delete();
	}
	UpdateWindow();
}

BOOL CDataDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = AfxGetApp()->GetProfileString("Settings","DataWindowPos");
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

void CDataDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer;
	strBuffer.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
				pwp->flags,pwp->showCmd,
				pwp->ptMinPosition.x,pwp->ptMinPosition.y,
				pwp->ptMaxPosition.x,pwp->ptMaxPosition.y,
				pwp->rcNormalPosition.left,pwp->rcNormalPosition.top,
				pwp->rcNormalPosition.right,pwp->rcNormalPosition.bottom);
	AfxGetApp()->WriteProfileString("Settings","DataWindowPos",strBuffer);
}

BEGIN_MESSAGE_MAP(CDataDlg, CDialog)
	//{{AFX_MSG_MAP(CDataDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_NOTIFY(NM_RCLICK, IDC_TABLELIST, OnRclickTablelist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_TABLELIST, OnKeydownTablelist)
	ON_NOTIFY(NM_DBLCLK, IDC_TABLELIST, OnDblclkTablelist)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_QUERY_OPEN,ID_TABLE_REMOVE_RECORDS, OnPopupCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataDlg message handlers

void CDataDlg::OnOK() 
{
	if (m_pView != NULL)
		m_pView->PostMessage(WM_DATA_GOODBYE,IDOK);
}

void CDataDlg::OnClose() 
{
	if (m_pView != NULL)
		m_pView->PostMessage(WM_DATA_GOODBYE,IDOK);
}

void CDataDlg::OnCancel() 
{
	if (m_pView != NULL)
		m_pView->PostMessage(WM_DATA_GOODBYE,IDOK);
}

BOOL CDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SmallImageList.Create(IDB_SMALLICONS,16,1,RGB(255,255,255));
	// set style so that list view will show its selections even when the
	// control loses focus
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	LONG style = ::GetWindowLong(pList->m_hWnd,GWL_STYLE);
	::SetWindowLong(pList->m_hWnd, GWL_STYLE,style | LVS_SHOWSELALWAYS);
	pList->SetImageList(&m_SmallImageList,LVSIL_SMALL);
	pList->SetBkColor(RGB(0,0,0));
	pList->SetTextColor(RGB(0,175,255));
	pList->SetTextBkColor(RGB(0,0,0));
	EraseList();
	InitList();

	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	short state = 0;	// restore
	pDoc->m_browse.VBForeWindowState(&state);
	BOOL visible = TRUE;
	pDoc->m_browse.VBForeWindowVisible(&visible);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataDlg::OnOpen() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1) {	// no selection
		if (m_bTable) return;
		// option to create new query
		OnNewQuery();
		return;
	}

	CString strTable = pList->GetItemText(nSelected,0);
	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();

	if (m_bTable) {
		if (!pDoc->OpenTable(strTable))
			AfxMessageBox("Server not yet started.");
	} else {
		if (!pDoc->RunQuery(strTable))
			AfxMessageBox("Server not yet started.");
	}
}

void CDataDlg::OnRclickTablelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_bTable)
		DoPopupMenu(IDR_POPUP_TABLE);
	else
		DoPopupMenu(IDR_POPUP_QUERY);

	*pResult = 0;
}

void CDataDlg::DoPopupMenu(UINT nMenuID)
{
	CMenu popMenu;

	popMenu.LoadMenu(nMenuID);
	
	CPoint posMouse;
	GetCursorPos(&posMouse);

	popMenu.GetSubMenu(0)->TrackPopupMenu(0,posMouse.x,posMouse.y,this);
}

void CDataDlg::OnPopupCommand(UINT nMenuID)
{
	switch (nMenuID)
	{
		case ID_TABLE_OPEN:
		case ID_QUERY_OPEN:
			OnOpen();
			break;
		case ID_TABLE_DESIGN:
			AfxMessageBox("Not yet implemented");
			break;
		case ID_TABLE_REMOVE_RECORDS:
			RemoveRecords();
			break;
		case ID_QUERY_COPY:
			OnCopy();
			break;
		case ID_QUERY_DELETE:
			OnDelete();
			break;
		case ID_QUERY_EDIT:
			OnEditQuery();
			break;
		case ID_QUERY_RENAME:
			RenameQuery();
			break;
		case ID_QUERY_NEW:
			OnNewQuery();
			break;
	}
}

void CDataDlg::OnCopy() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1  ||  m_bTable)
		return;
	m_strItemName = pList->GetItemText(nSelected,0);

	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	CDaoDatabase* pDB = pDoc->GetDatabase();

	if (CopyQuery(pDB,pDB)) 
		pList->AddItem(pList->GetItemCount(),0,m_strItemName,3);
}

BOOL CDataDlg::CopyQuery(CDaoDatabase* pDBSource,CDaoDatabase* pDBTarget)
{
	CRenameDlg dlgRename;
	dlgRename.m_strName = m_strItemName;

	// Find out if the querydef exists already
	CDaoQueryDefInfo queryInfo;
	int nQueryDefCount = pDBTarget->GetQueryDefCount();
	for (int i=0; i < nQueryDefCount; i++)
	{
		pDBTarget->GetQueryDefInfo(i,queryInfo);
		if (queryInfo.m_strName == dlgRename.m_strName)
		{
			if (dlgRename.DoModal() == IDCANCEL)
				return FALSE;
			i--;
		}
	}

	CDaoQueryDef qdTarget(pDBTarget);

	try
	{
		pDBSource->GetQueryDefInfo(m_strItemName,queryInfo,AFX_DAO_ALL_INFO);

		qdTarget.Create(dlgRename.m_strName,queryInfo.m_strSQL);

		qdTarget.Append();
		qdTarget.Close();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}

	m_strItemName = dlgRename.m_strName;
	return TRUE;
}

void CDataDlg::OnDelete() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1  ||  m_bTable)
		return;

	int retCode = IDYES;
	retCode = MessageBox(
		_T("Are you sure you want to delete this item ?"),
		_T("WinFore - Warning"),MB_YESNO);
	if (retCode != IDYES)
		return;

	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	CDaoDatabase* pDB = pDoc->GetDatabase();
	CString strTable = pList->GetItemText(nSelected,0);
	try
	{
		pDB->DeleteQueryDef(strTable);
		pList->DeleteItem(nSelected);
	}
	catch (CDaoException* e)
	{
		MessageBox(
			_T("WinFore - Warning"),
			e->m_pErrorInfo->m_strDescription);
		e->Delete();
	}
}

void CDataDlg::OnEditQuery() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;

	try
	{
		CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
		CDaoDatabase* pDB = pDoc->GetDatabase();
		CDlgSQL dlgSQL(pDoc);

		dlgSQL.m_bEditMode = TRUE;

		CDaoQueryDef qdEdit(pDB);
		qdEdit.Open(pList->GetItemText(nSelected,0));

		dlgSQL.m_strName = pList->GetItemText(nSelected,0);
		dlgSQL.m_strSQL = qdEdit.GetSQL();

		if (dlgSQL.DoModal() == IDOK)
			qdEdit.SetSQL(dlgSQL.m_strSQL);

		qdEdit.Close();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return;
	}
}
void CDataDlg::OnNewQuery() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	CDlgSQL dlgSQL(pDoc);

	if (dlgSQL.DoModal() != IDOK)
		return;

	try
	{
		CDaoDatabase* pDB = pDoc->GetDatabase();
		CDaoQueryDef qdTarget(pDB);

		qdTarget.Create(dlgSQL.m_strName,dlgSQL.m_strSQL);
		qdTarget.Append();
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
		return;
	}
	pList->AddItem(pList->GetItemCount(),0,dlgSQL.m_strName,3);
}

void CDataDlg::RenameQuery() 
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;
	m_strItemName = pList->GetItemText(nSelected,0);

	// Find out if the querydef exists already
	CRenameDlg dlgRename;
	dlgRename.m_strName = m_strItemName;
	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	CDaoDatabase* pDB = pDoc->GetDatabase();
	CDaoQueryDefInfo queryInfo;
	int nQueryDefCount = pDB->GetQueryDefCount();
	for (int i=0; i < nQueryDefCount; i++)
	{
		pDB->GetQueryDefInfo(i,queryInfo);
		if (queryInfo.m_strName == dlgRename.m_strName)
		{
			if (dlgRename.DoModal() == IDCANCEL)
				return;
			i--;
		}
	}

	// Rename query
	CString strNew = dlgRename.m_strName;
	CDaoQueryDef qdfDef(pDoc->GetDatabase());
	qdfDef.Open(m_strItemName);
	qdfDef.SetName(strNew);
	qdfDef.Close();
	pList->SetItemText(nSelected,0,strNew);
}

void CDataDlg::OnKeydownTablelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDown = (LV_KEYDOWN*)pNMHDR;

	if (pLVKeyDown->wVKey == VK_DELETE)
	{
		OnDelete();
	}
	else if (pLVKeyDown->wVKey == VK_RETURN)
	{
		OnOpen();	
	}
	else if (pLVKeyDown->wVKey == VK_F2)
	{
		CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
		pList->EditLabel(pList->GetNextItem(-1,LVNI_SELECTED));
	}

	*pResult = 0;
}

void CDataDlg::OnDblclkTablelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOpen();	
	*pResult = 0;
}

BOOL CDataDlg::DestroyWindow() 
{
	m_SmallImageList.Detach();

	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	BOOL visible = FALSE;
	pDoc->m_browse.VBForeWindowVisible(&visible);

	WINDOWPLACEMENT wp;
	if (GetWindowPlacement(&wp))
		SaveWindowPlacement(&wp);
	
	return CDialog::DestroyWindow();
}

void CDataDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED  ||  nType == SIZE_RESTORED) {
		CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
		if (pDoc->m_browse.m_lpDispatch != NULL) {
			short state = (nType == SIZE_MINIMIZED) ? 1 : 0 ;
			pDoc->m_browse.VBForeWindowState(&state);
		}
	}
}

void CDataDlg::RemoveRecords()
{
	CListCtrlEx* pList = (CListCtrlEx*) GetDlgItem(IDC_TABLELIST);
	int nSelected = pList->GetNextItem(-1,LVNI_SELECTED);
	if (nSelected == -1)
		return;

	CWinForeDoc* pDoc = (CWinForeDoc*)m_pView->GetDocument();
	CDaoDatabase* pDB = pDoc->GetDatabase();
	try
	{
		CString strMsg;
		strMsg.Format("delete all records from %s?",pList->GetItemText(nSelected,0));
		if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) {
			CWaitCursor waiter;
			CString strDelete;
			strDelete.Format("delete from %s",pList->GetItemText(nSelected,0));
			pDB->Execute(strDelete,dbFailOnError);
			if (pDB->GetRecordsAffected() > 0) {
				CString strResult;
				strResult.Format("%ld rows deleted!",pDB->GetRecordsAffected());
				AfxMessageBox(strResult,MB_ICONEXCLAMATION);
			}
		}
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}
}
