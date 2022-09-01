// Project: libv, File: app/star/game/client_config.hpp

#pragma once

// libv
#include <libv/serial/access_fwd.hpp>
#include <libv/utility/nexus.hpp>
// std
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>


namespace star {

// -------------------------------------------------------------------------------------------------

class BaseConfig;
class BaseConfigEntry;
class ImplConfig;
template <typename T>
class ConfigEntry;

// -------------------------------------------------------------------------------------------------

class BaseConfig {
	friend ImplConfig;
	template <typename T> friend class ConfigEntry;

protected:
	std::shared_ptr<ImplConfig> self;

public:
	BaseConfig(const libv::Nexus& nexus, std::filesystem::path&& configFilepath);
	virtual ~BaseConfig() = default;

protected:
	void create();
	void destroy();

public:
	void update();
	void unsubscribe(const void* slotPtr);
	void requestSave();

protected:
	[[nodiscard]] libv::Nexus& nexus() const;
	void markAsDirty(BaseConfigEntry& entry);
	void markFileDirty();

private:
	[[nodiscard]] virtual std::string saveJSON() const = 0;
	virtual void loadJSON(const std::string& settingsJSONData) = 0;
	virtual void resetToDefault() = 0;
};

// -------------------------------------------------------------------------------------------------

template <typename ConfigT>
class Config : public ConfigT {
public:
	Config(const libv::Nexus& nexus, std::filesystem::path configFilepath) :
		ConfigT(nexus, std::move(configFilepath)) {
		this->create();
	}

	~Config() {
		this->destroy();
	}
};

template <typename Config>
[[nodiscard]] inline std::shared_ptr<Config> make_config(const libv::Nexus& nexus, std::filesystem::path configFilepath) {
	return std::make_shared<Config>(nexus, std::move(configFilepath));
}

// =================================================================================================

class BaseConfigEntry {
protected:
	BaseConfig& config;

protected:
	mutable std::mutex mutex;

private:
	std::string name_;
	std::string description_;

public:
	inline BaseConfigEntry(BaseConfig& config, std::string&& name, std::string&& description) :
			config(config),
			name_(std::move(name)),
			description_(std::move(description)) {}

	virtual ~BaseConfigEntry() = default; // VTable to enable dynamic casts

	virtual void update() = 0;
	virtual void resetToDefault() = 0;

public:
	[[nodiscard]] constexpr inline const std::string& name() const noexcept {
		return name_;
	}

	[[nodiscard]] constexpr inline const std::string& description() const noexcept {
		return description_;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class ConfigEntry : public BaseConfigEntry {
public:
	struct Change {
		ConfigEntry<T>& entry;
	};

private:
	T value_;
	T valueNext_;
	const T valueDefault_;

public:
	inline ConfigEntry(BaseConfig& config, T value_, std::string name, std::string description) :
		BaseConfigEntry(config, std::move(name), std::move(description)),
		value_(value_),
		valueDefault_(std::move(value_)) {}

private:
	friend libv::serial::access;
	template <typename Archive> const T& save_minimal(const Archive&) const {
		return value_;
	}
	template <typename Archive> void load_minimal(const Archive&, const T& value) {
		value_ = value;
	}

public:
	[[nodiscard]] inline const T& value() const noexcept {
		auto lock = std::unique_lock(mutex);
		return value_;
	}

	[[nodiscard]] inline const T& valueDefault() const noexcept {
		return valueDefault_;
	}

	void store(T v) noexcept {
		{
			auto lock = std::unique_lock(mutex);
			valueNext_ = std::move(v);

			if (valueNext_ == value_)
				return;
		}
		config.markAsDirty(*this);
	}

	void store_and_update(T v) noexcept {
		{
			auto lock = std::unique_lock(mutex);
			value_ = std::move(v);
		}
//		config.markAsDirty(*this);
		config.markFileDirty();
		config.nexus().broadcast_channel<Change>(this, Change{*this});
	}

	virtual void update() override {
		{
			auto lock = std::unique_lock(mutex);
			value_ = std::move(valueNext_);
		}
		config.nexus().broadcast_channel<Change>(this, Change{*this});
	}

	virtual void resetToDefault() override {
		store(valueDefault_);
	}

public:
	template <typename F>
	void subscribe_and_call(const void* slotPtr, F&& callback) {
		config.nexus().connect_channel_and_call<Change>(this, slotPtr, std::forward<F>(callback), Change{*this});
	}
};

// -------------------------------------------------------------------------------------------------

struct BaseConfigGroup {
	BaseConfig& config;
};

// -------------------------------------------------------------------------------------------------

} // namespace star
