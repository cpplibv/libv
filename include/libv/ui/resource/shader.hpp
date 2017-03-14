// File:   ui_shader.hpp Author: Vader Created on 2016. szeptember 6., 15:20

#pragma once

// libv
#include <libv/gl/shader.hpp>
// pro
#include <libv/ui/resource/resource.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class UIShader : public Resource<UIShader> {
private:
	ResourceStepResult loadGL();
	ResourceStepResult unloadGL();
public:
	static constexpr const char ResourceLabel[] = "Shader";
	static constexpr Step ResourceLoadSequence[] = {
		{loadGL, ThreadAffinity::GL}
	};
	static constexpr Step ResourceUnloadSequence[] = {
		{unloadGL, ThreadAffinity::GL}
	};

	// ---------------------------------------------------------------------------------------------

public:
	enum class Status : uint8_t {
		OK = 0,
		NotLoaded,
		SourceError,
		CompileError,
		Unloaded,
	};

private:
	gl::Shader shader;
	std::string name;
	File_view source;
	Status status_ = Status::NotLoaded;

public:
	UIShader(const File_view& source);

public:
	auto& native() const {
		return shader;
	}
	Status status() const {
		return status_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
