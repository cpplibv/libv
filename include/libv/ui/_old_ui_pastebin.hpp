// File: _old_ui_pastebin.hpp - Created on 2017.10.18. 04:17 - Author: Vader

#pragma once


// =================================================================================================

//class ContextUI {
//	std::vector<std::shared_ptr<ResourceProvider>> providers_sync;
//	std::vector<std::shared_ptr<ResourceProvider>> providers_async;
//
//public:
//	auto_cancel_view file_async(const boost::filesystem::path& filePath);
//	File_view file_sync(const boost::filesystem::path& filePath);
//	auto_cancel_view font_async(const boost::filesystem::path& filePath);
//	UIFont2D_view font_sync(const boost::filesystem::path& filePath);
//	auto_cancel_view program_async(const std::string& name);
//	UIProgram_view program_sync(const std::string& name);
//	auto_cancel_view texture_async(const boost::filesystem::path& filePath);
//	UITexture2D_view texture_sync(const boost::filesystem::path& filePath);
//	template <typename T>
//	auto_cancel_view<T> work_async(const TaskCallable_with_progress_report_multi_entry_call& task);
//};
//
//struct ContextRender {
//	std::chrono::duration<double> elapsed_time;
//	std::chrono::time_point<std::chrono::steady_clock> previous_render;
//	std::chrono::time_point<std::chrono::steady_clock> current_render;
//};
//
//struct Context {
//	gl::GL gl;
//	ContextResource resource;
//	ContextConfig config;
//	ContextExecutor executor;
//	ContextStat stat;
//};
//
//File_view ContextResource::file(const boost::filesystem::path& filePath) {
//	LIBV_LOG_UI_RESOURCE_TRACE("Request file [%s]", filePath);
//	for (const auto& provider : providers | ranges::view::reverse) {
//		auto res = provider->file(filePath);
//		if (res)
//			return res;
//	}
//	LIBV_LOG_UI_RESOURCE_WARN("Failed to provide file for [%s]. Returning burnt in default.", filePath);
//	return fallback;
//}
//
//struct ResourceProvider {
//	virtual File_view file(const boost::filesystem::path& filePath) = 0;
//	virtual UIFont2D_view font(const boost::filesystem::path& filePath) = 0;
//	virtual UIProgram_view program(const std::string& name) = 0;
//	virtual UIShader_view shader(const boost::filesystem::path& filePath) = 0;
//	virtual UITexture2D_view texture(const boost::filesystem::path& filePath) = 0;
//	virtual ~ResourceProvider() = default;
//};

// -------------------------------------------------------------------------------------------------

//template <typename T, typename... Args>
//struct ResourceResolver {
//	virtual bool operator()(T::Arguments& arg, const Args&... args) = 0;
//};
//
//template <typename T>
//struct ResourceCache {
//	virtual bool operator()(ResourceView<T>& res, const T::Arguments& arg) = 0;
//};
//
//template <typename T>
//struct ResourceProvider {
//	virtual bool operator()(ResourceView<T>& res, const T::Arguments& arg) = 0;
//};

//template <typename T>
//struct ResourceHandler {
//	std::vector<std::shared_ptr<ResourceResolver<T>>> resolvers;
//	std::vector<std::shared_ptr<ResourceCache<T>>> caches;
//	std::vector<std::shared_ptr<ResourceProvider<T>>> providers;
//};

//class ProgramDescription;

// -------------------------------------------------------------------------------------------------

//struct ModuleFocus : Module {};
//struct ModuleProperties : Module {};
//struct Render_StretchPlane : ModuleRender {};
//struct Render_ImagePlane : ModuleRender {};
//struct Event_Keyboard : Module {};
//struct Event_Mouse : Module {};
//struct Event_MousePosition : Module {};
//struct Focus : ModuleFocus {};
//struct Layout_Fix : ModuleLayout {};
//struct Layout_Float : ModuleLayout {};
//struct Layout_Flow : ModuleLayout {};
//struct Layout_Layer : ModuleLayout {};
//struct Layout_ToolTip : ModuleLayout {};

// =================================================================================================

//struct UIShader {
//	bool dirty = true;
//	std::string name;
//	std::string source;
//	libv::gl::Shader shader;
//	libv::gl::ShaderType type;
//	UIGL& owner;
//};
//
//struct UIShaderProxy {
//	UIShader& owner;
//	uint32_t index;
//	// 32bit padding could be utilized for: generation ID, proxy ID or flags
//};
//
//struct UIProgram {
//	bool dirty = true;
//	libv::gl::Program program;
//	std::string name;
//	UIShaderProxy vs;
//	UIShaderProxy fs;
//	UIShaderProxy gs;
//	UIGL& owner;
//};
//
//struct UIProgramProxy {
//	UIProgram& owner;
//	uint32_t index;
//	// 32bit padding could be utilized for: generation ID, proxy ID or flags
//};
//
//template <typename T>
//struct UIUniform {
//	libv::gl::Uniform<T> uniform;
//	const char* name;
//	UIProgram& owner;
//};
//
//template <typename T>
//struct UIUniformProxy {
//	UIUniform<T>& owner;
//	uint32_t index;
//	// 32bit padding could be utilized for: generation ID, proxy ID or flags
//};
//
//struct UITextureProxy {
//};

// -------------------------------------------------------------------------------------------------

//struct Batch {
//	uint16_t num_texture;
//	uint16_t num_uniform;
//	uint16_t num_vao;
////	size_t start;
////	size_t size;
//
//	// Pass
//	//	UIProgramProxy program;
//	// Uniforms
//	// std::vector<unsiforms> uniforms;
//	// VAO
//};

//struct BatchProcess {
//	std::vector<Batch> batches;
//
//	// Pass passes;
//	std::vector<UIProgramProxy> program;
//	std::vector<libv::gl::TextureChannel, UITextureProxy> textures;
//	//	std::vector<UIUniform> uniform;
//	std::vector<libv::gl::VertexArray> vao;
//};
//
//struct ExecutionQueue {
//	std::vector<BatchProcess> processes;
//
//};

// -------------------------------------------------------------------------------------------------

//struct FontEffect {};
//struct FontFamily {};
//struct FontSize {}; //font-size/line-height
//struct FontStyle {}; //font-style - Specifies the font style. Default value is "normal". See font-style for possible values
//struct FontVariant {}; //font-variant - Specifies the font variant. Default value is "normal". See font-variant for possible values
//struct FontWeight {}; //font-weight - Specifies the font weight. Default value is "normal". See font-weight for possible values
//
//struct Text {};
//
//struct Program {};
//
//struct Alignment {};
////struct Orientation {};
//
////struct TextAlignment {};
//
////struct Anchor {};
////struct FontColor { vec4f value; };
////	anchor
////	background
////	font_color = #FFFFFFFF
////	layout = layout_float
////	onUpdate = function: 000000000094a570
////	zzz_children
//
//struct Image {
//};
//
//struct Background {
//};
//
////struct Layout {
////};

// -------------------------------------------------------------------------------------------------

// Notes:
// Things that I need to store in the backend:
//
// element
// 	name
// 	size
//
// props
// 	property 0..*
//
// container
// 	layout,
// 	<childptr, position> 0..*
//
// render
// 	image
// 		program
// 		texture
// 		vbo, vao
// 	text
//			text
// 		program
// 		font
// 		vbo, vao
// 	canvas
// 		anything
//
//	focus
//		order
//		active_index
//
// event
//		mouse
//			event_type
//			region
//			callback
//		keyboard
//			event_type
//			(?) focus
//			callback

// -------------------------------------------------------------------------------------------------

//struct Event {
////	region;
////	callback;
////	event_type;
////	focus;
////	keyboard/mouse;
//};
//
//struct Focus {
//	size_t order;
//	size_t sub_focus;
//	bool focused;
//};
//
//struct Properties {
//	std::vector<Property> properties;
//};
//
//struct Render {
//	struct RenderImpl {
//		virtual ~RenderImpl() = default;
//	};
//
//	std::unique_ptr<RenderImpl> pimpl;
////	Program program;
////	vao;
////	vbo;
////	texture;
//};

// -------------------------------------------------------------------------------------------------

//struct UI {
//	std::vector<Component> components;
//	std::vector<Container> containers;
//	std::vector<Event> events;
//	std::vector<Focus> focuses;
//	std::vector<Properties> properties;
//	std::vector<Render> renders;
//};

// -------------------------------------------------------------------------------------------------

//struct Renderer {
//	void create(ecs::System& es) {
//		GLenum err = glewInit();
//		if (err != GLEW_OK)
//			log_ui.error("Failed to initialize glew: {}", glewGetErrorString(err));
//
//		IRIS_LOG_UI_DEBUG("GL Vendor: {}", glGetString(GL_VENDOR));
//		IRIS_LOG_UI_DEBUG("GL Renderer: {}", glGetString(GL_RENDERER));
//		IRIS_LOG_UI_DEBUG("GL Version: {}", glGetString(GL_VERSION));
//		IRIS_LOG_UI_DEBUG("Shading Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
//
//		es.foreach_component<ComponentRender>([](auto& e) { e.create(); });
//	}
//	void update(ecs::System& es) {
//		es.foreach_component<ComponentRender>([](auto& e) { e.render(); });
//	}
//	void destroy(ecs::System& es) {
//		es.foreach_component<ComponentRender>([](auto& e) { e.destroy(); });
//	}
//};

// -------------------------------------------------------------------------------------------------

/* ------  Notes:
 *
 * Open Questions:
 * - Invalidation and parent
 *
 * Only one Layout can be present in a hive
 *
 * Hive {
 * 0..* Render
 * 0..* Hive
 *    1 Focus
 *    1 Layout
 *    1 Event_Mouse
 *    1 Event_Keyboard
 * }
 *
 * ------ */

// =================================================================================================
// =================================================================================================
// =================================================================================================

////		ComponentLayout
////		ComponentPosition
////		ComponentRender
////		ComponentMouse
////		ComponentKeyboard
////		ComponentTimer
////		ComponentProperty
////		ComponentName
//
//		struct ComponentBase {
//			Flag_t flags;
//			libv::observer_ref<ComponentBase> parent = libv::make_observer(this);
//
//			libv::vec3f position; /// Component position absolute to origin
//			libv::vec3f size;
//		};
//
//		struct ComponentName {
//			std::string name;
//		};
//
//		struct ComponentLayout {
//			void (*layouter)(libv::ecs::System&, libv::ecs::EntityID, const ComponentBase&);
//		};
//
//		struct ComponentProperty {
//			Size propertySize;
//			//boost::container::flat_map<PropertyID, Property> properties;
//		};
//
//		struct ComponentRender : Dynamic {
//			Font2D_view font;
//			ProgramFont_view program;
//			String2D string;
//			libv::vec4f color = {1.f, 0.f, 0.f, 1.f};
//		};
//
//		struct Label {
//			libv::ecs::EntityID id;
//			libv::ecs::System& system;
//
//			void setString(std::string string) {
//				system.get<ComponentRender>(id, [&](ComponentRender& render) {
//					render.string.setString(std::move(string));
//				});
//			}
//		};
//
//		system.foreach_children<ComponentLayout, ComponentPosition>(0, [&](auto& layout) {
////			layout
//		});

// =================================================================================================

//struct Label : public Hive<Label> {
//    // String2D, Something that requires complex binding of size and layouting
//};
//
//struct ToolTip : public Hive<ToolTip> {
//	Render_StretchPlane background;
//	Label text;
//	Layout_Flow layout;
//};
//
//struct ToolTip_Host : public Hive<ToolTip_Host> {
//	// This tooltip should follow the mouse, there should be ones that does not
//	ToolTip toolTip;
//	Layout_ToolTip layout;
//	Event_Mouse eventMouse;
//};
//
//struct CivIcon : public Hive<CivIcon> {
//	Render_ImagePlane plane;
//	Layout_Layer layout;
//
//	ToolTip_Host toolTip;
//	Event_Mouse eventMouse;
//};
//
//struct LoadingBar : public Hive<LoadingBar> {
//
//};
//
//struct DeclarationOfWarIcon : public Hive<DeclarationOfWarIcon> {
//	CivIcon left;
//	CivIcon right;
//	Render_ImagePlane roundBackground;
//
//	Layout_Layer layout;
//
//	ToolTip toolTip;
//	Event_Mouse eventMouse;
//};
//
//struct RightSidePanel : public Hive<RightSidePanel> {
//	std::vector<DeclarationOfWarIcon> notifications;
//
//    struct Layout_DroppingNewEntriesFromSky {};
//	Layout_DroppingNewEntriesFromSky layout;
//
//	template <typename T>
//    void ui_access(T& access) {
//        access(layout);
//
//		for (auto& notification : notifications)
//			access(notification);
//    }
//};
//
//struct PlanetHud : public Hive<PlanetHud> {
//	Label name;
//};
//
//struct Button : public Hive<Button> {
//	Label text;
//	Render_StretchPlane plane;
//
//	Layout_Layer layout;
//
//	Focus focus;
//	Event_Keyboard eventKeyboard;
//	Event_Mouse eventMouse;
//
//    template <typename T>
//    void ui_access(T& access) {
//        access(text);
//        access(plane);
//        access(layout);
//        access(focus);
//        access(eventKeyboard);
//        access(eventMouse);
//    }
//};
//
//struct InputField : public Hive<InputField> {
//    Label text;
//    Render_StretchPlane plane;
//
//    Layout_Layer layout;
//
//    Focus focus;
//    Event_Keyboard eventKeyboard;
//    Event_Mouse eventMouse;
//
//    template <typename T>
//    void ui_access(T& access) {
//        access(text);
//        access(plane);
//        access(layout);
//        access(focus);
//        access(eventKeyboard);
//        access(eventMouse);
//    }
//};
//
//struct LuaComponent : public Hive<LuaComponent> {
//	std::vector<Hive<LuaComponent>> children;
//	std::vector<std::unique_ptr<Render>> renders;
//
//	std::unique_ptr<Layout> layout;
//	std::unique_ptr<Focus> focus;
//	std::unique_ptr<Event_Keyboard> eventKeyboard;
//	std::unique_ptr<Event_Mouse> eventMouse;
//
//    template <typename T>
//    void ui_access(T& access) {
//        access(layout);
//
//		for (auto& child : children)
//			access(child);
//		for (auto& render : renders)
//			access(render);
//
//		if(layout) access(*layout);
//		if(focus) access(*focus);
//		if(eventKeyboard) access(*eventKeyboard);
//		if(eventMouse) access(*eventMouse);
//    }
//};

// -------------------------------------------------------------------------------------------------
