// Project: libv, File: app/space/shader.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/rev/shader.hpp>
// libv
#include <libv/utility/concat.hpp>
// pro
#include <vm4_viewer/shader_loader.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class InternalShader {
private:
	struct ShaderStage {
		libv::gl::ShaderType type;
		std::string source_path;
	};

private:
	libv::glr::Program glr;
	std::unique_ptr<class BaseUniformContainer> uniformContainer;

	class ShaderLoader& loader;

	std::vector<ShaderStage> stages;
//	std::vector<libv::fsw::WatchToken> fsw_tokens;
//	boost::container::flat_set<std::string> source_files;

public:
	InternalShader(ShaderLoader& loader, std::unique_ptr<BaseUniformContainer>&& uniformContainer) :
			uniformContainer(std::move(uniformContainer)),
			loader(loader) {
		loader.subscribe(this);
	}
	~InternalShader() {
		loader.unsubscribe(this);
	}
};


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

} // namespace rev
} // namespace libv
