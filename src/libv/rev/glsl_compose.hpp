// Project: libv, File: app/space/glsl_include_engine.hpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <span>
#include <string>
#include <string_view>
#include <system_error>
// libv
#include <libv/utility/function_ref.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct [[nodiscard]] IncludeResult {
	std::error_code ec;
	std::string result; /// If there was an error the file path, otherwise the include payload
};

using IncludeLoadFunc = libv::function_ref<IncludeResult(const std::string_view)>;

// -------------------------------------------------------------------------------------------------

struct glsl_failed_include_exception : public std::exception {
	// For additional mapping info, use glsl_source_loader's API

	std::string file_path;
	std::error_code ec;

	std::string include_path;
	/// include_path : line_number
	/// frame at index 0 is the most inner frame
	std::vector<std::pair<std::string, int>> include_stack;

public:
	glsl_failed_include_exception(std::string file_path, const std::error_code& ec, std::string include_path, std::vector<std::pair<std::string, int>> include_stack) :
			file_path(std::move(file_path)),
			ec(ec),
			include_path(std::move(include_path)),
			include_stack(std::move(include_stack)) {}

public:
	virtual const char* what() const noexcept override {
		return "glsl_failed_include_exception";
	}
};

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<const std::string> defines = {}, std::span<const std::string> includes = {});
[[nodiscard]] std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<std::span<const std::string>> defines_list = {}, std::span<const std::string> includes_list = {});
[[nodiscard]] std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<const std::string> defines = {}, std::span<std::span<const std::string>> includes = {});
[[nodiscard]] std::string glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<std::span<const std::string>> defines_list = {}, std::span<std::span<const std::string>> includes_list = {});

[[nodiscard]] std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<const std::string> defines = {}, std::span<const std::string> includes = {});
[[nodiscard]] std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<std::span<const std::string>> defines_list = {}, std::span<const std::string> includes_list = {});
[[nodiscard]] std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<const std::string> defines = {}, std::span<std::span<const std::string>> includes = {});
[[nodiscard]] std::string glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<std::span<const std::string>> defines_list = {}, std::span<std::span<const std::string>> includes_list = {});

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
