// Project: libv.glr, File: src/libv/glr/framebuffer.cpp

// hpp
#include <libv/glr/framebuffer.hpp>
// ext
#include <boost/container/small_vector.hpp>
// libv
#include <libv/gl/framebuffer.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
#include <libv/meta/similar.hpp>
// std
#include <span>
#include <variant>
// pro
#include <libv/glr/destroy_queue.hpp>
#include <libv/glr/remote.hpp>
#include <libv/glr/renderbuffer.hpp>
#include <libv/glr/texture.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct RemoteFramebuffer {
	struct AttachmentR {
		libv::gl::Attachment attachment;
		Renderbuffer renderbuffer;
	};

	struct Attachment1D {
		libv::gl::Attachment attachment;
		Texture texture;
		int32_t level;
	};

	struct Attachment2D {
		libv::gl::Attachment attachment;
		Texture texture;
		int32_t level;
	};

	struct Attachment2DLayer {
		libv::gl::Attachment attachment;
		Texture texture;
		int32_t level;
		int32_t layer;
	};

	struct Attachment2DCube {
		libv::gl::Attachment attachment;
		Texture texture;
		libv::gl::CubeSide side;
		int32_t level;
	};

	struct Attachment3D {
		libv::gl::Attachment attachment;
		Texture texture;
		int32_t level;
		int32_t layer;
	};

	using Attachment = std::variant<AttachmentR, Attachment1D, Attachment2D, Attachment2DLayer, Attachment2DCube, Attachment3D>;

public:
	libv::gl::Framebuffer object;
	bool dirty = true;

	std::size_t drawAttachmentCount = 0;
	libv::gl::Attachment drawAttachments[8];

	boost::container::small_vector<Attachment, 2> pendingAttachments; /// Pending attachments

private:
	libv::observer_ptr<DestroyQueues> remote = nullptr;

private:
	template <bool Both, bool Read, bool Draw>
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	void bind(libv::gl::GL& gl, Remote& remote_) noexcept;
	void bind_draw(libv::gl::GL& gl, Remote& remote_) noexcept;
	void bind_read(libv::gl::GL& gl, Remote& remote_) noexcept;

	void use_draw_buffer(libv::gl::GL& gl, libv::gl::Attachment attachment) noexcept;
	void use_read_buffer(libv::gl::GL& gl, libv::gl::Attachment attachment) noexcept;
	void reset_draw_buffer(libv::gl::GL& gl) noexcept;
	void reset_read_buffer(libv::gl::GL& gl) noexcept;

public:
	~RemoteFramebuffer() noexcept;
};

// -------------------------------------------------------------------------------------------------

template <bool Both, bool Draw, bool Read>
void RemoteFramebuffer::update(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (remote == nullptr) {
		gl(object).create();
		remote = make_observer_ptr(&remote_.destroyQueues());
	}

	if constexpr (Both)
		gl(object).bind();
	if constexpr (Draw)
		gl(object).bind_draw();
	if constexpr (Read)
		gl(object).bind_read();

	const auto visitor = [&](auto& a) {
		if constexpr (libv::similar_v<decltype(a), AttachmentR>) {
			if constexpr (Both || Draw)
				gl(object).attach_draw(a.attachment, AttorneyRemoteRenderbuffer::sync(a.renderbuffer, gl, remote_));
			else
				gl(object).attach_read(a.attachment, AttorneyRemoteRenderbuffer::sync(a.renderbuffer, gl, remote_));
		}

		if constexpr (libv::similar_v<decltype(a), Attachment1D>) {
			if constexpr (Both || Draw)
				gl(object).attach_draw1D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level);
			else
				gl(object).attach_read1D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level);
		}

		if constexpr (libv::similar_v<decltype(a), Attachment2D>) {
			if constexpr (Both || Draw)
				gl(object).attach_draw2D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level);
			else
				gl(object).attach_read2D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level);
		}

		if constexpr (libv::similar_v<decltype(a), Attachment2DLayer>) {
			if constexpr (Both || Draw)
				gl(object).attach_draw2D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level, a.layer);
			else
				gl(object).attach_read2D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level, a.layer);
		}

		if constexpr (libv::similar_v<decltype(a), Attachment2DCube>) {
			if constexpr (Both || Draw)
				gl(object).attach_draw2D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.side, a.level);
			else
				gl(object).attach_read2D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.side, a.level);
		}

		if constexpr (libv::similar_v<decltype(a), Attachment3D>) {
			if constexpr (Both || Draw)
				gl(object).attach_draw3D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level, a.layer);
			else
				gl(object).attach_read3D(a.attachment, AttorneyRemoteTexture::sync_no_bind(a.texture, gl, remote_), a.level, a.layer);
		}

		return a.attachment;
	};

	for (Attachment& item : pendingAttachments) {
		const auto attachmentCode = std::visit(visitor, item);
		if (attachmentCode != libv::gl::Attachment::Depth && attachmentCode != libv::gl::Attachment::DepthStencil) {
			assert(drawAttachmentCount < 8);
			drawAttachments[drawAttachmentCount++] = attachmentCode;
		}
	}
	pendingAttachments.clear();

	if constexpr (Both || Draw)
		gl(object).drawBuffers(std::span<libv::gl::Attachment>(drawAttachments, drawAttachmentCount));

	dirty = false;
}

void RemoteFramebuffer::bind(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (dirty)
		update<true, false, false>(gl, remote_);
	else
		gl(object).bind();
}

void RemoteFramebuffer::bind_draw(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (dirty)
		update<false, true, false>(gl, remote_);
	else
		gl(object).bind_draw();
}

void RemoteFramebuffer::bind_read(libv::gl::GL& gl, Remote& remote_) noexcept {
	if (dirty)
		update<false, false, true>(gl, remote_);
	else
		gl(object).bind_read();
}

void RemoteFramebuffer::use_draw_buffer(libv::gl::GL& gl, libv::gl::Attachment attachment) noexcept {
	gl(object).drawBuffer(attachment);
}

void RemoteFramebuffer::use_read_buffer(libv::gl::GL& gl, libv::gl::Attachment attachment) noexcept {
	gl(object).readBuffer(attachment);
}

void RemoteFramebuffer::reset_draw_buffer(libv::gl::GL& gl) noexcept {
	gl(object).drawBuffers(std::span<libv::gl::Attachment>(drawAttachments, drawAttachmentCount));
}

void RemoteFramebuffer::reset_read_buffer(libv::gl::GL& gl) noexcept {
	gl(object).readBuffer(libv::gl::Attachment::Color0);
}

RemoteFramebuffer::~RemoteFramebuffer() noexcept {
	if (remote)
		remote->gc(object);
}

// -------------------------------------------------------------------------------------------------

Framebuffer::Framebuffer() noexcept :
	remote(std::make_shared<RemoteFramebuffer>()) { }

void Framebuffer::attach(libv::gl::Attachment attachment, Renderbuffer renderbuffer) noexcept {
	remote->pendingAttachments.emplace_back(std::in_place_type<RemoteFramebuffer::AttachmentR>, attachment, std::move(renderbuffer));
	remote->dirty = true;
}

void Framebuffer::attach1D(libv::gl::Attachment attachment, Texture texture, int32_t level) noexcept {
	remote->pendingAttachments.emplace_back(std::in_place_type<RemoteFramebuffer::Attachment1D>, attachment, std::move(texture), level);
	remote->dirty = true;
}

void Framebuffer::attach2D(libv::gl::Attachment attachment, Texture texture, int32_t level) noexcept {
	remote->pendingAttachments.emplace_back(std::in_place_type<RemoteFramebuffer::Attachment2D>, attachment, std::move(texture), level);
	remote->dirty = true;
}

void Framebuffer::attach2D(libv::gl::Attachment attachment, Texture texture, libv::gl::CubeSide side, int32_t level) noexcept {
	remote->pendingAttachments.emplace_back(std::in_place_type<RemoteFramebuffer::Attachment2DCube>, attachment, std::move(texture), side, level);
	remote->dirty = true;
}

void Framebuffer::attach2D(libv::gl::Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
	remote->pendingAttachments.emplace_back(std::in_place_type<RemoteFramebuffer::Attachment2DLayer>, attachment, std::move(texture), level, layer);
	remote->dirty = true;
}

void Framebuffer::attach3D(libv::gl::Attachment attachment, Texture texture, int32_t level, int32_t layer) noexcept {
	remote->pendingAttachments.emplace_back(std::in_place_type<RemoteFramebuffer::Attachment3D>, attachment, std::move(texture), level, layer);
	remote->dirty = true;
}

// -------------------------------------------------------------------------------------------------

libv::gl::Framebuffer& Framebuffer::out_of_order_gl() {
	return remote->object;
}

// -------------------------------------------------------------------------------------------------

void AttorneyRemoteFramebuffer::bind(const Framebuffer& framebuffer, libv::gl::GL& gl, Remote& remote_) noexcept {
	framebuffer.remote->bind(gl, remote_);
}

void AttorneyRemoteFramebuffer::bind_draw(const Framebuffer& framebuffer, libv::gl::GL& gl, Remote& remote_) noexcept {
	framebuffer.remote->bind_draw(gl, remote_);
}

void AttorneyRemoteFramebuffer::bind_read(const Framebuffer& framebuffer, libv::gl::GL& gl, Remote& remote_) noexcept {
	framebuffer.remote->bind_read(gl, remote_);
}

void AttorneyRemoteFramebuffer::use_draw_buffer(const Framebuffer& framebuffer, libv::gl::GL& gl, libv::gl::Attachment attachment) noexcept {
	framebuffer.remote->use_draw_buffer(gl, attachment);
}

void AttorneyRemoteFramebuffer::use_read_buffer(const Framebuffer& framebuffer, libv::gl::GL& gl, libv::gl::Attachment attachment) noexcept {
	framebuffer.remote->use_read_buffer(gl, attachment);
}

void AttorneyRemoteFramebuffer::reset_draw_buffer(const Framebuffer& framebuffer, libv::gl::GL& gl) noexcept {
	framebuffer.remote->reset_draw_buffer(gl);
}

void AttorneyRemoteFramebuffer::reset_read_buffer(const Framebuffer& framebuffer, libv::gl::GL& gl) noexcept {
	framebuffer.remote->reset_read_buffer(gl);
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
