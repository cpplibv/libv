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


	constexpr std::size_t size4 = 4 * 1024;
	constexpr std::size_t size = size4 * 4;

	measure("vector_2D float", [] {
		libv::vector_2D<float> container{size, size};
	});

//	measure("vector_2D float uninitialized", [] {
//		libv::vector_2D<float> container{size, size, libv::uninitialized};
//	});

	measure("vector_2D float zero", [] {
		libv::vector_2D<float> container{size, size, 0};
	});

	measure("vector_2D float one", [] {
		libv::vector_2D<float> container{size, size, 42};
	});

	measure("vector_2D vec4f", [] {
		libv::vector_2D<libv::vec4f> container{size4, size};
	});

	measure("vector_2D vec4f uninitialized", [] {
		libv::vector_2D<libv::vec4f> container{size4, size, libv::uninitialized};
	});

	measure("vector_2D vec4f zero", [] {
		libv::vector_2D<libv::vec4f> container{size4, size, libv::vec4f{0, 0, 0, 0}};
	});

	measure("vector_2D vec4f one", [] {
		libv::vector_2D<libv::vec4f> container{size4, size, libv::vec4f{42, 42, 42, 42}};
	});

	// -------------------------------------------------------------------------------------------------

	measure("vector float", [] {
		std::vector<float> container;
		container.resize(size * size);
	});

	measure("vector float zero", [] {
		std::vector<float> container;
		container.resize(size * size, 0.f);
	});

	measure("vector vec4f", [] {
		std::vector<libv::vec4f> container;
		container.resize(size4 * size);
	});

	measure("vector vec4f uninitialized", [] {
		std::vector<libv::vec4f> container;
		container.resize(size4 * size, libv::uninitialized);
	});

//	measure("vector vec4f uninitialized allocator", [] {
//		std::vector<libv::vec4f, libv::uninitialized_allocator<libv::vec4f>> container;
//
//		container.resize(size * size);
//	});

	return 0;
}

// -------------------------------------------------------------------------------------------------
