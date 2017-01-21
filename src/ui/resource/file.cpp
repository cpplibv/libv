// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <libv/ui/resource/file.hpp>
// libv
#include <libv/read_file.hpp>
// pro
#include <libv/ui/log.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

namespace {

File::Status convertRFEtoFS(ReadFileError e) {
	switch (e) {
	case ReadFileError::OK: return File::Status::OK;
	case ReadFileError::InvalidPath: return File::Status::InvalidPath;
	case ReadFileError::FailedOpen: return File::Status::FailedOpen;
	default: return File::Status::Unknown;
	}
}

} // namespace

// -------------------------------------------------------------------------------------------------

ResourceStepResult File::load() {
	LIBV_LOG_UI_RESOURCE_TRACE("Load file [%s]", path_);
	ReadFileError ec;
	data_ = read_file(path_, ec);
	status_ = convertRFEtoFS(ec);
	return (ec == ReadFileError::OK) ? ResourceStepResult::OK : ResourceStepResult::Failed;
}

ResourceStepResult File::unload() {
	LIBV_LOG_UI_RESOURCE_TRACE("Unload file [%s]", path_);
	data_.clear();
	status_ = Status::Unknown;
	return ResourceStepResult::OK;
}

constexpr const char File::ResourceLabel[];
constexpr File::Step File::ResourceLoadSequence[];
constexpr File::Step File::ResourceUnloadSequence[];

// -------------------------------------------------------------------------------------------------

File::File(const boost::filesystem::path& path_) : path_(path_) { }

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
