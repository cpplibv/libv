// File: libv_gl_main3.cpp, Created on 2017.11.17. at 22:38, Author: Vader

// ext
#include <immer/vector.hpp>
#include <immer/vector_transient.hpp>
// std
#include <cassert>
#include <iostream>


// -------------------------------------------------------------------------------------------------

int main() {
    const auto v0 = immer::vector<int>{};
    const auto v0a = v0.push_back(13); // Slow multi update
    const auto v0b = v0a.push_back(56);
    const auto v1 = v0b.push_back(81);
    assert(v0.size() == 0 && v1.size() == 1 && v1[0] == 13);

    const auto v2 = v1.set(0, 42).push_back(4).push_back(8); // Fast multi update on rvalues
    assert(v1[0] == 13 && v2[0] == 42);

	auto v3_ = v2.transient(); // Fast multi update
	v3_.set(0, v3_[0] - 50);
	v3_.update(1, [](const auto v) { return v - 50; } );
	const auto v3 = v3_.persistent();

	std::cout << "v0: ";
	for (const auto& i : v0)
		std::cout << i << " ";
	std::cout << std::endl;

	std::cout << "v1: ";
	for (const auto& i : v1)
		std::cout << i << " ";
	std::cout << std::endl;

	std::cout << "v2: ";
	for (const auto& i : v2)
		std::cout << i << " ";
	std::cout << std::endl;

	std::cout << "v3: ";
	for (const auto& i : v3)
		std::cout << i << " ";
	std::cout << std::endl;
}

// -------------------------------------------------------------------------------------------------
