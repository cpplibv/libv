// Project: libv.ui, File: app/gen_ui_theme/engine.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <gen_ui_theme/engine.hpp>
// libv
#include <libv/lua/object_parser.hpp>
#include <libv/parse/color.hpp>
// pro
//#include <gen_ui_theme/effect.hpp>
//#include <gen_ui_theme/theme.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct FlexPoint {
	bool horizontal = false;
	float point;
};

struct Task {
	std::string name;
	libv::vec2i texture_size;
	std::vector<FlexPoint> flex_points;
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

// -------------------------------------------------------------------------------------------------

Engine::Engine(std::filesystem::path script_file, std::function<void(libv::vector_2D<libv::vec4uc>, libv::vec2i)> callback) :
		script_file(std::move(script_file)),
		callback(std::move(callback)) {

	init();

	load_cd.execute_async([this] { load(); }, worker_thread);
	file_watcher.subscribe_file(this->script_file, [this](const auto&) {
		load_cd.execute_async([this] { load(); }, worker_thread);
	});
}

// -------------------------------------------------------------------------------------------------

static inline Task load_task(const sol::object& object) {
	Task result;

	const auto table = verify_table(object);

	for (const auto& item : table) {
		verify_number(item.first);
		const auto effect = verify_table(item.second);

		const auto effect_type = verify_string(effect["type"]);

		if (effect_type == "rounded_box") {
			result.effects.emplace_back(std::make_unique<app::EffectRoundedBox>(
					verify_userdata<libv::vec2f>(effect["pos"]),
					verify_userdata<libv::vec2f>(effect["size"]),
					verify_number<float>(effect["corner"]),
					verify_number<float>(effect["sharpness"])
			));
		} else if (effect_type == "glow") {
			result.effects.emplace_back(std::make_unique<app::EffectGlow>(
					verify_number<float>(effect["size"]),
					verify_number<float>(effect["falloff"]),
					verify_userdata<libv::vec4f>(effect["color"])
			));
		}
	}

	return result;
}

void Engine::init() {
	lua["dpi"] = libv::vec2f{92, 92};

	lua.set_function("register_var", [this](std::string_view name, double low, double high, double step) {

	});

//		lua.set_function("define_flex_point", [](libv::vec2f bl, libv::vec2f tr) {
//
//		});
}

// -------------------------------------------------------------------------------------------------

void Engine::load() {
	std::unique_lock lock(mutex);
	log_app.info("Loading...");

	std::vector<Task> tasks;
	libv::Timer timer;

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

		log_app.info("Script loading successful: {:7.3f}ms", timer.timef_ms().count());

		libv::vec2i texture_size;
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

		const auto result_main = sol::function(func_main)(lua_theme_table);

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
		app::EffectApplyEngine engine(12);
		engine.process(task_image, task.effects);

		builder.add(task.name, task_image.generate_8bit_channels());
	}

	log_app.info("Work execution successful: {:7.3f}ms", timer.timef_ms().count());

	static constexpr int32_t atlas_size_max = 1024;
	for (int32_t i = 64; i <= atlas_size_max; i *= 2) {
		try {
			const auto atlas_size = libv::vec2i{i, i};
			Atlas result = builder.build_atlas(atlas_size);

			callback(result.image, atlas_size);
			break;
		} catch (const std::exception& e) {
			if (i != atlas_size_max) {
				log_app.warn("{}", e.what());
			} else {
				log_app.error("{}", e.what());
				throw;
			}
		}
	}
}

//Atlas Engine::export_atlas() {
//	AtlasBuilder builder;
//
////		build.add(...);
//
//	static constexpr int32_t atlas_size_max = 1024;
//	for (int32_t i = 64; i <= atlas_size_max; i *= 2) {
//		try {
//			Atlas result = builder.build_atlas(libv::vec2i{i, i});
//			return result;
//		} catch (const std::exception& e) {
//			if (i != atlas_size_max) {
//				log_app.warn("{}", e.what());
//			} else {
//				log_app.error("{}", e.what());
//				throw;
//			}
//		}
//	}
//}

// -------------------------------------------------------------------------------------------------

} // namespace app
