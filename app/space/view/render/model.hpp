// Project: libv, File: app/space/view/model.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/glr/mesh.hpp>
//#include <libv/math/quat.hpp>
//#include <libv/math/vec.hpp>
#include <libv/vm4/model.hpp>
// pro
#include <space/view/render/shaders.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

struct Node {
//	std::string name;
//	Node** children;
//	std::vector<std::unique_ptr<Node>> children;
};

struct Object : Node {
//	libv::vec3f position;
	// TODO P4: use quaternion for rotation
//	libv::quat orientation; // RAD
//	libv::vec3f scale;
};

class Model : Object {
private:
	libv::glr::Mesh mesh;

public:
	libv::vm4::Model vm4;

//	uint32_t rootNodeID;

public:
	explicit Model(libv::vm4::Model model);

private:
	void load();

public:
	void render(libv::glr::Queue& gl, ShaderFleet& shader, libv::glr::UniformBuffer& uniform_stream);

private:
	void render_node(libv::glr::Queue& gl, uint32_t id, ShaderFleet& shader, libv::glr::UniformBuffer& uniform_stream);
};


// -------------------------------------------------------------------------------------------------

} // namespace space

