// Project: libv.container, File: sandbox/libv_container_main.cpp

// ext
#include <fmt/printf.h>
// libv
#include <libv/container/vector_2d.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/timer.hpp>
// std
#include <iostream>


// -------------------------------------------------------------------------------------------------

int main() {

	const auto measure = [](std::string_view name, auto fn) {
		double sumTime = 0;
		int32_t sumCount = 5;

		fn(); // Warm up the code
		for (int i = 0; i < sumCount; ++i) {
			libv::Timer timer;
			fn();
			sumTime += timer.timed_ms().count();
		}

		sumTime /= sumCount;

		fmt::print("{:40} {:>8.3f}ms\n", name, sumTime); std::cout << std::flush;
	};


	constexpr std::size_t size = 10240;

	measure("float", [] {
		libv::vector_2D<float> container{size, size};
	});

//	measure("float uninitialized", [] {
//		libv::vector_2D<float> container{size, size, libv::uninitialized};
//	});

	measure("float zero", [] {
		libv::vector_2D<float> container{size, size, 0};
	});

	measure("float one", [] {
		libv::vector_2D<float> container{size, size, 42};
	});

	measure("vec4f", [] {
		libv::vector_2D<libv::vec4f> container{size, size};
	});

	measure("vec4f uninitialized", [] {
		libv::vector_2D<libv::vec4f> container{size, size, libv::uninitialized};
	});

	measure("vec4f zero", [] {
		libv::vector_2D<libv::vec4f> container{size, size, libv::vec4f{0, 0, 0, 0}};
	});

	measure("vec4f one", [] {
		libv::vector_2D<libv::vec4f> container{size, size, libv::vec4f{42, 42, 42, 42}};
	});

	return 0;
}

// -------------------------------------------------------------------------------------------------
