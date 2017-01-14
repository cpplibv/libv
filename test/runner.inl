// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#define CATCH_CONFIG_RUNNER

#include <catch.hpp>
#include "vcomp_reporter.hpp"

int main(int argc, char* const argv[]) {
	Catch::Session session; // There must be exactly one instance

	session.configData().reporterNames.emplace_back("vcomp");
	session.configData().useColour = Catch::UseColour::Yes;

	int returnCode = session.applyCommandLine(argc, argv);
	if (returnCode != 0) // Indicates a command line error
		return returnCode;

	return session.run();
}
