// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/glr/mesh.hpp>
#include <libv/math/mat.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/utility/ordering.hpp>
// std
#include <string>
#include <string_view>
// pro
#include <libv/rev/resource/material.hpp>
#include <libv/rev/resource/material_override.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

struct InternalModelNode {
	libv::mat4f transformation;
	Material material;
	int baseVertex;
	int baseIndex;
	int numIndices;
};

// -------------------------------------------------------------------------------------------------

class InternalModel : public libv::ref_count_base<InternalModel> {
	friend libv::ref_count_access;

public:
	libv::glr::Mesh glrMesh;
	boost::container::small_vector<InternalModelNode, 5> nodes;

	std::string name_;
	int64_t movID = 0; // Material Override ID: 0 = no override, -1 = unspecified override

	std::weak_ptr<InternalModelLoader> loader_;

public:
	inline InternalModel(
			std::weak_ptr<InternalModelLoader> loader,
			std::string name,
			const libv::glr::Mesh& defaultGLRMesh,
			const InternalModelNode& defaultNode,
			int64_t movID) :
		glrMesh(defaultGLRMesh),
		nodes(1, defaultNode),
		name_(std::move(name)),
		movID(movID),
		loader_(std::move(loader)) {
	}

	InternalModel(const libv::vm4::Model& model, const Material& mov);
	InternalModel(const libv::vm4::Model& model, const MaterialLoaderFunc& mov);

	[[nodiscard]] inline int compare(std::string_view name, int64_t movID_) const noexcept {
		if (const auto v = name_ <=> name; v != 0) return libv::ordering_as_int(v);
		if (const auto v = movID <=> movID_; v != 0) return libv::ordering_as_int(v);

		return 0;
	}

private:
	void ref_count_one() noexcept;

public:
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) const;

public:
	void load_mesh(const libv::vm4::Model& vm4);
	void load_node(const libv::vm4::Model& vm4, const std::vector<Material>& mats);
	void load_node(const libv::vm4::Model& vm4, const std::vector<Material>& mats, int nodeID, const libv::mat4f& parent_transformation);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
