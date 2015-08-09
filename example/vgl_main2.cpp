// File: Main2.cpp, Created on 2015. január 14. 1:29, Author: Vader

#include <iostream>

#include <vl/log.hpp>
#include <vl/worker_thread.hpp>

int main(int, char**) {
	vl::log().output(std::cout);
	VLOG_TRACE(vl::log(), "trace");

	for (int i = 0; i < 50000; i++) {
		vl::WorkerThread context;
		VLOG_TRACE(vl::log(), "trace %d", i);
	}
	for (int i = 0; i < 500000; i++) {
		vl::WorkerThread context;
		context.executeAsync([&] {
			context.executeAsync([&] {
				std::cerr << '-';
				context.executeSync([&] {
					std::cerr << 0;
				});
			});
			std::cerr << 1;
		});
		std::cerr << 2;
		context.executeSync([i] {
			std::cerr << 3;
		});
	}

	VLOG_TRACE(vl::log(), "trace");
}



//		context.onInit([i]() {
//			VLOG_TRACE(vl::log(), "trace %d", i);
//		});
//		context.onTerm([]() {
//			VLOG_TRACE(vl::log(), "trace");
//		});