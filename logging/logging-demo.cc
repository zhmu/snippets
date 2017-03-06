#include "loggingsystem.h"
#include "logger.h"
#include <assert.h>

namespace {

int
count()
{
	static int value = 0;
	return ++value;
}

}

int
main()
{
	// First step is to create a logging system
	zhmu::snippets::LoggingSystem loggingSystem;

	// Create some loggers
	zhmu::snippets::Logger* firstLog = loggingSystem.Get("First");
	zhmu::snippets::Logger* secondLog = loggingSystem.Get("Second");

	// Initially, all loggers are disabled so this won't do anything
	// Arguments will not be evaluated - this only happens if loggers are
	// enabled
	LOG(firstLog, "not visible not called, %d", count());
	LOG(secondLog, "not visible either");

	// Enable a logger
	firstLog->Enable();

	// Now this will be visible
	LOG(firstLog, "hi, %s log", "first");
	LOG(firstLog, "some counter: %d", count());

	// You can obtain a logger as often as you like, you will always get
	// the same one. Note that you should just keep the pointer as that is
	// much more efficient.
	{
		zhmu::snippets::Logger* firstLog2 = loggingSystem.Get("First");
		assert(firstLog == firstLog2);
		LOG(firstLog2, "again incrementing counter: %d", count());
		firstLog2->Disable();
	}

	// This won't be visible anymore
	LOG(firstLog, "won't increment counter to %d", count());
	assert(count() == 3); 

	// Destroying the LoggingSystem automatically removes all Logger's.
	return 0;
}
