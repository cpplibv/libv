// Project: libv.re, File: src/libv/re/resource/model_loader.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/r.hpp> // IWYU pragma: export // Convenience include to access the libv::re::r.model object
#include <libv/re/resource/common_resource.hpp>
#include <libv/re/resource/material_func.hpp>
#include <libv/utility/function_ref.hpp>
#include <libv/utility/nexus_fwd.hpp>
#include <libv/vm4/fwd.hpp>

#include <memory>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct DelayedModelLoadGuard {
	using Resource_ptr = Model_ptr;
	Resource_ptr resource;
	LoadingMode mode;
	bool requestLoadOnDestroy = false;

	DelayedModelLoadGuard(Resource_ptr&& resource, LoadingMode mode, bool requestLoadOnDestroy) :
			resource(std::move(resource)), mode(mode), requestLoadOnDestroy(requestLoadOnDestroy) {}

	DelayedModelLoadGuard(const DelayedModelLoadGuard&) = delete;
	DelayedModelLoadGuard& operator=(const DelayedModelLoadGuard&) & = delete;
	DelayedModelLoadGuard(DelayedModelLoadGuard&&) = delete;
	DelayedModelLoadGuard& operator=(DelayedModelLoadGuard&&) & = delete;

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
	~DelayedModelLoadGuard();
};

// -------------------------------------------------------------------------------------------------

class ImplModelLoader;

/// Builtin models:
///     builtin:cube
///
/// @Nexus emit global libv::re::ModelLoadSuccess
/// @Nexus emit global libv::re::ModelLoadFailure
/// @Nexus emit global libv::re::ModelUnload
class ModelLoader {
private:
	std::unique_ptr<ImplModelLoader> self;

public:
	ModelLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl);
	void init();
	void reset();
	~ModelLoader();

public:
	/// @thread any
	Model_ptr load(std::string_view identifier, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Model_ptr load(std::string_view identifier, Material_ptr matOverride, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Model_ptr load(std::string_view identifier, MaterialScanFunc matOverride, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Model_ptr load(std::string_view identifier, MaterialLoaderFunc matOverride, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	Model_ptr load_async(std::string_view identifier, VariantID variant = VariantID::Default);
	/// @thread any
	Model_ptr load_async(std::string_view identifier, Material_ptr matOverride, VariantID variant = VariantID::Default);
	/// @thread any
	Model_ptr load_async(std::string_view identifier, MaterialScanFunc matOverride, VariantID variant = VariantID::Default);
	/// @thread any
	Model_ptr load_async(std::string_view identifier, MaterialLoaderFunc matOverride, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedModelLoadGuard load_delayed(std::string_view identifier, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedModelLoadGuard load_delayed(std::string_view identifier, Material_ptr matOverride, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedModelLoadGuard load_delayed(std::string_view identifier, MaterialScanFunc matOverride, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedModelLoadGuard load_delayed(std::string_view identifier, MaterialLoaderFunc matOverride, VariantID variant = VariantID::Default, LoadingMode mode = LoadingMode::blocking);
	/// @thread any
	DelayedModelLoadGuard load_delayed_async(std::string_view identifier, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedModelLoadGuard load_delayed_async(std::string_view identifier, Material_ptr matOverride, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedModelLoadGuard load_delayed_async(std::string_view identifier, MaterialScanFunc matOverride, VariantID variant = VariantID::Default);
	/// @thread any
	DelayedModelLoadGuard load_delayed_async(std::string_view identifier, MaterialLoaderFunc matOverride, VariantID variant = VariantID::Default);

	/// @thread any
	Model_ptr fallback();

private:
	friend BasicResource;
	void executeLoad(Model_ptr model, LoadingMode mode); // Hook to execute resource loading
	void unload(Model* model); // Hook to unregister from the cache

public:
	/// @thread any, but the callback cannot reenter the object as a mutex is held during the whole iteration
	void foreach_model(libv::function_ref<void(const Model_ptr&)> func);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
