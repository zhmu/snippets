#ifndef __LOGGINGSYSTEM_H__
#define __LOGGINGSYSTEM_H__

#include <mutex>
#include <vector>

namespace zhmu {
namespace snippets {

class Logger;

//! \brief Handles the logging system itself
class LoggingSystem {
	friend class Logger;

public:
	//! \brief Constructor
	LoggingSystem();
	
	//! \brief Destructor
	~LoggingSystem();

	/*! \brief Retrieve a logger object
	 *  \param name Name of the logger to use
	 *  \returns Pointer to the new object
	 *
	 *  The logger will be created as necessary.
	 */
	Logger* Get(const char* name);

protected:
	/*! \brief Called by a logger when there is something to log
	 *  \param oLogger Logger requesting the log
	 *  \param sMessage String to log
	 */
	void PerformLog(Logger& oLogger, const char* sMessage);

private:
	typedef std::vector<Logger*> TLoggerPtrList;
	TLoggerPtrList m_Logger;
	std::mutex m_Mutex;
	
};

}
}

#endif /* __LOGGINGSYSTEM_H__ */
