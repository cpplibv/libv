// Project: libv, File: app/space/view/render/model.hpp

// hpp
#include <libv/rev/resource/resource_manager.hpp>
#include <libv/rev/resource/resource_manager_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

ResourceManager::ResourceManager(Settings settings) :
	self(libv::make_intrusive2_ptr<InternalResourceManager>(std::move(settings))),
	texture(self) {
}

ResourceManager::~ResourceManager() {
	// For the sake of forward declared ptr
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
