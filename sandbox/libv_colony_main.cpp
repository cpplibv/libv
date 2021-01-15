// Project: libv, File: sandbox/libv_colony_main.cpp, Author: Császár Mátyás [Vader]

// ext
#include <colony/plf_colony.h>
// std
#include <cassert>
#include <iostream>


// -------------------------------------------------------------------------------------------------

int main() {
//	plf::colony<int> colony;
//	const int* ptr = &*colony.insert(0);
////	int* ptr = &*colony.insert(0);
//	auto it = colony.get_iterator_from_pointer(ptr);
//	colony.erase(it);

	plf::colony<int> i_colony;
	plf::colony<int>::iterator it;
	plf::colony<int*> p_colony;
	plf::colony<int*>::iterator p_it;

	// Insert 100 ints to i_colony and pointers to those ints to p_colony:
	for (int i = 0; i != 100; ++i) {
		it = i_colony.insert(i);
		p_colony.insert(&(*it));
	}

	// Erase half of the ints:
	for (it = i_colony.begin(); it != i_colony.end(); ++it) {
		it = i_colony.erase(it);
	}

	// Erase half of the int pointers:
	for (p_it = p_colony.begin(); p_it != p_colony.end(); ++p_it) {
		p_it = p_colony.erase(p_it);
	}

	// Total the remaining ints via the pointer colony (pointers will still be valid even after insertions and erasures):
	int total = 0;

	for (p_it = p_colony.begin(); p_it != p_colony.end(); ++p_it) {
		total += *(*p_it);
	}

	std::cout << "Total: " << total << std::endl;

	if (total == 2500) {
		std::cout << "Pointers still valid!" << std::endl;
	}

	std::cout << "Total: " << total << std::endl;

	// Erase element with an pointer
	std::vector<const int*> pointers;
	for (const auto& i : i_colony)
		pointers.push_back(&i);

	for (const auto& p : pointers)
		i_colony.erase(i_colony.get_iterator_from_pointer(p));

	std::cout << "Erasure by pointer ran" << std::endl;
	std::cout << i_colony.size() << std::endl;

	return 0;
}

// -------------------------------------------------------------------------------------------------
