// Project: libv.utility, File: src/libv/utility/bytes/output_bytes.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/utility/bytes/output_bytes.hpp>
// std
#include <cstring>
#include <ostream>
#include <string>
#include <vector>


namespace libv {

// -------------------------------------------------------------------------------------------------

output_bytes::output_bytes(std::byte* data, size_t size) noexcept :
	object(data),
	output_offset(0),
	output_size(size),
	write_fn(+[](void* object_, const std::byte* ptr, size_t pos, size_t size_) noexcept {
		std::memcpy(reinterpret_cast<std::byte*>(object_) + pos, ptr, size_);
	}) { }

output_bytes::output_bytes(std::string& s) noexcept :
	object(&s),
	output_offset(s.size()),
	output_size(std::numeric_limits<std::size_t>::max()),
	write_fn(+[](void* object_, const std::byte* ptr, size_t pos, size_t size_) noexcept {
		auto& s = *reinterpret_cast<std::string*>(object_);
		if (s.size() < pos + size_)
			s.resize(pos + size_);
		std::memcpy(s.data() + pos, ptr, size_);
	}) { }

output_bytes::output_bytes(std::vector<std::byte>& s) noexcept :
	object(&s),
	output_offset(s.size()),
	output_size(std::numeric_limits<std::size_t>::max()),
	write_fn(+[](void* object_, const std::byte* ptr, size_t pos, size_t size_) noexcept {
		auto& s = *reinterpret_cast<std::vector<std::byte>*>(object_);
		if (s.size() < pos + size_)
			s.resize(pos + size_);
		std::memcpy(s.data() + pos, ptr, size_);
	}) { }

output_bytes::output_bytes(std::ostream& s) noexcept {
	object = &s;

	const auto cur_p = s.tellp();
	output_offset = cur_p < 0 ? 0 : static_cast<size_t>(cur_p);

	output_size = std::numeric_limits<std::size_t>::max();

	write_fn = +[](void* object, const std::byte* ptr, size_t pos, size_t size) noexcept {
		auto& ss = *reinterpret_cast<std::ostream*>(object);

		const auto g = ss.tellp();
		if (static_cast<size_t>(g) != pos)
			ss.seekp(pos);

		ss.write(reinterpret_cast<const char*>(ptr), size);
	};
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
