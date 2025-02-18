// DateTime.h: interface for the CDateTime class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _DATE_TIME_H
#define _DATE_TIME_H


#include <windows.h>
#include <string>


// Time related constants
#define _SECOND ((INT64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR) 


class CDateTime  
{
private:
	SYSTEMTIME systemTime;

public:
	CDateTime ();
	CDateTime (SYSTEMTIME &timeStruct);
	CDateTime (char *cDateTimeString);
	CDateTime(const std::string& cDateTime);
		
	ULONG64 getTicks();
	bool refresh ();
	bool addSeconds (unsigned int iSeconds);
	bool addDays (int iDays);
	bool getWeekDay (char *cBuffer);
	bool toString (char *cBuffer);
	bool toString(std::string& buffer);
	bool toStringFull( char *buffer );
	bool toStringFull(std::string& buffer);
	bool toGMTString (char *cBuffer);
	bool toGMTString(std::string& buffer);
	bool toDateString (char *cBuffer);
	bool toDateString(std::string& buffer);
	bool toTimeString (char *cBuffer);
	bool toTimeString(std::string& buffer);
	bool toFormattedString (char *cBuffer);
	bool toFormattedString(std::string& buffer);
	SYSTEMTIME getSystemTime(void);
	virtual ~CDateTime();
};

#endif
