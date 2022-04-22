// monitor thread function
UINT LaunchAndWait(LPVOID pParam);

// helper functions
BOOL LaunchApplication(LPCTSTR pCmdLine, PROCESS_INFORMATION* pProcessInfo);
void AddToList(HWND hWnd, LPCTSTR pCmdLine, DWORD hThread);
void RemoveFromList(HWND hWnd, HANDLE hThread);
