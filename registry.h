#pragma once

//#include <Windows.h>
#include <iostream>
#include <tchar.h>

using namespace std;

struct exRegistryFaults {};

class Registry{
	HKEY hKey;

	const DWORD cdwMaxKeyLen = 100;

	void CloseKey();

public:
	Registry();
	~Registry();

	bool CreateKey(const TCHAR* pszSubKey);
	bool OpenKey(const TCHAR* pszSubKey);

	bool SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, int data);
	bool SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, TCHAR* data);
	bool SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, __int64 data);
	bool SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, LPBYTE lpbData);

	int GetValue(const TCHAR* pszValueName);
	CString GetStrValue(const TCHAR* pszValueName, int bufferSize);
};