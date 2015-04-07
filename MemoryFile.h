#pragma once
#include "stdafx.h"
#include "resource.h"

using namespace std;

class MemoryFile
{

private:
	HANDLE hFile;
	HANDLE hFileMapping;

	const size_t nMaxPoolSize = MAX_POOL_SIZE;
	const size_t nBytesToMap = 1 << sizeof(int) * 8;

	size_t positionInFile;
	size_t nNumberOfBytesToMap;

public:
	int* szMap;
	struct exCreateFile {};
	struct exCreateFileMapping {};
	struct exAddStringFault {};
	MemoryFile(CString fileName);
	~MemoryFile();

	friend MemoryFile& operator << (MemoryFile&, const int);
	friend MemoryFile& operator >> (MemoryFile&, int&);
	int getSize();
	void addNumber(int number);
	void addString(wchar_t* str);
	void setPosition(int number);
	wchar_t getChar(int index);
};
