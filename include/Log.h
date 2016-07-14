#pragma once

#include <cstdio>
#include <cstdint>
#include <memory>
#include <map>
#include <codecvt>
#include "unichar.h"

#define LogE(format, ...)  Log::PrintLog(LL_ERROR, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define LogW(format, ...)  Log::PrintLog(LL_WARN, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#define LogI(format, ...)  Log::PrintLog(LL_INFO, __FILE__, __func__, __LINE__, format, __VA_ARGS__)

#ifdef _DEBUG
#define LogD(format, ...)  Log::PrintLog(LL_DEBUG, __FILE__, __func__, __LINE__, format, __VA_ARGS__);
//#define LogV(format, ...)  Log::PrintLog(LL_VERBOSE, __FILE__, __func__, __LINE__, format, __VA_ARGS__)
#else
#define LogD(format, ...) ;
//#define LogV(format, ...)  ;
#endif // _DEBUG

using namespace yup;

enum LogLevel
{
	LL_ERROR,
	LL_WARN,
	LL_INFO,
	LL_DEBUG,
	LL_VERBOSE
};

typedef uint8_t LogOutput;
#define LO_NONE 0
#define LO_STDOUT 1
#define LO_FILE 2
#define LO_DEBUGWINDOW 4
#define LO_ALL 0xFF

class Log
{
	friend struct std::default_delete<Log>;

private:

	std::map<LogLevel, ustring> mLogLevelNames;
	LogLevel mLogLevel;
	LogOutput mLogOutput;

	ufstream mLogFile;

private:
	Log(LogLevel level = LL_VERBOSE);
	~Log();

	// singleton implementation
	static Log & Instance() {

		static std::unique_ptr<Log> instance;

		if (!instance)
			instance = std::unique_ptr<Log>(new Log());

		return *instance;
	}

	void outputLog(LogLevel level, const char * file, const char * func, int line, const ustring & str);
	void output(const ustring & str);

public:

	static void SetLogLevel(LogLevel level) { Instance().mLogLevel = level; }
	static void SetLogOutput(LogOutput output) { Instance().mLogOutput = output; }
	static void AddLogOutput(LogOutput output) { Instance().mLogOutput = Instance().mLogOutput | output; }
	static void RemoveLogOutput(LogOutput output) { Instance().mLogOutput = Instance().mLogOutput & ~output; }

	static void OpenLogFile(const ustring & filename);
	static void CloseLogFile();

	static void PrintLog(LogLevel level, const char * file, const char * func, int line, const char *format, ...);
	static void PrintLog(LogLevel level, const char * file, const char * func, int line, const wchar_t *format, ...);

	static void Print(const char *format, ...);
	static void Print(const wchar_t *format, ...);
};



Log::Log(LogLevel level)
	: mLogLevel(level)
{
	mLogLevelNames[LL_ERROR] = TEXT("ERROR");
	mLogLevelNames[LL_WARN] = TEXT("WARN");
	mLogLevelNames[LL_INFO] = TEXT("INFO");
	mLogLevelNames[LL_DEBUG] = TEXT("DEBUG");
	mLogLevelNames[LL_VERBOSE] = TEXT("VERBOSE");

	mLogOutput = LO_STDOUT | LO_DEBUGWINDOW;
}

Log::~Log()
{
	if (Instance().mLogFile.is_open())
		Instance().mLogFile.close();
}

void Log::outputLog(LogLevel level, const char * file, const char * func, int line, const ustring & str)
{
	ustring fileStr = ToUString(file);

	ustringstream ss;

	ss << "[" << Instance().mLogLevelNames[level] << "] "
		<< fileStr.substr(fileStr.find_last_of(TEXT("\\/")) + 1, fileStr.length()) << " "
		<< "(" << line << ") => "
		<< func << ": "
		<< str << std::endl;

	output(ss.str());
}

void Log::output(const ustring & str)
{
	if (mLogOutput & LO_STDOUT)
		ucout << str;

	if (mLogOutput & LO_DEBUGWINDOW)
		OutputDebugString(str.c_str());

	if (mLogOutput & LO_FILE && mLogFile.is_open())
		mLogFile << str;
}

void Log::OpenLogFile(const ustring & filename)
{
	Instance().mLogFile.open(filename, ufstream::out);

#ifdef _UNICODE
	const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	Instance().mLogFile.imbue(utf8_locale);
#endif

	Instance().AddLogOutput(LO_FILE);
}

void Log::CloseLogFile()
{
	if (Instance().mLogFile.is_open())
	{
		Instance().mLogFile.close();
		Instance().RemoveLogOutput(LO_FILE);
	}
}


////////////////////////////////////////////////////////////////////////////////
// - Narrow version
////////////////////////////////////////////////////////////////////////////////

void Log::Print(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().output(str);
}

void Log::PrintLog(LogLevel level, const char * file, const char * func, int line, const char *format, ...)
{
	if (level > Instance().mLogLevel)
		return;

	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().outputLog(level, file, func, line, str);
}

////////////////////////////////////////////////////////////////////////////////
// - Wide version
////////////////////////////////////////////////////////////////////////////////

void Log::Print(const wchar_t *format, ...)
{
	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().output(str);
}

void Log::PrintLog(LogLevel level, const char * file, const char * func, int line, const wchar_t *format, ...)
{
	if (level > Instance().mLogLevel)
		return;

	va_list arg;
	va_start(arg, format);
	ustring str = ToUString(format, arg);
	va_end(arg);

	Instance().outputLog(level, file, func, line, str);
}
