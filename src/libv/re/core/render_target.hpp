// Project: libv.re, File: src/libv/re/core/render_target.hpp

#pragma once

#include <libv/container/small_vector.hpp>
#include <libv/gl/enum.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/core/safe_gl.hpp>
#include <libv/re/fwd.hpp>

#include <span>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

// TODO P5: Think about / find a better place for Texture2DSpec
struct Texture2DSpec {
	libv::gl::Wrap wrapX = libv::gl::Wrap::Repeat;
	libv::gl::Wrap wrapY = libv::gl::Wrap::Repeat;
	libv::gl::MagFilter magFilter = libv::gl::MagFilter::Linear;
	libv::gl::MinFilter minFilter = libv::gl::MinFilter::LinearMipmapLinear;

	[[nodiscard]] bool operator==(const Texture2DSpec& rhs) const = default;
};

// -------------------------------------------------------------------------------------------------

class RenderTarget : public libv::ref_count_base {
protected:
	libv::vec2i position_;
	libv::vec2i size_;
	int sampleCount_ = 0;
	bool sampleFixed_ = false;

public:
	using ptr = RenderTarget_ptr;
	using cptr = RenderTarget_cptr;

	virtual ~RenderTarget() = default;

public:
	[[nodiscard]] constexpr inline libv::vec2i size() const noexcept {
		return size_;
	}
	[[nodiscard]] constexpr inline libv::vec2i position() const noexcept {
		return position_;
	}
	constexpr inline void position(libv::vec2i position) noexcept {
		position_ = position;
	}
	[[nodiscard]] constexpr inline int sampleCount() const noexcept {
		return sampleCount_;
	}
	constexpr inline void sampleCount(int sampleCount) noexcept {
		sampleCount_ = sampleCount;
	}
	[[nodiscard]] constexpr inline bool sampleFixed() const noexcept {
		return sampleFixed_;
	}
	constexpr inline void sampleFixed(bool sampleFixed) noexcept {
		sampleFixed_ = sampleFixed;
	}

	void gl_drawBuffer(libv::GL& gl, libv::gl::Attachment attachment);
	void gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0);
	void gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0, libv::gl::Attachment a1);
	void gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0, libv::gl::Attachment a1, libv::gl::Attachment a2);
	void gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0, libv::gl::Attachment a1, libv::gl::Attachment a2, libv::gl::Attachment a3);

public:
	virtual void gl_bindDraw(libv::GL& gl, libv::vec2i size, int sampleCount, bool sampleFixed) = 0;
	virtual void gl_drawBuffers(libv::GL& gl, std::span<libv::gl::Attachment> attachments) = 0;
	virtual void gl_drawBuffersAll(libv::GL& gl) = 0;
	virtual void gl_invalidateColorAll(libv::GL& gl) = 0;
	virtual void gl_resolve(libv::GL& gl, libv::gl::BufferBit bits) = 0;
	/// \param attachment
	/// \return MS texture pointer and null if the requested attachment is not part of the render target
	[[nodiscard]] virtual Texture_ptr textureMSColor(libv::gl::Attachment attachment) = 0;
	/// \return MS texture pointer and null if the requested attachment is not part of the render target
	[[nodiscard]] virtual Texture_ptr textureMSDepthStencil() = 0;
	/// Render target must be resolved with the corresponding buffers before calling this function.
	/// \param attachment
	/// \return SS texture pointer and null if the requested attachment is not part of the render target
	[[nodiscard]] virtual Texture_ptr textureSSColor(libv::gl::Attachment attachment) = 0;
	/// Render target must be resolved with the corresponding buffers before calling this function.
	/// \return SS texture pointer and null if the requested attachment is not part of the render target
	[[nodiscard]] virtual Texture_ptr textureSSDepthStencil() = 0;
};

class RenderTargetDefault : public RenderTarget {
public:
	virtual void gl_bindDraw(libv::GL& gl, libv::vec2i size, int sampleCount, bool sampleFixed) override;
	virtual void gl_drawBuffers(libv::GL& gl, std::span<libv::gl::Attachment> attachments) override;
	virtual void gl_drawBuffersAll(libv::GL& gl) override;
	virtual void gl_invalidateColorAll(libv::GL& gl) override;
	virtual void gl_resolve(libv::GL& gl, libv::gl::BufferBit bits) override;
	[[nodiscard]] virtual Texture_ptr textureMSColor(libv::gl::Attachment attachment) override;
	[[nodiscard]] virtual Texture_ptr textureMSDepthStencil() override;
	[[nodiscard]] virtual Texture_ptr textureSSColor(libv::gl::Attachment attachment) override;
	[[nodiscard]] virtual Texture_ptr textureSSDepthStencil() override;
};

class RenderTargetTextures : public RenderTarget {
private:
	bool dirty = true;
	bool dirtyDrawAttachments = true;

	libv::re::GLSafeFramebuffer fboSS;
	libv::re::GLSafeFramebuffer fboMS;

	struct Entry {
		libv::gl::Format format;
		Texture_ptr textureSS = nullptr;
		Texture_ptr textureMS = nullptr;
	};

	libv::gl::Attachment depthAttachment = libv::gl::Attachment::None;
	Entry depthTexture;
	libv::small_vector<libv::gl::Attachment, 3> colorAttachments;
	libv::small_vector<libv::gl::Attachment, 3> currentDrawAttachments;
	libv::small_vector<Entry, 3> colorTextures;

public:
	void add(libv::gl::Attachment attachment, libv::gl::Format format, Texture2DSpec spec);
	void add(libv::gl::Attachment attachment, Texture_ptr textureSS, Texture_ptr textureMS = nullptr);

public:
	virtual void gl_bindDraw(libv::GL& gl, libv::vec2i size, int sampleCount, bool sampleFixed) override;
	virtual void gl_drawBuffers(libv::GL& gl, std::span<libv::gl::Attachment> attachments) override;
	virtual void gl_drawBuffersAll(libv::GL& gl) override;
	virtual void gl_invalidateColorAll(libv::GL& gl) override;
	virtual void gl_resolve(libv::GL& gl, libv::gl::BufferBit bits) override;
	[[nodiscard]] virtual Texture_ptr textureMSColor(libv::gl::Attachment attachment) override;
	[[nodiscard]] virtual Texture_ptr textureMSDepthStencil() override;
	[[nodiscard]] virtual Texture_ptr textureSSColor(libv::gl::Attachment attachment) override;
	[[nodiscard]] virtual Texture_ptr textureSSDepthStencil() override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
