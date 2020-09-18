// Project: libv.fsw, File: sandbox/libv_fsw_main.cpp, Author: Császár Mátyás [Vader]

// std
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
// pro
#include <libv/fsw/watcher.hpp>


// -------------------------------------------------------------------------------------------------

int main() {
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	std::filesystem::create_directories("tmp/p/p");
	std::filesystem::create_directories("tmp/p/b");
	std::filesystem::create_directories("tmp/a/p");
	std::filesystem::create_directories("tmp/b/p");

	{
		libv::fsw::FileWatcher watcher;

		watcher.subscribe_file("tmp/p/p/a.txt", [](const libv::fsw::Event& event) { std::cout << "\tWatch of tmp/p/p/a.txt got: " << event << std::endl; });
		watcher.subscribe_file("tmp/p/p/b.txt", [](const libv::fsw::Event& event) { std::cout << "\tWatch of tmp/p/p/b.txt got: " << event << std::endl; });
		watcher.subscribe_file("tmp/p/a.txt",   [](const libv::fsw::Event& event) { std::cout << "\tWatch of tmp/p/a.txt   got: " << event << std::endl; });
		watcher.subscribe_file("tmp/p/b.txt",   [](const libv::fsw::Event& event) { std::cout << "\tWatch of tmp/p/b.txt   got: " << event << std::endl; });
		watcher.subscribe_file("tmp/a.txt",     [](const libv::fsw::Event& event) { std::cout << "\tWatch of tmp/a.txt     got: " << event << std::endl; });
		auto i = watcher.subscribe_file("a.txt",         [](const libv::fsw::Event& event) { std::cout << "\tWatch of a.txt         got: " << event << std::endl; });
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		std::cout << "---" << std::endl;

		{ std::ofstream file("tmp/p/p/a.txt"); }
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		{ std::ofstream file("tmp/p/p/b.txt"); }
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		watcher.unsubscribe(i);
		{ std::ofstream file("a.txt"); }
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	std::filesystem::remove_all("tmp");
	std::filesystem::remove("a.txt");

	// -------------------------------------------------------------------------------------------------

//	std::cout << "> Start" << std::endl;
//
//	std::filesystem::path path_a = "tmp/a/a.txt";
//	std::filesystem::path path_b = "tmp/a/b.txt";
//	std::filesystem::path path_c = "tmp/c.txt";
//	std::filesystem::create_directories("tmp/a");
//
//	{
//		libv::fsw::FileWatcher watcher;
//
//		std::cout << "> Subscribe A" << std::endl;
//		auto token_a = watcher.subscribe_file(path_a, [](const libv::fsw::Event& event) {
//			std::cout << "    A: " << event << std::endl;
//		});
//
//		std::cout << "> Subscribe B" << std::endl;
//		watcher.subscribe_file(path_b, [](const libv::fsw::Event& event) {
//			std::cout << "    B: " << event << std::endl;
//		});
//
//		std::cout << "> Subscribe C" << std::endl;
//		watcher.subscribe_file(path_c, [](const libv::fsw::Event& event) {
//			std::cout << "    C: " << event << std::endl;
//		});
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Create A" << std::endl;
//		std::ofstream file_a(path_a.string());
//		file_a.close();
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Create B" << std::endl;
//		std::ofstream file_b(path_b.string());
//		file_b.close();
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Create C" << std::endl;
//		std::ofstream file_c(path_c.string());
//		file_c.close();
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Modify C" << std::endl;
//		file_c.open(path_c.string());
//		file_c << "Hi";
//		file_c.close();
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Remove A" << std::endl;
//		std::filesystem::remove(path_a);
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Rename B -> A" << std::endl;
//		std::filesystem::rename(path_b, path_a);
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Rename C -> A" << std::endl;
//	//	std::filesystem::rename(path_c, path_a); // MINGW Implementation bug, rename is not allowed to overwrite a file
//		std::filesystem::remove(path_a);
//		std::filesystem::rename(path_c, path_a);
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//		std::cout << "> Unsubscribe A" << std::endl;
//		watcher.unsubscribe(token_a);
//
//		std::cout << "> Remove A" << std::endl;
//		std::filesystem::remove(path_a);
//		std::this_thread::sleep_for(std::chrono::milliseconds(200));
//	}
//
//	std::filesystem::remove_all("tmp");
//
//	std::this_thread::sleep_for(std::chrono::seconds(1));
//	std::cout << "> Stop" << std::endl;
}
