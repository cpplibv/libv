// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

#include <catch/catch.hpp>

#include <libv/range/reversed_if.hpp>
#include <iostream>
#include <vector>


//using namespace libv;

// -------------------------------------------------------------------------------------------------

TEST_CASE("reverse_if adaptor vec baseline") {
	std::vector<int> vec{1, 2, 3, 4};

	auto it = vec.begin();
	CHECK(*it++ == 1);
	CHECK(*it++ == 2);
	CHECK(*it++ == 3);
	CHECK(*it++ == 4);
}

TEST_CASE("reverse_if adaptor false") {
	std::vector<int> vec{1, 2, 3, 4};
	auto rev = vec | libv::reverse_if(false);

	auto it = rev.begin();
	CHECK(*it++ == 1);
	CHECK(*it++ == 2);
	CHECK(*it++ == 3);
	CHECK(*it++ == 4);
}

TEST_CASE("reverse_if adaptor true") {
	std::vector<int> vec{1, 2, 3, 4};
	auto rev = vec | libv::reverse_if(true);

	auto it = rev.begin();
	CHECK(*it++ == 4);
	CHECK(*it++ == 3);
	CHECK(*it++ == 2);
	CHECK(*it++ == 1);
}

TEST_CASE("reverse_if adaptor distance") {
	std::vector<int> vec{1, 2, 3, 4};
	auto rev_true = vec | libv::reverse_if(true);
	auto rev_false = vec | libv::reverse_if(false);

	CHECK(std::distance(rev_true.begin(), rev_true.end()) == 4);
	CHECK(std::distance(rev_false.begin(), rev_false.end()) == 4);
}

TEST_CASE("reverse_if adaptor false in for") {
	std::vector<int> vec{1, 2, 3, 4};
	auto rev = vec | libv::reverse_if(false);

	int i = 1;
	for (const auto& item : rev) {
		CHECK(item == i++);
	}
}

TEST_CASE("reverse_if adaptor true in for") {
	std::vector<int> vec{1, 2, 3, 4};
	auto rev = vec | libv::reverse_if(true);

	int i = 4;
	for (const auto& item : rev) {
		CHECK(item == i--);
	}
}

TEST_CASE("reverse_if adaptor false in for inline") {
	std::vector<int> vec{1, 2, 3, 4};

	int i = 1;
	for (const auto& item : vec | libv::reverse_if(false)) {
		CHECK(item == i++);
	}
}

TEST_CASE("reverse_if adaptor true in for inline") {
	std::vector<int> vec{1, 2, 3, 4};

	int i = 4;
	for (const auto& item : vec | libv::reverse_if(true)) {
		CHECK(item == i--);
	}
}

TEST_CASE("reverse_if adaptor function false in for inline") {
	std::vector<int> vec{1, 2, 3, 4};

	int i = 1;
	for (const auto& item : libv::reverse_if(vec, false)) {
		CHECK(item == i++);
	}
}

TEST_CASE("reverse_if adaptor function true in for inline") {
	std::vector<int> vec{1, 2, 3, 4};

	int i = 4;
	for (const auto& item : libv::reverse_if(vec, true)) {
		CHECK(item == i--);
	}
}
