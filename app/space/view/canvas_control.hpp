// Project: libv, File: app/space/view/canvas_control.hpp

#pragma once

// libv
#include <libv/ctrl/fwd.hpp>
#include <libv/utility/chrono.hpp>
//#include <libv/utility/entity/entity_ptr.hpp>
// pro
#include <space/fwd.hpp>




#include <space/game/player.hpp> // TODO P2: (space) fwd

#include <libv/math/vec.hpp>



namespace space {

// -------------------------------------------------------------------------------------------------

struct CanvasControl {
	SpaceCanvas& canvas;
	Playout& playout;
//	Universe& universe;
	Galaxy& galaxy;
	Player& player;
//	Faction& controlledFaction;
//	libv::entity_ptr<Faction> controlledFaction;

//	private:
//		Controls controls;
//		~CtrlContextVar() {
//			controls.leave_context(this);
//		}

	enum class SelectionMode {
		none,
		d25,
		d3,
	};
	SelectionMode selectionMode = SelectionMode::none;
	libv::vec3f selectionStartGridPoint;
	libv::vec3f selectionStartDir;

public:
	static void register_controls(libv::ctrl::FeatureRegister controls);
	static void update(CanvasControl& ctx, libv::time_duration delta_time);
	static void bind_default_controls(libv::ctrl::BindingRegister controls);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
