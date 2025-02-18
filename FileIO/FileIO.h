#ifndef _FILE_IO_H_
#define _FILE_IO_H_
//#pragma once
#include <stdarg.h>
#include <windows.h>
#include <vector>
#include <string>
#include "Exception.h"
#include "globals.h"
using namespace std;

#define MAX_FILE_LENGTH		9999999

class CFileIO
{
public:
	CFileIO(char *cFileName, int iFlag=CFileIO::FLAG_OPEN_EXISTING);
	virtual ~CFileIO(void);
	// Parameter File R/W operations
	int WriteParamLine (const char* cFormat, ...);	// appends a new line to the file
	int ReadParamLine (vector <string> &vecBuffer, UINT maxSize);				// reads a line from the file (in the argument style)
	int ReadFullLine (string &sOut);				// reads a line from the file (in the argument style)
	int ReadDelimitedLine (vector <string> &vecBuffer, char *cDelimitedStr, UINT maxSize);	// reads the delimited parameters from the line
	int ReadParamLine (char **Buffer, UINT maxSize);				// reads a line from the file (in the argument style)
	int ReadDelimitedLine (char **Buffer, char *cDelimitedStr, UINT maxSize);	// reads the delimited parameters from the line


	static const int FLAG_CREATE_NEW;
	static const int FLAG_CREATE_ALWAYS;
	static const int FLAG_OPEN_EXISTING;
	static const int FLAG_OPEN_ALWAYS;
	static const int FLAG_TRUNCATE_EXISTING;

protected:
	char cFileName [MAX_STRING_LENGTH];
	HANDLE hFile;
	HANDLE hMutex;
	__int64 FilePos;
	__int64 FileSeek (__int64 distance, DWORD MoveMethod);
	long ReadLine (char *cBuffer);
	long WriteLine (char *cBuffer, int iBufferLen);
};

#endif