// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// std
#include <iostream>
// pro
#include <vm4_viewer/log.hpp>
#include <vm4_viewer/frame.hpp>


// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	std::cout << libv::logger;

	{
		app::VM4ViewerFrame frame;
		frame.show();
		frame.join();
	}

	return 0;
}
