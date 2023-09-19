// Project: libv.re, File: src/libv/re/core/render_target.cpp

#include <fmt/format.h>

#include <libv/gl/framebuffer.hpp>
#include <libv/gl/gl.hpp>
#include <libv/re/core/render_target.hpp>
#include <libv/re/log.hpp>
#include <libv/re/resource/texture.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

void increase_ref_count(RenderTarget* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(RenderTarget* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

void increase_ref_count(RenderTargetDefault* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(RenderTargetDefault* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

void increase_ref_count(RenderTargetTextures* ptr) {
	libv::ref_count_access::increase_ref_count(ptr);
}
void decrease_ref_count(RenderTargetTextures* ptr) {
	libv::ref_count_access::decrease_ref_count(ptr);
}

// -------------------------------------------------------------------------------------------------

void RenderTarget::gl_drawBuffer(libv::GL& gl, libv::gl::Attachment attachment) {
	libv::gl::Attachment attachments[] = {attachment};
	gl_drawBuffers(gl, std::span(attachments));
}

void RenderTarget::gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0) {
	libv::gl::Attachment attachments[] = {a0};
	gl_drawBuffers(gl, std::span(attachments));
}

void RenderTarget::gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0, libv::gl::Attachment a1) {
	libv::gl::Attachment attachments[] = {a0, a1};
	gl_drawBuffers(gl, std::span(attachments));
}

void RenderTarget::gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0, libv::gl::Attachment a1, libv::gl::Attachment a2) {
	libv::gl::Attachment attachments[] = {a0, a1, a2};
	gl_drawBuffers(gl, std::span(attachments));

}

void RenderTarget::gl_drawBuffers(libv::GL& gl, libv::gl::Attachment a0, libv::gl::Attachment a1, libv::gl::Attachment a2, libv::gl::Attachment a3) {
	libv::gl::Attachment attachments[] = {a0, a1, a2, a3};
	gl_drawBuffers(gl, std::span(attachments));
}

// -------------------------------------------------------------------------------------------------

void RenderTargetDefault::gl_bindDraw(libv::GL& gl, libv::vec2i newSize, int newSampleCount, bool newFixedSamples) {
	size_ = newSize;
	sampleCount_ = newSampleCount;
	sampleFixed_ = newFixedSamples;
	gl.framebuffer_default_draw();
	gl.viewport(position_, size_);
}

void RenderTargetDefault::gl_drawBuffers(libv::GL& gl, std::span<libv::gl::Attachment> attachments) {
	(void) gl;
	(void) attachments;
	throw std::logic_error("Cannot alter draw buffers of the default render target");
}

void RenderTargetDefault::gl_drawBuffersAll(libv::GL& gl) {
	(void) gl;
}

void RenderTargetDefault::gl_invalidateColorAll(GL& gl) {
	(void) gl;
	throw std::logic_error("Cannot invalidate the default render target");
}

void RenderTargetDefault::gl_resolve(libv::GL& gl, libv::gl::BufferBit bits) {
	(void) gl;
	(void) bits;
}

Texture_ptr RenderTargetDefault::textureMSColor(libv::gl::Attachment attachment) {
	(void) attachment;
	throw std::logic_error("Cannot access textures of the default render target");
}

Texture_ptr RenderTargetDefault::textureMSDepthStencil() {
	throw std::logic_error("Cannot access textures of the default render target");
}

Texture_ptr RenderTargetDefault::textureSSColor(libv::gl::Attachment attachment) {
	(void) attachment;
	throw std::logic_error("Cannot access textures of the default render target");
}

Texture_ptr RenderTargetDefault::textureSSDepthStencil() {
	throw std::logic_error("Cannot access textures of the default render target");
}

// -------------------------------------------------------------------------------------------------

void RenderTargetTextures::add(libv::gl::Attachment attachment, libv::gl::Format format, Texture2DSpec spec) {
	dirty = true;

	const auto isDepthStencil =
			attachment == libv::gl::Attachment::Depth ||
			attachment == libv::gl::Attachment::Stencil ||
			attachment == libv::gl::Attachment::DepthStencil;

	auto textureSS = Texture_ptr::make(fmt::format("fbo-{}{}-SS", isDepthStencil ? "d" : "a", isDepthStencil ? 0 : colorAttachments.size()));
	textureSS->set(spec.wrapX, spec.wrapY);
	textureSS->set(spec.minFilter);
	textureSS->set(spec.magFilter);

	auto textureMS = Texture_ptr::make(fmt::format("fbo-{}{}-MS", isDepthStencil ? "d" : "a", isDepthStencil ? 0 : colorAttachments.size()));

	if (isDepthStencil) {
		depthAttachment = attachment;
		depthTexture.format = format;
		depthTexture.textureSS = std::move(textureSS);
		depthTexture.textureMS = std::move(textureMS);
	} else {
		colorAttachments.emplace_back(attachment);
		colorTextures.emplace_back(
				format,
				std::move(textureSS),
				std::move(textureMS));
	}
}

void RenderTargetTextures::add(libv::gl::Attachment attachment, Texture_ptr textureSS, Texture_ptr textureMS) {
	dirty = true;

	const auto format = textureSS->format();
	const auto isDepthStencil =
			attachment == libv::gl::Attachment::Depth ||
			attachment == libv::gl::Attachment::Stencil ||
			attachment == libv::gl::Attachment::DepthStencil;

	if (isDepthStencil) {
		depthAttachment = attachment;
		depthTexture.format = format;
		depthTexture.textureSS = std::move(textureSS);
		depthTexture.textureMS = std::move(textureMS);
	} else {
		colorAttachments.emplace_back(attachment);
		colorTextures.emplace_back(format, std::move(textureSS), std::move(textureMS));
	}
}

void RenderTargetTextures::gl_bindDraw(libv::GL& gl, libv::vec2i newSize, int newSampleCount, bool newFixedSamples) {
	assert(newSize != libv::vec2i(0, 0));
	const auto isMS = newSampleCount > 1;
	const auto recreateFBO = dirty || size_ != newSize || sampleCount_ != newSampleCount || sampleFixed_ != newFixedSamples;

	if (recreateFBO) {
		dirty = false;
		size_ = newSize;
		sampleCount_ = newSampleCount;
		sampleFixed_ = newFixedSamples;

		// Create / Destroy (SS FBO is always kept alive, MS can be destroyed with sample changes)
		if (!isMS && fboMS.id != 0)
			gl(fboMS).destroy();
		if (fboSS.id == 0)
			gl(fboSS).dsa_create();
		if (isMS && fboMS.id == 0)
			gl(fboMS).dsa_create();

		if (depthAttachment != libv::gl::Attachment::None) {
			depthTexture.textureSS->storage(libv::gl::TextureTarget::_2D, depthTexture.format, 1, size_, 0, false);
			gl(fboSS).dsa_attach(depthAttachment, depthTexture.textureSS->gl_id(gl), 0);
			if (isMS) {
				depthTexture.textureMS->storage(libv::gl::TextureTarget::_2DMultisample, depthTexture.format, 1, size_, sampleCount_, sampleFixed_);
				gl(fboMS).dsa_attach(depthAttachment, depthTexture.textureMS->gl_id(gl), 0);
			} else if (depthTexture.textureMS && depthTexture.textureMS->gl_id(gl).id != 0) {
				depthTexture.textureMS->gl_destroy(gl);
			}
		}

		for (uint32_t i = 0; i < colorTextures.size(); ++i) {
			colorTextures[i].textureSS->storage(libv::gl::TextureTarget::_2D, colorTextures[i].format, 1, size_, 0, false);
			gl(fboSS).dsa_attach(colorAttachments[i], colorTextures[i].textureSS->gl_id(gl), 0);
			if (isMS) {
				colorTextures[i].textureMS->storage(libv::gl::TextureTarget::_2DMultisample, colorTextures[i].format, 1, size_, sampleCount_, sampleFixed_);
				gl(fboMS).dsa_attach(colorAttachments[i], colorTextures[i].textureMS->gl_id(gl), 0);
			} else if (colorTextures[i].textureMS && colorTextures[i].textureMS->gl_id(gl).id != 0)
				colorTextures[i].textureMS->gl_destroy(gl);
		}

		const auto fboStatusSS = gl(fboSS).dsa_statusDraw();
		if (fboStatusSS != libv::gl::FramebufferStatus::Complete)
			log_re.error("Failed to created SS FBO for render target: {}", libv::gl::to_string(fboStatusSS));
		if (isMS) {
			const auto fboStatusMS = gl(fboMS).dsa_statusDraw();
			if (fboStatusSS != libv::gl::FramebufferStatus::Complete)
				log_re.error("Failed to created MS FBO for render target: {}", libv::gl::to_string(fboStatusMS));
		}
	}

	auto& fbo = isMS ? fboMS : fboSS;
	if (recreateFBO || dirtyDrawAttachments) {
		dirtyDrawAttachments = false;
		if (currentDrawAttachments.empty())
			currentDrawAttachments = colorAttachments;
		gl(fbo).dsa_drawBuffers(currentDrawAttachments);
	}
	gl(fbo).bind_draw();
	gl.viewport(position_, size_);
}

void RenderTargetTextures::gl_drawBuffers(libv::GL& gl, std::span<libv::gl::Attachment> attachments) {
	const auto isMS = sampleCount_ > 1;
	auto& fbo = isMS ? fboMS : fboSS;

	currentDrawAttachments.resize(attachments.size());
	std::memcpy(currentDrawAttachments.data(), attachments.data(), attachments.size() * sizeof(libv::gl::Attachment));

	if (fbo.id != 0 && gl.framebuffer_draw().id == fbo.id)
		gl(fbo).dsa_drawBuffers(currentDrawAttachments);
	else
		dirtyDrawAttachments = true;
}

void RenderTargetTextures::gl_drawBuffersAll(libv::GL& gl) {
	const auto isMS = sampleCount_ > 1;
	auto& fbo = isMS ? fboMS : fboSS;

	currentDrawAttachments = colorAttachments;

	if (fbo.id != 0 && gl.framebuffer_draw().id == fbo.id)
		gl(fbo).dsa_drawBuffers(currentDrawAttachments);
	else
		dirtyDrawAttachments = true;
}

void RenderTargetTextures::gl_invalidateColorAll(GL& gl) {
	const auto isMS = sampleCount_ > 1;
	auto& fbo = isMS ? fboMS : fboSS;
	gl(fbo).dsa_invalidate(colorAttachments);
}

void RenderTargetTextures::gl_resolve(libv::GL& gl, libv::gl::BufferBit bits) {
	assert(!dirty && "gl_bindDraw must be called before");

	const auto isMS = sampleCount_ > 1;
	if (!isMS)
		return;

	gl.dsa_blit(fboMS,
			fboSS,
			{0, 0}, size_,
			{0, 0}, size_,
			bits,
			libv::gl::MagFilter::Nearest);
}

Texture_ptr RenderTargetTextures::textureMSColor(libv::gl::Attachment attachment) {
	for (std::size_t i = 0; i < colorAttachments.size(); ++i)
		if (colorAttachments[i] == attachment)
			return colorTextures[i].textureMS;
	return nullptr;
}

Texture_ptr RenderTargetTextures::textureMSDepthStencil() {
	return depthTexture.textureMS;
}

Texture_ptr RenderTargetTextures::textureSSColor(libv::gl::Attachment attachment) {
	for (std::size_t i = 0; i < colorAttachments.size(); ++i)
		if (colorAttachments[i] == attachment)
			return colorTextures[i].textureSS;
	return nullptr;
}

Texture_ptr RenderTargetTextures::textureSSDepthStencil() {
	return depthTexture.textureSS;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
