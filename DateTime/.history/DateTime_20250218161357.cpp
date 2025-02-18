// DateTime.cpp: implementation of the CDateTime class.
//
//////////////////////////////////////////////////////////////////////

#include "DateTime.h"
#include <stdio.h>
#include <string.h>
#include <array>
#include <string>
#include <cstring>


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


CDateTime::CDateTime (const std::string& cDateTime)
{
	// define variables to hold part of date and time
	std::array<char, DT_STRING_LEN> cYear = {0};	
	std::array<char, DT_STRING_LEN> cMonth = {0};
	std::array<char, DT_STRING_LEN> cDate = {0};
	std::array<char, DT_STRING_LEN> cHour = {0};
	std::array<char, DT_STRING_LEN> cMinute = {0};
	std::array<char, DT_STRING_LEN> cSecond = {0};

	// initialize variables with the passed date and time string
	std::memcpy(cYear.data(), cDateTime.data(), 4);
	std::memcpy(cMonth.data(), cDateTime.data() + 4, 2);
	std::memcpy(cDate.data(), cDateTime.data() + 6, 2);
	std::memcpy(cHour.data(), cDateTime.data() + 8, 2);
	std::memcpy(cMinute.data(), cDateTime.data() + 10, 2);
	std::memcpy(cSecond.data(), cDateTime.data() + 12, 2);

	// set system time structure
	SYSTEMTIME timeStruct;
	timeStruct.wYear = std::stoi(cYear.data());
	timeStruct.wMonth = std::stoi(cMonth.data());
	timeStruct.wDay = std::stoi(cDate.data());
	timeStruct.wHour = std::stoi(cHour.data());
	timeStruct.wMinute = std::stoi(cMinute.data());
	timeStruct.wSecond = std::stoi(cSecond.data());
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


bool CDateTime::toString (std::string& buffer)
{
	buffer = std::format("{:04}{:02}{:02}{:02}{:02}{:02}", 
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	return true;
}

bool CDateTime::toStringFull(std::string& buffer)
{
	buffer = std::format("{:04}{:02}{:02}{:02}{:02}{:02}{:03}",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	return true;
}

bool CDateTime::toFormattedString (std::string& buffer)
{
	std::string cReceiveTime;
	toString(cReceiveTime);

	// format receive time
	buffer = "    /  /     :  :";	// yyyy/mm/dd hh:mm s
	std::memcpy(buffer.data(), cReceiveTime.data(), 4);		// copy year
	std::memcpy(buffer.data() + 5, cReceiveTime.data() + 4, 2);	// copy month
	std::memcpy(buffer.data() + 8, cReceiveTime.data() + 6, 2);	// copy day
	std::memcpy(buffer.data() + 11, cReceiveTime.data() + 8, 2);	// copy hours
	std::memcpy(buffer.data() + 14, cReceiveTime.data() + 10, 2);
	std::memcpy(buffer.data() + 17, cReceiveTime.data() + 12, 2);
	buffer[19] = '\0';
	return true;
}

bool CDateTime::toGMTString (std::string& buffer)
{
	SYSTEMTIME universalTime;
	GetSystemTime(&universalTime);
	buffer = std::format("{:04}{:02}{:02}{:02}{:02}{:02}", 
		universalTime.wYear, universalTime.wMonth, universalTime.wDay,
		universalTime.wHour, universalTime.wMinute, universalTime.wSecond);
	return true;
}

bool CDateTime::toDateString (std::string& buffer)
{
	buffer = std::format("{:04}{:02}{:02}", 
		systemTime.wYear, systemTime.wMonth, systemTime.wDay);
	return true;
}

bool CDateTime::toTimeString (std::string& buffer)
{
	buffer = std::format("{:02}{:02}{:02}", 
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	return true;
}