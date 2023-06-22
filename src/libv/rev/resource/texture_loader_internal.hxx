// Project: libv, File: app/space/view/render/model.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/glr/texture.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
// std
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
// pro
#include <libv/rev/gl/swizzle.hpp>
#include <libv/rev/resource/basic_resource.hpp>
#include <libv/rev/resource/texture_internal.hxx>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

using InternalTexture_ptr = libv::intrusive2_ptr<InternalTexture>;

// -------------------------------------------------------------------------------------------------

struct InternalTexturePtrNameComp {
	using is_transparent = void;

	[[nodiscard]] static inline bool operator()(const InternalTexture_ptr& ptr, const std::string_view name) noexcept {
		return ptr->name_ < name;
	}
	[[nodiscard]] static inline bool operator()(const std::string_view name, const InternalTexture_ptr& ptr) noexcept {
		return name < ptr->name_;
	}
	[[nodiscard]] static inline bool operator()(const InternalTexture_ptr& lhs, const InternalTexture_ptr& rhs) noexcept {
		return lhs->name_ < rhs->name_;
	}
};

// -------------------------------------------------------------------------------------------------

class InternalTextureLoader : public std::enable_shared_from_this<InternalTextureLoader> {
public:
	std::mutex mutex;
	BasicResourceLoaderCache<InternalTexture> cache;

	std::shared_ptr<InternalResourceManager> irm;
	std::mutex includeDirectories_m;
	std::vector<std::string> includeDirectories;

	libv::glr::Texture defaultGLR;
	InternalTexture_ptr fallback_;
	boost::container::flat_set<InternalTexture_ptr, InternalTexturePtrNameComp> builtins;

public:
	explicit InternalTextureLoader(const std::shared_ptr<InternalResourceManager>& irm);
	~InternalTextureLoader();

private:
	void initDefaultGLR();
	libv::glr::Texture createColorTextureGLR(libv::vec4uc color);
	void initBuiltin(std::string name, libv::vec4uc color);

public:
	InternalTexture_ptr load(std::string_view name, Swizzle swizzle);
	InternalTexture_ptr fallback() const;
	void unload(InternalTexture* resource);

private:
	InternalTexture_ptr create_resource(std::string_view name, Swizzle swizzle);
	void process_fs(InternalTexture_ptr&& res);
	void process_cpu(InternalTexture_ptr&& res, std::string&& data);
	void process_res(InternalTexture_ptr&& resource, libv::gl::Image&& image);
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
