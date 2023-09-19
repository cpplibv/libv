// Project: libv.re, File: src/libv/re/resource/model.hpp

#pragma once

#include <libv/container/small_vector.hpp>
#include <libv/gl/indirect_command.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/mesh/mesh_model_static.hpp>
#include <libv/re/resource/common_resource.hpp>
#include <libv/re/resource/material_func.hpp> // Hard, but could be fwd-able
#include <libv/utility/function_ref.hpp>
#include <libv/vm4/fwd.hpp>

#include <span>
#include <string>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

struct ColdModelData;

class Model : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	friend ResourceAccess;
	friend BasicResource;
	using ptr = Model_ptr;
	using cptr = Model_cptr;

private:
	// OpenGL/Resource handlers - GL Thread access only
	struct ModelStaticSubObject {
		Material_ptr material = nullptr;
		uint32_t indirectCommandIndex = 0;
		uint32_t indirectCommandCount = 0;
	};
	MeshModelStatic_ptr mesh_; /// Can be null if the fallback should be used
	libv::small_vector<ModelStaticSubObject, 1> subObjects;
	libv::small_vector<libv::gl::IndirectCommandDrawElements, 1> subObjectCommands;
	uint32_t indirectCommandBufferOffset = 0;

	// Cold data
	const std::unique_ptr<ColdModelData> cold;

public:
	/// @thread any
	explicit Model(std::string identifier, VariantID variant = VariantID::Default);
	/// @thread any
	Model(std::string_view identifier, VariantID variant, MaterialLoaderFunc&& materialLoaderFunc, CachedTag, bool delayed, const Model_ptr& fallback);
	Model(const Model&) = delete;
	Model& operator=(const Model&) & = delete;
	Model(Model&&) = delete;
	Model& operator=(Model&&) & = delete;
private:
	void ref_count_one();
public:
	/// @thread any
	~Model();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(std::string identifier = "noname") {
		return ptr::make(std::move(identifier));
	}

public:
	/// Not an OpenGL ID, it is a stable C++ Unique ID that can be used as key
	/// @thread any
	[[nodiscard]] ModelID id() const noexcept {
		return std::bit_cast<ModelID>(this);
	}
	/// @thread any
	[[nodiscard]] const std::string& identifier() const noexcept;
	/// @thread any
	[[nodiscard]] VariantID variant() const noexcept;
	/// @thread any
	[[nodiscard]] int currentVersion() const noexcept;
	/// @thread any
	[[nodiscard]] int loadVersion() const noexcept;

	/// @thread any
	[[nodiscard]] std::size_t hash() const noexcept;
	/// @thread any
	[[nodiscard]] bool operator==(const Model& rhs) const noexcept;

	/// @thread any
	[[nodiscard]] MaterialLoaderFunc& materialLoaderFunc() const noexcept;

// 	[[nodiscard]] bool isLoaded() const noexcept;

private:
	void requestLoad(LoadingMode mode) noexcept;
	void requestActivatingLoad(LoadingMode mode) noexcept;
	void recordLoadSuccess() noexcept;
	void recordLoadFailure() noexcept;
	void recordUnloadFromCache() noexcept;

public:
	/// @thread GL
	void gl_assignModel(const libv::vm4::Model& model, std::span<Material_ptr> materials) noexcept;
	/// @thread GL
	void gl_assignMeshMaterial(MeshModelStatic_ptr&& mesh, Material_ptr&& material) noexcept;
// 	/// Requires loaded state
// 	[[nodiscard]] libv::vec3f AABB_max() const noexcept;
// 	/// Requires loaded state
// 	[[nodiscard]] libv::vec3f AABB_min() const noexcept;

public:
	/// @thread GL
	[[nodiscard]] BoundingSphere boundingSphere() const;
	/// @thread GL
	void foreachSubObject(const function_ref<void(Mesh*, Material*, SubObjectIndex)>& func);

public:
	/// @thread GL
	void gl_update(ContextUpdate& ctx);
	/// @thread GL
	void gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex);
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::re

// =================================================================================================
// =================================================================================================
// =================================================================================================

//enum class RenderMode {
//	color = 0,
//
//	positionW = 1,
//	normalW = 2,
//	tangentW = 3,
//	bitangentW = 4,
//
//	texture0 = 5,
//	depth = 6,
//
//	sample0 = 10,
//	sample1 = 11,
//	sample2 = 12,
//	sample3 = 13,
//	sample4 = 14,
//	sample5 = 15,
//	sample6 = 16,
//	sample7 = 17,
//
//	lambert = 20,
//};
//
//ShaderModel()
//	define("MODE_COLOR", RenderMode::color);
//
//	define("MODE_POSITION_W", RenderMode::positionW);
//	define("MODE_NORMAL_W", RenderMode::normalW);
//	define("MODE_TANGENT_W", RenderMode::tangentW);
//	define("MODE_BITANGENT_W", RenderMode::bitangentW);
//
//	define("MODE_TEXTURE0", RenderMode::texture0);
//	define("MODE_DEPTH", RenderMode::depth);
//
//	define("MODE_SAMPLE0", RenderMode::sample0);
//	define("MODE_SAMPLE1", RenderMode::sample1);
//	define("MODE_SAMPLE2", RenderMode::sample2);
//	define("MODE_SAMPLE3", RenderMode::sample3);
//	define("MODE_SAMPLE4", RenderMode::sample4);
//	define("MODE_SAMPLE5", RenderMode::sample5);
//	define("MODE_SAMPLE6", RenderMode::sample6);
//	define("MODE_SAMPLE7", RenderMode::sample7);
//
//	define("MODE_LAMBERT", RenderMode::lambert);
