// File: _old_ui_pastebin.hpp - Created on 2017.10.18. 04:17 - Author: Vader

#pragma once


// =================================================================================================

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

//struct ECUI {
//	std::vector<Component> components;
//	std::vector<Container> containers;
//	std::vector<Event> events;
//	std::vector<Focus> focuses;
//	std::vector<Properties> properties;
//	std::vector<Render> renders;
//};

//struct Renderer {
//	void create(ecs::System& es) {
//		es.foreach_component<ComponentRender>([](auto& e) { e.create(); });
//	}
//	void update(ecs::System& es) {
//		es.foreach_component<ComponentRender>([](auto& e) { e.render(); });
//	}
//	void destroy(ecs::System& es) {
//		es.foreach_component<ComponentRender>([](auto& e) { e.destroy(); });
//	}
//};

// =================================================================================================

//		ComponentLayout
//		ComponentPosition
//		ComponentRender
//		ComponentMouse
//		ComponentKeyboard
//		ComponentTimer
//		ComponentProperty
//		ComponentName

//
//		struct CoreComponent {
//			Flag_t flags;
//			libv::observer_ref<CoreComponent> parent = libv::make_observer(this);
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
//			void (*layouter)(libv::ecs::System&, libv::ecs::EntityID, const CoreComponent&);
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

// -------------------------------------------------------------------------------------------------
