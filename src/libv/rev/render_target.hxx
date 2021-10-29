// Project: libv.rev, File: src/libv/rev/shader.hpp

#pragma once

// hpp
#include <libv/rev/render_target.hpp>
// libv
#include <libv/glr/framebuffer.hpp>
#include <libv/glr/fwd.hpp>
#include <libv/glr/renderbuffer.hpp>
#include <libv/glr/texture.hpp>
#include <libv/math/vec.hpp>
// std
//#include <memory>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------
// TODO P4: If RenderTarget is not MS, use different type, or use a flag, but do not create MS stuff in it

class ImplRenderTarget {
public:
	libv::glr::Framebuffer ms_framebuffer;
	libv::glr::Texture2DMultisample::R11F_G11F_B10F ms_color0;
	libv::glr::Renderbuffer::D32 ms_depth;

	libv::glr::Framebuffer ss_framebuffer;
	libv::glr::Texture2D::R11F_G11F_B10F ss_color0;

	libv::vec2i framebufferSize;
	int sampleCount_;

public:
	void createMSFramebuffer();
	void createSSFramebuffer();
	void destroyMSFramebuffer();
	void destroySSFramebuffer();
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
