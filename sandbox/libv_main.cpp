// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

#include <iostream>

#include <libv/worker_thread.hpp>

int main(int, char **) {
	libv::WorkerThread worker;

	for (int i = 0; i < 50000; i++) {
		worker.executeAsync([i]{
			std::cout << i << ' ';
		});
	}

	return 0;
}