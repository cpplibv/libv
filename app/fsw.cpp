// Project: libv, File: app/fsw.cpp

// ext
#include <fmt/chrono.h>
#include <fmt/format.h>
// libv
#include <libv/fsw/watcher.hpp>
#include <libv/mt/work_cooldown.hpp>
#include <libv/mt/worker_thread.hpp>
#include <libv/utility/timer.hpp>
// std
#include <chrono>
#include <iostream>
#include <thread>


// -------------------------------------------------------------------------------------------------

std::string cmd;

void dotted(int64_t a) {
	int64_t c = 1;

	if (a < 0) {
		a *= -1;
		std::cout << "-";
	}
	while ((c *= 1000) < a);
	while (c > 1) {
		int64_t t = (a % c) / (c / 1000);
		std::cout << (((c > a) || (t > 99)) ? "" : ((t > 9) ? "0" : "00")) << t;
		std::cout << (((c /= 1000) == 1) ? "" : "'");
	}
}

void run() {
	libv::Timer timer;
	std::cout << "Executing: " << cmd << std::endl;
	std::cout << "--------------------------------------------------------------------------------" << std::endl;

	// TODO P2: Instead of a system call use 'fork' and start in 'context' like what the unix 'time' command does
	//		Also the os signals forwarding part is also nice
	//		Source:
	// 			https://www.gnu.org/software/time/
	// 			https://ftp.gnu.org/gnu/time/
	const auto result = ::system(cmd.c_str());

	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	const auto runtime_ns = timer.time_ns().count();
	std::cout << "Execution " << (result == 0 ? "successful" : "failed: " + std::to_string(result)) << ". Time: ";
	dotted(runtime_ns / 1'000'000);
	std::cout << "ms" << std::endl;
}

// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	if (argc != 3) {
		std::cout << "File System Watcher - " << __DATE__ << " " << __TIME__ << std::endl;
		std::cout << "Usage: " << argv[0] << " <watching_path> <command>" << std::endl;
		return EXIT_FAILURE;
	}

	cmd = argv[2];

	libv::mt::worker_thread worker_thread;
	libv::mt::work_warmup_cooldown work_wu_cd{std::chrono::milliseconds{100}, std::chrono::milliseconds{100}};

	libv::fsw::Watcher watcher;

	const auto path = std::filesystem::path{argv[1]};

	bool is_dir = std::filesystem::exists(path) && std::filesystem::is_directory(path);
	bool is_file = std::filesystem::exists(path) && std::filesystem::is_regular_file(path);

	const auto schedule_run = [&work_wu_cd, &worker_thread](const libv::fsw::Event& event) {
		const auto now = std::chrono::system_clock::now();

		work_wu_cd.execute_async(worker_thread, [event, now] {
			fmt::print("\033[2J\033[1;1H"); // ::system("clear");
			fmt::print("{:%Y.%m.%d %H:%M:%S}: ", now);
			std::cout << event << std::endl;
			run();
		});
	};

	if (is_dir)
		watcher.subscribe_directory(path, schedule_run);
	else if (is_file)
		watcher.subscribe_file(path, schedule_run);
	else {
		watcher.subscribe_directory(path, schedule_run);
		watcher.subscribe_file(path, schedule_run);
	}

	work_wu_cd.execute_async(worker_thread, [argv] {
		fmt::print("\033[2J\033[1;1H"); // ::system("clear");
		std::cout << "Watching: " << argv[1] << std::endl;
		run();
	});

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return EXIT_SUCCESS;
}

