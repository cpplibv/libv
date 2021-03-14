// Project: libv.utility, File: src/libv/utility/hex_dump.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/utility/hex_dump.hpp>
// std
#include <iomanip>
#include <sstream>


namespace libv {

// -------------------------------------------------------------------------------------------------

std::string hex_dump(const std::string_view& s) {
	std::ostringstream ss;
	ss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase;
	for (size_t i = 0; i < s.size();) {
		ss << std::setw(2) << uint16_t{static_cast<unsigned char>(s[i])};
		++i;
		if (i % 32 == 0)
			ss << "\n";
		else if (i % 8 == 0)
			ss << "  ";
		else
			ss << ' ';
	}

	return std::move(ss).str();
}

std::string hex_dump(const std::span<const std::byte> s) {
	return hex_dump(std::string_view(reinterpret_cast<const char*>(s.data()), s.size()));
}

std::string hex_dump_with_ascii(const std::string_view& s) {
	if (s.empty())
		return "";

	std::ostringstream ss;
	ss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase;

	size_t i = 0;
	while (true) {
		ss << std::setw(2) << uint16_t{static_cast<unsigned char>(s[i])};
		++i;

		if (i == s.size())
			break;

		if (i % 32 == 0) {
			ss << " | ";
			for (size_t j = i - 32; j < i;) {
				ss << (std::isprint(s[j]) ? s[j] : '.');
				++j;
				if (j % 8 == 0 && j % 32 != 0)
					ss << ' ';
			}
			ss << "\n";
		} else if (i % 8 == 0) {
			ss << "  ";
		} else {
			ss << ' ';
		}
	}

	if (i % 32 != 0) {
		const auto r_begin = i;
		const auto r_end = (i / 32 + 1) * 32;

		for (size_t r = r_begin; r < r_end; ++r) {
			if (r % 32 == 0) {
				ss << " ";
			} else if (r % 8 == 0) {
				ss << "    ";
			} else {
				ss << "   ";
			}
		}
	}

	ss << " | ";
	for (size_t j = (i - 1) / 32 * 32; j < i;) {
		ss << (std::isprint(s[j]) ? s[j] : '.');
		++j;
		if (j % 8 == 0 && j % 32 != 0)
			ss << ' ';
	}

	return std::move(ss).str();
}

std::string hex_dump_with_ascii(const std::span<const std::byte> s) {
	return hex_dump_with_ascii(std::string_view(reinterpret_cast<const char*>(s.data()), s.size()));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
