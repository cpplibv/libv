// Project: libv.rev, File: src/libv/rev/resource/shader_loader.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/gl/fwd.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
// pro
#include <libv/rev/resource/shader.hpp>
#include <libv/rev/resource/shader_load_event.hpp>


// TODO P5: First load should always load immediately, but reloads could use a small heat-up delay (to not conflict with multiple file saving)

namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class InternalShaderLoader;

/// @Nexus Every update event is called under the same mutex
/// libv::rev::ShaderLoadSuccess
/// libv::rev::ShaderLoadFailure
/// libv::rev::ShaderUnload
class ShaderLoader {
	friend BaseShader;

private:
	std::shared_ptr<InternalShaderLoader> self;

public:
	ShaderLoader(libv::Nexus& nexus, std::filesystem::path base_include_directory);
	~ShaderLoader();

public:
	void add_include_directory(std::string include_dir, std::string filesystem_dir);

public:
	/// Can be called from any thread
	template <typename T, typename... Args>
	[[nodiscard]] inline Shader<T> load(Args&&... args);

	/// Loads and on-demand reloads any shader associated with this loader
	/// Must be called from the OpenGL thread
	void update(libv::GL& gl);

	/// Can be called from any thread, but the callback cannot reenter the object
	void foreach_shader(libv::function_ref<void(BaseShader)> func);
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename... Args>
inline Shader<T> ShaderLoader::load(Args&&... args) {
	return Shader<T>(*this, std::forward<Args>(args)...);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
