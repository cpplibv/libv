// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

#include <iostream>

int main(int, char **) {

	uint32_t v1 = 255;
	uint32_t vh = 127;
	uint32_t v0 = 0;

	uint32_t r1 = (v1 << 24);
	uint32_t g1 = (v1 << 16);
	uint32_t b1 = (v1 << 8);
	uint32_t a1 = (v1 << 0);

	uint32_t rh = (vh << 24);
	uint32_t gh = (vh << 16);
	uint32_t bh = (vh << 8);
	uint32_t ah = (vh << 0);

	uint32_t r0 = (v0 << 24);
	uint32_t g0 = (v0 << 16);
	uint32_t b0 = (v0 << 8);
	uint32_t a0 = (v0 << 0);

	std::cout << r0 + gh + bh + a1 << std::endl;
	std::cout << r1 + gh + bh + a1 << std::endl;
	std::cout << rh + g0 + bh + a1 << std::endl;
	std::cout << rh + g1 + bh + a1 << std::endl;
	std::cout << rh + gh + b0 + a1 << std::endl;
	std::cout << rh + gh + b1 + a1 << std::endl;

	return 0;
}
