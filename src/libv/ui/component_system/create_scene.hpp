// Project: libv.ui, File: src/libv/ui/component_system/create_scene.hpp

#pragma once

// std
#include <memory>
// pro
#include <libv/ui/component/component.hpp>
#include <libv/ui/component_system/attach_state.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename T>
struct AssembleSceneProxy {
	std::unique_ptr<T> t;

	template <typename... Args>
	libv::ui::Component operator()(Args&&... args) {
		auto c = t->createScene(std::forward<Args>(args)...);
		libv::ui::attach_state<std::unique_ptr<T>>(c)(std::move(t));
		return c;
	}
};

/// Requires a libv::ui::Component createScene(...) member function
/// @usage: createScene<SceneType>(SceneCtorArgs...)(CreateSceneArgs...);
template <typename T, typename... Args>
[[nodiscard]] inline AssembleSceneProxy<T> createScene(Args&&... args) {
	return AssembleSceneProxy<T>(std::make_unique<T>(std::forward<Args>(args)...));
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
