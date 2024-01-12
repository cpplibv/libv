// Project: libv.re, File: src/libv/re/resource/style_load_event.hpp

#pragma once

#include <libv/res/common_resource_event.hpp>

#include <optional>
#include <string>
#include <vector>
#include <stdexcept>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

using StyleFileID = std::size_t;

struct StyleFileLoadSuccess {
	bool firstLoad = false;
	StyleFileID id = 0;
	std::string fileIdentifier;
};

struct StyleFileLoadFailure {
	StyleFileID id = 0;
	std::string fileIdentifier;
	/// frame at index 0 is the most outer frame
	std::vector<std::string> includeStack;

	struct IncludeFailure {
		std::string includePath;
		std::vector<libv::res::ResourceMappingUnmatched> unmatchedMappings;
		std::vector<libv::res::ResourceMappingError> mappingErrors;
		std::vector<libv::res::LoadFilePhysicalError> physicalErrors;
	};

	struct LuaError {
		std::string message;
	};

	struct StyleError {
		std::string message;
	};

	std::optional<IncludeFailure> includeFailure;
	std::optional<LuaError> luaError;
	std::optional<StyleError> styleError;
};

struct StyleFileUnload {
	StyleFileID id = 0;
};

// -------------------------------------------------------------------------------------------------

struct StyleFileLoadException : public std::exception {
	StyleFileLoadFailure failure;

	template <typename... Args>
	StyleFileLoadException(Args&&... args) :
		failure{std::forward<Args>(args)...} {
	}

	const char* what() const noexcept override {
		return "StyleFileLoadException";
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
