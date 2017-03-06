#include "loggingsystem.h"
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "logger.h"

namespace zhmu {
namespace snippets {

LoggingSystem* g_LoggingSystem = NULL;

LoggingSystem::LoggingSystem()
{
}

LoggingSystem::~LoggingSystem()
{
	for (auto pLogger: m_Logger)
		delete pLogger;
}

Logger*
LoggingSystem::Get(const char* name)
{
	std::lock_guard<std::mutex> lockGuard(m_Mutex);

	for (auto& pLogger: m_Logger) {
		if (strcmp(pLogger->GetName(), name) != 0)
			continue;

		// Found the logger; just return it
		return pLogger;
	}

	// Not found; we need to make a new logger
	Logger* pLogger = new Logger(*this, name);
	m_Logger.push_back(pLogger);
	return pLogger;
}

void
LoggingSystem::PerformLog(Logger& oLogger, const char* sMessage)
{
	// We don't lock anything - we assume fprintf() buffers and won't
	// interleave output, which should be a fair assumption
	struct timeval tv;
	gettimeofday(&tv, NULL);

	struct tm tm;
	localtime_r(&tv.tv_sec, &tm);
	fprintf(stderr, "[%04u-%02u-%02u %02u:%02u:%02u.%03u - %s] - %s\n",
	 tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
	 tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_usec / 1000, oLogger.GetName(), sMessage);
}

}
}
