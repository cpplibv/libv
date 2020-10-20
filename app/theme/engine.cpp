// Project: libv.ui, File: app/gen_ui_theme/engine.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <theme/engine.hpp>
// ext
//#include <boost/container/flat_set.hpp>
// libv
#include <libv/algorithm/linear_find.hpp>
#include <libv/container/vector_2d.hpp>
#include <libv/fsw/watcher.hpp>
#include <libv/lua/lua.hpp>
//#include <libv/lua/object_parser.hpp>
//#include <libv/parse/color.hpp>
#include <libv/thread/work_cooldown.hpp>
#include <libv/thread/worker_thread.hpp>
#include <libv/thread/hardware_concurrency.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <filesystem>
#include <chrono>
#include <mutex>
// pro
#include <theme/effect.hpp>
#include <theme/theme.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------


// =================================================================================================
// =================================================================================================

//template <typename T>
//struct SOWScreenSet {
//	enum class State {
//		addition,
//		removal,
//		update,
//	};
//
//	struct Item {
//		T var;
//		State state;
//	};
//	std::vector<T> vars;
//};
//
//struct DynamicVar {
//	std::string name;
//	double low;
//	double high;
//	double step;
//	double value;
//
//	[[nodiscard]] friend inline bool operator==(const DynamicVar& lhs, const std::string_view& rhs) noexcept {
//		return lhs.name == rhs;
//	}
//};

// =================================================================================================
// =================================================================================================


//struct FlexPoint {
//	bool horizontal = false;
//	float point;
//};

struct Task {
	std::string name;
	libv::vec2i texture_size;
//	std::vector<FlexPoint> flex_points;
	std::vector<std::unique_ptr<app::BasePixelEffect>> effects;
};

// -------------------------------------------------------------------------------------------------

namespace lvs {

template <typename CppT, const char* CppTypeName, const char* LuaTypeName, sol::type LuaTypeEnum>
[[nodiscard]] static inline CppT aux_verify(const sol::object& object) {
	const auto type = object.get_type();

	if (type == sol::type::none)
		throw std::runtime_error("Missing {}");

	if (type != LuaTypeEnum)
		throw std::runtime_error("Expected {} but was {}");

	if constexpr (LuaTypeEnum == sol::type::userdata || LuaTypeEnum == sol::type::lightuserdata)
		if (!object.is<CppT>())
			throw std::runtime_error("Expected usertype {} but was {}");

	return object.as<CppT>();
}

constexpr inline char LuaTypeNameNil          [] = "Nil";
constexpr inline char LuaTypeNameBoolean      [] = "Boolean";
constexpr inline char LuaTypeNameNumber       [] = "Number";
constexpr inline char LuaTypeNameString       [] = "String";
constexpr inline char LuaTypeNameFunction     [] = "Function";
constexpr inline char LuaTypeNameTable        [] = "Table";
constexpr inline char LuaTypeNameThread       [] = "Thread";
constexpr inline char LuaTypeNameUserdata     [] = "Userdata";
constexpr inline char LuaTypeNameLightUserdata[] = "LightUserdata";

constexpr inline char CppTypeNameDontCare     [] = "";

} // namespace lvs ---

static inline std::string_view verify_string(const sol::object& object) {
	return lvs::aux_verify<std::string_view, lvs::CppTypeNameDontCare, lvs::LuaTypeNameString, sol::type::string>(object);
}

static inline sol::table verify_table(const sol::object& object) {
	return lvs::aux_verify<sol::table, lvs::CppTypeNameDontCare, lvs::LuaTypeNameTable, sol::type::table>(object);
}

template <typename T = double>
static inline T verify_number(const sol::object& object) {
	return lvs::aux_verify<T, lvs::CppTypeNameDontCare, lvs::LuaTypeNameNumber, sol::type::number>(object);
}

template <typename T>
static inline T verify_userdata(const sol::object& object) {
	return lvs::aux_verify<T, lvs::CppTypeNameDontCare, lvs::LuaTypeNameUserdata, sol::type::userdata>(object);
}

// =================================================================================================

struct ImplEngine {
public:
	std::filesystem::path script_file;

public:
	libv::lua::State lua = libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::vec);
	std::function<void(libv::vector_2D<libv::vec4uc>, libv::vec2i)> callback;
	sol::safe_function lua_main_func;

public:
//	boost::container::flat_set<DynamicVar, std::less<>> dynamic_vars;
	std::vector<DynamicVar> dynamic_vars;
	std::function<void(std::vector<DynamicVar>)> on_dynamic_var;

public:
	std::mutex mutex;

public:
	app::EffectApplyEngine engine;

public:
	libv::mt::worker_thread worker_thread{"lua-engine-worker"};
	libv::mt::work_heatup_cooldown load_cd{std::chrono::milliseconds{20}, std::chrono::milliseconds{100}};
	libv::mt::work_cooldown run_cd{std::chrono::milliseconds{5}};
	libv::fsw::Watcher file_watcher;

public:
	ImplEngine(size_t n) : engine(n) {}

public:
	void init();

public:
	void schedule_load_script();
	void schedule_run_script();
	void schedule_broadcast_dynamic_vars();

	void _load_script();
	void _run_script();
	void _broadcast_dynamic_vars();

};

// =================================================================================================

Engine::Engine(std::filesystem::path script_file, std::function<void(libv::vector_2D<libv::vec4uc>, libv::vec2i)> callback) {
	auto processor_count = libv::mt::hardware_concurrency_or(8);
	log_app.info("Starting engine with {} worker thread", processor_count);

	self = std::make_unique<ImplEngine>(processor_count);
	self->script_file = std::move(script_file);
	self->callback = std::move(callback);

	self->init();

	self->schedule_load_script();
	self->file_watcher.subscribe_file(self->script_file, [this](const auto&) {
		self->schedule_load_script();
	});
}

Engine::~Engine() {
	// For the sake of forward declared unique_ptr
}

// -------------------------------------------------------------------------------------------------

static inline Task load_task(const sol::object& object) {
	Task result;

	const auto table = verify_table(object);

	for (const auto& item : table) {
		verify_number(item.first);
		const auto effect = verify_table(item.second);

		const auto effect_type = verify_string(effect.get<sol::object>("type"));

		if (effect_type == "rounded_box") {
			result.effects.emplace_back(std::make_unique<app::EffectRoundedBox>(
//					effect["pos"].get_or_throw<libv::vec2f>(),
//					effect.get_or_throw<libv::vec2f>("pos"),
					verify_userdata<libv::vec2f>(effect.get<sol::object>("pos")),
					verify_userdata<libv::vec2f>(effect.get<sol::object>("size")),
					verify_number<float>(effect.get<sol::object>("corner")),
					verify_number<float>(effect.get<sol::object>("sharpness"))
			));
		} else if (effect_type == "glow") {
			result.effects.emplace_back(std::make_unique<app::EffectGlow>(
					verify_number<float>(effect.get<sol::object>("size")),
					verify_number<float>(effect.get<sol::object>("falloff")),
					verify_userdata<libv::vec4f>(effect.get<sol::object>("color"))
			));
		}
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

void ImplEngine::init() {
	lua["dpi"] = libv::vec2f{92, 92};

	lua.set_function("register_var", [this](std::string_view name, double low, double high, double step, double init) {
		const auto it = libv::linear_find_optional(dynamic_vars, name);
		if (it) {
			it->low = low;
			it->high = high;
			it->step = step;
			// We do not reset the value to init, only clamp it
			it->value = std::clamp(it->value, low, high);
			it->removed = false;
		} else {
			dynamic_vars.emplace_back(std::string(name), low, high, step, init, true, false);
		}
	});

	lua.set_function("define_flex_point", [](libv::vec2f bl, libv::vec2f tr) {

	});
}

void ImplEngine::schedule_load_script() {
	load_cd.execute_async([this] {

		std::unique_lock lock(mutex);
		_load_script();
		_run_script();
		_broadcast_dynamic_vars();

	}, worker_thread);
}

void ImplEngine::schedule_run_script() {
	run_cd.execute_async([this] {

		std::unique_lock lock(mutex);
		_run_script();

	}, worker_thread);
}

void ImplEngine::schedule_broadcast_dynamic_vars() {
	worker_thread.execute_async([this] {

		std::unique_lock lock(mutex);
		_broadcast_dynamic_vars();

	});
}

void ImplEngine::_load_script() {
	log_app.info("Loading script...");

	libv::Timer timer;

	for (auto& var : dynamic_vars)
		var.removed = true;

	try {
		const auto script_str = libv::read_file_or_throw(script_file);
		const auto result = lua.safe_script(script_str, sol::script_pass_on_error);

		if (!result.valid()) {
			sol::error err = result;
			return log_app.error("Script loading failed: {}", err.what());
		}

		if (result.get_type() != sol::type::none)
			log_app.warn("Script return value is unused: {} - {}", libv::to_value(result.get_type()), std::string(result));

		const auto func_main = lua["main"];

		if (!func_main.valid())
			return log_app.error("Main function is missing.");

		if (func_main.get_type() != sol::type::function)
			return log_app.error("Main is not a function: {} - {}", libv::to_value(func_main.get_type()), std::string(func_main));

		lua_main_func = func_main;
		log_app.info("Script loading successful  : {:7.3f}ms", timer.timef_ms().count());

		// ...
	} catch (const std::exception& e) {
		log_app.error("Failed to load texture due to exception: {}", e.what());
	}
}

void ImplEngine::_run_script() {
	std::vector<Task> tasks;
	libv::Timer timer;

	try {
		libv::vec2i texture_size;

		for (const auto& var : dynamic_vars)
			if (!var.removed)
				lua[var.name] = var.value;

		const auto lua_theme_table = lua.create_table(0, 2,
				"atlas", [&](std::string_view name, sol::table effects) {
					tasks.push_back(load_task(effects));
					tasks.back().texture_size = texture_size;
					tasks.back().name = name;
				},
				"texture_size", [&](int x, int y) {
					texture_size = {x, y};
				}
		);

		const auto result_main = lua_main_func(lua_theme_table);

		if (!result_main.valid()) {
			sol::error err = result_main;
			return log_app.error("Script execution failed: {}", err.what());
		}

//		if (result_main.get_type() != sol::type::table) {
//			return log_app.error("Script main function not return a table: {} - {}", libv::to_value(result_main.get_type()), std::string(result_main));
//		}

		log_app.info("Script execution successful: {:7.3f}ms", timer.timef_ms().count());
	} catch (const std::exception& e) {
		log_app.error("Failed to load texture due to exception: {}", e.what());
	}

	AtlasBuilder builder;

	for (const Task& task : tasks) {
		app::Image task_image(task.texture_size.cast<size_t>());
		engine.process(task_image, task.effects);

		builder.add(task.name, task_image.generate_8bit_channels());
	}

	log_app.info("Work execution successful  : {:7.3f}ms", timer.timef_ms().count());

	static constexpr int32_t atlas_size_max = 2048;
	for (int32_t i = 64; i <= atlas_size_max; i *= 2) {
		try {
			const auto atlas_size = libv::vec2i{i, i};
			Atlas result = builder.build_atlas(atlas_size);

			log_app.info("Atlas building successful  : {:7.3f}ms with size {}x{}", timer.timef_ms().count(), i, i);
			callback(result.image, atlas_size);
			break;
		} catch (const std::exception& e) {
			if (i != atlas_size_max) {
//				log_app.warn("{}", e.what());
			} else {
				log_app.error("{}", e.what());
				throw;
			}
		}
	}
}

void ImplEngine::_broadcast_dynamic_vars() {
	if (!on_dynamic_var)
		return;

	on_dynamic_var(dynamic_vars);

	std::erase_if(dynamic_vars, [](auto& var) {
		var.added = false;
		return var.removed;
	});
}

void Engine::set_dynamic_var(const std::string_view name, double value) {
	std::unique_lock lock(self->mutex);

	const auto it = libv::linear_find_optional(self->dynamic_vars, name);
	if (!it)
		return;

	it->value = value;

	self->schedule_run_script();
}

void Engine::on_dynamic_var(std::function<void(std::vector<DynamicVar>)> callback) {
	std::unique_lock lock(self->mutex);

	self->on_dynamic_var = std::move(callback);
	self->schedule_broadcast_dynamic_vars();
}

// -------------------------------------------------------------------------------------------------

} // namespace app
