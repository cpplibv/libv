--- STACK ------------------------------------------------------------------------------------------

thread wait: https://www.youtube.com/watch?v=mPxIegd9J3w&list=PLHTh1InhhwT75gykhs7pqcR_uSiG601oh&index=43

http://learnopengl.com/#!Advanced-OpenGL/Advanced-GLSL
http://learnopengl.com/code_viewer.php?code=advanced/advanced_glsl_uniform_buffer_objects
http://learnopengl.com/#!Advanced-OpenGL/Cubemaps


adopting new vgl in ui
-- commit --

use VBO for string2D
use VAO for string2D
-- commit --

cache_ptr
-- commit --

font cache
-- commit --

shader cache
-- commit --

btn / regions
-- commit --

?kill def renderer if there is still any
-- commit --

--- AWAITING ---------------------------------------------------------------------------------------

gl: uniformbuffer?
gl: framebuffer
gl: renderbuffer
gl: templated buffer for binding
gl: remove irrelevant member function from templated textures
gl: glEnable(GL_DEBUG_OUTPUT);
layout: size percent should use the leftover space not the whole parent
layout: size ratio
layout: Layout Property: what is the situation with per-component but container based properties?
layout: think layout as a graph instad of a stack..., just think and see whats going on with that approach
layout: hard type (enum) align anchor and orient
ui: setText => set()(Label::Text, "Main Menu");
ui: take a look at frame and component events
ui: component property serialization and validation
ui: rework renderer - opengl independent api in ui - 4 way: template or linkage or external or dynamic
cpp: what is and how to use ADL
cpp: intel vtune
cpp: replace every raw ptr with a smart counter part (incl observer_ptr)
cpp: look after variant https://isocpp.org/blog/2016/01/cpp-language-support-for-pattern-matching-and-variants
cpp: look after any
libv: LIBV_ASSERT, LIBV_DEBUG_ASSERT, LIBV_STATIC_ASSERT in utility header
libv: provide exception free alternative api EVERYWHERE! hehehehehe.
libv: move vec costume getter functions from member to public -> reducing symbols...
seg fault in resource! (just run libv_test in debug...) // however it will be rewritten
FIX: 3 5 [libv.ui.glfw] 65537 - The GLFW library is not initialized // This is a core issue
merge vsig back and create vmeta and vtmta (too many tamplate argument)

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

I dont know yet what it could be used for, but this is could be VERY handy!
sfiane + decltype + operator ,
decltype(os << obj, void())

// -------------------------------------------------------------------------------------------------

Fully separating the ui from the frame handler is completely possible and desirable.
It is also possible do it without any dependency between the two with an additional "UIFrame lib".

// -------------------------------------------------------------------------------------------------

operator| on enums
https://www.youtube.com/watch?v=cZ3TNrRzHfM
http://cppcast.com/2016/07/gabriel-dos-reis/

// -------------------------------------------------------------------------------------------------

Base class for lights and cameras...


license		https://www.youtube.com/watch?v=cJIi-hIlCQM
be aware that i will need copyright for every contribution into libv


http://www.cmake.org/cmake/help/v3.3/command/configure_file.html

Optimalizált fordítás - cmake:
https://github.com/sakra/cotire


Új fajta signal: timerSignal
Mivel ez egy kicsit másabb, kell hozzá egy timer thread, meg az egész timer architektúra így nem a signal.hpp-ben kellene definiálni, hanem a timer.hpp-ban.
Valszeg ez egy egyszerü kompizitciója egy timer-nek és egy signalnak... Lehet, hogy nem is kell ehez külön signal tipus, csak a timernek kell tudni signalba adni... majd meglátjuk

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








class Model {
private:
	uint32_t vao;
	uint32_t vbo_vertex;
	uint32_t vbo_index;

private:
	libv::vm3::Model model;
	std::string name;

private:
//	libv::gl::Uniform<glm::mat4> uniformMVPmat;
//	libv::gl::Uniform<glm::mat4> uniformMmat;

public:
	Model(const char* data, const size_t size, const std::string& name = DEFAULT_MODEL_NAME);

	virtual ~Model();

private:
	void init(const char* data, const size_t size);
	void loadGL();
	void unloadGL();

public:
	inline const std::string& getName() const {
		return name;
	}
	void render(libv::gl::GL&);
	void renderNode(uint32_t id, libv::gl::GL&);
	bool loaded();
};

Model::Model(const char* data, size_t size, const std::string& name) :
	name(name) {
	init(data, size);
}

void Model::init(const char* data, size_t size) {
	if (!model.load(data, size)) {
		LIBV_GL_ERROR("Failed to load model: [%s]", name);
		return;
	}
	loadGL();
}

Model::~Model() {
	unloadGL();
}

// -------------------------------------------------------------------------------------------------

void Model::loadGL() {
	LIBV_GL_TRACE("GL Loading model: [%s]", name);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertex);
	glGenBuffers(1, &vbo_index);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof (vm3::Vertex) * model.vertices.size(), model.vertices.data(), GL_STATIC_DRAW);
	checkGL();

	enableVertexAttribArray(Attribute::position);
	vertexAttribPointer(Attribute::position, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, position));
	enableVertexAttribArray(Attribute::normal);
	vertexAttribPointer(Attribute::normal, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, normal));
	enableVertexAttribArray(Attribute::tangent);
	vertexAttribPointer(Attribute::tangent, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, tangent));
	enableVertexAttribArray(Attribute::bitangent);
	vertexAttribPointer(Attribute::bitangent, 3, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, bitangent));
	enableVertexAttribArray(Attribute::texcoord0);
	vertexAttribPointer(Attribute::texcoord0, 2, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, texCoord0));
	enableVertexAttribArray(Attribute::boneindices);
	vertexAttribIPointer(Attribute::boneindices, 4, VertexAttribType::INT, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, boneID));
	enableVertexAttribArray(Attribute::boneweight);
	vertexAttribPointer(Attribute::boneweight, 4, VertexAttribType::FLOAT, GL_FALSE, sizeof (vm3::Vertex), MEMBER_OFFSET(vm3::Vertex, boneWieght));
	checkGL();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (model.indices[0]) * model.indices.size(), model.indices.data(), GL_STATIC_DRAW);
	checkGL();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	checkGL();
}

void Model::unloadGL() {
	LIBV_GL_TRACE("GL Unloading model: [%s]", name);

	glDeleteBuffers(1, &vbo_index);
	glDeleteBuffers(1, &vbo_vertex);
	glDeleteVertexArrays(1, &vao);
}

void Model::render(libv::gl::GL& gl) {
	glBindVertexArray(vao);
	checkGL();

	renderNode(model.lods[0].rootNodeID, gl);

	glBindVertexArray(0);
	checkGL();
}

void Model::renderNode(uint32_t id, libv::gl::GL& gl) {
	gl.pushMatrixModel();
	gl.matrixModel() *= model.nodes[id].transformation;
//	uniformMmat = gl.matrixModel();
//	uniformMVPmat = gl.matrixMVP(); //<<<Assign uniforms
	//node->material->get<std::string>("diffuseTexture") //<<<Bind Textures here
	//libv::glsl::material = materials[entries[i].MaterialIndex]; //<<<Material here

	if (gl.matrixModel()[0][0] * gl.matrixModel()[1][1] * gl.matrixModel()[2][2] < 0) {
		glFrontFace(GL_CCW);
	} else {
		glFrontFace(GL_CW);
	}

	for (auto meshID : model.nodes[id].meshIDs) {
		glDrawElementsBaseVertex(GL_TRIANGLES,
				model.meshes[meshID].numIndices,
				GL_UNSIGNED_INT,
				(void*) (sizeof (GLuint) * model.meshes[meshID].baseIndex),
				model.meshes[meshID].baseVertex);
		checkGL();
	}

	for (auto childID : model.nodes[id].childrenIDs) {
		renderNode(childID, gl);
	}

	gl.popMatrixModel();
	checkGL();
}


//		model0("res/model/test_group.dae.pb.vm3"),
//		model1("res/model/fighter_01_eltanin.dae.pb.vm3"),
//		model2("res/model/test_sp.dae.pb.vm3"),
//		model3("res/model/projectile_missile_01_hellfire.0001.dae.pb.vm3"),
//		model4("res/model/asteroid_02.dae.pb.vm3")


// Light -------------------------------------------------------------------------------------------

struct LightType {
	static const int point = 0;
	static const int dir = 1;
	static const int spot = 2;
};

struct Light {
	int type;
	bool enabled; //0 false 1 true

	glm::dvec3 position;
	glm::dvec3 direction;
	glm::vec4 diffuse;
	glm::vec4 specular;

	double range;
	double intensity;
	double innerCosAngle; // Angles closer to 1 produce tighter cones
	double outerCosAngle; // Angles of -1 will emulate point lights.

	bool shadowCast; //0 false 1 true
	//			GLuint frameBuffer;
	//			GLuint shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
	//			GLuint shadowMapSampler;
	unsigned int frameBuffer;
	unsigned int shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
	unsigned int shadowMapSampler;
	glm::dmat4 shadowMVPTmat; //MVPT mat

	glm::dmat4 getVmat();
	glm::dmat4 getPmat();
	Light();
	~Light();
};

// -------------------------------------------------------------------------------------------------

Light::Light() :
	type(LightType::point),
	enabled(true),
	position(0, 0, 0),
	direction(1, 0, 0),
	diffuse(1, 1, 1, 1),
	specular(1.0f, 1.0f, 1.0f, 1.0f),
	range(75.0f),
	intensity(1.0f),
	innerCosAngle(0.8f),
	outerCosAngle(0.6f),
	shadowCast(false) { }

// -------------------------------------------------------------------------------------------------


// Planned attribute locations:
//   0  |     position  |             |
//   1  |       normal  |     psize   |
//   2  |      diffuse  |    color0   |
//   3  |     specular  |    color1   |
//   4  |   boneweight  |             |
//   5  |  boneindices  |             |
//   6  |               |    tangent  |
//   7  |               |  bitangent  |
//   8  |               |  texcoord0  |
//   9  |   tessfactor  |  texcoord1  |
//  10  |     instVec0  |  texcoord2  |
//  11  |     instVec1  |  texcoord3  |
//  12  |   instMatrix  |  texcoord4  |
//  13  |   ^^^^^^^^^^  |  texcoord5  |
//  14  |   ^^^^^^^^^^  |  texcoord6  |
//  15  |   ^^^^^^^^^^  |  texcoord7  |

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

class Framebuffer {
public:
	Framebuffer(const Framebuffer& other);
	Framebuffer(uint width, uint height, uchar color = 32, uchar depth = 24);
	~Framebuffer();

	operator GLuint() const;
	const Framebuffer& operator=(const Framebuffer& other);

	const Texture& GetTexture();
	const Texture& GetDepthTexture();

private:
	static GC gc;
	GLuint obj;
	Texture texColor;
	Texture texDepth;
};


// -------------------------------------------------------------------------------------------------

class Renderbuffer {
public:
	Renderbuffer();
	Renderbuffer(const Renderbuffer& other);
	Renderbuffer(uint width, uint height, InternalFormat::internal_format_t format);

	~Renderbuffer();

	operator GLuint() const;
	const Renderbuffer& operator=(const Renderbuffer& other);

	void Storage(uint width, uint height, InternalFormat::internal_format_t format);

private:
	static GC gc;
	GLuint obj;
};


// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================


#    define PUSHSTATE() GLint restoreId; glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &restoreId );
#    define POPSTATE() glBindFramebuffer( GL_DRAW_FRAMEBUFFER, restoreId );
Framebuffer::Framebuffer(const Framebuffer& other) {
	gc.Copy(other.obj, obj);
	texColor = other.texColor;
	texDepth = other.texDepth;
}
Framebuffer::Framebuffer(uint width, uint height, uchar color, uchar depth) {
	PUSHSTATE()

	// Determine appropriate formats
	InternalFormat::internal_format_t colorFormat;
	if (color == 24) colorFormat = InternalFormat::RGB;
	else if (color == 32) colorFormat = InternalFormat::RGBA;
	else throw FramebufferException();

	InternalFormat::internal_format_t depthFormat;
	if (depth == 8) depthFormat = InternalFormat::DepthComponent;
	else if (depth == 16) depthFormat = InternalFormat::DepthComponent16;
	else if (depth == 24) depthFormat = InternalFormat::DepthComponent24;
	else if (depth == 32) depthFormat = InternalFormat::DepthComponent32F;
	else throw FramebufferException();

	// Create FBO
	gc.Create(obj, glGenFramebuffers, glDeleteFramebuffers);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, obj);

	// Create texture to hold color buffer
	texColor.Image2D(0, DataType::UnsignedByte, Format::RGBA, width, height, colorFormat);
	texColor.SetWrapping(GL::Wrapping::ClampEdge, GL::Wrapping::ClampEdge);
	texColor.SetFilters(GL::Filter::Linear, GL::Filter::Linear);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);

	// Create renderbuffer to hold depth buffer
	if (depth > 0) {
		glBindTexture(GL_TEXTURE_2D, texDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, depthFormat, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		texDepth.SetWrapping(GL::Wrapping::ClampEdge, GL::Wrapping::ClampEdge);
		texDepth.SetFilters(GL::Filter::Nearest, GL::Filter::Nearest);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepth, 0);
	}

	// Check
	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw FramebufferException();

	POPSTATE()
}
Framebuffer::~Framebuffer() {
	gc.Destroy(obj);
}
Framebuffer::operator GLuint() const {
	return obj;
}
const Framebuffer& Framebuffer::operator=(const Framebuffer& other) {
	gc.Copy(other.obj, obj, true);
	texColor = other.texColor;
	texDepth = other.texDepth;

	return *this;
}
const Texture& Framebuffer::GetTexture() {
	return texColor;
}
const Texture& Framebuffer::GetDepthTexture() {
	return texDepth;
}

// -------------------------------------------------------------------------------------------------

GC Framebuffer::gc;
Renderbuffer::Renderbuffer() {
	gc.Create(obj, glGenRenderbuffers, glDeleteRenderbuffers);
}
Renderbuffer::Renderbuffer(const Renderbuffer& other) {
	gc.Copy(other.obj, obj);
}
Renderbuffer::Renderbuffer(uint width, uint height, InternalFormat::internal_format_t format) {
	gc.Create(obj, glGenRenderbuffers, glDeleteRenderbuffers);
	Storage(width, height, format);
}
Renderbuffer::~Renderbuffer() {
	gc.Destroy(obj);
}
Renderbuffer::operator GLuint() const {
	return obj;
}
const Renderbuffer& Renderbuffer::operator=(const Renderbuffer& other) {
	gc.Copy(other.obj, obj, true);
	return *this;
}
void Renderbuffer::Storage(uint width, uint height, InternalFormat::internal_format_t format) {
	glBindRenderbuffer(GL_RENDERBUFFER, obj);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

GC Renderbuffer::gc;

}

std::this_thread::sleep_for(std::chrono::seconds(2));
