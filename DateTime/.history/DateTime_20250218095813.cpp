// DateTime.cpp: implementation of the CDateTime class.
//
//////////////////////////////////////////////////////////////////////

#include "DateTime.h"
#include <stdio.h>
#include <string.h>


#define DT_STRING_LEN	256



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDateTime::CDateTime()
{
	GetLocalTime (&systemTime);
}


CDateTime::CDateTime (SYSTEMTIME &timeStruct)
{
	this->systemTime = timeStruct;
}


CDateTime::CDateTime (char *cDateTime)
{
	// define variables to hold part of date and time
	char cYear [DT_STRING_LEN]={0};	
	char cMonth[DT_STRING_LEN]={0};
	char cDate [DT_STRING_LEN]={0};
	char cHour [DT_STRING_LEN]={0};
	char cMinute [DT_STRING_LEN]={0};
	char cSecond [DT_STRING_LEN]={0};
	/*memset (cYear, 0, DT_STRING_LEN);
	memset (cMonth, 0, DT_STRING_LEN);
	memset (cDate, 0, DT_STRING_LEN);
	memset (cHour, 0, DT_STRING_LEN);
	memset (cMinute, 0, DT_STRING_LEN);
	memset (cSecond, 0, DT_STRING_LEN);*/

	// initialize variables with the passed date and time string
	memcpy (cYear, cDateTime, 4);
	memcpy (cMonth, cDateTime+4, 2);
	memcpy (cDate, cDateTime+6, 2);
	memcpy (cHour, cDateTime+8, 2);
	memcpy (cMinute, cDateTime+10, 2);
	memcpy (cSecond, cDateTime+12, 2);

	// set system time structure
	SYSTEMTIME timeStruct;
	timeStruct.wYear = atoi (cYear);
	timeStruct.wMonth = atoi (cMonth);
	timeStruct.wDay = atoi (cDate);
	timeStruct.wHour = atoi (cHour);
	timeStruct.wMinute = atoi (cMinute);
	timeStruct.wSecond = atoi (cSecond);
	timeStruct.wMilliseconds = 0;
	timeStruct.wDayOfWeek = 0;

	this->systemTime = timeStruct;
}

CDateTime::~CDateTime()
{

}

SYSTEMTIME CDateTime::getSystemTime(void)
{
	return systemTime;
}

ULONG64 CDateTime::getTicks()
{
	FILETIME fileTime;
	SystemTimeToFileTime (&systemTime, &fileTime);
	ULARGE_INTEGER uLargeInteger;
	memcpy (&uLargeInteger, &fileTime, sizeof (ULARGE_INTEGER));
	return uLargeInteger.QuadPart;
}


bool CDateTime::refresh ()
{
	GetLocalTime (&systemTime);
	return true;
}


bool CDateTime::addSeconds (unsigned int iSeconds)
{
	FILETIME fileTime;
	if (SystemTimeToFileTime (&systemTime, &fileTime) == FALSE)
		return false;

	ULARGE_INTEGER uLargeInteger;
	memcpy (&uLargeInteger, &fileTime, sizeof (ULARGE_INTEGER));
	uLargeInteger.QuadPart += (iSeconds * _SECOND);

	memcpy (&fileTime, &uLargeInteger, sizeof (ULARGE_INTEGER));
	FileTimeToSystemTime (&fileTime, &systemTime);
	return true;
}


bool CDateTime::addDays (int iDays)
{
	unsigned int iSeconds = (iDays) * (_DAY) / _SECOND;
	return addSeconds (iSeconds);
}


bool CDateTime::getWeekDay (char *cBuffer)
{
	if (GetDateFormat (0, 0, &systemTime, "dddd", cBuffer, DT_STRING_LEN) > 0)
		return true;
	else
		return false;
}


bool CDateTime::toString (char *buffer)
{
	sprintf (buffer, "%04d%02d%02d%02d%02d%02d", 
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	return true;
}

bool CDateTime::toStringFull( char *buffer )
{
	sprintf( buffer, "%04d%02d%02d%02d%02d%02d%03d",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds );

	return true;
}

bool CDateTime::toFormattedString (char *buffer)
{
	char cReceiveTime[50];

	toString (cReceiveTime);

	// format receive time
	strcpy (buffer, "    /  /     :  :");	// yyyy/mm/dd hh:mm s
	memcpy (buffer, cReceiveTime, 4);		// copy year
	memcpy (buffer+5, cReceiveTime+4, 2);	// copy month
	memcpy (buffer+8, cReceiveTime+6, 2);	// copy day
	memcpy (buffer+11, cReceiveTime+8, 2);	// copy hours
	memcpy (buffer+14, cReceiveTime+10, 2);
	memcpy (buffer+17, cReceiveTime+12, 2);
	buffer[19]= '\0';
	return true;
}

bool CDateTime::toGMTString (char *buffer)
{
	SYSTEMTIME universalTime;
	GetSystemTime (&universalTime);
	sprintf (buffer, "%04d%02d%02d%02d%02d%02d", 
		universalTime.wYear, universalTime.wMonth, universalTime.wDay,
		universalTime.wHour, universalTime.wMinute, universalTime.wSecond);
	return true;
}


bool CDateTime::toDateString (char *buffer)
{
	sprintf (buffer, "%04d%02d%02d", 
		systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	return true;
}

bool CDateTime::toTimeString (char *buffer)
{
	sprintf (buffer, "%02d%02d%02d", 
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	return true;
}