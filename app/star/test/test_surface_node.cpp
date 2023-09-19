// Project: libv, File: app/star/test/test_surface_node.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// std
#include <sstream>
// pro
#include <star/game/surface/surface_node.hpp>

std::string make_string_compact(std::string str) {
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	return str;
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("GLSL Script generation: NodeConstant", "[star.game.surface]") {
	const star::NodeConstant node{10.0f};
	const star::SurfaceNodeContext magic;

	// vec2 c0 = uv;
	CHECK(node.glsl(magic) == "const float v0 = 10.0f;");
}


TEST_CASE("GLSL Script generation: NodePlus", "[star.game.surface]") {
	const star::NodeConstant node1{10.0f};
	const star::NodeConstant node2{5.0f};
	auto container = star::NodePlus::container();
	container.emplace_back(std::make_unique<star::NodeConstant>(node1));
	container.emplace_back(std::make_unique<star::NodeConstant>(node2));
	const star::NodePlus node3(std::move(container));
	const star::SurfaceNodeContext magic;

	// vec2 c0 = uv;
	CHECK(node3.glsl(magic) == "const float v0 = 10.0f;");
}

TEST_CASE("GLSL Script generation: Utility", "[star.game.surface]") {
	CHECK(make_string_compact("\t   asd") == " asd");
}


