// Project: libv.rev, File: src/libv/rev/shader_loader.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// libv
#include <libv/gl/gl_fwd.hpp>
#include <libv/utility/function_ref.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
// pro
#include <libv/rev/shader.hpp>
#include <libv/rev/shader_load_event.hpp>


// TODO P5: First load should always load immediately, but reloads could use a small heat-up delay (to not conflict with multiple file saving)

namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class ShaderLoader {
	friend class BaseShader;

private:
	std::shared_ptr<class InternalShaderLoader> self;

public:
	explicit ShaderLoader(std::filesystem::path base_include_directory);
	~ShaderLoader();

public:
	/// Can be called from any thread
	template <typename T, typename... Args>
	[[nodiscard]] inline Shader<T> load(Args&&... args);

	/// Loads and on-demand reloads any shader associated with this loader
	/// Must be called from the OpenGL thread
	void update(libv::gl::GL& gl);

	/// Can be called from any thread, but the callback cannot reenter the object
	void foreach_shader(libv::function_ref<void(BaseShader)> func);

public:
	/// Every update event is called under the same mutex
	void on_success(shader_load_success_cb success_cb);
	/// Every update event is called under the same mutex
	void on_failure(shader_load_failure_cb failure_cb);
	/// Every update event is called under the same mutex
	void on_unload(shader_unload_cb unload_cb);
	void clear_on_updates();
};

// -------------------------------------------------------------------------------------------------

template <typename T, typename... Args>
inline Shader<T> ShaderLoader::load(Args&&... args) {
	return Shader<T>(*this, std::forward<Args>(args)...);
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
