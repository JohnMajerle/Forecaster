#include "stdafx.h"
#include "WinFore.h"
#include "WinForeDoc.h"
#include "WinForeView.h"
#include "Thread.h"

UINT LaunchAndWait(LPVOID pParam)
{
	// assign address of parameter
	LPTHREADINFO pThreadInfo = (LPTHREADINFO)pParam;

	// process structure that will receive info on new process
	PROCESS_INFORMATION stProcessInfo;

	// launch application
	if (LaunchApplication(pThreadInfo->strPathName,&stProcessInfo)) {

		// store thread in list
		pThreadInfo->pcsList->Lock();
		AddToList(pThreadInfo->hWndList,
				pThreadInfo->strPathName,
				(DWORD)AfxGetThread()->m_hThread);
		pThreadInfo->pcsList->Unlock();

		// wait for application to terminate or event to be signaled
		HANDLE hThreads[2];
		hThreads[0] = pThreadInfo->pTermThreadEvent->m_hObject;
		hThreads[1] = stProcessInfo.hProcess;
		DWORD dwIndex = WaitForMultipleObjects(2,hThreads,FALSE,INFINITE);

		// remove from list only if application signaled
		if (dwIndex) {
			pThreadInfo->pcsList->Lock();
			RemoveFromList(pThreadInfo->hWndList,AfxGetThread()->m_hThread);
			pThreadInfo->pcsList->Unlock();
		}

		// close handles tp process and its main thread
		CloseHandle(stProcessInfo.hThread);
		CloseHandle(stProcessInfo.hProcess);
	}

	// free thread info structure
	if (pThreadInfo)
		delete pThreadInfo;

	return 0;
}

BOOL LaunchApplication(LPCTSTR pCmdLine, PROCESS_INFORMATION* pProcessInfo) 
{
	STARTUPINFO si;

	// Initialize the STARTUPINFO structure.
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWDEFAULT;

	return CreateProcess(NULL,LPTSTR(pCmdLine),NULL,NULL,FALSE,
					NORMAL_PRIORITY_CLASS,NULL,NULL,
					&si,pProcessInfo);
}

void AddToList(HWND hWnd, LPCTSTR pCmdLine, DWORD hThread)
{
	CListBox* pList = (CListBox*)CWnd::FromHandle(hWnd);
	int nIndex = pList->AddString(pCmdLine);
	pList->SetItemData(nIndex,hThread);
}

void RemoveFromList(HWND hWnd, HANDLE hThread)
{
	CListBox* pList = (CListBox*)CWnd::FromHandle(hWnd);

	int nItems = pList->GetCount();
	int nIndex = 0;
	BOOL bFound = FALSE;
	while (!bFound && (nIndex < nItems)) {
		if ((HANDLE)pList->GetItemData(nIndex) == hThread) {
			pList->DeleteString(nIndex);
			bFound = TRUE;
		}
		nIndex++;
	}
}
