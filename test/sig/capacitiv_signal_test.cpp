// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch.hpp>
#include <vl/sig/signal.hpp>

#include "signal_test_util.hpp"

using vl::CapacitivSignal;

TEST_CASE() {
	CapacitivSignal<int> source;
	SpyResultTypeFor(source) result;
	source.output(spyInto<int>(result));

	SECTION("First spy is empty") {
		CHECK(result.size() == 0u);
	}

	SECTION("Flushing an empty signal result no output") {
		source.flush();
		CHECK(result.size() == 0u);
	}
	
	SECTION("Firing then flushing reaches the output") {
		source.fire(0);
		CHECK(result.size() == 0u);

		source.flush();
		CHECK(result.size() == 1u);
	}

	SECTION("Firing multiple time then flushing reaches the output") {
		source.fire(0);
		source.fire(0);
		CHECK(result.size() == 0u);

		source.flush();
		CHECK(result.size() == 2u);
	}
}