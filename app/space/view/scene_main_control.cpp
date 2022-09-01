// Project: libv, File: app/space/view/scene_main_control.cpp

// hpp
#include <space/view/scene_main_control.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/binding_register.hpp> // Temporary for the default binds
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/ui/component/label.hpp>
// std
#include <sstream>
// pro
#include <space/log.hpp>
#include <space/view/scene_main.hpp>



// Save/Load related
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/archive/json_any.hpp>
#include <libv/serial/archive/json.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
#include <space/game/game_session.hpp>
#include <space/sim/engine/snapshot_archive.hpp>
#include <space/sim/universe.hpp>

#include <libv/utility/nexus.hpp>
#include <space/message/internal_events.hpp>



namespace space {

// -------------------------------------------------------------------------------------------------

void SceneMainControl::register_controls(libv::ctrl::FeatureRegister controls) {
	controls.feature_action<SceneMain>("space.save", [](libv::ctrl::arg_action, SceneMain& ctx) {
		std::vector<std::byte> save_data;

		{
//			SnapshotArchive<libv::archive::BasicBinaryOutput> oar{true, save_data};
//			oar & LIBV_NVP_FORCED("universe", ctx.game_session->universe);
			libv::archive::BasicBinaryOutput oar{save_data};
			ctx.game_session->playout.saveSimulationSnapshot(oar, SnapshotType::local);
		}
//		{
//			std::string debug_data;
//			{
////				SnapshotArchive<libv::archive::BasicJSONAnyOutput> oar(true, debug_data);
////				oar & LIBV_NVP_FORCED("universe", ctx.game_session->universe);
////				libv::archive::BasicJSONAnyOutput oar(debug_data);
////				ctx.game_session->simulation.saveSnapshot(oar, SnapshotType::local);
////				0;
//				// !!1
//			}
//			log_space.info("Saved data: {}", debug_data);
//		}

		libv::write_file_or_throw("universe.sav", save_data);
		// <<< handle write error

		// TODO P4: Place / folder of save file, async write file, more logging, event on saving
		log_space.info("Saved: {} ({} byte)", "universe.sav", save_data.size());
	});

	controls.feature_action<SceneMain>("space.load", [](libv::ctrl::arg_action, SceneMain& ctx) {
		const auto save_data = libv::read_file_or_throw("universe.sav");
		// <<< handle read error

		{
			libv::archive::BinaryInput iar{save_data};
//			SnapshotArchive<libv::archive::BinaryInput> proxy_ar{ar, ctx.game_session->simulation.context, SnapshotType::local};
//			SnapshotArchive<libv::archive::BinaryInput> proxy_ar{ar, SnapshotType::local};

//			proxy_ar(LIBV_NVP_FORCED("universe", ctx.game_session->universe));

			ctx.nexus.broadcast_global<mc::ClearControlledFaction>(); // TODO P1: app.space:
			ctx.game_session->playout.loadSimulationSnapshot(iar, SnapshotType::local);
			ctx.nexus.broadcast_global<mc::ChangeControlledFaction>(); // TODO P1: app.space:
		}

		log_space.info("Loaded: {} ({} byte)", "universe.sav", save_data.size());
	});

	controls.feature_binary<SceneMain>("space.show_controls", [](libv::ctrl::arg_binary arg, SceneMain& ctx) {
		if (arg.value) {
			std::ostringstream os;

			std::size_t max_feature_name_length = 0;
			ctx.controls.foreach_bindings([&](const libv::ctrl::Binding& binding) {
				max_feature_name_length = std::max(max_feature_name_length, binding.feature_name().size());
			});

			ctx.controls.foreach_bindings([&](const libv::ctrl::Binding& binding) {
//				os << fmt::format("{:{}} : {}\n", binding.feature_name(), max_feature_name_length, binding.sequence().to_string_symbol());
				os << fmt::format("{:{}} : {}\n", binding.feature_name(), max_feature_name_length, binding.sequence().to_string_name());
			});

			auto text = std::move(os).str();
			if (!text.empty())
				text.pop_back(); // Discard the last \n character

			log_space.trace("Controls:\n{}", text);

			libv::ui::Label label("help-controls");
			label.style("space.overlay.controls-help.lbl");
			label.text(std::move(text));

			ctx.main_layers.add(label);
		} else {
			ctx.main_layers.remove("help-controls");
		}
	});
}

void SceneMainControl::bind_default_controls(libv::ctrl::BindingRegister controls) {
	controls.bind("space.show_controls", "F1");
	controls.bind("space.save", "F5");
	controls.bind("space.load", "F6");
}

// -------------------------------------------------------------------------------------------------

} // namespace space
