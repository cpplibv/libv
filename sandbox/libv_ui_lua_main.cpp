// Project: libv.lua, File: sandbox/libv_ui_lua_main.cpp

// libv
#include <libv/log/log.hpp>
#include <libv/lua/lua.hpp>
#include <libv/utility/read_file.hpp>
// std
#include <iostream>
// pro
#include <libv/ui/context/context_style.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/lua/script_style.hpp>
#include <libv/ui/property_visit.hpp>
#include <libv/ui/style.hpp>
#include <libv/ui/ui.hpp>


#include <thread>
#include <chrono>
#include <libv/fsw/watcher.hpp>

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

// -------------------------------------------------------------------------------------------------

int main(int, char**) {
	std::cout << libv::logger_stream;

	// Change working directory
	if (!std::filesystem::exists("res/"))
		std::filesystem::current_path(".."); // During development artifact created under /bin

	libv::fsw::Watcher fsw;

	auto run = [](const auto&) {
		libv::ui::UI ui;
		libv::lua::State lua = libv::lua::create_state(libv::lua::lualib::base);

		log_sandbox.info(" --- Execute script ---");

//		libv::ui::script_style(ui, lua, libv::read_file_or_throw("app/space/style.lua"));
		libv::ui::script_style(ui, lua, libv::read_file_or_throw("app/star/style.lua"));

		auto f = [](std::string_view name, libv::ui::Style& style) {
			log_sandbox.info("Style: {}", name);

			style.foreach([](std::string_view key, const libv::ui::PropertyDynamic& property, libv::ui::StyleState mask, libv::ui::StyleState state) {
				libv::ui::visitProperty(property, [&](auto& v) {
					if constexpr (requires { std::cout << v; })
						log_sandbox.info("    {} = {} mask: {} state: {} [{}]", key, v, +mask, +state, typeid(v).name());
					else if constexpr (std::is_enum_v<decltype(v)>)
						log_sandbox.info("    {} = {} mask: {} state: {} [{}]", key, libv::to_underlying(v), +mask, +state, typeid(v).name());
					else
						log_sandbox.info("    {} = ?? mask: {} state: {} [{}]", key, +mask, +state, typeid(v).name());
				});
			});
		};
		ui.context().foreach_style(f);
	};

	run(42);
	fsw.subscribe_file("app/space/style.lua", run);

	while (true)
		std::this_thread::sleep_for(std::chrono::seconds(5));

	return 0;
}

// =================================================================================================

//// libv
//#include <libv/frame/frame.hpp>
//#include <libv/input/event.hpp>
//#include <libv/input/input.hpp>
//#include <libv/log/log.hpp>
//#include <libv/utility/read_file.hpp>
//// std
//#include <iostream>
//// pro
//#include <libv/lua/lua.hpp>
//#include <libv/ui/lua/script_style.hpp>
//#include <libv/ui/ui.hpp>

//class SandboxFrame : public libv::Frame {
//public:
//	libv::ui::UI ui;
//	libv::lua::State lua;
//
//public:
//	SandboxFrame() :
//		Frame("UI sandbox", 1280, 800),
//		lua(libv::lua::create_state(libv::lua::lualib::base | libv::lua::lualib::ui)) {
//
//		setPosition(FramePosition::center_current_monitor);
//		setOpenGLProfile(OpenGLProfile::core);
//		setOpenGLVersion(3, 3);
//		setOpenGLSamples(OpenGLSamples{4});
//		ui.attach(*this);
//
////		lua_component = libv::ui::script_file(ui, lua, "res/script/sandbox_ui.lua");
////		ui.add(lua_component);
//
//		onKey.output([&](const libv::input::EventKey& e) {
//			if (e.action == libv::input::Action::release)
//				return;
//
//			if (e.keycode == libv::input::Keycode::Escape)
//				closeDefault();
//		});
//	}
//	~SandboxFrame() {
//		disconnectAll();
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//int main(int, char**) {
//	std::cout << libv::logger_stream;
//
//	{
//		SandboxFrame frame;
//		frame.show();
//		frame.join();
//	}
//
//	return 0;
//}
