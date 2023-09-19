// Project: libv.re, File: src/libv/re/resource/shader_loader.cpp

#include <cppcoro/generator.hpp>

#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/shader.hpp>
#include <libv/re/core/glsl/glsl_compose.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/basic_resource_loader.hxx>
#include <libv/re/resource/shader.hpp>
#include <libv/re/resource/shader_fmt.hpp>
#include <libv/re/resource/shader_load_event.hpp>
#include <libv/re/resource/shader_loader.hpp>
#include <libv/re/settings.hpp>
#include <libv/utility/hash.hpp>
#include <libv/utility/nexus.hpp>

// For builtin shader:
#include <libv/re/common/attribute.hpp>
#include <libv/re/uniform/builtin.hpp>
#include <libv/re/uniform/std140_string.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

DelayedShaderLoadGuard::~DelayedShaderLoadGuard() {
	if (requestLoadOnDestroy)
		ResourceAccess::requestActivatingLoad(*resource, mode);
}

// -------------------------------------------------------------------------------------------------

struct ShaderKey {
	std::span<const libv::gl::ShaderType> types;
	std::span<const std::string_view> paths;
	VariantID variant;

	[[nodiscard]] std::size_t hash() const noexcept {
		// NOTE: This hash function MUST BE kept in sync with Shader.hash()
		std::size_t seed = 0;
		for (std::size_t i = 0; i < types.size(); ++i)
			seed = libv::hash_combine_add(seed, paths[i], types[i]);
		seed = libv::hash_combine_add(seed, variant);
		return seed;
	}

	[[nodiscard]] friend bool operator==(const Shader& lhs, const ShaderKey& rhs) {
		const auto lhsStages = lhs.stages();
		if (lhsStages.size() != rhs.types.size())
			return false;
		for (std::size_t i = 0; i < lhsStages.size(); ++i) {
			if (lhsStages[i].sourcePath != rhs.paths[i])
				return false;
			if (lhsStages[i].type != rhs.types[i])
				return false;
		}
		if (lhs.variant() != rhs.variant)
			return false;
		return true;
	}
	[[nodiscard]] friend bool operator==(const ShaderKey& lhs, const Shader& rhs) {
		return rhs == lhs;
	}
};

struct TraitsShaderLoader {
	static constexpr std::string_view name_resource = "shader";
	static constexpr std::string_view name_Resource = "Shader";
	static constexpr std::string_view name_Loader = "ShaderLoader";
	using Resource = Shader;
	using ResourceID = ShaderID;
	using ResourceKey = ShaderKey;
	using DelayedLoadGuard = DelayedShaderLoadGuard;
};

struct ImplShaderLoader : BasicResourceLoader<ImplShaderLoader, TraitsShaderLoader> {
	Shader_ptr fallbackGraphics;
	Shader_ptr fallbackCompute;

	libv::Nexus nexus;

public:
	explicit ImplShaderLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl) :
		BasicResourceLoader<ImplShaderLoader, TraitsShaderLoader>(settings.shader),
		nexus(nexus) {
		(void) gl;
	}

	~ImplShaderLoader() {
		fallbackGraphics = nullptr;
		fallbackCompute = nullptr;
		auto lock = std::unique_lock(mutex);
		if (!cache.empty())
			log_re.error("ShaderLoader is destroyed before every cached Shader instance is destroyed. {} Shader alive.", cache.size());
		if (!watchedFiles.empty())
			log_re.error("ShaderLoader is destroyed before every watched fsw token is destroyed. {} File token alive.", watchedFiles.size());
	}

public:
	void initFallbacks() {
		// TODO P4: This fallback shader does not account for 'model' / 'anim' / instanced objects
		//			During loading the fallback could be specified and that way it would account for these
		const auto fallbackGraphicsVS = R"(
		#version 330 core
)" +
		string_std140<BlockModelMatrices>(BlockModelMatrices::name) +
		string_std140<BlockRenderPass>(BlockRenderPass::name) +
R"(
		layout(location = )" + std::to_string(attrPosition.channel) + R"() in vec3 vertexPosition;

		invariant gl_Position; // invariant - Enforce consitent depth value calculations

		void main() {
			gl_Position = matP * matMV * vec4(vertexPosition, 1);
		})";

		static constexpr auto fallbackGraphicsFS = R"(
		#version 330 core

		out vec4 result;

		void main() {
			result = vec4(1.0, 0.0, 0.0, 1.0);
		})";

		static constexpr auto fallbackComputeCS = R"(
		#version 420 core

		layout(local_size_x = 8, local_size_y = 8) in;
		void main() {
		})";

		fallbackGraphics = Shader_ptr::make("fallback:red.vs", "fallback:red.fs");
		fallbackGraphics->vertex(fallbackGraphicsVS);
		fallbackGraphics->fragment(fallbackGraphicsFS);

		fallbackCompute = Shader_ptr::make("fallback:compute.cs");
		fallbackCompute->compute(fallbackComputeCS);
	}

	void initBuiltins() {
	}

public:
	// NOTE: Events could broadcast inline or on any other thread, but for easier UI interactions they are switched to GL thread
	inline void broadcast(ShaderLoadSuccess&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcast(ShaderLoadFailure&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcast(ShaderUnload&& event) const {
		r.queueTaskGL([this, e = std::move(event)]{ nexus.broadcast_global(e); });
	}
	inline void broadcastResourceUnload(Shader_ptr&& resource) const {
		broadcast(ShaderUnload{resource->id(), std::move(resource)});
	}

	[[nodiscard]] Shader_ptr _loadBuiltin(std::span<const libv::gl::ShaderType> types, std::span<const std::string_view> paths, VariantID variant) {
		(void) types;
		(void) paths;
		(void) variant;
		// Builtin shaders will be handled with virtual files
		return nullptr;
	}

	cppcoro::generator<ThreadAffinity> loadingRoutine(Shader_ptr shader, LoadingMode mode) {
		// NOTE: Owning ptr keeps alive the resource during the whole loading process
		log_re.trace("{} {} shader: {}",
				shader->loadVersion() == -1 ? "Loading" : "Reloading", to_string(mode), *shader);

		std::vector<WatchedFile>* watchTokens = nullptr;
		std::vector<WatchedFile> oldTokens;
		{
			auto lock = std::unique_lock(mutex);
			if (trackFiles) {
				// NOTE: Taking the address relies on that std::unordered_map is stable
				watchTokens = &watchedFiles[shader->id()];
				// Moving out old fsw tokens and keeping them alive until the new watches are created to avoid subscription changes in fsw
				std::swap(*watchTokens, oldTokens);
			}
		}

		co_yield ThreadAffinity::FS; // -------------------------------------------------------------------

		const auto localDefines = shader->defines();

	 	assert(shader->stages().size() <= 5);
	 	GLSLSourceCode shaderSourceCodes[5];
	 	const auto stages = shader->stages();

		std::span<const std::string> defineLists[2]{
				r.settings.shader.globalDefines,
				localDefines
		};
		std::span<std::span<const std::string>> includeLists{};
		// 		r.settings.shader.globalIncludes,
		// 		localIncludes

		for (std::size_t i = 0; i < stages.size(); ++i) {
			const auto& mainIncludePath = stages[i].sourcePath;

			const auto includeHook = [&](const std::string_view includePath, const std::vector<std::pair<std::string, int>>& includeStack) {
				auto fileData = _fs_loadFile(
						watchTokens,
						r.settings.shader.resourceMappings,
						includePath,
						shader->id());

				if (!fileData)
					throw ShaderLoadFailure::IncludeFailure{
							std::string{includePath},
							includeStack,
							std::move(fileData.error().unmatchedMappings),
							std::move(fileData.error().mappingErrors),
							std::move(fileData.error().physicalErrors)
					};

				return *fileData;
			};

			try {
				shaderSourceCodes[i] = glsl_compose_from_include(includeHook, mainIncludePath, defineLists, includeLists);
			} catch (ShaderLoadFailure::IncludeFailure& e) {
				std::string message;
				message += fmt::format("Failed to include: '{}'", e.includePath);
				for (const auto& [file, line] : e.includeStack)
					message += fmt::format("\n    Included from: {}:{}", file, line);
				for (const auto& [resourcePrefix, virtualPrefix] : e.unmatchedMappings)
					message += fmt::format("\nMapping '{}' -> '{}' - Unmatched prefix", resourcePrefix, virtualPrefix);
				for (const auto& [virtualFilepath, ec] : e.mappingErrors)
					message += fmt::format("\nVirtual '{}' - {}", virtualFilepath, libv::res::to_message(ec));
				for (const auto& [physicalFilepath, ec] : e.physicalErrors)
					message += fmt::format("\nPhysical '{}' - {} ({})", physicalFilepath, ec.message(), ec);

				log_re.error("Failed to load shader: {} v{} ({})\n{}",
						*shader, shader->loadVersion(), shader->id(), message);

				ResourceAccess::recordLoadFailure(*shader);
				broadcast(ShaderLoadFailure{
					shader->id(),
					shader,
					std::move(e),
					std::nullopt,
					std::nullopt
				});
				co_return;
			}
		}

		oldTokens.clear(); // Discard old fsw tokens, as the new tokens are already in place

		co_yield ThreadAffinity::GL; // -------------------------------------------------------------------

		libv::GL& gl = r.gl;

		libv::gl::Program program;
		gl(program).create();

		// Compile shader stages
		bool compileSuccess = true;
		std::string compileErrorMessage;
		for (std::size_t i = 0; i < stages.size() && compileSuccess; ++i) {
			libv::gl::Shader shaderStage;
			gl(shaderStage).create(stages[i].type);
			gl(shaderStage).compile(shaderSourceCodes[i].code, false);
			compileSuccess &= gl(shaderStage).status();
			if (compileSuccess) {
				gl(program).attach(shaderStage);
			} else {
				compileErrorMessage = shaderSourceCodes[i].fixupErrorMessage(gl(shaderStage).info());
			}
			gl(shaderStage).destroy();
		}

		if (!compileSuccess) {
			log_re.error("Failed to compile shader: {} v{} ({})\n{}",
					*shader, shader->loadVersion(), shader->id(), compileErrorMessage);

			ResourceAccess::recordLoadFailure(*shader);
			broadcast(ShaderLoadFailure{
					shader->id(),
					shader,
					std::nullopt,
					ShaderLoadFailure::CompileFailure{std::move(compileErrorMessage)},
					std::nullopt});
			co_return;
		}

		// Link shader program
		gl(program).link(false);

		if (!gl(program).status()) {
			auto linkErrorMessage = gl(program).info();
			log_re.error("Failed to link shader: {} v{} ({})\n{}",
					*shader, shader->loadVersion(), shader->id(), linkErrorMessage);

			ResourceAccess::recordLoadFailure(*shader);
			broadcast(ShaderLoadFailure{
					shader->id(),
					shader,
					std::nullopt,
					std::nullopt,
					ShaderLoadFailure::LinkFailure{std::move(linkErrorMessage)}});
			co_return;
		}

		shader->directAssign(program);

		ResourceAccess::recordLoadSuccess(*shader);
		broadcast(ShaderLoadSuccess{shader->id(), shader});

		co_return;
	}
};

// -------------------------------------------------------------------------------------------------

ShaderLoader::ShaderLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl) :
	self(std::make_unique<ImplShaderLoader>(settings, nexus, gl)) { }

void ShaderLoader::init() {
	self->initFallbacks();
	self->initBuiltins();
}

void ShaderLoader::reset() {
	self.reset();
}

ShaderLoader::~ShaderLoader() {
	// For the sake of forward declared types
}

Shader_ptr ShaderLoader::load(std::string_view path_cs, VariantID variant, LoadingMode mode) {
	const auto types = std::array{libv::gl::ShaderType::Compute};
	const auto paths = std::array{path_cs};
	return self->load(mode, self->fallbackCompute, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load(std::string_view path_vs, std::string_view path_fs, VariantID variant, LoadingMode mode) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_fs};
	return self->load(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load(std::string_view path_vs, std::string_view path_gs, std::string_view path_fs,
		VariantID variant, LoadingMode mode) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Geometry, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_gs, path_fs};
	return self->load(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load(libv::gl::ShaderType type0, std::string_view path0, VariantID variant, LoadingMode mode) {
	const auto types = std::array{type0};
	const auto paths = std::array{path0};
	auto fallback = type0 == libv::gl::ShaderType::Compute ? self->fallbackCompute : self->fallbackGraphics;
	return self->load(mode, std::move(fallback), std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
		std::string_view path1, VariantID variant, LoadingMode mode) {
	const auto types = std::array{type0, type1};
	const auto paths = std::array{path0, path1};
	return self->load(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
		std::string_view path1, libv::gl::ShaderType type2, std::string_view path2, VariantID variant,
		LoadingMode mode) {
	const auto types = std::array{type0, type1, type2};
	const auto paths = std::array{path0, path1, path2};
	return self->load(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}

Shader_ptr ShaderLoader::load_async(std::string_view path_cs, VariantID variant) {
	const auto types = std::array{libv::gl::ShaderType::Compute};
	const auto paths = std::array{path_cs};
	return self->load(LoadingMode::async, self->fallbackCompute, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load_async(std::string_view path_vs, std::string_view path_fs, VariantID variant) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_fs};
	return self->load(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load_async(
		std::string_view path_vs, std::string_view path_gs, std::string_view path_fs, VariantID variant) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Geometry, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_gs, path_fs};
	return self->load(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load_async(libv::gl::ShaderType type0, std::string_view path0, VariantID variant) {
	const auto types = std::array{type0};
	const auto paths = std::array{path0};
	auto fallback = type0 == libv::gl::ShaderType::Compute ? self->fallbackCompute : self->fallbackGraphics;
	return self->load(LoadingMode::async, std::move(fallback), std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load_async(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
		std::string_view path1, VariantID variant) {
	const auto types = std::array{type0, type1};
	const auto paths = std::array{path0, path1};
	return self->load(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
Shader_ptr ShaderLoader::load_async(libv::gl::ShaderType type0, std::string_view path0, libv::gl::ShaderType type1,
		std::string_view path1, libv::gl::ShaderType type2, std::string_view path2, VariantID variant) {
	const auto types = std::array{type0, type1, type2};
	const auto paths = std::array{path0, path1, path2};
	return self->load(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}

DelayedShaderLoadGuard ShaderLoader::load_delayed(std::string_view path_cs, VariantID variant, LoadingMode mode) {
	const auto types = std::array{libv::gl::ShaderType::Compute};
	const auto paths = std::array{path_cs};
	return self->load_delayed(mode, self->fallbackCompute, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed(
		std::string_view path_vs, std::string_view path_fs, VariantID variant, LoadingMode mode) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_fs};
	return self->load_delayed(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed(std::string_view path_vs, std::string_view path_gs,
		std::string_view path_fs, VariantID variant, LoadingMode mode) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Geometry, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_gs, path_fs};
	return self->load_delayed(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed(
		libv::gl::ShaderType type0, std::string_view path0, VariantID variant, LoadingMode mode) {
	const auto types = std::array{type0};
	const auto paths = std::array{path0};
	auto fallback = type0 == libv::gl::ShaderType::Compute ? self->fallbackCompute : self->fallbackGraphics;
	return self->load_delayed(mode, std::move(fallback), std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed(libv::gl::ShaderType type0, std::string_view path0,
		libv::gl::ShaderType type1, std::string_view path1, VariantID variant, LoadingMode mode) {
	const auto types = std::array{type0, type1};
	const auto paths = std::array{path0, path1};
	return self->load_delayed(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed(libv::gl::ShaderType type0, std::string_view path0,
		libv::gl::ShaderType type1, std::string_view path1, libv::gl::ShaderType type2, std::string_view path2,
		VariantID variant, LoadingMode mode) {
	const auto types = std::array{type0, type1, type2};
	const auto paths = std::array{path0, path1, path2};
	return self->load_delayed(mode, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}

DelayedShaderLoadGuard ShaderLoader::load_delayed_async(std::string_view path_cs, VariantID variant) {
	const auto types = std::array{libv::gl::ShaderType::Compute};
	const auto paths = std::array{path_cs};
	return self->load_delayed(LoadingMode::async, self->fallbackCompute, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed_async(
		std::string_view path_vs, std::string_view path_fs, VariantID variant) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_fs};
	return self->load_delayed(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed_async(
		std::string_view path_vs, std::string_view path_gs, std::string_view path_fs, VariantID variant) {
	const auto types = std::array{libv::gl::ShaderType::Vertex, libv::gl::ShaderType::Geometry, libv::gl::ShaderType::Fragment};
	const auto paths = std::array{path_vs, path_gs, path_fs};
	return self->load_delayed(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed_async(
		libv::gl::ShaderType type0, std::string_view path0, VariantID variant) {
	const auto types = std::array{type0};
	const auto paths = std::array{path0};
	auto fallback = type0 == libv::gl::ShaderType::Compute ? self->fallbackCompute : self->fallbackGraphics;
	return self->load_delayed(LoadingMode::async, std::move(fallback), std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed_async(libv::gl::ShaderType type0, std::string_view path0,
		libv::gl::ShaderType type1, std::string_view path1, VariantID variant) {
	const auto types = std::array{type0, type1};
	const auto paths = std::array{path0, path1};
	return self->load_delayed(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}
DelayedShaderLoadGuard ShaderLoader::load_delayed_async(libv::gl::ShaderType type0, std::string_view path0,
		libv::gl::ShaderType type1, std::string_view path1, libv::gl::ShaderType type2, std::string_view path2,
		VariantID variant) {
	const auto types = std::array{type0, type1, type2};
	const auto paths = std::array{path0, path1, path2};
	return self->load_delayed(LoadingMode::async, self->fallbackGraphics, std::span(types), std::span(paths), variant);
}

// -------------------------------------------------------------------------------------------------

void ShaderLoader::executeLoad(Shader_ptr resource, LoadingMode mode) {
	self->executeLoad(std::move(resource), mode);
}

void ShaderLoader::unload(Shader* resource) {
	self->requestUnload(resource);
}

void ShaderLoader::foreach_shader(libv::function_ref<void(const Shader_ptr&)> func) {
	const auto lock = std::unique_lock(self->mutex);
	for (const auto& shader : self->cache)
		func(shader);
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
