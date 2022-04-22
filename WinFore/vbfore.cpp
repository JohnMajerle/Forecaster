// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "vbfore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _VBForeClass properties

/////////////////////////////////////////////////////////////////////////////
// _VBForeClass operations

void _VBForeClass::VBForeOpenDatabase(BSTR* sDatabaseName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sDatabaseName);
}

void _VBForeClass::VBForeOpenTable(BSTR* sTableName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sTableName);
}

void _VBForeClass::VBForeRunQueryDef(BSTR* sQueryName)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sQueryName);
}

void _VBForeClass::VBForeRunQuery(BSTR* sQuery)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sQuery);
}

CString _VBForeClass::VBForeQuery()
{
	CString result;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _VBForeClass::VBForeWindowState(short* Value)
{
	static BYTE parms[] =
		VTS_PI2;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

void _VBForeClass::VBForeWindowVisible(BOOL* Value)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

void _VBForeClass::VBForeWindowFocus()
{
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _VBForeClass::VBForeClose()
{
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
