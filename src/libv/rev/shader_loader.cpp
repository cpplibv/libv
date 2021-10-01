// Project: libv.rev, File: src/libv/rev/shader_loader.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/rev/shader_loader.hpp>
// libv
#include <libv/algo/erase_if_stable.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
// pro
#include <libv/rev/glsl_compose.hpp>
#include <libv/rev/internal_shader.hxx>
#include <libv/rev/internal_shader_loader.hxx>
#include <libv/rev/log.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

ShaderLoader::ShaderLoader(std::filesystem::path base_include_directory) :
	self(std::make_shared<InternalShaderLoader>()) {

	auto file_tracker = [this](std::string_view include_path, std::string_view file_path) {
		// Callback for glsl_source_loader whenever a new include loaded a file
		// NOTE: Indirectly protected by mutex lock in internal_load

		auto queue_reload = [internal_wp = std::weak_ptr(*self->current_loading_internal)](const libv::fsw::Event& e) {
			// Callback for fsw whenever a file changes
			// NOTE: Not protected by mutex but queue_source_reload is thread safe

			auto internal_sp = internal_wp.lock();
			if (!internal_sp)
				return; // Shader was destroyed in the meantime

			auto loader_sp = internal_sp->loader.lock();
			if (!loader_sp)
				return; // Loader was destroyed in the meantime

			log_rev.info("Requesting shader reload for {} because: {}", internal_sp->name_, e);
			loader_sp->queue_source_reload.push_back(std::move(internal_sp));
		};

		log_rev.trace("Tracking file {} for {} because of include {}", file_path, (*self->current_loading_internal)->name_, include_path);
		const auto token = self->watcher.subscribe_file(file_path, std::move(queue_reload));
		self->current_loading_stage->included_sources.emplace_back(std::string(include_path), token);
	};

//	self->glsl_source_loader.add_include_directory("block/", lookup_block_source);
	self->glsl_source_loader.add_include_directory("", base_include_directory.generic_string(), std::move(file_tracker));
//	for (const auto mod : mods) {
//		self->glsl_source_loader.add_include_directory(
//				mod.shader_include_path,
//				mod.shader_filesystem_path,
//				std::move(file_loader));
//	}
}

ShaderLoader::~ShaderLoader() {
	// For the sake of forward declared shared_ptr
}

// -------------------------------------------------------------------------------------------------

void InternalShaderLoader::unsubscribe(InternalShader* internal_ptr) {
	for (auto& stage : internal_ptr->stages)
		for (auto& included_sources : stage.included_sources)
			watcher.unsubscribe(included_sources.fsw_token);

	const auto lock = std::unique_lock(mutex);

	_broadcast(ShaderUnload{internal_ptr->id()});

	libv::erase_stable(shaders, internal_ptr);
}

// -------------------------------------------------------------------------------------------------

std::shared_ptr<InternalShader> InternalShaderLoader::internal_load(libv::type_uid uniformTID, ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1) {
	const auto lock = std::unique_lock(mutex);

	const auto comp = [&](const auto* s) { return s->compare(uniformTID, type0, path0, type1, path1); };
	const auto it = std::ranges::lower_bound(shaders, 0, {}, comp);
	if (it != shaders.end() && 0 == (*it)->compare(uniformTID, type0, path0, type1, path1))
		return (*it)->shared_from_this();

	auto internal_shader = std::make_shared<InternalShader>(weak_from_this(), ucc(), uniformTID);
	internal_shader->stages.emplace_back(type0, path0);
	internal_shader->stages.emplace_back(type1, path1);
	internal_shader->finish();
	log_rev.trace("Create new shader: {}", internal_shader->name_);

	shaders.emplace(it, internal_shader.get());
	queue_source_reload.push_back(internal_shader);
	return internal_shader;
}

std::shared_ptr<InternalShader> InternalShaderLoader::internal_load(libv::type_uid uniformTID, ucc_type ucc, libv::gl::ShaderType type0, std::string path0, libv::gl::ShaderType type1, std::string path1, libv::gl::ShaderType type2, std::string path2) {
	const auto lock = std::unique_lock(mutex);

	const auto comp = [&](const auto* s) { return s->compare(uniformTID, type0, path0, type1, path1, type2, path2); };
	const auto it = std::ranges::lower_bound(shaders, 0, {}, comp);
	if (it != shaders.end() && 0 == (*it)->compare(uniformTID, type0, path0, type1, path1, type2, path2))
		return (*it)->shared_from_this();

	auto internal_shader = std::make_shared<InternalShader>(weak_from_this(), ucc(), uniformTID);
	internal_shader->stages.emplace_back(type0, path0);
	internal_shader->stages.emplace_back(type1, path1);
	internal_shader->stages.emplace_back(type2, path2);
	internal_shader->finish();
	log_rev.trace("Create new shader: {}", internal_shader->name_);

	shaders.emplace(it, internal_shader.get());
	queue_source_reload.push_back(internal_shader);
	return internal_shader;
}

void InternalShaderLoader::update_fs() {
	auto old_tokens = std::vector<libv::fsw::WatchToken>{};

	while (auto internal_opt = queue_source_reload.pop_optional()) {
		auto& internal = *internal_opt;

		++internal->load_version;
		log_rev.info("Loading {} v{}", internal->name_, internal->load_version);

		bool had_load_source_error = false;
		for (auto& stage : internal->stages) {

			// Move out old watch tokens
			for (auto& [_, token] : stage.included_sources)
				old_tokens.emplace_back(token);
			stage.included_sources.clear();

			// Load source
			try {
				current_loading_internal = &internal; // Setup temp var
				current_loading_stage = &stage; // Setup temp var
				stage.source = glsl_source_loader.load_source(stage.source_path, stage.defines);
				current_loading_stage = nullptr; // Clear temp var
				current_loading_internal = nullptr; // Clear temp var

			} catch (const glsl_failed_include_exception& e) {
				log_rev.error("--- Failed to load shader: {} v{} ({}) ---", internal->name_, internal->load_version, internal->id());
				log_rev.error("Failed to include: \"{}\" from file: \"{}\" - {}: {}", e.include_path, e.file_path, e.ec, e.ec.message());
				for (const auto& [file, line] : e.include_stack)
					log_rev.error("    Included from: {}:{}", file, line);

				_broadcast(ShaderLoadFailure{
						internal->id(),
						BaseShader(internal),
						ShaderLoadFailure::IncludeFailure{
							e.file_path,
							e.ec,
							e.include_path,
							e.include_stack,
						},
						std::nullopt,
						std::nullopt});

				had_load_source_error = true;
				current_loading_stage = nullptr; // Clear temp var
				current_loading_internal = nullptr; // Clear temp var
				break; // No need to look at the other stages
			}
		}

		// Clean up old watch tokens
		for (const auto& token : old_tokens)
			watcher.unsubscribe(token);
		old_tokens.clear();

		if (had_load_source_error)
			queue_shader_failed_load.push_back(std::move(internal));
		else
			queue_shader_update.push_back(std::move(internal));
	}
}

void InternalShaderLoader::update_gl(libv::gl::GL& gl) {
	const auto fallback_to_default = [](auto& internal) {
		// If this is the first load, create a new program to replace the default one
		static constexpr auto fallback_red_vs = R"(
		#version 330 core

		layout(location = 0) in vec3 vertexPosition;

		void main() {
			gl_Position = vec4(vertexPosition, 1);
		})";

		static constexpr auto fallback_red_fs = R"(
		#version 330 core

		out vec4 result;

		void main() {
			result = vec4(1.0, 0.0, 0.0, 1.0);
		})";

		if (internal->current_version == -1) {
			// Assigning a default red sources to the shader
			internal->program.vertex(fallback_red_vs);
			internal->program.fragment(fallback_red_fs);
			// We still bind the uniforms to ensure allocation of layout/index variables inside glr
			internal->uniformContainer->bind_assign(internal->program);
			internal->uniformContainer->bind_blocks(internal->program);

			log_rev.error("Settings shader: {} v{} ({}) to fallback red", internal->name_, internal->load_version, internal->id());
		} else {
			log_rev.error("Keeping shader: {} v{} ({}) as last working version v{}", internal->name_, internal->load_version, internal->id(), internal->current_version);
		}
	};

	auto previous_program = gl.bound_program();

	while (auto internal_opt = queue_shader_failed_load.pop_optional()) {
		// For those who failed to load: just assign the fallback
		auto& internal = *internal_opt;
		fallback_to_default(internal);
	}

	while (auto internal_opt = queue_shader_update.pop_optional()) {
		auto& internal = *internal_opt;

		log_rev.info("Updating {} v{}", internal->name_, internal->load_version);

		libv::gl::Program program;
		gl(program).create();

		auto shader_compile_failed = false;
		auto shader_compile_error = std::string{};

		for (const auto& stage : internal->stages) {
			libv::gl::Shader shader;

			gl(shader).create(stage.type);
			gl(shader).compile(stage.source.code);

			if (!gl(shader).status()) {
				shader_compile_failed = true;
				shader_compile_error = stage.source.translateErrorMessageFilenames(gl(shader).info());
			} else {
				gl(program).attach(shader);
			}

			gl(shader).destroy();

			if (shader_compile_failed)
				break;
		}

		if (shader_compile_failed) {
			log_rev.error("--- Failed to compile shader: {} v{} ({}) ---\n{}", internal->name_, internal->load_version, internal->id(), shader_compile_error);
			_broadcast(ShaderLoadFailure{
					internal->id(),
					BaseShader(internal),
					std::nullopt,
					ShaderLoadFailure::CompileFailure{std::move(shader_compile_error)},
					std::nullopt});
			fallback_to_default(internal);

		} else {
			// Shaders compiled
			gl(program).link();

			if (!gl(program).status()) {
				auto shader_link_error = gl(program).info();
				log_rev.error("--- Failed to link shader: {} v{} ({}) ---\n{}", internal->name_, internal->load_version, internal->id(), shader_link_error);
				_broadcast(ShaderLoadFailure{
						internal->id(),
						BaseShader(internal),
						std::nullopt,
						std::nullopt,
						ShaderLoadFailure::LinkFailure{std::move(shader_link_error)}});
				fallback_to_default(internal);

			} else {
				internal->current_version = internal->load_version;
				internal->program._native_swap(program);
				internal->uniformContainer->bind_assign(internal->program);
				internal->uniformContainer->bind_blocks(internal->program);
				_broadcast(ShaderLoadSuccess{internal->id(), BaseShader(internal)});
			}
		}

		if (program.id != 0)
			// Destroy the failed or the swapped out old program
			gl(program).destroy();
	}

	// Cleanup - Restore the OpenGL state's original program use
	gl(previous_program).use();
	libv::gl::checkGL();
}

void ShaderLoader::update(libv::gl::GL& gl) {
	auto lock = std::unique_lock(self->mutex);
	self->update_fs();
	self->update_gl(gl);
}

// -------------------------------------------------------------------------------------------------

void ShaderLoader::foreach_shader(libv::function_ref<void(BaseShader)> func) {
	const auto lock = std::unique_lock(self->mutex);
	for (const auto& internal_ptr : self->shaders) {
		auto internal_sp = internal_ptr->shared_from_this();
		if (!internal_sp)
			continue; // This shader was destroyed in the meantime, skip

		func(BaseShader(std::move(internal_sp)));
	}
}

void ShaderLoader::on_success(shader_load_success_cb success_cb) {
	const auto lock = std::unique_lock(self->mutex);
	self->success_cbs.emplace_back(std::move(success_cb));
}

void ShaderLoader::on_failure(shader_load_failure_cb failure_cb) {
	const auto lock = std::unique_lock(self->mutex);
	self->failure_cbs.emplace_back(std::move(failure_cb));
}

void ShaderLoader::on_unload(shader_unload_cb unload_cb) {
	const auto lock = std::unique_lock(self->mutex);
	self->unload_cbs.emplace_back(std::move(unload_cb));
}

void ShaderLoader::clear_on_updates() {
	const auto lock = std::unique_lock(self->mutex);
	self->success_cbs.clear();
	self->failure_cbs.clear();
	self->unload_cbs.clear();
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
