// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_CONSOLE_WIDTH 100

#include <catch/catch.hpp>
#include "libv_reporter.hpp"

int main(int argc, char* const argv[]) {
	Catch::Session session; // There must be exactly one instance

	session.configData().reporterName = "libv_minimal";
	session.configData().useColour = Catch::UseColour::Yes;

	int returnCode = session.applyCommandLine(argc, argv);
	if (returnCode != 0) // Indicates a command line error
		return returnCode;

	return session.run();
}
