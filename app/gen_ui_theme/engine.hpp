// Project: libv.ui, File: app/gen_ui_theme/engine.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/container/vector_2d.hpp>
#include <libv/math/vec.hpp>
// std
#include <filesystem>
#include <functional>
#include <memory>
//#include <span>
#include <string>
#include <string_view>
//#include <vector>
// pro
//#include <gen_ui_theme/effect.hpp>
//#include <gen_ui_theme/theme.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct DynamicVar {
	std::string name;
	double low;
	double high;
	double step;
	double value;

	bool added;
	bool removed;

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
	void set_dynamic_var(const std::string_view name, double value);
};

// -------------------------------------------------------------------------------------------------

} // namespace app



