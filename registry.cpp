#include "stdafx.h"
#include "registry.h"
#include "Resource.h"

Registry::Registry() {
	hKey = NULL;
}

Registry::~Registry(){
	CloseKey();
}

void Registry::CloseKey() {
	if (hKey != NULL) {
		RegCloseKey(hKey);
	}
}

bool Registry::CreateKey(const TCHAR* pszSubKey) {
	CloseKey();

	LONG lRes = RegCreateKeyEx(HKEY_CURRENT_USER, pszSubKey, NULL, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, NULL);

	if (lRes != ERROR_SUCCESS) {
		return false;
	}

	return true;
}

bool Registry::OpenKey(const TCHAR* pszSubKey) {
	CloseKey();

	LONG lRes = RegOpenKeyEx(HKEY_CURRENT_USER, pszSubKey, 0, KEY_ALL_ACCESS, &hKey);

	if (lRes != ERROR_SUCCESS) {
		return false;
	}

	return true;
}

bool Registry::SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, int data){
	LONG lRes = RegSetKeyValue(hKey, pszSubKey, pszValueName, REG_DWORD, &data, sizeof(int));

	if (lRes != ERROR_SUCCESS){
		return false;
	}

	return true;
}

bool Registry::SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, TCHAR* data){
	LONG lRes = RegSetKeyValue(hKey, pszSubKey, pszValueName, REG_SZ, data, sizeof(TCHAR)* (_tcslen(data) + 1));

	if (lRes != ERROR_SUCCESS){
		return false;
	}

	return true;
}

bool Registry::SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, __int64 data){
	LONG lRes = RegSetKeyValue(hKey, pszSubKey, pszValueName, REG_QWORD, (BYTE*)&data, sizeof(double));

	if (lRes != ERROR_SUCCESS){
		return false;
	}

	return true;
}

bool Registry::SetKeyValue(const TCHAR* pszSubKey, const TCHAR* pszValueName, LPBYTE lpbData){
	LONG lRes = RegSetKeyValue(hKey, pszSubKey, pszValueName, REG_BINARY, lpbData, sizeof(char));

	if (lRes != ERROR_SUCCESS){
		return false;
	}

	return true;
}

int Registry::GetValue(const TCHAR* pszValueName) {

	DWORD dwType = REG_DWORD;
	DWORD dwBufSize = sizeof(DWORD);
	DWORD data = 0;

	LONG lRes = RegQueryValueEx(hKey, pszValueName, NULL, &dwType, (LPBYTE)&data, &dwBufSize);

	if (lRes != ERROR_SUCCESS) {
		return -1;
	}

	return data;
}

CString Registry::GetStrValue(const TCHAR* pszValueName, int bufferSize) {

	DWORD dwType = REG_SZ;
	TCHAR data[FONT_NAME_BUFFER];
	DWORD dwBufSize = bufferSize * sizeof(WCHAR);

	LONG lRes = RegQueryValueEx(hKey, pszValueName, NULL, &dwType, (LPBYTE)data, &dwBufSize);
	CString string(data);

	if (lRes != ERROR_SUCCESS) {
		return 0;
	}

	return data;
}