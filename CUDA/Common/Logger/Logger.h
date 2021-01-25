#pragma once

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "DestinationPlatform.h"

#include <mutex>
#include <thread>

#include <sstream>
#include <fstream>

#include <vector>
#include <unordered_map>

#include <functional>
#include <initializer_list>

using ThreadIdType = std::uint64_t;

class Logger
{
public:
	ThreadIdType ThisThreadId;
private:
	std::uint64_t FileNumberInLog = 1;
	std::uint64_t LineNumberInLog = 1;
private:
	std::string LogDirectory;
	std::string MainDirectoryName;
	std::string LoggerName;
	std::string TaskName;
private:
	std::vector<std::ofstream> Files;
private:
	static std::mutex LogMessageCoutMutexObject;
public:
	Logger(const char* LogDirectoryParameter, const char* MainDirectoryNameParameter, const char* LoggerNameParameter, const char* TaskNameParameter, const std::uint64_t ThisThreadIdParameter);
	~Logger();
private:
	void AllocResourcesForFiles();
	void CreateDirectories();
	void OpenLogFiles();
	void CloseLogFiles();
private:
	void CloseOldLogFilesAndOpenNewLogFiles();
	void LimitLogSizeByClosingOldLogFilesAndOpeningNewLogFilesAfterMaximalLimitOfLinesInOldFileIsExceeded(const ThreadIdType CurrentThreadId);
private:
	std::string CreateLogString(const std::string& MessageStr, const bool LogLineInfo, const ThreadIdType CurrentThreadId, std::uint64_t LineNumberInCommonLog, const bool PrintLogLineNumber, const bool PrintLogDateTime, const bool PrintLogProcessId, const bool PrintLogProcessPriorityLevel, const bool PrintLogThreadId) const;
private:
	void WriteToCommonLogFromThread(const bool Condition, const std::string& MessageStr, std::ostream& StreamObject, const ThreadIdType CurrentThreadId, const std::uint64_t FileNumber);
	void WriteToLogsFromThread(const std::string& MessageStrToFile, const ThreadIdType CurrentThreadId);
public:
	void LogMessageBool(const std::string& MessageStr, const bool LogLineInfo, const ThreadIdType CurrentThreadId, const bool PrintToConsole);
};

class LoggersManager
{
	friend class Logger;
private:
	bool PrintLogToConsole = true;
	bool PrintLogToFiles = true;
	
	bool PrintLogLineNumberToConsole = false;
	bool PrintLogDateTimeToConsole = false;
	bool PrintLogProcessIdToConsole = false;
	bool PrintLogProcessPriorityLevelToConsole = false;
	bool PrintLogThreadIdToConsole = false;

	bool PrintLogLineNumberToFile = true;
	bool PrintLogDateTimeToFile = true;
	bool PrintLogProcessIdToFile = false;
	bool PrintLogProcessPriorityLevelToFile = false;
	bool PrintLogThreadIdToFile = false;

	uint64_t MaximalNumberOfLinesInOneFile = 100000;
private:
	std::string TaskName;
	std::string LogDirectory;
	std::string ActualDateTimeStr;
	bool LogThreadsToSeparatedFiles;
	std::uint64_t FileNumberToIncreaseLineNumber;
private:
	std::function<void(const ThreadIdType CurrentThreadId, const std::uint64_t FileNumber, const std::string& MessageStr)> DrawMessageFunctionObject;
private:
	std::unique_ptr<Logger> LoggerMainObjectPointer;
	std::unordered_map<ThreadIdType, std::unique_ptr<Logger>> LoggersThreadsObjectsPointersMap;
private:
	static std::mutex CreateNewLoggerForThreadMutexObject;
private:
	void LogMessageBool(const std::string& MessageStr, const bool LogLineInfo, const bool PrintToConsole);
public:
	void Log(const std::stringstream& Message);
	void LogOnlyToFiles(const std::stringstream& Message);
	void LogWithoutLineInfo(const std::stringstream& Message);
	void LogWithoutLineInfoOnlyToFiles(const std::stringstream& Message);
    void LogInColorTerminal(std::ostream& color(std::ostream& s), const std::stringstream& Message);
private:
	std::vector<std::string> FilesNames;
	std::vector<std::string> SelectiveWords;
	std::vector<std::function<bool(const std::string&)>> SelectiveWordsFunctions;
public:	
	void InitializeFilesNames(const std::initializer_list<const std::string> InitialFilesNames);
	void InitializeSelectiveWordsFunctions(const std::initializer_list<std::function<bool(const std::string&)>> InitialSelectiveWordsFunctions);
	void InitializePrintingParameters(bool PrintLogToConsole, bool PrintLogToFiles, bool PrintLogLineNumberToConsole, bool PrintLogDateTimeToConsole, bool PrintLogProcessIdToConsole, bool PrintLogProcessPriorityLevelToConsole, bool PrintLogThreadIdToConsole, bool PrintLogLineNumberToFile, bool PrintLogDateTimeToFile, bool PrintLogProcessIdToFile, bool PrintLogProcessPriorityLevelToFile, bool PrintLogThreadIdToFile, uint64_t MaximalNumberOfLinesInOneFile);
	void InitializeLoggerManagerDataForTask(const std::string& TaskNameParameter, const std::string& LogDirectoryParameter, const std::string& ActualDateTimeStrParameter, const bool LogThreadsToSeparatedFilesParameter, const std::uint64_t FileNumberToIncreaseLineNumberParameter, std::function<void(const ThreadIdType CurrentThreadId, const std::uint64_t FileNumber, const std::string& MessageStr)> DrawMessageFunctionObjectParameter);
};

extern LoggersManager LoggersManagerObject;

#ifdef WINDOWS_PLATFORM
#define STREAM(text) (std::stringstream() << text)
#define STRING_TO_NUMBER_FROM_STRINGSTREAM(text) stoi((std::stringstream() << text).str())
#endif
#ifdef UNIX_PLATFORM
#define STREAM(text) static_cast<std::stringstream&>(std::stringstream() << text)
#define STRING_TO_NUMBER_FROM_STRINGSTREAM(text) stoll(static_cast<std::stringstream&>(std::stringstream() << text).str())
#endif

#endif
