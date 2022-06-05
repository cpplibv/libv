// Project: libv, File: app/mimp/mimp_main.cpp

// libv
#include <libv/algo/wildcard.hpp>
#include <libv/mt/hardware_concurrency.hpp>
#include <libv/mt/worker_thread_pool.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
// std
#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4/model_serial.hpp>
#include <libv/vm4imp/importer.hpp>
#include <libv/vm4imp/log.hpp>
#include <mimp/lua_driver.hpp>


namespace mimp {

// -------------------------------------------------------------------------------------------------

bool execute_import(const std::string& path) {
	const auto output_path = std::filesystem::path(path).replace_extension("vm4").generic_string();
	const auto script_path = std::filesystem::path(path).replace_extension("lua").generic_string();

//	if (!std::filesystem::is_regular_file(script_path)) return false; // For testing just to only run one

	libv::vm4::log_vm4.info("Importing {}...", path);
	auto model = libv::vm4::import(path);

	if (model && std::filesystem::is_regular_file(script_path)) {
		libv::vm4::log_vm4.info("Executing {}...", script_path);
		auto script = libv::read_file_or_throw(script_path);

		const auto success = execute_script(*model, script);
		if (!success)
			return false;
	}

	if (model) {
		libv::vm4::log_vm4.info("Saving    {}...", output_path);

		std::ofstream ofs(output_path, std::ios::out | std::ios::binary);
		libv::archive::BinaryOutput oar(ofs);
		oar << LIBV_NVP_FORCED("model", *model);
		return true;
	} else {
		libv::vm4::log_vm4.error("Failed to import {}", path);
		return false;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace mimp

int main(int, char**) {
	std::cout << libv::logger_stream;

	libv::Timer timer;

	const auto folder_path = "res/model/";
//	const auto filter_pattern = "**.dae";
//	const auto filter_pattern = "**.game.fbx";
	const auto filter_pattern = "**.fbx";

	libv::mt::worker_thread_pool thread_pool{libv::mt::hardware_concurrency_or(4) - 1, "vm4 importer thread pool"};

	libv::vm4::log_vm4.info("Scanning: {}/{}", std::filesystem::current_path().generic_string(), folder_path);
	libv::vm4::log_vm4.info("Pattern : {}", filter_pattern);
	libv::vm4::log_vm4.info("Threads : {}", thread_pool.thread_count());
	std::cout << "--------------------------------------------------------------------------------" << std::endl;

	std::atomic<int> num_success = 0;
	std::atomic<int> num_failure = 0;

	for(const auto& p : std::filesystem::recursive_directory_iterator(folder_path)) {
		if (not p.is_regular_file())
			continue;

		auto filepath = p.path().generic_string();

		if (not libv::match_wildcard_glob(filepath, filter_pattern))
			continue;

		thread_pool.execute_async([&num_success, &num_failure, f = std::move(filepath)] {
			(mimp::execute_import(f) ? num_success : num_failure)++;
		});
	}

	thread_pool.stop();
	thread_pool.join();

	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	libv::vm4::log_vm4.info("Success: {}", num_success);
	libv::vm4::log_vm4.info_if(num_failure == 0, "Failure: {}", num_failure);
	libv::vm4::log_vm4.error_if(num_failure != 0, "Failure: {}", num_failure);
	libv::vm4::log_vm4.info("Time   : {:6.3f} ms", timer.timef_ms().count());

	return num_failure;
}
