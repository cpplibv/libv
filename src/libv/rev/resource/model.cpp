// Project: libv.rev, File: src/libv/rev/resource/model.cpp

// hpp
#include <libv/rev/resource/model.hpp>
// pro
#include <libv/rev/resource/model_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

Model::Model(libv::intrusive2_ptr<InternalModel> internal) :
		self(std::move(internal)) {
}

Model::Model(const libv::vm4::Model& model, const Material& materialOverride) :
		self(libv::make_intrusive2_ptr<InternalModel>(model, materialOverride)) {
}

Model::Model(const libv::vm4::Model& model, const MaterialLoaderFunc& materialLoaderFunc) :
		self(libv::make_intrusive2_ptr<InternalModel>(model, materialLoaderFunc)) {
}

Model::Model(const Model&) noexcept = default;
Model& Model::operator=(const Model&)& noexcept = default;
Model::Model(Model&&) noexcept = default;
Model& Model::operator=(Model&&)& noexcept = default;

Model::~Model() {
	// For the sake of forward declared types
}

void Model::render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) const {
	self->render(glr, uniform_stream);
}

//bool Model::isLoaded() const noexcept {
//	return ?;
//}

libv::vec3f Model::AABB_max() const noexcept {
//	assert(isLoaded());

	return self->AABB_max;
}

libv::vec3f Model::AABB_min() const noexcept {
//	assert(isLoaded());

	return self->AABB_min;
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
