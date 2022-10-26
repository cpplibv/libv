// Project: libv, File: app/star/game/config/client_config.cpp

// hpp
#include <star/game/config/client_config.hpp>
// std
#include <sstream>
// libv
#include <libv/serial/archive/json.hpp>
#include <libv/serial/foreach.hpp>
#include <libv/serial/types/std_string.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] std::string ClientConfigT::saveJSON() const {
	std::ostringstream ss(std::ios::out | std::ios::binary);
	{
		libv::archive::JSONOutput ar(ss);
		const_cast<ClientConfigT*>(this)->serialize(ar);
	}
	return std::move(ss).str();
}

void ClientConfigT::loadJSON(const std::string& settingsJSONData) {
	std::istringstream ss(settingsJSONData, std::ios::in | std::ios::binary);
	{
		libv::archive::JSONInput ar(ss);
		this->serialize(ar);
	}
}

void ClientConfigT::resetToDefault() {
	libv::serial::foreach(*this, [](BaseConfigEntry& entry) {
		entry.resetToDefault();
	});
}

// -------------------------------------------------------------------------------------------------

} // namespace star
