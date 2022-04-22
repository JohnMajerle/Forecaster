// WinForeDoc.cpp : implementation of the CWinForeDoc class
//

#include "stdafx.h"
#include "WinFore.h"
#include "WinForeDoc.h"
#include "WinForeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString ForeTables[12];
CString DosForeTables[12] = {
			"AREXCPT",
			"CAEXCPT",
			"CALENDAR",
			"DTEXCPT",
			"GEOMETRY",
			"RATE",
			"RFEXCPT",
			"",
			"SEQUENCE",
			"STEXCPT",
			"ADJSTMNT",
			"SAMPLE"};

/////////////////////////////////////////////////////////////////////////////
// Global routines

void _stripfilename(CString& path)
{
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	strcpy(path_buffer,LPCSTR(path));
	_splitpath( path_buffer, drive, dir, fname, ext );
	_makepath( path_buffer, drive, dir, NULL, NULL );
	CString tmp(path_buffer);
	path = tmp;
}

/////////////////////////////////////////////////////////////////////////////
// CWinForeDoc

IMPLEMENT_DYNCREATE(CWinForeDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinForeDoc, CDocument)
	//{{AFX_MSG_MAP(CWinForeDoc)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT, OnUpdateFileImport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinForeDoc construction/destruction

CWinForeDoc::CWinForeDoc()
{
	m_pDB = NULL;
}

CWinForeDoc::~CWinForeDoc()
{
	if(m_pDB) {
		if(m_pDB->IsOpen())
			m_pDB->Close();
		delete m_pDB;
	}
	TRY
	{
		m_browse.VBForeClose();
	}
	END_TRY
}

BOOL CWinForeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWinForeDoc serialization

void CWinForeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWinForeDoc diagnostics

#ifdef _DEBUG
void CWinForeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinForeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinForeDoc operations

void CWinForeDoc::GetDBName(CString& strDBName) 
{
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szName[_MAX_FNAME];

	_splitpath(m_pDB->GetName(),szDrive,szDir,szName,NULL);
	strDBName.Format(_T("%s%s%s"),szDrive,szDir,szName);
}

void CWinForeDoc::OpenBrowser() 
{
	// create the browser object that we'll drive through OLE automation
	COleException e;
	CLSID clsid;
	if (CLSIDFromProgID(OLESTR("PCForecaster.VBForeClass"), &clsid) != NOERROR)
	{
		AfxMessageBox(IDP_UNABLE_TO_CREATE);
		return;
	}

	// try to get the active browser before creating a new one
	LPUNKNOWN lpUnk;
	LPDISPATCH lpDispatch;
	if (GetActiveObject(clsid, NULL, &lpUnk) == NOERROR)
	{
		HRESULT hr = lpUnk->QueryInterface(IID_IDispatch, 
			(LPVOID*)&lpDispatch);
		lpUnk->Release();
		if (hr == NOERROR)
			m_browse.AttachDispatch(lpDispatch, TRUE);
	}

	// if not dispatch ptr attached yet, need to create one
	if (m_browse.m_lpDispatch == NULL && 
		!m_browse.CreateDispatch(clsid, &e))
	{
		AfxMessageBox(IDP_UNABLE_TO_CREATE);
		return;
	}

}

BOOL CWinForeDoc::OpenTable(CString& strTable) 
{
	if (m_browse.m_lpDispatch != NULL) {
		short state = 0;	// restore
		m_browse.VBForeWindowState(&state);
		BOOL visible = TRUE;
		m_browse.VBForeWindowVisible(&visible);
		if (strTable == "SAMPLES"  ||  strTable == "ADJUSTMENTS") {
			CString strSQL("select * from ");
			strSQL += strTable;
			BSTR bstr;
			bstr = strSQL.AllocSysString();
			m_browse.VBForeRunQuery(&bstr);
			SysFreeString(bstr);
		} else {
			BSTR bstr;
			bstr = strTable.AllocSysString();
			m_browse.VBForeOpenTable(&bstr);
			SysFreeString(bstr);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CWinForeDoc::RunQuery(CString& strTable) 
{
	if (m_browse.m_lpDispatch != NULL) {
		short state = 0;	// restore
		m_browse.VBForeWindowState(&state);
		BOOL visible = TRUE;
		m_browse.VBForeWindowVisible(&visible);
		BSTR bstr;
		bstr = strTable.AllocSysString();
		m_browse.VBForeRunQueryDef(&bstr);
		SysFreeString(bstr);
		return TRUE;
	}
	return FALSE;
}

BOOL CWinForeDoc::VCMineOpenDatabase(LPCTSTR lpszPathName, UINT nCount) 
{
	HWND hWnd = ::FindWindow(VCMINE_CLASSNAME,NULL);
	if (hWnd) {
		static char mybuf[1024];
		memcpy(mybuf,lpszPathName,nCount);
		COPYDATASTRUCT cds;
		cds.dwData = ID_VCMINE_OPEN_DATABASE;
		cds.cbData = nCount;
		cds.lpData = mybuf;
		CWinApp* pApp = AfxGetApp();
		HWND hMainWnd = pApp ? pApp->m_pMainWnd->GetSafeHwnd() : NULL;
		::SendMessage(hWnd,WM_COPYDATA,(WPARAM)hMainWnd,(LPARAM)&cds);
		return TRUE;
	} else
		return FALSE;
}

BOOL CWinForeDoc::VCReportOpenResults(LPCTSTR lpszPathName, UINT nCount) 
{
	HWND hWnd = ::FindWindow(VCREPORT_CLASSNAME,NULL);
	if (hWnd) {
		static char mybuf[1024];
		memcpy(mybuf,lpszPathName,nCount);
		COPYDATASTRUCT cds;
		cds.dwData = ID_VCREPORT_OPEN_RESULTS;
		cds.cbData = nCount;
		cds.lpData = mybuf;
		CWinApp* pApp = AfxGetApp();
		HWND hMainWnd = pApp ? pApp->m_pMainWnd->GetSafeHwnd() : NULL;
		::SendMessage(hWnd,WM_COPYDATA,(WPARAM)hMainWnd,(LPARAM)&cds);
		return TRUE;
	} else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CWinForeDoc commands

BOOL CWinForeDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;

	CWinForeApp* pApp = (CWinForeApp *)AfxGetApp();
	pApp->UpdateRegWithDocPath(lpszPathName);
	
	CForeDB* tmpDB = new CForeDB();

	// Attempt to open the new database before replacing our ptr
	try
	{
		tmpDB->Open(lpszPathName,FALSE,FALSE);
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		delete tmpDB;
		e->Delete();
		return FALSE;
	}


	if (m_pDB)
	{
		if (m_pDB->IsOpen())
			m_pDB->Close();
		delete m_pDB;
	}
	m_pDB = tmpDB;

	// refresh browser
	if (m_browse.m_lpDispatch == NULL)
		OpenBrowser();
	if (m_browse.m_lpDispatch != NULL) {	// may have been problem opening browser
		BSTR bstr;
		bstr = m_pDB->GetName().AllocSysString();
		m_browse.VBForeOpenDatabase(&bstr);
		SysFreeString(bstr);
	}

	// refresh graphics and results
	CString strName = m_pDB->GetName();
	VCMineOpenDatabase(strName,strName.GetLength());
	VCReportOpenResults(strName,strName.GetLength());

	// refresh title
	CString title(_T("WinFore - "));
	title += lpszPathName;
	AfxGetMainWnd()->SetWindowText(title);

	return TRUE;
}

void CWinForeDoc::OnUpdateFileImport(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_pDB != NULL);
}

void CWinForeDoc::OnFileImport() 
{
	const char fileDialogFilter[]=
		"Access files (*.mdb)|*.mdb|FORE DOS Ver. 2.54 (*.dbf)|*.dbf||";
	CFileDialog dlg(TRUE,"dbf",NULL,OFN_FILEMUSTEXIST,
		fileDialogFilter);
	dlg.m_ofn.lpstrTitle = "Import Existing Forecast Database";
	if (dlg.DoModal() != IDOK) return;

	CString strConnect;
	CString* pInTables;
	if (dlg.GetFileExt() == "mdb") {
		strConnect.Format("[;database=%s]",dlg.GetPathName());
		pInTables = ForeTables;
	} else {
		CString path(dlg.GetPathName());
		_stripfilename(path);
		strConnect.Format("[FoxPro 2.5;database=%s]",path);
		pInTables = DosForeTables;
	}
	try
	{
		CWaitCursor waiter;
		for (int i = 0; i < 12; i++) {
			if (i == 7)	continue;	// skip RESULTS
			if (i == 8  &&  dlg.GetFileExt() != "mdb") {
				// handle old recovery field in sequence
				CDaoTableDef daoTblDef(m_pDB);
				daoTblDef.Open("SEQUENCE");
				daoTblDef.CreateField("RECOV",dbDouble,0);
				daoTblDef.Close();
			}
			CString strSQL;
			strSQL.Format("insert into %s select * from %s in \"\" ",
				ForeTables[i],*(pInTables + i));
			strSQL += strConnect;
			m_pDB->Execute(strSQL,dbFailOnError);
		}
		if (dlg.GetFileExt() != "mdb") {
			m_pDB->Execute("select * into GEOTEMP from GEOMETRY",dbFailOnError);
			m_pDB->Execute("delete from GEOMETRY",dbFailOnError);
			m_pDB->Execute("insert into GEOMETRY select distinct GEOTEMP.SECT,"
				"TYPE,INIT_STN,X_COORD,Y_COORD,AZIMUTH,LENGTH,WIDTH,SEQUENCE.RECOV "
				"from GEOTEMP inner join SEQUENCE on GEOTEMP.SECT = SEQUENCE.SECT",
				dbFailOnError);
			m_pDB->Execute("update GEOMETRY set LINEAR_FACTOR = 1.0",dbFailOnError);
			m_pDB->Execute("drop table GEOTEMP",dbFailOnError);
			// delete recovery field from sequence
			CDaoTableDef daoTblDef(m_pDB);
			daoTblDef.Open("SEQUENCE");
			daoTblDef.DeleteField("RECOV");
			daoTblDef.Close();
		}
		AfxMessageBox("Data Successfully Imported",MB_ICONEXCLAMATION);
	}
	catch (CDaoException* e)
	{
		DisplayDaoException(e);
		e->Delete();
	}
}
