#include <string>
using namespace std;

class CDirectory
{
private:
	static bool splitOnChar (const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim);

public:
	CDirectory(void);

	// listing methods
	static bool doesFolderExist (const char *cFolderName);
	static bool doesFileExist (const char *cFilename);

	// manipulation methods
	static bool createFolder (const char *cFolderName);
	static bool moveFile (const char *cDestFolder, const char *cSrcFolder, const char *cFilename);

	// utility methods
	static string getTempPath ();
	static bool breakQualifiedName (const char *cQualifiedName, char *cPath, char *cFilename);
	static bool breakFilename (const char *cFilenameWithExt, char *cFilename, char *cExtension);

	virtual ~CDirectory(void);
};

#pragma once
#include <string>
using namespace std;

class CDirectory
{
private:
	static bool splitOnChar (const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim);

public:
	CDirectory(void);

	// listing methods
	static bool doesFolderExist (const char *cFolderName);
	static bool doesFileExist (const char *cFilename);

	// manipulation methods
	static bool createFolder (const char *cFolderName);
	static bool moveFile (const char *cDestFolder, const char *cSrcFolder, const char *cFilename);

	// utility methods
	static string getTempPath ();
	static bool breakQualifiedName (const char *cQualifiedName, char *cPath, char *cFilename);
	static bool breakFilename (const char *cFilenameWithExt, char *cFilename, char *cExtension);

	virtual ~CDirectory(void);
};

#pragma once
#include <string>
using namespace std;

class CDirectory
{
private:
	static bool splitOnChar (const char *cSource, const char delim, char *cPartBeforeDelim, char *cPartAfterDelim);

public:
	CDirectory(void);

	// listing methods
	static bool doesFolderExist (const char *cFolderName);
	static bool doesFileExist (const char *cFilename);

	// manipulation methods
	static bool createFolder (const char *cFolderName);
	static bool moveFile (const char *cDestFolder, const char *cSrcFolder, const char *cFilename);

	// utility methods
	static string getTempPath ();
	static bool breakQualifiedName (const char *cQualifiedName, char *cPath, char *cFilename);
	static bool breakFilename (const char *cFilenameWithExt, char *cFilename, char *cExtension);

	virtual ~CDirectory(void);
};
