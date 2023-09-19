// Project: libv.re, File: src/libv/re/core/glsl/glsl_compose.hpp

#pragma once

#include <libv/re/core/glsl/glsl_source_code.hpp>
#include <libv/utility/function_ref.hpp>

#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using IncludeLoadFunc = libv::function_ref<std::string(const std::string_view includePath, const std::vector<std::pair<std::string, int>>& includeStack)>;

// -------------------------------------------------------------------------------------------------

[[nodiscard]] GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<const std::string> defines, std::span<const std::string> includes);
[[nodiscard]] GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<std::span<const std::string>> defines_list, std::span<const std::string> includes_list);
[[nodiscard]] GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<const std::string> defines, std::span<std::span<const std::string>> includes = {});
[[nodiscard]] GLSLSourceCode glsl_compose_from_include(const IncludeLoadFunc& loader, const std::string_view main_path, std::span<std::span<const std::string>> defines_list = {}, std::span<std::span<const std::string>> includes_list = {});

[[nodiscard]] GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<const std::string> defines, std::span<const std::string> includes);
[[nodiscard]] GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<std::span<const std::string>> defines_list, std::span<const std::string> includes_list);
[[nodiscard]] GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<const std::string> defines, std::span<std::span<const std::string>> includes = {});
[[nodiscard]] GLSLSourceCode glsl_compose_from_source(const IncludeLoadFunc& loader, const std::string_view main_source, std::span<std::span<const std::string>> defines_list = {}, std::span<std::span<const std::string>> includes_list = {});

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
