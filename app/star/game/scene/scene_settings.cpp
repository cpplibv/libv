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
#include <libv/utility/nexus.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
#include <libv/utility/parse_number.hpp>
// pro
#include <star/game/client_config.hpp>
#include <star/game/game_client.hpp>
#include <star/game/scene/scene_main_menu.hpp>
#include <star/game/scene/scene_root.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class ConfigWatcher {
private:
	libv::Nexus& nexus;
	std::unordered_multimap<libv::observer_ptr<BaseConfigEntry>, std::function<void(BaseConfigEntry&)>> handlers;

public:
	explicit ConfigWatcher(libv::Nexus& nexus) : nexus(nexus) {
		nexus.connect<ConfigEntryChange>(this, [this](const ConfigEntryChange& event) {
			const auto [begin, end] = handlers.equal_range(libv::make_observer_ptr(&event.entry));
			for (auto it = begin; it != end; ++it)
				it->second(event.entry);
		});
	}

	~ConfigWatcher() {
		nexus.disconnect_all(this);
	}

	template <typename T, typename Fn>
	void watch(ConfigEntry<T>& entry, Fn&& fn) {
		fn(entry);

		handlers.emplace(libv::make_observer_ptr(&entry), [f = std::forward<Fn>(fn)](BaseConfigEntry& entry_) mutable {
			auto ptr = dynamic_cast<ConfigEntry<T>*>(&entry_);
			assert(ptr);
			f(*ptr);
		});
	}
};

struct SettingsBuilder {
	ConfigWatcher& watcher;
	GameClient& gameClient;

	libv::ui::PanelLine main;
	libv::ui::PanelGrid grid{"grid"};

	explicit SettingsBuilder(ConfigWatcher& watcher, GameClient& gameClient) :
		watcher(watcher),
		gameClient(gameClient) {

		grid.style("settings.grid");

		libv::ui::Label title;
		title.style("settings.title");
		title.text("Settings");
		main.add(std::move(title));

		main.style("settings.main");
		main.add(grid);

		{
			libv::ui::PanelLine ctrl;
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
				btn.text("Apply");
				btn.event().submit.connect_system([](libv::ui::Button& source) {
//					source.event().global.fire<SwitchPrimaryScene>(createSceneMainMenu(gameClient));
				});
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
	}

	void toggle(ConfigEntry<bool>& entry, std::string name, std::string description) {
	//	libv::ui::PanelLine line;
	//	line.style("settings.entry.panel");
	////	line.tooltip(std::move(description));
	//	(void) description;

	//	libv::ui::CheckBox cbox;
		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(name);
		grid.add(std::move(label));

		libv::ui::Button btn;
		btn.style("settings.entry.value.toggle");
		watcher.watch(entry, [btn](ConfigEntry<bool>& entry_) mutable {
			btn.text(entry_() ? "On" : "Off");
		});
		btn.event().submit.connect_system([&entry]() {
			entry(!entry());
		});
		grid.add(std::move(btn));
	}

	void number(ConfigEntry<int>& entry, std::string name, std::string description) {
	//	libv::ui::PanelLine line;
	//	line.style("settings.entry.panel");
	////	line.tooltip(std::move(description));
	//	(void) description;

		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(std::move(name));
		grid.add(std::move(label));

		libv::ui::InputField input;
		input.style("settings.entry.value.number");
		watcher.watch(entry, [input](ConfigEntry<int>& entry_) mutable {
			input.text(std::to_string(entry_()));
		});
		input.event().change.connect_system([&entry, input]() {
			entry(libv::parse_number_or_throw<int>(input.text()));
		});
		grid.add(std::move(input));
	}

	void text(ConfigEntry<std::string>& entry, std::string name, std::string description) {
	//	libv::ui::PanelLine line;
	//	line.style("settings.entry.panel");
	////	line.tooltip(std::move(description));
	//	(void) description;

		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(std::move(name));
		grid.add(std::move(label));

		libv::ui::InputField input;
		input.style("settings.entry.value.text");
		watcher.watch(entry, [input](ConfigEntry<std::string>& entry_) mutable {
			input.text(entry_());
			// !!! Event loop abortion logic, or nah, as no change will be called if text matches old value
			//			^ second half is not true, make it true
		});
		input.event().change.connect_system([&entry, input]() {
			entry(input.text());
		});
		grid.add(std::move(input));
	}

	struct OptionProxy {
		ConfigWatcher& watcher;
		ConfigEntry<int>& entry;
		libv::ui::PanelLine row;

		void operator()(std::string name, int value) {
			libv::ui::Button btn;
			btn.style("settings.entry.value.option.btn");
			btn.text(std::move(name));

			watcher.watch(entry, [btn, value](ConfigEntry<int>& entry_) mutable {
//				if (entry_() == value)
				// !!! Event loop abortion logic, or nah, as no change will be called if text matches old value
				//			^ second half is not true, make it true
			});
			btn.event().submit.connect_system([e = &entry, btn]() {
//				entry(btn.text());
			});

			row.add(std::move(btn));
		}
	};

	OptionProxy options(ConfigEntry<int>& entry, std::string name, std::string description) {
		libv::ui::Label label;
		label.style("settings.entry.lbl");
		label.text(std::move(name));
		grid.add(std::move(label));

		libv::ui::PanelLine row;
		row.style("settings.entry.value.option.row");
		grid.add(row);

		return OptionProxy{watcher, entry, std::move(row)};
	}

	libv::ui::Component build() {
		return main;
	}
};


libv::ui::Component createSceneSettings(GameClient& gameClient) {
	ConfigWatcher watcher(gameClient.nexus());

	const auto& config = gameClient.settings();

	libv::ui::PanelAnchor layers{"layers"};

	{
		SettingsBuilder settings{watcher, gameClient};

		settings.text(config->profile.player_name, "Player Name", "Display name of the player in multiplayer sessions");
		settings.toggle(config->graphics.msaa_enable, "MSAA", "Enable/Disable MSAA");
		settings.number(config->graphics.msaa_sample, "MSAA Sample", "Number of MSAA samples");

		auto test_setting3 = settings.options(config->graphics.test_setting, "Test Setting 3", "Test Setting description");
		test_setting3("Off", 1);
		test_setting3("2", 2);
		test_setting3("4", 4);
		test_setting3("8", 8);
		test_setting3("16", 16);

		auto test_setting0 = settings.options(config->graphics.test_setting, "Test Setting 0", "Test Setting description");
		test_setting0("Low", 0);
		test_setting0("Medium", 1);

		auto test_setting1 = settings.options(config->graphics.test_setting, "Test Setting 1", "Test Setting description");
		test_setting1("Low", 0);
		test_setting1("Medium", 1);
		test_setting1("High", 2);

		auto test_setting2 = settings.options(config->graphics.test_setting, "Test Setting 2", "Test Setting description");
		test_setting2("Low", 0);
		test_setting2("Medium", 1);
		test_setting2("High", 2);
		test_setting2("Ultra", 2);

		layers.add(settings.build());
	}

	return layers;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
