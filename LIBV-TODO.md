--- STACK ------------------------------------------------------------------------------------------

component property
a system where every property used by a component is listed and described to
check invalid configurations (boost serialization like function)
-- commit --

rework renderer
opengl independent api in ui
4 way: template or linkage or external or dynamic
-- commit --

do not warn label without property size
...optional property use deafult would solve that one
-- commit --

component shortcut property setters for component specific modifier methods
setComponentID => set()(Component::ID, "main-menu-title")
setText		   => set()(Label::Text, "Main Menu");
-- commit --

percent is kind of shit... should be ratio and use the leftover (not statically assigned) space not all
-- commit --

use VBO for string2D
use VAO for string2D
-- commit --

font cache
-- commit --

shader cache
-- commit --

i "fucked up" vgl "rework" it
remove vm3 from gl
-- commit --

?kill def renderer
-- commit --

merge vsig back and create vmeta and vtmta (too many tamplate argument)
-- commit --

--- AWAITING ---------------------------------------------------------------------------------------

btn / regions
Graph based layout
Layout Property: what is the situation with per-component but container based properties?
what is ADL
think layout as a graph instad of a stack..., just think and see whats going on with that approach
c++ opengl api
PanelFlow::statics to enums
no opengl include in ui, some kind of link as ui-opengl
intel vtune
LIBV_ASSERT, LIBV_DEBUG_ASSERT, LIBV_STATIC_ASSERT in utility header
glEnable(GL_DEBUG_OUTPUT);
take a look at frame and component events
replace every raw ptr with a smart counter part (incl observer_ptr)
look after and variant https://isocpp.org/blog/2016/01/cpp-language-support-for-pattern-matching-and-variants
look after any
moving vec costume getter functions from member to public -> reducing symbols...
provide exception free alternative api EVERYWHERE! hehehehehe.
seg fault in resource! (just run libv_test in debug...) // however it will be rewritten
FIX: 3 5 [libv.ui.glfw] 65537 - The GLFW library is not initialized // This is a core issue
layout invalidation with change bitmask: OFFSET_BIT, SIZE_BIT, STRUCTURE_BIT? ...

--- ABANDONED --------------------------------------------------------------------------------------

LIBV_STRONG_TYPEDEF(int, Severity);
log thread naming
logger client - network connected different app (real time log viewer) with retrospective and real-time filtering and stuff...

// -------------------------------------------------------------------------------------------------

Window mode switching

GLFW now supports switching between windowed and full screen modes and updating the monitor and desired resolution and refresh rate of full screen windows with glfwSetWindowMonitor.
Window maxmimization support

GLFW now supports window maximization with glfwMaximizeWindow and the GLFW_MAXIMIZED window hint and attribute.
Window input focus control

GLFW now supports giving windows input focus with glfwFocusWindow.
Window size limit support

GLFW now supports setting both absolute and relative window size limits with glfwSetWindowSizeLimits and glfwSetWindowAspectRatio.
Localized key names

GLFW now supports querying the localized name of printable keys with glfwGetKeyName, either by key token or by scancode.
Wait for events with timeout

GLFW now supports waiting for events for a set amount of time with glfwWaitEventsTimeout.
Window icon support

GLFW now supports setting the icon of windows with glfwSetWindowIcon.
Raw timer access

GLFW now supports raw timer values with glfwGetTimerValue and glfwGetTimerFrequency.
Joystick connection callback

GLFW now supports notifying when a joystick has been connected or disconnected with glfwSetJoystickCallback.
Context-less windows

GLFW now supports creating windows without a OpenGL or OpenGL ES context with GLFW_NO_API.
Run-time context creation API selection

GLFW now supports selecting the context creation API at run-time with the GLFW_CONTEXT_CREATION_API window hint value.
Error-free context creation

GLFW now supports creating OpenGL and OpenGL ES contexts that do not emit errors with the GLFW_CONTEXT_NO_ERROR window hint, provided the machine supports the GL_KHR_no_error extension.
CMake config-file package support

GLFW now supports being used as a config-file package from other projects for easy linking with the library and its dependencies.

// -------------------------------------------------------------------------------------------------

I dont know yet what it could be used for, but this is could be VERY handy!
sfiane + decltype + operator ,
decltype(os << obj, void())

// -------------------------------------------------------------------------------------------------

Fully separating the ui from the frame handler is completely possible and desirable.
It is also possible do it without any dependency between the two with an additional "UIFrame lib".

cutting vm3 from vgl? has some advantage and vui doesnt need it... so why keep it around if it too
much pain to integrate

// -------------------------------------------------------------------------------------------------

Base class for lights and cameras...


license		https://www.youtube.com/watch?v=cJIi-hIlCQM
be aware that i will need copyright for every contribution into libv


http://www.cmake.org/cmake/help/v3.3/command/configure_file.html

Optimalizált fordítás - cmake:
https://github.com/sakra/cotire

P1 - std::future family

Priority levels for each operation
IN - in-context operation (fast and not context sensitive)
       GL Task        | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 Initialization       |   0000   |
 D.UIShaderProgram    |   1100   | Unloading as ShaderProgram
 D.ShaderProgram      |   1150   | Unloading as ShaderProgram
 D.UIShader           |   1200   | Unloading as Shader
 D.Shader             |   1250   | Unloading as Shader
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Render-Immediate     |   2000   | Starts by a timer when render time-window runs out
 Unload Texture       |   3100   |
 Unload Model         |   3200   |
 Unload Font          |   3300   |
 Unload ShaderProgram |   3400   |
 Unload Shader        |   3500   |
 Load UIShaderProgram |   4100   | Unloading as ShaderProgram
 Load ShaderProgram   |   4150   |
 Load UIShader        |   4200   | Unloading as Shader
 Load Shader          |   4250   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |
 Termination          |   9800   |
 Render-Residual      |   9900   | Always stays in queue as last operation

       IO Task        | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 D.UIShader File      |   1100   | Unloading as Shader File
 D.Shader File        |   1200   | Unloading as Shader File
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Unload Texture       |    IN    |
 Unload Model         |    IN    |
 Unload Font          |    IN    |
 Unload Shader File   |    IN    |
 Load UIShader File   |   4100   | Unloading as Shader File
 Load Shader File     |   4150   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |

top layer std::function and forward!

Move frame from disconnected monitor

Tracing every event for right state enums / defines / handlers:
	Char
	CharMods
	CursorEnter
	CursorPos
	Drop
	FramebufferSize
	Key
	MouseButton
	Scroll
	WindowClose
	WindowFocus
	WindowIconify
	WindowPos
	WindowRefresh
	WindowSize
	Monitor

// -------------------------------------------------------------------------------------------------

Update to GLFW 3.2

What is GLFW_USE_DWM_SWAP_INTERVAL?.... Test it off

Support for Vulkan
GLFW now supports basic integration with Vulkan with glfwVulkanSupported, glfwGetRequiredInstanceExtensions, glfwGetInstanceProcAddress, glfwGetPhysicalDevicePresentationSupport and glfwCreateWindowSurface. Vulkan header inclusion can be selected with GLFW_INCLUDE_VULKAN.
// Dont care / what is it?

Window mode switching
GLFW now supports switching between windowed and full screen modes and updating the monitor and desired resolution and refresh rate of full screen windows with glfwSetWindowMonitor.
// Must adopt

Window maxmimization support
GLFW now supports window maximization with glfwMaximizeWindow and the GLFW_MAXIMIZED window hint and attribute.
// Must adopt

Window input focus control
GLFW now supports giving windows input focus with glfwFocusWindow.
// Must adopt

Window size limit support
GLFW now supports setting both absolute and relative window size limits with glfwSetWindowSizeLimits and glfwSetWindowAspectRatio.
// Must adopt

Localized key names
GLFW now supports querying the localized name of printable keys with glfwGetKeyName, either by key token or by scancode.
// What is it? Adopt if useful. Is this keyboard layout related?

Wait for events with timeout
GLFW now supports waiting for events for a set amount of time with glfwWaitEventsTimeout.
// No use for me

Window icon support
GLFW now supports setting the icon of windows with glfwSetWindowIcon.
// Must adopt

Raw timer access
GLFW now supports raw timer values with glfwGetTimerValue and glfwGetTimerFrequency.
// No use for me

Joystick connection callback
GLFW now supports notifying when a joystick has been connected or disconnected with glfwSetJoystickCallback.
// Must adopt

Context-less windows
GLFW now supports creating windows without a OpenGL or OpenGL ES context with GLFW_NO_API.
// Could be useful but currently no plan for it. Document is as a TODO with very low priority

Run-time context creation API selection
GLFW now supports selecting the context creation API at run-time with the GLFW_CONTEXT_CREATION_API window hint value.
// What? Maybe not needed

Error-free context creation
GLFW now supports creating OpenGL and OpenGL ES contexts that do not emit errors with the GLFW_CONTEXT_NO_ERROR window hint, provided the machine supports the GL_KHR_no_error extension.
// Maybe not needed

CMake config-file package support
GLFW now supports being used as a config-file package from other projects for easy linking with the library and its dependencies.
// What? Maybe not needed



// -------------------------------------------------------------------------------------------------

Priority levels for each operation
IN - in-context operation (FAST and not context sensitive)

 GL Task              | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 Initialization       |   0100   |
 D.UIShaderProgram    |   1100   | Unloading as ShaderProgram
 D.ShaderProgram      |   1150   | Unloading as ShaderProgram
 D.UIShader           |   1200   | Unloading as Shader
 D.Shader             |   1250   | Unloading as Shader
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Render-Immediate     |   2000   | Starts by a timer when render time-window runs out
 Unload Texture       |   3100   |
 Unload Model         |   3200   |
 Unload Font          |   3300   |
 Unload ShaderProgram |   3400   |
 Unload Shader        |   3500   |
 Load UIShaderProgram |   4100   | Unloading as ShaderProgram
 Load ShaderProgram   |   4150   |
 Load UIShader        |   4200   | Unloading as Shader
 Load Shader          |   4250   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |
 Termination          |   9800   |
 Render-Residual      |   9900   | Always stays in queue as last operation

 IO Task              | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 D.UIShader File      |   1100   | Unloading as Shader File
 D.Shader File        |   1200   | Unloading as Shader File
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Unload Texture       |     IN   |
 Unload Model         |     IN   |
 Unload Font          |     IN   |
 Unload Shader File   |     IN   |
 Load UIShader File   |   4100   | Unloading as Shader File
 Load Shader File     |   4150   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |


variant
https://github.com/JasonL9000/cppcon14


two phase lookup, what, why, how?

-----
CMake resource folder
Cube / Sky Textures http://sourceforge.net/projects/spacescape/
Skeleton animation

Render Target
Shadow Pass
Multi Pass

VM4 Animated mesh

http://www.oldunreal.com/editing/s3tc/ARB_texture_compression.pdf
http://ogldev.atspace.co.uk/www/tutorial43/tutorial43.html
OpenGL Reference page: https://www.opengl.org/sdk/docs/man/


http://hmijailblog.blogspot.hu/2013/09/type-punning-aliasing-unions-strict.html

----

Adopt TCLAP http://tclap.sourceforge.net if suitable

Timed Event / Timer Support / Timer thread
Resettable and clearable timer / timer tasks

Config Entry:
	- default
	- description
	- isset
	- name
	- type
	- value

Optimizing for prefetcher.
Optimizing for minimum number of opengl bind.
Resource pack

----
Fresnel shader - Atmosphere
Cook-Torrance shader - Metal
Minnaert - More depth?
OrenNayar - More avg lambert


--- LIB Merge --------------------------------------------------------------------------------------


http://stackoverflow.com/questions/13128/how-to-combine-several-c-c-libraries-into-one

http://stackoverflow.com/questions/3821916/how-to-merge-two-ar-static-libraries-into-one

There are at least three ways to do this natively. The first and most portable way is to use libtool. After having built the other libraries also with libtool, you can combine them just by adding the .la libs to an automake libaz_la_LIBADD variable, or directly from a Makefile with something like:

libtool --mode=link cc -static -o libaz.la libabc.la libxyz.la

The other two are at least available when using GNU ar. You can use an MRI script (named for example libaz.mri), such as:

create libaz.a
addlib libabc.a
addlib libxyz.a
save
end

and then execute ar as:

ar -M <libaz.mri

Or you can use a thin archive (option -T), which will allow adding other archives without getting them nested inside, although the downside is that if you want to distribute the static library, the detached object will be missing:

ar -rcT libaz.a libabc.a libxyz.a

All the above methods gracefully handle overlapping member names from the original archives.

Otherwise, you'd have to unpack into different directories and repack again, to avoid replacing overlapping member names:

mkdir abc; cd abc; ar -x ../libabc.a
mkdir xyz; cd xyz; ar -x ../libxyz.a
ar -qc libaz.a abc xyz

--- PASTEBIN ---------------------------------------------------------------------------------------

> RANDOM
template<class URNG>
int rnd(int n, URNG &engine) {
    using dist_t = std::uniform_int_distribution<>;
    using param_t = dist_t::param_type;

    static dist_t dist;
    param_t params{0,n-1};

    return dist(engine, params);
}

//struct UniformLight {
//	Uniform<int> type;
//	Uniform<bool> enabled;
//
//	Uniform<glm::vec3> position;
//	Uniform<glm::vec3> direction;
//	Uniform<glm::vec4> diffuse;
//	Uniform<glm::vec4> specular;
//
//	Uniform<double> range;
//	Uniform<double> intensity;
//	Uniform<double> innerCosAngle;
//	Uniform<double> outerCosAngle;
//
//	Uniform<bool> shadowCast;
//	Uniform<int> shadowMapSampler;
//	Uniform<glm::mat4> shadowMVPTmat;
//
//	UniformLight(const std::string& name);
//	void operator=(const Light &v);
//};

diffuse(1.0f, 1.0f, 1.0f, 1.0f),
specular(0.8f, 0.8f, 0.8f, 1.0f),
emission(0.0f, 0.0f, 0.0f, 1.0f),
ambient(0.1f, 0.1f, 0.1f, 1.0f),
reflective(1.0f, 1.0f, 1.0f, 1.0f),
shininess(32.0f) { }


GLuint fullScreenQuadVBO;

void cleanFullScreenQuad() {
	glDeleteBuffers(1, &fullScreenQuadVBO);
}

void initFullScreenQuad() {
	float vertexData[]{
		0.0f, 0.0f, -1.0f, -1.0f,
		1.0f, 0.0f, 1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	glGenBuffers(1, &fullScreenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fullScreenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof (float)) * 16, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vglPassFullScreenQuad() {
	glBindBuffer(GL_ARRAY_BUFFER, fullScreenQuadVBO);
	glEnableVertexAttribArray(ATTRIBUTE_POSITION);
	glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD0);

	glVertexAttribPointer(ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof (float) * 4, ((void*) (sizeof (float) * 2)));
	glVertexAttribPointer(ATTRIBUTE_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof (float) * 4, ((void*) (sizeof (float) * 0)));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(ATTRIBUTE_TEXCOORD0);
	glDisableVertexAttribArray(ATTRIBUTE_POSITION);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vglViewportFullScreen() {
	glViewport(0, 0, options.graphics.resolution.get().x
			, options.graphics.resolution.get().y);
}


//glm::mat4 Light::getPmat() {
//	if (type == spotLight)
//		return perspective<float>(acos(outerCosAngle) * 180.0f / PI * 2, 1.0f, range / 15.0f, range); //2szeres outer sz�g, mivel nek�nk nem a 'fele' kell hanem a teljes 'sug�r'
//	else if (type == dirLight)
//		return ortho<float>(-90, 90, -90, 90, -90, 90);
//	else //if (type == pointLight)
//		return ortho<float>(-30, 30, -30, 30, -10, 150);
//}
//
//glm::mat4 Light::getVmat() {
//	if (type == spotLight)
//		return lookAt(position, position + direction, glm::glm::vec3(0, 1, 0));
//	else if (type == dirLight)
//		return lookAt(glm::vec3(0, 0, 0), direction, glm::glm::vec3(0, 1, 0));
//	else //if (type == pointLight)
//		return ortho<float>(-30, 30, -30, 30, -10, 150);
//}


std::this_thread::sleep_for(std::chrono::seconds(2));
