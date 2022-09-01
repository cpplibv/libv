// Project: libv.rev, File: src/libv/rev/render_target.cpp

// hpp
#include <libv/rev/render_target.hpp>
#include <libv/rev/render_target.hxx>
// libv
#include <libv/glr/queue.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

RenderTarget::RenderTarget(libv::vec2i size_, int sampleCount_) :
		self(std::make_unique<ImplRenderTarget>()) {

	// TODO P4: If RenderTarget is not MS, use different type, or use a flag, but do not create MS stuff in it

	self->framebufferSize = size_;
	self->sampleCount_ = sampleCount_;
	self->createMSFramebuffer();
	self->createSSFramebuffer();
}

RenderTarget::RenderTarget(RenderTarget&&) noexcept = default;

RenderTarget& RenderTarget::operator=(RenderTarget&&) & noexcept = default;

RenderTarget::~RenderTarget() {
	// For the sake of forward declared ptr
}

void ImplRenderTarget::createMSFramebuffer() {
	if (sampleCount_ > 1) {
		ms_color0.storage_ms(framebufferSize, sampleCount_, true);
		ms_depth.storage_ms(framebufferSize, sampleCount_);

		ms_framebuffer.attach2D(libv::gl::Attachment::Color0, ms_color0, 0);
		ms_framebuffer.attach(libv::gl::Attachment::Depth, ms_depth);
	}
}

void ImplRenderTarget::createSSFramebuffer() {
	ss_color0.storage(1, framebufferSize);
	// NOTE: ClampToBorder is mostly used because of blur effect in bloom post-processing
	//			In the future this might not be suitable for all application of the RenderTarget class
	ss_color0.set(libv::gl::Wrap::ClampToBorder, libv::gl::Wrap::ClampToBorder);
//	ss_color0.set(libv::gl::Wrap::ClampToEdge, libv::gl::Wrap::ClampToEdge);
	ss_color0.set(libv::gl::MagFilter::Linear);
	ss_color0.set(libv::gl::MinFilter::Linear);

	ss_framebuffer.attach2D(libv::gl::Attachment::Color0, ss_color0);
}

void ImplRenderTarget::destroyMSFramebuffer() {
	ms_framebuffer = libv::glr::Framebuffer{};

	ms_color0 = MSTexture{};
	ms_depth = libv::glr::Renderbuffer::D32{};
}

void ImplRenderTarget::destroySSFramebuffer() {
	ss_framebuffer = libv::glr::Framebuffer{};

	ss_color0 = SSTexture{};
}

void RenderTarget::sampleCount(int sampleCount_) noexcept {
	self->sampleCount_ = sampleCount_;

	self->destroyMSFramebuffer();
	self->createMSFramebuffer();
}
int RenderTarget::sampleCount() const noexcept {
	return self->sampleCount_;
}

void RenderTarget::size(libv::vec2i framebufferSize) noexcept {
	if (self->framebufferSize == framebufferSize)
		return;

	self->framebufferSize = framebufferSize;

	self->destroyMSFramebuffer();
	self->destroySSFramebuffer();
	self->createMSFramebuffer();
	self->createSSFramebuffer();
}

libv::vec2i RenderTarget::size() const noexcept {
	return self->framebufferSize;
}

const libv::glr::Texture2D::R11F_G11F_B10F& RenderTarget::resolve(libv::glr::Queue& glr) {
	if (self->sampleCount_ > 1) {
		glr.blit(self->ms_framebuffer, self->ss_framebuffer,
				{}, self->framebufferSize,
				{}, self->framebufferSize,
				libv::gl::BufferBit::Color, libv::gl::MagFilter::Nearest);
	}

	return self->ss_color0;
}

const libv::glr::Framebuffer& RenderTarget::framebuffer() const noexcept {
	return self->sampleCount_ > 1 ? self->ms_framebuffer : self->ss_framebuffer;
}

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
