// Project: libv, File: app/theme/engine.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/container/vector_2d.hpp>
#include <libv/math/vec.hpp>
// std
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

struct DynamicVar {
	enum class State {
		create,
		remove,
		update,
	};
//	enum class Type {
//		scalar,
//		vec2,
//		vec3,
//		vec4,
//		rgb,
//		rgba,
//	};

public:
	std::string name;
	libv::vec4d low;
	libv::vec4d high;
	libv::vec4d step;
	libv::vec4d value;
	uint8_t dim = 0;
//	Type type;
	State state = State::create;

	explicit DynamicVar(std::string_view name) :
		name(name) {}

	[[nodiscard]] friend inline bool operator==(const DynamicVar& lhs, const std::string_view& rhs) noexcept {
		return lhs.name == rhs;
	}
};

// -------------------------------------------------------------------------------------------------

class Engine {
	std::unique_ptr<struct ImplEngine> self;

public:
	explicit Engine(std::filesystem::path script_file, std::function<void(libv::vector_2D<libv::vec4uc>, libv::vec2i)> callback);
	~Engine();

public:
	void on_dynamic_var(std::function<void(std::vector<DynamicVar>)> callback);
	void set_dynamic_var(const std::string_view name, const libv::vec4d& value);
};

// -------------------------------------------------------------------------------------------------

} // namespace app
