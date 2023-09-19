// Project: libv.re, File: src/libv/re/resource/shader.hpp

#pragma once

#include <libv/gl/enum_fwd.hpp>
#include <libv/gl/fwd.hpp>
#include <libv/re/fwd.hpp>
#include <libv/container/small_vector.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/resource/common_resource.hpp>

#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using ShaderType = libv::gl::ShaderType; // Convenience alias

// -------------------------------------------------------------------------------------------------

struct ColdShaderData;

struct ShaderStage {
	libv::gl::ShaderType type;
	std::string sourcePath;
};

/// To create a new Shader instance use the ShaderLoader.load() functions
class Shader : public libv::ref_count_base {
public:
	friend libv::ref_count_access;
	friend ResourceAccess;
	friend BasicResource;
	using ptr = Shader_ptr;
	using cptr = Shader_cptr;

private:
	std::atomic_bool dirty = false;
	// OpenGL handler - GL Thread access only
	GLSafeProgram program;
	libv::small_vector<int32_t, 8> uniformLocations;
	// Cold data
	std::unique_ptr<ColdShaderData> cold;

public:
	/// @thread any
	explicit Shader(std::string path_cs, VariantID variant = VariantID::Default);
	/// @thread any
	Shader(std::string path_vs, std::string path_fs, VariantID variant = VariantID::Default);
	/// @thread any
	Shader(std::string path_vs, std::string path_gs, std::string path_fs,
			VariantID variant = VariantID::Default);
	/// @thread any
	Shader(libv::gl::ShaderType type0, std::string path0, VariantID variant = VariantID::Default);
	/// @thread any
	Shader(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1,
			VariantID variant = VariantID::Default);
	/// @thread any
	Shader(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2,
			std::string path2, VariantID variant = VariantID::Default);
	/// @thread any
	Shader(std::span<const libv::gl::ShaderType> types, std::span<const std::string_view> paths,
			VariantID variant = VariantID::Default);
	/// @thread any
	Shader(std::span<const libv::gl::ShaderType> types, std::span<const std::string_view> paths,
			VariantID variant, CachedTag, bool delayed, const Shader_ptr& fallback);
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) & = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) & = delete;
private:
	void ref_count_one();
public:
	/// @thread any
	~Shader();

public:
	/// Not an OpenGL ID, it is a stable C++ Unique ID that can be used as key
	[[nodiscard]] ShaderID id() const noexcept {
		return std::bit_cast<ShaderID>(this);
	}
	[[nodiscard]] const std::string& name() const noexcept;
	/// @thread any
	[[nodiscard]] VariantID variant() const noexcept;
	/// @thread any
	[[nodiscard]] int currentVersion() const noexcept;
	/// @thread any
	[[nodiscard]] int loadVersion() const noexcept;

	/// For thread safety reasons the copy is necessary
	/// @thread any
	[[nodiscard]] std::vector<std::string> defines() const noexcept;
	/// @thread any
	[[nodiscard]] std::span<const ShaderStage> stages() const noexcept;

	/// @thread any
	[[nodiscard]] std::size_t hash() const noexcept;
	/// @thread any
	[[nodiscard]] bool operator==(const Shader& rhs) const noexcept;

private:
	void _markDirty();

public:
	/// @usage:
	/// shader.define("ENABLE_BLOOM 1");
	/// shader.define("TONEMAP_FN tonemapAcesFilmic");
	/// shader.define("COMBINE(LEFT, RIGHT) ((LEFT) + (RIGHT))");
	/// @thread any
	void define(std::string expression);
	void undefine(std::string_view identifier);

	void directAssign(libv::gl::Program program) noexcept;

	void vertex(std::string source) noexcept;
	void geometry(std::string source) noexcept;
	void fragment(std::string source) noexcept;
	void compute(std::string source) noexcept;
	void tessControl(std::string source) noexcept;
	void tessEvaluation(std::string source) noexcept;

	uint16_t registerUniform(std::string_view name);
	void registerBlockBinding(std::string_view blockName, BlockBinding blockBinding);

private:
	void requestLoad(LoadingMode mode) noexcept;
	void requestActivatingLoad(LoadingMode mode) noexcept;
	void recordLoadSuccess() noexcept;
	void recordLoadFailure() noexcept;
	void recordUnloadFromCache() noexcept;

	void gl_updateAndBind(libv::GL& gl);

public:
	/// @thread GL
	void gl_bind(libv::GL& gl);
	/// Must call gl_bind first
	/// @thread GL
	[[nodiscard]] inline int32_t gl_uniformLocation(uint16_t indirectIndex) const {
		assert(indirectIndex < uniformLocations.size());
		return uniformLocations[indirectIndex];
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
