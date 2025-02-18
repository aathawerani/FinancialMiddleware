#include "fileio.h"
#include "DBSmartConnection.h"
/**
* static constants
*/
const int CFileIO::FLAG_CREATE_NEW = CREATE_NEW;
const int CFileIO::FLAG_CREATE_ALWAYS = CREATE_ALWAYS;
const int CFileIO::FLAG_OPEN_EXISTING = OPEN_EXISTING;
const int CFileIO::FLAG_OPEN_ALWAYS = OPEN_ALWAYS;
const int CFileIO::FLAG_TRUNCATE_EXISTING = TRUNCATE_EXISTING;

CFileIO::CFileIO(char *cFileName, int iFlag)
{
	char cMutex[MAX_STRING_LENGTH];
	char cTempFileName[MAX_STRING_LENGTH];

	FilePos = 0;
	strcpy (this->cFileName, cFileName);

	// replace all slashes from the filename because slashes cannot be used in the mutex strings
	strcpy (cTempFileName, cFileName);
	for (unsigned int i=0;i < strlen(cTempFileName); i++)
	{
		if (cTempFileName[i]=='\\')
			cTempFileName[i]='-';
	}
	sprintf (cMutex, "MutexForFile: %s", cTempFileName);
	hMutex = CreateMutex (NULL, FALSE, cMutex);
	if (hMutex == NULL)
		throw CRdvException ("CFileIO :: CreateMutex for [%s] failed", cFileName);

	if (iFlag == FLAG_CREATE_NEW || iFlag == FLAG_CREATE_ALWAYS)
	{
		hFile = CreateFile (cFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL,
													iFlag, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			throw CRdvException ("CFileIO :: Unable to create file [%s]", cFileName);

		FilePos = FileSeek (NULL, FILE_BEGIN);
	}
	else
	{
		hFile = CreateFile (cFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL,
													iFlag, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			throw CRdvException ("CFileIO :: Unable to open file [%s]", cFileName);

		FilePos = FileSeek (NULL, FILE_BEGIN);
	}
}

CFileIO::~CFileIO(void)
{
	CloseHandle (hFile);
	CloseHandle (hMutex);
}

// Private function used to set file pointer
__int64 CFileIO::FileSeek (__int64 distance, DWORD MoveMethod)
{
   LARGE_INTEGER li;

   li.QuadPart = distance;
   li.LowPart = SetFilePointer (hFile, li.LowPart, &li.HighPart, MoveMethod);
   if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
      li.QuadPart = -1;

   return li.QuadPart;
}

// Private function to write contents to a file
long CFileIO::WriteLine (char *cBuffer, int iBufferLen)
{
	unsigned long iBytesWritten;

	if (hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}

	WaitForSingleObject (hMutex, INFINITE);
	try
	{
		if (WriteFile (hFile, cBuffer, iBufferLen, &iBytesWritten, NULL)==FALSE) {
			ReleaseMutex (hMutex);
			return -1;
		}
	}
	catch(...)
	{
		ReleaseMutex (hMutex);
		throw;
	}
	ReleaseMutex (hMutex);
	return iBytesWritten;
}

// Private function to read a line from file
long CFileIO::ReadLine (char *cBuffer)
{
	unsigned long iBytesRead;

	if (hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}

	WaitForSingleObject (hMutex, INFINITE);
	try
	{
		if (ReadFile (hFile, cBuffer, 1000, &iBytesRead, NULL)==FALSE) {
			ReleaseMutex (hMutex);
			return -1;
		}
	}
	catch(...)
	{
		ReleaseMutex (hMutex);
		throw;
	}
	ReleaseMutex (hMutex);
	return iBytesRead;
}

// Public function to write a formatted string to a file
int CFileIO::WriteParamLine (const char* cFormat, ...)
{
	static char tempStr[MAX_FILE_LENGTH];
	static char logCharString[MAX_FILE_LENGTH];

	va_list arglist;
	va_start(arglist, cFormat);
    vsprintf(logCharString, cFormat, arglist);
	va_end(arglist);

	sprintf (tempStr, "%s\r\n", logCharString);
	return WriteLine (tempStr, strlen(tempStr));
}

// Public function to read a delimited line
int CFileIO::ReadDelimitedLine (char **cBuffer, char *cDelimitedStr, UINT maxSize)
{
	char cLineBrkChars[]  = {'\n', '\r', '\0'};
	char *cParamBrkChars = cDelimitedStr;

	auto_ptr_array<char> atempStr(maxSize);
	long iBytesRead;

	if (FilePos == FileSeek (NULL, FILE_END))	// File Processed
		return 0;

	// Seek to the last read location
	FileSeek (FilePos, FILE_BEGIN);

	// Read File
	iBytesRead = ReadLine (atempStr.get());
	if (iBytesRead <= 0)
		return 0;

	atempStr[iBytesRead] = 0;

	char *cPos=NULL, *cParamString=atempStr.get();

	// find a line break charater
	while (cPos = strpbrk (cParamString, cLineBrkChars))
	{
		if (cPos - cParamString == 0)
		{
			cParamString++;
			FilePos++;
			continue;
		}
		else
			break;
	}
	if (cPos == NULL) {
		cPos = cParamString + strlen(cParamString);
	}

	// Save the File Position
	FilePos = FilePos + (cPos-cParamString);
	cParamString[cPos-cParamString] = 0;

	// Check for Comment line i.e. #
	if (cParamString[0] == '#')
		return -1;

	// remove spaces on the right side
	for (int j=strlen(cParamString)-1; j > 0, cParamString[j] == ' '; j--)
		cParamString[j] = 0;

	cPos = NULL;
	int iParamsRead = 0;

	// Search parameters in the line
	while (cPos = strpbrk (cParamString, cParamBrkChars))
	{
		if (cPos - cParamString == 0)
		{
			cParamString++;
			continue;
		}

		strncpy (cBuffer[iParamsRead], cParamString, cPos-cParamString);
		cBuffer[iParamsRead++][cPos-cParamString]=0;
		cParamString = cPos + 1;
	}
	// copy the last parameter
	strcpy (cBuffer[iParamsRead], cParamString);

	return iParamsRead+1;
}

// Public function to read tab or space separated values.
int CFileIO::ReadParamLine (char **cBuffer, UINT maxSize)
{
	char cParamBrkChars[] = {'\t', ' ', '\0'};
	return ReadDelimitedLine (cBuffer, cParamBrkChars,maxSize);
}

// Public function to read a delimited line (vector edition)
int CFileIO::ReadDelimitedLine (vector <string> &vecBuffer, char *cDelimitedStr, UINT maxSize)
{
	char cLineBrkChars[]  = {'\n', '\r', '\0'};
	char *cParamBrkChars = cDelimitedStr;

	auto_ptr_array<char> aTempStr(maxSize);
	long iBytesRead;

	if (FilePos == FileSeek (NULL, FILE_END))	// File Processed
		return 0;

	// Seek to the last read location
	FileSeek (FilePos, FILE_BEGIN);

	// Read File
	iBytesRead = ReadLine (aTempStr.get());
	if (iBytesRead <= 0)
		return 0;

	aTempStr[iBytesRead] = 0;

	char *cPos=NULL, *cParamString=aTempStr.get();

	// find a line break charater
	while (cPos = strpbrk (cParamString, cLineBrkChars))
	{
		if (cPos - cParamString == 0)
		{
			cParamString++;
			FilePos++;
			continue;
		}
		else
			break;
	}
	if (cPos == NULL) {
		cPos = cParamString + strlen(cParamString);
	}

	// Save the File Position
	FilePos = FilePos + (cPos-cParamString);
	cParamString[cPos-cParamString] = 0;

	// Check for Comment line i.e. #
	if (cParamString[0] == '#')
		return -1;

	// remove spaces on the right side
	for (int j=strlen(cParamString)-1; j > 0, cParamString[j] == ' '; j--)
		cParamString[j] = 0;

	cPos = NULL;
	int iParamsRead = 0;

	// Search parameters in the line
	while (cPos = strpbrk (cParamString, cParamBrkChars))
	{
		if (cPos - cParamString == 0)
		{
			cParamString++;
			continue;
		}

		strncpy (aTempStr.get(), cParamString, cPos-cParamString);
		aTempStr[cPos-cParamString]=0;
		vecBuffer.push_back (aTempStr.get());
		cParamString = cPos + 1;
	}

	// copy the last parameter
	strcpy (aTempStr.get(), cParamString);
	vecBuffer.push_back(aTempStr.get());

	return vecBuffer.size();
}

// Public function to read tab or space separated values (vector edition)
int CFileIO::ReadParamLine (vector <string> &vecBuffer, UINT maxSize)
{
	char cParamBrkChars[] = {'\t', ' ', '\0'};
	return ReadDelimitedLine (vecBuffer, cParamBrkChars,maxSize);
}
int CFileIO::ReadFullLine (string &sOut)
{
	vector <string> vecBuffer;
	char cParamBrkChars[] = {'\r', '\n', '\0'};
	int rc = ReadDelimitedLine (vecBuffer, cParamBrkChars, MAX_STRING_LENGTH * 4);

	if(vecBuffer.empty() == false)
		sOut = vecBuffer[0];

	return rc;
}
