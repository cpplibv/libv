// File: ui_program.hpp Author: Vader Created on 2016. szeptember 6., 15:20

#pragma once

// libv
#include <libv/gl/program.hpp>
// pro
#include <libv/ui/resource/resource.hpp>
#include <libv/ui/resource/fwd.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class UIProgram : public Resource<UIProgram> {
private:
	ResourceStepResult load();
	ResourceStepResult unload();

public:
	static constexpr char ResourceLabel[] = "Program";
	static constexpr Step ResourceLoadSequence[] = {
		{load, ThreadAffinity::GL}
	};
	static constexpr Step ResourceUnloadSequence[] = {
		{unload, ThreadAffinity::GL}
	};

	// ---------------------------------------------------------------------------------------------

public:
	enum class Status : uint8_t {
		OK = 0,
		NotLoaded,
		ShaderError,
		LinkError,
		Unloaded,
	};
private:
	gl::Program program;
	std::string name;
	UIShader_view vs;
	UIShader_view fs;
	Status status_ = Status::NotLoaded;

public:
	gl::Uniform_mat4f uniformMVPmat;
	gl::Uniform_texture uniformTextureSampler;
	gl::Uniform_vec4f uniformColor;

public:
	Status status() const {
		return status_;
	}
	void use() {
		program.use();
	}
	UIProgram(std::string name, const UIShader_view& vs, const UIShader_view& fs);
	~UIProgram() { }

//public:
//	bool operator<(const UIProgram& rhs) const {
//		return name < rhs.name;
//	}
//	friend bool operator<(const std::string& t, const UIProgram& rhs) {
//		return t < rhs.name;
//	}
//	friend bool operator<(const UIProgram& lhs, const std::string& t) {
//		return lhs.name < t;
//	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv