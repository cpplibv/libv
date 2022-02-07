// Project: libv.net, File: sandbox/libv_net_main.cpp

// libv
#include <libv/container/vector_2d.hpp>
#include <libv/math/vec.hpp>
#include <libv/mt/hardware_concurrency.hpp>
#include <libv/mt/thread_bulk.hpp>
#include <libv/utility/int_ceil_div.hpp>

//#include <libv/log/log.hpp>
//#include <libv/net/error.hpp>
//#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/acceptor_he.hpp>
//#include <libv/net/mtcp/connection_he.hpp>
//#include <libv/utility/concat.hpp>
// std
//#include <deque>
//#include <iostream>
//#include <memory>
//#include <set>
//#include <string>
//#include <utility>
//#include <mutex>


float calculate(float x, float y) {
	return x + y;
}

int main(int argc, char** argv) {
//	std::cout << libv::logger_stream;

//	std::mutex mutex;

	libv::vec2z size{1024, 1024};
	libv::vector_2D<float> data(size);

	libv::mt::thread_bulk threads(libv::mt::hardware_concurrency_or(4));

	// -------------------------------------------------------------------------------------------------
	// Y-Scan

//	parallel_for(threads, 0, size.y, [&](auto y) {
//		for (int x = 0; x < size.x; ++x) {
//			const auto yf = static_cast<float>(y);
//			const auto xf = static_cast<float>(x);
//			data(x, y) = calculate(xf, yf);
//		}
//	});

	// -------------------------------------------------------------------------------------------------
	// Index group scan

//	static constexpr auto group_size = 64;
//
//	parallel_for(threads, 0, data.size_total() / group_size, [&](auto i) {
//		const auto op_start = i * group_size;
//		const auto op_end = std::min(op_start + group_size, data.size_total());
//
//		for (int j = op_start; j < op_end; ++j) {
//			const auto pos = data.total_to_index(j);
//			const auto xf = static_cast<float>(pos.x);
//			const auto yf = static_cast<float>(pos.y);
//			data(pos.x, pos.y) = calculate(xf, yf);
//		}
//	});

	// -------------------------------------------------------------------------------------------------
	// Tile scan

//	static constexpr auto tile_size = 32;
//
//	const auto tile_count_x = libv::int_ceil_div(data.size_x(), tile_size);
//	const auto tile_count_y = libv::int_ceil_div(data.size_y(), tile_size);
//	const auto tile_count = tile_count_x * tile_count_y;
//
//	parallel_for(threads, 0, tile_count, [&](auto i) {
//		const auto tile_index_y = i / tile_count_x;
//		const auto tile_index_x = i - tile_index_y * tile_count_y;
//
//		const auto tile_start_x = tile_index_x * tile_size;
//		const auto tile_start_y = tile_index_y * tile_size;
//		const auto tile_end_x = std::min(tile_start_x + tile_size, data.size_x());
//		const auto tile_end_y = std::min(tile_start_y + tile_size, data.size_y());
//
//		for (std::size_t x = tile_start_x; x < tile_start_y; ++x) {
//			for (std::size_t y = tile_start_y; y < tile_start_y; ++y) {
//				const auto xf = static_cast<float>(x);
//				const auto yf = static_cast<float>(y);
//				data(x, y) = calculate(xf, yf);
//			}
//		}
//	});

	// -------------------------------------------------------------------------------------------------

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
