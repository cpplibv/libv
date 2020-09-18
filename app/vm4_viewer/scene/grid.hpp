// Project: libv, File: app/vm4_viewer/scene/grid.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/glr/mesh.hpp>
//#include <libv/glr/program.hpp>
//#include <libv/glr/queue_fwd.hpp>
//#include <libv/glr/uniform.hpp>
// std
//#include <optional>
// pro
//#include <vm4_viewer/scene/camera.hpp>
//#include <vm4_viewer/scene/camera_2.hpp>
//#include <vm4_viewer/scene/light.hpp>
//#include <vm4_viewer/scene/model.hpp>
//#include <vm4_viewer/scene/shader_color.hpp>
//#include <vm4_viewer/scene/shader_line.hpp>
//#include <vm4_viewer/scene/shader_model.hpp>
#include <vm4_viewer/scene/shader_grid.hpp>
#include <vm4_viewer/shader_loader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct Grid {
	static constexpr libv::vec4f color_grid = {0.8f, 0.8f, 0.8f, 0.15f};
//	static constexpr libv::vec4f color_low = {0.8f, 0.8f, 0.8f, 0.5f};

	static constexpr libv::vec4f color_x = {0.8f, 0.0f, 0.0f, 0.5f};
	static constexpr libv::vec4f color_y = {0.0f, 0.8f, 0.0f, 0.5f};
	static constexpr libv::vec4f color_z = {0.0f, 0.0f, 0.8f, 0.5f};

	app::ShaderGrid shader_grid;

	libv::glr::Mesh mesh;

public:
	Grid(ShaderLoader& shader_loader);

public:
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
