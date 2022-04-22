//*****************************************************************************
//
// Module:  CError.h
// Author:  Paul S. Dovenor
//
//          (C) Copyright Consol Inc. 1997, All Rights Reserved
//
// Purpose: C++ class header file for the CError class
//
// Notes:   This is a base class.
//
//*****************************************************************************

#ifndef __CError_H
#define __CError_H

extern void DaoErrorMsg(CDaoException* e);
extern void ReportSaveLoadException(LPCTSTR lpszPathName,	CException* e,
									BOOL bSaving, UINT nIDPDefault);

enum {
	CFore_NO_ERROR,
	CForeDB_NO_RECORDS,
	FILE_OPEN_ERROR,
	FILE_WRITE_ERROR,
	FILE_READ_ERROR
};

class ErrorInfo
{
public:
	long    error;
	CString source;

	ErrorInfo() {}
	ErrorInfo(long err, CString src) : error(err), source(src) {} 
	void ErrorMsg();
};

#endif

//*****************************************************************************

// end of CError.h
