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
libv.glr.std140: constexpr static string to pretty name structs
libv.ui: implement float container/layout based on the new container based layout property system
libv.ui: rework panel container to the container based layout property system
libv.ui: rework default layout to the container based layout property system
libv.glr: add viewport feature
libv.ui: make window resize work
libv.ui.style: style safe (parent) storage, replace observer_ref with intrusive_ptr
libv.ui: context ptr in component
libv.ui: attach must be called before create
libv.ui: style auto inherit based on dots
libv.ui: switch foreach children to fill a std::vector<component*>, this vector should be a reusable memory, or (?) just a small_vector<, 32> on the stack | layouts were promoted to containers
libv.ui: integrate style into component and container
libv.ui: auto-read container properties from component's styles
libv.ui: auto-read component properties from component's styles
libv.ui: hard type (enum) align anchor and orient
libv.ui.style: layout properties
libv.ui.style: font properties
libv.ui: use generic_path in context for logging and lookup
libv.ui: None of the module functions should be virtual (?) | they got removed
libv.ui: context_render: move out of module, it is not one
libv.ui: default * property should never be null, provide context access for the property system via archive
libv.ui: manually assigned values to a property shall not be overridden by style (this will be related for animation)
libv.ui: add component exclusive properties
libv.ui: Structure properties and use libv reflection for property sets
libv.ui: unify style and property function that are currently at 3 place (component_base::set/reset/value, propertySet::set, AccessComponent::style) (doStyle can stay, that is a different tale)
libv.ui: setStyle should notify the parent, or signal a flag that its needs to restyled
libv.ui: propagate property set change to actual invalidation of flags
libv.ui: sort out property change auto invalidate flag
libv.ui: releaseing a property override should trigger style lookup (reset function) if there is no style, give back the fallback value
libv.ui: sort out flags, invalidation rules and validation traverses
libv.ui: layout strong and week: flags/invalidations
libv.ui: sort out component_base
libv.ui: search and replace layoutPass -> layout
libv.algorithm: Implement a bisect algorithm
libv.range: Implement deinterleave view
libv.ui: make layouts unit testable, its a must given the requirements, traits class for layout related types
libv.ui: grid layout - every element has its own size
libv.ui: port line layout and tests
libv.ui: layouting with content size should not account for nested child non-requested content size: so image(100px, 100px) inside a panel(d,d): here panel should have 100px,100px size (and not the underlying image's size)
libv.ui: port full layout and tests
libv.ui: fix content size bug in full layout
libv.ui: port float layout and tests
libv.ui: fix content size bug in float layout
libv.ui: basic panel base class to handle current panels
libv.ui: remove / removaAll functions for every container
libv.input: New lib to story input enums for frame, ui and hotkey
libv.ui: ContextMouse
libv.ui: MouseWatcher
libv.ui: MouseInterest
libv.ui: MouseEvent
libv.ui: Implement mouse order, for now as a simple depth counter passed along in ContextLayout2 (update the code in button dolayout2)
libv.ui: button
libv.ui: Events are mandatory at this point, implement them
libv.ui: prettify EventMouse to have all the fancy accessors and query subevents
libv.ui: hook up mouse window leave/enter
libv.ui: layout, do not use component internal states as temp variables
libv.ui: sort out lastPosition / lastSize (aka component layout caching)
libv.ui: remove LastSize and LastPosition from component_base
libv.ui.style: Instead of sub/unsub an alternative more safer API would be to traverse every component if any style changed and update components who actually needs it
libv.ui: style should not track components, but restyle everything if style update happens as it should be extremely rare
libv.ui: property should only invalidate if its != to previous value
libv.frame: update glfw
libv.ui: more foreach children usage, MORE and that could solve my Self/Child issue
libv.ui: need doAttachSelf / doAttachChild separation, too easy to fuck-up watching a flag (?)
libv.ui: childID
libv.ui.style: Container child properties are not following style changes
libv.ui.style: Container child properties are not following child's style changes
libv.ui: more of property should only invalidate if its != to previous value
libv.ui: implement component detach
libv.ui: implement component remove
libv.ui: implement layout flag
libv.ui: implement render flag
libv.ui: implement component destroy
libv.ui: implement top level ui detach on exit
libv.ui: implement top level ui destroy on exit
libv.ui: Hide component internals
libv.ui: kill every immediate importance TODO and make a proper commit
libv.ui.style: account for event based style changing or state based style declaration
libv.ui.style: style and property update | might be optional, or the animation system itself used for it | fullscan approch was implemented
libv.ui.style: style could store each component using them making invalidation possible, would listen to attach/detach | fullscan
libv.ui: implement detach and component removal
libv.ui: wire in component create and destroy
libv.ui: String2D would be nice to have both last line and non last line justify
libv.ui: Label text change layout invalidation (lazy?) | verify
wish: rename LIBV_REQUIRES to WISH_REQUIRES | (?)
wish: rename GIT_BRANCH and ..HASH to WISH_GIT_BRANCH and ..HASH | only the C++ macros, the cmake vars are correct
libv.ui: rename ComponentBase to BaseComponent or even to Component (?) | (Component or BasicComponent might end up being a CRTP class)
libv.log: rename libv::logger to libv::logger_stream
libv.ui: rename size's "content" to "dynamic"
libv.meta: rename n_times.hpp to for_constexpr
libv.meta: rename if_void to lnv (as leftmost-non-void) and make it variadic
libv.utility: rename approxing.hpp to approx.hpp and the class name too
libv.frame: rename onContextInitialization, onContextRefresh, onContextTerminate to Create Update/Render Destroy
libv.utility: move Slice from utility to algorithm
libv.frame: update glfw
libv.frame.glfw.frame: Deprecated charmods callback
libv.frame: serialize events
libv.frame: Merge mouse position events (only if there was no intermediate event)
libv.frame.glfw.core: Deprecated window parameter of clipboard string functions
libv.frame.glfw.frame: Added glfwFocusWindow for giving windows input focus
libv.frame.glfw.frame: Added glfwRequestWindowAttention function for requesting attention from the user
libv.frame.glfw.frame: Added glfwGetWindowFrameSize for retrieving the size of the frame around the client area of a window
libv.frame.glfw.frame: Added glfwMaximizeWindow for window maximization
libv.frame.glfw.frame: Added glfwSetWindowMaximizeCallback and GLFWwindowmaximizefun for receiving window maximization events (#778)
libv.frame.glfw.hint: Added GLFW_MAXIMIZED for window maximization
libv.frame.glfw.frame: Added glfwSetWindowSizeLimits for setting absolute window size limits
libv.frame.glfw.frame: Added glfwSetWindowAspectRatio for setting relative window size limits
libv.frame.glfw.core: Added glfwSetWindowAttrib function for changing window attributes (#537)
libv.frame.glfw.frame: glfwSetWindowAttrib supports GLFW_DECORATED for existing windows
libv.frame.glfw.frame: glfwSetWindowAttrib supports GLFW_RESIZABLE for existing windows
libv.frame: remove unnecessary atomics
libv.frame: replace fix state arrays with flat_maps
libv.frame: Implement floating aka always on top
libv.frame.glfw.frame: glfwSetWindowAttrib supports GLFW_FLOATING for existing windows
libv.frame.glfw.hint: Added GLFW_FLOATING for creating always-on-top windowed mode windows
libv.frame.glfw.hint: Added GLFW_FOCUSED window hint for controlling initial input focus
libv.frame.glfw.hint: Added GLFW_FOCUS_ON_SHOW window hint and attribute to control input focus on calling show window (#1189)
libv.frame.glfw.hint: Added GLFW_LOCK_KEY_MODS input mode and GLFW_MOD_*_LOCK mod bits (#946)
libv.frame.glfw.frame: glfwSetWindowAttrib supports GLFW_FOCUS_ON_SHOW for existing windows
libv.frame: CursorMode and glfwSetInputMode
libv.frame: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
libv.frame: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
libv.frame: glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
libv.frame: fix clipboard thread
libv.frame.glfw.frame: Added glfwSetWindowIcon for setting the icon of a window
libv.frame: icon support, glfwSetWindowIcon(window, 2, images);, Preferred in 3 resolutions are: 16x16, 32x32, 48x48, (maybe as an extra 24x24, 64x64)
libv.frame.glfw.monitor: Added glfwGetMonitorPos, glfwGetMonitorPhysicalSize and glfwGetMonitorName for retrieving monitor properties
libv.frame.glfw.monitor: Added glfwGetMonitorWorkarea function for retrieving the monitor work area (#920,#989,#1322)
libv.frame.glfw.monitor: Added glfwGetMonitorContentScale
libv.frame.glfw.frame: Added glfwSetWindowMonitor for switching between windowed and full screen modes and updating the monitor and desired video mode of full screen windows
libv.frame: DISPLAY_MODE_BORDERLESS is not perfect, fix it | DisplayMode change itself has some issues
libv.frame: implement setDisplayMode without cmdFrameRecreate
libv.frame: bug: hide then restore breaks something (the redraw?), repeated show fixes it | invalid events caused the issue, now they are discarded with workaround
libv.frame.glfw.frame: Added glfwGetWindowContentScale and glfwSetWindowContentScaleCallback for DPI-aware rendering
libv.frame: cleanup core and core proxy
libv.frame: add a mutex to event callbacks and event queue
libv.frame: add a mutex to windowHandlers
libv.frame: add a mutex to monitors map
libv.frame: add a mutex to frame (its needs one because of the getters could access data members)
libv.frame: Review threading and ownership models of the whole library | I just throw a bunch of mutex at the problem, I feel guilt
libv.frame: cleanup includes
libv.ui: use the following pattern for unnamed components: struct adopt_lock_t {}; static constexpr adopt_lock_t adopt_lock;
libv.ui: focus - Two main operation: focus-traversal (finding the next focus target) and focus-change (event of focus change to the component selected by focus-traversal)
libv.ui.focus: Focusable is new optional property (not the property system) and FocusableSelf / FocusableChild are new flags for it
libv.ui.focus: Focus traversal order: component hierarchy
libv.ui.focus: Focus-traversal with precise direction instead of just the 2/4/8 base direction (1 float is enough for angle)
libv.ui.focus: UI should have a pointer to the currently focused object
libv.ui.focus: Focus-change event
libv.ui.focus: Focusable Components should store a flag if its focused (this also helps with detach)
libv.ui.focus: Focus-traversal operation triggers a Focus-change event on both end
libv.ui.focus: Direct focus can be triggered by code
libv.ui.focus: Mouse events can trigger direct focus | non-automated version
libv.ui: key input
libv.ui: char input
libv.ui.component: input field
libv.ui.keyboard: keyboard event are forwarded directly to the focused object and traversed up the focus stack until intercepted
libv.ui: Implement / check on detach
libv.ui: Implement / check on destroy
libv.ui: remove layout2 pass member variables in component_base
libv.ui: Question: does style change means property update? | this could be a rare enough event to brute force the whole ui style refresh, but only if its not event driven | style does not change, style change is only supported for debug/development
libv.ui.event: add a component based mouse event (virtual function) option BESIDE the current watcher one
libv.ui: automate MouseRegion update, request a component to be passed alongside the watcher, set a flag, if flag set, auto update in layout2 | only works if mouse region matches the component position and size, or if it does a separate pass | added a component based watcher BESIDE the current watcher one
libv.ui: consolidate MouseInterest flags
libv.ui: flags set in ctor -> parent might not get the relevant *child flags, check on propagation (self/child masks)
libv.ui: component detach does not remove certain flags from parent, parent have to rescan child to determine correct flags
libv.ui: with capability oriented flags, the flags.reset should prompt reevaluation of flags in the parent ('set' part is taken care by flagAuto) | its already taken care by the top-down passes of the capabilities
libv.ui: event oriented flags, reset should unsub from events | solved by separate set/get and flagPurge
libv.ui: event oriented flags, set should sub for events | solved by separate set/get and flagPurge
libv.ui: focus, key and char watcher and dynamic change of them
libv.ui: pimpl ContextUI
libv.ui: style should be forward declared
libv.ui: Input field have/display cursor
libv.ui: direct access font texture | i think this will also improve texture bind codes, making them more uniform
libv.ui: Position InputField cursor correctly
libv.ui: Stream2D move back to utf8
libv.ui: Stream2D merge back to String2D
libv.ui: Shelf Stream2D as StringU2D
libv.ui: String2D API get position of character between space, by index: 0 = before first char, 1 = before second char, size() and size()+k = after last character
libv.ui: ui.settings and its FAST availability from context() | settings might end up being really stored in the context
libv.ui.property: remove style code from base_component.hpp (ContextStyle) (optional / best effort)
libv.ui.property: free property name binding
libv.ui.property: need support for structure and different names, libv.reflection might gets a hit because of this
libv.ui.property: remove fallback from properties and place it into context_ui as a fallback style
libv.ui.property: detect to property changes
libv.ui.property: react to property changes
libv.ui.property: property set / get via functions to eliminate mandated property storage in component
libv.ui.property: set/get/reset changed/manual in general not only in hybrid
libv.ui.property: setter / getter issue: does not set manual/changed flag when called manually
libv.ui.property: have a single line in set/get function that refers to the property?
libv.ui.property: hybrid reflection - static
libv.ui.property: hybrid implement doStyle work
libv.ui.property: hybrid Add a default / init value per instance ("above" fallback)
libv.ui.property: hybrid reset (address could be used to lookup) | simple full style invalidation at first
libv.ui: rename cursor to caret
libv.ui.property: migrate to hybrid reflection
libv.ui.property: cleanup old non hybrid property codes: PropertyFF, valueFF
libv.ui.style: cleanup fallback style
libv.ui: setX() / getX() -> void X(T) / T X()
libv.ui: mark every component type one paramed ctors as explicit (and maybe check on every ui class too)
libv.ui.font: line 179 not just log but return default on error
libv.ui: using FontSize = int16_t; -> enum class FontSize : int16_t {};
libv.ui: flatten - flatten EventMouse to combat variant complexity
libv.ui: signal-slot and event proxies, do not store a signal inside the component, use a member function event() to yield a proxy that has many signals | Couple of changes, global ContextEvent dispatcher, no proxy
libv.ui: contextlessness
libv.ui: events for input field
libv.ui: rename UnnamedTag_t to GenerateName_t
libv.ui: events for button
ext: adopt https://github.com/dacap/clip
libv.ui: clipboard access from ContextUI
libv.ui.input_field: paste support
libv.ui.input_field: copy support
libv.ui.input_field: caret support
libv.ui: Is text is a component property that cannot be set from style, BUT it can be accessed dynamically in a uniform manner? | do I really have to implement a 'property' method ? | Yep, implemented as synthetize property
libv.ui: String2D fix for new line (glyphless) mean no entry in the mesh for it | solved by hidden glyphs
libv.ui: String2D fix new line before after glyph is not possible if there is no glyph in the line | solved by hidden glyphs
libv.ui: String2D fix new line before after glyph bearing issue (test char '[') | solved by hidden glyphs
libv.ui: String2D new line alignment tests
libv.ui: Verify if String2D is pen.x += kerning.x; to early | it is not, it helps with correct line width
libv.ui: String2D fix new line alignment
libv.ui: String2D fix justify cursor placement | space glyph should be snapped to the left and not to the right
libv.ui.input_field: test cursor placement with a non mono font
libv.ui.input_field: cut support
libv.ui: fix cut-paste breaks search field (? with newline involved)
libv.ui: add exception catch beds to main ui operations
libv: fix build for all / tests / run tests
libv.ui: String2D set/get modernization
libv.ui: Every event: focus, mouse, key, char shall provide access to the entire state universe | Access to state universe from everywhere
libv.ui: Make focus nullable
libv.ui: Remove context_state.focus_ handling from ImplUI, remove ImplUI from ContextState
libv.ui.input_field: caret mouse support
libv.ui: String2D getClosestCharacterIndex should use pen position, some glyphs are all over the place
libv.ui.input_field: synthetize caret property
libv.ui.input_field: mouse caret placement respect line
libv.ui.input_field: fix that lines tend to snap down | Solved by determining that LineAdvance != LineHeight
libv.ui: String2D API to find nearest character position
libv.gl: framebuffer
libv.gl: Texture2DMultisample
libv.gl: Texture2DMultisampleArray
libv.gl: blit
libv.gl: renderbuffer
libv.gl: multisample capability
libv.gl: glReadPixels
libv.gl: rename framebuffer read|draw*D function family to attach_read|draw*D and add plain attach
libv.glr: framebuffer
libv.glr: framebuffer binding
libv.gl: ability to current bound texture
libv.glr: solution for internal hiding but exposing correct offset for inline "direct" access to mandatory info (id and dirty) | aliasing head
libv.glr: framebuffer auto init textures | solved with sync_no_bind()
libv.glr: optimize textures with head access
libv.glr: texture_fwd.hpp
libv.glr: RemoteTexture should have its own header file
libv.glr: renderbuffer
libv.glr: blit
libv.gl: track framebuffer bindings and eliminate duplicate bind
libv.glr: Texture2DMultisample | RemoteTexture will be the most work, might worth to think about a different type?
libv.glr: Texture2DMultisampleArray
libv.glr: multisample capability
libv.glr: glReadPixels | only makes sense in remote, and not for queue
libv.glr: rename gc to DestroyQueue
libv.glr: first half of a major overhaul on includes and code structure | hide remotes, provide head access
libv.glr: cleanup queue
libv.ui: debug zoom in overlay
libv.ui: component level overlay system
libv.ui: component fix attach-detach
libv.ui: debug zoom proper frame buffer size setup | based on component level layout
libv.ui: debug zoom toward center
libv.ui: mouse grab system, a way to soft lock mouse event positions to an initiator mouse watcher
libv.ui: bug: mouse acquire might generate extra events? | null update was broadcasted on accident
libv.ui: scroll bar (aka slider) without the scroll pane
libv.ui: remap mouse input in debug zoom mode (remap function) | inline remap causes hazard on movement | fixed by control/view split but rounding might broke things
libv.ui: overlay zoom, zoom toward mouse
app.vm4_viewer: list models
app.vm4_viewer: load model
libv.vm4: Generate flipped winding order version of index sequences for mesh inside inverted nodes
app.vm4_viewer: show model wireframe (gl fill mode based)
libv.vm4: fix vm4 root node issue
app.vm4_viewer: show model fragment input values
app.vm4_viewer: Implement camera
app.vm4_viewer: Implement camera controls
libv.vm4: Import calculate AABB
libv.vm4: Import calculate BS
app.vm4_viewer: Visualize AABB
app.vm4_viewer: Visualize BS
app.vm4_viewer: Reset camera on model loading
app.vm4_viewer: Camera center and focus model (without altering the rotation)
app.vm4_viewer: Reset camera / Focus camera
app.vm4_viewer: Visualize grid-XY
libv.fsw: Lib to wrap a file system watcher
libv.fsw: improve file watcher to handle single file and multi callbacks
libv.fsw: use absolute path internally, pass rel or abs path to the callback based on the sub
libv.fsw: guard redundant watchers and ownerships
libv.fsw: subscribe_directory
libv.fsw: WA0001 workaround
libv.fsw: unsubscribe (with cleanup)
libv.fsw: Remove WA0001 workaround
libv.ui: instead of return value use (with different name) void stop_propagation() const; / bool propagation_stopped() const; / mutable bool stop_propagation_{ false };
libv.ui: Mouse events should respect stop_propagation
libv.ui: Mouse event absorb/shield/plates
libv.ui: alias any libv.input event in libv.ui for consistent access
app.vm4_viewer: proper camera grab
libv.ui: Rename mouse_table to context_mouse
libv.gl: Implement a GLSL engine
libv.gl.glsl: Implement primitive preprocessor with #include and include dirs
libv.gl.glsl: Warning option for crlf line ending.
libv.gl.glsl: Warning option for space indentation or if indentation characters are mixed


--- STACK ------------------------------------------------------------------------------------------


app.vm4_viewer: single directional light
app.vm4_viewer: show model grey lighted (phong)

libv.ui: relative - mouse event should contain a watcher relative (local) coordinates too
app.vm4_viewer: camera movement should acquire and lock mouse position


> Render Data Dependency Graph aka Scene structure reorganization
	Node
		string name
		Node*[] children

	Object : Node
		transformation

	Scene
		Camera
		Model
		Light[]
		Shader[]

	Camera : Object
		camera properties

	Model : Object
		gl::mesh
		Material[]
		vm4::model

	Shader
		texture channels
		gl::uniform location miscellaneous
		gl::uniform location transformation
		gl::uniform location light[]
		gl::uniform location material[]

	Light : Object
		light properties

	Material
		Shader*
		material properties

	Texture
		map type (channel)
		gl::texture

libv.ui: UI based file watcher, libv.fsw > queue > ui loop event stage > broadcast
app.vm4_viewer: UI shader should unsub from file watcher, ATM there is an issue during program termination

app: about page with executable path, build and compiler information, git hash and software version
libv.main: Small library to wrap main and command line arguments into a modern C++ format with global access: span<string_view>

vm4 hash and timestamp
	ext: cryptopp https://github.com/weidai11/cryptopp
	ext: openssl https://github.com/janbar/openssl-cmake
	libv: MD5
	libv: SHA3-256
	libv: SHA3-512
	libv.vm4: model hash (calculated at import)
	libv.serialization: std::chrono::system_clock::time_point (convert to UTC) (serialize a normalized value around a fixed epoch)
	libv.vm4: model import timestamp

libv.vm4lua: lua binding for vm4 and vm4 importer

glsl: quaternion https://github.com/mattatz/ShibuyaCrowd/blob/master/source/shaders/common/quaternion.glsl and http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
libv.ui: read nana 3rd party property tree lib API
libv.ui: non-shared_ptr based panels, aka static_container (?)
app.vm4_viewer: display statistics

app.vm4_viewer: import model
app.vm4_viewer: Add config option to disable reset camera on model loading
app.vm4_viewer: Add config options for AABB
app.vm4_viewer: Add config options for BS
app.vm4_viewer: Add config options for grid-XY
app.vm4_viewer: Add project level size comparison functionality
app.vm4_viewer: Thumbnails
app.vm4_viewer: Better focus camera based on the actual view angles and model ineast of the BSO

libv.ui: static_component system
libv.ui: list
libv.ui: table layout - only the columns and/or rows have size
libv.ui: not owning container views (list and/or table)

libv.ui: local mouse position (for both button, scroll and movement), update related code in scroll_bar | or 'global' way to query local mouse position (or query component global position (account for zoom overlay))

libv.gl: learn the meaning of multisample fixedlocation (in case of Texture2DMultisample (and why there is none for RBO))
libv.gl: learn the difference between read/write framebuffer on attachment, can even a FBO have different read/draw attachments and how does that work? Func in question: glFramebufferTexture*D


component
	libv.ui: clipping vertex shader (with on/off)
			| stencil could also be a solution, and it would be even better, more generic, non intrusive for the other shaders
			| or just use a viewport call and correct the projection matrix
	libv.ui: scroll pane | shader clip plane (scissors), (effects every ui shader) | only pane without scroll bar | NOTE: Check git stash
	libv.ui: progress bar | progress bar can have unknown max value, have a mode for it | 3 part: bg, bar, spark

atlas
	libv.ui: texture atlas definition/parsing
	libv.ui.atlas: ui theme atlas loading and auto-preview, semi-auto atlas definition
	libv.ui: support atlas based images

ui
	libv.ui: if ui size is 0, do not execute anything from ui loop (frame buffer protection)
	libv.ui: layout padding
	libv.ui: fragments, sub component reusable codes
	libv.ui: background fragment, property type that can be quad/image/stretch/paramed_stretch/gradient with color/shader/params
	libv.ui: ContextResource
	libv.ui: ContextConfig
	libv.ui: ContextFileWatcher
	libv.ui: ContextLoopExecutor: any_thread_callable_cb = context.loop.callback_as_event(my_event_callback)
	libv.ui: ContextExecutor
		libv.ui: async_task<T> work_async(const function<progress_report(multi_entry_call, stop_token)>& task);
	libv.ui: ContextStat (not ContextState, stat is for ui statistics)
	libv.ui: mark remove is non-sense for static component system, or composite objects, hide it
	libv.ui: add a glr::remote& to UI to simplify app::frame
	libv.ui: component position is currently relative to origin, once 'frame' and 'scroll' component comes in this will change

layout
	libv.ui: doLayout1 should use the return channel instead of member cache
	libv.ui: remove layout1 pass member variables in component_base
	libv.ui: broken layout with String2D with size = "100px, d" if text is longer than 100px, layout1 issue | main reason is that layout1 pass uses no limits

cleanup
	libv.ui: context_ui and libv.gl:image verify that targets are matching the requested target (2D)
	libv.ui: cleanup context_ui redundant codes
	libv.ui: fatal log before every assert

mouse
	libv.ui: mouse events should consider depending on if the window is focused or not | non trivial either way, might be best to have both option dynamically | need this as component level dynamically (camera controls need global, for other ui actions local is enough)
	libv.ui: unchanged - updating watcher (any property) without change should not yield any event | do I care about it?
	libv.ui: Mouse acquire should leave any over component (?)
	libv.ui: Mouse release should enter any over component (?) (respect non-pass_through)
	libv.ui: Absorb - make sure absorb/shield/plates is easy to have/access for even non interactive components

event
	libv.ui: Common base class for event "host stubs" to handle general events (mouse / keyboard / component lifetime)
	libv.ui: Provide general events for every component: read more about http://nanapro.org/en-us/documentation/core/events.htm
	libv.ui: Read http://nanapro.org/en-us/documentation/core/events.htm
	libv.ui: Unignorable event handlers
	libv.ui: Improve event connection: operator() / connect / connect_front / connect_unignorable
	libv.ui: if 'everything' 'above' is done re-read the requirements of mouse events and verify if all of them are met

properties / style
	libv.ui.property: property system interaction with static_component system
	libv.ui.property: complex composite component would result in "nested" property sets
	libv.ui.property: scope / sheet / component type based style rules
	libv.ui.property: if fallback value is requested log a warning
	libv.ui.property: dynamic access
	libv.ui.property: style property (literally a property that is a style ptr, useful for interactive components and their changes)
	libv.ui.property: account for cyclic owning references with style property
	libv.ui.style: parent depends on layout invalidation could be introduced into the property as function test just like fallback
	libv.ui.style: verify that style change in child causes restyle in properties stored inside the parent
	libv.ui.style: either I centralize and static bind every property name to a single type OR i allow multiple different type/value under a single name
	libv.ui.style: do not track style changes, require ui to be prompted about change
	libv.ui.property: solve name/type collusion
	libv.ui.style: Style Sheet / A proper default style or a way to auto assign styles to components would be nice (this is not fallback.)
	libv.ui.style: Style based on component state (bit-mask)
	libv.ui.property: layout properties: size / anchors (+padding) (+?merge cells) | can size + anchor be generalized?
	libv.ui.property: typed property registry
	libv.ui.property: optimize property reset: address could be used to lookup

interactive
	libv.ui: Cursor image change
	libv.ui: Make a sandbox for a input->button->label->list
	libv.ui: mouse drag and drop system
	libv.ui.input_field: Implement FocusSelectPolicy
	libv.ui.input_field: Implement NewLine/MultiLinePolicy
	libv.ui.input_field: if text does not fit, crop/layer it and only display around caret
	libv.ui.input_field: if text does not fit, display a popup with full text on mouse hover and idle
	libv.ui.input_field: input mask (this will possibly a different input_field type)
	libv.ui.input_field: mask('*') for passwords
	libv.ui.input_field: mouse hover cursor change to cursor-caret symbol
	libv.ui.input_field: multi_line(true)
	libv.ui.input_field: selection support
	libv.ui.input_field: multi-selection support
	libv.ui.input_field: synthetize selection property
	libv.ui.input_field: synthetize multi-selection property
	libv.ui.input_field: text function call should produce event
	libv.ui.input_field: background_shadow_tip_string("Password")
	libv.ui.input_field: tip_string("Generic password related tip")
	libv.ui.input_field: undo/redo support

hotkey
	libv.hotkey: design API
	libv.hotkey: review glfwGetKeyName and glfwSetInputMode http://www.glfw.org/docs/latest/group__keys.html
	libv.hotkey: There will be a need for logical and physical key definition (99% physical, ctrl+z logical)
	libv.frame.input: Added glfwGetKeyName for querying the layout-specific name of printable keys
	libv.frame.input: Added glfwGetKeyScancode function that allows retrieving platform dependent scancodes for keys (#830)
	libv.ui.hotkey: hotkey system are an extension to the keyboard system
	libv.ui.hotkey: ui.focus-backward = shift+tab
	libv.ui.hotkey: ui.focus-forward = tab
	libv.ui.hotkey: ui.focus-tab-backward = ctrl+shift+tab
	libv.ui.hotkey: ui.focus-tab-forward = ctrl+tab
	libv.ui.hotkey: ui.new-line = enter
	libv.ui.hotkey: ui.select = space
	libv.ui.hotkey: ui.send = enter
	libv.ui.hotkey: ui.send = kp-enter
	libv.ui.hotkey: ui.send-alt = shift+enter
	libv.ui.hotkey: ui.send-alt = shift+kp-enter
	libv.ui.hotkey: ui.text.cursor-down = down
	libv.ui.hotkey: ui.text.cursor-left = left
	libv.ui.hotkey: ui.text.cursor-right = right
	libv.ui.hotkey: ui.text.cursor-up = up
	libv.ui.hotkey: ui.text.jump-end = end
	libv.ui.hotkey: ui.text.jump-home = home
	libv.ui.hotkey: ui.text.remove-backward = backspace
	libv.ui.hotkey: ui.text.remove-forward = delete
	libv.ui.hotkey: ui.text.remove-word-backward = ctrl+backspace
	libv.ui.hotkey: ui.text.remove-word-forward = ctrl+delete
	libv.ui.hotkey: ui.text.select = lmb [hold]
	libv.ui.hotkey: ui.text.select-left = shift+left
	libv.ui.hotkey: ui.text.select-right = shift+right
	libv.ui.hotkey: ui.text.select-word = lmb, lmb
	libv.ui.hotkey: ui.text.select-word-left = ctrl+shift+left
	libv.ui.hotkey: ui.text.select-word-right = ctrl+shift+right
	libv.ui.hotkey: ui.redo = ctrl+[y] // logical y key
	libv.ui.hotkey: ui.undo = ctrl+[z] // logical z key
	libv.ui.hotkey: ui.redo-non-destructive // emacs style redo, not the default
	libv.ui.hotkey: ui.undo-non-destructive // emacs style undo, not the default

focus
	libv.ui.focus: Key/Hotkey events can trigger focus-traversal
	libv.ui.focus: easy way to trigger Focus traversal from events/components
	libv.ui.focus: by default unhandled [tab] hotkeys should trigger focus traversal
	libv.ui.focus: by default unhandled [esc] hotkeys should trigger focus clear
	libv.ui.focus: Ability to yield focus gain, when a component receives the onFocus (gain) event it would be nice to have a way to refusing it forcing it to travel further
	libv.ui.focus: Focus traversal order: direct link (ptr, ptr)
	libv.ui.focus: Focus-traversal needs a component hierarchy independent way to be defined, no raw id or index, use ptr/refs when direct setting it

	libv.ui.focus: Focus traversal order: layout driven (layout knows the orientation)
	libv.ui.focus: Focus traversal order: position based

libv.ui: implement parentsDependOnLayout, reduce the number of layout invalidation caused by String2D edit
libv.ui: include check everything / fwd everything
libv.ui: statistics: each ui operation (attach, style, render, ...) histogram, min, max, count

libv.console: new console lib? A UI component will also be needed for it, but the backend should work without gui
libv.console: should depend on libv.arg

style
	libv.ui: style sheets
	libv.ui: lua style parsing and lua file tracking with auto re-style | only style parsing
	libv.ui.style: (style exclusive / multiple) multiple style usage in a component would still be nice, maybe synthetized styles?
	libv.ui.layout: anchor should be a general property, when a component placed somewhere beside unused space single enum which of the 9 corner should it use

overlay
	libv.ui: generic orthogonal component level overlay system
		libv.ui: overlay event freeze
			libv.ui: overlay event grab / multi-root / fake-root / overlay focus shaky
			libv.ui: overlay controls / mode switches / overlay mode display / overlay control display

		libv.ui: overlay zoom
			libv.ui: pixel distance measure / pixel region measure

		libv.ui: overlay cursor pixel highlight, coordinate display and mouse region stack display
		libv.ui: overlay magnifier (local zoom) (optionally slow mouse movement speed (freeze cursor with frame))
		libv.ui: overlay (mouse) stack display
		libv.ui: overlay component tree display
		libv.ui: overlay debug data stream, a context based output that can be written and displayed as a HUD

		libv.ui.property: hybrid reflection - dynamic
			libv.ui: detach should also be an event (mouse_table could utilize it to cleanup, debug observers could also use it)
				libv.ui: overlay property inspector (name it inspector)

		libv.ui: overlay resource viewer (textures, fonts, shaders (?))


--- [[[ deadline: 2019.12.01 ]]] ---


libv.utility: Implement a proper match file iterator "dir/part*.cpp", possibly with filesystem + ranges | use wildcard functions, but split pattern / match for performance | design API allow async (give next N passed entry or M failed entry)

app.vm4_viewer: implement a small light gui app to provide guidance to GUI development
app.vm4_viewer: display statistics of texture density and estimated texture pixel world space size

libv.math: create vec_fwd and mat_fwd headers
libv.sig: merge back the sig codebase rework a lighter version of the lib
libv.gl: move glew init into GL (with the ability to optionally disable it with a constructor argument)

libv.ui: 2Dify UI: layout only makes sense in 2D, this does not forbids 3D element nor 3D layers not 3D positions, but layouts makes no sense in 3D 99.99% of the cases | positions are 3D sizes are 2D and maybe 3D normals (?) | normal is not necessary per component, its enough to have one on the "tilting" container
libv.ui.layout: size over 100% is not an error
libv.glr: layout_to_string.hpp stream_struct_name test / use #include <boost/type_index.hpp> boost::typeindex::type_id_with_cvr<T>.pretty_name()

libv.ui: lua binding | or rather a lua component or prototype parsing
libv.ui: make sandbox_ui.lua work

wish: update cmake version and use add_compile_definitions() instead of add_definitions()
wish: target_link_directories()
wish: file glob CONFIGURE_DEPENDS https://cmake.org/cmake/help/v3.14/command/file.html#command:file
wish: add support for OUTPUT_NAME and use it for the apps https://cmake.org/cmake/help/latest/prop_tgt/OUTPUT_NAME.html
wish: use NUMBER_OF_LOGICAL_CORES https://cmake.org/cmake/help/v3.14/command/cmake_host_system_information.html#command:cmake_host_system_information
wish: use include guards https://cmake.org/cmake/help/v3.14/command/include_guard.html#command:include_guard
wish: correct cmake target private/interface/public dependency (link and include (and source and definitions)) usage https://www.youtube.com/watch?v=y7ndUhdQuU8
wish: revisit catch object file linkage (only need to figure out a way to build it) as "target_link_libraries() command now supports Object Libraries"

libv.ui: Label select / copy from a label (?) if specific property is set for it | its a property

libv.ui: font failure to load means fallback | verify
libv.ui: shader failure to load means fallback | verify
libv.ui: shader dynamic loading from file

libv.ui.font: fallback to Unicode Character 'REPLACEMENT CHARACTER' (U+FFFD) | add it back to fallback consolas_min font | https://www.fileformat.info/info/unicode/char/fffd/index.htm
libv.ui.font: fallback to ascii '?'
libv.ui.font: fallback to simple square (render the not found character by 'hand')
libv.ui.font: https://www.freetype.org/freetype2/docs/tutorial/step2.html
libv.ui.font: We do not check the error code returned by FT_Get_Kerning. This is because the function always sets the content of delta to (0,0) if an error occurs.

libv.ui: Auto set mvp matricies for the UI shaders | (?) | might not be possible

libv.ppc: new Pre-Processor Compiler library, a cpp "like" pre-processor implementation for shader loading

libv.color: New libv.color library, color space conversion and manipulations, template color space, template representation
libv.color: implement HCL and other color conversion functions http://www.chilliant.com/rgb2hsv.html
libv.ui: Color picker

libv.ui: Docker layout with movable components (frames), who handles which responsibility? Think about it and postpone this task

libv.ui: group: RadioButton
libv.ui: Splitter
libv.ui: composite: TextField (Label, TableImage, Label, +Events)
libv.ui: possible component list https://en.wikipedia.org/wiki/Widget_(GUI)

libv.ui: batch component meshes into a bigger ui mesh cluster and use subcomponents, optimizations-optimizations

libv.utility: add/verify structured binding support for vec_t

libv.ui.warning: warning if percent used inside a content is invalid | not just log, but a generalized ui report system | console with extras | Cancel! 100%+ is valid (Example: button with a 120% shadow around it)

libv.glr: Cleanup includes for:
			libv/glr/mesh.cpp
			libv/glr/mesh.hpp
			libv/glr/program.cpp
			libv/glr/program.hpp
			libv/glr/uniform_buffer.cpp
			libv/glr/uniform_buffer.hpp
			libv/glr/uniform_stream.hpp
libv.glr: Implement head pattern for:
			libv/glr/mesh.cpp
			libv/glr/mesh.hpp
			libv/glr/program.cpp
			libv/glr/program.hpp
			libv/glr/uniform_buffer.cpp
			libv/glr/uniform_buffer.hpp
			libv/glr/uniform_stream.hpp
			libv/glr/framebuffer.cpp
			libv/glr/framebuffer.hpp
			libv/glr/texture.cpp
			libv/glr/texture.hpp
libv.glr: GLR User facing API shall not expose to the OpenGL header
libv.glr: better name for DestroyQueue
libv.glr: libv/glr/remote.hpp only included in glr files to fetch DestroyQueue, pass that around instead (with a better name), could be boundeled with the gl reference.
libv.glr: create bool -> remote == nullptr

libv.glr: too many variant, function / arg streams?, uniform_stream has a similar pattern that could be generalized

libv.gl: Check on TextureRect if it is working properly (attempted to use it in font2D but failed, image was correct indexing/sampler issues (?))
libv.math: make every vec / mat operator a hidden friend | Is it possible or is it worth it (it might make 5 overload from the current 3 per operator)?

libv.glr: Mesh attributes inside the remote should be stable, vector<unique_ptr<Attribute>>
libv.glr: Mesh attributes should use a single VBO

libv.ui: Label link support (?)
libv.glr: Procedural gizmo mesh
libv.glr: UniformBlockSharedView_std140


--- [[[ deadline: 2020.01.01 ]]] ---


libv.ui: constraints: a way of syncing data between the world and the ui
libv.ui.layout: Flow
libv.ui.layout: 2D <-> 3D based on game state
libv.ui: ui <-> 3D layout linkage: planet names and additional informations are part of the ui and not the scene
		ui therefore has to access the game state (trivial, but this code has to happen now)

libv.ui: text | easy text
libv.ui: Idea that a component could signal the UI if it want to execute a heavy computation task before (attach, layout, create, render, destroy)
libv.ui: String2D scream at the user if the API get a \r (or any non printable character beside \n \t), and ignore it

libv.glr: strong type locations and indices with enum classes, also use libv::gl::uniform
libv.glr: Implement sub-mesh API
libv.glr: Fix uniform naming mess, Reduce the number of public members

libv.glr: vm4 | non trivial
libv.vm4: in header include version
libv.vm4: in header include date
libv.vm4: in header include hash
libv.vm4: geomax / geoorig: find the biggest distance between any two vertex, avg(a, b) = geoorig, dist(a, b) / 2 = geomax
libv.va4/ia4: (Iris/Vader Asset 4) New "library" to handle vm4 models and bundle them.
libv.va4/ia4: Animation drivers: look at, rotate around world/local xyz, pulsate, play animation, etc...

libv.gl: https://learnopengl.com/PBR/Lighting
libv.glr: post-processing emission / bloom
libv.glr: post-processing gamma
libv.glr: post-processing haze

libv.glr: Shadow
libv.glr: Cascaded shadow maps
libv.glr: Use instanced render for world shadow pass and clip with gl_ClipDistance[i] / glEnable(GL_CLIP_DISTANCEi);
libv.glr: Tiled Forward Shading (aka Forward+) https://www.3dgep.com/forward-plus/
libv.glr: Volume Tiled Forward Shading https://www.3dgep.com/wp-content/uploads/2017/07/3910539_Jeremiah_van_Oosten_Volume_Tiled_Forward_Shading.pdf
libv.glr: Deferred-Shading https://learnopengl.com/Advanced-Lighting/Deferred-Shading and https://www.3dgep.com/volume-tiled-forward-shading/
libv.glr: SSAO https://learnopengl.com/Advanced-Lighting/SSAO
libv.glr: HDR https://learnopengl.com/Advanced-Lighting/HDR
libv.glr: Bloom https://learnopengl.com/Advanced-Lighting/Bloom
libv.glr: FXAA
libv.glr: SRAA
libv.glr: Tiled-Deferred-Shading
libv.glr: Order Independent Transparency (OIT)


--- [[[ deadline: 2020.02.01 ]]] ---


libv.ecui: state based ui, separate control and data

libv.frame: cleanup states by adding a single state for show/hidden/maximized/minimized/fullscreen/borderless_maximized
libv.frame: cleanup monitor, provide a thread-safe access to monitors list
libv.frame: cleanup global variables, at least place them next to each other and reason about thread access
libv.frame.input: Question should I couple scancode with key for each key states | observe use-case
libv.frame: remove clipboard support, ext clip will take care of it.

libv.utility: pointer facade for: observer_ptr, observer_ref, etc...

libv.ui.event: mouse/keyboard/joystick ability to query sub-frame resolution of press/held/release cycle. Events are timed (a lot of timestamp)

ext.sol2: update and there is a new fwd header, use it if/where appropriate: sol_forward.hpp
cpp: enun class default underlying type is int, specify underlying type for every enum class
cpp: check if every possible operator had been made to a hidden friend
cpp: check if i have any recursive variadic function that is not using if constexpr but uses a tail overload
libv.gl: use mdspan for image updates instead of raw loops
libv.gl: glReadBuffer and glDrawBuffer | not urgent as the defaults are correct
libv.gl: learn glPixelStore / implement
libv.gl: glsl preprocessor support for "#pragma vertex" and "#pragma fragment" to auto split files, useful for small test and debug shaders

libv.glr: texture, do not store the whole image, only have pending chunks, this will get rid of a lot of memcopy nightmare
libv.glr: optimize every remote resource with head access
libv.ui: switch of direct glr usage to a UI wrapper bulk renderer
libv.ui: question: for bulk rendering would stencil buffer help for fonts | yes-kind of but alpha makes it tricky
libv.glr: IDEA: An Entry object the group every call/state to a single render operation, both in queue and on the user API
libv.glr: IDEA: Versioned resources, beside what tell which version are we want to use
libv.glr: Make the reordering explicit and opt-in
libv.glr: Refactor the variants

libv.ecs: Test fails with an assert in boost vector

libv.lua: function to immediately destroy a variable (useful for game object or gui object cleanup that would hold a resource otherwise)

ext.clara: remove dependency
ext.lua: update to 8eb1482b493a3a44f004c86baeeb0683ec094542 at https://github.com/xpol/lua for 5.3.5

libv.math: Catmull-Rom spline https://www.youtube.com/watch?v=9_aJGUTePYo and code https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_Splines2.cpp

libv.ui: Could boost::sync_queue be used?
libv.utility: Add lexically_normal to generic_path and cleanup relevant usages


--- AWAITING ---------------------------------------------------------------------------------------

asnyc: https://www.youtube.com/watch?v=t4etEwG2_LY
cli: Clara, most important questions are: multi, optional, required, positional and default arguments
			argument parsing lib idea to specify requirements: bool like temple expression: args.require(arg0 && arg1 || arg2), (arg3.count > 5 || arg4)
cmake: generator expressions https://cmake.org/cmake/help/v3.8/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)
cmake: Revisit the external auto rebuild feature, if() + file(TOUCH) + target_dependency() | could I use separate cmake file for each ext and make a dependency on it ?
color.math: colorspace of humans https://en.wikipedia.org/wiki/Lab_color_space
color.math: http://graficaobscura.com/matrix/index.html
color.picker: http://tristen.ca/hcl-picker/#/hlc/10/1/F68385/D1E468
color.picker: https://tools.lgm.cl/colorpicker.html
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
cpp.proposal: P3 template specialization without opening a namespace
		issue: inline namespace, namespace/class scope disambiguation
cpp.proposal: P4 allow trailing comma for function arguments and lambda captures and init lists, its already there for arrays and enums
cpp.proposal: P4 file watcher
cpp.stacktrace: Seams like a solid alternative for boost.stacktrace https://github.com/bombela/backward-cpp
cpp: (adaptive) radix tree - O(1) lookup
cpp: aligned_storage is UB, use this instead: alignas(T) std::byte storage[sizeof(T)];
cpp: can there be multiple definition error during linkage if two lib contains the same (symbol) definition
cpp: clarify template vs auto type deduction rules
cpp: keyword order: [[nodiscard]] virtual explicit friend static constexpr inline const void&& function() const&& noexcept override final;
cpp: learn std::launder and std::bless
doc / blog: Klipse plugin - http://blog.klipse.tech/cpp/2016/12/29/blog-cpp.html
doc: code snippet generation mdsnippets.com https://github.com/simonCropp/MarkdownSnippets
ext.mysql: mysql connector source https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.0.17-src.tar.gz
ext: adopt a better hash_map and remove every std::unordered container
ext: adopt colony https://github.com/mattreecebentley/plf_colony
ext: adopt mdspan https://github.com/kokkos/mdspan/wiki/A-Gentle-Introduction-to-mdspan
ext: adopt zlib (remove assimp internal zlib) https://github.com/madler/zlib (light wrapper for usage: https://gist.github.com/gomons/9d446024fbb7ccb6536ab984e29e154a )
gl: docs http://docs.gl
gl: glEnable(GL_DEBUG_OUTPUT);
gl: renderdoc https://renderdoc.org/
gold: And if thou gaze long at a finite automaton, a finite automaton also gazes into thee.
gold: UNLESS someone like you cares a whole awful lot, nothing is going to get better. It's not.
gold: existence / super-position based predication
learn: https://gafferongames.com/post/state_synchronization/ or just https://gafferongames.com/
math: blue noise, a stable uniform noise
net: distributed servers (RAFT joint consensus algorithm) https://raft.github.io/
observe: https://bkaradzic.github.io/bgfx/examples.html
observe: https://github.com/bkaradzic/bgfx
observe: https://github.com/hugoam/mud
resource: dns like resource resolver for custom arguments: Args... -> ResourceDescriptor -> Resource
resource: forbid usage of absolute paths
resource: forbid usage of relative paths with starting ..

libv.ecs: Provide a component that has a special storage and can work as structure of arrays (SOA) instead of array of structures (AOS) to enable massive use of simd with a special foreach, so the general idea that share the indexing between tiny-tiny sub components
libv.frame: Move frames from disconnected monitor / off-screen
libv.frame: remove default own thread, give them an io_context like executor
libv.gl: templated buffer for binding
libv.gl: uniformbuffer?
libv.log: log thread naming
libv.net: Possible C++20 feature std::ispanstream would allow direct deserialization out from a received packet
libv.net: compression lib (fast, but not the best compression for me) https://github.com/google/snappy
libv.range: view::input_pass resumable range pass: auto pass = range | view::input_pass; for (auto i : pass) fooA(i) if (i % 2) break; for (auto i : pass) fooB(i);
libv.sig: merge back and place meta (too many tamplate argument) into libv.meta, (or dont, please, it is too many template)
libv.ui.layout: think layout as a graph instead of a stack..., just think and see whats going on with that approach
libv.ui.lua: https://www.wowace.com/projects/ace3/pages/ace-gui-3-0-widgets
libv.ui: (shader) Program Descriptor: program is defined by a descriptor (which can be identified with a simple string key), this could also be applied for the rest of the resources
libv.ui: https://www.factorio.com/blog/post/fff-246
libv.ui: strong constraint: It has to keep up with 1000 character/sec input in mid sequence. Why? Because the 7.5cps is a reasonable high typing speed.
libv.utility: Make a proper copy-pastable noisy type
libv: LIBV_ASSERT, LIBV_DEBUG_ASSERT, LIBV_STATIC_ASSERT in utility header
libv: think about versioned namespace: namespace LIBV_NAMESPACE_VERSION { ... content ... } namespace libv = LIBV_NAMESPACE_VERSION

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

libv: generic general binary data storage
		 Name               | Implementation                   | Capacity | Size    | SBO | Heap
		--------------------+----------------------------------+----------+---------+-----+---------
		byte_array<N>       | std::array<byte>                 | N        | N       |  N  | never
		byte_static<N>      | std::array<byte>, size_t         | N        | dynamic |  N  | never
		byte_small<N>       | boost::small_vector<byte, N>     | dynamic  | dynamic |  N  | above N
		byte_dynamic        | std::vector<byte>                | dynamic  | dynamic |  -  | above 0
		byte_cold           | byte*                            | fixed    | fixed   |  -  | above 0
		byte_very_cold      | int32_t (index to global byte**) | fixed    | fixed   |  -  | above 0

		binary<-1>          | binary_dynamic
		binary<0+>          | binary_array

libv: generic general container storage
		 Name                    | Implementation            | Capacity | Size    | SBO | Heap
		-------------------------+---------------------------+----------+---------+-----+---------
		vector_static<T..., N>   | std::array<T, N>          | N        | N       |  N  | never
		vector_local<T..., N>    | std::array<T, N>, size_t  | N        | dynamic |  N  | never
		vector_small<T..., N>    | boost::small_vector<T, N> | dynamic  | dynamic |  N  | above N
		vector_dynamic<T...>     | std::vector<T>            | dynamic  | dynamic |  -  | above 0
		vector_cold<T...>        | T*                        | fixed    | fixed   |  -  | above 0
		vector_very_cold<T...>   | int32_t (+ global T**)    | fixed    | fixed   |  -  | above 0

		vector<static<N>, T...>  | std::array<T, N>          | N        | N       |  N  | never
		vector<local<N>, T...>   | std::array<T, N>, size_t  | N        | dynamic |  N  | never
		vector<small<N>, T...>   | boost::small_vector<T, N> | dynamic  | dynamic |  N  | above N
		vector<dynamic<N>, T...> | std::vector<T>            | dynamic  | dynamic |  -  | above 0
		vector<cold, T...>       | T*                        | fixed    | fixed   |  -  | above 0
		vector<very_cold, T...>  | int32_t (+ global T**)    | fixed    | fixed   |  -  | above 0

--- NOTE -------------------------------------------------------------------------------------------

app: for apps you can cd next to the binary to solve any relative path issue (command line arguments should be handled beforehand)

--- ABANDONED --------------------------------------------------------------------------------------

logger: binlog - https://www.youtube.com/watch?v=FyJI4Z6jD4w
logger: client - network connected different app (real time log viewer) with retrospective and real-time filtering and stuff...
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

GLFW Update:

glfw.core: Added glfwGetError function for querying the last error code and its description
glfw.core: Added glfwInitHint for setting initialization hints
glfw.core: Added glfwSetErrorCallback, GLFWerrorfun and error type tokens for receiving error notifications
glfw.core: Added glfwSetMonitorUserPointer and glfwGetMonitorUserPointer for per-monitor user pointers

glfw.frame: Added glfwCreateCursor, glfwCreateStandardCursor, glfwDestroyCursor and glfwSetCursor for managing system cursor images
glfw.frame: Added glfwGetWindowOpacity and glfwSetWindowOpacity for controlling whole window transparency (#1089)

glfw.input: Added glfwRawMouseMotionSupported function for querying raw motion support (glfwRawMouseMotionSupported must be checked)
glfw.input: Added GLFW_RAW_MOUSE_MOTION input mode for selecting raw motion input (#125,#1400,#1401)

glfw.joystick: Added glfwGetGamepadName function for querying the name provided by the gamepad mapping (#900)
glfw.joystick: Added glfwGetGamepadState function, GLFW_GAMEPAD_* and GLFWgamepadstate for retrieving gamepad input state (#900)
glfw.joystick: Added glfwGetJoystickGUID function for querying the SDL compatible GUID of a joystick (#900)
glfw.joystick: Added glfwGetJoystickHats function for querying joystick hats (#889,#906,#934)
glfw.joystick: Added glfwJoystickIsGamepad function for querying whether a joystick has a gamepad mapping
glfw.joystick: Added glfwSetJoystickCallback for joystick connection and disconnection events
glfw.joystick: Added glfwSetJoystickUserPointer and glfwGetJoystickUserPointer for per-joystick user pointers
glfw.joystick: Added glfwUpdateGamepadMappings function for importing gamepad mappings in SDL_GameControllerDB format

glfw.sample: Added 'cursor' simple cursor test programs
glfw.sample: Added 'empty' test program for verifying posting of empty events
glfw.sample: Added 'modes' video mode enumeration and setting test program
glfw.sample: Added 'sharing' simple OpenGL object sharing test program
glfw.sample: Added 'threads' simple multi-threaded rendering test program
glfw.sample: Added 'windows' simple multi-window test program

glfw: glfwSetMonitorUserPointer and glfwGetMonitorUserPointer
glfw: glfwSetGammaRamp

// -------------------------------------------------------------------------------------------------

I dont know yet what it could be used for, but this is could be VERY handy!
sfinae + decltype + operator ,
decltype(os << obj, void())
yeee i get it now: void to regular void by: "(void_expression, regular_void)" and operator,(T, regular_void)

// -------------------------------------------------------------------------------------------------

Base class for lights and cameras

http://www.cmake.org/cmake/help/v3.3/command/configure_file.html

Optimized compiling - cmake https://github.com/sakra/cotire

CMake resource folder
Cube / Sky Textures http://sourceforge.net/projects/spacescape/

http://ogldev.atspace.co.uk/www/tutorial43/tutorial43.html
OpenGL Reference page: https://www.khronos.org/registry/OpenGL-Refpages/gl4/

Shader types
	Fresnel shader - Atmosphere
	Cook-Torrance shader - Metal
	Minnaert - More depth?
	OrenNayar - More avg lambert

--- PASTEBIN ---------------------------------------------------------------------------------------

diffuse(1.0f, 1.0f, 1.0f, 1.0f),
specular(0.8f, 0.8f, 0.8f, 1.0f),
emission(0.0f, 0.0f, 0.0f, 1.0f),
ambient(0.1f, 0.1f, 0.1f, 1.0f),
reflective(1.0f, 1.0f, 1.0f, 1.0f),
shininess(32.0f) { }

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
	DEFINE -D "WISH_REQUIRES\\(...\\)=\"requires __VA_ARGS__\""
	IDE_DEFINE WISH_REQUIRES(...)

	DEFINE -DWISH_GIT_BRANCH="${WISH_GIT_BRANCH}"
	DEFINE -DWISH_GIT_COMMIT_HASH="${WISH_GIT_COMMIT_HASH}"
	DEFINE -DWISH_SHORT_PATH_CUTOFF=${WISH_SHORT_PATH_CUTOFF}
	DEFINE -DWISH_SHORT_PATH_PREFIX="${CMAKE_SOURCE_DIR}/"
)

wish_create_library(
	PARENT libv
	TARGET vm4imp
	LINK PUBLIC libv::log libv::vm4 ext::assimp
)

// -------------------------------------------------------------------------------------------------

local nato_phonetic_alphabet = {
	"Alfa",
	"Bravo",
	"Charlie",
	"Delta",
	"Echo",
	"Foxtrot",
	"Golf",
	"Hotel",
	"India",
	"Juliet",
	"Kilo",
	"Lima",
	"Mike",
	"November",
	"Oscar",
	"Papa",
	"Quebec",
	"Romeo",
	"Sierra",
	"Tango",
	"Uniform",
	"Victor",
	"Whisky",
	"X-ray",
	"Yankee",
	"Zulu",
}

local greek_alphabet = {
	"Alpha",
	"Beta",
	"Gamma",
	"Delta",
	"Epsilon",
	"Zeta",
	"Eta",
	"Theta",
	"Iota",
	"Kappa",
	"Lambda",
	"Mu",
	"Nu",
	"Xi",
	"Omicron",
	"Pi",
	"Rho",
	"Sigma",
	"Tau",
	"Upsilon",
	"Phi",
	"Chi",
	"Psi",
	"Omega",
}
