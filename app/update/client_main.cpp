// Project: libv.update, File: app/update/client_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/net/address.hpp>
#include <libv/update/client/updater.hpp>
// std
#include <iostream>
// pro
#include <update/common/config.hpp>
#include <update/common/log.hpp>
//#include <update/common/client.hpp>
//#include <libv/update/updater.hpp>
//#include <libv/update/update_client.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Update client", "LIBV Update client program");

//	const auto config_file = args.require<std::string>
//			("-c", "--config")
//			("config_file", "File path of the config file");

	const auto tmp_version = args.require<int>
			("-v", "--version")
			("tmp", "tmp");

	args.require_no_unused();

	if (!args.parse(argc, argv)) {
		std::cerr << args.error_message(100) << args.usage(100);
		return EXIT_FAILURE;
	}

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	app::log_app.info("{}", args.report(100));

	// -------------------------------------------------------------------------------------------------

	{
		libv::update::UpdaterSettings updater_settings;

		updater_settings.program_name = app::program_name;
		updater_settings.program_variant = app::program_variant;
//		updater_settings.current_version = app::current_version; // <<<
		updater_settings.current_version = libv::update::version_number{tmp_version.value()};
		updater_settings.path_executable = std::filesystem::path(argv[0]);
		updater_settings.path_root = updater_settings.path_executable.parent_path();
		updater_settings.path_lock_file = ".lock";
		updater_settings.path_temp_folder = ".update";
		updater_settings.update_servers = std::vector<libv::net::Address>{
				{"rs0.corruptedai.com", 25090},
//				{"rs1.corruptedai.com", 25091},
//				{"rs2.corruptedai.com", 25092},
//				{"rs3.corruptedai.com", 25093},
//				{"rs4.corruptedai.com", 25094},
		};

//		open_program_frame();
//		load_minimal_ui();

		libv::update::Updater updater(updater_settings);

//		const auto update_info = updater.check_for_update();
//		update_client.init(path_root_dir, path_executable, update_servers, current_version);
//		update_client.check_for_update();
//		client.update_from(libv::net::Address(address.value(), port.value()));

		switch (updater.check_for_update()) {
		case libv::update::update_check_result::version_up_to_date: {
			app::log_app.info("Current version is up to date");
			break;
		}
		case libv::update::update_check_result::version_not_supported: {
			app::log_app.info("Current version is not supported");
			break;
		}
		case libv::update::update_check_result::communication_error: {
			app::log_app.info("Communication error: Failed to check for update");
			break;
		}
		case libv::update::update_check_result::version_outdated: {
			app::log_app.info("Update found for current version");
//			updater.update([](auto operation, uint64_t progress_current, uint64_t progress_total) {
//				switch (operation) {
//				case libv::update::UpdateOperation::download:
//					app::log_app.info("Downloading update {}/{}...", progress_current, progress_total);
//					break;
//				case libv::update::UpdateOperation::update:
//					app::log_app.info("Updating update {}/{}...", progress_current, progress_total);
//					break;
//				}
//			});
			break;
		}
		}

//		discard_minimal_ui();
//		load_full_ui();
	}

	return EXIT_SUCCESS;
}

