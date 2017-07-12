#include "log.h"
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <boost/log/sources/logger.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

src::severity_logger<log_severity_level> lg;

void FileLogInit(string sLogFileHead, int iRotationSize, int iMinFreeSize, bool bAutoFlush)
{
	typedef sinks::synchronous_sink<sinks::text_file_backend> TextSink;
	boost::shared_ptr<sinks::text_file_backend> backend = boost::make_shared<sinks::text_file_backend>(
		keywords::file_name = sLogFileHead + "%Y-%m-%d_%H-%M-%S.%N.log",
		keywords::rotation_size = iRotationSize,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		keywords::min_free_space = iMinFreeSize);

	backend->auto_flush(bAutoFlush);
	boost::shared_ptr<TextSink> sink(new TextSink(backend));
	sink->set_formatter(
		expr::format("[%1%]<%2%>(%3%)%4%")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		% expr::attr<log_severity_level>("Severity")
		% expr::attr<attrs::current_thread_id::value_type >("ThreadID")
		% expr::smessage
	);
	logging::core::get()->add_sink(sink);
	logging::add_common_attributes();
	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());

}
void SysLogInit(string sServerAddr, int iPort)
{
	/*boost::shared_ptr< sinks::synchronous_sink< sinks::syslog_backend > > sink(new sinks::synchronous_sink< sinks::syslog_backend >());
	sinks::syslog::custom_severity_mapping< log_severity_level > mapping("Severity");
	mapping[log_severity_level::normal] = sinks::syslog::info;
	mapping[log_severity_level::warning] = sinks::syslog::warning;
	mapping[log_severity_level::error] = sinks::syslog::critical;
	sink->locked_backend()->set_severity_mapper(mapping);
	sink->locked_backend()->set_target_address(_ServerAddress, _Port);
	logging::core::get()->add_sink(sink);*/

	boost::shared_ptr< sinks::synchronous_sink< sinks::syslog_backend > > sink(new sinks::synchronous_sink< sinks::syslog_backend >());
	sink->set_formatter(
		expr::format("[%1%]<%2%>(%3%)%4%")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		% expr::attr<log_severity_level>("Severity")
		% expr::attr<attrs::current_thread_id::value_type >("ThreadID")
		% expr::smessage
	);
	sink->locked_backend()->set_target_address(sServerAddr,static_cast<unsigned short> (iPort));
	logging::core::get()->add_sink(sink);
	logging::add_common_attributes();
	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());


}

void ConsoleLogInit()
{
	auto console_sink = logging::add_console_log();
	console_sink->set_formatter(
		expr::format("[%1%]<%2%>(%3%)%4%")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
		% expr::attr<log_severity_level>("Severity")
		% expr::attr<attrs::current_thread_id::value_type >("ThreadID")
		//% expr::format_named_scope("Scope", keywords::format = "%f:%l")
		% expr::smessage
	);
	logging::core::get()->add_sink(console_sink);
	logging::add_common_attributes();
	logging::core::get()->add_global_attribute("ThreadID", attrs::current_thread_id());
	//logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());
}

void SetLogLevel(log_severity_level level)
{
	logging::core::get()->set_filter(expr::attr< log_severity_level >("Severity") >= level);
}
void LogError(const char * pcFormat, ...)
{
	char sBuf[1024] = { 0 };
	//string newFormat = "\033[44;37m " + string(pcFormat) + " \033[0m";
	string newFormat = string(pcFormat);
	va_list args;
	va_start(args, pcFormat);
	vsnprintf(sBuf, sizeof(sBuf), newFormat.c_str(), args);
	va_end(args);

	BOOST_LOG_SEV(lg, kERROR) << LOCATION << sBuf;
}
void LogFatal(const char * pcFormat, ...)
{
	char sBuf[1024] = { 0 };
	//string newFormat = "\033[44;37m " + string(pcFormat) + " \033[0m";
	string newFormat = string(pcFormat);
	va_list args;
	va_start(args, pcFormat);
	vsnprintf(sBuf, sizeof(sBuf), newFormat.c_str(), args);
	va_end(args);

	BOOST_LOG_SEV(lg, kFATAL) << LOCATION << sBuf;
	exit(-1);
}
