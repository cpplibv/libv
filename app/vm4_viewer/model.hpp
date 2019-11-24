// File: test.cpp, Created on 2014. október 25. 23:38, Author: Vader

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue_fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/vm4/model.hpp>
//#include <libv/math/quaternion.hpp>
// pro
#include <vm4_viewer/shader.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class Model {
private:
	libv::vec3f position;
//	libv::quatf direction;

private:
	libv::glr::Mesh mesh;
	ShaderModel shader;

private:
	libv::vm4::Model model;

//	uint32_t rootNodeID;

public:
	Model(libv::vm4::Model model);

private:
	void load();

public:
	void render(libv::glr::Queue& gl);

private:
	void node(libv::glr::Queue& gl, uint32_t id);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
