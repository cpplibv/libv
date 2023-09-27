// Project: libv.noise, File: test/libv/noise/test_glsl_cpp_noise_consistency.cpp

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
#include <libv/noise/noise.hpp>


#include <iostream>

// -------------------------------------------------------------------------------------------------

const std::string& runner_shader_code();

libv::vec2z test_size{256, 256};

// -------------------------------------------------------------------------------------------------

struct EvalResult {
	float max_abs_diff = 0.f;
	float avg_abs_diff = 0.f;
	float sum_abs_diff = 0.f;
	float sum_diff = 0.f;
	float abs_sum_diff = 0.f;
};

template <typename T>
EvalResult eval(libv::GL& gl, std::string_view glsl_function, T&& function, uint32_t numChannels = 1) {
	struct GL_RAII {
		libv::GL& gl;
		libv::gl::Texture2D texture;
		libv::gl::Shader shader;
		libv::gl::Program program;

		explicit GL_RAII(libv::GL& gl) :
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

	const auto uloc_global_seed = gl(g.program).uniformLocation("global_seed");
	gl(g.program).use();

	{
		const uint32_t global_seed = 0x5EED;

		gl.bindImageTexture(0, g.texture, 0, libv::gl::BufferAccessFull::Write, libv::gl::FormatSized::RGBA32F);
		libv::gl::uniform(uloc_global_seed, global_seed);
		gl.dispatchCompute(static_cast<uint32_t>(test_size.x / 8), static_cast<uint32_t>(test_size.y / 8));

		libv::vector_2D<libv::vec4f> result_cpu{test_size, libv::uninitialized};
		libv::vector_2D<libv::vec4f> result_gpu{test_size, libv::uninitialized};

		for (uint32_t y = 0; y < test_size.y; ++y) {
			const auto frequency_offset = static_cast<float>(y / 8 + 1);
			const float yf = static_cast<float>(y) / static_cast<float>(test_size.y);
			for (uint32_t x = 0; x < test_size.x; ++x) {
				const auto seed_offset = x / 8;
				const float xf = static_cast<float>(x) / static_cast<float>(test_size.x);
				result_cpu(x, y) = libv::vec4f{function(global_seed + seed_offset, xf * frequency_offset, yf * frequency_offset)};
			}
		}

		gl.memoryBarrier(libv::gl::BarrierBit::ShaderImageAccess);
		gl.readTextureImage(g.texture, 0, libv::gl::ReadFormat::RGBA, libv::gl::DataType::F32, result_gpu.data());

		for (uint32_t y = 0; y < test_size.y; ++y) {
			for (uint32_t x = 0; x < test_size.x; ++x) {
				for (uint32_t c = 0; c < numChannels; ++c) {
					const auto cpu = result_cpu(x, y)[c];
					const auto gpu = result_gpu(x, y)[c];
					const auto diff = gpu - cpu;
					const auto abs_diff = std::abs(diff);

					if (abs_diff > result.max_abs_diff)
						result.max_abs_diff = abs_diff;
					result.sum_abs_diff += abs_diff;
					result.sum_diff += diff;
					// fmt::print("x: {} y: {} CPU: {:7.5f} GPU: {:7.5f} Diff: {: 10.8f}\n", x, y, cpu, gpu, diff);
				}
			}
		}

		result.avg_abs_diff = result.sum_abs_diff / static_cast<float>(test_size.x) / static_cast<float>(test_size.y);
		result.abs_sum_diff = std::abs(result.sum_diff);
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

	libv::GL gl;
	gl.enableDebug();

	// Execute the tests:

	{
		const auto metrics = eval(gl, "value(seed, uv), 0, 0, 0", libv::noise::value);
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "value 2D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000001788f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000109f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0007141792f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0002089838f * 1.5f);
	} {
		const auto metrics = eval(gl, "perlin(seed, uv), 0, 0, 0", libv::noise::perlin);
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "perlin 2D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000002682f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000249f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0016331940f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0001006823f * 1.5f);
	} {
		const auto metrics = eval(gl, "simplex(seed, uv), 0, 0, 0", libv::noise::simplex);
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "simplex 2D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000214353f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000012064f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0790653676f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0038014713f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_euclidean, cellular_return_cellvalue, 1.0), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::cellvalue, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D euclidean cellvalue", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000000001f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000001f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0000000001f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000000001f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_euclidean, cellular_return_distance, 1.0), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D euclidean distance", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000001192f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000116f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0007621646f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0003847480f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_euclidean, cellular_return_distance, 0.25), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance, 0.25f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D euclidean distance 0.25", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000001192f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000094f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0006165504f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0003367662f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_euclideansq, cellular_return_distance2, 1.0), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::euclideansq, libv::noise::CellularReturn::distance2, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D euclideansq distance2", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000003576f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000179f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0011711121f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000808239f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_manhattan, cellular_return_distance2add, 1.0), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::manhattan, libv::noise::CellularReturn::distance2add, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D manhattan distance2add", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0939140320f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000064143f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.4203691483f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0521994829f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_hybrid, cellular_return_distance2sub, 1.0), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::hybrid, libv::noise::CellularReturn::distance2sub, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D hybrid distance2sub", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000007153f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000686f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0044928193f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000556111f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_euclidean, cellular_return_distance2mul, 1.0), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance2mul, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D euclidean distance2mul", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000001788f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000110f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0007233024f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0002639890f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, uv, cellular_distance_euclidean, cellular_return_distance2div, 1.0), 0, 0, 0",
				libv::noise::cellular(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance2div, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 2D euclidean distance2div", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000003278f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000386f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0025265494f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0009163731f * 1.5f);
	} {
		const auto metrics = eval(gl, "simplex_grad(seed, uv), 0, 0", [](auto seed, float x, float y) {
			return libv::vec4f{libv::noise::simplex_grad(seed, x, y), 0, 0};
		}, 2);
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "simplex_grad 2D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000002169f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000161f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0010523195f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000096193f * 1.5f);
	}

	{
		const auto metrics = eval(gl, "value(seed, vec3(uv, uv.x * 0.5 - 5.0)), 0, 0, 0", [](auto seed, float x, float y) {
			return libv::noise::value(seed, x, y, x * 0.5f - 5.0f);
		});
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "value 3D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000002980f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000159f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0010392759f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000167174f * 1.5f);
	} {
		const auto metrics = eval(gl, "perlin(seed, vec3(uv, uv.x * 0.5 - 5.0)), 0, 0, 0", [](auto seed, float x, float y) {
			return libv::noise::perlin(seed, x, y, x * 0.5f - 5.0f);
		});
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "perlin 3D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000002235f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000139f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0009136040f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000026493f * 1.5f);
	} {
		const auto metrics = eval(gl, "simplex(seed, vec3(uv, uv.x * 0.5 - 5.0)), 0, 0, 0", [](auto seed, float x, float y) {
			return libv::noise::simplex(seed, x, y, x * 0.5f - 5.0f);
		});
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "simplex 3D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0026926557f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000001825f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0119635751f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0046866857f * 1.5f);
	}

	const auto cellular3D = [](libv::noise::CellularDistance dist, libv::noise::CellularReturn ret, float jitter = 1.0f) {
		return [=](auto seed, float x, float y) {
			return libv::noise::cellular(seed, x, y, x * 0.5f - 5.0f, dist, ret, jitter);
		};
	};

	{
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_euclidean, cellular_return_cellvalue, 1.0), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::cellvalue, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D euclidean cellvalue", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000000001f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000001f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0000000001f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000000001f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_euclidean, cellular_return_distance, 1.0), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D euclidean distance", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000001192f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000139f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0009099841f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0004538894f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_euclidean, cellular_return_distance, 0.25), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance, 0.25f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D euclidean distance 0.25", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000001192f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000117f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0007647276f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0004464388f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_euclideansq, cellular_return_distance2, 1.0), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::euclideansq, libv::noise::CellularReturn::distance2, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D euclideansq distance2", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000004768f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000192f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0012560487f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000171065f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_manhattan, cellular_return_distance2add, 1.0), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::manhattan, libv::noise::CellularReturn::distance2add, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D manhattan distance2add", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0374628901f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000016428f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.1076636910f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0895094275f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_hybrid, cellular_return_distance2sub, 1.0), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::hybrid, libv::noise::CellularReturn::distance2sub, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D hybrid distance2sub", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000007153f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000857f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0056177378f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000590086f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_euclidean, cellular_return_distance2mul, 1.0), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance2mul, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D euclidean distance2mul", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000001788f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000131f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0008578897f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0003206134f * 1.5f);
	} {
		const auto metrics = eval(gl, "cellular(seed, vec3(uv, uv.x * 0.5 - 5.0), cellular_distance_euclidean, cellular_return_distance2div, 1.0), 0, 0, 0",
				cellular3D(libv::noise::CellularDistance::euclidean, libv::noise::CellularReturn::distance2div, 1.0f));
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "cellular 3D euclidean distance2div", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000002980f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000426f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0027911032f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0009998925f * 1.5f);
	}{
		const auto metrics = eval(gl, "simplex_grad(seed, vec3(uv, uv.x * 0.5 - 5.0)), 0", [](auto seed, float x, float y) {
			return libv::vec4f{libv::noise::simplex_grad(seed, x, y, x * 0.5f - 5.0f), 0};
		}, 3);
		fmt::print("{:<35} | max_abs_diff {: 12.10f}, avg_abs_diff {: 12.10f}, sum_abs_diff {: 12.10f}, sum_diff {: 12.10f}\n", "simplex_grad 3D", metrics.max_abs_diff, metrics.avg_abs_diff, metrics.sum_abs_diff, metrics.sum_diff);
		CHECK(metrics.max_abs_diff < 0.0000568619f * 1.5f);
		CHECK(metrics.avg_abs_diff < 0.0000000094f * 1.5f);
		CHECK(metrics.sum_abs_diff < 0.0006139706f * 1.5f);
		CHECK(metrics.abs_sum_diff < 0.0000124975f * 1.5f);
	}
}

// -------------------------------------------------------------------------------------------------

const std::string& runner_shader_code() {
	static std::string code = [] {
		const auto shader_main = R"(
#version 450 core

#include <lib/noise/noise.glsl>

layout(local_size_x = 8, local_size_y = 8) in;

layout(rgba32f, binding = 0) uniform image2D target;
uniform uint global_seed = 0;

void main() {
	ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 coordsSize = ivec2(gl_NumWorkGroups.xy * gl_WorkGroupSize.xy);
	uint seed_offset = uint(coords.x) / 8;

	float frequency_offset = float(coords.y / 8 + 1);
	vec2 uv = vec2(coords) / vec2(coordsSize) * frequency_offset;

	uint seed = global_seed + seed_offset;
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
