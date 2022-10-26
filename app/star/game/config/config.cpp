// Project: libv, File: app/star/game/config/config.cpp

// hpp
#include <star/game/config/config.hpp>
// libv
#include <libv/mt/queue_unique.hpp>
#include <libv/utility/nexus.hpp>
#include <libv/utility/polling_cooldown.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <chrono>
#include <filesystem>
// pro
#include <star/log.hpp>


namespace star {

// -------------------------------------------------------------------------------------------------

class ImplConfig {
public:
	BaseConfig& config;
	libv::Nexus nexus;

	std::mutex mutex;
	std::filesystem::path configFilepath;
	libv::polling_warmup_cooldown save_cooldown{std::chrono::seconds(60), std::chrono::seconds(300)};

	bool dirtyFile = false; /// Marks if file has to be re-saved
	libv::mt::queue_unique_batch_st<BaseConfigEntry*> dirtyEntries; /// Marks config entries that are need to be updated

public:
	explicit ImplConfig(BaseConfig& config, libv::Nexus nexus, std::filesystem::path configFilepath_) :
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
				log_star.error("Failed to deserilize config. Reseting to default and resaving the config file\nConfig file: {}\nError: {}\nContent was:\n{}", configFilepath.generic_string(), ex.what(), configFileString.data);
				config.resetToDefault();
				saveFile();
			}

		} else if (configFileString.ec == std::errc::no_such_file_or_directory) {
			log_star.trace("Config file is missing. Generating default config file {}", configFilepath.generic_string());
			saveFile();

		} else {
			log_star.error("Failed to load config file {}: {} - {}. Falling back to default config", configFilepath.generic_string(), configFileString.ec, configFileString.ec.message());
		}
	}

	void saveFile() {
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

BaseConfig::BaseConfig(const libv::Nexus& nexus, std::filesystem::path&& configFilepath) :
	self(std::make_shared<ImplConfig>(*this, nexus, std::move(configFilepath))) {
}

void BaseConfig::create() {
	self->loadFile();
}

void BaseConfig::destroy() {
	if (self->dirtyFile)
		self->saveFile();
}

void BaseConfig::update() {
	auto lock = std::unique_lock(self->mutex);
	if (self->dirtyEntries.empty()) {
		if (!self->dirtyFile) {
			if (self->save_cooldown.test())
				self->saveFile();
		} else {
			if (self->save_cooldown.fire_and_test())
				self->saveFile();
		}

	} else {
		self->dirtyEntries.drain_and_clear([&](BaseConfigEntry* dirtyEntry) {
			lock.unlock();
			dirtyEntry->update();
			lock.lock();
		});

		if (self->save_cooldown.fire_and_test())
			self->saveFile();
	}
}

void BaseConfig::unsubscribe(const void* slotPtr) {
	self->nexus.disconnect_slot_all(slotPtr);
}

void BaseConfig::requestSave() {
	auto lock = std::unique_lock(self->mutex);
	self->dirtyFile = true;
	self->save_cooldown.force_ready();
}

libv::Nexus& BaseConfig::nexus() const {
	return self->nexus;
}

void BaseConfig::markAsDirty(BaseConfigEntry& entry) {
	auto lock = std::unique_lock(self->mutex);
	self->dirtyFile = true;
	self->dirtyEntries.push_back(&entry);
}

void BaseConfig::markFileDirty() {
	auto lock = std::unique_lock(self->mutex);
	self->dirtyFile = true;
}

// -------------------------------------------------------------------------------------------------

} // namespace star
