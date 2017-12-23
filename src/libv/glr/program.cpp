// File:   program.cpp Author: Vader Created on 2019. január 25., 4:17

// hpp
#include <libv/glr/program.hpp>
// ext
#include <range/v3/view/drop.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/shader.hpp>
// pro
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

void RemoteProgram::create(libv::gl::GL& gl, Remote& remote_) noexcept {
	gl(program).create();

	remote = libv::make_observer(remote_);
	created = true;
}

void RemoteProgram::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (!created)
		create(gl, remote_);

	if (dirty_source) {
		const auto attach = [&](libv::gl::ShaderType type, const std::string& source) {
			if (!source.empty()) {
				libv::gl::Shader shader;
				gl(shader).create(type);
				gl(shader).compile(source);
				gl(program).attach(shader);
				gl(shader).destroy();
			}
		};

		attach(libv::gl::ShaderType::Vertex, vertex);
		attach(libv::gl::ShaderType::Geometry, geometry);
		attach(libv::gl::ShaderType::Fragment, fragment);
		attach(libv::gl::ShaderType::Compute, compute);
		attach(libv::gl::ShaderType::TessControl, tess_control);
		attach(libv::gl::ShaderType::TessEvaluation, tess_evaluation);

		gl(program).link();
		dirty_source = false;
	}

	if (dirty_binding) {
		for (const auto& pendingBinding : pending_bindings)
			gl(program).uniformBlockBinding(pendingBinding.identifier.c_str(), pendingBinding.binding);

		pending_bindings.clear();
		dirty_binding = false;
	}

	if (dirty_location) {
		for (const auto& identifier : uniform_identifiers
				| ranges::view::drop(uniform_locations.size())) {
			const auto location = gl(program).uniformLocation(identifier.c_str());
			uniform_locations.emplace_back(location);
		}

		dirty_location = false;
	}
}

void RemoteProgram::use(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (dirty)
		update(gl, remote_);

	gl(program).use();

	uniformStream.stream(uniform_locations);
}

RemoteProgram::~RemoteProgram() noexcept {
	if (remote)
		remote->gc(program);
}

// -------------------------------------------------------------------------------------------------

Program::Program() noexcept :
	remote(std::make_shared<RemoteProgram>()) { }

void Program::vertex(std::string source) noexcept {
	remote->vertex = std::move(source);
	remote->dirty = true;
	remote->dirty_source = true;
}

void Program::geometry(std::string source) noexcept {
	remote->geometry = std::move(source);
	remote->dirty = true;
	remote->dirty_source = true;
}

void Program::fragment(std::string source) noexcept {
	remote->fragment = std::move(source);
	remote->dirty = true;
	remote->dirty_source = true;
}

void Program::compute(std::string source) noexcept {
	remote->compute = std::move(source);
	remote->dirty = true;
	remote->dirty_source = true;
}

void Program::tess_control(std::string source) noexcept {
	remote->tess_control = std::move(source);
	remote->dirty = true;
	remote->dirty_source = true;
}

void Program::tess_evaluation(std::string source) noexcept {
	remote->tess_evaluation = std::move(source);
	remote->dirty = true;
	remote->dirty_source = true;
}

void Program::block_binding(UniformBlockBinding binding) noexcept {
	remote->pending_bindings.emplace_back(std::move(binding));
	remote->dirty = true;
	remote->dirty_binding = true;
}

void Program::assign(Uniform& uniform_, std::string identifier) noexcept {
	uniform_.location = static_cast<uint32_t>(remote->uniform_identifiers.size());
	remote->uniform_identifiers.emplace_back(std::move(identifier));
	remote->dirty = true;
	remote->dirty_location = true;
}


// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
