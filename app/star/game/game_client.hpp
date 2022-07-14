// Project: libv, File: app/star/game/game_client.hpp

#pragma once

// fwd
#include <star/game/fwd.hpp>
// libv
#include <libv/utility/nexus_fwd.hpp>
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
	explicit GameClient(const std::filesystem::path& configFilepath);
	~GameClient();

private:
	void register_nexus();
	void unregister_nexus();
	void init_ui();
	void update();

public:
	void run();

public:
	[[nodiscard]] libv::Nexus2& nexus() noexcept;
	[[nodiscard]] std::shared_ptr<ClientConfig> config() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace star


