--- DONE -------------------------------------------------------------------------------------------

libv.glr: Implement shader / program API
libv.glr: Implement Uniform Buffers and Uniform Blocks
libv.glr: Implement UBO layout std140
libv.glr: Generate glsl code for uniform blocks from the C++ code, not the other way around
libv.glr: Uniform<T>
libv.glr: UniformStream
libv.glr: Solve UniformStream batch counting
libv.glr: Texture
libv.gl: cleanup program1/2 shader1/2
libv.gl: cleanup texture1/2
libv.glr: Destroy, Remote ptr, GC
libv.glr: Cleanup mesh API and its includes, split sources as necessary
libv.gl: simplify the functionality, no auto create or destroy, no createData, createLink, plain structs
libv.glr: Texture manipulation, generation
libv.ui: Font2D
libv.ui: String2D
libv.ui: Font cache
libv.ui: Revive UI with minimal set
libv.ui: Label
libv.ui: font loading
libv.ui: context / gl unload que
libv.ui: shader / program unload que
libv.ui: vao / vbo unload que
libv.ui: Render position and render size aka mvp matrix
libv.ui: Rebase ui4 on top of glr
libv.ui: Kill and leftover of ui4
libv.ui.font: font check if bearing is calculated in the font engine
libv.ui.font: Bearing
libv.ui.font: Right side bearing
libv.ui: late attach
libv.ui: selective flag propagation
libv.ui: Program cache
libv.ui: shader / program loading
libv.ui: Revive quad
libv.ui: String2D justify also applies to the last line
libv.ui: Revive image
libv.ui: Solution to the ui context access problem: components should not initiate resource request? Setters should receive the resource type itself
libv.ui: Texture cache
libv.ui: Texture load
libv.ui: Revive stretch
libv.parse: Parse color: source file and pretty API
libv.ui: Parse size
libv.ui.style: properties and minimalistic style, with only static one time read / setup support
libv.ui.style: style-style inheritance, override and composition are the problem of the style definition system and isolated from the properties itself
libv.ui.style: UI component's visualization and behaviour is controlled by properties; Properties are statically typed values; Styles are named dynamic collections of properties; Animations are real time changes of properties
GCC 9.1: Using <filesystem> does not require linking with -lstdc++fs now.
libv.lua: object parser
libv.glr.std140: constexpr static string to name structs

--- STACK ------------------------------------------------------------------------------------------

libv.ui: hard type (enum) align anchor and orient
libv.ui.style: layout properties
libv.ui.style: font properties

libv.ui: make sandbox_ui.lua work

libv.ui.style: style safe (parent) storage, replace observer_ref with something

libv.ui.style: account for event based style changing or state based style declaration
libv.ui.style: style and property update | might be optional, or the animation system itself
libv.ui.style: style could store each component using them making invalidation possible, would listen to attach/detach

libv.ui.style: complex composite component would result in "nested" property sets
libv.ui.style: multiple style usage in a component would still be nice, maybe synthetized styles?

libv.ui: switch foreach children to fill a std::vector<component*>, this vector should be a reusable memory, or (?) just a small_vector<, 32> on the stack

wish: update cmake version and use add_compile_definitions() instead of add_definitions()
wish: target_link_directories()
wish: file glob CONFIGURE_DEPENDS https://cmake.org/cmake/help/v3.14/command/file.html#command:file

libv.ui: implement detach and component removal

libv.ui: rename size's "content" to "dynamic"

libv.frame: update glfw
libv.frame: icon support

libv.ui: cleanup context_ui redundant codes
libv.ui: context_ui and libv.gl:image verify that targets are matching the requested target

libv.glr: texture_fwd.hpp
libv.glr: RemoteTexture should have its own header file

libv.glr: Mesh attributes inside the remote should be stable, vector<unique_ptr<Attribute>>
libv.glr: Mesh attributes should use a single VBO

libv.ui: Label text change layout invalidation (lazy?)

libv.ui: Hide component internals
libv.ui: Auto set mvp matricies for the UI shaders | (?) | might not be possible

libv.ui: Focus
libv.ui: Event Input
libv.ui: Input field
libv.ui: Button

libv.color: New libv.color library, color space conversion and manipulations, template color space, template representation
libv.color: implement HCL and other color conversion functions http://www.chilliant.com/rgb2hsv.html
libv.ui: Color picker

libv.ui: Floating layout with movable components (frames), who handles which responsibility? Think about it and postpone this task

libv.ui: Label select / copy from a label (?) if specific property is set for it
libv.ui: Label link support (?)

libv.ui: List or Table - Not owning container view
libv.ui: Make a sandbox for a input->button->label->list

libv.ui: scroll pane

libv.ui: lua binding
libv.ui: None of the module functions should be virtual (?)

libv.ui: String2D would be nice to have both last line and non last line justify

libv.meta: PLEASE rename n_times.hpp

libv.ui: Can something be done about: change render position and change render size flags
libv.ui: Can something be done about: layout pass member variables in component_base, they are a lot of memory
libv.ui: I am sure that LastSize and LastPosition can be removed from component_base!

libv.ui: layout strong and week invalidation

libv.ui: font failure to load means fallback
libv.ui: shader failure to load means fallback
libv.ui: shader dynamic loading from file

libv.ui.text: render not found character

libv.glr: Procedural gizmo mesh
libv.glr: UniformBlockSharedView_std140

libv.ui: context_render: move out of module, it is not one

libv.ui: constraints: a way of syncing data between the world and the ui
libv.ui: ui <-> 3D layout linkage: planet names and additional informations are part of the ui and not the scene
		ui therefore has to access the game state (trivial, but this code has to happen now)

libv.ui.warning: warning if percent used inside a content is invalid

libv.ui: text
libv.ui: Idea that a component could signal the UI if it want to execute a heavy computation task before (attach, layout, create, render, destroy)

libv.ecui: component: Passive Label - verify
libv.ecui: component: Passive Image - verify
libv.ecui: component: Passive TableImage - verify
libv.ecui: component: Interactive Button
libv.ecui: layout: Flow
libv.ecui: layout: Float
libv.ecui: layout: 2D <-> 3D based on game state
libv.ecui: layout: Line - verify
libv.ecui: layout: Default (everything overlapped max)
libv.ecui: group: RadioButton
libv.ecui: wrapper: Scroll
libv.ecui: wrapper: Splitter
libv.ecui: composite: TextField (Label, TableImage, Label, +Events)

libv.ecui: state based ui, separate control and data!
libv.ecui: ui resource local proxies

libv.meta: rename if_void to lnv (as left non void) and make it variadic
libv.utility: Rename approxing.hpp to approx.hpp and the class name too
libv.utility: add/verify structured binding support for vec_t
libv.utility: Slice is an algorithm and not a utility

libv.ui: libv/ui/render/context.hpp -> libv/ui/context.hpp (?)
libv.ui: implement some ui stuff: btn / regions

libv.glr: strong type locations and indices with enum classes, also use libv::gl::uniform
libv.glr: Implement sub-mesh API
libv.glr: Fix uniform naming mess, Reduce the number of public members
libv.glr: vm4
libv.gl: https://learnopengl.com/PBR/Lighting
libv.glr: frame buffer
libv.glr: render target
libv.glr: post-processing emission / bloom
libv.glr: post-processing gamma
libv.glr: post-processing haze


libv.glr: shadow
libv.glr: Use instanced render for world shadow pass and clip with gl_ClipDistance[i] / glEnable(GL_CLIP_DISTANCEi);
libv.glr: Deferred-Shading https://learnopengl.com/Advanced-Lighting/Deferred-Shading
libv.glr: SSAO https://learnopengl.com/Advanced-Lighting/SSAO
libv.glr: HDR https://learnopengl.com/Advanced-Lighting/HDR
libv.glr: Bloom https://learnopengl.com/Advanced-Lighting/Bloom
libv.glr: FXAA
libv.glr: SRAA
libv.glr: Tiled-Deferred-Shading

libv.vm4: Model viewer, display statistics of texture density and estimated texture pixel world space size

libv.frame: frame calling show after show may brake things?
libv.frame: dual check lock every async frame operations

libv.utility: pointer facade for: observer_ptr, observer_ref, etc...
libv.utility: implement erase_unstable: x[index] = std::move(x[last]); x.erase(last)

wish: use NUMBER_OF_LOGICAL_CORES https://cmake.org/cmake/help/v3.14/command/cmake_host_system_information.html#command:cmake_host_system_information
wish: use include guards https://cmake.org/cmake/help/v3.14/command/include_guard.html#command:include_guard
wish: correct cmake target private/interface/public dependency (link and include) usage https://www.youtube.com/watch?v=y7ndUhdQuU8
wish: revisit catch object file linkage (only need to figure out a way to build it) as "target_link_libraries() command now supports Object Libraries"

--- AWAITING ---------------------------------------------------------------------------------------

asnyc: https://www.youtube.com/watch?v=t4etEwG2_LY
cli: Clara, most important questions are: multi, optional, required, positional and default arguments
			argument parsing lib idea to specify requirements: bool like temple expression: args.require(arg0 && arg1 || arg2), (arg3.count > 5 || arg4)
cmake: generator expressions https://cmake.org/cmake/help/v3.8/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)
cmake: Revisit the external auto rebuild feature, if() + file(TOUCH) + target_dependency()
cpp.proposal: P1 member_offset alternative for offsetof macro
		- similar approach is [p0908r0]
		- template <typename T, typename M> size_t member_offset(M T::* ptr) { /* implementation-defined*/ }
cpp.proposal: P2 std::uninitialized
		- ability to declare a primitive variable with explicitly uninitialized value
		- int variable = std::uninitialized;
cpp.proposal: P2 add auto ctor call to return MyType(arg0, arg1); => return (arg0, arg1);
		- decltype(auto) foo();
		- auto foo();
		- return (result);
cpp.proposal: P2 std::argument_with _name<T>: void foo(std::with_call_name<const T&> arg) { std::cout << arg.name << “: ” << args.value << std::endl; }
		void foo(variable) -> void foo(MAGIC(variable)) -> void foo(std::with_call_name{“variable”, variable});
		void foo(exp + ression) -> void foo(MAGIC(exp + ression)) -> void foo(std::with_call_name{“exp + ression”, exp + ression});
		Conversion would only fire after the overload resolution already took place. Or it would count as an implicit user defined conversion
		Questions: Is whitespace included? Newlines? Expressions? Whitespace in expressions? Constexpr? Just do what macros do?
cpp.proposal: P3 unrestricted template template parameters (and template concept parameters)
		template <template<typename...> container> struct S{}; // Current syntax
		template <template              container> struct S{}; // Proposed syntax
		It was always possible to use a wrapper type and traffic such unrestricted template params as member template typedef
cpp.proposal: P3 generalized type pack "using... ", type pack in non template parameter context
		using... types = int, double, std::string;
		std::variant<types...> global;
		Open question: issue of pack disambiguation in dependent name context: "dependent::pack types", "dependent::... types", "dependent::typepack types"
cpp.proposal: P3 structured binding pack
		auto& [...members] = object;
cpp.proposal: P3 vec_t<N, T>, matrix_t<N, M, T>
cpp.proposal: P4 allow trailing comma for function arguments and lambda captures and init lists, its already there for arrays and enums

cpp.stacktrace: Seams like a solid alternative for boost.stacktrace https://github.com/bombela/backward-cpp
cpp: (adaptive) radix tree - O(1) lookup
cpp: can there be multiple definition error during linkage if two lib contains the same (symbol) definition
cpp: clarify template vs auto type deduction rules
cpp: replace every raw ptr with a smart counter part (incl observer_ptr)
doc / blog: Klipse plugin - http://blog.klipse.tech/cpp/2016/12/29/blog-cpp.html
ecs: existence / super-position based predication
frame.core: remove core
frame: Move frames from disconnected monitor / off-screen
frame: remove default own thread, give them an io_context like executor
gl: docs http://docs.gl
gl: framebuffer
gl: glEnable(GL_DEBUG_OUTPUT);
gl: remove irrelevant member function from templated textures
gl: renderbuffer
gl: templated buffer for binding
gl: uniformbuffer?
layout: think layout as a graph instead of a stack..., just think and see whats going on with that approach
learn: https://gafferongames.com/post/state_synchronization/ or just https://gafferongames.com/
libv.ecs: Provide a component that has a special storage and can work as structure of arrays (SOA) instead of array of structures (AOS) to enable massive use of simd with a special foreach, so the general idea that share the indexing between tiny-tiny sub components
libv.log: log thread naming
libv.net: compression lib (fast, but not the best compression for me) https://github.com/google/snappy
libv.sig: merge back and place meta (too many tamplate argument) into libv.meta, (or dont, please, that is too many template)
libv.utility: Make a proper copy-pastable noisy type
libv.vm4: geomax / geoorig: find the biggest distance between any two vertex, avg(a, b) = geoorig, dist(a, b) / 2 = geomax
libv: LIBV_ASSERT, LIBV_DEBUG_ASSERT, LIBV_STATIC_ASSERT in utility header
libv: think about versioned namespace: namespace LIBV_NAMESPACE_VERSION { ... content ... } namespace libv = LIBV_NAMESPACE_VERSION
net: distributed servers (RAFT joint consensus algorithm) https://raft.github.io/
observe: https://bkaradzic.github.io/bgfx/examples.html
observe: https://github.com/bkaradzic/bgfx
observe: https://github.com/hugoam/mud
resource: dns like resource resolver for custom arguments: Args... -> ResourceDescriptor -> Resource
resource: forbid usage of absolute paths
resource: forbid usage of relative paths with starting ..
true: UNLESS someone like you cares a whole awful lot, nothing is going to get better. It's not.
ui.lua: https://www.wowace.com/projects/ace3/pages/ace-gui-3-0-widgets
ui: (shader) Program Descriptor: program is defined by a descriptor (which can be identified with a simple string key), this could also be applied for the rest of the resources
ui: https://www.factorio.com/blog/post/fff-246
ui: strong constraint: It has to keep up with 1000 character/sec input in mid sequence. Why? Because the 7.5cps is a reasonable high typing speed.
ui: take a look at frame and component events

ide.options:
	- profile:
	- built-in:     built-in / plug-in / user-defined
	- type:         string / color / font (or font_effect as one big blob)  / vec(2|3|4)
	- name:         group_a.group_b.item
	- base:         inherit_parent_project / reference(group_a.group_b.item)
	- value:        value / alter / merge: value: rgb() / hex / "string" / x,y,z / ... ; alter: hsl / alter hex / alter x,y,z / ...
	- effect:       value / alter / merge: underline / strikethrough / background
	- scripted:     lua
	profiles can inherit from other profiles
	project profile: each project has a built in profile that can be used for project specific settings
	online profiles / uri based profiles (if changes keep last N old versions, auto fetch on start)
colorpicker:
	- support all color space slider in one tab (all) and in different tabs too
	- support all color spaces, swatches, rgb, hsv, hsl, hcl with each 2x1 dimension change option
	- support auto relax color distances
	- support lua profile reference (?)
	- support lua operations
	- support multi color pick at the same time, mouse select, list select
	- support realtime result preview
	- visualize color distances, distance "bubbles"
	- visualize picked colors in sliders too

--- ABANDONED --------------------------------------------------------------------------------------

logger: client - network connected different app (real time log viewer) with retrospective and real-time filtering and stuff...
logger: binlog
cpp.compile: things I want to know about my compile time:
		- Instantiation time, count, location and arguments for every template and their size in binary (inline = 0)
		- List of headers included for every translation unit
		- Size of the resulting translation unit
		- Symbol content of the translation unit
		- Lists of translation units that includes (even if transitively) a given header
		- clang patch: https://www.youtube.com/watch?v=NPWQ7xKfIHQ
		- record in CI history per commit changes in every statistics
		- https://github.com/google/bloaty
		- https://github.com/mikael-s-persson/templight
		- https://www.cppdepend.com/

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
sfinae + decltype + operator ,
decltype(os << obj, void())
yeee i get it now: void to regular void by: "(void_expression, regular_void)" and operator,(T, regular_void)

// -------------------------------------------------------------------------------------------------

Base class for lights and cameras...

http://www.cmake.org/cmake/help/v3.3/command/configure_file.html

Optimalizált fordítás - cmake:
https://github.com/sakra/cotire


Priority levels for each operation
IN - in-context operation (fast and not context sensitive)
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
	WindowPosition
	WindowRefresh
	WindowSize
	Monitor

// -------------------------------------------------------------------------------------------------

TimerSignal
Mivel ez egy kicsit másabb, kell hozzá egy timer thread, meg az egész timer architektúra így nem a signal.hpp-ben kellene definiálni, hanem a timer.hpp-ban.
Valszeg ez egy egyszerü kompizitciója egy timer-nek és egy signalnak... Lehet, hogy nem is kell ehez külön signal tipus, csak a timernek kell tudni signalba adni... majd meglátjuk

// -------------------------------------------------------------------------------------------------

CMake resource folder
Cube / Sky Textures http://sourceforge.net/projects/spacescape/
Skeleton animation

Render Target
Shadow Pass
Multi Pass

VM4 Animated mesh

http://ogldev.atspace.co.uk/www/tutorial43/tutorial43.html
OpenGL Reference page: https://www.khronos.org/registry/OpenGL-Refpages/gl4/

----

Fresnel shader - Atmosphere
Cook-Torrance shader - Metal
Minnaert - More depth?
OrenNayar - More avg lambert

--- PASTEBIN ---------------------------------------------------------------------------------------

// Argument parsing

// Missing: default value, optional is there, and its fine, but default would be nicer.
// Missing: commands / sub commands / sub-sub commands

auto args = libv::arg::ArgumentParser();

const auto input_file = args.required<std::string>
		("input_file")
		["-i"]["--input"]
		("File path of the binary input file");

const auto output_file = args.optional<std::string>
		("output_file")
		["-o"]["--output"]
		("File path of the generated C++ output");

const auto function_name = args.optional<std::string>
		("function_name")
		["-f"]["--function"]
		("Name of the generated function");

const auto indexes = args.multiple<std::string>
		("indexes_name")
		["-i"]["--index"]
		("Indexes of something")
		.at_least(1).at_most(5);

const auto flag = args.flag
		("flag")
		["-f"]["--flag"]
		("Flag for something");

const auto rest = args.rest;

args.positional(input_file, output_file, function_name, indexes);

args.require(output_file && function_name || flag);
args.require_no_unused();

if (args.parse(argc, argv)) {
	std::err << args.error_message(100) << args.usage(100);
	std::exit(1);
}

std::cout << args.report(100);

// API:

args.flag               (name)[flag]+(description)?
args.multiple<T>        (name)[flag]+(description)?
args.optional<T>        (name)[flag]+(description)?
args.required<T>        (name)[flag]+(description)?
args.positional         (arg0, arg1)
args.require            (arg0 && arg1 || arg2)
args.require_no_unused  ()
args.parse              (argc, argv)
args.error_message      (width)
args.report             (width)
args.usage              (width)

-------------------------------------------

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
		log_gl.error("Failed to load model: {}", name);
		return;
	}
	loadGL();
}

Model::~Model() {
	unloadGL();
}


// -------------------------------------------------------------------------------------------------

void Model::loadGL() {
	log_gl.trace("GL Loading model: {}", name);

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
	log_gl.trace("GL Unloading model: {}", name);

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


// Light -------------------------------------------------------------------------------------------

struct LightType {
	static const int point = 0;
	static const int dir = 1;
	static const int spot = 2;
};

struct Light {
	int type = LightType::point;
	bool enabled = true;

	glm::vec3 position = {0, 0, 0};
	glm::vec3 direction = {1, 0, 0};
	glm::vec4 diffuse = {1, 1, 1, 1};
	glm::vec4 specular = {1, 1, 1, 1};

	double range = 75.0;
	double intensity = 1.0;
	double innerCosAngle = 0.8; // Angles closer to 1 produce tighter cones
	double outerCosAngle = 0.6; // Angles of -1 will emulate point lights.

	bool shadowCast = false;
	//			GLuint frameBuffer;
	//			GLuint shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
	//			GLuint shadowMapSampler;
	unsigned int frameBuffer;
	unsigned int shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
	unsigned int shadowMapSampler;
	glm::mat4 shadowMVPTmat; //MVPT mat

	glm::mat4 getVmat();
	glm::mat4 getPmat();
};

// -------------------------------------------------------------------------------------------------

// Planned attribute locations:
//   0  |     position  |             |
//   1  |       normal  |     psize   |
//   2  |               |   diffuse   |
//   3  |               |  specular   |
//   4  |   boneweight  |             |
//   5  |  boneindices  |             |
//   6  |      tangent  |             |
//   7  |    bitangent  |             |
//   8  |    texcoord0  |             |
//   9  |   tessfactor  |  texcoord1  |
//  10  |     instVec0  |  texcoord2  |
//  11  |     instVec1  |  texcoord3  |
//  12  |   instMatrix  |  texcoord4  |
//  13  |     ^^^^^     |  texcoord5  |
//  14  |     ^^^^^     |  texcoord6  |
//  15  |     ^^^^^     |  texcoord7  |

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

Framebuffer::Framebuffer(const Framebuffer& other) {
	gc.Copy(other.obj, obj);
	texColor = other.texColor;
	texDepth = other.texDepth;
}
Framebuffer::Framebuffer(uint width, uint height, uchar color, uchar depth) {
	GLint restoreId; glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &restoreId );

	// Determine appropriate formats
	InternalFormat::internal_format_t colorFormat;
	if (color == 24) colorFormat = InternalFormat::RGB;
	else if (color == 32) colorFormat = InternalFormat::RGBA;
	else throw FramebufferException();

	InternalFormat::internal_format_t FormatDepth;
	if (depth == 8) FormatDepth = InternalFormat::DepthComponent;
	else if (depth == 16) FormatDepth = InternalFormat::DepthComponent16;
	else if (depth == 24) FormatDepth = InternalFormat::DepthComponent24;
	else if (depth == 32) FormatDepth = InternalFormat::DepthComponent32F;
	else throw FramebufferException();

	// Create FBO
	gc.Create(obj, glGenFramebuffers, glDeleteFramebuffers);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, obj);

	// Create texture to hold color buffer
	texColor.Image2D(0, DataType::UByte, Format::RGBA, width, height, colorFormat);
	texColor.SetWrapping(GL::Wrapping::ClampEdge, GL::Wrapping::ClampEdge);
	texColor.SetFilters(GL::Filter::Linear, GL::Filter::Linear);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);

	// Create renderbuffer to hold depth buffer
	if (depth > 0) {
		glBindTexture(GL_TEXTURE_2D, texDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, FormatDepth, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		texDepth.SetWrapping(GL::Wrapping::ClampEdge, GL::Wrapping::ClampEdge);
		texDepth.SetFilters(GL::Filter::Nearest, GL::Filter::Nearest);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepth, 0);
	}

	// Check
	if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw FramebufferException();

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, restoreId );
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

// -------------------------------------------------------------------------------------------------

Additional cmake.wish ideas:

wish_config(
	TARGET                      __PARENT_TARGET_____TARGET__
	FOLDER                      __PARENT_FOLDER__/__TARGET__
	ALIAS                       __PARENT_ALIAS__::__TARGET__
	SOURCE                      src/__FOLDER__/*.cpp
	HEADER                      src/__FOLDER__/*.hpp

	RESULT_PATH_BINARY_APP      bin/__FOLDER__/
	RESULT_PATH_BINARY_EXAMPLE  bin/__FOLDER__/
	RESULT_PATH_BINARY_SANDBOX  bin/__FOLDER__/
	RESULT_PATH_BINARY_TEST     bin/__FOLDER__/
	RESULT_PATH_HEADER          include/__FOLDER__/
	RESULT_PATH_LIBRARY_DYNAMIC lib/__FOLDER__/
	RESULT_PATH_LIBRARY_STATIC  lib/__FOLDER__/
)

string(LENGTH ${CMAKE_SOURCE_DIR}_ WISH_SHORT_PATH_CUTOFF)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

wish_create_base(
	TARGET libv

	WARNING GNU VERSION_GREATER 7.0 -Wduplicated-branches
	WARNING GNU VERSION_GREATER 7.0 -Wrestrict
	WARNING GNU VERSION_GREATER 7.0 -Wshadow-compatible-local
	WARNING GNU VERSION_GREATER 8.0 -Wcast-align=strict
	WARNING GNU VERSION_GREATER 8.0 -Wmultistatement-macros

	OPTION DEBUG -Og
	OPTION DEBUG -ggdb3
	OPTION RELEASE -O3
	OPTION RELEASE -static
	OPTION DEV -O3
	OPTION -m64
	OPTION -std=c++2a
	OPTION -fconcepts

	DEFINE -Dconcept="concept bool"
	DEFINE -D "LIBV_REQUIRES\\(...\\)=\"requires __VA_ARGS__\""
	IDE_DEFINE LIBV_REQUIRES(...)

	DEFINE -DGIT_BRANCH="${WISH_GIT_BRANCH}"
	DEFINE -DGIT_COMMIT_HASH="${WISH_GIT_COMMIT_HASH}"
	DEFINE -DWISH_SHORT_PATH_CUTOFF=${WISH_SHORT_PATH_CUTOFF}
	DEFINE -DWISH_SHORT_PATH_PREFIX="${CMAKE_SOURCE_DIR}/"
)

wish_create_library(
	PARENT libv
	TARGET vm4imp
	LINK PUBLIC libv::log libv::vm4 ext::assimp
)

// -------------------------------------------------------------------------------------------------

A - ALPHA
B - BRAVO
C - CHARLIE
D - DELTA
E - ECHO
F - FOXTROT
G - GOLF
H - HOTEL
I - INDIA
J - JULIET
K - KILO
L - LIMA
M - MIKE
N - NOVEMBER
O - OSCAR
P - PAPA
Q - QUEBEC
R - ROMEO
S - SIERRA
T - TANGO
U - UNIFORM
V - VICTOR
W - WHISKY
X - X-RAY
Y - YANKEE
Z - ZULU
