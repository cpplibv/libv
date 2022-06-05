// Project: libv, File: app/star/game/client_config.hpp

// hpp
#include <star/game/config/config.hpp>
// libv
#include <libv/container/flat_set.hpp>
//#include <libv/mt/work_cooldown.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <chrono>
#include <filesystem>
#include <mutex>
// pro
#include <star/log.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class poll_warmup_cooldown {
private:
	enum class State {
		idle,
		warmup,
		cooldown,
	};

private:
	State state = State::idle;
	std::chrono::steady_clock::time_point action_at;

public:
	std::chrono::steady_clock::duration warmup{std::chrono::milliseconds{5000}};
	std::chrono::steady_clock::duration cooldown{std::chrono::milliseconds{30000}};

public:
	inline poll_warmup_cooldown(std::chrono::steady_clock::duration warmup, std::chrono::steady_clock::duration cooldown) :
			warmup(warmup),
			cooldown(cooldown) {}

	bool fire_and_test() {
		const auto now = std::chrono::steady_clock::now();

		switch (state) {
		case State::idle:
			state = State::warmup;
			action_at = now + warmup;
			return false;

		case State::warmup:
			if (now > action_at) {
				state = State::cooldown;
				action_at = now + cooldown;
				return true;
			} else {
				return false;
			}

		case State::cooldown:
			if (now > action_at) {
				state = State::warmup;
				action_at = now + warmup;
				return false;
			} else {
				state = State::warmup;
				action_at = std::max(now + warmup, action_at);
				return false;
			}
		}

		return false; // Unreachable
	}

//	void fire();
	bool test() {
		const auto now = std::chrono::steady_clock::now();

		switch (state) {
		case State::idle:
			return false;

		case State::warmup:
			if (now > action_at) {
				state = State::cooldown;
				action_at = now + cooldown;
				return true;
			} else {
				return false;
			}

		case State::cooldown:
			if (now > action_at)
				state = State::idle;

			return false;
		}

		return false; // Unreachable
	}
};

// -------------------------------------------------------------------------------------------------

class ImplConfig {
public:
	BaseConfig& config;

	std::mutex mutex;
	poll_warmup_cooldown save_cooldown{std::chrono::seconds(5), std::chrono::seconds(30)};

	libv::Nexus2 nexus;

	std::filesystem::path configFilepath;

	bool dirtyFile = false;
	libv::flat_set<BaseConfigEntry*> dirtyEntries;

public:
	explicit ImplConfig(BaseConfig& config, libv::Nexus2 nexus, std::filesystem::path configFilepath_) :
		config(config),
		nexus(std::move(nexus)),
		configFilepath(std::move(configFilepath_)) {
	}

	void loadFile() {
		const auto configFileString = libv::read_file_str_ec(configFilepath);

		if (!configFileString.ec) {
			try {
				config.loadJSON(configFileString.data);
				log_star.trace("Loaded config file {}", configFilepath.generic_string());

			} catch (const std::exception& ex) {
				// Failed to parse: reset to default and re-save
				log_star.error("Failed to deserilize config. Reseting to default and resaving the config file\nConfig file {}. Content was:\n{}", configFilepath.generic_string(), configFileString.data);
				config.resetToDefault();
				dirtyFile = true; // Force save by dirty flag
				saveFile();
			}

		} else if (configFileString.ec == std::errc::no_such_file_or_directory) {
			log_star.trace("Config file is missing. Generating default config file {}", configFilepath.generic_string());
			dirtyFile = true; // Force save by dirty flag
			saveFile();

		} else {
			log_star.error("Failed to load config file {}: {} - {}. Falling back to default config", configFilepath.generic_string(), configFileString.ec, configFileString.ec.message());
		}
	}

	void saveFile() {
		if (!dirtyFile)
			return;
		dirtyFile = false; // Even if failed, reset dirty flag to stop retry attempts

		try {
			const auto ec = libv::write_file_ec(configFilepath, config.saveJSON());
			log_star.error_if(ec, "Failed to save config file {}: {} - {}", configFilepath.generic_string(), ec, ec.message());
			log_star.info_if(!ec, "Saved config file {}", configFilepath.generic_string());

		} catch (const std::exception& ex) {
			log_star.error("Failled to serialize the config. Config is not saved");
		}
	}
};

// -------------------------------------------------------------------------------------------------

BaseConfig::BaseConfig(const libv::Nexus2& nexus, std::filesystem::path&& configFilepath) :
	self(std::make_shared<ImplConfig>(*this, nexus, std::move(configFilepath))) {
}

void BaseConfig::update() {
	auto lock = std::unique_lock(self->mutex);

	if (self->dirtyEntries.empty()) {
		if (self->save_cooldown.test())
			self->saveFile();

	} else {
		while (!self->dirtyEntries.empty()) {
			const auto it = self->dirtyEntries.begin() + (self->dirtyEntries.size() - 1);
//			const auto it = self->dirtyEntries.rbegin();
			const auto entry = *it;
			self->dirtyEntries.erase(it);
			lock.unlock();

			entry->update();

			lock.lock();
		}

		if (self->save_cooldown.fire_and_test())
			self->saveFile();
	}
}

void BaseConfig::unsubscribe(const void* slotPtr) {
	self->nexus.disconnect_slot_all(slotPtr);
}

void BaseConfig::create() {
	self->loadFile();
}

void BaseConfig::destroy() {
	self->saveFile();
}

libv::Nexus2& BaseConfig::nexus() const {
	return self->nexus;
}

void BaseConfig::markAsDirty(BaseConfigEntry& entry) {
	auto lock = std::unique_lock(self->mutex);

	self->dirtyFile = true;
	self->dirtyEntries.emplace(&entry);
}

// -------------------------------------------------------------------------------------------------

} // namespace star
