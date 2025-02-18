#include "directory.h"
#include <windows.h>

CDirectory::CDirectory(void)
{
}

CDirectory::~CDirectory(void)
{
}


/**
* Does Folder Exist?
* Checks whether the given folder exists in the file system or not
* Parameter: 
*	cDirName	fully resolved directory name
*/
bool CDirectory::doesFolderExist (const char *cFolderName)
{
	WIN32_FIND_DATA	fileInfo;
	HANDLE hFolder = FindFirstFile(cFolderName, &fileInfo);

	// if the file exists and it is a directory
	if(fileInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		// directory exists close file and return
		FindClose (hFolder);
		return true;
	}

	return false;
}


/**
* Does File Exist?
* Checks whether the given file exists in the file system or not
* Parameter
*	cFilename	fully resolved filename
*/
bool CDirectory::doesFileExist (const char *cFilename)
{
	HANDLE hDir = CreateFile (cFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDir == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastError = GetLastError ();
		if (dwLastError == ERROR_ALREADY_EXISTS)
		{
			return true;
		}
	}

	DeleteFile (cFilename);
	return false;
}


/**
* Create Folder
* Given folder name is created
* Returns false if failed to create the folder for any reason
*/
bool CDirectory::createFolder (const char *cFolderName)
{
	if (CreateDirectory (cFolderName, NULL) == FALSE) return false;
	else return true;
}


/**
* Get Temp Path
* Returns the path to tempoarary folder
*/
string CDirectory::getTempPath ()
{
	char cBuffer [MAX_PATH];
	DWORD iSize = MAX_PATH;
	string sPath = "";
	if (GetTempPath (iSize, cBuffer) > 0)
	{
		sPath = cBuffer;
	}
	return sPath;
}

/**
* Break Filename
* Breaks a complete filename into filename and extension
*/
bool CDirectory::breakFilename (const char *cFilenameWithExt, char *cFilename, char *cExtension)
{
	return splitOnChar (cFilenameWithExt, '.', cFilename, cExtension);
}



/**
* Split On Character
* Breaks a string based on the split character passed (finds in reverse order)
*/
bool CDirectory::splitOnChar (const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim)
{
	int i;
	for (i=strlen(cSource)-1; i>0; i--)
	{
		if (cSource[i] == delim) break;
	}

	if (i > 0) 
	{
		strncpy (cPartBeforeDelim, cSource, i);
		cPartBeforeDelim[i] = 0;
		strcpy (cPartAfterDelim, cSource+i+1);
	}
	else
	{
		strcpy (cPartBeforeDelim, cSource);
		strcpy (cPartAfterDelim, "");
	}

	return true;
}



/**
* Break Qualified Name
* Breaks a fully qualified name into path and filename
*/
bool CDirectory::breakQualifiedName (const char *cQualifiedName, char *cPath, char *cFilename)
{
	if (splitOnChar (cQualifiedName, '\\', cPath, cFilename) == false) return false;

	// if the character is not found, swap filename and path
	if (strcmp (cFilename, "") == 0) {
		strcpy (cFilename, cPath);
		strcpy (cPath, "");
	}

	return true;
}


/**
* Move File
* Moves given file from the source folder to destination folder
* Deletes destination file if it already exists
*/
bool CDirectory::moveFile (const char *cDestFolder, const char *cSrcFolder, const char *cFilename)
{
	char cSrcFilename [MAX_PATH];
	char cDestFilename [MAX_PATH];

	// form qualified source filename
	strcpy (cSrcFilename, cSrcFolder);
	strcat (cSrcFilename, "\\");
	strcat (cSrcFilename, cFilename);

	// for qualified destination filename
	strcpy (cDestFilename, cDestFolder);
	strcat (cDestFilename, "\\");
	strcat (cDestFilename, cFilename);

	// move file
	DeleteFile (cDestFilename);
	if (MoveFile (cSrcFilename, cDestFilename) == TRUE)
		return true;
	else
		return false;
}

#include "directory.h"
#include <windows.h>

CDirectory::CDirectory(void)
{
}

CDirectory::~CDirectory(void)
{
}


/**
* Does Folder Exist?
* Checks whether the given folder exists in the file system or not
* Parameter: 
*	cDirName	fully resolved directory name
*/
bool CDirectory::doesFolderExist (const char *cFolderName)
{
	WIN32_FIND_DATA	fileInfo;
	HANDLE hFolder = FindFirstFile(cFolderName, &fileInfo);

	// if the file exists and it is a directory
	if(fileInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		// directory exists close file and return
		FindClose (hFolder);
		return true;
	}

	return false;
}


/**
* Does File Exist?
* Checks whether the given file exists in the file system or not
* Parameter
*	cFilename	fully resolved filename
*/
bool CDirectory::doesFileExist (const char *cFilename)
{
	HANDLE hDir = CreateFile (cFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDir == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastError = GetLastError ();
		if (dwLastError == ERROR_ALREADY_EXISTS)
		{
			return true;
		}
	}

	DeleteFile (cFilename);
	return false;
}


/**
* Create Folder
* Given folder name is created
* Returns false if failed to create the folder for any reason
*/
bool CDirectory::createFolder (const char *cFolderName)
{
	if (CreateDirectory (cFolderName, NULL) == FALSE) return false;
	else return true;
}


/**
* Get Temp Path
* Returns the path to tempoarary folder
*/
string CDirectory::getTempPath ()
{
	char cBuffer [MAX_PATH];
	DWORD iSize = MAX_PATH;
	string sPath = "";
	if (GetTempPath (iSize, cBuffer) > 0)
	{
		sPath = cBuffer;
	}
	return sPath;
}

/**
* Break Filename
* Breaks a complete filename into filename and extension
*/
bool CDirectory::breakFilename (const char *cFilenameWithExt, char *cFilename, char *cExtension)
{
	return splitOnChar (cFilenameWithExt, '.', cFilename, cExtension);
}



/**
* Split On Character
* Breaks a string based on the split character passed (finds in reverse order)
*/
bool CDirectory::splitOnChar (const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim)
{
	int i;
	for (i=strlen(cSource)-1; i>0; i--)
	{
		if (cSource[i] == delim) break;
	}

	if (i > 0) 
	{
		strncpy (cPartBeforeDelim, cSource, i);
		cPartBeforeDelim[i] = 0;
		strcpy (cPartAfterDelim, cSource+i+1);
	}
	else
	{
		strcpy (cPartBeforeDelim, cSource);
		strcpy (cPartAfterDelim, "");
	}

	return true;
}



/**
* Break Qualified Name
* Breaks a fully qualified name into path and filename
*/
bool CDirectory::breakQualifiedName (const char *cQualifiedName, char *cPath, char *cFilename)
{
	if (splitOnChar (cQualifiedName, '\\', cPath, cFilename) == false) return false;

	// if the character is not found, swap filename and path
	if (strcmp (cFilename, "") == 0) {
		strcpy (cFilename, cPath);
		strcpy (cPath, "");
	}

	return true;
}


/**
* Move File
* Moves given file from the source folder to destination folder
* Deletes destination file if it already exists
*/
bool CDirectory::moveFile (const char *cDestFolder, const char *cSrcFolder, const char *cFilename)
{
	char cSrcFilename [MAX_PATH];
	char cDestFilename [MAX_PATH];

	// form qualified source filename
	strcpy (cSrcFilename, cSrcFolder);
	strcat (cSrcFilename, "\\");
	strcat (cSrcFilename, cFilename);

	// for qualified destination filename
	strcpy (cDestFilename, cDestFolder);
	strcat (cDestFilename, "\\");
	strcat (cDestFilename, cFilename);

	// move file
	DeleteFile (cDestFilename);
	if (MoveFile (cSrcFilename, cDestFilename) == TRUE)
		return true;
	else
		return false;
}

#include "directory.h"
#include <windows.h>

CDirectory::CDirectory(void)
{
}

CDirectory::~CDirectory(void)
{
}


/**
* Does Folder Exist?
* Checks whether the given folder exists in the file system or not
* Parameter: 
*	cDirName	fully resolved directory name
*/
bool CDirectory::doesFolderExist (const char *cFolderName)
{
	WIN32_FIND_DATA	fileInfo;
	HANDLE hFolder = FindFirstFile(cFolderName, &fileInfo);

	// if the file exists and it is a directory
	if(fileInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		// directory exists close file and return
		FindClose (hFolder);
		return true;
	}

	return false;
}


/**
* Does File Exist?
* Checks whether the given file exists in the file system or not
* Parameter
*	cFilename	fully resolved filename
*/
bool CDirectory::doesFileExist (const char *cFilename)
{
	HANDLE hDir = CreateFile (cFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDir == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastError = GetLastError ();
		if (dwLastError == ERROR_ALREADY_EXISTS)
		{
			return true;
		}
	}

	DeleteFile (cFilename);
	return false;
}


/**
* Create Folder
* Given folder name is created
* Returns false if failed to create the folder for any reason
*/
bool CDirectory::createFolder (const char *cFolderName)
{
	if (CreateDirectory (cFolderName, NULL) == FALSE) return false;
	else return true;
}


/**
* Get Temp Path
* Returns the path to tempoarary folder
*/
string CDirectory::getTempPath ()
{
	char cBuffer [MAX_PATH];
	DWORD iSize = MAX_PATH;
	string sPath = "";
	if (GetTempPath (iSize, cBuffer) > 0)
	{
		sPath = cBuffer;
	}
	return sPath;
}

/**
* Break Filename
* Breaks a complete filename into filename and extension
*/
bool CDirectory::breakFilename (const char *cFilenameWithExt, char *cFilename, char *cExtension)
{
	return splitOnChar (cFilenameWithExt, '.', cFilename, cExtension);
}



/**
* Split On Character
* Breaks a string based on the split character passed (finds in reverse order)
*/
bool CDirectory::splitOnChar (const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim)
{
	int i;
	for (i=strlen(cSource)-1; i>0; i--)
	{
		if (cSource[i] == delim) break;
	}

	if (i > 0) 
	{
		strncpy (cPartBeforeDelim, cSource, i);
		cPartBeforeDelim[i] = 0;
		strcpy (cPartAfterDelim, cSource+i+1);
	}
	else
	{
		strcpy (cPartBeforeDelim, cSource);
		strcpy (cPartAfterDelim, "");
	}

	return true;
}



/**
* Break Qualified Name
* Breaks a fully qualified name into path and filename
*/
bool CDirectory::breakQualifiedName (const char *cQualifiedName, char *cPath, char *cFilename)
{
	if (splitOnChar (cQualifiedName, '\\', cPath, cFilename) == false) return false;

	// if the character is not found, swap filename and path
	if (strcmp (cFilename, "") == 0) {
		strcpy (cFilename, cPath);
		strcpy (cPath, "");
	}

	return true;
}


/**
* Move File
* Moves given file from the source folder to destination folder
* Deletes destination file if it already exists
*/
bool CDirectory::moveFile (const char *cDestFolder, const char *cSrcFolder, const char *cFilename)
{
	char cSrcFilename [MAX_PATH];
	char cDestFilename [MAX_PATH];

	// form qualified source filename
	strcpy (cSrcFilename, cSrcFolder);
	strcat (cSrcFilename, "\\");
	strcat (cSrcFilename, cFilename);

	// for qualified destination filename
	strcpy (cDestFilename, cDestFolder);
	strcat (cDestFilename, "\\");
	strcat (cDestFilename, cFilename);

	// move file
	DeleteFile (cDestFilename);
	if (MoveFile (cSrcFilename, cDestFilename) == TRUE)
		return true;
	else
		return false;
}