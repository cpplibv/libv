// Project: libv.utility, File: src/libv/utility/read_file_stop.cpp

// hpp
#include <libv/utility/read_file_stop.hpp>
// std
#include <fstream>


namespace libv {

// -------------------------------------------------------------------------------------------------

ReadFileResult fn_read_file_stop::operator()(std::stop_token st, const std::filesystem::path& filepath) const noexcept {
	return operator()(st, filepath, default_read_file_stop_chunk_size);
}

ReadFileResult fn_read_file_stop::operator()(std::stop_token st, const std::filesystem::path& filepath, size_t chunk_size) const noexcept {
	ReadFileResult result;
	std::ifstream file(filepath, std::ios::binary | std::ios::in | std::ios::ate);

	if (!file) {
		result.ec = std::make_error_code(static_cast<std::errc>(errno));
		result.data.clear();
		return result;
	}

	const auto size = file.tellg();
	file.seekg(0);

	if (file.fail()) {
		result.ec = std::make_error_code(static_cast<std::errc>(errno));
		result.data.clear();
		return result;
	}

	if (st.stop_requested()) {
		result.ec = std::make_error_code(std::errc::operation_canceled);
		return result;
	}

	result.data.resize(size);

	auto ptr = reinterpret_cast<char*>(result.data.data());
	const auto ptr_end = ptr + size;
	while (ptr + chunk_size < ptr_end) {
		if (st.stop_requested()) {
			result.ec = std::make_error_code(std::errc::operation_canceled);
			return result;
		}

		file.read(ptr, chunk_size);
		ptr += chunk_size;

		if (file.fail()) {
			result.ec = std::make_error_code(static_cast<std::errc>(errno));
			result.data.clear();
			return result;
		}
	}

	if (st.stop_requested()) {
		result.ec = std::make_error_code(std::errc::operation_canceled);
		return result;
	}

	const auto leftover_size = ptr_end - ptr;
	file.read(ptr, leftover_size);

	if (file.fail()) {
		result.ec = std::make_error_code(static_cast<std::errc>(errno));
		result.data.clear();
		return result;
	}

	result.ec.clear();
	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
