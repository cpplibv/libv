// Project: libv, File: app/vm4_viewer/scene/model.hpp

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/glr/queue_fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/vm4/model.hpp>
//#include <libv/math/quaternion.hpp>
// pro
#include <vm4_viewer/scene/shader_model.hpp>
#include <vm4_viewer/scene/object.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class Model : Object {
private:
//	libv::vec3f position;
//	libv::quatf direction;

private:
	libv::glr::Mesh mesh;

public:
	libv::vm4::Model vm4;

//	uint32_t rootNodeID;

public:
	Model(libv::vm4::Model model);

private:
	void load();

public:
	void render(libv::glr::Queue& gl, ShaderModel& shader);

private:
	void node(libv::glr::Queue& gl, uint32_t id, ShaderModel& shader);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
