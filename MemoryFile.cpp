#pragma once
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include"MemoryFile.h"

MemoryFile::MemoryFile(CString fileName)
{
	hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS  /*CREATE_ALWAYS*/, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		throw new exCreateFile;

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, NULL, nMaxPoolSize, NULL);

	if (!hFileMapping)
		throw new exCreateFileMapping;

	szMap = (int*)MapViewOfFile(hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, NULL, NULL, nBytesToMap);

	if (szMap == NULL)
		throw new exCreateFileMapping;
	positionInFile = 0;
}

MemoryFile::~MemoryFile()
{
	UnmapViewOfFile(szMap);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
}

void MemoryFile::addNumber(int number)
{
	szMap[positionInFile++] = (int)number;
}


int MemoryFile::getSize()
{
	return positionInFile;
}


void MemoryFile::setPosition(int number)
{
	positionInFile = number;
}

wchar_t MemoryFile::getChar(int index)
{
	return szMap[index];
}

MemoryFile& operator << (MemoryFile& memMap, const int number)
{
	memMap.addNumber(number);
	return memMap;
}

MemoryFile& operator >> (MemoryFile& memMap, int& number)
{
	number = memMap.szMap[memMap.positionInFile++];
	return memMap;
}