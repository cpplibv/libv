// Project: libv.rev, File: test/libv/rev/test_glsl_compose.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/rev/glsl_compose.hpp>


// -------------------------------------------------------------------------------------------------

#include <iostream>
#include <libv/log/log.hpp>
TEST_CASE("enable logger", "[libv.gl.glsl_compiler]") {
	std::cout << libv::logger_stream;
}

TEST_CASE("GLSL source include pre-processing", "[libv.rev.glsl_compose]") {
	const auto includer = [](const auto& str) -> libv::rev::IncludeResult {
		if (str == "A.glsl")
			return {std::error_code(), "A1\nA2\nA3"}; // No newline at the end
		if (str == "B.glsl")
			return {std::error_code(), "B1\nB2\nB3\n"};
		if (str == "C.glsl")
			return {std::error_code(), "C1\nC2\nC3\n"};

		if (str == "DinA.glsl")
			return {std::error_code(), "#include <A.glsl>\nD2\nD3\nD4\n"};
		if (str == "DinAB.glsl")
			return {std::error_code(), "#include <A.glsl>\n#include <B.glsl>\nD3\nD4\nD5\n"};
		if (str == "DinABC.glsl")
			return {std::error_code(), "#include <A.glsl>\n#include <B.glsl>\n#include <C.glsl>\nD4\nD5\nD6\n"};

		if (str == "FinABC.glsl")
			return {std::error_code(), "F1\n#include <A.glsl>\nF3\n#include <B.glsl>\nF5\n#include <C.glsl>\nF7\n"};

		if (str == "EinE.glsl")
			return {std::error_code(), "#include <EinE.glsl>\nE2\nE3\nE4\n"};

		if (str == "HinGinH.glsl")
			return {std::error_code(), "#include <GinHinG.glsl>\nH2"};
		if (str == "GinHinG.glsl")
			return {std::error_code(), "#include <HinGinH.glsl>\nG2"};

		if (str == "P.glsl")
			return {std::error_code(), "#pragma once\nP2"};
		if (str == "Q.glsl")
			return {std::error_code(), "#include <P.glsl>\nQ2"};

		if (str == "W.glsl")
			return {std::error_code(), "\r\n01234\r\n56789"};

		if (str == "Z.glsl")
			return {std::error_code(), "    \tZ1\n\t Z2"};

		return {std::make_error_code(std::errc::no_such_file_or_directory), "ERROR Could not find source \"" + std::string(str) + '"'};
	};

	const auto test = [&] (auto source) {
		return libv::rev::glsl_compose_from_source(includer, source).code;
	};

	CHECK(test("") == "");
	CHECK(test("M1") == "M1\n");
	CHECK(test("M1\nM2\nM3") == "M1\nM2\nM3\n");
	CHECK(test("M1\nM2\nM3\n") == "M1\nM2\nM3\n");

	CHECK(test("#include <A.glsl>\nM1\nM2\nM3\n") ==
			"#line 1 2090909000 // A.glsl\n"
			"A1\n"
			"A2\n"
			"A3\n"
			"#line 2 2090909001 // main\n"
			"M1\n"
			"M2\n"
			"M3\n");
	CHECK(test("#include \"A.glsl\"\nM1\nM2\nM3\n") ==
			"#line 1 2090909000 // A.glsl\n"
			"A1\n"
			"A2\n"
			"A3\n"
			"#line 2 2090909001 // main\n"
			"M1\n"
			"M2\n"
			"M3\n");

	CHECK(test("#include <A.glsl>") ==
			"#line 1 2090909000 // A.glsl\n"
			"A1\n"
			"A2\n"
			"A3\n"
			"#line 2 2090909001 // main\n");
	CHECK(test("#include <B.glsl>") ==
			"#line 1 2090909000 // B.glsl\n"
			"B1\n"
			"B2\n"
			"B3\n"
			"#line 2 2090909001 // main\n");
	CHECK(test("#include <C.glsl>") ==
			"#line 1 2090909000 // C.glsl\n"
			"C1\n"
			"C2\n"
			"C3\n"
			"#line 2 2090909001 // main\n");

	CHECK(test("#include <DinA.glsl>") ==
			"#line 1 2090909000 // DinA.glsl\n"
			"#line 1 2090909001 // A.glsl\n"
			"A1\n"
			"A2\n"
			"A3\n"
			"#line 2 2090909000 // DinA.glsl\n"
			"D2\n"
			"D3\n"
			"D4\n"
			"#line 2 2090909002 // main\n");
	CHECK(test("#include <DinAB.glsl>") ==
			"#line 1 2090909000 // DinAB.glsl\n"
			"#line 1 2090909001 // A.glsl\n"
			"A1\n"
			"A2\n"
			"A3\n"
			"#line 2 2090909000 // DinAB.glsl\n"
			"#line 1 2090909002 // B.glsl\n"
			"B1\n"
			"B2\n"
			"B3\n"
			"#line 3 2090909000 // DinAB.glsl\n"
			"D3\n"
			"D4\n"
			"D5\n"
			"#line 2 2090909003 // main\n");
	CHECK(test("#include <DinABC.glsl>") ==
			"#line 1 2090909000 // DinABC.glsl\n"
			"#line 1 2090909001 // A.glsl\n"
			"A1\n"
			"A2\n"
			"A3\n"
			"#line 2 2090909000 // DinABC.glsl\n"
			"#line 1 2090909002 // B.glsl\n"
			"B1\n"
			"B2\n"
			"B3\n"
			"#line 3 2090909000 // DinABC.glsl\n"
			"#line 1 2090909003 // C.glsl\n"
			"C1\n"
			"C2\n"
			"C3\n"
			"#line 4 2090909000 // DinABC.glsl\n"
			"D4\n"
			"D5\n"
			"D6\n"
			"#line 2 2090909004 // main\n");

	CHECK(test("#include <FinABC.glsl>") ==
			"#line 1 2090909000 // FinABC.glsl\n"
			"F1\n"
			"#line 1 2090909001 // A.glsl\n"
			"A1\n"
			"A2\n"
			"A3\n"
			"#line 3 2090909000 // FinABC.glsl\n"
			"F3\n"
			"#line 1 2090909002 // B.glsl\n"
			"B1\n"
			"B2\n"
			"B3\n"
			"#line 5 2090909000 // FinABC.glsl\n"
			"F5\n"
			"#line 1 2090909003 // C.glsl\n"
			"C1\n"
			"C2\n"
			"C3\n"
			"#line 7 2090909000 // FinABC.glsl\n"
			"F7\n"
			"#line 2 2090909004 // main\n");

	CHECK(test("#include <EinE.glsl>") ==
			"#line 1 2090909000 // EinE.glsl\n"
			"\n" // Empty line for failed include
			"E2\n"
			"E3\n"
			"E4\n"
			"#line 2 2090909001 // main\n");
	CHECK(test("#include <GinHinG.glsl>\nM2") ==
			"#line 1 2090909000 // GinHinG.glsl\n"
			"#line 1 2090909001 // HinGinH.glsl\n"
			"\n" // Empty line for failed include
			"H2\n"
			"#line 2 2090909000 // GinHinG.glsl\n"
			"G2\n"
			"#line 2 2090909002 // main\n"
			"M2\n");

	CHECK(test("#include <P.glsl>") ==
			"#line 1 2090909000 // P.glsl\n"
			"\n" // Empty line for #pragma once
			"P2\n"
			"#line 2 2090909001 // main\n");
	CHECK(test(
			"#include <P.glsl>\n"
			"#include <P.glsl>")
			==
			"#line 1 2090909000 // P.glsl\n"
			"\n" // Empty line for #pragma once
			"P2\n"
			"#line 2 2090909001 // main\n"
			"\n");
	CHECK(test(
			"#include <P.glsl>\n"
			"#include <P.glsl>\n"
			"#include <P.glsl>")
			==
			"#line 1 2090909000 // P.glsl\n"
			"\n" // Empty line for #pragma once
			"P2\n"
			"#line 2 2090909001 // main\n"
			"\n" // Empty line for failed include
			"\n"); // Empty line for failed include

	CHECK(test("#include <P.glsl>\n"
			"#include <Q.glsl>")
			==
			"#line 1 2090909000 // P.glsl\n"
			"\n" // Empty line for #pragma once
			"P2\n"
			"#line 2 2090909001 // main\n"
			"#line 1 2090909002 // Q.glsl\n"
			"\n" // Empty line for failed include
			"Q2\n"
			"#line 3 2090909001 // main\n");
	CHECK(test("#include <Q.glsl>\n"
			"#include <Q.glsl>")
			==
			"#line 1 2090909000 // Q.glsl\n"
			"#line 1 2090909001 // P.glsl\n"
			"\n" // Empty line for #pragma once
			"P2\n"
			"#line 2 2090909000 // Q.glsl\n"
			"Q2\n"
			"#line 2 2090909002 // main\n"
			"#line 1 2090909000 // Q.glsl\n"
			"\n" // Empty line for failed include
			"Q2\n"
			"#line 3 2090909002 // main\n");

	CHECK(test("#include <W.glsl>") ==
			"#line 1 2090909000 // W.glsl\n"
			"\n" // Empty line in source
			"01234\n"
			"56789\n"
			"#line 2 2090909001 // main\n");
	CHECK(test("#include <Z.glsl>") ==
			"#line 1 2090909000 // Z.glsl\n"
			"    \tZ1\n"
			"\t Z2\n"
			"#line 2 2090909001 // main\n");
}
