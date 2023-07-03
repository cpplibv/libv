// Project: libv.math, File: test/libv/math/test_quat.cpp

// hpp
#include <catch2/catch_test_macros.hpp>
// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// libv
#include <libv/algo/replace_all.hpp>
#include <libv/container/vector_2d.hpp>
#include <libv/gl/gl.hpp>
#include <libv/gl/program.hpp>
#include <libv/gl/shader.hpp>
#include <libv/gl/texture.hpp>
#include <libv/rev/glsl/glsl_compose.hpp>
#include <libv/utility/approx.hpp>
#include <libv/utility/read_file.hpp>
// pro
#include <libv/noise/noise_2.hpp>


#include <iostream>

// -------------------------------------------------------------------------------------------------

const std::string& runner_shader_code();

const uint32_t test_seeds[] = {
		0x5EED,
		0,
		1,
};

libv::vec2z test_size{256, 256};

// -------------------------------------------------------------------------------------------------

struct EvalResult {
	float max_discrepancy = 0.f;
	float avg_discrepancy = 0.f;
	float sum_discrepancy = 0.f;
	float sum_difference = 0.f;
};
template <typename T>
EvalResult eval(libv::gl::GL& gl, std::string_view glsl_function, T&& function) {
	struct GL_RAII {
		libv::gl::GL& gl;
		libv::gl::Texture2D texture;
		libv::gl::Shader shader;
		libv::gl::Program program;

		explicit GL_RAII(libv::gl::GL& gl) :
			gl(gl) {}
		~GL_RAII() {
			if (program.id != 0)
				gl(program).destroy();
			if (shader.id != 0)
				gl(shader).destroy();
			if (texture.id != 0) {
				gl(texture).unbind();
				gl(texture).destroy();
			}
		}
	} g{gl};

	EvalResult result;

	gl(g.texture).create();
	gl(g.texture).bind();
	gl(g.texture).storage(1, libv::gl::FormatSized::RGBA32F, static_cast<int>(test_size.x), static_cast<int>(test_size.y));

	gl(g.shader).create(libv::gl::ShaderType::Compute);
	const auto glsl_source = libv::replace_all_copy(runner_shader_code(), "%NOISE_FUNCTION%", glsl_function);
	gl(g.shader).compile(glsl_source);
	if (!gl(g.shader).status())
		throw std::runtime_error(gl(g.shader).info());

	gl(g.program).create();
	gl(g.program).link(g.shader);
	if (!gl(g.program).status())
		throw std::runtime_error(gl(g.program).info());

	const auto uloc_seed = gl(g.program).uniformLocation("seed");
	gl(g.program).use();

	{
		const auto seed = test_seeds[0];

		gl.bindImageTexture(0, g.texture, 0, libv::gl::BufferAccessFull::Write, libv::gl::FormatSized::RGBA32F);
		libv::gl::uniform(uloc_seed, seed);
		gl.dispatchCompute(static_cast<uint32_t>(test_size.x / 8), static_cast<uint32_t>(test_size.y / 8));

		libv::vector_2D<libv::vec4f> result_cpu{test_size, libv::uninitialized};
		libv::vector_2D<libv::vec4f> result_gpu{test_size, libv::uninitialized};

		for (uint32_t y = 0; y < test_size.y; ++y) {
			const float yf = static_cast<float>(y) / static_cast<float>(test_size.y);
			for (uint32_t x = 0; x < test_size.x; ++x) {
				const float xf = static_cast<float>(x) / static_cast<float>(test_size.x);
				result_cpu(x, y) = libv::vec4f{function(seed, xf, yf)};
			}
		}

		gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);
		gl.readTextureImage(g.texture, 0, libv::gl::ReadFormat::RGBA, libv::gl::DataType::F32, result_gpu.data());

		for (uint32_t y = 0; y < test_size.y; ++y) {
			for (uint32_t x = 0; x < test_size.x; ++x) {
				const auto cpu = result_cpu(x, y).x;
				const auto gpu = result_gpu(x, y).x;
				const auto diff = gpu - cpu;

				if (std::abs(diff) > result.max_discrepancy)
					result.max_discrepancy = std::abs(diff);
				result.sum_discrepancy += std::abs(diff);
				result.sum_difference += diff;
				// fmt::print("x: {} y: {} CPU: {:7.5f} GPU: {:7.5f} Diff: {: 10.8f}\n", x, y, cpu, gpu, diff);
			}
		}

		result.avg_discrepancy = result.sum_discrepancy / static_cast<float>(test_size.x) / static_cast<float>(test_size.y);
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

TEST_CASE("Test GLSL and C++ noise consistency", "[libv.noise]") {
	std::cerr << libv::logger_stream;
	libv::logger_stream.deny_below(libv::Logger::Severity::Info);

	struct GLFW_RAII {
		GLFWwindow* window = nullptr;

		GLFW_RAII() {
			glfwSetErrorCallback([](int code, const char* description) {
				throw std::runtime_error(fmt::format("GLFW {}: {}", code, description));
			});
			if (!glfwInit())
				throw std::runtime_error("Failed to initialize GLFW.");
		}
		~GLFW_RAII() {
			if (window != nullptr) {
				glfwMakeContextCurrent(nullptr);
				glfwDestroyWindow(window);
			}
			glfwTerminate();
		}
	} glfw;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, false);

	glfw.window = glfwCreateWindow(100, 100, "unittest", nullptr, nullptr);
	glfwMakeContextCurrent(glfw.window);

	libv::gl::GL gl;
	gl.enableDebug();

	// Execute the tests:

	const auto r2 = eval(gl, "value(seed, uv), 0, 0, 0", libv::noise::value);
	fmt::print("value   | max_discrepancy {: 12.10f}, avg_discrepancy {: 12.10f}, sum_discrepancy {: 12.10f}, sum_difference {: 12.10f}\n", r2.max_discrepancy, r2.avg_discrepancy, r2.sum_discrepancy, r2.sum_difference);

	const auto r0 = eval(gl, "perlin(seed, uv), 0, 0, 0", libv::noise::perlin);
	fmt::print("perlin  | max_discrepancy {: 12.10f}, avg_discrepancy {: 12.10f}, sum_discrepancy {: 12.10f}, sum_difference {: 12.10f}\n", r0.max_discrepancy, r0.avg_discrepancy, r0.sum_discrepancy, r0.sum_difference);

	const auto r1 = eval(gl, "simplex(seed, uv), 0, 0, 0", libv::noise::simplex);
	fmt::print("simplex | max_discrepancy {: 12.10f}, avg_discrepancy {: 12.10f}, sum_discrepancy {: 12.10f}, sum_difference {: 12.10f}\n", r1.max_discrepancy, r1.avg_discrepancy, r1.sum_discrepancy, r1.sum_difference);


	CHECK(eval(gl, "perlin(seed, uv), 0, 0, 0", libv::noise::perlin).sum_difference < 0.1f);
	CHECK(eval(gl, "perlin(seed, uv), 0, 0, 0", libv::noise::perlin).max_discrepancy < 0.00001f);

	// value
	//		2D / 3D
	// perlin
	//		2D / 3D
	// simplex
	//		2D / 3D
	// simplexGradient
	//		2D / 3D
	// cellular
	//		2D / 3D
	// 		enum class CellularDistanceFunction {
	// 			euclidean = 0,
	// 			euclideanSq = 1,
	// 			manhattan = 2,
	// 			hybrid = 3,
	// 		};
	// 		enum class CellularReturnType {
	// 			cellValue = 0,
	// 			distance = 1,
	// 			distance2 = 2,
	// 			distance2Add = 3,
	// 			distance2Sub = 4,
	// 			distance2Mul = 5,
	// 			distance2Div = 6,
	// 		};
	// 		jitter
	// 			0.f
	// 			0.25f
	// 			0.5f
	// 			0.95f
	// 			1.f
	//
}

// -------------------------------------------------------------------------------------------------

const std::string& runner_shader_code() {
	static std::string code = []{
		const auto shader_main = R"(
#version 450 core

#include <lib/noise/noise.glsl>

layout(local_size_x = 8, local_size_y = 8) in;

layout(rgba32f, binding = 0) uniform image2D target;
uniform uint seed = 0;

void main() {
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 coordsSize = ivec2(gl_NumWorkGroups.xy * gl_WorkGroupSize.xy);
	vec2 uv = vec2(coords) / vec2(coordsSize);

	vec4 result = vec4(%NOISE_FUNCTION%);
	imageStore(target, coords, result);
}
)";
		const auto loader = [](const std::string_view filepath) -> libv::rev::IncludeResult {
			libv::rev::IncludeResult result;
			result.result = libv::read_file_str(std::string(WISH_PATH_TO_SOURCE) + "res/shader/" + std::string(filepath), result.ec);
			return result;
		};
		return libv::rev::glsl_compose_from_source(loader, shader_main).code;
	}();

	return code;
}
