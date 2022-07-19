// Project: libv, File: app/star/game/scene/scene_settings.cpp

// hpp
#include <star/game/scene/scene_settings.hpp>
// libv
#include <libv/ui/component/button.hpp>
#include <libv/ui/component/gap.hpp>
#include <libv/ui/component/input_field.hpp>
#include <libv/ui/component/label.hpp>
#include <libv/ui/component/panel_anchor.hpp>
#include <libv/ui/component/panel_grid.hpp>
#include <libv/ui/component/panel_line.hpp>
#include <libv/utility/parse_number.hpp>
// pro
#include <star/game/config/client_config.hpp>
// Only for scene switching and manipulation, there should be a better way:
#include <star/game/game_client.hpp>
#include <star/game/scene/scene_main_menu.hpp>
#include <star/game/scene/scene_root.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

struct SettingsBuilder {
	const void* ownerPtr;
	GameClient& gameClient;

	libv::ui::PanelLine main;
	libv::ui::PanelGrid grid{"grid"};

	explicit SettingsBuilder(const void* ownerPtr, GameClient& gameClient) :
		ownerPtr(ownerPtr),
		gameClient(gameClient) {

		grid.style("settings.grid");

		libv::ui::Label title;
		title.style("settings.title");
		title.text("Settings");
		main.add(std::move(title));

		main.style("settings.main");
		main.add(grid);
	}

	void toggle(ConfigEntry<bool>& entry) {
		//	libv::ui::PanelLine line;
		//	line.style("settings.entry.panel");
		//line.tooltip(entry.description());

	//	libv::ui::CheckBox cbox;
		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(entry.name());
		grid.add(std::move(label));

		libv::ui::Button input;
		input.style("settings.entry.value.toggle");
		input.event().submit.connect_system([&entry, input]() mutable {
			const auto reentry_guard = input.event_reentry_guard(input.ptr(), &entry);
			if (!reentry_guard)
				return;

			entry.store_and_update(!entry.value());
			input.text(entry.value() ? "On" : "Off");
		});
		entry.subscribe_and_call(ownerPtr, [input](const ConfigEntry<bool>::Change& event) mutable {
			const auto reentry_guard = input.event_reentry_guard(&event.entry, input.ptr());
			if (!reentry_guard)
				return;

			input.text(event.entry.value() ? "On" : "Off");
		});

		grid.add(std::move(input));
	}

	void number(ConfigEntry<int>& entry) {
		//	libv::ui::PanelLine line;
		//	line.style("settings.entry.panel");
		//line.tooltip(entry.description());

		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(entry.name());
		grid.add(std::move(label));

		libv::ui::InputField input;
		input.style("settings.entry.value.number");
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
		entry.subscribe_and_call(ownerPtr, [input](const ConfigEntry<int>::Change& event) mutable {
			const auto reentry_guard = input.event_reentry_guard(&event.entry, input.ptr());
			if (!reentry_guard)
				return;

			input.style_state(libv::ui::StyleState::error, false);
			input.text(std::to_string(event.entry.value()));
		});

		grid.add(std::move(input));
	}

	void text(ConfigEntry<std::string>& entry) {
		//	libv::ui::PanelLine line;
		//	line.style("settings.entry.panel");
		//line.tooltip(entry.description());

		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(entry.name());
		grid.add(std::move(label));

		libv::ui::InputField input;
		input.style("settings.entry.value.text");
		input.event().change.connect_system([&entry, input]() {
			const auto reentry_guard = input.event_reentry_guard(input.ptr(), &entry);
			if (!reentry_guard)
				return;

			entry.store_and_update(input.text());
		});
		entry.subscribe_and_call(ownerPtr, [input](const ConfigEntry<std::string>::Change& event) mutable {
			const auto reentry_guard = input.event_reentry_guard(&event.entry, input.ptr());
			if (!reentry_guard)
				return;

			input.text(event.entry.value());
		});

		grid.add(std::move(input));
	}

	struct OptionProxy {
		const void* ownerPtr;
		ConfigEntry<int>& entry;
		libv::ui::PanelLine row;

		OptionProxy(const void* ownerPtr, ConfigEntry<int>& entry, libv::ui::PanelLine&& row) :
			ownerPtr(ownerPtr), entry(entry), row(std::move(row)) {
		}

		void operator()(std::string name, int value) {
			libv::ui::Button input;
			input.style("settings.entry.value.option.button");
			input.text(std::move(name));
			input.event().submit.connect_system([e = &entry, input, value]() mutable {
				const auto reentry_guard = input.event_reentry_guard(input.ptr(), e);
				if (!reentry_guard)
					return;

				e->store_and_update(value);
				// !!! instead of style state change, radio button / group select will be a better solution
				input.style_state(libv::ui::StyleState::select, true);
			});
			// !!! Do not sub per option entry, sub per config entry, or this would be super simple hack
			entry.subscribe_and_call(ownerPtr, [input, value](const ConfigEntry<int>::Change& event) mutable {
				const auto reentry_guard = input.event_reentry_guard(&event.entry, input.ptr());
				if (!reentry_guard)
					return;

				// !!! instead of style state change, radio button / group select will be a better solution
				input.style_state(libv::ui::StyleState::select, value == event.entry.value());
			});

			row.add(std::move(input));
		}
	};

	[[nodiscard]] OptionProxy options(ConfigEntry<int>& entry) {
		//	libv::ui::PanelLine line;
		//	line.style("settings.entry.panel");
		//line.tooltip(entry.description());

		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(entry.name());
		grid.add(std::move(label));

		libv::ui::PanelLine row;
		row.style("settings.entry.value.option.row");
		grid.add(row);

		return OptionProxy{ownerPtr, entry, std::move(row)};
	}

	void restart_group() {
		grid = libv::ui::PanelGrid{"grid"};
		grid.style("settings.grid");
		main.add(grid);
	}

	[[nodiscard]] libv::ui::Component build() {
		{
			libv::ui::PanelLine ctrl;
			ctrl.style("settings.ctrl_line");

			{
				libv::ui::Button btn;
				btn.style("settings.ctrl");
				btn.text("Back");
				btn.event().submit.connect_system([gameClient = &gameClient](libv::ui::Button& source) {
//					back_to_main_menu();
//					source.event().global.fire<SwitchPrimaryScene>(
//							createSceneMainMenu(gameClient.nexus(), gameClient.settings())
//					);
					source.event().global.fire<SwitchPrimaryScene>(createSceneMainMenu(*gameClient));
				});
				ctrl.add(std::move(btn));

			} {
				libv::ui::Gap gap;
				ctrl.add(std::move(gap));

			} {
				libv::ui::Button btn;
				btn.style("settings.ctrl");
				btn.text("Reset");
				btn.event().submit.connect([gameClient = &gameClient]() {
					gameClient->config()->resetToDefault();
				});
				ctrl.add(std::move(btn));

			} {
				libv::ui::Button btn;
				btn.style("settings.ctrl");
				btn.text("Apply");
//				btn.event().submit.connect_system([](libv::ui::Button& source) {
//					source.event().global.fire<SwitchPrimaryScene>(createSceneMainMenu(gameClient));
//				});
				ctrl.add(std::move(btn));

			} {
				libv::ui::Button btn;
				btn.style("settings.ctrl");
				btn.text("Save");
				btn.event().submit.connect_system([gameClient = &gameClient](libv::ui::Button& source) {
					source.event().global.fire<SwitchPrimaryScene>(createSceneMainMenu(*gameClient));
				});
				ctrl.add(std::move(btn));
			}

			main.add(std::move(ctrl));
		}

		return main;
	}
};


libv::ui::Component createSceneSettings(GameClient& gameClient) {
	libv::ui::PanelAnchor layers{"layers"};

	auto config = gameClient.config();

	// Layers's ptr is used to subscribe/unsubscribe guard any subcomponent event handler
	const auto ownerPtr = layers.ptr();
	layers.event().detach.connect_system([config, ownerPtr] {
		config->unsubscribe(ownerPtr);
	});

	{
		SettingsBuilder builder{ownerPtr, gameClient};

		builder.text(config->profile.player_name);

		builder.toggle(config->graphics.vsync_enable);
		auto test_setting3 = builder.options(config->graphics.msaa_samples);
		test_setting3("Off", 1);
		test_setting3("2", 2);
		test_setting3("4", 4);
		test_setting3("8", 8);
		test_setting3("16", 16);
		auto test_setting4 = builder.options(config->graphics.msaa_samples);
		test_setting4("Off", 1);
		test_setting4("2", 2);
		test_setting4("4", 4);
		test_setting4("8", 8);
		test_setting4("16", 16);
		auto test_setting5 = builder.options(config->graphics.msaa_samples);
		test_setting5("Off", 1);
		test_setting5("2", 2);
		test_setting5("4", 4);
		test_setting5("8", 8);

		builder.number(config->development.test_setting);
		builder.number(config->development.test_setting);
		builder.number(config->development.test_setting);

		auto test_setting0 = builder.options(config->development.test_setting);
		test_setting0("Low", 0);
		test_setting0("Medium", 1);

		auto test_setting1 = builder.options(config->development.test_setting);
		test_setting1("Low", 0);
		test_setting1("Medium", 1);
		test_setting1("High", 2);

		auto test_setting2 = builder.options(config->development.test_setting);
		test_setting2("Low", 0);
		test_setting2("Medium", 1);
		test_setting2("High", 2);
		test_setting2("Ultra", 2);

		builder.restart_group();

//		builder.page("Development") {
			builder.toggle(config->development.logging_trace_ui);
			builder.toggle(config->development.always_on_top);
			builder.toggle(config->development.always_on_top);
			builder.toggle(config->development.always_on_top);
			builder.text(config->development.test_setting_str);
			builder.text(config->development.test_setting_str);
			builder.text(config->development.test_setting_str);
//		}


		layers.add(builder.build());
	}

	return layers;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
