// Project: libv.ui, File: src/libv/ui/component_system/attach_state.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
// std
#include <optional>
// pro
#include <libv/ui/component/component.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

template <typename State>
struct AttachedState {
	std::optional<State> object;

	LIBV_FORCE_INLINE void operator()() {
		object.reset();
	}
};

template <typename State>
struct AttachedStateProxy {
	AttachedState<State>& state;

	template <typename... Args>
	LIBV_FORCE_INLINE State& operator()(Args&&... args) {
		return state.object.emplace(std::forward<Args>(args)...);
	}
};

/// Attaches a state to a ui component.
/// The state can be created by calling the operator() of the returned proxy object.
/// The state is destroyed when the ui component is detached. (Note: Not usable with reattachable components)
///
/// The state is allowed to store an owning reference to the component (During detach the state is destroyed, so it will not keep alive the component indefinitely)
template <typename State>
[[nodiscard]] LIBV_FORCE_INLINE AttachedStateProxy<State> attach_state(Component& component) {
	return {component.event().detach.connect_system_out_ref(AttachedState<State>{})};
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
