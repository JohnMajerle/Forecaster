//*****************************************************************************
//
// Module:  CError.CPP
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class source file for the CError class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#include "stdafx.h"

#include "CError.h"

//*****************************************************************************
//
// CError - global helper functions to display error message
//
//*****************************************************************************

void DaoErrorMsg(CDaoException* e)
{
    char errorMsg[301];
   	wsprintf(errorMsg, "DAO error %d, SOURCE = %s, DESCR = %s",
        e->m_pErrorInfo->m_lErrorCode,
        (const char*) e->m_pErrorInfo->m_strSource,
        (const char*) e->m_pErrorInfo->m_strDescription);
   	AfxMessageBox(errorMsg);
}

void ReportSaveLoadException(LPCTSTR lpszPathName,	CException* e,
							 BOOL bSaving, UINT nIDPDefault)
{
	UINT nIDP = nIDPDefault;
	UINT nHelpContext = nIDPDefault;
	CString prompt;

	if (e != NULL)
	{
		ASSERT_VALID(e);

		if (e->IsKindOf(RUNTIME_CLASS(CArchiveException)))
		{
			switch (((CArchiveException*)e)->m_cause)
			{
			case CArchiveException::badSchema:
			case CArchiveException::badClass:
			case CArchiveException::badIndex:
			case CArchiveException::endOfFile:
				nIDP = AFX_IDP_FAILED_INVALID_FORMAT;
				break;
			default:
				break;
			}
		}
		else if (e->IsKindOf(RUNTIME_CLASS(CFileException)))
		{
			TRACE1("Reporting file I/O exception on Save/Load with lOsError = $%lX.\n",
				((CFileException*)e)->m_lOsError);

			CFileException* pFileException = (CFileException*)e;
			if (pFileException->m_strFileName.IsEmpty())
				pFileException->m_strFileName = lpszPathName;

			LPTSTR lpszMessage = prompt.GetBuffer(255);
			ASSERT(lpszMessage != NULL);
			if (!e->GetErrorMessage(lpszMessage, 256, &nHelpContext))
			{
				switch (((CFileException*)e)->m_cause)
				{
					case CFileException::fileNotFound:
					case CFileException::badPath:
						nIDP = AFX_IDP_FAILED_INVALID_PATH;
						break;
					case CFileException::diskFull:
						nIDP = AFX_IDP_FAILED_DISK_FULL;
						break;
					case CFileException::accessDenied:
						nIDP = bSaving ? AFX_IDP_FAILED_ACCESS_WRITE :
								AFX_IDP_FAILED_ACCESS_READ;
						break;

					case CFileException::badSeek:
					case CFileException::generic:
					case CFileException::tooManyOpenFiles:
					case CFileException::invalidFile:
					case CFileException::hardIO:
					case CFileException::directoryFull:
						break;

					default:
						break;
				}
			}
			prompt.ReleaseBuffer();
		}
	}

	if (prompt.IsEmpty())
	{
		TCHAR szTitle[_MAX_PATH];
		lstrcpyn(szTitle, lpszPathName, sizeof(szTitle));
		AfxFormatString1(prompt, nIDP, szTitle);
	}

	AfxMessageBox(prompt, MB_ICONEXCLAMATION, nHelpContext);
}

void ErrorInfo::ErrorMsg()
{
    char errorMsg[301];
	if (error == CForeDB_NO_RECORDS) 
		wsprintf(errorMsg, "Error with %s table: No records.",source);
	else if (error == FILE_OPEN_ERROR) 
		wsprintf(errorMsg, "Error opening file %s.",source);
	else if (error == FILE_WRITE_ERROR) 
		wsprintf(errorMsg, "Error writing to file %s.",source);
	else if (error == FILE_READ_ERROR) 
		wsprintf(errorMsg, "Error reading file %s.",source);
	else
		wsprintf(errorMsg, "Unknown Error");
   	AfxMessageBox(errorMsg,MB_ICONEXCLAMATION);
}

//*****************************************************************************

// end of CError.CPP
