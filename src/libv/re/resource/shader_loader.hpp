// Project: libv.re, File: src/libv/re/resource/shader_loader.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>
#include <libv/re/r.hpp> // Convenience include to access the libv::re::r.shader object
#include <libv/re/resource/common_resource.hpp>
#include <libv/utility/function_ref.hpp>

#include <memory>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct DelayedShaderLoadGuard {
	using Resource_ptr = Shader_ptr;
	Resource_ptr resource;
	LoadingMode mode;
	bool requestLoadOnDestroy = false;

	DelayedShaderLoadGuard(Resource_ptr&& resource, LoadingMode mode, bool requestLoadOnDestroy) :
			resource(std::move(resource)), mode(mode), requestLoadOnDestroy(requestLoadOnDestroy) {}

	DelayedShaderLoadGuard(const DelayedShaderLoadGuard&) = delete;
	DelayedShaderLoadGuard& operator=(const DelayedShaderLoadGuard&) & = delete;
	DelayedShaderLoadGuard(DelayedShaderLoadGuard&&) = delete;
	DelayedShaderLoadGuard& operator=(DelayedShaderLoadGuard&&) & = delete;

	inline Resource_ptr::value_type* operator->() const {
		return resource.get();
	}

	inline Resource_ptr& operator*() {
		return resource;
	}

	inline explicit(false) operator const Resource_ptr&() const& {
		return resource;
	}

	[[nodiscard]] constexpr inline bool isFirstLoad() const noexcept {
		return requestLoadOnDestroy;
	}

	/// The destructor dispatches the async loading of the resource
	~DelayedShaderLoadGuard();
};

// -------------------------------------------------------------------------------------------------

class ImplShaderLoader;

/// @Nexus emit global libv::re::ShaderLoadSuccess
/// @Nexus emit global libv::re::ShaderLoadFailure
/// @Nexus emit global libv::re::ShaderUnload
/// Every nexus event is called under the same mutex
class ShaderLoader {
private:
	std::unique_ptr<ImplShaderLoader> self;

public:
	ShaderLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl);
	void init();
	void reset();
	~ShaderLoader();

public:
	/// @thread any
	Shader_ptr load(
			std::string_view path_cs, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Shader_ptr load(std::string_view path_vs, std::string_view path_fs, VariantID variant = VariantID::Default,
			LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Shader_ptr load(std::string_view path_vs, std::string_view path_gs, std::string_view path_fs,
			VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Shader_ptr load(libv::gl::ShaderType type0, std::string_view path0, VariantID variant = VariantID::Default,
			LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Shader_ptr load(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
			std::string_view path1, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Shader_ptr load(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
			std::string_view path1, libv::gl::ShaderType type2, std::string_view path2,
			VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);

	/// @thread any
	Shader_ptr load_async(std::string_view path_cs, VariantID variant = VariantID::Default);
	/// @thread any
	Shader_ptr load_async(std::string_view path_vs, std::string_view path_fs, VariantID variant = VariantID::Default);
	/// @thread any
	Shader_ptr load_async(std::string_view path_vs, std::string_view path_gs, std::string_view path_fs,
			VariantID variant = VariantID::Default);
	/// @thread any
	Shader_ptr load_async(libv::gl::ShaderType type0, std::string_view path0, VariantID variant = VariantID::Default);
	/// @thread any
	Shader_ptr load_async(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
			std::string_view path1, VariantID variant = VariantID::Default);
	/// @thread any
	Shader_ptr load_async(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
			std::string_view path1, libv::gl::ShaderType type2, std::string_view path2,
			VariantID variant = VariantID::Default);

	/// @thread any
	DelayedShaderLoadGuard load_delayed(
			std::string_view path_cs, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedShaderLoadGuard load_delayed(std::string_view path_vs, std::string_view path_fs,
			VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedShaderLoadGuard load_delayed(std::string_view path_vs, std::string_view path_gs, std::string_view path_fs,
			VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedShaderLoadGuard load_delayed(libv::gl::ShaderType type0, std::string_view path0,
			VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedShaderLoadGuard load_delayed(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
			std::string_view path1, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedShaderLoadGuard load_delayed(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
			std::string_view path1, libv::gl::ShaderType type2, std::string_view path2,
			VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);

	/// @thread any
	DelayedShaderLoadGuard load_delayed_async(std::string_view path_cs, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedShaderLoadGuard load_delayed_async(
			std::string_view path_vs, std::string_view path_fs, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedShaderLoadGuard load_delayed_async(std::string_view path_vs, std::string_view path_gs,
			std::string_view path_fs, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedShaderLoadGuard load_delayed_async(
			libv::gl::ShaderType type0, std::string_view path0, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedShaderLoadGuard load_delayed_async(libv::gl::ShaderType type0, std::string_view path0,
			libv::gl::ShaderType type1, std::string_view path1, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedShaderLoadGuard load_delayed_async(libv::gl::ShaderType type0, std::string_view path0,
			libv::gl::ShaderType type1, std::string_view path1, libv::gl::ShaderType type2, std::string_view path2,
			VariantID variant = VariantID::Default);

private:
	friend BasicResource;
	void executeLoad(Shader_ptr shader, LoadingMode mode); // Hook to execute resource loading
	void unload(Shader* shader); // Hook to unregister from the cache

public:
	/// @thread any, but the callback cannot reenter the object as a mutex is held during the whole iteration
	void foreach_shader(libv::function_ref<void(const Shader_ptr&)> func);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
