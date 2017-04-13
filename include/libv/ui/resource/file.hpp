// File: file.hpp Author: Vader Created on 2016. szeptember 6., 15:23

#pragma once

// ext
#include <boost/filesystem/path.hpp>
// std
#include <cassert>
#include <string>
// pro
#include <libv/ui/resource/resource.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct File : Resource<File> {
private:
	ResourceStepResult load();
	ResourceStepResult unload();

public:
	static constexpr char ResourceLabel[] = "File";
	static constexpr Step ResourceLoadSequence[] = {
		{load, ThreadAffinity::IO}
	};
	static constexpr Step ResourceUnloadSequence[] = {
		{unload, ThreadAffinity::IN}
	};

//	struct Descriptor : std::tuple<boost::filesystem::path> {
//		decltype(auto) path() const {
//			return std::get<0>(*this);
//		}
//	};

	// ---------------------------------------------------------------------------------------------
public:
	enum class Status {
		OK = 0,
		InvalidPath,
		FailedOpen,
		Unknown,
	};

private:
	std::string data_;
	Status status_ = Status::Unknown;
	boost::filesystem::path path_;
	//Descriptor descriptor;

public:
//	File(const boost::filesystem::path& descriptor);
	File(const boost::filesystem::path& path_);

public:
	const std::string& data() const {
		return data_;
	}
	const char* c_data() const {
		return data_.data();
	}
	size_t size() const {
		return data_.size();
	}
	const boost::filesystem::path& path() const {
		//return descriptor.path();
		return path_;
	}
	Status status() const {
		return status_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
