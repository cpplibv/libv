// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

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
	app::ShaderGrid shader_grid;
	static constexpr libv::vec4f color_grid = {0.8f, 0.8f, 0.8f, 0.15f};

	libv::glr::Mesh mesh;

public:
	Grid(ShaderLoader& shader_loader);

public:
	void render(libv::glr::Queue& gl, libv::vec2f canvas_size);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
