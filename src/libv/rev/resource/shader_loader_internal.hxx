// Project: libv.rev, File: src/libv/rev/internal_shader_loader.lpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/fsw/watcher.hpp>
#include <libv/mt/queue_unique.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/type_uid.hpp>
// std
#include <mutex>
#include <vector>
// pro
#include <libv/rev/glsl/glsl_source_loader.hpp>
#include <libv/rev/resource/shader_base.hpp>
#include <libv/rev/resource/shader_load_event.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class InternalShaderLoader : public std::enable_shared_from_this<InternalShaderLoader> {
public:
	const std::shared_ptr<InternalShader>* current_loading_internal = nullptr; /// Temporal variable used by file tracker callback during loading
	InternalShaderStage* current_loading_stage = nullptr; /// Temporal variable used by file tracker callback during loading

	GLSLSourceLoader glsl_source_loader;

	libv::fsw::Watcher watcher;

	std::mutex mutex;
	std::vector<InternalShader*> shaders;

	libv::mt::queue_unique<std::shared_ptr<InternalShader>> queue_source_reload;
	libv::mt::queue_unique<std::shared_ptr<InternalShader>> queue_shader_failed_load;
	libv::mt::queue_unique<std::shared_ptr<InternalShader>> queue_shader_update;

	libv::Nexus nexus;

public:
	explicit InternalShaderLoader(Nexus& nexus) :
		nexus(nexus) {}

public:
	inline void _broadcast(const ShaderLoadSuccess& e) {
		nexus.broadcast_global(e);
	}
	inline void _broadcast(const ShaderLoadFailure& e) {
		nexus.broadcast_global(e);
	}
	inline void _broadcast(const ShaderUnload& e) {
		nexus.broadcast_global(e);
	}

public:
	using ucc_type = std::unique_ptr<detail::BaseUniformContainer>(*)(); /// Uniform container creator function
	[[nodiscard]] std::shared_ptr<InternalShader> internal_load(libv::type_uid uniformTID, ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1);
	[[nodiscard]] std::shared_ptr<InternalShader> internal_load(libv::type_uid uniformTID, ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2);

public:
	void unsubscribe(InternalShader* internal_shader);

public:
	void update_fs();
	void update_gl(libv::gl::GL& gl);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
