// ComputeDlg.cpp : implementation file
//

#include "stdafx.h"

#include <d:\projects\classlib\date.h>
#include <d:\projects\forelib\error.h>
#include "WinFore.h"
#include "ComputeDlg.h"
#include "CError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

volatile int g_nCount;
volatile int g_nPeriods;
volatile int g_nAnother;

/////////////////////////////////////////////////////////////////////////////
//

inline void YieldControl(void)
{
	MSG message;
	if (::PeekMessage(&message,NULL,0,0,PM_REMOVE)) {
	  ::TranslateMessage(&message);
	  ::DispatchMessage(&message);
	}	
}

////////////////////////////////////////////////////////////////////////////
// CComputeDlg dialog


CComputeDlg::CComputeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComputeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComputeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CComputeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComputeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComputeDlg, CDialog)
	//{{AFX_MSG_MAP(CComputeDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComputeDlg message handlers

BOOL CComputeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	g_nCount = 0;
	m_nTimer = 0;
	return TRUE;  // return TRUE unless you set the
                  //  focus to a control
	              // EXCEPTION: OCX Property Pages
                  //  should return FALSE
}

void CComputeDlg::OnTimer(UINT nIDEvent) 
{
	if (!g_nAnother) return;

	static CString str;
	str.Format("Period: %i  Date: %s",g_nCount,m_pFore->ToDay());
	CWnd *pText = GetDlgItem(IDC_STATIC1);
	if (pText != NULL) pText->SetWindowText(str);

	CProgressCtrl* pBar =
		(CProgressCtrl*) GetDlgItem(IDC_PROGRESS1);
	pBar->SetPos(g_nCount * 100 / g_nPeriods);
}

void CComputeDlg::OnStart() 
{
	CString strReport(m_strDBName+".tmp");
	CQual   *pQual=0;
	CForeDB *pQualDB=0;

	try {
		//
		// read database
		//
		CWnd *pText = GetDlgItem(IDC_STATIC1);
		pText->SetWindowText("Reading DataBase. Please Wait...");
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		m_pFore = new CFore();
		m_pDB->Load(*m_pFore,m_nCal,m_nWeeks);
		//
		// read quality
		//
		pText->SetWindowText("Reading Quality. Please Wait...");
		CProgressCtrl* pBar =
			(CProgressCtrl*) GetDlgItem(IDC_PROGRESS1);
		pBar->SetPos(100);
		pQualDB = new CForeDB();
		pQualDB->Open(m_strQuality,FALSE,FALSE);
		pQual = new CQual(pQualDB->SampleNumber());
		int nLW = pQualDB->LoadSamples("LW",pQual->Sample(),
											pQual->XCoord(),
											pQual->YCoord(),
											pQual->Quality());
		int nCM = pQualDB->LoadSamples("CM",pQual->Sample()+nLW,
											pQual->XCoord()+nLW,
											pQual->YCoord()+nLW,
											pQual->Quality()+nLW);
		pQual->SetLWNumber(nLW);
		pQual->SetCMNumber(nCM);
		pQualDB->LoadAdjustments("LW",pQual->LWAdjustments());
		pQualDB->LoadAdjustments("CM",pQual->CMAdjustments());
		pQual->StartEstimator();
		CGeometry::AttachQuality(pQual);
		//
		// setup timer to allow user to interrupt forecast
		//
		g_nAnother = 0;
		m_nTimer = SetTimer(1, 100, NULL); // 1/10 second
		ASSERT(m_nTimer != 0);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		//
		// do forecast
		//
		m_pDB->ClearResults();
		m_pDB->OpenResults();
		m_pFore->Start(m_cPeriod);
		g_nPeriods = m_pFore->WorkCalendar().size();
		if (m_cPeriod == 'D') g_nPeriods *= 7;
		if (m_cPeriod == 'M') g_nPeriods /= 4;
		g_nCount = 0;
		do {
			g_nAnother = m_pFore->NextPeriod();
			m_pFore->PeriodQuality();
			m_pDB->SaveResults(m_pFore->Results(),m_cPeriod,m_pFore->AvailableDays(),
							   pQual->PercentWashed(),pQual->RawMoisture(),
							   pQual->CleanMoisture());
			YieldControl();
			++g_nCount;
		} while (g_nAnother);
		m_pFore->End();
		m_pDB->CloseResults();
	}
	catch (CException* e) {
		if (e->IsKindOf(RUNTIME_CLASS(CDaoException)))
			::DisplayDaoException((CDaoException *)e);
		else if (e->IsKindOf(RUNTIME_CLASS(CMemoryException)))
			AfxMessageBox("Out of Memory.",MB_ICONEXCLAMATION);
		e->Delete();
	}
	catch (ErrorInfo e) {
		e.ErrorMsg();
	}
	catch (ForeError e) {
		AfxMessageBox(e.ErrorMsg(),MB_ICONEXCLAMATION);
	}
	//
	// Done
	//
	if(pQualDB) {
		pQualDB->Close();
		delete pQualDB;
	}
	if(pQual) delete pQual;
	if(m_pFore) delete m_pFore;
	if(m_nTimer) KillTimer(m_nTimer);
	AfxMessageBox("Done",MB_ICONEXCLAMATION);

	CDialog::OnOK();
}

void CComputeDlg::OnCancel() 
{
	if (g_nCount == 0)
		CDialog::OnCancel();
	else 
		g_nAnother = 0;
}
