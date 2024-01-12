// Project: libv.re, File: src/libv/re/resource/model_load_event.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/res/common_resource_event.hpp>

#include <optional>
#include <string>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct ModelLoadSuccess {
	ModelID id = 0;
	Model_ptr model;
};

struct ModelLoadFailure {
	ModelID id = 0;
	Model_ptr model;

	struct IOFailure {
		std::vector<libv::res::ResourceMappingUnmatched> unmatchedMappings;
		std::vector<libv::res::ResourceMappingError> mappingErrors;
		std::vector<libv::res::LoadFilePhysicalError> physicalErrors;
	};

	std::optional<IOFailure> ioFailure;
	std::optional<std::string> loadFailure; /// Failure reason
};

struct ModelUnload {
	ModelID id = 0;
	Model_ptr model;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
