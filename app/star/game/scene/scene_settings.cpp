// Project: libv, File: app/star/game/scene/scene_settings.cpp

// hpp
#include <star/game/scene/scenes.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/gap.hpp>
#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_grid.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/ui/component/selection_group.hpp>
#include <libv/ui/component/toggle_button.hpp>
#include <libv/utility/parse_number.hpp>
// pro
#include <star/game/config/client_config.hpp>
#include <star/game/scene/utility.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct SettingsBuilder {
	libv::Nexus& nexus;
	const void* owner_ptr;

	libv::ui::PanelLine main = libv::ui::PanelLine::s("settings.main");
	libv::ui::PanelGrid grid = libv::ui::PanelGrid::ns("grid", "settings.grid");

	SettingsBuilder(libv::Nexus& nexus, const void* owner_ptr) :
		nexus(nexus),
		owner_ptr(owner_ptr) {

		main.add(libv::ui::Label::sa("settings.title", "Settings"));

		main.add(grid);
	}

	void toggle(ConfigEntry<bool>& entry) {
//		auto line = libv::ui::PanelLine::s("settings.entry.panel");
//		line.tooltip(entry.description());

		grid.add(libv::ui::Label::sa("settings.entry.lbl", entry.name()));

		auto button = libv::ui::ToggleButton::s("settings.entry.value.toggle");
		button.text_on("On");
		button.text_off("Off");
		button.select(entry.value());
		button.event().submit.connect_system([&entry, button]() mutable {
			const auto reentry_guard = button.event_reentry_guard(button.ptr(), &entry);
			if (!reentry_guard)
				return;

			entry.store_and_update(!entry.value());
		});
		entry.subscribe(owner_ptr, [button](const ConfigEntry<bool>::Change& event) mutable {
			const auto reentry_guard = button.event_reentry_guard(&event.entry, button.ptr());
			if (!reentry_guard)
				return;

			button.select(event.entry.value());
		});

		grid.add(std::move(button));
	}

	void number(ConfigEntry<int>& entry) {
//		auto line = libv::ui::PanelLine::s("settings.entry.panel");
//		line.tooltip(entry.description());

		grid.add(libv::ui::Label::sa("settings.entry.lbl", entry.name()));

		auto input = libv::ui::InputField::s("settings.entry.value.number");
		input.event().change.connect_system([&entry, input]() mutable {
			const auto reentry_guard = input.event_reentry_guard(input.ptr(), &entry);
			if (!reentry_guard)
				return;

			const auto number = libv::parse_number_ec<int>(input.text());
			if (number.ec) {
				input.style_state(libv::ui::StyleState::error, true);
				return;
			} else {
				input.style_state(libv::ui::StyleState::error, false);
			}

			entry.store_and_update(number.value); // Uses 0 as fallback
		});
		entry.subscribe_and_call(owner_ptr, [input](const ConfigEntry<int>::Change& event) mutable {
			const auto reentry_guard = input.event_reentry_guard(&event.entry, input.ptr());
			if (!reentry_guard)
				return;

			input.style_state(libv::ui::StyleState::error, false);
			input.text(std::to_string(event.entry.value()));
		});

		grid.add(std::move(input));
	}

	void text(ConfigEntry<std::string>& entry) {
//		auto line = libv::ui::PanelLine::s("settings.entry.panel");
//		line.tooltip(entry.description());

		grid.add(libv::ui::Label::sa("settings.entry.lbl", entry.name()));

		auto input = libv::ui::InputField::s("settings.entry.value.text");
		input.event().change.connect_system([&entry, input]() {
			const auto reentry_guard = input.event_reentry_guard(input.ptr(), &entry);
			if (!reentry_guard)
				return;

			entry.store_and_update(input.text());
		});
		entry.subscribe_and_call(owner_ptr, [input](const ConfigEntry<std::string>::Change& event) mutable {
			const auto reentry_guard = input.event_reentry_guard(&event.entry, input.ptr());
			if (!reentry_guard)
				return;

			input.text(event.entry.value());
		});

		grid.add(std::move(input));
	}

	struct OptionProxy {
		const void* owner_ptr;
		ConfigEntry<int>& entry;
		libv::ui::PanelLine row;
		libv::ui::SelectionGroupUnique<int> group;

		OptionProxy(const void* owner_ptr, ConfigEntry<int>& entry, libv::ui::PanelLine&& row) :
			owner_ptr(owner_ptr), entry(entry), row(std::move(row)) {

			group.event().change.connect_system([en_ptr = &entry](libv::ui::SelectionGroupUnique<int>& group) {
				libv::ui::reentry_guard(group.ptr(), en_ptr, [&] {
					en_ptr->store_and_update(group.value());
				});
			});

			entry.subscribe(owner_ptr, [group = this->group](const auto& event) mutable {
				libv::ui::reentry_guard(&event.entry, group.ptr(), [&] {
					group.select(event.entry.value());
				});
			});
		}

		OptionProxy& operator()(std::string name, int value) {
			auto button = libv::ui::ToggleButton::s("settings.entry.value.option.button");
			button.text_on(name);
			button.text_off(std::move(name));
			button.select(entry.value() == value);

			group.add(button, value);
			row.add(std::move(button));

			return *this;
		}
	};

	[[nodiscard]] OptionProxy options(ConfigEntry<int>& entry) {
//		auto line = libv::ui::PanelLine::s("settings.entry.panel");
//		line.tooltip(entry.description());

		auto label = libv::ui::Label::sa("settings.entry.lbl", entry.name());
		grid.add(std::move(label));

		auto row = libv::ui::PanelLine::s("settings.entry.value.option.row");
		grid.add(row);

		return OptionProxy{owner_ptr, entry, std::move(row)};
	}

	void restart_group() {
		grid = libv::ui::PanelGrid::ns("grid", "settings.grid");
		main.add(grid);
	}

	[[nodiscard]] libv::ui::Component build(ClientConfig& config) {
		auto ctrl = libv::ui::PanelLine::s("settings.ctrl_line");

		{
			auto btn = libv::ui::Button::sa("settings.ctrl", "Back");
			btn.event().submit.connect([nexus_ = nexus](libv::ui::Button& source) mutable {
				switchParentScene("main", source, createSceneMainMenu(nexus_));
			});
			ctrl.add(std::move(btn));

		} {
			libv::ui::Gap gap;
			ctrl.add(std::move(gap));

		} {
			auto btn = libv::ui::Button::sa("settings.ctrl", "Reset");
			btn.event().submit.connect([config_ptr = &config] mutable {
				config_ptr->resetToDefault();
			});
			ctrl.add(std::move(btn));

		} {
			auto btn = libv::ui::Button::sa("settings.ctrl", "Apply");
//			btn.event().submit.connect([](libv::ui::Button& source) {
//				active_config.assign(local_config);
//			});
			ctrl.add(std::move(btn));

		} {
			auto btn = libv::ui::Button::sa("settings.ctrl", "Save");
			btn.event().submit.connect([nexus_ = nexus](libv::ui::Button& source) mutable {
//				active_config.assign(local_config);
				switchParentScene("main", source, createSceneMainMenu(nexus_));
			});

			ctrl.add(std::move(btn));
		}

		main.add(std::move(ctrl));
		return main;
	}
};

libv::ui::Component createSceneSettings(libv::Nexus& nexus) {
	auto& config = requireBean<ClientConfig>(nexus, "Controls", "ClientConfig");

	libv::ui::PanelAnchor layers{"layers"};

	// Layers's ptr is used to subscribe/unsubscribe guard any subcomponent event handler
	const auto owner_ptr = layers.ptr();
	layers.event().detach.connect_system([&config, owner_ptr] mutable {
		config.unsubscribe(owner_ptr);
	});

	{
		SettingsBuilder builder{nexus, owner_ptr};

		builder.text(config.profile.player_name);

		builder.toggle(config.graphics.vsync_enable);
		builder.options(config.graphics.msaa_samples)
				("Off", 1)
				("2", 2)
				("4", 4)
				("8", 8)
				("16", 16);
		builder.options(config.graphics.msaa_samples)
				("Off", 1)
				("2", 2)
				("4", 4)
				("8", 8)
				("16", 16);
		builder.options(config.graphics.msaa_samples)
				("Off", 1)
				("2", 2)
				("4", 4)
				("8", 8);
//		builder.options(config.graphics.af_samples)
//				("Off", 1)
//				("2", 2)
//				("4", 4)
//				("8", 8)
//				("16", 16);

		builder.number(config.development.test_setting);
		builder.number(config.development.test_setting);
		builder.number(config.development.test_setting);

		builder.options(config.development.test_setting)
				("Low", 0)
				("Medium", 1);

		builder.options(config.development.test_setting)
				("Low", 0)
				("Medium", 1)
				("High", 2);

		builder.options(config.development.test_setting)
				("Low", 0)
				("Medium", 1)
				("High A", 2)
				("High B", 2);

		builder.restart_group();

//		builder.page("Development") {
			builder.toggle(config.development.logging_trace_ui);
			builder.toggle(config.development.always_on_top);
			builder.toggle(config.development.always_on_top);
			builder.toggle(config.development.always_on_top);
			builder.text(config.development.test_setting_str);
			builder.text(config.development.test_setting_str);
			builder.text(config.development.test_setting_str);
//		}

		layers.add(builder.build(config));
	}

	return layers;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
