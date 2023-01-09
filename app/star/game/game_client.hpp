// Project: libv, File: app/star/game/game_client.hpp

#pragma once

// fwd
#include <star/game/fwd.hpp>
// libv
#include <libv/ui/fwd.hpp>
#include <libv/utility/nexus_fwd.hpp>
// #include <libv/ui/component/component_fwd.hpp>
//#include <libv/ctrl/controls.hpp>
// std
#include <filesystem>
#include <memory>


namespace star {

// -------------------------------------------------------------------------------------------------

class ImplGameClient;

class GameClient {
private:
	std::unique_ptr<ImplGameClient> self;

public:
	GameClient(bool devMode, const std::filesystem::path& configFilepath);
	~GameClient();

private:
	void register_nexus();
	void register_controls();
	void unregister_nexus();
//	void update();

	[[nodiscard]] libv::ui::Component overlay_version(bool devMode);
	[[nodiscard]] libv::ui::Component overlay_fps();
	void init_ui(bool devMode);

public:
	void run();

public:
	[[nodiscard]] libv::Nexus& nexus() noexcept;
	[[nodiscard]] std::shared_ptr<ClientConfig> config() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace star


