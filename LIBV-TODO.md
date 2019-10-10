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
libv.ui: MouseTable
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


--- STACK ------------------------------------------------------------------------------------------


libv.ui.property: hybrid reflection - dynamic



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

debug
	libv.ui: a way to debug / test / display every textures (font and other ui) | every resource
	libv.ui: ui debug view, tree display, property viewer (including property and style 'editor')

cleanup
	libv.ui: using FontSize = int16_t; -> enum class FontSize : int16_t {};
	libv.ui.font: line 179 not just log but return default on error
	libv.ui: doLayout1 should use the return channel instead of member cache
	libv.ui: remove layout1 pass member variables in component_base
	libv.ui: context_ui and libv.gl:image verify that targets are matching the requested target
	libv.ui: cleanup context_ui redundant codes
	libv.ui: mark every component type one paramed ctors as explicit (and maybe check on every ui class too)
	libv.ui: setX() / getX() -> void X(T) / T X()

mouse
	libv.ui: flatten - flatten EventMouse to combat variant complexity
	libv.ui: mouse events should consider depending on if the window is focused or not | non trivial either way, might be best to have both option dynamically
	libv.ui: absorb - mouse event absorb/shield/plates
	libv.ui: absorb - make sure absorb/shield/plates is easy to have/access for even non interactive components
	libv.ui: relative - mouse event should contain a watcher relative coordinates too
	libv.ui: unchanged - updating watcher (any property) without change should not yield any event | do I care about it?

event
	libv.ui: Every event: focus, mouse, key, char shall provide access to the entire state universe
	libv.ui: if 'everything' 'above' is done re-read the requirements of mouse events and verify if all of them are met

component
	libv.ui: atlas definition/parsing
	libv.ui.atlas: ui theme atlas loading and auto-preview, semi-auto atlas definition
	libv.ui: support atlas based images

	libv.ui: clipping vertex shader (with on/off)
	libv.ui: scroll pane | shader clip plane (scissors), (effects every ui shader) | only pane without scroll bar

	libv.ui: list
	libv.ui: table layout - only the columns and/or rows have size
	libv.ui: not owning container views (list and/or table)

ui
	libv.ui: layout padding
	libv.ui: fragments
	libv.ui: static_component system
	libv.ui: progress bar
	libv.ui: add a glr::remote& to UI to simplify app::frame
	libv.ui: component position is currently relative to origin, once 'frame' and 'scroll' component comes in this will change


--- [[[ deadline: 2019.09.30 ]]] ---

libv.ui: style sheets

debug
	libv.gl: framebuffer
	libv.glr: framebuffer
	libv.gl: renderbuffer
	libv.glr: renderbuffer
	libv.gl: blit
	libv.glr: blit
	libv.ui: debug zoom in mode (might be able to getaway with a glBlitFramebuffer, note that an extra frame buffer is required), its not strictly an ui debug feature

interactive
	libv.ui: String2D API to find nearest character position
	libv.ui: Make a sandbox for a input->button->label->list
	libv.ui: callback system for button (and other interactive component) aka: signal-slot
	libv.ui: signal-slot and event proxies, do not store a signal inside the component, use a member function event() to yield a proxy that has many signal
	libv.ui.input_field: paste support
	libv.ui.input_field: selection support
	libv.ui.input_field: copy support
	libv.ui.input_field: undo/redo support
	libv.ui.input_field: input mask (this will possibly a different input_field type)
	libv.ui.input_field: if text does not fit, crop/layer it and only display around caret
	libv.ui.input_field: if text does not fit, display a popup with full text on mouse hover and idle

hotkey
	libv.hotkey: review glfwGetKeyName and glfwSetInputMode http://www.glfw.org/docs/latest/group__keys.html
	libv.frame.input: Added glfwGetKeyName for querying the layout-specific name of printable keys
	libv.frame.input: Added glfwGetKeyScancode function that allows retrieving platform dependent scancodes for keys (#830)
	libv.hotkey: There will be a need for logical and physical key definition (99% physical, ctrl+z logical)
	libv.ui.focus: Key/Hotkey events can trigger focus-traversal
	libv.ui.hotkey: hotkey system are an extension to the keyboard system
	libv.ui.hotkey: ui.focus-backward = shift+tab
	libv.ui.hotkey: ui.focus-forward = tab
	libv.ui.hotkey: ui.focus-tab-backward = ctrl+shift+tab
	libv.ui.hotkey: ui.focus-tab-forward = ctrl+tab
	libv.ui.hotkey: ui.new-line = enter
	libv.ui.hotkey: ui.select = space
	libv.ui.hotkey: ui.send = ctrl+enter
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
	libv.ui.focus: easy way to trigger Focus traversal from events/components
	libv.ui.focus: Ability to yield focus gain, when a component receives the onFocus (gain) event it would be nice to have a way to refusing it forcing it to travel further, only downside is the members in the event class previous and current would be invalid
	libv.ui.focus: Focus traversal order: direct link (ptr, ptr)
	libv.ui.focus: Focus-traversal needs a component hierarchy independent way to be defined, no raw id or index, use ptr/refs when direct setting it

	libv.ui.focus: Focus traversal order: layout driven (layout knows the orientation)
	libv.ui.focus: Focus traversal order: position based

libv.math: create vec_fwd and mat_fwd headers

libv.ui: implement parentsDependOnLayout, reduce the number of layout invalidation caused by string2D edit

libv.sig: merge back the sig codebase rework a lighter version of the lib

libv.gl: move glew init into GL (with the ability to optionally disable it with a constructor argument)

libv.ui: lua style parsing and lua file tracking with auto re-style | only style parsing

libv.ui: Question: Is text is a component property that cannot be set from style, BUT it can be accessed dynamically in a uniform manner? | do I really have to implement a 'property' method ?

libv.ui: include check everything / fwd everything

libv.console: new console lib? A UI component will also be needed for it, but the backend should work without gui
libv.console: should depend on libv.arg

libv.ui.style: (style exclusive / multiple) multiple style usage in a component would still be nice, maybe synthetized styles?
libv.ui.layout: anchor should be a general property, when a component placed somewhere beside unused space single enum which of the 9 corner should it use

libv.utility: Implement a proper match file iterator "dir/part*.cpp", possibly with filesystem + ranges
app.vm4_viewer: implement a small light gui app to provide guidance to GUI development
app.vm4_viewer: display statistics of texture density and estimated texture pixel world space size

libv.ui: statistics: each ui operation (attach, style, render, ...) histogram, min, max, count

--- [[[ deadline: 2019.10.31 ]]] ---

libv.ui.layout: size over 100% is not an error
libv.gl: Implement a GLSL engine
libv.gl.glsl: Implement primitive preprocessor with #include and include dirs
libv.gl.glsl: Warning option for crlf line ending.
libv.gl.glsl: Warning option for space indentation or if indentation character is mixed

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

libv.ui.font: render the not found character by 'hand' (simple square)
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

libv.ui: batch component meshes into a bigger ui mesh cluster and use subcomponents, optimizations-optimizations

libv.utility: add/verify structured binding support for vec_t

libv.ui.warning: warning if percent used inside a content is invalid | not just log, but a generalized ui report system | console with extras

--- [[[ deadline: 2019.11.30 ]]] ---

libv.gl: Check on TextureRect if it is working properly (attempted to use it in font2D but failed, image was correct indexing/sampler issues (?))
libv.math: make every vec / mat operator a hidden friend | Is it possible or is it worth it (it might make 5 overload from the current 3 per operator)?

libv.glr: texture_fwd.hpp
libv.glr: RemoteTexture should have its own header file

libv.glr: Mesh attributes inside the remote should be stable, vector<unique_ptr<Attribute>>
libv.glr: Mesh attributes should use a single VBO

libv.ui: Label link support (?)
libv.glr: Procedural gizmo mesh
libv.glr: UniformBlockSharedView_std140

libv.ui: constraints: a way of syncing data between the world and the ui
libv.ui.layout: Flow
libv.ui.layout: 2D <-> 3D based on game state
libv.ui: ui <-> 3D layout linkage: planet names and additional informations are part of the ui and not the scene
		ui therefore has to access the game state (trivial, but this code has to happen now)

libv.ui: text | easy text
libv.ui: Idea that a component could signal the UI if it want to execute a heavy computation task before (attach, layout, create, render, destroy)

libv.glr: strong type locations and indices with enum classes, also use libv::gl::uniform
libv.glr: Implement sub-mesh API
libv.glr: Fix uniform naming mess, Reduce the number of public members
libv.glr: vm4 | non trivial
libv.gl: https://learnopengl.com/PBR/Lighting
libv.glr: frame buffer
libv.glr: render target
libv.glr: post-processing emission / bloom
libv.glr: post-processing gamma
libv.glr: post-processing haze

libv.glr: shadow
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

--- [[[ deadline: 2019.12.31 ]]] ---

libv.ecui: state based ui, separate control and data!

libv.frame: cleanup states by adding a single state for show/hidden/maximized/minimized/fullscreen/borderless_maximized
libv.frame: cleanup monitor, provide a thread-safe access to monitors list
libv.frame: cleanup global variables, at least place them next to each other and reason about thread access
libv.frame.input: Question should I couple scancode with key for each key states | observe use-case

libv.utility: pointer facade for: observer_ptr, observer_ref, etc...

libv.ui.event: mouse/keyboard/joystick ability to query sub-frame resolution of press/held/release cycle. Events are timed (a lot of timestamp)

sol2: new fwd header, use it if/where appropriate: sol_forward.hpp
cpp: enun class default underlying type is int, specify underlying type for every enum class
cpp: check if every possible operator had been made to a hidden friend
cpp: check if i have any recursive variadic function that is not using if constexpr but uses a tail overload
libv.gl: use mdspan for image updates instead of raw loops


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
cpp.proposal: P4 allow trailing comma for function arguments and lambda captures and init lists, its already there for arrays and enums
cpp.stacktrace: Seams like a solid alternative for boost.stacktrace https://github.com/bombela/backward-cpp
cpp: (adaptive) radix tree - O(1) lookup
cpp: can there be multiple definition error during linkage if two lib contains the same (symbol) definition
cpp: clarify template vs auto type deduction rules
cpp: keyword order: [[nodiscard]] virtual explicit friend static constexpr inline const void&& function() const&& noexcept override final;
cpp: replace every raw ptr with a smart counter part (incl observer_ptr)
doc / blog: Klipse plugin - http://blog.klipse.tech/cpp/2016/12/29/blog-cpp.html
ecs: existence / super-position based predication
ext: adopt zlib (remove assimp internal zlib) https://github.com/madler/zlib (light wrapper for usage: https://gist.github.com/gomons/9d446024fbb7ccb6536ab984e29e154a )
ext: adopt mdspan https://github.com/kokkos/mdspan/wiki/A-Gentle-Introduction-to-mdspan
gl: docs http://docs.gl
gl: glEnable(GL_DEBUG_OUTPUT);
gold: And if thou gaze long at a finite automaton, a finite automaton also gazes into thee.
gold: UNLESS someone like you cares a whole awful lot, nothing is going to get better. It's not.
learn: https://gafferongames.com/post/state_synchronization/ or just https://gafferongames.com/
mysql: mysql connector source https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-8.0.17-src.tar.gz
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
libv.sig: merge back and place meta (too many tamplate argument) into libv.meta, (or dont, please, it is too many template)
libv.ui.layout: think layout as a graph instead of a stack..., just think and see whats going on with that approach
libv.ui.lua: https://www.wowace.com/projects/ace3/pages/ace-gui-3-0-widgets
libv.ui: (shader) Program Descriptor: program is defined by a descriptor (which can be identified with a simple string key), this could also be applied for the rest of the resources
libv.ui: https://www.factorio.com/blog/post/fff-246
libv.ui: strong constraint: It has to keep up with 1000 character/sec input in mid sequence. Why? Because the 7.5cps is a reasonable high typing speed.
libv.utility: Make a proper copy-pastable noisy type
libv.vm4: geomax / geoorig: find the biggest distance between any two vertex, avg(a, b) = geoorig, dist(a, b) / 2 = geomax
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

--- NOTE -------------------------------------------------------------------------------------------

app: for apps you can cd next to the binary to solve any relative path issue (command line arguments should be handled beforehand)

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

// -------------------------------------------------------------------------------------------------

CMake resource folder
Cube / Sky Textures http://sourceforge.net/projects/spacescape/

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

// --- GLSL Pre-processor --------------------------------------------------------------------------

correct regex for include = R"qq(^[ \t]*#[ \t]*include[ \t]+(?:"(.*)"|<(.*)>).*)qq";

auto foo(const std::string_view subject) {
    return ctre::match<R"qq(^[ \t]*#[ \t]*include[ \t]+(?:"(.*)"|<(.*)>).*)qq">(subject);
}

std::optional<std::string_view> extract_number(std::string_view s) noexcept {
	if (auto m = ctre::match<"[a-z]+([0-9]+)">(s)) {
        return m.get<1>().to_view();
    } else {
        return std::nullopt;
    }
}

std::string Shader::PreprocessIncludes(const std::string& source, const boost::filesystem::path& filename, int level /*= 0 */ ) {
	PrintIndent();
	if(level > 32)
		LogAndThrow(ShaderException,"header inclusion depth limit reached, might be caused by cyclic header inclusion");
	using namespace std;

	static const boost::regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
	stringstream input;
	stringstream output;
	input << source;

	size_t line_number = 1;
	boost::smatch matches;

	string line;
	while(std::getline(input, line)) {
		if (boost::regex_search(line, matches, re)) {
			std::string include_file = matches[1];
			std::string include_string;

			try {
				include_string = Core::FileIO::LoadTextFile(include_file);
			} catch (Core::FileIO::FileNotFoundException& e) {
				stringstream str;
				str << filename.file_string() <<"(" << line_number << ") : fatal error: cannot open include file " << e.File();
				LogAndThrow(ShaderException,str.str())
			}
			output << PreprocessIncludes(include_string, include_file, level + 1) << endl;
																					// Why not here? (and as the first line)
		} else {
			output << "#line "<< line_number << " \"" << filename << "\"" << endl; // Why here? ^^^
			output << line << endl;
		}
		++line_number;
	}
	PrintUnindent();
	return output.str();
}

// -------------------------------------------------------------------------------------------------

/// Calculates the fast inverse square root approximation. Error rate is around 0.175%
/// For more information see: https://en.wikipedia.org/wiki/Fast_inverse_square_root
constexpr inline float fast_rsqrt(float y) noexcept {
	auto i = libv::bit_cast<int32_t>(y);
	i = 0x5f3759df - (i >> 1); // evil floating point bit level hacking

	y = libv::bit_cast<float>(i);
	y *= 1.5f - (0.5f * y * y * y); // One iteration of Newton's method, repeated application would reduce error rate

	return y;
}

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
