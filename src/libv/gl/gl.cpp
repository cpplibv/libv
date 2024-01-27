// Project: libv.gl, File: src/libv/gl/gl.cpp

// hpp
#include <libv/gl/gl.hpp>
#include <libv/gl/check.hpp>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

void logGLError(libv::source_location loc, GLenum err) noexcept {
	log_gl.error({"OpenGL: {}: {}", loc}, err, reinterpret_cast<const char*>(glewGetErrorString(err)));
}

// -------------------------------------------------------------------------------------------------

void GL::init() {
	glewExperimental = true;

	if (GLenum err = glewInit() != GLEW_OK) {
		log_gl.error("Failed to initialize glew: {} (Does the current thread has an OpenGL context?)", reinterpret_cast<const char*>(glewGetErrorString(err)));
		return;
	}

	versionMajor = get<GLint>(GL_MAJOR_VERSION);
	versionMinor = get<GLint>(GL_MINOR_VERSION);

	// Log capabilities
	log_gl.debug("GL Vendor     {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	log_gl.debug("GL Renderer   {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	log_gl.debug("GL Version    {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	log_gl.debug("GLSL Version  {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

	if (const auto curr_mem = getCurrentAvailableVideoMemory(); curr_mem >= 0)
		log_gl.debug("CurrentAvailableVideoMemory        [{:>8} ]", curr_mem);
	else
		log_gl.debug("CurrentAvailableVideoMemory        [{:>8} ]", "unknown");

	log_gl.debug("MaxColorAttachments                [{:>8} ]", getMaxColorAttachments());
	log_gl.debug("MaxSamples                         [{:>8} ]", getMaxSamples());
	log_gl.debug("MaxSamplesInteger                  [{:>8} ]", getMaxSamplesInteger());
	log_gl.debug("MaxTextureImageUnits               [{:>8} ]", getMaxTextureImageUnits());
	log_gl.debug("MaxTextureImageUnitsCompute        [{:>8} ]", getMaxTextureImageUnitsCompute());
	log_gl.debug("MaxTextureImageUnitsFragment       [{:>8} ]", getMaxTextureImageUnitsFragment());
	log_gl.debug("MaxTextureImageUnitsGeometry       [{:>8} ]", getMaxTextureImageUnitsGeometry());
	log_gl.debug("MaxTextureImageUnitsTessControl    [{:>8} ]", getMaxTextureImageUnitsTessControl());
	log_gl.debug("MaxTextureImageUnitsTessEvaluation [{:>8} ]", getMaxTextureImageUnitsTessEvaluation());
	log_gl.debug("MaxTextureImageUnitsVertex         [{:>8} ]", getMaxTextureImageUnitsVertex());
	log_gl.debug("MaxTextureSize                     [{:>8} ]", getMaxTextureSize());
	log_gl.debug("MaxUniformBlockSize                [{:>8} ]", getMaxUniformBlockSize());
	log_gl.debug("MaxUniformBufferBindings           [{:>8} ]", getMaxUniformBufferBindings());
	log_gl.debug("MaxVertexUniformBlocks             [{:>8} ]", getMaxVertexUniformBlocks());
	log_gl.debug("MaxGeometryUniformBlocks           [{:>8} ]", getMaxGeometryUniformBlocks());
	log_gl.debug("MaxFragmentUniformBlocks           [{:>8} ]", getMaxFragmentUniformBlocks());
	log_gl.debug("MaxComputeUniformBlocks            [{:>8} ]", getMaxComputeUniformBlocks());
	log_gl.debug("MaxCombinedUniformBlocks           [{:>8} ]", getMaxCombinedUniformBlocks());
	log_gl.debug("MaxVertexAttribs                   [{:>8} ]", getMaxVertexAttribs());
	log_gl.debug("MaxVertexUniformComponents         [{:>8} ]", getMaxVertexUniformComponents());
	log_gl.debug("UniformBufferOffsetAlignment       [{:>8} ]", getUniformBufferOffsetAlignment());
	log_gl.debug("EXT_texture_filter_anisotropic     [{:>8} ]", GLEW_EXT_texture_filter_anisotropic ? "PASS" : "MISSING");
	log_gl.debug("ARB_bindless_texture               [{:>8} ]", GLEW_ARB_bindless_texture ? "PASS" : "MISSING");
	log_gl.debug("ARB_shader_draw_parameters         [{:>8} ]", GLEW_ARB_shader_draw_parameters ? "PASS" : "MISSING");

	log_gl.debug("MaxComputeWorkGroupCount           [{}]", getMaxComputeWorkGroupCount());
	log_gl.debug("MaxComputeWorkGroupSize            [{}]", getMaxComputeWorkGroupSize());
	log_gl.debug("MaxComputeWorkGroupInvocations     [{:>8} ]", getMaxComputeWorkGroupInvocations());

	// Disable row alignment for texture and pixel operations
	// Otherwise a default 4 byte alignment is required for pixels row starts (which is broken on R8G8B8 formats if the width is not multiple of 4)
	// For more information:
	//      https://www.khronos.org/opengl/wiki/Common_Mistakes#Texture_upload_and_pixel_reads
	//      https://www.khronos.org/opengl/wiki/Pixel_Transfer#Pixel_layout
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	checkGL();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	checkGL();

	if (versionOrGreater(4, 0)) {
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		checkGL();
	}

	// glDisable(GL_FRAMEBUFFER_SRGB);
	// glEnable(GL_FRAMEBUFFER_SRGB);

	// Fetch OpenGL context current state
	capability.blend.init();
	capability.cull.init();
	capability.depthTest.init();
	capability.multisample.init();
	capability.rasterizerDiscard.init();
	capability.scissorTest.init();
	capability.stencilTest.init();
	capability.textureCubeMapSeamless.init();

	capability.textureCubeMapSeamless.enable();
}

void GL::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userData) {

	(void) length;
	auto& self = *static_cast<const GL*>(userData);

	const auto source_str = [&] {
		switch (source) {
		case GL_DEBUG_SOURCE_API:             return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
		case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
		case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
		case GL_DEBUG_SOURCE_OTHER:           return "Other";
		default:                              return "?";
		}
	}();

	const auto type_str = [&] {
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:               return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
		case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
		case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
		case GL_DEBUG_TYPE_MARKER:              return "Marker";
		case GL_DEBUG_TYPE_PUSH_GROUP:          return "Push Group";
		case GL_DEBUG_TYPE_POP_GROUP:           return "Pop Group";
		case GL_DEBUG_TYPE_OTHER:               return "Other";
		default:                                return "?";
		}
	}();

	const auto severity_str = [&] {
		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         return "high";
		case GL_DEBUG_SEVERITY_MEDIUM:       return "medium";
		case GL_DEBUG_SEVERITY_LOW:          return "low";
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "notification";
		default:                             return "?";
		}
	}();

	if (type == GL_DEBUG_TYPE_ERROR && self.debug)
		log_gl.error("OpenGL: src {} ({}), type {} ({:X}), id {}, severity {} ({:X}), message: {}",
				source_str, source, type_str, type, id, severity_str, severity, message);
	else if (self.trace && type != GL_DEBUG_TYPE_PUSH_GROUP && type != GL_DEBUG_TYPE_POP_GROUP)
		log_gl.trace("OpenGL: src {} ({}), type {} ({:X}), id {}, severity {} ({:X}), message: {}",
				source_str, source, type_str, type, id, severity_str, severity, message);
};

// -------------------------------------------------------------------------------------------------

} // namespace gl
} // namespace libv
