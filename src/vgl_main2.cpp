// File: Main2.cpp, Created on 2015. január 14. 1:29, Author: Vader

#include <iostream>

#include <vl/log.hpp>
#include <vl/context.hpp>

int main(int, char**) {
	vl::log().output(std::cout);
	VLOG_TRACE(vl::log(), "trace");

	for (int i = 0; i < 100000; i++) {
		vl::Context context;
		VLOG_TRACE(vl::log(), "trace %d", i);
	}
	for (int i = 0; i < 100000; i++) {
		vl::Context context;
//		context.onInit([i]() {
//			VLOG_TRACE(vl::log(), "trace %d", i);
//		});
//		context.onTerm([]() {
//			VLOG_TRACE(vl::log(), "trace");
//		});
		context.executeSync([i] {
			VLOG_TRACE(vl::log(), "trace %d", i);
		});
		VLOG_TRACE(vl::log(), "trace %d", i);
	}

	VLOG_TRACE(vl::log(), "trace");
}