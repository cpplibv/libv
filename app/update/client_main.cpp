// Project: libv.update, File: app/update/client_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/net/address.hpp>
#include <libv/net/io_context.hpp>
#include <libv/update/update_client/update_client.hpp>
// std
#include <iostream>
// pro
#include <update/common/config.hpp>
#include <update/common/log.hpp>


#include <libv/update/resource_client/resource_client.hpp>


// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Update client", "LIBV Update client program");

//	const auto config_file = args.require<std::string>
//			("-c", "--config")
//			("config_file", "File path of the config file");

	const auto tmp_version = args.require<uint64_t>
			("-v", "--version")
			("tmp", "tmp") = 2;

	if (!args.standard_validate(argc, argv, std::cerr, 100))
		return EXIT_FAILURE;

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	app::log_app.info("{}", args.report(100));

	// -------------------------------------------------------------------------------------------------

	libv::update::ResourceClient::Settings settings;

//	settings.limit_bps_global_download = 0;
	settings.resource_servers = app::resource_servers;

	libv::net::IOContext io_context(4);
	libv::update::ResourceClient rc(io_context, settings);

	const std::filesystem::path save_dir = "tmp/";
	rc.download_file("app.update::update::dev::app.update-dev-02-00.update", save_dir / "app.update-dev-02-00.update");
	rc.download_file("app.update::update::dev::app.update-dev-02-01.update", save_dir / "app.update-dev-02-01.update");
	rc.download_file("app.update::update::dev::app.update-dev-02-02.update", save_dir / "app.update-dev-02-02.update");
	rc.download_file("app.update::update::dev::app.update-dev-02-03.update", save_dir / "app.update-dev-02-03.update");
	rc.download_file("app.update::update::dev::app.update-dev-02-04.update", save_dir / "app.update-dev-02-04.update");
	rc.wait();

	// -------------------------------------------------------------------------------------------------
/*
	{
		libv::update::UpdateClient::Settings uc_settings;

		uc_settings.program_name = app::program_name;
		uc_settings.program_variant = app::program_variant;
//		uc_settings.current_version = app::current_version; // <<<
		uc_settings.current_version = libv::update::version_number{tmp_version.value()};
		uc_settings.path_executable = std::filesystem::path(argv[0]);
		uc_settings.path_root = uc_settings.path_executable.parent_path();
		uc_settings.path_lock_file = ".lock";
//		uc_settings.path_temp_folder = ".update";
		uc_settings.path_temp_folder = ".update/";
		uc_settings.resource_servers = app::resource_servers;
		uc_settings.update_servers = app::update_servers;

//		open_program_frame();
//		load_minimal_ui();

		libv::update::UpdateClient update_client(uc_settings);

//		const auto update_info = update_client.check_for_update();
//		update_client.init(path_root_dir, path_executable, update_servers, current_version);
//		update_client.check_for_update();
//		client.update_from(libv::net::Address(address.value(), port.value()));

		switch (update_client.check_for_update()) {
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

			auto status = update_client.update();
			while (status.progress_for(std::chrono::milliseconds(10))) {
				app::log_app.info("Status: {}/{}", status.progress_current(), status.progress_total());

//				app::log_app.info("Recovering {}...");
//				app::log_app.info("Downloading {}...");
//				app::log_app.info("Updating {}...");

//				case libv::update::UpdateOperation::download:
//				case libv::update::UpdateOperation::update:
//					app::log_app.info("Downloading update {}/{}...", progress_current, progress_total);
//					app::log_app.info("Updating update {}/{}...", progress_current, progress_total);
			}
			break;
		}
		}

//		discard_minimal_ui();
//		load_full_ui();
	}
*/
	return EXIT_SUCCESS;
}

