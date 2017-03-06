#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loggingsystem.h"

namespace {
size_t maximumLogLineOutput = 1024;
}

namespace zhmu {
namespace snippets {

Logger::Logger(LoggingSystem& oSystem, const char* name)
	: m_System(oSystem), m_Enabled(false)
{
	m_Name = strdup(name);
}

Logger::~Logger()
{
	free(m_Name);
}

void
Logger::_Log(const char* fmt, ...)
{
	char temp[maximumLogLineOutput];

	va_list va;
	va_start(va, fmt);
	vsnprintf(temp, sizeof(temp), fmt, va);
	temp[sizeof(temp) - 1] = '\0';
	va_end(va);

	m_System.PerformLog(*this, temp);
}

}
}
