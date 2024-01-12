// Project: libv.re, File: src/libv/re/resource/texture_load_event.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/res/common_resource_event.hpp>

#include <optional>
#include <string>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct TextureLoadSuccess {
	TextureID id = 0;
	Texture_ptr texture;
};

struct TextureLoadFailure {
	TextureID id = 0;
	Texture_ptr texture;

	struct IOFailure {
		std::vector<libv::res::ResourceMappingUnmatched> unmatchedMappings;
		std::vector<libv::res::ResourceMappingError> mappingErrors;
		std::vector<libv::res::LoadFilePhysicalError> physicalErrors;
	};

	std::optional<IOFailure> ioFailure;
	std::optional<std::string> loadFailure; /// Failure reason
};

struct TextureUnload {
	TextureID id = 0;
	Texture_ptr texture;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
