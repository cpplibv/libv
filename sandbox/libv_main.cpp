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

//#include <iostream>
//#include "libv/vec.hpp"
//
//int main(int, char **) {
////	int ax = 0, ay = 0;
////	int bx = 0, by = 0;
////	auto cx = ax + bx;
////	auto cy = ay + by;
//
//	libv::ivec2 a;
//	libv::ivec2 b;
//	auto c = a + b;
//
//	std::cout << c.x << c.y << std::endl;
////	std::cout << cx << cy << std::endl;
//
//	return 0;
//}