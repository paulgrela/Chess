
#include "DestinationPlatform.h"

#ifdef WINDOWS_PLATFORM
#include <windows.h>
#endif

#include <sstream>
#include <iomanip>
#include <iostream>

#include "StringUtils.h"
#include "DateTimeUtils.h"
#include "ExceptionsMacro.h"

#include "Logger.h"

using namespace std;

string GetActualDateTimeStandardCPP(const char* sep1, const char* sep2, const char* sep3, const char* sep4, const char* sep5)
{
	using namespace string_utils;

	time_t current_time;
	struct tm local_time;
	time(&current_time);
	local_time = *localtime(&current_time);
	return string(to_string(local_time.tm_year + 1900) + sep1 + align_str(to_string(local_time.tm_mon + 1), '0', 2) + sep2 + align_str(to_string(local_time.tm_mday), '0', 2) + sep3 + align_str(to_string(local_time.tm_hour), '0', 2) + sep4 + align_str(to_string(local_time.tm_min), '0', 2) + sep5 + align_str(to_string(local_time.tm_sec), '0', 2));
}

#ifdef WINDOWS_PLATFORM
string GetActualDateTimeWindows(const char* sep1, const char* sep2, const char* sep3, const char* sep4, const char* sep5, const char* sep6)
{
	using namespace string_utils;

	SYSTEMTIME st;
	GetLocalTime(&st);
	return string(to_string(st.wYear) + sep1 + align_str(to_string(st.wMonth), '0', 2) + sep2 + align_str(to_string(st.wDay), '0', 2) + sep3 + align_str(to_string(st.wHour), '0', 2) + sep4 + align_str(to_string(st.wMinute), '0', 2) + sep5 + align_str(to_string(st.wSecond), '0', 2) + sep6 + align_str(to_string(st.wMilliseconds), '0', 3));
}
#endif

string GetDurationTimeFromNanoInOneLineStr(const uint64_t DurationTimeNanoCount, const char* TextToPrint, const char* ExceptionTextToPrint)
{
	stringstream TempDurationTimeStr;

	try
	{
		TempDurationTimeStr << TextToPrint;

		TempDurationTimeStr << setfill('0');

		TempDurationTimeStr << (DurationTimeNanoCount % 3600'000'000'000'000) / 3600'000'000'000 << " hours ";
		TempDurationTimeStr << (DurationTimeNanoCount % 3600'000'000'000) / 60'000'000'000 << " minutes ";
		TempDurationTimeStr << (DurationTimeNanoCount % 60'000'000'000) / 1000'000'000 << " seconds ";

		TempDurationTimeStr << (DurationTimeNanoCount % 1000'000'000) / 1000'000; // format milliseconds
		TempDurationTimeStr << ".";
		TempDurationTimeStr << setw(3) << (DurationTimeNanoCount % 1000'000) / 1000; // format microseconds
		TempDurationTimeStr << ".";
		TempDurationTimeStr << setw(3) << DurationTimeNanoCount % 1000; // format nanoseconds
	}
	CATCH(ExceptionTextToPrint)

	return TempDurationTimeStr.str();
}

string GetDurationTimeInOneLineStr(const chrono::high_resolution_clock::time_point start_time, const chrono::high_resolution_clock::time_point stop_time, const char* TextToPrint, const char* ExceptionTextToPrint)
{
	return GetDurationTimeFromNanoInOneLineStr(chrono::duration_cast<chrono::nanoseconds>(stop_time - start_time).count(), TextToPrint, ExceptionTextToPrint);
}

void PrintDurationTimeAllFormats(const chrono::high_resolution_clock::time_point start_time, const chrono::high_resolution_clock::time_point stop_time, const char* TextToPrint, const char* ExceptionTextToPrint)
{
	try
	{
		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.Log(STREAM(GetDurationTimeInOneLineStr(start_time, stop_time, TextToPrint, ExceptionTextToPrint)));

		LoggersManagerObject.Log(STREAM(""));

		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::nanoseconds>(stop_time - start_time).count() << " nanoseconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::microseconds>(stop_time - start_time).count() << " microseconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::milliseconds>(stop_time - start_time).count() << " miliseconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::seconds>(stop_time - start_time).count() << " seconds in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::minutes>(stop_time - start_time).count() << " minutes in total"));
		LoggersManagerObject.Log(STREAM(TextToPrint << chrono::duration_cast<chrono::hours>(stop_time - start_time).count() << " hours in total"));

		LoggersManagerObject.Log(STREAM(""));
	}
	CATCH(ExceptionTextToPrint)
}
