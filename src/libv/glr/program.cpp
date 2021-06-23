// Project: libv.glr, File: src/libv/glr/program.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/glr/program.hpp>
// ext
#include <range/v3/view/drop.hpp>
// libv
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/shader.hpp>
// pro
#include <libv/glr/destroy_queue.hpp>
#include <libv/glr/remote.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

void RemoteProgram::create(libv::gl::GL& gl, Remote& remote_) noexcept {
	gl(program).create();

	remote = make_observer_ptr(&remote_.destroyQueues());
	created = true;
}

void RemoteProgram::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	bool first_update = !created;

	if (!created)
		create(gl, remote_);

	if (dirty_source) {
		// TODO P5: cleanup glr shader a bit better, consider shader detach
		if (!first_update) {
			remote->gc(program);
			program.id = 0;
			gl(program).create();
		}

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
		for (const auto& pendingBinding : block_bindings
				| ranges::view::drop(bound_block_count)) {
			gl(program).uniformBlockBinding(pendingBinding.identifier.c_str(), pendingBinding.binding);
			bound_block_count++;
		}

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

Program::Program(std::shared_ptr<RemoteProgram> remote) noexcept :
	remote(std::move(remote)) { }

void Program::mark_for_full_reload() {
	remote->dirty = true;
	remote->dirty_source = true;
	remote->dirty_binding = true;
	remote->dirty_location = true;
	remote->uniform_locations.clear();
	remote->bound_block_count = 0;
}

void Program::vertex(std::string source) noexcept {
	remote->vertex = std::move(source);
	mark_for_full_reload();
}

void Program::geometry(std::string source) noexcept {
	remote->geometry = std::move(source);
	mark_for_full_reload();
}

void Program::fragment(std::string source) noexcept {
	remote->fragment = std::move(source);
	mark_for_full_reload();
}

void Program::compute(std::string source) noexcept {
	remote->compute = std::move(source);
	mark_for_full_reload();
}

void Program::tess_control(std::string source) noexcept {
	remote->tess_control = std::move(source);
	mark_for_full_reload();
}

void Program::tess_evaluation(std::string source) noexcept {
	remote->tess_evaluation = std::move(source);
	mark_for_full_reload();
}

void Program::block_binding(UniformBlockBinding binding) noexcept {
	remote->block_bindings.emplace_back(std::move(binding));

	remote->dirty = true;
	remote->dirty_binding = true;
}

void Program::assign(Uniform& uniform_, std::string identifier) noexcept {
	uniform_.location = static_cast<uint32_t>(remote->uniform_identifiers.size());
	remote->uniform_identifiers.emplace_back(std::move(identifier));

	remote->dirty = true;
	remote->dirty_location = true;
}

void Program::_native_swap(libv::gl::Program& program) noexcept {
	std::swap(remote->program, program);

	remote->vertex.clear();
	remote->geometry.clear();
	remote->fragment.clear();
	remote->compute.clear();
	remote->tess_control.clear();
	remote->tess_evaluation.clear();

	remote->uniform_identifiers.clear();
	remote->uniform_locations.clear();
	remote->bound_block_count = 0;
	remote->block_bindings.clear();

	remote->created = true;
	remote->dirty = true;
	remote->dirty_source = false;
	remote->dirty_binding = true;
	remote->dirty_location = true;
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
