#pragma once
#include "stdafx.h"

using namespace std;

class MemoryFile
{

private:
	HANDLE hFile;
	HANDLE hFileMapping;

	const size_t nMaxPoolSize = 1 << sizeof(int) * 8;
	const size_t nBytesToMap = 1 << sizeof(int) * 8;

	size_t positionInFile;
	size_t nNumberOfBytesToMap;

	int* szMap;

public:
	struct exCreateFile {};
	struct exCreateFileMapping {};
	struct exAddStringFault {};
	MemoryFile(CString fileName);
	~MemoryFile();

	friend MemoryFile& operator << (MemoryFile&, const int);
	friend MemoryFile& operator >> (MemoryFile&, int&);
	void addNumber(int number);
	void addString(wchar_t* str);
	void setPosition(int number);
	wchar_t getChar(int index);
};
