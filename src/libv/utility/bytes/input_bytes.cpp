// Project: libv.utility, File: src/libv/utility/bytes/input_bytes.cpp

// hpp
#include <libv/utility/bytes/input_bytes.hpp>
// std
#include <cstring>
#include <istream>
#include <span>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

input_bytes::input_bytes(const std::byte* data, std::size_t size) noexcept :
	object(const_cast<std::byte*>(data)), // No modification will be made on the pointer by read_fn
	input_offset(0),
	input_size(size),
	read_fn(+[](void* object_, std::byte* ptr, std::size_t pos, std::size_t size_) noexcept {
		std::memcpy(ptr, reinterpret_cast<const std::byte*>(object_) + pos, size_);
		return size_;
	}) { }

input_bytes::input_bytes(const std::string_view s) noexcept :
	input_bytes(reinterpret_cast<const std::byte*>(s.data()), s.size()) {
}

input_bytes::input_bytes(const std::span<const std::byte> s) noexcept :
	input_bytes(reinterpret_cast<const std::byte*>(s.data()), s.size()) {
}

input_bytes::input_bytes(std::istream& s) noexcept :
	object(&s) {

	const auto cur_g = s.tellg();
	s.seekg(0, std::ios::end);
	const auto end_g = s.tellg();
	s.seekg(0, std::ios::beg);

	input_offset = cur_g < 0 ? 0 : static_cast<std::size_t>(cur_g);
	input_size = end_g < 0 ? 0 : static_cast<std::size_t>(end_g);

	read_fn = +[](void* object, std::byte* ptr, std::size_t pos, std::size_t size) noexcept {
		auto& ss = *reinterpret_cast<std::istream*>(object);

		const auto g = ss.tellg();
		if (static_cast<std::size_t>(g) != pos)
			ss.seekg(pos);

		ss.read(reinterpret_cast<char*>(ptr), size);

		return static_cast<std::size_t>(ss.gcount());
	};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
