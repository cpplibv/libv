// Project: libv.arg, File: test/libv/arg/test_args.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// std
#include <array>
#include <iostream>
// pro
#include <libv/arg/arg.hpp>


// -------------------------------------------------------------------------------------------------

TEST_CASE("parser: match argument short and long", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	SECTION("short") {
		const auto a0 = args.require<std::string>("-0")("a0", "a0 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a0.value() == "0");
	}

	SECTION("long") {
		const auto a1 = args.require<std::string>("--a1")("a1", "a1 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a1.value() == "1");
	}

	SECTION("multi short") {
		const auto a1 = args.require<std::string>("-1", "--a1")("a1", "a1 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a1.value() == "1");
	}

	SECTION("multi long") {
		const auto a1 = args.require<std::string>("-1", "--a1")("a1", "a1 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a1.value() == "1");
	}
}

TEST_CASE("parser: argument flag", "[libv.arg.parser]") {
	libv::arg::Parser args;

	SECTION("short and long") {
		const auto argv = std::array{"path/exe", "-0", "--3"};

		const auto a0 = args.flag("-0", "--0")("a0", "a0 desc");
		const auto a1 = args.flag("-1", "--1")("a1", "a1 desc");
		const auto a2 = args.flag("-2", "--2")("a2", "a2 desc");
		const auto a3 = args.flag("-3", "--3")("a3", "a3 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a0.value());
		CHECK(not a1.value());
		CHECK(not a2.value());
		CHECK(a3.value());
	}

	SECTION("short and long all set") {
		const auto argv = std::array{"path/exe", "-0", "-1", "--2", "--3"};

		const auto a0 = args.flag("-0", "--0")("a0", "a0 desc");
		const auto a1 = args.flag("-1", "--1")("a1", "a1 desc");
		const auto a2 = args.flag("-2", "--2")("a2", "a2 desc");
		const auto a3 = args.flag("-3", "--3")("a3", "a3 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a0.value());
		CHECK(a1.value());
		CHECK(a2.value());
		CHECK(a3.value());
	}

	SECTION("short") {
		const auto argv = std::array{"path/exe", "-0", "-1", "--2", "--3"};

		const auto a0 = args.flag("-0")("a0", "a0 desc");
		const auto a1 = args.flag("-1")("a1", "a1 desc");
		const auto a2 = args.flag("-2")("a2", "a2 desc");
		const auto a3 = args.flag("-3")("a3", "a3 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a0.value());
		CHECK(a1.value());
		CHECK(not a2.value());
		CHECK(not a3.value());
	}
}

TEST_CASE("parser: argument require typed", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	auto test = [&args, &argv](auto expected){
		const auto a1 = args.require<decltype(expected)>("-1", "--a1")("a1", "a1 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a1.value() == expected);
	};

	SECTION("double") { test(double{1}); }
	SECTION("float") { test(float{1}); }
	SECTION("int8_t") { test(int8_t{1}); }
	SECTION("int16_t") { test(int16_t{1}); }
	SECTION("int32_t") { test(int32_t{1}); }
	SECTION("int64_t") { test(int64_t{1}); }
	SECTION("uint8_t") { test(uint8_t{1}); }
	SECTION("uint16_t") { test(uint16_t{1}); }
	SECTION("uint32_t") { test(uint32_t{1}); }
	SECTION("uint64_t") { test(uint64_t{1}); }
	SECTION("std::string") { test(std::string{"1"}); }
}

TEST_CASE("parser: argument require typed with default", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	auto test = [&args, &argv](auto expected){
		const auto a1 = args.require<decltype(expected)>("-1", "--a1")("a1", "a1 desc") = expected;

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a1.value() == expected);
	};

	SECTION("double") { test(double{1}); }
	SECTION("float") { test(float{1}); }
	SECTION("int8_t") { test(int8_t{1}); }
	SECTION("int16_t") { test(int16_t{1}); }
	SECTION("int32_t") { test(int32_t{1}); }
	SECTION("int64_t") { test(int64_t{1}); }
	SECTION("uint8_t") { test(uint8_t{1}); }
	SECTION("uint16_t") { test(uint16_t{1}); }
	SECTION("uint32_t") { test(uint32_t{1}); }
	SECTION("uint64_t") { test(uint64_t{1}); }
	SECTION("std::string") { test(std::string{"1"}); }
}

TEST_CASE("parser: argument require type not set with default", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	auto test = [&args, &argv](auto expected){
		const auto a1 = args.require<decltype(expected)>("-3", "--a3")("a3", "a3 desc") = expected;

		REQUIRE(args.parse(std::size(argv), argv.data()));
		CHECK(a1.value() == expected);
	};

	SECTION("double") { test(double{1}); }
	SECTION("float") { test(float{1}); }
	SECTION("int8_t") { test(int8_t{1}); }
	SECTION("int16_t") { test(int16_t{1}); }
	SECTION("int32_t") { test(int32_t{1}); }
	SECTION("int64_t") { test(int64_t{1}); }
	SECTION("uint8_t") { test(uint8_t{1}); }
	SECTION("uint16_t") { test(uint16_t{1}); }
	SECTION("uint32_t") { test(uint32_t{1}); }
	SECTION("uint64_t") { test(uint64_t{1}); }
	SECTION("std::string") { test(std::string{"1"}); }
}

TEST_CASE("parser: argument optional typed", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	auto test = [&args, &argv](auto expected){
		const auto a1 = args.optional<decltype(expected)>("-1", "--a1")("a1", "a1 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		REQUIRE(a1.value().has_value());
		CHECK(a1.value().value() == expected);
	};

	SECTION("double") { test(double{1}); }
	SECTION("float") { test(float{1}); }
	SECTION("int8_t") { test(int8_t{1}); }
	SECTION("int16_t") { test(int16_t{1}); }
	SECTION("int32_t") { test(int32_t{1}); }
	SECTION("int64_t") { test(int64_t{1}); }
	SECTION("uint8_t") { test(uint8_t{1}); }
	SECTION("uint16_t") { test(uint16_t{1}); }
	SECTION("uint32_t") { test(uint32_t{1}); }
	SECTION("uint64_t") { test(uint64_t{1}); }
	SECTION("std::string") { test(std::string{"1"}); }
}

TEST_CASE("parser: argument optional typed not set", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	auto test = [&args, &argv](auto expected){
		const auto a1 = args.optional<decltype(expected)>("-3", "--a3")("a3", "a3 desc");

		REQUIRE(args.parse(std::size(argv), argv.data()));
		REQUIRE(not a1.value().has_value());
	};

	SECTION("double") { test(double{1}); }
	SECTION("float") { test(float{1}); }
	SECTION("int8_t") { test(int8_t{1}); }
	SECTION("int16_t") { test(int16_t{1}); }
	SECTION("int32_t") { test(int32_t{1}); }
	SECTION("int64_t") { test(int64_t{1}); }
	SECTION("uint8_t") { test(uint8_t{1}); }
	SECTION("uint16_t") { test(uint16_t{1}); }
	SECTION("uint32_t") { test(uint32_t{1}); }
	SECTION("uint64_t") { test(uint64_t{1}); }
	SECTION("std::string") { test(std::string{"1"}); }
}

TEST_CASE("parser: argument optional typed with default", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	auto test = [&args, &argv](auto expected){
		const auto a1 = args.optional<decltype(expected)>("-1", "--a1")("a1", "a1 desc") = expected;

		REQUIRE(args.parse(std::size(argv), argv.data()));
		REQUIRE(a1.value().has_value());
		CHECK(a1.value().value() == expected);
	};

	SECTION("double") { test(double{1}); }
	SECTION("float") { test(float{1}); }
	SECTION("int8_t") { test(int8_t{1}); }
	SECTION("int16_t") { test(int16_t{1}); }
	SECTION("int32_t") { test(int32_t{1}); }
	SECTION("int64_t") { test(int64_t{1}); }
	SECTION("uint8_t") { test(uint8_t{1}); }
	SECTION("uint16_t") { test(uint16_t{1}); }
	SECTION("uint32_t") { test(uint32_t{1}); }
	SECTION("uint64_t") { test(uint64_t{1}); }
	SECTION("std::string") { test(std::string{"1"}); }
}

TEST_CASE("parser: argument optional typed not set with default", "[libv.arg.parser]") {
	libv::arg::Parser args;
	const auto argv = std::array{"path/exe", "-0", "0", "--a1", "1", "--a2", "2"};

	auto test = [&args, &argv](auto expected){
		const auto a1 = args.optional<decltype(expected)>("-3", "--a3")("a3", "a3 desc") = expected;

		REQUIRE(args.parse(std::size(argv), argv.data()));
		REQUIRE(a1.value().has_value());
		CHECK(a1.value().value() == expected);
	};

	SECTION("double") { test(double{1}); }
	SECTION("float") { test(float{1}); }
	SECTION("int8_t") { test(int8_t{1}); }
	SECTION("int16_t") { test(int16_t{1}); }
	SECTION("int32_t") { test(int32_t{1}); }
	SECTION("int64_t") { test(int64_t{1}); }
	SECTION("uint8_t") { test(uint8_t{1}); }
	SECTION("uint16_t") { test(uint16_t{1}); }
	SECTION("uint32_t") { test(uint32_t{1}); }
	SECTION("uint64_t") { test(uint64_t{1}); }
	SECTION("std::string") { test(std::string{"1"}); }
}

TEST_CASE("parser: file watcher test", "[libv.arg.parser]") {
	libv::arg::Parser args;

//	const auto exe = args.exe();

	const auto path = args.require<std::string>
//	const auto path = args.multiple<std::string>(1)
			("-p", "--path")
			("path", "File path to the file system entry to be watched");

	const auto command = args.require<std::string>
			("-c", "--command")
			("command", "The shell command to be executed on any change of the watched path(s)");

//	const auto time_args_group = args.group("time", "Time related arguments");
//	const auto interval = time_args_group.optional<double>
	const auto interval = args.optional<double>
			("-i", "--interval")
			("interval", "The minimum required time between command execution");

	const auto delay = args.optional<double>
			("-d", "--delay")
			("delay", "The delay between the file system change event and the command execution")
			= 42.0;

	const auto help = args.flag
			("-?", "-h", "--help")
			("help", "Display usage information");


	SECTION("normal") {
		const auto argv = std::array{"path/exe", "--path", "path0", "--command", "command0"};
		CHECK(args.parse(std::size(argv), argv.data()));
	}

	SECTION("require no unused") {
		args.require_no_unused();
		const auto argv = std::array{"path/exe", "--path", "path0", "--command", "command0", "--unused"};
		CHECK(not args.parse(std::size(argv), argv.data()));
	}

	SECTION("accept unused") {
		const auto argv = std::array{"path/exe", "--path", "path0", "--command", "command0", "--unused"};
		CHECK(args.parse(std::size(argv), argv.data()));
	}

	SECTION("accept positional") {
		args.positional(path, command);
		const auto argv = std::array{"path/exe", "path0", "command0"};
		CHECK(args.parse(std::size(argv), argv.data()));
	}

	SECTION("report") {
		const auto argv = std::array{"path/exe", "--path", "path0", "--command", "command0", "--unused"};
		CHECK(args.parse(std::size(argv), argv.data()));

		const auto report = args.report();

		CHECK(report.find("Parsed arguments:") != std::string::npos);
		CHECK(report.find("path: path0") != std::string::npos);
		CHECK(report.find("command: command0") != std::string::npos);
		CHECK(report.find("help: false") != std::string::npos);
		CHECK(report.find("interval is not set") != std::string::npos);
		CHECK(report.find("delay: 42") != std::string::npos);
		CHECK(report.find("Unused arguments:") != std::string::npos);
		CHECK(report.find("5: \"--unused\"") != std::string::npos);

		std::cout << report << std::endl;
	}

	SECTION("usage") {
		const auto argv = std::array{"path/exe", "--help"};
		CHECK(not args.parse(std::size(argv), argv.data()));
		CHECK(help.value());

		const auto usage = args.usage();

		CHECK(usage.find("-p, --path") != std::string::npos);
		CHECK(usage.find("-c, --command") != std::string::npos);
		CHECK(usage.find("-i, --interval") != std::string::npos);
		CHECK(usage.find("-d, --delay") != std::string::npos);
		CHECK(usage.find("-?, -h, --help") != std::string::npos);
		CHECK(usage.find("File path to the file system entry to be watched") != std::string::npos);
		CHECK(usage.find("The shell command to be executed on any change of the watched path(s)") != std::string::npos);
		CHECK(usage.find("The minimum required time between command execution") != std::string::npos);
		CHECK(usage.find("The delay between the file system change event and the command execution") != std::string::npos);
		CHECK(usage.find("Display usage information") != std::string::npos);

		std::cout << usage << std::endl;
	}

//	CHECK(help);
//	CHECK(args.error_message(100));
//	CHECK(args.usage(100));
}

// -------------------------------------------------------------------------------------------------
