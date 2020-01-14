// File: VecTest.cpp, Created on 2014. december 8. 2:19, Author: Vader

// hpp
#include <catch/catch.hpp>
// pro
#include <libv/gl/glsl_compiler.hpp>


// -------------------------------------------------------------------------------------------------

namespace {



} // namespace

// -------------------------------------------------------------------------------------------------

#include <iostream>
#include <libv/log/log.hpp>
TEST_CASE("enable logger", "[libv.gl.glsl_compiler]") {
	std::cout << libv::logger_stream;
}

TEST_CASE("GLSL Include pre-processing", "[libv.gl.glsl_compiler]") {
	libv::gl::GLSLCompiler compiler([](const auto& str) -> libv::gl::IncludeResult {
		if (str == "A.glsl")
			return {true, "A1\nA2\nA3"}; // No newline at the end
		if (str == "B.glsl")
			return {true, "B1\nB2\nB3\n"};
		if (str == "C.glsl")
			return {true, "C1\nC2\nC3\n"};

		if (str == "DinA.glsl")
			return {true, "#include <A.glsl>\nD2\nD3\nD4\n"};
		if (str == "DinAB.glsl")
			return {true, "#include <A.glsl>\n#include <B.glsl>\nD3\nD4\nD5\n"};
		if (str == "DinABC.glsl")
			return {true, "#include <A.glsl>\n#include <B.glsl>\n#include <C.glsl>\nD4\nD5\nD6\n"};

		if (str == "FinABC.glsl")
			return {true, "F1\n#include <A.glsl>\nF3\n#include <B.glsl>\nF5\n#include <C.glsl>\nF7\n"};

		if (str == "EinE.glsl")
			return {true, "#include <EinE.glsl>\nE2\nE3\nE4\n"};

		if (str == "HinGinH.glsl")
			return {true, "#include <GinHinG.glsl>\nH2"};
		if (str == "GinHinG.glsl")
			return {true, "#include <HinGinH.glsl>\nG2"};

		if (str == "P.glsl")
			return {true, "#pragma once\nP1"};
		if (str == "Q.glsl")
			return {true, "#include <P.glsl>\nQ1"};

		if (str == "W.glsl")
			return {true, "\r\n01234\r\n56789"};

		if (str == "Z.glsl")
			return {true, "    \tZ1\n\t Z2"};

		return {false, "ERROR Could not find source \"" + std::string(str) + '"'};
	});

	const auto test = [&] (auto source) {
		return compiler.compile(source, "main");
	};

	CHECK(test("") == "");
	CHECK(test("M1") == "M1\n");
	CHECK(test("M1\nM2\nM3") == "M1\nM2\nM3\n");
	CHECK(test("M1\nM2\nM3\n") == "M1\nM2\nM3\n");

	CHECK(test("#include <A.glsl>\nM1\nM2\nM3\n") == "#line 1 \"A.glsl\"\nA1\nA2\nA3\n#line 2 \"main\"\nM1\nM2\nM3\n");
	CHECK(test("#include \"A.glsl\"\nM1\nM2\nM3\n") == "#line 1 \"A.glsl\"\nA1\nA2\nA3\n#line 2 \"main\"\nM1\nM2\nM3\n");

	CHECK(test("#include <A.glsl>") == "#line 1 \"A.glsl\"\nA1\nA2\nA3\n#line 2 \"main\"\n");
	CHECK(test("#include <B.glsl>") == "#line 1 \"B.glsl\"\nB1\nB2\nB3\n#line 2 \"main\"\n");
	CHECK(test("#include <C.glsl>") == "#line 1 \"C.glsl\"\nC1\nC2\nC3\n#line 2 \"main\"\n");

	CHECK(test("#include <DinA.glsl>") == "#line 1 \"DinA.glsl\"\n#line 1 \"A.glsl\"\nA1\nA2\nA3\n#line 2 \"DinA.glsl\"\nD2\nD3\nD4\n#line 2 \"main\"\n");
	CHECK(test("#include <DinAB.glsl>") == "#line 1 \"DinAB.glsl\"\n#line 1 \"A.glsl\"\nA1\nA2\nA3\n#line 2 \"DinAB.glsl\"\n#line 1 \"B.glsl\"\nB1\nB2\nB3\n#line 3 \"DinAB.glsl\"\nD3\nD4\nD5\n#line 2 \"main\"\n");
	CHECK(test("#include <DinABC.glsl>") == "#line 1 \"DinABC.glsl\"\n#line 1 \"A.glsl\"\nA1\nA2\nA3\n#line 2 \"DinABC.glsl\"\n#line 1 \"B.glsl\"\nB1\nB2\nB3\n#line 3 \"DinABC.glsl\"\n#line 1 \"C.glsl\"\nC1\nC2\nC3\n#line 4 \"DinABC.glsl\"\nD4\nD5\nD6\n#line 2 \"main\"\n");

	CHECK(test("#include <FinABC.glsl>") == "#line 1 \"FinABC.glsl\"\nF1\n#line 1 \"A.glsl\"\nA1\nA2\nA3\n#line 3 \"FinABC.glsl\"\nF3\n#line 1 \"B.glsl\"\nB1\nB2\nB3\n#line 5 \"FinABC.glsl\"\nF5\n#line 1 \"C.glsl\"\nC1\nC2\nC3\n#line 7 \"FinABC.glsl\"\nF7\n#line 2 \"main\"\n");

	CHECK(test("#include <EinE.glsl>") == "#line 1 \"EinE.glsl\"\nE2\nE3\nE4\n#line 2 \"main\"\n");
	CHECK(test("#include <GinHinG.glsl>\nM2") == "#line 1 \"GinHinG.glsl\"\n#line 1 \"HinGinH.glsl\"\nH2\n#line 2 \"GinHinG.glsl\"\nG2\n#line 2 \"main\"\nM2\n");

	CHECK(test("#include <P.glsl>") == "#line 1 \"P.glsl\"\nP1\n#line 2 \"main\"\n");
	CHECK(test("#include <P.glsl>\n#include <P.glsl>") == "#line 1 \"P.glsl\"\nP1\n#line 2 \"main\"\n");
	CHECK(test("#include <P.glsl>\n#include <P.glsl>\n#include <P.glsl>") == "#line 1 \"P.glsl\"\nP1\n#line 2 \"main\"\n");

	CHECK(test("#include <P.glsl>\n#include <Q.glsl>") == "#line 1 \"P.glsl\"\nP1\n#line 2 \"main\"\n#line 1 \"Q.glsl\"\nQ1\n#line 3 \"main\"\n");
	CHECK(test("#include <Q.glsl>\n#include <Q.glsl>") == "#line 1 \"Q.glsl\"\n#line 1 \"P.glsl\"\nP1\n#line 2 \"Q.glsl\"\nQ1\n#line 2 \"main\"\n#line 1 \"Q.glsl\"\nQ1\n#line 3 \"main\"\n");

	CHECK(test("#include <W.glsl>") == "#line 1 \"W.glsl\"\n\n01234\n56789\n#line 2 \"main\"\n");
	CHECK(test("#include <Z.glsl>") == "#line 1 \"Z.glsl\"\n    \tZ1\n\t Z2\n#line 2 \"main\"\n");
}
