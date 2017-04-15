// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <libv/ui/resource/program.hpp>
// pro
#include <libv/ui/resource/shader.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

UIProgram::UIProgram(std::string name, const UIShader_view& vs, const UIShader_view& fs) :
	name(name),
	vs(vs),
	fs(fs) {

	//		dependency(vs);
	//		dependency(fs);
}

ResourceStepResult UIProgram::load() {
	if ((vs && vs->status() != UIShader::Status::OK) ||
			(fs && fs->status() != UIShader::Status::OK)) {
		status_ = Status::ShaderError;
	}

	program.createLink(vs->native(), fs->native());

	if (!program.status()) {
		status_ = Status::LinkError;
	}

	program.assign(uniformMVPmat, "MVPmat");
	program.assign(uniformTextureSampler, "textureSampler");
	program.assign(uniformColor, "color");

	status_ = Status::OK;
	return ResourceStepResult::OK;
}

ResourceStepResult UIProgram::unload() {
	program.destroy();
	vs.reset();
	fs.reset();
	status_ = Status::Unloaded;
	return ResourceStepResult::OK;
}

constexpr char UIProgram::ResourceLabel[];
constexpr UIProgram::Step UIProgram::ResourceLoadSequence[];
constexpr UIProgram::Step UIProgram::ResourceUnloadSequence[];

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
