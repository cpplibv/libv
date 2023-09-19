// Project: libv.utility, File: test/libv/utility/memory/test_intrusive2_ptr.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// pro
#include <libv/utility/memory/intrusive2_ptr.hpp>


// --- Incomplete type support ---------------------------------------------------------------------

// Incomplete types

struct IncompleteType;
void increase_ref_count(IncompleteType* ptr);
void decrease_ref_count(IncompleteType* ptr);
namespace ns {
struct IncompleteTypeNS;
void increase_ref_count(IncompleteTypeNS* ptr);
void decrease_ref_count(IncompleteTypeNS* ptr);
} // namespace ns

// Incomplete usage

struct IncompleteTypeTester {
	const libv::intrusive2_ptr<IncompleteType> p0;
	const libv::intrusive2_ptr<ns::IncompleteTypeNS> p1;
	~IncompleteTypeTester();
};

TEST_CASE("Test intrusive2_ptr with IncompleteType", "[libv.utility.memory.intrusive2_ptr]") {
	IncompleteTypeTester x;
	(void) x;
}

// Complete the incomplete types

class IncompleteType : public libv::ref_count_base {
	friend libv::ref_count_access;
public:
	~IncompleteType(){};
};
void increase_ref_count(IncompleteType* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(IncompleteType* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}
namespace ns {
class IncompleteTypeNS : public libv::ref_count_base {
	friend libv::ref_count_access;
public:
	~IncompleteTypeNS(){};
};
void increase_ref_count(IncompleteTypeNS* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(IncompleteTypeNS* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}
} // namespace ns

// Complete usage

IncompleteTypeTester::~IncompleteTypeTester() { }

// -------------------------------------------------------------------------------------------------


