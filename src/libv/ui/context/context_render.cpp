// Project: libv.ui, File: src/libv/ui/context/context_render.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/context/context_render.hpp>
// std
#include <vector>
// libv
#include <libv/glr/layout_std140.hpp>
#include <libv/glr/mesh.hpp>
#include <libv/glr/program.hpp>
#include <libv/glr/queue.hpp>
#include <libv/glr/texture.hpp>
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_buffer.hpp>
// pro
#include <libv/ui/component/detail/component.hpp>
#include <libv/ui/font_2D.hpp>
#include <libv/ui/shader/shader_font.hpp>
#include <libv/ui/shader/shader_image.hpp>
#include <libv/ui/shader/shader_quad.hpp>
#include <libv/ui/text_layout.hpp>
#include <libv/ui/texture_2D.hpp>
#include <libv/ui/log.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//constexpr auto attribute_position  = libv::glr::Attribute<0, libv::vec3f>{};
//constexpr auto attribute_color0    = libv::glr::Attribute<1, libv::vec4f>{};
//constexpr auto attribute_texture0  = libv::glr::Attribute<2, libv::vec2f>{};

//constexpr auto attribute_normal    = libv::glr::Attribute<1, libv::vec3f>{};
//constexpr auto attribute_tangent   = libv::glr::Attribute<6, libv::vec3f>{};
//constexpr auto attribute_bitangent = libv::glr::Attribute<7, libv::vec3f>{};

//const auto uniformBlock_sphere   = libv::glr::UniformBlockBinding{0, "Sphere"};
//const auto uniformBlock_matrices = libv::glr::UniformBlockBinding{1, "Matrices"};
//const auto uniformBlock_material = libv::glr::UniformBlockBinding{2, "Material"};
//const auto uniformBlock_lights   = libv::glr::UniformBlockBinding{3, "Lights"};
//const auto uniformBlock_skeleton = libv::glr::UniformBlockBinding{4, "Skeleton"};

//constexpr auto textureChannel_diffuse = libv::gl::TextureChannel{0};
//constexpr auto textureChannel_normal  = libv::gl::TextureChannel{1};

// -------------------------------------------------------------------------------------------------

class ImplContextRender {
public:
	float current_time;
	time_point current_time_frame;

public:
	std::vector<libv::vec3f> vtx_positions;
	std::vector<libv::vec4f> vtx_color0s;
	std::vector<libv::vec2f> vtx_texture0s;
	std::vector<uint32_t> vtx_indices;

	struct UniformLayoutUIInfo {
		libv::glr::Uniform_mat4f matMVP;
		libv::glr::Uniform_mat4f matM;
		libv::glr::Uniform_vec2f clip_pos;
		libv::glr::Uniform_vec2f clip_size;
		libv::glr::Uniform_vec2f component_pos;
		libv::glr::Uniform_vec2f component_size;
		libv::glr::Uniform_vec2f mouse_position;
		libv::glr::Uniform_vec2f ui_size;
		libv::glr::Uniform_float time_frame; // Time in seconds, Resets each hour
		// Uniform_float time_frame;
		// Uniform_float time_simulation;

		LIBV_REFLECTION_ACCESS(matMVP);
		LIBV_REFLECTION_ACCESS(matM);
		LIBV_REFLECTION_ACCESS(clip_pos);
		LIBV_REFLECTION_ACCESS(clip_size);
		LIBV_REFLECTION_ACCESS(component_pos);
		LIBV_REFLECTION_ACCESS(component_size);
		LIBV_REFLECTION_ACCESS(mouse_position);
		LIBV_REFLECTION_ACCESS(ui_size);
		LIBV_REFLECTION_ACCESS(time_frame);
	};

	const libv::glr::UniformBlockLayout<UniformLayoutUIInfo> layout_UIInfo = libv::glr::layout_std140<UniformLayoutUIInfo>(uniform_block_UIInfo);

	libv::glr::Mesh mesh_stream{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StreamDraw};
	libv::glr::UniformBufferStream uniform_stream;

	bool initialized = false;
	libv::glr::Texture white_zero_texture;

public:
	libv::vec2f mouse_position;
	libv::vec2f ui_size;

public:
	size_t ll_base_index;
	size_t ll_base_vertex;
	size_t ll_num_vertex;
	size_t ll_num_index;

public:
	struct TaskUI {
		libv::glr::State state;
		libv::glr::Program shader;
		libv::glr::Texture texture0;

		libv::glr::UniformBlockStreamView_std140 uniform_block;

		libv::glr::VertexIndex base_vertex;
		libv::glr::VertexIndex base_index;
		libv::glr::VertexIndex num_indices;
	};

	struct TaskNative {
		libv::glr::State state;
		std::function<void(libv::glr::Queue&)> callback;
	};

	std::vector<std::variant<TaskUI, TaskNative>> tasks;

private:
	void init() {
		libv::vec4uc white_zero_texture_data = {255, 255, 255, 255};
		libv::glr::Texture2D::R8_G8_B8_A8 texture;
		texture.storage(1, {1, 1});
		texture.image(0, {0, 0}, {1, 1}, &white_zero_texture_data);
		white_zero_texture = texture.base_ref();
	}

public:
	void begin_render(const Component& root, libv::glr::Queue& glr) {
		if (!initialized) {
			init();
			initialized = true;
		}

		// NOTE: No state guards as we are manipulating the glr state for the whole render process
		//			Fixed saved states (UI only uses a handful of them, could improve this)

		glr.state.enableBlend();
		glr.state.blendSrc_SourceAlpha();
		glr.state.blendDst_One_Minus_SourceAlpha();

		glr.state.enableCullFace();
		glr.state.cullBackFace();
		glr.state.frontFaceCCW();

		glr.state.disableDepthTest();
		glr.state.depthFunctionLess();

		glr.state.polygonModeFill();
		glr.state.clipPlanes(4);

		glr.projection = libv::mat4f::ortho(root.layout_position2(), root.layout_size2());
		glr.view = libv::mat4f::identity();
		glr.model = libv::mat4f::identity();

		glr.viewport(
				root.layout_position2().cast<int32_t>(),
				root.layout_size2().cast<int32_t>()
		);
	}

	void execute_render(libv::glr::Queue& glr) {
		const auto guard_m = glr.model.push_guard();
		const auto guard_v = glr.view.push_guard();
		const auto guard_p = glr.projection.push_guard();
		const auto guard_s = glr.state.push_guard();

		// Upload new state
		{
			mesh_stream.clear();

			auto position = mesh_stream.attribute(attribute_position);
			auto color0 = mesh_stream.attribute(attribute_color0);
			auto texture0 = mesh_stream.attribute(attribute_texture0);
			auto index = mesh_stream.index();

			position.set_from_range(vtx_positions);
			color0.set_from_range(vtx_color0s);
			texture0.set_from_range(vtx_texture0s);
			index.set_from_range(vtx_indices);

			vtx_positions.clear();
			vtx_color0s.clear();
			vtx_texture0s.clear();
			vtx_indices.clear();
		}

		// Execute (queue) tasks
		for (auto& task : tasks) {
			if (auto* t = std::get_if<ImplContextRender::TaskUI>(&task)) {
				glr.state = t->state;
				glr.program(t->shader);
				glr.uniform(t->uniform_block);
				glr.texture(t->texture0, texture_channel0);
				glr.render(mesh_stream, t->base_vertex, t->base_index, t->num_indices);
			} else if (auto* t = std::get_if<ImplContextRender::TaskNative>(&task)) {
				glr.state = t->state;
				t->callback(glr);
			}
		}

		// Cleanup
		tasks.clear();
		glr.callback([this](libv::gl::GL&) {
			uniform_stream.clear();
		});
	}

	void texture_2D(libv::vec2f pos, libv::vec2f size, libv::vec2f uv00, libv::vec2f uv11, libv::vec4f color, libv::glr::Texture texture, libv::glr::Program shader,
			libv::glr::Queue& glr,
			const Component& current_component,
			libv::vec2f clip_pos,
			libv::vec2f clip_size);
};

// =================================================================================================

ContextRender::ContextRender() :
	self(std::make_unique<ImplContextRender>()) { }

ContextRender::~ContextRender() {
	// For the sake of forward declared unique_ptr
}

Renderer ContextRender::root_renderer(const Component& root, libv::glr::Queue& glr, time_point time_frame, time_duration time, libv::vec2f mouse_position, libv::vec2f ui_size) {
	auto r = Renderer{*self, glr, root};
	self->current_time = static_cast<float>(std::fmod(time.count(), 3600.0f));
	self->current_time_frame = time_frame;
	self->mouse_position = mouse_position;
	self->ui_size = ui_size;
	self->begin_render(root, glr);
	return r;
}

void ContextRender::execute_render(libv::glr::Queue& glr) {
	self->execute_render(glr);
}

// -------------------------------------------------------------------------------------------------

Renderer::Renderer(ImplContextRender& context, libv::glr::Queue& glr, const Component& root) noexcept :
	context(context),
	glr(glr),
	current_component(root),
	clip_pos(current_component.layout_position2()),
	clip_size(current_component.layout_size2()) {
}

Renderer::Renderer(const Renderer& parent_renderer, const Component& child) noexcept :
	context(parent_renderer.context),
	glr(parent_renderer.glr),
	current_component(child),
	clip_pos(parent_renderer.clip_pos),
	clip_size(parent_renderer.clip_size) {

	// IDEA: If create/destroy -> allocate/deallocate data storage for the component and reuse it if not invalidated
}

// --- Context -------------------------------------------------------------------------------------

time_point Renderer::time_frame() const noexcept {
	return context.current_time_frame;
}

// --- State ---------------------------------------------------------------------------------------

void Renderer::translate(libv::vec3f value) noexcept {
	glr.model.translate(value);
	clip_pos -= xy(value);
}

void Renderer::clip(libv::vec2f pos, libv::vec2f size) noexcept {
	const auto bl = libv::vec::max(clip_pos, pos);
	const auto tr = libv::vec::min(clip_pos + clip_size, pos + size);

	clip_pos = bl;
	clip_size = tr - bl;
}

// --- Low level -----------------------------------------------------------------------------------

void Renderer::begin_triangles() {
	context.ll_base_vertex = context.vtx_positions.size();
	context.ll_base_index = context.vtx_indices.size();
	context.ll_num_vertex = 0;
	context.ll_num_index = 0;
}

void Renderer::end(const Texture2D_view& texture, const ShaderImage_view& shader) {
	const auto& layout_UIInfo = context.layout_UIInfo;

	auto& task = std::get<ImplContextRender::TaskUI>(context.tasks.emplace_back(
			std::in_place_type<ImplContextRender::TaskUI>,
			glr.state.state(),
			shader->base_ref(),
			texture->texture().base_ref(),
			context.uniform_stream.block_stream(layout_UIInfo),
			static_cast<uint32_t>(context.ll_base_vertex),
			static_cast<uint32_t>(context.ll_base_index),
			static_cast<uint32_t>(context.ll_num_index)
	));

	task.uniform_block[layout_UIInfo.matMVP] = glr.mvp();
	task.uniform_block[layout_UIInfo.matM] = glr.model.top();
	task.uniform_block[layout_UIInfo.clip_pos] = clip_pos;
	task.uniform_block[layout_UIInfo.clip_size] = clip_size;
	task.uniform_block[layout_UIInfo.component_pos] = current_component.layout_position2();
	task.uniform_block[layout_UIInfo.component_size] = current_component.layout_size2();
	task.uniform_block[layout_UIInfo.mouse_position] = context.mouse_position;
	task.uniform_block[layout_UIInfo.ui_size] = context.ui_size;
	task.uniform_block[layout_UIInfo.time_frame] = context.current_time;

	assert(context.vtx_positions.size() == context.vtx_texture0s.size());
	assert(context.vtx_positions.size() == context.vtx_color0s.size());
	assert(context.vtx_positions.size() == context.ll_base_vertex + context.ll_num_vertex);
	assert(context.vtx_indices.size() == context.ll_base_index + context.ll_num_index);
}

void Renderer::index_strip(std::span<const uint32_t> indices) {
	size_t i = 0;

	for (; i < indices.size() && context.ll_num_index + i < 3; ++i) {
		context.vtx_indices.emplace_back(indices[i]);
	}

	for (; i < indices.size(); ++i) {
		const auto n = context.vtx_indices.size();
		const auto is_even = ((n - context.ll_base_index) % 2) == 0;

		if (is_even) {
			// Even index: New triangle (n-1, n-2, n)
			context.vtx_indices.emplace_back(context.vtx_indices[n - 1]);
			context.vtx_indices.emplace_back(context.vtx_indices[n - 2]);
			context.vtx_indices.emplace_back(indices[i]);

		} else {
			// Odd index: New triangle (n-2, n, n-1)
			context.vtx_indices.emplace_back(context.vtx_indices[n - 2]);
			context.vtx_indices.emplace_back(indices[i]);
			context.vtx_indices.emplace_back(context.vtx_indices[n - 1]);
		}
	}

	context.ll_num_index = context.vtx_indices.size() - context.ll_base_index;
}

void Renderer::index_strip(std::initializer_list<const uint32_t> indices) {
	index_strip(std::span<const uint32_t>(indices.begin(), indices.size()));
}

void Renderer::vertex(libv::vec3f pos, libv::vec2f uv, libv::vec4f color) {
	context.vtx_positions.emplace_back(pos);
	context.vtx_texture0s.emplace_back(uv);
	context.vtx_color0s.emplace_back(color);
	context.ll_num_vertex++;
}

//void Renderer::vertex_2(libv::vec2f pos, libv::vec2f uv, libv::vec4f color) {
//	(void) pos;
//	(void) uv;
//	(void) color;
//}
//
//void Renderer::vertex_2s(libv::vec2f pos, libv::vec2f uv, libv::vec4f color) {
//	(void) pos;
//	(void) uv;
//	(void) color;
//}
//
//void Renderer::vertex_3(libv::vec2f pos, libv::vec2f uv, libv::vec4f color) {
//	(void) pos;
//	(void) uv;
//	(void) color;
//}
//
//void Renderer::vertex_3s(libv::vec2f pos, libv::vec2f uv, libv::vec4f color) {
//	(void) pos;
//	(void) uv;
//	(void) color;
//}

// --- Mid level -----------------------------------------------------------------------------------

//void Renderer::quad(libv::vec2f pos, libv::vec2f size, libv::vec2f uv00, libv::vec2f uv11, libv::vec4f color) {
//	(void) pos;
//	(void) size;
//	(void) uv00;
//	(void) uv11;
//	(void) color;
//}

// --- High level ----------------------------------------------------------------------------------

void ImplContextRender::texture_2D(libv::vec2f pos, libv::vec2f size, libv::vec2f uv00, libv::vec2f uv11, libv::vec4f color, libv::glr::Texture texture, libv::glr::Program shader,
		libv::glr::Queue& glr,
		const Component& current_component,
		libv::vec2f clip_pos,
		libv::vec2f clip_size) {
	const auto base_vertex = vtx_positions.size();
	const auto base_index = vtx_indices.size();
	const auto num_vertex = 4;
	const auto num_index = 6;

//	const auto batch = batch(4, 6, texture, shader);
//
//	const auto vtx = batch.vertex();
//	const auto idx = batch.index();
//
// TODO P5: Can be optimized with batch request of vtx data and just index into it
//	const auto vtx = request_vertex(4);
//	const auto idx = request_index(6);
//
//	vtx.position[0] = {pos.x, pos.y, 0.f};
//	vtx.position[1] = {pos.x + size.x, pos.y, 0.f};
//	vtx.position[2] = {pos.x + size.x, pos.y + size.y, 0.f};
//	vtx.position[3] = {pos.x, pos.y + size.y, 0.f};
//
//	vtx.texture0[0] = {uv00.x, uv00.y};
//	vtx.texture0[1] = {uv11.x, uv00.y};
//	vtx.texture0[2] = {uv11.x, uv11.y};
//	vtx.texture0[3] = {uv00.x, uv11.y};
//
//	vtx.color0[0] = color;
//	vtx.color0[1] = color;
//	vtx.color0[2] = color;
//	vtx.color0[3] = color;
//
//	idx[0] = base_vertex + 0;
//	idx[1] = base_vertex + 1;
//	idx[2] = base_vertex + 2;
//	idx[3] = base_vertex + 2;
//	idx[4] = base_vertex + 3;
//	idx[5] = base_vertex + 0;

	vtx_positions.emplace_back(pos.x, pos.y, 0.f);
	vtx_positions.emplace_back(pos.x + size.x, pos.y, 0.f);
	vtx_positions.emplace_back(pos.x + size.x, pos.y + size.y, 0.f);
	vtx_positions.emplace_back(pos.x, pos.y + size.y, 0.f);

	vtx_texture0s.emplace_back(uv00.x, uv00.y);
	vtx_texture0s.emplace_back(uv11.x, uv00.y);
	vtx_texture0s.emplace_back(uv11.x, uv11.y);
	vtx_texture0s.emplace_back(uv00.x, uv11.y);

	vtx_color0s.insert(vtx_color0s.end(), num_vertex, color);

	vtx_indices.emplace_back(0);
	vtx_indices.emplace_back(1);
	vtx_indices.emplace_back(2);
	vtx_indices.emplace_back(2);
	vtx_indices.emplace_back(3);
	vtx_indices.emplace_back(0);

	auto& task = std::get<TaskUI>(tasks.emplace_back(
			std::in_place_type<TaskUI>,
			glr.state.state(),
			std::move(shader),
			std::move(texture),
			uniform_stream.block_stream(layout_UIInfo),
			static_cast<uint32_t>(base_vertex),
			static_cast<uint32_t>(base_index),
			static_cast<uint32_t>(num_index)
	));

	task.uniform_block[layout_UIInfo.matMVP] = glr.mvp();
	task.uniform_block[layout_UIInfo.matM] = glr.model;
	task.uniform_block[layout_UIInfo.clip_pos] = clip_pos;
	task.uniform_block[layout_UIInfo.clip_size] = clip_size;
	task.uniform_block[layout_UIInfo.component_pos] = current_component.layout_position2();
	task.uniform_block[layout_UIInfo.component_size] = current_component.layout_size2();
	task.uniform_block[layout_UIInfo.mouse_position] = mouse_position;
	task.uniform_block[layout_UIInfo.ui_size] = ui_size;
	task.uniform_block[layout_UIInfo.time_frame] = current_time;

	assert(vtx_positions.size() == vtx_texture0s.size());
	assert(vtx_positions.size() == vtx_color0s.size());
	assert(base_vertex + num_vertex == vtx_positions.size());
	assert(base_index + num_index == vtx_indices.size());
}

void Renderer::quad(libv::vec2f pos, libv::vec2f size, libv::vec4f color, const ShaderQuad_view& shader) {
	context.texture_2D(pos, size, {0, 0}, {1, 1}, color, context.white_zero_texture, shader->base_ref(),
			glr, current_component, clip_pos, clip_size);
}

void Renderer::text(libv::vec2f pos, TextLayout& text_, libv::vec4f color, const Font2D_view& font, const ShaderFont_view& shader) {
	const auto& vd = text_.vertices_data();

	const auto base_vertex = context.vtx_positions.size();
	const auto base_index = context.vtx_indices.size();
	const auto num_vertex = vd.positions.size();
	const auto num_index = vd.indices.size();

	context.vtx_positions.insert(context.vtx_positions.end(), vd.positions.begin(), vd.positions.end());
	context.vtx_texture0s.insert(context.vtx_texture0s.end(), vd.texture0s.begin(), vd.texture0s.end());
	context.vtx_color0s.insert(context.vtx_color0s.end(), num_vertex, color);
	context.vtx_indices.insert(context.vtx_indices.end(), vd.indices.begin(), vd.indices.end());

	const auto guard_s = glr.state.push_guard();

	glr.state.blendSrc_Source1Color();
	glr.state.blendDst_One_Minus_Source1Color();

	const auto& layout_UIInfo = context.layout_UIInfo;

	auto& task = std::get<ImplContextRender::TaskUI>(context.tasks.emplace_back(
			std::in_place_type<ImplContextRender::TaskUI>,
			glr.state.state(),
			shader->base_ref(),
			font->texture().base_ref(),
			context.uniform_stream.block_stream(layout_UIInfo),
			static_cast<uint32_t>(base_vertex),
			static_cast<uint32_t>(base_index),
			static_cast<uint32_t>(num_index)
	));

	task.uniform_block[layout_UIInfo.matMVP] = glr.mvp().translate({pos, 0});
	task.uniform_block[layout_UIInfo.matM] = glr.model.top().translate_copy({pos, 0});
	task.uniform_block[layout_UIInfo.clip_pos] = clip_pos;
	task.uniform_block[layout_UIInfo.clip_size] = clip_size;
	task.uniform_block[layout_UIInfo.component_pos] = current_component.layout_position2();
	task.uniform_block[layout_UIInfo.component_size] = current_component.layout_size2();
	task.uniform_block[layout_UIInfo.mouse_position] = context.mouse_position;
	task.uniform_block[layout_UIInfo.ui_size] = context.ui_size;
	task.uniform_block[layout_UIInfo.time_frame] = context.current_time;

	assert(context.vtx_positions.size() == context.vtx_texture0s.size());
	assert(context.vtx_positions.size() == context.vtx_color0s.size());
	assert(base_vertex + num_vertex == context.vtx_positions.size());
	assert(base_index + num_index == context.vtx_indices.size());
}

void Renderer::texture_2D(libv::vec2f pos, libv::vec2f size, libv::vec2f uv00, libv::vec2f uv11, libv::vec4f color, const Texture2D_view& texture, const ShaderImage_view& shader) {
	context.texture_2D(pos, size, uv00, uv11, color, texture->texture().base_ref(), shader->base_ref(),
			glr, current_component, clip_pos, clip_size);
}

// --- Component level -----------------------------------------------------------------------------

Renderer Renderer::enter(const Component& child) {
	return Renderer{*this, child};
}

// --- Native level --------------------------------------------------------------------------------

void Renderer::native(std::function<void(libv::glr::Queue&)> func) {
	context.tasks.emplace_back(
			std::in_place_type<ImplContextRender::TaskNative>,
			glr.state.state(),
			std::move(func)
	);
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
