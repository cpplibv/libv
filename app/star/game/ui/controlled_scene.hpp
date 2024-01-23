// Project: libv.ui, File: app/star/game/ui/controlled_scene.hpp

#pragma once

#include <libv/ctrl/controls.hpp>
#include <libv/ui/component/component.hpp>
#include <libv/ui/component_system/create_scene.hpp>
#include <libv/ui/context/context_state.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class ControllerContext {
private:
	libv::ctrl::Controls& controls;
	libv::ui::Component& component;

public:
	ControllerContext(libv::ctrl::Controls& controls, libv::ui::Component& component) :
		controls(controls),
		component(component) {
	}

	template<typename ControlsType, typename K>
	void attached(K* contextVar) {
		static_assert(std::derived_from<K, ControlsType>);

		component.event().attach.connect_system([ctrls = controls, contextVar] mutable {
			ctrls.context_enter<ControlsType>(contextVar);
		});

		component.event().detach.connect_system([ctrls = controls, contextVar] mutable {
			ctrls.context_leave_if_matches<ControlsType>(contextVar);
		});
	}

	template<typename ControlsType, typename K>
	void focused(K* contextVar) {
		static_assert(std::derived_from<K, ControlsType>);

		component.event().focus.connect_system([ctrls = controls, contextVar](const libv::ui::EventFocus& event) mutable {
			if (event.gain())
				ctrls.context_enter<ControlsType>(contextVar);
			else
				ctrls.context_leave_if_matches<ControlsType>(contextVar);
		});
		component.event().focus_within.connect_system([ctrls = controls, contextVar](const libv::ui::EventFocusWithin& event) mutable {
			if (event.gain())
				ctrls.context_enter<ControlsType>(contextVar);
			else
				ctrls.context_leave_if_matches<ControlsType>(contextVar);
		});
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
struct ControlledScene {
	libv::ctrl::Controls controls; // Would be nice to not store controls
	T controlledScene;

public:
	template <typename... Args>
	explicit ControlledScene(libv::ctrl::Controls& controls, Args&&... args) :
		controls(controls),
		controlledScene(std::forward<Args>(args)...) {
	}

	template <typename... Args>
	[[nodiscard]] libv::ui::Component createScene(Args&&... args) {
		auto component = controlledScene.createScene(std::forward<Args>(args)...);
		{
			ControllerContext cctx{controls, component};
			controlledScene.controls(cctx);
		}
		if constexpr (requires { &T::update; }) {
			component.event().global_before_render.connect_system([this](libv::ui::Component& comp, const libv::ui::EventBeforeRender& event) {
				(void) event;
				controlledScene.update(comp.ui().state.time_frame(), comp.ui().state.time_delta());
			});
		}
		return component;
	}
};

/// The scene is destroyed when the ui component is detached. (Note: Not usable with reattachable components)
/// Required member functions:
///		libv::ui::Component createScene(CreateSceneArgs...);
///		void controls(ControllerContext& ctx);
/// @usage: createScene<SceneType>(controls, SceneCtorArgs...)(CreateSceneArgs...);
template <typename T, typename... Args>
[[nodiscard]] inline auto createControlledScene(libv::ctrl::Controls& controls, Args&&... args) {
	return libv::ui::createScene<ControlledScene<T>>(controls, std::forward<Args>(args)...);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
