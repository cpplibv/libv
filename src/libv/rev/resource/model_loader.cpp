// Project: libv.rev, File: src/libv/rev/resource/model_loader.cpp

// hpp
#include <libv/rev/resource/model_loader.hpp>
// pro
#include <libv/rev/resource/material.hpp>
#include <libv/rev/resource/model.hpp>
#include <libv/rev/resource/model_loader_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

ModelLoader::ModelLoader(const std::shared_ptr<InternalResourceManager>& irm, const MaterialLoader& materialLoader) :
	self(std::make_shared<InternalModelLoader>(irm, materialLoader)) {
}

Model ModelLoader::load(std::string_view name) {
	return Model{self->load(name)};
}

Model ModelLoader::load(std::string_view name, Material mov, int64_t movID) {
	return Model{self->load(name, std::move(mov), movID)};
}

Model ModelLoader::load(std::string_view name, MaterialScanFunc scanFn, int64_t movID) {
	return Model{self->load(name, std::move(scanFn), movID)};
}

Model ModelLoader::load(std::string_view name, MaterialLoaderFunc loadFn, int64_t movID) {
	return Model{self->load(name, std::move(loadFn), movID)};
}

Model ModelLoader::fallback() const {
	return Model{self->fallback()};
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
