// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// ext
//#include <boost/container/flat_set.hpp>
// libv
#include <libv/glr/mesh.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
// std
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/rev/resource/basic_resource.hpp>
#include <libv/rev/resource/material_loader.hpp>
#include <libv/rev/resource/material_override.hpp>
#include <libv/rev/resource/model_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

using InternalModel_ptr = libv::intrusive2_ptr<InternalModel>;

// -------------------------------------------------------------------------------------------------

struct InternalModelLoader : public std::enable_shared_from_this<InternalModelLoader> {
	std::mutex mutex;
	BasicResourceLoaderCache<InternalModel> cache;

	std::shared_ptr<InternalResourceManager> irm;
	std::mutex includeDirectories_m;
	std::vector<std::string> includeDirectories;

	MaterialLoader materialLoader;

	libv::glr::Mesh defaultGLRMesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};
	InternalModelNode defaultNode;
	InternalModel_ptr fallback_;

//	boost::container::flat_set<libv::intrusive2_ptr<InternalModel>, InternalModelPtrNameComp> builtins;
//	boost::container::flat_set<Model, InternalModelPtrNameComp> builtins;

public:
	explicit InternalModelLoader(const std::shared_ptr<InternalResourceManager>& irm, const MaterialLoader& materialLoader);
	~InternalModelLoader();

private:
	void initDefaultGLR();

public:
	InternalModel_ptr load(std::string_view name);
	InternalModel_ptr load(std::string_view name, Material mov, int64_t movID);
	InternalModel_ptr load(std::string_view name, MaterialScanFunc scanFn, int64_t movID);
	InternalModel_ptr load(std::string_view name, MaterialLoaderFunc loadFn, int64_t movID);
	InternalModel_ptr fallback() const;
	void unload(InternalModel* resource);

private:
	InternalModel_ptr create_resource(std::string_view name, MaterialLoaderFunc&& mov, int64_t movID);
	void process_fs(InternalModel_ptr&& resource, MaterialLoaderFunc&& mov);
	void process_cpu(InternalModel_ptr&& resource, MaterialLoaderFunc&& mov, std::string&& data);
	void process_res(InternalModel_ptr&& resource, libv::vm4::Model&& model, std::vector<Material>&& materials);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
