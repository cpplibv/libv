// Project: libv.rev, File: src/libv/rev/internal_shader.cpp

// fwd
#include <libv/rev/fwd.hpp>
// hpp
#include <libv/rev/internal_shader.hxx>
// libv
#include <libv/algo/slice.hpp>
// pro
#include <libv/rev/internal_shader_loader.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

InternalShader::InternalShader(
		std::weak_ptr<InternalShaderLoader> loader,
		std::unique_ptr<detail::BaseUniformContainer>&& uniformContainer,
		libv::type_uid uniformTID) :
	uniformContainer(std::move(uniformContainer)),
	loader(std::move(loader)),
	uniformTID(uniformTID) {
}

InternalShader::~InternalShader() {
	// For the sake of forward declared unique_ptr

	if (const auto loader_sp = loader.lock())
		loader_sp->unsubscribe(this);
}

void InternalShader::finish() {
	name_ = [this] { // Generate Shader Name

		const auto view_basename = [](std::string_view path) {
			const auto last_slash = path.find_last_of('/');
			if (last_slash != std::string::npos)
				path = libv::slice_off_view(path, last_slash + 1);

			auto last_dot = path.find_last_of('.');
			if (last_dot == std::string::npos)
				last_dot = path.size();

			return libv::slice_view(path, static_cast<int64_t>(last_dot));
		};

		if (stages.empty())
			return std::string("<empty>");

		bool matching_basenames = true;
		const auto first_basename = view_basename(stages.front().source_path);
		for (std::size_t i = 1; i < stages.size(); ++i)
			matching_basenames &= first_basename == view_basename(stages[i].source_path);

		if (matching_basenames)
			return std::string(first_basename);

		std::ostringstream oss;
		bool first = true;
		for (const auto& stage : stages) {
			const auto need_type_spec = !stage.source_path.ends_with(libv::gl::to_string_ext(stage.type));

			if (!first)
				oss << '|';
			else
				first = false;

			if (need_type_spec)
				oss << libv::gl::to_string_short(stage.type) << ':';

			oss << stage.source_path;
		}

		return std::move(oss).str();
	}();
}

int InternalShader::compare(libv::type_uid uniformTID, libv::gl::ShaderType t0, const std::string& p0, libv::gl::ShaderType t1, const std::string& p1) const {
	if (this->uniformTID < uniformTID) return -1;
	if (this->uniformTID > uniformTID) return 1;

	if (stages.size() < 2) return -1;
	if (stages.size() > 2) return 1;

	if (stages[0].type < t0) return -1;
	if (stages[0].type > t0) return 1;

	if (stages[0].source_path < p0) return -1;
	if (stages[0].source_path > p0) return 1;

	if (stages[1].type < t1) return -1;
	if (stages[1].type > t1) return 1;

	if (stages[1].source_path < p1) return -1;
	if (stages[1].source_path > p1) return 1;

	return 0;
}

int InternalShader::compare(libv::type_uid uniformTID, libv::gl::ShaderType t0, const std::string& p0, libv::gl::ShaderType t1, const std::string& p1, libv::gl::ShaderType t2, const std::string& p2) const {
	if (this->uniformTID < uniformTID) return -1;
	if (this->uniformTID > uniformTID) return 1;

	if (stages.size() < 2) return -1;
	if (stages.size() > 2) return 1;

	if (stages[0].type < t0) return -1;
	if (stages[0].type > t0) return 1;

	if (stages[0].source_path < p0) return -1;
	if (stages[0].source_path > p0) return 1;

	if (stages[1].type < t1) return -1;
	if (stages[1].type > t1) return 1;

	if (stages[1].source_path < p1) return -1;
	if (stages[1].source_path > p1) return 1;

	if (stages[2].type < t2) return -1;
	if (stages[2].type > t2) return 1;

	if (stages[2].source_path < p2) return -1;
	if (stages[2].source_path > p2) return 1;

	return 0;
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
