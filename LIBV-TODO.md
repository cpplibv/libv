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
app.vm4_viewer: single directional light
app.vm4_viewer: show model grey lighted (phong)
app.vm4_viewer: single point light
app.vm4_viewer: single spot light
libv.glr: general function callback and remote bypass / callback with gl state
app.vm4_viewer: shader manager bypass libv.glr to access libv.gl
libv.glr: program adopt and swap with libv.gl
app.vm4_viewer: shader include base directory
app.vm4_viewer: shader loader
app.vm4_viewer: do not change shader if shader load/compile/link failed
app.vm4_viewer: throw out old shader code, rename shader2 to shader
app.vm4_viewer: shader should sub for every included file
app.vm4_viewer: shader should unsub from file watcher, ATM there is an issue during program termination
app.vm4_viewer: solve 3 line of error while program reloading happens
app.vm4_viewer: cleanup cpp shader codes
app.vm4_viewer: ui feedback for glsl shader error, big red text
libv.hotkey: Rename to libv.control
libv.control: design API | multiple sub-system: normalization, routing, binding, conversion, action, context, presentation
libv.control: context template static cast void* trick to set new context payload
libv.control.sandbox: start a sandbox, compile a null operation
libv.control.sandbox: compile/implement/run API front of feature_action
libv.control.sandbox: compile/implement/run API front of feature_binary
libv.control.sandbox: compile/implement/run API front of feature_analog
libv.control: auto promote input to combination on demand in every context (preferably with overloads)
libv.control: auto promote input to sequence on demand in every context (preferably with overloads)
libv.control: auto promote combination to sequence on demand in every context (preferably with overloads)
libv.control.sandbox: compile/implement/run API front of binding
libv.control.sandbox: compile/implement/run API front of keycode/scancode input
libv.control: Build internal SOW from input events
libv.control.sandbox: compile/implement/run API front of update
libv.control: InputID as_enum member function helpers, renaming of members getters
libv.control: to_string for InputID
libv.control: to_string for InputID enum members
libv.control: change "--invalid--" to "<<invalid>>" and "<?>"
libv.control: Move InputID tests to a test format
libv.control: Improve InputID parse to handle mouse half dimension
libv.control: Parsing joystick analog
libv.control: Parsing joystick button
libv.control: InputID Gamepad
libv.control: Explicit Gamepad support
libv.control: Gamepad support (except SOW)
libv.control: Gamepad any support
libv.control: Joystick any support
libv.control: Parsing gamepad analog
libv.input: Move Keycode parsing tests to libv.control and remove key parsing from libv.input
libv.control: Gamepad SOW support
libv.control: rename parse_input_sequence files
libv.control: AnalogInputAction is not present in Input | invalid as it is present in inputID
libv.control.parse: Active tests for keycode parsing
libv.control: InputID alias matcher
libv.control: InputID alias matcher tests
libv.control: Routing: Every DIA x button
libv.control: Routing: Every DIA x time
libv.control: Routing: add binding states
libv.control: design normalization layer
libv.control: design routing layer
libv.control: design binding and conversion layer
libv.control: design context layer
libv.control: design feature layer
libv.control: Routing: maintain binding states
libv.control: Routing combination of buttons
libv.control.parse: Parsing gamepad button
libv.control.parse: Test parsing invalid gamepad analog
libv.control.parse: Test parsing invalid gamepad button
libv.control: Routing issue: sequence checkpoint is uselessly too weak, "Ctrl+A, A" the first "A" release resets the checkpoint on the second step | solved by only reseting sequence from the last step
libv.control: Button input processing
libv.control: design presentation layer
libv.control.parse: Input parsing
libv.control.parse: Combination parsing
libv.control.parse: Sequence parsing
libv.control: Function to verify sequence rules
libv.control.parse: Combination timeout parser | OR cut out combination timeout | choose latter | Invalidated
libv.control: cut out combination timeout, make a global sequence timeout
libv.control: Only Control level  sequence timeout, no per combination
libv.control: libv/control/input.hpp is wrong name, it should be enum or someting like event enums, and Input class should have this header
libv.control: implement rest of bind functions, call to parser
libv.input: Add support for joysticks and game-pads
libv.control: Sequence normalization
libv.control: Sequence alias normalization
libv.control: List binding fail conditions
libv.control: Report binding errors, and fail if needed
libv.control: Implement Controls::bind outcomes
libv.control: Controls::bind and feature functions are not using ctors
libv.control: Routing: Analog input processing
libv.control: Routing: Time input processing
libv.control: Routing: feature gather, and broadcast in step 2
libv.control: Implement codepoint mapping
libv.control: Routing: scancode
libv.control: Routing: codepoint
libv.control: Control level sequence timeout settings
libv.control: Idea: sequence could mandate by rule to not have any funny dia for non last, hmmm, it should, even if I dont utalize this while coding... its really should. | verify does this, but verify is not yet really used
libv.control: Idea: move dia from input to combination | what would it solve? | nothing, invalidated
libv.control: AIA inside input ID is not a proper fit (!) blocking Analog input processing | I might be able to just ignore this one by two parameter | ignored with two parameter
libv.control: scale and impulse settings for each input source (and for global)
libv.control: Determine the correct scales, how each scale-er effect the final value (for simple and sequence case)
libv.control: mouse is not diff but position based atm I think
libv.control: mouse px is bad, use value normalized to shortest window dimension | both are needed: cannot change mouse sensitivity based on window or interactive area size without concent | mouse device px speed is the best solution | mouse px is ok if scaled
libv.control: mouse hardcoded scale
libv.control: Bind could use str instead of str_view, same goes for feature
libv.control: cleanup or remove input_table.cpp/hpp and input_id.cpp | input_id.cpp stays
libv.control: "2x4" to_string for sequence and combination | only name and symbol
libv.control: Query if a binding has actually feature behind it
libv.control: Feature register proxy
libv.control: Feature register, a limited API view that only allows registering features
libv.control: Feature auto-register solution to client global
libv.control: Auto feature register, without the backed in global variable into the lib
libv.control: Feature hash storage
libv.control: Sequence builder | invalidated
libv.control: Implement codepoint mapping observation
libv.control: What is the format for IO? Json? Lua? XML? Text? | Some very rigid text for now
libv.control: A way to quickly save and load bindings, aka binding serialization | Very plain text, bit painful
libv.control: Design persistence layer
libv.control: A way to quickly save and load settings: impulse, scale and timeout
libv.control.parse: Automated test for every input_id: symbol, identifier and name to be parsable | not automated, but a decent test, automation would have not saved much time
ext.x3: detect end of input, control and other parsers could benefit | x3::eoi
libv.control.parse: Mouse scroll without dimension should default to Y, update symbol table and parser
libv.control.parse: Mouse scroll should not require word_mouse | only if long names or mouse word are used so its not ambiguous
libv.control.parse: InputID scroll parsing
libv.control.parse: InputID parsing
libv.control: reorganize files
libv.control: Remove unused/commented codes
libv.control: Cleanup first lines in every source file
libv.control: Routing: Implement deadkey set single decision point
libv.control: Routing: Sequences dead-key like behaviour
libv.control: Routing: Idea: sequence can only progress to the next step if the current step is fully dissatisfied (maybe with the only exception to modifier keys, or a special flaggy thingy handwavey thing around dia); This would be the Netbeans way, but that does not allows holding down the modifiers and pressing keys and clion allows that | The Clion way is better
libv.control: Routing: Time timeout sequences
libv.control: Implement unbinding
libv.control: Cleanup binding_outcome and unbinding_outcome
libv.control: Binding storage (and active_bindings ptr stability)
libv.control: Routing: Select bindings based on more specialized
libv.control: Routing
libv.control: Scale in analog member, they should be scale / time / impulse, and i should use them
libv.control: Routing: record scale for entering active bindings | invalidated: killed button scales
libv.control: Determine the correct scales for multi type combinations | invalidated: killed button scales
libv.control: Scale implemented up to process functions, implement inside process and think about fullfilled binding
libv.control: List (into the paper) what is: global/per source/per binding/per feature setting
libv.control: Configuration and settings
libv.control: Remove debug trace messages, CONTROL_DEBUG, iostream, etc...
libv.control: 2 (+ 1) Example sandbox to presented in the paper
libv.control: Rename libv.control to libv.ctrl



--- STACK ------------------------------------------------------------------------------------------


libv.ui: Remove mask_watchMouse in favor of a single bool flag as mouse movement determines the other event targets
idea: For UI 2D picker use the mouse wheel with indication beside the cursor to select underlying components

ext.x3: currently on debug build x3 parser in libv.ctrl fails with an assert, it will be the utf8 string parsing with char...

libv.ctrl: clean up visibility (especially for Sequence and StateSequence and related types) cleanup distant member accesses (from control)
libv.ctrl: introspection API should not see stated types | info type proxies | maybe even pimpl
libv.ctrl: Binding binary operation setting and serialization

libv.ctrl: InputID, Input, AIA and DIA should be merged this way: both Input and InputID should be 64bit types, but one should contain the action related information, they should be converted to one another with extra info or info loss, inheritance or CRTP might save some work

libv.ctrl: More specialized binding can hide normal feature if there is not context under it -> the order of context existence check and binding specialization test should be flipped

libv.math: Add TAU constant as PI * 2

Sequence: dead-key like behaviour
		process analog will need to differentiate between mouse and analog, but that is already needed for, or not, at least not yet, something got cancelled with this one

		if there are dead-keys active
			only consider dead-key set bindings
		otherwise
			consider every binding

		if a combination got fulfilled
			if it was a sequence's non-last combination or if it was not a sequence
				collect into normal list
			else
				collect into ongoing_sequence list

		select and filter (normal list AND ongoing_sequence list)
		remove elements from ongoing_sequence that were not selected
		broadcast among selected normal list


		if (not ongoing_sequence.empty())
			store ongoing_sequence as dead-key set and therefore enter dead-key mode


		when to cancel a dead-key situation
		if we are in a dead-key situation and any of the following occurs
				event: that is not part of any new combination in dead-key set
				event: not an opposite or holding of the previous history step of dead-key set
				on time: sequence_timeout
			then cancel dead-key situation by clearing the dead-key set and discarding the event




libv.ctrl:
	TODO notes:
		Issue: Binding input sub sequence collusion
			Given 'Shift + E' and 'E' binding, inputting 'Shift + E' will incorrectly satisfy both
			Mistake:
				Assumption: Inputs are serial
				Reality: Input might be parallel (Throw grenade with Shift + F while WASD active)
				Additional note: Multi combination inputs are expected to be serial at all times.
				Additional note: Due to this, heuristic might be acceptable?

			Solution A: (heuristic) prefer the more specialized bindings (? based on setting of the binding) | causes issues with non valid shortcuts matching a smaller one
			Solution B: (at most internally) specify as the E version as not pressed: Shift [free] + E
			Solution C: modifier bits, but that would introduce the inability to define custom modifiers
			Solution D: per binding exclusivity definition:
				- yield toward more specialized
				- require exclusivity
				- ignore independent
				- allow non-interfering independent changes
				- allow non-interleaving independent states
				- ...

			Solution placement 1: controls settings
			Solution placement 2: per binding
			Solution placement 3: per feature

				Research required: both source and usage examples in multiple domain and programs
				Document findings
				Research required: feature based interruptibility matrix

			Additional note:
				Callbacks passed events could/should have an absorb flag. Binding and features could demand certain absorbs properties like only one absorb or allow subset sequences to be fired or allow parallel and so on...

		Issue: Given 'E' and 'Shift + E' binding, inputting 'E' then pressing 'Shift' will do what? And why?

		Issue: Need 2D input support (?), for example pie menu needs it too | that is UI absolute position

		Click - Press > at most X time > Release
		Double Click - Press > at most X time > Release > at most T time > Press > at most X time > Release

		sequence (definition) + binding (info) + feature (name) [+ operation] = a single record

	Outside of scope notes:
		Issue: need a way to prevent scale above 1 on certain D->A time based bindings
			Solution: just a flag for the analog feature to clamp scale to 0..1 or -1..1 before multiplying with time

		Hidden features: option to register feature as non listable, but settable
		Hidden binding: option to register a non listable binding, but still settable

		Press type aka additional information on binding (AKA A/D converter):
			N/A, Continuous (Hold), Press, Release, Double click, (Toggle), etc...
			Threshold, Scale, Repeat rules, Offset, Ramp-up

	sub-systems:
		input normalization (definition): input id and parsing
		routing                         : decision matrix
		binding and conversion          :
		context                         : injected objects
		feature                         : callbacks
		presentation                    : introspection
		persistence                     : parsers and output streams, save and load of bindings

	input sources:
		keyboard
			codepoint = codepoint resolution into keycode/scancode + warning on the UI unless it is part of the safe (A-Z0-9 and (?) more) range
			keycode
			modifier
			scancode = warning on the UI
		mouse
			button
			scroll
			move
		gamepad (and gamepad any)
			button
			analog
		joystick (and joystick any)
			button
			analog
		time
		events (API)
		context (API)

	input type (used for binding information)
		button - select state change edge
		analog (as joystick or gamepad) - linearization, deadzone
		movement - normalization px to 0..1

	input event aliasing (event identification can produce alias events):
		L/R Ctrl
		L/R Shift
		L/R Super
		L/R Alt
		Mouse X/X+/X-
		Mouse Y/Y+/Y-
		Scroll X/X+/X-
		Scroll Y/Y+/Y-
		Joystick Analog X/X+/X-
		Joystick Analog Y/Y+/Y-
		Any joystick (joystick without index)
		Any gamepad (gamepad without index)

	configuration and settings (wrong place here)
		per source type
			Analog: scale
			Button: impulse/scale

		per source (for gamepad and joystick)
			Analog: scale
			Button: impulse/scale

		per binding
			scale

		per feature
			Action: -
			Binary: binary_operation
			Analog: scale_analog/scale_binary <<< wrong

	scales:
		from event to binding
			global time/button/analog/impulse
			input type button/analog/impulse
			optionally for multi devise input types per device button/analog/impulse
			+1 hidden hard coded mouse / 600px

		determine the correct scales
		time scale/degrees/px/normalized_px/? scale
		old scales are still somewhere in this paper, cleanup them

	digital input action:
		press   - input::press
		release - input::release
		repeat  - input::repeat
		any     - maps to press, repeat, release
		hold    - event on each frame if input is 1 (held down)
		free    - event on each frame if input is 0 (released) (virtual, disabled to due logic)
		auto    - maps to press, release and hold depending on output

		tap - for <N sec                      (outside of scope of initial release) (digital)
		double tap - for <N sec within <M sec (outside of scope of initial release) (digital)
		hold - for N sec                      (outside of scope of initial release) (analog or digital)

	feature:
		types:
			action: single action - void() // single shot event
			binary: discrete binary state - void(bool state) // (accepts binary input, allows toggle)
			analog: continuous control - void(float value) //

	binding:
		connect =      what      +  where  + (     how     +    do    )
		binding = input sequence + feature + (conversation + operation)

		binding operation on action features:
			(no extra operation)

		binding operation on binary features:
			enable
			disable
			toggle
			button state
			inverted button state

		binding operation on analog features:
			scale (includes invert with negative scale)
			(?) position
			(?) difference
			(?) dimension select
			(?) dimension direction select
			(?) dimension convert
			(?) threshold
			(?) multiple analog
			(?) gamepad axis dead-zone
			(?) gamepad axis linearization

		exclusivity:
			???

		binding level:
			Primary, binding information can be used to display on HUD
			Secondary, binding information can be used to display in tutorial texts
			Hidden

	rule for input/combination/sequence:
		sequence: only the last combination can be a non action dia or aia
		combination: only one dia or aia can be an action (every aia can be considered an action, so only dia matters)
		input: dia can have these values based on what inputID is
		input: cannot have dia if inputID is analog
		input: cannot have aia if inputID is button (solved by including aia in inputID)

	context:
		string + function_ptr + type_index + void* = context
		if void* is nullptr then the context is inactive
		context iteration order shall be deterministic

		overloading: (ui.edit context can have different action sets based on focus)
				context = function_ptr + type_index + void* (without string)

	presentation:
		ability to list every feature
		ability to list every binding
		ability to list every warnings / errors
		ability to search in features / bindings
		ability to search in features / bindings based on the current state
		ability to search in features / bindings based on input sequence

	interesting usages examples, things to investigate:
		prefix collusion, especially for multi-stroke controls
		example mouse movement while holding a button

		app.editor.camera_orbit_X = Left Mouse + Mouse X
		app.editor.camera_orbit_X = W
		app.editor.camera_orbit_Z = Left Mouse + Right Mouse + Mouse Y
		app.editor.grab_manipulator_under_cursor = Left Mouse
		app.editor.move_selection_X = X + Mouse X * 2
		app.editor.selection.to_upper = Ctrl + U, U

		half control - how would walk forward and walk forward toggle look like

		circle_quick_menu and mouse position | real name found: pie menu
			app.editor.circle_quick_menu.open = Ctrl + Space
			app.editor.circle_quick_menu.select = Click
			app.editor.circle_quick_menu.select = Space
			app.editor.circle_quick_menu.switch = Mouse // How would this one work?
			app.editor.circle_quick_menu.ccw = Up
			app.editor.circle_quick_menu.ccw = Left
			app.editor.circle_quick_menu.cw = Down
			app.editor.circle_quick_menu.cw = Right

		right click to move command, long right click to move command with final direction/shape

		granade wind and granade throw jó lenne kettő G [press] + G [release] helyett G [auto]
			aka ez egy feature type lesz amivel az auto jól müködik együtt

	NOTE:
		32 context add / tick
		32 context remove / tick
		64 active context
		64000 mouse position / tick (max if mouse position is rasterized, should be less with segment intersection)

	Observations and research:
		... in the xls


libv.ctrl: If I have enough time - IIHET
	libv.ctrl: IIHET: Binding profiles, and inheritance
	libv.ctrl: IIHET: Gamepad/joystick analog should be time related, but mouse and scroll are event based
	libv.ctrl: IIHET: Click and double click support would be nice
	libv.ctrl: IIHET: User should not be exposed to stated sequence (binding does that)
	libv.ctrl: IIHET: Idea: Stated and normal sequence common templated base class (?)
	libv.ctrl.parse: IIHET: identifier parsing (_ skipper or adjusting infos to match current parser)
	libv.ctrl: IIHET: Routing: Select sequence progression (beside bindings) based on more specialized

libv.ctrl: Integration queue:
	libv.frame.input: Add support for joysticks and game-pads

	libv.ctrl: review glfwGetKeyName http://www.glfw.org/docs/latest/group__keys.html
	libv.ctrl: review glfwSetInputMode http://www.glfw.org/docs/latest/group__keys.html
	libv.frame.input: Added glfwGetKeyName for querying the layout-specific name of printable keys
	libv.frame.input: Added glfwGetKeyScancode function that allows retrieving platform dependent scancodes for keys (#830)

	glfw.joystick: Added glfwGetGamepadName function for querying the name provided by the gamepad mapping (#900)
	glfw.joystick: Added glfwGetGamepadState function, GLFW_GAMEPAD_* and GLFWgamepadstate for retrieving gamepad input state (#900)
	glfw.joystick: Added glfwGetJoystickGUID function for querying the SDL compatible GUID of a joystick (#900)
	glfw.joystick: Added glfwGetJoystickHats function for querying joystick hats (#889,#906,#934)
	glfw.joystick: Added glfwJoystickIsGamepad function for querying whether a joystick has a gamepad mapping
	glfw.joystick: Added glfwSetJoystickCallback for joystick connection and disconnection events
	glfw.joystick: Added glfwSetJoystickUserPointer and glfwGetJoystickUserPointer for per-joystick user pointers
	glfw.joystick: Added glfwUpdateGamepadMappings function for importing gamepad mappings in SDL_GameControllerDB format

libv.ctrl: Outside of paper scope - OOPS, but must implement
	libv.ctrl.routing: OOPS: Unrelated analog event cancels button sequence
	libv.ctrl: OOPS: Feature collection is collecting empty functions too
	libv.ctrl: OOPS: Would be nice to merge keycode-codepoint-scancode events to a single pass | Its must due to sequence cancellation

libv.ctrl: Outside of paper scope - OOPS
	libv.ctrl: OOPS: Split analog dimension to dimension and aia
	libv.ctrl: OOPS: Clamped analog (feature or control type that can only be 0..1 in a 1 sec time frame)
	libv.ctrl: OOPS: Feature middle ground is missing: Input > Binding > Feature > Feature Instance > callback
					In other word: an additional type between the current binding and feature, this would clean up string operations
	libv.ctrl: OOPS: Contexts information, state collusion information among contexts, use that for queries
	libv.ctrl: OOPS: Idea: Analog/Action fulfilment hold satisfied for small time | SOLVED BY MOVING ANALOGS TO TIME BASED (also multi analog combination is no longer a problem)
	libv.ctrl.parse: OOPS: Quirk: Combination parsing bug on '+'+' would mean reverse order | Forward parsing would solve it
	libv.ctrl.parse: OOPS: Quirk: Symbol ' has to contain an extra space because parsing error around + and ,
	libv.ctrl.parse: OOPS: x3::eoi could improve matching, relax orders, optimize for most probably input
	libv.ctrl.parse: OOPS: Quirk: word_plus and word_minus could have p/m due to the forbidden reverse order in mouse movement, test cases are required
	libv.ctrl: OOPS: DigitalInputAction/(?)AnalogInputAction could be folded into InputID's reserved 8 bit, aliasing and specialization would have to follow | this would also fold InputID into Input
	libv.ctrl: OOPS: Binding two separate additional information form | This is important, this is the core solution!: Input based, Feature based (and operation based)
	libv.ctrl: OOPS: event/feature chain interruptibility on colliding bindings:
						add member to event: is_ambiguous
						add member to event: is_consumed and consume (or whatever it is called in ui similar event pattern)
	libv.ctrl: OOPS: Invalid sequence automated fixup | only if there is consent
	libv.ctrl: OOPS: In parsing Gamepad Button-s make word_button optional, maybe make gamepad selection more distinct
	libv.ctrl: OOPS: In parsing Joystick Button-s make word_button optional if possible, maybe make joystick selection more distinct
	libv.ctrl: OOPS: binding levels should be grouped primaries for keyboard or gamepad, also it need a way to switch them, maybe assign group or groups to bindings
	libv.ctrl: OOPS: feature registration show provide way for custom properties
	libv.ctrl: OOPS: hash based binding index for event where manually map everything to the most similar form
	libv.ctrl: OOPS: why can I not x3::attr InputID directly (it just default constructs it and drop the value on the floor)
	libv.ctrl: OOPS: More introspection is possible: search feature by context with or without name
	libv.ctrl: OOPS: More introspection is possible: search binding with feature and sequence
	libv.ctrl: OOPS: More search possible: search_mode::wildcard (with . as the level separator)
	libv.ctrl: OOPS: More search possible: search_mode::any_word (with space as the word separator)
	libv.ctrl: OOPS: More search possible: search_mode::all_word (with space as the word separator)
	libv.ctrl: OOPS: Super flatten sequence and stated sequence
	libv.ctrl: OOPS: check_binding and check_feature are algorithm that should be generalized
	libv.ctrl: OOPS: Change in codepoint mapping can cause events with codepoint based inputs

	libv.ctrl.profile: OOPS: each binding entry would select: inherit (noop) / override / extend its parent binding entry

	libv.ctrl: OOPS: Idea: why not require codepoints to be inside the inputs events
	libv.ctrl: OOPS: Idea: dia as bitmask
	libv.ctrl: OOPS: Rep source type could be signed and analogs on negative, buttons on positive values
	libv.ctrl: OOPS: Ability to list every (active) context
	libv.ctrl: OOPS: Bind and profile api and serialization: override, empty override, append
	libv.ctrl: OOPS: A serialization... A serious one
	libv.ctrl: OOPS: Solve FeatureRegister code duplication with inheritance (FeatureRegister even could be a base of Controls)

	libv.ctrl: OOPS: libv::ctrl -> libv::ctrl
	libv.ctrl: OOPS: Context level feature containers, or index so lookup faster (?)
	libv.ctrl: OOPS: Store more information about Context. Name, orthogonal and concurrent contexts
	libv.ctrl: OOPS: Idea of context groups, and under a group there are multiple types

visual.grid: Make a pretty grid: Blender grid is awesome. Implement it:
visual.grid: For grids expand and fade out line endings
visual.grid: For grids fade out lines based on ray incident angle, lines should fade into the sky-box as seamlessly as possible
visual.grid: For grids fade out small lines based on distance from the camera and make higher grid lines more visible
visual.editor_bg: Non uniform color, use a grainy texture (like blue noise)
cpp: move from to holds_alternative to get_if https://en.cppreference.com/w/cpp/utility/variant/get_if

app.vm4_viewer: Camera controller class
app.vm4_viewer: Camera controller class jump controls with multiple interpolation type (linear camera motion is sickening, jumping camera movement/rotation should always ease out and in, in case of very long jumps skip in the middle segment)

app.vm4_viewer: light source indicator
app.vm4_viewer: camera pivot indicator
app.vm4_viewer: camera orientation indicator
app.vm4_viewer: ortho camera, and swapping

app.vm4_viewer: Camera controller class with lua binding

libv.input: rename EventKey::key to EventKey::keycode (?)
libv.ui: MUST! Hide component memory management: sp<Label> -> Label = magic<ImplLabel> | AND move component creation to attach time -> that removes the ctor overhead, but add the issue that there can be no ctor arg passed
libv.ui: MUST! Hide component parent management: thread local variable to hide UI* context in it?
libv.ui: Remove mask_watchMouse in favor of a single bool flag as mouse movement determines the other event targets

libv.ui: move x3 parse rules to globals with internal linkage to improve performance BUT ! static initialization order fiasco
libv.parse: move x3 parse rules to globals with internal linkage to improve performance BUT ! static initialization order fiasco

app.vm4_viewer: Camera controller class with lua binding

libv.ui: Remove mask_watchMouse in favor of a single bool flag as mouse movement determines the other event targets

app.vm4_viewer: light source mover

app.vm4_viewer: Rulers: Display size rulers along the 3 dimension (3/6/9 value) of the max ranges

app.vm4_viewer: Run time (and config time) options for debug tool toggles, or overlays: ui toggle buttons
app.vm4_viewer: Add config option to auto open shader on errors: /netbeans/bin/netbeans64.exe --open {file}:{line}
app.vm4_viewer: Add config option to disable reset camera on model loading
app.vm4_viewer: Add config option for AABB
app.vm4_viewer: Add config option for BS
app.vm4_viewer: Add config option for grid-XY
app.vm4_viewer: Add config option to not reset camera on model change

libv.args: support or extension for the lib to support command line "late" commands, from a different process instance with a network hook
app.vm4_viewer: Command line argument --open "file" to auto open file
app.vm4_viewer: Command line argument --active open file in already running instance (overrides config option)


libv.ui: UI level message/event bus/system might be required:
		context().events().connect<ShaderReportFailure>(this, "shader_reload", [](const auto& report){ ... });
		context().events().fire("shader_reload", ...);
libv.ui: UI level message/event bus/system could use support for up-walking context iteration
libv.ui: UI level storage system
		context().storage<UIUserConfig>() : UIUserConfig&
libv.ui: UI based file watcher, libv.fsw > queue > ui loop event stage > broadcast

app.bin_to_src: libv.arg-ify
app.bin_to_src: command line argument for line length
app.bin_to_src: use libv::span<const std::byte>
app.bin_to_src: use // <editor-fold defaultstate="collapsed" desc="Binary data ..."> and // </editor-fold>

libv: Update every file first lines with: // Project: LIBV, Author: Császár Mátyás [Vader], Email: vaderhun@gmail.com

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


gl: learn https://www.khronos.org/opengl/wiki/Image_Load_Store

libv.gl.glsl: Warning for abs          include path (path normal form start with root)
libv.gl.glsl: Warning for non base-abs include path (path normal form start with ..)
libv.gl.glsl: Warning for non generic directory separator in include path (backlash)
libv.gl.glsl: Warning option for mixed indentation in different lines and/or files
libv.gl.glsl: Warning for line trailing white space

libv.ui: relative - mouse event should contain a watcher relative (local) coordinates too
app.vm4_viewer: camera movement should acquire and lock mouse position

libv.ui: read nana 3rd party property tree lib API

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
libv.ui: non-shared_ptr based panels, aka static_container (?)
app.vm4_viewer: display statistics

app.vm4_viewer: import model
app.vm4_viewer: Add project level size comparison functionality
app.vm4_viewer: Thumbnails and thumbnail generation
app.vm4_viewer: Better focus camera based on the actual view angles and model ineast of the BSO

app.vm4_viewer.shader: uniform (sun) auto iteration and name auto chaining
app.vm4_viewer.shader: ui feedback for glsl shader reload success
app.vm4_viewer.shader: ui feedback for glsl shader failed include
app.vm4_viewer.shader: use a fallback shader on init failure
app.vm4_viewer.shader: do not notify success on first shader init, it is not needed

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


libv.utility: Implement a proper match file iterator "dir/part*.cpp", possibly with filesystem + ranges | use wildcard functions, but split pattern / match for performance | design API allow async/iterative (give next N passed entry or M failed (not matched) entry)

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
wish: add a dummy.cpp to targets that would require at least one source file but GLOB was empty

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

libv.fsw: Improve callback and tokens to not hold the mutex during event broadcast (callback), currently it is a deadlock

libv.arg: recruit
		(1) Tegye fel a kezét aki unja már, hogy nincsen értelmes argument parsing lib C++-hoz
		(2) Tegye fel a kezét aki n
		 ha van egy jó lib ötletem ami alkalmas lenne CPP FTW teamworkre
		(Who would have guessed... Vader has a lib idea...)

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
math: blue noise, a stable uniform noise https://gist.github.com/pixelmager/5d25fa32987273b9608a2d2c6cc74bfa
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
bash: To repleace first line in files: sed -i '1s/.*/\/\/ Created on 2020.05.01. 10:30, Author: Vader/' *

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

IDEA: NamedUniform_t + reflection

template <libv::static_string Name, typename Type>
struct NamedUniform_t : Uniform_t<Type> {
	static constexpr inline libv::static_string name = Name;
};

struct UniformsShaderLine {
	app::Uniform_vec4f<"color"> uniform_color;
	app::Uniform_mat4f<"matMVP"> uniform_matMVP;
};

// -------------------------------------------------------------------------------------------------

libv::gl::Uniform_vec3f cameraPositionW;
libv::gl::Uniform_mat4f matrixMVP;
libv::gl::Uniform_mat4f matrixModel;
libv::gl::Uniform_mat4f matrixModelView;
libv::gl::Uniform_mat4f matrixProjection;
libv::gl::Uniform_mat4f matrixView;
libv::gl::Uniform_vec2f mousePositionH; // Not sure about this one
libv::gl::Uniform_vec2f mousePositionS; // Not sure about this one
libv::gl::Uniform_vec2f resolution;
libv::gl::Uniform_float timeReal;
libv::gl::Uniform_float timeSimulation;

void assignBaseUniforms() {
	program.assign(cameraPositionW, "cameraPositionW");
	program.assign(matrixMVP, "matrixMVP");
	program.assign(matrixModel, "matrixModel");
	program.assign(matrixModelView, "matrixModelView");
	program.assign(matrixProjection, "matrixProjection");
	program.assign(matrixView, "matrixView");
	program.assign(mousePositionH, "mousePositionH");
	program.assign(mousePositionS, "mousePositionS");
	program.assign(resolution, "resolution");
	program.assign(timeReal, "timeReal");
	program.assign(timeSimulation, "timeSimulation");
}

void use(Queue& queue) {
	queue.program(program);

	if (matrixMVP.isActive())
		queue.uniform(uniformMatrixMVP, mvp());
	if (matrixModel.isActive())
		queue.uniform(uniformMatrixModel, model);
	if (matrixModelView.isActive())
		queue.uniform(uniformMatrixModelView, view * model);
	if (matrixProjection.isActive())
		queue.uniform(uniformMatrixProjection, projection);
	if (matrixView.isActive())
		queue.uniform(uniformMatrixView, view);

	if (cameraPositionW.isActive())
		queue.uniform(uniformCameraPositionW, eye());

	if (timeReal.isActive())
		queue.uniform(uniformTimeReal, timeFrame);
	if (timeSimulation.isActive())
		queue.uniform(uniformTimeSimulation, timeSimulation);

	if (mousePositionH.isActive())
		queue.uniform(uniformMousePositionH, mousePosition / resolution);
	if (mousePositionS.isActive())
		queue.uniform(uniformMousePositionS, mousePosition);
	if (resolution.isActive())
		queue.uniform(uniformResolution, resolution);
}

// -------------------------------------------------------------------------------------------------

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

Additional cmake.wish ideas 2:

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

	MODE DEBUG
		OPTION -Og
		OPTION -ggdb3
	MODE RELEASE
		OPTION -O3
		OPTION -static
	MODE DEV
		OPTION -O3

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

Additional lua.wish ideas:


debug = mode("debug")
release = mode("release")
dev = mode("dev")

config.target = "{PARENT_TARGET}_{TARGET}"
config.folder = "{PARENT_FOLDER}/{TARGET}"
config.alias  = "{PARENT_ALIAS}::{TARGET}"
config.source = "src/{FOLDER}/**.cpp"
config.header = "src/{FOLDER}/**.hpp"

config.result_path_binary_app      = "bin/{FOLDER}/"
config.result_path_binary_example  = "bin/{FOLDER}/"
config.result_path_binary_sandbox  = "bin/{FOLDER}/"
config.result_path_binary_test     = "bin/{FOLDER}/"
config.result_path_header          = "include/{FOLDER}/"
config.result_path_library_dynamic = "lib/{FOLDER}/"
config.result_path_library_static  = "lib/{FOLDER}/"

set.CMAKE_THREAD_PREFER_PTHREAD = "TRUE"
set.THREADS_PREFER_PTHREAD_FLAG = "TRUE"

find.package_require("Threads")
find.package_require("OpenGL")


ext_assimp = create_external("assimp", "https://github.com/assimp/assimp.git", "v5.0.0", {
	BUILD_SHARED_LIBS = OFF,
	CMAKE_EXPORT_COMPILE_COMMANDS = ON,
	ASSIMP_BUILD_ASSIMP_TOOLS = OFF,
	ASSIMP_BUILD_TESTS = OFF,
	ASSIMP_BUILD_ZLIB = ON,
	ASSIMP_NO_EXPORT = ON,

	ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT = OFF,
	ASSIMP_BUILD_FBX_IMPORTER = ON,
})
ext_assimp.link_public("assimp", "IrrXML", "zlibstatic")


libv = create_base("libv")

libv.warning("GNU VERSION_GREATER 7.0", "-Wduplicated-branches")
libv.warning("GNU VERSION_GREATER 7.0", "-Wrestrict")
libv.warning("GNU VERSION_GREATER 7.0", "-Wshadow-compatible-local")
libv.warning("GNU VERSION_GREATER 8.0", "-Wcast-align=strict")
libv.warning("GNU VERSION_GREATER 8.0", "-Wmultistatement-macros")

libv.option(debug, "-Og")
libv.option(debug, "--ggdb3")
libv.option(release, "-O3")
libv.option(release, "-static")
libv.option(dev, "-O3")
libv.option("-m64")
libv.option("-std=c++2a")
libv.option("-fconcepts")

libv.define("concept=\"concept bool\"")
libv.define("\"WISH_REQUIRES\\\\(...\\\\)=\\\"requires __VA_ARGS__\\\"\"")
libv.ide_define("WISH_REQUIRES(...)")

libv.define_wish_git_branch()
libv.define_wish_git_commit()
libv.define_wish_short_path_cutoff()
libv.define_wish_short_path_prefix()

libv_vm4imp = create_library(libv, "vm4imp");
libv_vm4imp.link_public(libv_log libv_vm4 ext_assimp);


// -------------------------------------------------------------------------------------------------

Additional lua.wish ideas 2:


debug = mode("debug")
release = mode("release")
dev = mode("dev")

set.CMAKE_THREAD_PREFER_PTHREAD = "TRUE"
set.THREADS_PREFER_PTHREAD_FLAG = "TRUE"
find.package_require("Threads")
find.package_require("OpenGL")


ext_assimp = create_external("assimp", "https://github.com/assimp/assimp.git", "v5.0.0", {
	BUILD_SHARED_LIBS = OFF,
	CMAKE_EXPORT_COMPILE_COMMANDS = ON,
	ASSIMP_BUILD_ASSIMP_TOOLS = OFF,
	ASSIMP_BUILD_TESTS = OFF,
	ASSIMP_BUILD_ZLIB = ON,
	ASSIMP_NO_EXPORT = ON,

	ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT = OFF,
	ASSIMP_BUILD_FBX_IMPORTER = ON,
})
ext_assimp.link_public("assimp", "IrrXML", "zlibstatic")


libv = create_base("libv")
libv.warning_all()

libv.option(debug, "-Og")
libv.option(debug, "--ggdb3")
libv.option(release, "-O3")
libv.option(release, "-static")
libv.option(dev, "-O3")
libv.option("-m64")
libv.option("-std=c++2a")
libv.option("-fconcepts")

libv.define("concept=\"concept bool\"")
libv.define("\"WISH_REQUIRES\\\\(...\\\\)=\\\"requires __VA_ARGS__\\\"\"")
libv.ide_define("WISH_REQUIRES(...)")

libv.define_wish_git_branch()
libv.define_wish_git_commit()
libv.define_wish_short_path_cutoff()
libv.define_wish_short_path_prefix()

libv_vm4imp = create_library(libv, "vm4imp");
libv_vm4imp.link_public(libv_log libv_vm4 ext_assimp);

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
