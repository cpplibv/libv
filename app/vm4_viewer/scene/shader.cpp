// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

// hpp
#include <vm4_viewer/scene/shader.hpp>
// libv
#include <libv/utility/concat.hpp>
// pro
#include <vm4_viewer/shader_loader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

BaseShader::BaseShader(ShaderLoader& loader, update_signature update_ptr) :
	loader(loader),
	update_ptr(update_ptr) {
	loader.subscribe(*this);
}

BaseShader::~BaseShader() {
	loader.unsubscribe(*this);
}

void BaseShader::addStage(libv::gl::ShaderType type, std::string path) {
	stages.emplace_back(type, std::move(path));
}

std::string BaseShader::name() const {
	std::string result;

	bool first = true;
	for (const auto& stage : stages) {
		if (first)
			result = libv::concat(std::move(result), libv::gl::to_string_short(stage.type), ":", stage.path);
		else
			result = libv::concat(std::move(result), "|", libv::gl::to_string_short(stage.type), ":", stage.path);

		first = false;
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

} // namespace app
