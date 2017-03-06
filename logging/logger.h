#ifndef __LOGGER_H__
#define __LOGGER_H__

namespace zhmu {
namespace snippets {

class LoggingSystem;

//! \brief Implements a logging producer
class Logger {
	friend class LoggingSystem;

public:
	//! \brief Enables logging
	void Enable() { m_Enabled = true; }

	//! \brief Disables logging
	void Disable() { m_Enabled = false; }

	/* Functions below are intended to be used by the LOG() macro */

	//! \brief Is the logger enabled?
	bool _IsEnabled() const { return m_Enabled; }

	//! \brief Retrieve the logger name
	const char* GetName() const { return m_Name; }

	//! \brief Performs logging
	void _Log(const char* fmt, ...);

protected:
	Logger() = delete;
	Logger(const Logger& oLogger) = delete;

	/*! \brief Constructor
	 *
	 *  This is only to be used by the LoggingSystem
	*/
	Logger(LoggingSystem& oSystem, const char* name);

	//! \brief Destroys the logger - note that the LoggingSystem is owner of each Logger
	~Logger();

private:
	//! \brief Logger name
	char* m_Name;

	//! \brief Enabled state
	bool m_Enabled;

	//! \brief Logging system we belong to
	LoggingSystem& m_System;
};

#define LOG(l, a...) \
	(l)->_IsEnabled() ? (l)->_Log(a) : (void)0

}
}

#endif /* __LOGGER_H__ */
