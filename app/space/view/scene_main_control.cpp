// Project: libv, File: app/space/view/scene_main.hpp

// hpp
#include <space/view/scene_main_control.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/ctrl/binding.hpp>
#include <libv/ctrl/controls.hpp>
#include <libv/ctrl/feature_register.hpp>
#include <libv/ui/component/label.hpp>
// std
#include <sstream>
// pro
#include <space/log.hpp>
#include <space/view/scene_main.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

void SceneMainControl::register_controls(libv::ctrl::FeatureRegister controls) {
	controls.feature_binary<app::SceneMain>("space.show_controls", [](libv::ctrl::arg_binary& arg, SceneMain& ctx) {
		if (arg.value) {
			std::ostringstream os;

			size_t max_feature_name_length = 0;
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

			app::log_space.trace("Controls:\n{}", text);

			libv::ui::Label label("help-controls");
			label.style("space.overlay.controls-help.lbl");
			label.text(std::move(text));

			ctx.main_layers.add(label);
		} else {
			ctx.main_layers.remove("help-controls");
		}
	});
}

void SceneMainControl::bind_default_controls(libv::ctrl::Controls& controls) {
	controls.bind("space.show_controls", "F1");
}

// -------------------------------------------------------------------------------------------------

} // namespace app
