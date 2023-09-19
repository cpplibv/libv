// Project: libv.re, File: src/libv/re/core/glsl/glsl_source_code.cpp

// hpp
#include <libv/re/core/glsl/glsl_source_code.hpp>
// libv
#include <libv/algo/slice.hpp>
#include <libv/range/view_lines_string_view.hpp>
// std
#include <sstream>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

namespace {

struct ProcessResult {
	bool matched = false;
	std::string_view line_number;
	std::string_view message;
};

auto process_filename_index_nvidia(std::string_view line, std::string_view index_str) {
	ProcessResult result;

	// NVIDIA syntax:
	// {file_index}({line}) : error {error_code}: {description}

	if (!line.starts_with(index_str))
		return result;
	line.remove_prefix(index_str.length());

	if (!line.starts_with('('))
		return result;
	line.remove_prefix(1);

	auto close_parenthesis = line.find(')');
	if (close_parenthesis == std::string_view::npos)
		return result;

	result.line_number = libv::slice_view(line, close_parenthesis);
	line = libv::slice_off_view(line, close_parenthesis);

	if (!line.starts_with(") : "))
		return result;
	line.remove_prefix(4);

	result.message = line;
	result.matched = true;

	return result;
}

auto process_filename_index_intel(std::string_view line, std::string_view index_str) {
	ProcessResult result;

	// Intel syntax:
	// ERROR: {file_index}:{line}: '{some_id}' : {description}

	if (!line.starts_with("ERROR: "))
		return result;
	line.remove_prefix(7);

	if (!line.starts_with(index_str))
		return result;
	line.remove_prefix(index_str.length());

	if (!line.starts_with(':'))
		return result;
	line.remove_prefix(1);

	auto close_colon = line.find(':');
	if (close_colon == std::string_view::npos)
		return result;

	result.line_number = libv::slice_view(line, static_cast<int64_t>(close_colon));
	line = libv::slice_off_view(line, static_cast<int64_t>(close_colon));

	if (!line.starts_with(": "))
		return result;
	line.remove_prefix(2);

	result.message = line;
	result.matched = true;

	return result;
}

auto process_filename_index(std::string_view line, int index) {
	ProcessResult result;

	const auto index_str = std::to_string(index);

	result = process_filename_index_nvidia(line, index_str);
	if (result.matched)
		return result;

	result = process_filename_index_intel(line, index_str);
	if (result.matched)
		return result;

	return result;
}

} // namespace -------------------------------------------------------------------------------------

std::string GLSLSourceCode::fixupErrorMessage(std::string_view errorMessage) const {
	std::ostringstream os;

	for (std::string_view line : errorMessage | libv::view::lines_string_view()) {
		bool matchedFileIndex = false;

		// TODO P5: Instead of foreach process_filename_index could parse the index out, and use that for lookup
		for (const auto& [index, filename] : mappingIndexToFilename) {
			auto r = process_filename_index(line, index);
			if (!r.matched)
				continue;

			os << filename << ":" << r.line_number << ": " << r.message;
			matchedFileIndex = true;
			break;
		}

		if (!matchedFileIndex)
			os << line;
	}

	return std::move(os).str();
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
