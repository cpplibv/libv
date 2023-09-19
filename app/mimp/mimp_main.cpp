// Project: libv, File: app/mimp/mimp_main.cpp

// libv
#include <libv/algo/wildcard.hpp>
#include <libv/arg/arg.hpp>
#include <libv/fsw/watcher.hpp>
#include <libv/mt/hardware_concurrency.hpp>
#include <libv/mt/work_cooldown.hpp>
#include <libv/mt/worker_thread_pool.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/timer.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <atomic>
#include <filesystem>
#include <iostream>
#include <mutex>
// pro
#include <libv/vm4/model.hpp>
#include <libv/vm4/model_serial.hpp>
#include <libv/vm4imp/importer.hpp>
#include <libv/vm4imp/log.hpp>
#include <mimp/lua_driver.hpp>


namespace mimp {

// -------------------------------------------------------------------------------------------------

enum class ImportResult {
	success,
	failure,
	skipped,
};

ImportResult execute_import(const std::string& path, bool allowSkip) {
	const auto output_path = std::filesystem::path(path).replace_extension("vm4").generic_string();
	const auto script_path = std::filesystem::path(path).replace_extension("lua").generic_string();

//	if (!std::filesystem::is_regular_file(script_path)) return false; // For testing just to only run one

	const auto scriptExists = std::filesystem::is_regular_file(script_path);

	const auto needsUpdate = [&] {
		const auto vm4Exists = std::filesystem::is_regular_file(output_path);
		if (!vm4Exists)
			return true;
		const auto lwtFBX = std::filesystem::last_write_time(path);
		const auto lwtVM4 = std::filesystem::last_write_time(output_path);
		const auto inputChanged = lwtFBX > lwtVM4;
		if (inputChanged)
			return true;
		if (!scriptExists)
			return false;
		const auto scriptChanged = scriptExists && std::filesystem::last_write_time(script_path) > lwtVM4;
		if (scriptChanged)
			return true;
		return false;
		// TODO P4: consider executable lwt, if the executable changed, update files too (only important on first 'run' after start)
	}();
	if (allowSkip && !needsUpdate) {
		libv::vm4::log_vm4.info("Skipping  {}", path);
		return ImportResult::skipped;
	}

	libv::Timer timer;

	libv::vm4::log_vm4.info("Importing {}...", path);
	auto model = libv::vm4::import(path);

	if (model && scriptExists) {
		libv::vm4::log_vm4.info("Executing {}...", script_path);
		auto script = libv::read_file_or_throw(script_path);

		try {
			execute_script(*model, script);
		} catch (const std::exception& e) {
			libv::vm4::log_vm4.error("Script execution failed: {}:\n{}", script_path, e.what());
			libv::vm4::log_vm4.error("Failure   {} ({:0.3f} ms)", output_path, timer.timef_ms().count());
			return ImportResult::failure;
		}
	}

	if (model) {
		libv::vm4::log_vm4.info("Saving    {}...", output_path);

		std::vector<std::byte> oss;
		oss.reserve(8 * 1024 * 1024);
		{
			libv::archive::BinaryOutput oar(oss);
			oar << LIBV_NVP_FORCED("model", *model);
		}

		libv::write_file_or_throw(output_path, oss);
		libv::vm4::log_vm4.info("Success   {} ({:0.3f} ms)", output_path, timer.timef_ms().count());
		return ImportResult::success;
	} else {
		libv::vm4::log_vm4.error("Failure   {} ({:0.3f} ms)", output_path, timer.timef_ms().count());
		return ImportResult::failure;
	}
}

// -------------------------------------------------------------------------------------------------

} // namespace mimp

int main(int argc, const char** argv) {
	std::cout << libv::logger_stream;

	auto args = libv::arg::Parser("Editor", "Source file editor and formatter utility");

	auto arg_watch = args.flag
			("-w", "--watch")
			("watch", "Keep running and watch input files and rerun upon change.");

	auto arg_force = args.flag
			("-f", "--force")
			("force", "Execute import even if there was no change detected.");

	auto arg_inputDirectory = args.require<std::string>
			("-i", "--input")
			("input-dir", "The input directory containing the fbx and lua files to be imported");

	auto arg_threadCount = args.require<uint32_t>
			("-j", "--thread")
			("thread", "Number of threads to be used. Defaults to number of threads.")
			= libv::mt::hardware_concurrency_or(4);

	auto arg_verbose = args.flag
			("-v", "--verbose")
			("verbose", "Output more information");

	args.positional(arg_inputDirectory);
	args.require_no_unused();

	if (!args.standard_validate(argc, argv, std::cerr, 100))
		return EXIT_FAILURE;

	if (arg_verbose.value())
		std::cout << args.report(100);

	libv::Timer timer;

	const auto folder_path = arg_inputDirectory.value();
	// libv::WildcardGlobMatcher patternFBX("**.game.fbx");
	libv::WildcardGlobMatcher patternFBX("**.fbx");
	libv::WildcardGlobMatcher patternLUA("**.lua");

	libv::mt::worker_thread_pool thread_pool{arg_threadCount.value(), "vm4 importer thread pool"};

	if (arg_verbose.value()) {
		fmt::print("Scanning: {}/{}\n", std::filesystem::current_path().generic_string(), folder_path);
		// fmt::print("Pattern : {}", filter_pattern);
		fmt::print("Threads : {}\n", thread_pool.thread_count());
		fmt::print("--------------------------------------------------------------------------------\n");
	}

	std::atomic<int> num_success = 0;
	std::atomic<int> num_failure = 0;
	std::atomic<int> num_skipped = 0;
	std::optional<libv::fsw::Watcher> fsw;
	std::mutex queue_m;
	std::map<std::string, libv::mt::work_warmup_cooldown> queueStates;

	const auto queueWork = [&](auto&& filepath, bool wait, bool allowSkip) {
		if (wait) {
			auto lock = std::unique_lock(queue_m);
			auto& state = queueStates.emplace(std::piecewise_construct,
					std::tuple(filepath),
					std::tuple(std::chrono::milliseconds{25}, std::chrono::milliseconds{100})).first->second;
			state.execute_async(thread_pool, [&num_success, &num_failure, &num_skipped, allowSkip, f = filepath] {
				switch (mimp::execute_import(f, allowSkip)) {
				case mimp::ImportResult::success: ++num_success; break;
				case mimp::ImportResult::failure: ++num_failure; break;
				case mimp::ImportResult::skipped: ++num_skipped; break;
				}
			});
		} else {
			thread_pool.execute_async([&num_success, &num_failure, &num_skipped, allowSkip, f = std::move(filepath)] {
				switch (mimp::execute_import(f, allowSkip)) {
				case mimp::ImportResult::success: ++num_success; break;
				case mimp::ImportResult::failure: ++num_failure; break;
				case mimp::ImportResult::skipped: ++num_skipped; break;
				}
			});
		}
	};

	if (arg_watch.value()) {
		fsw.emplace();
		fsw->subscribe_directory(arg_inputDirectory.value(), [&](const libv::fsw::Event& event) {
			if (event.action == libv::fsw::Action::remove)
				return;

			auto filepath = event.path.generic_string();
			if (patternLUA.match(filepath)) {
				auto pathFBX = event.path;
				pathFBX.replace_extension(".fbx");

				if (!std::filesystem::exists(pathFBX)) {
					fmt::print("Warning: There is no .fbx found for changed script: {}\n", filepath);
				} else {
					queueWork(pathFBX.generic_string(), true, false);
				}
			}

			if (patternFBX.match(filepath)) {
				queueWork(filepath, true, false);
			}
		});
	}

	for(const auto& p : std::filesystem::recursive_directory_iterator(folder_path)) {
		if (not p.is_regular_file())
			continue;

		auto filepath = p.path().generic_string();
		patternFBX.pattern("**.fbx");
		if (not patternFBX.match(filepath))
			continue;

		queueWork(filepath, false, !arg_force.value());
	}

	if (arg_watch.value()) {
		const auto ignore = getchar();
		(void) ignore;
	}

	fsw.reset();

	thread_pool.stop();
	thread_pool.join();

	fmt::print("--------------------------------------------------------------------------------\n");
	fmt::print("Success: {}\n", num_success);
	fmt::print("Failure: {}\n", num_failure);
	fmt::print("Skipped: {}\n", num_skipped);
	if (!arg_watch.value())
		fmt::print("Time   : {:6.3f} ms\n", timer.timef_ms().count());

	return num_failure;
}
