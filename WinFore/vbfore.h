// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _VBForeClass wrapper class

class _VBForeClass : public COleDispatchDriver
{
public:
	_VBForeClass() {}		// Calls COleDispatchDriver default constructor
	_VBForeClass(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_VBForeClass(const _VBForeClass& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void VBForeOpenDatabase(BSTR* sDatabaseName);
	void VBForeOpenTable(BSTR* sTableName);
	void VBForeRunQueryDef(BSTR* sQueryName);
	void VBForeRunQuery(BSTR* sQuery);
	CString VBForeQuery();
	void VBForeWindowState(short* Value);
	void VBForeWindowVisible(BOOL* Value);
	void VBForeWindowFocus();
	void VBForeClose();
};
