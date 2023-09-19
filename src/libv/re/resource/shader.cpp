// Project: libv.re, File: src/libv/re/resource/shader.cpp

#include <libv/algo/erase_stable.hpp>
#include <libv/algo/linear_find.hpp>
#include <libv/algo/slice.hpp>
#include <libv/container/dyn_array.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/shader.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/core/glsl/glsl_source_code.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/basic_resource.hxx>
#include <libv/re/resource/shader.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/utility/hash.hpp>

#include <mutex>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(Shader* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(Shader* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

struct ColdShaderData {
	struct BlockInfo {
		BlockBinding location;
		std::string name;
	};

	std::mutex mutex;
	Shader_ptr fallback = nullptr;
	bool dirtySource = false;
	bool dirtyLocations = false;
	bool dirtyBlockBindings = false;
	bool dirtyDirectAssign = false;

	bool cached = false;

	// Informative name
	const std::string name;

	// Immutable key data
	const libv::dyn_array<ShaderStage> stages;
	const VariantID variant;

	// Loading state
	LoadingState loadingState = LoadingState::standby;
	int currentVersion = -1; /// Value -1 means loading never succeeded, using fallback shader instead
	int loadVersion = -1;

	// Mutable data
	std::vector<std::string> defines;
	std::vector<std::string> uniformNames;
	std::vector<BlockInfo> uniformBlockBindings;
	std::vector<GLSLSourceCode> sourceCodes;

	std::optional<GLSafeProgram> directAssign;
	std::string vertex;
	std::string geometry;
	std::string fragment;
	std::string compute;
	std::string tessControl;
	std::string tessEvaluation;

public:
	ColdShaderData(bool cached, LoadingState initState, VariantID variant, libv::dyn_array<ShaderStage>&& stages, const Shader_ptr& fallback) :
		fallback(fallback),
		cached(cached),
		name(generateShaderName(stages, variant)),
		stages(std::move(stages)),
		variant(variant),
		loadingState(initState) {

		// Register built-in block bindings (This could be conditional with shader creation flags)
		dirtyBlockBindings = true;
		uniformBlockBindings.emplace_back(BlockBinding::pass, std::string(builtinBlockNamePass));
		uniformBlockBindings.emplace_back(BlockBinding::lights, std::string(builtinBlockNameLights));
		uniformBlockBindings.emplace_back(BlockBinding::model, std::string(builtinBlockNameModel));
	}

	static std::string generateShaderName(const libv::dyn_array<ShaderStage>& stages, VariantID variant);
};

// -------------------------------------------------------------------------------------------------

std::string ColdShaderData::generateShaderName(const libv::dyn_array<ShaderStage>& stages, VariantID variant) {
	// Chooses the shortest name:
	// <empty>
	// shader
	// shader|G3
	// shader.vs|different.fs
	// shader.vs|different.fs|G3
	// vs:shader.glsl|fs:different.glsl
	// vs:shader.glsl|fs:different.glsl|G3

	const auto viewBasename = [](std::string_view path) {
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

	bool matchingBasenames = true;
	const auto firstBasename = viewBasename(stages.front().sourcePath);
	for (std::size_t i = 1; i < stages.size(); ++i)
		matchingBasenames &= firstBasename == viewBasename(stages[i].sourcePath);

	if (matchingBasenames) {
		if (variant != VariantID::Default)
			return std::string(firstBasename) + "|G" + std::to_string(+variant);
		else
			return std::string(firstBasename);
	}

	std::ostringstream oss;
	bool first = true;
	for (const auto& stage : stages) {
		const auto need_type_spec = !stage.sourcePath.ends_with(libv::gl::to_string_ext(stage.type));

		if (!first)
			oss << '|';
		else
			first = false;

		if (need_type_spec)
			oss << libv::gl::to_string_short(stage.type) << ':';

		oss << stage.sourcePath;
	}
	if (variant != VariantID::Default)
		oss << "%" << +variant;

	return std::move(oss).str();
}

namespace {

libv::dyn_array<ShaderStage> makeStages(libv::gl::ShaderType type0, std::string path0) {
	libv::dyn_array<ShaderStage> result(1);
	result[0].type = type0;
	result[0].sourcePath = std::move(path0);
	return result;
}

libv::dyn_array<ShaderStage> makeStages(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1) {
	libv::dyn_array<ShaderStage> result(2);
	result[0].type = type0;
	result[0].sourcePath = std::move(path0);
	result[1].type = type1;
	result[1].sourcePath = std::move(path1);
	return result;
}

libv::dyn_array<ShaderStage> makeStages(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2) {
	libv::dyn_array<ShaderStage> result(3);
	result[0].type = type0;
	result[0].sourcePath = std::move(path0);
	result[1].type = type1;
	result[1].sourcePath = std::move(path1);
	result[2].type = type2;
	result[2].sourcePath = std::move(path2);
	return result;
}

libv::dyn_array<ShaderStage> makeStages(std::span<const libv::gl::ShaderType> types, std::span<const std::string_view> paths) {
	assert(types.size() == paths.size());
	libv::dyn_array<ShaderStage> result(types.size());
	for (std::size_t i = 0; i < types.size(); ++i) {
		result[i].type = types[i];
		result[i].sourcePath = std::string(paths[i]);
	}
	return result;
}

} // namespace

// -------------------------------------------------------------------------------------------------

Shader::Shader(std::string path_cs, VariantID variant) :
	cold(std::make_unique<ColdShaderData>(false, LoadingState::standby, variant, makeStages(
			libv::gl::ShaderType::Compute, std::move(path_cs)), nullptr)) { }

Shader::Shader(std::string path_vs, std::string path_fs, VariantID variant) :
	cold(std::make_unique<ColdShaderData>(false, LoadingState::standby, variant, makeStages(
			libv::gl::ShaderType::Vertex, std::move(path_vs),
			libv::gl::ShaderType::Fragment, std::move(path_fs)), nullptr)) { }

Shader::Shader(std::string path_vs, std::string path_gs, std::string path_fs, VariantID variant) :
	cold(std::make_unique<ColdShaderData>(false, LoadingState::standby, variant, makeStages(
			libv::gl::ShaderType::Vertex, std::move(path_vs),
			libv::gl::ShaderType::Geometry, std::move(path_gs),
			libv::gl::ShaderType::Fragment, std::move(path_fs)), nullptr)) { }

Shader::Shader(libv::gl::ShaderType type0, std::string path0, VariantID variant) :
	cold(std::make_unique<ColdShaderData>(false, LoadingState::standby, variant, makeStages(
			type0, path0), nullptr)) { }

Shader::Shader(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, VariantID variant) :
	cold(std::make_unique<ColdShaderData>(false, LoadingState::standby, variant, makeStages(
			type0, path0,
			type1, path1), nullptr)) { }

Shader::Shader(libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2,
		std::string path2, VariantID variant) :
	cold(std::make_unique<ColdShaderData>(false, LoadingState::standby, variant, makeStages(
			type0, path0,
			type1, path1,
			type2, path2), nullptr)) { }

Shader::Shader(std::span<const libv::gl::ShaderType> types, std::span<const std::string_view> paths, VariantID variant) :
	cold(std::make_unique<ColdShaderData>(false, LoadingState::standby, variant, makeStages(
			types, paths), nullptr)) { }

Shader::Shader(std::span<const libv::gl::ShaderType> types, std::span<const std::string_view> paths, VariantID variant, CachedTag, bool delayed, const Shader_ptr& fallback) :
	dirty(fallback != nullptr),
	cold(std::make_unique<ColdShaderData>(
			true,
			delayed ? LoadingState::delayed : LoadingState::standby,
			variant,
			makeStages(types, paths),
			fallback)) {
}

void Shader::ref_count_one() {
	BasicResource::impl_ref_count_one(this, r.shader);
}

Shader::~Shader() {
	// For the sake of forward declared types
}

// -------------------------------------------------------------------------------------------------

const std::string& Shader::name() const noexcept {
	return cold->name;
}
VariantID Shader::variant() const noexcept {
	return cold->variant;
}
int Shader::currentVersion() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->currentVersion;
}
int Shader::loadVersion() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->loadVersion;
}

std::vector<std::string> Shader::defines() const noexcept {
	auto lock = std::unique_lock(cold->mutex);
	return cold->defines;
}

std::span<const ShaderStage> Shader::stages() const noexcept {
	return {cold->stages};
}

// -------------------------------------------------------------------------------------------------

std::size_t Shader::hash() const noexcept {
	// NOTE: This hash function MUST BE kept in sync with ShaderKey.hash()
	std::size_t seed = 0;
	for (const auto& stage : cold->stages)
		seed = libv::hash_combine_add(seed, stage.sourcePath, stage.type);
	seed = libv::hash_combine_add(seed, cold->variant);
	return seed;
}

bool Shader::operator==(const Shader& rhs) const noexcept {
	const auto& lhs = *this;
	if (lhs.cold->stages.size() != rhs.cold->stages.size())
		return false;
	for (std::size_t i = 0; i < lhs.cold->stages.size(); ++i) {
		if (lhs.cold->stages[i].sourcePath != rhs.cold->stages[i].sourcePath)
			return false;
		if (lhs.cold->stages[i].type != rhs.cold->stages[i].type)
			return false;
	}
	if (lhs.variant() != rhs.variant())
		return false;
	return true;
}

// -------------------------------------------------------------------------------------------------

void Shader::_markDirty() {
	dirty.store(true, std::memory_order_release);
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] constexpr inline std::string_view extractMacroIdentifierFromExpression(std::string_view expression) {
	std::size_t i = 0;
	while (i < expression.size() &&
			(
					('a' <= expression[i] && expression[i] <= 'z') ||
					('A' <= expression[i] && expression[i] <= 'Z') ||
					('0' <= expression[i] && expression[i] <= '9') ||
					expression[i] == '_'
			)
	)
		++i;

	return expression.substr(0, i);
}

void Shader::define(std::string expression) {
	{
		auto lock = std::unique_lock(cold->mutex);
		// NOTE: No _markDirty() call as source reload will invalidate this resource

		bool existing = false;
		const auto identifier = extractMacroIdentifierFromExpression(expression);
		for (auto& storedDefine : cold->defines) {
			if (extractMacroIdentifierFromExpression(storedDefine) == identifier) {
				if (storedDefine == expression)
					return; // No change, no need to reload shader
				storedDefine = std::move(expression);
				existing = true;
				break;
			}
		}
		if (!existing)
			cold->defines.emplace_back(std::move(expression));
	}
	requestLoad(LoadingMode::blocking);
}

void Shader::undefine(std::string_view identifier) {
	{
		auto lock = std::unique_lock(cold->mutex);
		// NOTE: No _markDirty() call as source reload will invalidate this resource

		const auto oldCount = cold->defines.size();
		libv::erase_stable(cold->defines, identifier, extractMacroIdentifierFromExpression);
		if (oldCount == cold->defines.size())
			return; // No change, no need to reload shader
	}
	requestLoad(LoadingMode::blocking);
}

void Shader::directAssign(libv::gl::Program program) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();
	cold->dirtyDirectAssign = true;
	if (cold->directAssign)
		cold->directAssign.reset();
	cold->directAssign.emplace();
	cold->directAssign->id = program.id;
}

// -------------------------------------------------------------------------------------------------

void Shader::vertex(std::string source) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();
	cold->dirtySource = true;
	cold->vertex = std::move(source);
}

void Shader::geometry(std::string source) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();
	cold->dirtySource = true;
	cold->geometry = std::move(source);
}

void Shader::fragment(std::string source) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();
	cold->dirtySource = true;
	cold->fragment = std::move(source);
}

void Shader::compute(std::string source) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();
	cold->dirtySource = true;
	cold->compute = std::move(source);
}

void Shader::tessControl(std::string source) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();
	cold->dirtySource = true;
	cold->tessControl = std::move(source);
}

void Shader::tessEvaluation(std::string source) noexcept {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();
	cold->dirtySource = true;
	cold->tessEvaluation = std::move(source);
}

// -------------------------------------------------------------------------------------------------

uint16_t Shader::registerUniform(std::string_view name) {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();

	const auto it = libv::linear_find_index(cold->uniformNames, name);
	if (it != -1)
		return static_cast<uint16_t>(it);

	cold->dirtyLocations = true;
	const auto lastIndex = static_cast<uint16_t>(cold->uniformNames.size());
	cold->uniformNames.emplace_back(name);
	return lastIndex;
}

void Shader::registerBlockBinding(std::string_view blockName, BlockBinding blockBinding) {
	auto lock = std::unique_lock(cold->mutex);
	_markDirty();

	const auto it = libv::linear_find_index(cold->uniformBlockBindings, blockName, &ColdShaderData::BlockInfo::name);
	if (it != -1) {
		log_re.error_if(
				cold->uniformBlockBindings[it].location != blockBinding,
				"Inconsistent block binding in shader '{}' for block '{}' with original location {}. Binding request to new location {} is ignored.",
				name(), blockName, to_underlying(cold->uniformBlockBindings[it].location), to_underlying(blockBinding));
		return;
	}

	cold->dirtyBlockBindings = true;
	cold->uniformBlockBindings.emplace_back(blockBinding, std::string(blockName));
}

// -------------------------------------------------------------------------------------------------

void Shader::requestLoad(LoadingMode mode) noexcept {
	BasicResource::implRequestLoad(this, r.shader, mode);
}
void Shader::requestActivatingLoad(LoadingMode mode) noexcept {
	BasicResource::implRequestActivatingLoad(this, r.shader, mode);
}
void Shader::recordLoadSuccess() noexcept {
	BasicResource::implRecordLoadSuccess(this, r.shader);
}
void Shader::recordLoadFailure() noexcept {
	BasicResource::implRecordLoadFailure(this, r.shader);
}
void Shader::recordUnloadFromCache() noexcept {
	BasicResource::implRecordUnloadFromCache(this);
}

// -------------------------------------------------------------------------------------------------

void Shader::gl_updateAndBind(libv::GL& gl) {
	auto lock = std::unique_lock(cold->mutex);

	if (cold->fallback) {
		// The fallback state is active, redirect to it.
		cold->fallback->gl_bind(gl);

		// In fallback mode new uniformLocations has to be filled with -1s as that code is currently blocked by the
		// fallback state
		const auto numNames = cold->uniformNames.size();
		const auto numLocations = uniformLocations.size();
		if (numNames != numLocations) {
			uniformLocations.resize(numNames);
			for (std::size_t i = numLocations; i < numNames; ++i)
				uniformLocations[i] = -1;
		}

		// To keep using the fallback shader the dirty has to be kept set.
		// Keep every dirty flag as-is and will execute everything once the fallback state is cleared
		return;
	}

	dirty.store(false, std::memory_order_release);

	if (cold->dirtyDirectAssign) {
		cold->dirtyDirectAssign = false;
		LIBV_RE_ASSERT(!cold->dirtySource);

		if (program.id != 0)
			gl(program).destroy();
		program.id = cold->directAssign->id;
		cold->directAssign->id = 0;
		cold->directAssign.reset();

		cold->dirtyLocations = true;
		cold->dirtyBlockBindings = true;
	}

	if (cold->dirtySource) {
		cold->dirtySource = false;

		if (program.id != 0)
			gl(program).destroy();
		gl(program).create();

		const auto attach = [&](libv::gl::ShaderType type, const std::string& source) {
			if (!source.empty()) {
				libv::gl::Shader shader;
				gl(shader).create(type);
				gl(shader).compile(source);
				// In this mode, just ignore errors, libv.gl will produce the appropriate log message
				gl(program).attach(shader);
				gl(shader).destroy();
			}
		};

		attach(libv::gl::ShaderType::Vertex, cold->vertex);
		attach(libv::gl::ShaderType::Geometry, cold->geometry);
		attach(libv::gl::ShaderType::Fragment, cold->fragment);
		attach(libv::gl::ShaderType::Compute, cold->compute);
		attach(libv::gl::ShaderType::TessControl, cold->tessControl);
		attach(libv::gl::ShaderType::TessEvaluation, cold->tessEvaluation);
		gl(program).link();

		// In this mode, just ignore errors, libv.gl will produce the appropriate log message
		if (gl(program).status()) {
			cold->dirtyLocations = true;
			cold->dirtyBlockBindings = true;
		}
	}

	if (cold->dirtyLocations) {
		cold->dirtyLocations = false;
		const auto size = cold->uniformNames.size();
		uniformLocations.resize(size);
		for (std::size_t i = 0; i < size; ++i)
			uniformLocations[i] = gl(program).uniformLocation(cold->uniformNames[i].c_str());
	}

	if (cold->dirtyBlockBindings) {
		cold->dirtyBlockBindings = false;
		for (const auto& block : cold->uniformBlockBindings)
			gl(program).uniformBlockBinding(block.name.c_str(), libv::to_underlying(block.location));
	}

	gl(program).use();
}

void Shader::gl_bind(libv::GL& gl) {
	// Thread safety:
	// This solution is similar to a 'double check locking' pattern.
	// A single atomic load decides if we are dirty or not at the point of binding.
	// If the dirty signal is missed it must have been coming from a concurrent event/thread therefore
	// ordering is not guaranteed anyway. It will be picked up during the next binding or in the next frame.
	// The dirty flag can also indicate if the fallback state is active.
	if (dirty.load(std::memory_order_acquire))
		gl_updateAndBind(gl);
	else
		gl(program).use();
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
