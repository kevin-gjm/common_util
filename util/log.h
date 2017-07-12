#ifndef UTIL_LOG_H_
#define UTIL_LOG_H_

#include <string>
#include <string.h>
#include <stdarg.h>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


namespace src = boost::log::sources;
using namespace std;

enum log_severity_level
{
	kTRACE,
	kDEBUG,
	kINFO,
	kWARN,
	kERROR,
	kFATAL,
};
// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, log_severity_level lvl)
{
	static const char* const str[] =
	{
		"TRACE",
		"DEBUG",
		//"\033[41;37m INFO \033[0m",
		"INFO ",
		"WARN ",
		"ERROR",
		"FATAL",
	};
	if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast<int>(lvl);
	return strm;
}

extern src::severity_logger< log_severity_level > lg;
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOCATION		"["<<__FILENAME__<<":"<<__FUNCTION__<<":"<<__LINE__<<"]"
#define LOG_TRACE		BOOST_LOG_SEV(lg, kTRACE) << LOCATION
#define LOG_DEBUG		BOOST_LOG_SEV(lg, kDEBUG) << LOCATION
#define LOG_INFO		BOOST_LOG_SEV(lg, kINFO)  << LOCATION
#define LOG_WARN		BOOST_LOG_SEV(lg, kWARN)  << LOCATION
#define LOG_ERROR		BOOST_LOG_SEV(lg, kERROR) << LOCATION
//#define LOG_FATAL		BOOST_LOG_SEV(lg, kFATAL) << LOCATION


#define LOG_TRADE_TRACE		BOOST_LOG_SEV(lg, kTRACE) << LOCATION
#define LOG_TRADE_DEBUG		BOOST_LOG_SEV(lg, kDEBUG) << LOCATION
#define LOG_TRADE_INFO		BOOST_LOG_SEV(lg, kINFO)  << LOCATION
#define LOG_TRADE_WARN		BOOST_LOG_SEV(lg, kWARN)  << LOCATION
#define LOG_TRADE_ERROR		BOOST_LOG_SEV(lg, kERROR) << LOCATION


void LogError(const char * pcFormat, ...);
void LogFatal(const char * pcFormat, ...);

void FileLogInit(string sLogFileHead, int iRotationSize, int iMinFreeSize, bool bAutoFlush);

void SysLogInit(string sServerAddr, int iPort);

void ConsoleLogInit();

void SetLogLevel(log_severity_level level);

#endif //UTIL_LOG_H_
