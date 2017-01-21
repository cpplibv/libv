// File: %<%NAME%>%.%<%EXTENSION%>%, Created on %<%DATE%>% %<%TIME%>%, Author: %<%USER%>%

// hpp
#include <libv/ui/resource/shader.hpp>
// pro
#include <libv/ui/log.hpp>
#include <libv/ui/resource/file.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

UIShader::UIShader(const File_view& source) : name(source->path().string()), source(source) { }

ResourceStepResult UIShader::loadGL() {
	if (source->status() != File::Status::OK) {
		status_ = Status::SourceError;
		return ResourceStepResult::Failed;
	}

	if (source->path().extension() == ".vs")
		shader.create(gl::ShaderType::Vertex);
	else if (source->path().extension() == ".fs")
		shader.create(gl::ShaderType::Fragment);
	else
		LIBV_LOG_UI_RESOURCE_ERROR("Could not determine shader type: [%s]", source->path());

	shader.compile(source->data());

	if (!shader.status()) {
		status_ = Status::CompileError;
		return ResourceStepResult::Failed;
	}

	source.reset();
	return ResourceStepResult::OK;
}

ResourceStepResult UIShader::unloadGL() {
	shader.destroy();
	status_ = Status::Unloaded;
	return ResourceStepResult::OK;
}

constexpr const char UIShader::ResourceLabel[];
constexpr UIShader::Step UIShader::ResourceLoadSequence[];
constexpr UIShader::Step UIShader::ResourceUnloadSequence[];

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv