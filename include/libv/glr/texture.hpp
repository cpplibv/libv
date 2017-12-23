// File: program.hpp - Created on 2017.12.20. 11:27 - Author: Vader

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>
// std
#include <memory>
#include <optional>
// pro
#include <libv/gl/enum.hpp>
#include <libv/gl/image.hpp>
#include <libv/gl/texture_object.hpp>


namespace libv {
namespace gl {

class GL;

} // namespace gl
namespace glr {

// -------------------------------------------------------------------------------------------------

class Remote;
class AttorneyTextureRemote;

struct RemoteTexture {
	libv::gl::Texture texture;

	bool created = false;
	bool dirty = false;
	bool dirty_image = false;

	//uint32_t ref_count = 0;

public:
	std::optional<libv::gl::Image> image;

private:
	/// Can be nullptr if the object is not yet associated with a remote OpenGL context
	libv::observer_ptr<Remote> remote;

private:
	void create(libv::gl::GL& gl, Remote& remote_) noexcept;
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	void bind(libv::gl::GL& gl, Remote& remote_) noexcept;
	~RemoteTexture() noexcept;
};

template <libv::gl::TextureTarget Target>
struct Texture_t {
	friend class AttorneyTextureRemote;
	friend class Texture;

private:
	std::shared_ptr<RemoteTexture> remote;

public:
	Texture_t() :
		remote(std::make_shared<RemoteTexture>()) { }

public:
	inline void image(libv::gl::Image image) {
		remote->image = std::move(image);
		remote->dirty = true;
		remote->dirty_image = true;
	}
};

struct Texture {
	friend class AttorneyTextureRemote;

private:
	std::shared_ptr<RemoteTexture> remote;

public:
	Texture() :
		remote(std::make_shared<RemoteTexture>()) { }

	template <libv::gl::TextureTarget Target>
	Texture(const Texture_t<Target>& orig) :
		remote(orig.remote) { }

public:
	inline void image(libv::gl::Image image) {
		remote->image = std::move(image);
		remote->dirty = true;
		remote->dirty_image = true;
	}
};

using Texture1D = Texture_t<libv::gl::TextureTarget::_1D>;
using Texture1DArray = Texture_t<libv::gl::TextureTarget::_1DArray>;
using Texture2D = Texture_t<libv::gl::TextureTarget::_2D>;
using Texture2DArray = Texture_t<libv::gl::TextureTarget::_2DArray>;
using Texture3D = Texture_t<libv::gl::TextureTarget::_3D>;
using TextureCube = Texture_t<libv::gl::TextureTarget::CubeMap>;
using TextureCubeArray = Texture_t<libv::gl::TextureTarget::CubeMapArray>;
using TextureRectangle = Texture_t<libv::gl::TextureTarget::Rectangle>;


class AttorneyTextureRemote {
	friend class Queue;
	inline static RemoteTexture& remote(const Texture& texture) {
		return *texture.remote;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
