// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
#include <boost/filesystem.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
// libv
#include <libv/timer.hpp>
#include <libv/worker_thread.hpp>
// std
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
// pro
#include <libv/gl/gl.hpp>
#include <libv/gl/log.hpp>
#include <libv/gl/model.hpp>
#include <libv/gl/shader.hpp>
#include <libv/gl/texture.hpp>
#include <libv/gl/uniform.hpp>

// -------------------------------------------------------------------------------------------------

float angle = 0;

std::atomic_bool running{true};

// -------------------------------------------------------------------------------------------------
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 900

#define checkGLEWSupport(ext) VLOG_INFO(libv::log(), "GLEW: %-40s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")

void initGLEW() {
	if (GLenum err = glewInit() != GLEW_OK)
		VLOG_ERROR(libv::log(), "Failed to initialize glew: %s", (const char*) glewGetErrorString(err));

	VLOG_INFO(libv::log(), "GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
	VLOG_INFO(libv::log(), "GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
	VLOG_INFO(libv::log(), "GL Version: %s", (const char*) glGetString(GL_VERSION));

	checkGLEWSupport(GL_VERSION_3_3);
	checkGLEWSupport(GL_VERSION_4_5);
	checkGLEWSupport(GL_ARB_draw_elements_base_vertex);
	checkGLEWSupport(GL_ARB_gpu_shader_fp64);

	checkGL();
}

void initGL() {
	glEnable(GL_DEPTH_TEST); //Depth
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
	glEnable(GL_BLEND); //Alpha
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW); //Counter clockwise polys only

	glEnable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, true ? GL_FILL : GL_LINE);
	checkGL();
}

void initGLSL() {
	checkGL();
}

static void error_callback(int code, const char* description) {
	VLOG_ERROR(libv::log(), "GLFW %d: %s", code, description);
}

// -------------------------------------------------------------------------------------------------
struct Example {
	libv::gl::GL gl;
	libv::gl::ShaderProgram shaderDebug;
	libv::gl::ShaderProgram shaderDepth;
	libv::gl::Texture texture0;
	libv::gl::Texture texture1;
	libv::gl::Texture texture2;
	libv::gl::Model model0;
	libv::gl::Model model1;
	libv::gl::Model model2;
	libv::gl::Model model3;
	libv::gl::Model model4;

	libv::gl::Uniform<glm::mat4> glslMVPmat;
	libv::gl::Uniform<glm::mat4> glslMmat;
	libv::gl::Uniform<libv::gl::TextureType> glslTextureDiffuseSampler;
	libv::gl::Uniform<libv::gl::TextureType> glslTextureNormalSampler;
	libv::gl::Uniform<libv::gl::TextureType> glslTextureAmbientSampler;

	Example() :
		shaderDebug("Debug 0",
		std::make_shared<libv::gl::ShaderVertex>("Data/Shader/debug0.vs"),
		std::make_shared<libv::gl::ShaderFragment>("Data/Shader/debug0.fs")),
		shaderDepth("Depth 0",
		std::make_shared<libv::gl::ShaderVertex>("Data/Shader/depth.vs"),
		std::make_shared<libv::gl::ShaderFragment>("Data/Shader/depth.fs")),
		texture0("Data/Texture/asteorid_02_diffuse.dds"),
		texture1("Data/Texture/asteorid_02_normal.dds"),
		texture2("Data/Texture/asteorid_02_ambient.dds"),

		model0("Data/Model/test_group.dae.pb.vm3"),
		model1("Data/Model/fighter_01_eltanin.dae.pb.vm3"),
		model2("Data/Model/test_sp.dae.pb.vm3"),
		model3("Data/Model/projectile_missile_01_hellfire.0001.dae.pb.vm3"),
		model4("Data/Model/asteroid_02.dae.pb.vm3"),

		glslMVPmat(shaderDebug, "MVPmat"),
		glslMmat(shaderDebug, "Mmat"),
		glslTextureDiffuseSampler(shaderDebug, "textureDiffuseSampler"),
		glslTextureNormalSampler(shaderDebug, "textureNormalSampler"),
		glslTextureAmbientSampler(shaderDebug, "textureAmbientSampler") {
		checkGL();

		shaderDebug.use();
		glslTextureDiffuseSampler = libv::gl::TextureType::diffuse;
		glslTextureNormalSampler = libv::gl::TextureType::normal;
		glslTextureAmbientSampler = libv::gl::TextureType::ambient;
		checkGL();
	}
	void render();
};

// -------------------------------------------------------------------------------------------------

void Example::render() {
	glClearColor(0.236f, 0.311f, 0.311f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl.pushMatrixProjection();
	gl.pushMatrixView();
	gl.pushMatrixModel();

	gl.matrixProjection() = glm::perspective(1.f, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 1.f, 1000.f);
	gl.matrixView() = glm::lookAt(glm::vec3(20.f, 15.f, 20.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	gl.matrixView() *= glm::rotate(angle / 90, glm::vec3(0, 1, 0));

	// ---------------------------------------------------------------------------------------------

	shaderDebug.use();
	checkGL();

	glslMVPmat = gl.matrixMVP();
	glslMmat = gl.matrixModel();

	// TODO P4: Binding a texture to an uniform instead of TextureType sounds like a good idea.

	// ---------------------------------------------------------------------------------------------

	//	texture0.bind(libv::gl::TextureType::diffuse);
	//	texture1.bind(libv::gl::TextureType::normal);
	//	texture2.bind(libv::gl::TextureType::ambient);
	//	checkGL();
	//
	//	libv::gl::renderCube(-9, 0, 0, 4.0f);
	//	libv::gl::renderCube(0, 0, 0, 5.0f);
	//	libv::gl::renderCube(11, 0, 0, 6.0f);
	//
	//	texture2.unbind(libv::gl::TextureType::ambient);
	//	texture1.unbind(libv::gl::TextureType::normal);
	//	texture0.unbind(libv::gl::TextureType::diffuse);
	//	checkGL();

	// ---------------------------------------------------------------------------------------------

	texture0.bind(libv::gl::TextureType::diffuse);
	texture1.bind(libv::gl::TextureType::normal);
	texture2.bind(libv::gl::TextureType::ambient);

	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, -20));
	model0.render(gl);
	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	model1.render(gl);
	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	model2.render(gl);
	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	model3.render(gl);
	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	model4.render(gl);

	texture2.unbind(libv::gl::TextureType::ambient);
	texture1.unbind(libv::gl::TextureType::normal);
	texture0.unbind(libv::gl::TextureType::diffuse);

	// ---------------------------------------------------------------------------------------------

	gl.popMatrixModel();
	gl.popMatrixView();
	gl.popMatrixProjection();

	checkGL();
}

// -------------------------------------------------------------------------------------------------

void init() {
	initGLEW();
	initGL();
	initGLSL();
}

int main(void) {
	libv::log().output(std::cout);

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		VLOG_ERROR(libv::log(), "Failed to initialize GLFW.");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window;

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		VLOG_ERROR(libv::log(), "Failed to create GLFW window.");
		exit(EXIT_FAILURE);
	}
	glfwSetWindowPos(window, 200, 200);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	init();

	{
		Example example;

		libv::Timer timer;
		size_t time = 0, i = 0;


		while (!glfwWindowShouldClose(window)) {
			checkGL();
			example.render();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

			i++;
			time += timer.time().count();
			if (time > 1'000'000'000) {
				VLOG_INFO(libv::log(), "FPS: %d", i);
				i = 0;
				time -= 1'000'000'000;
			}
		}
	}
	glfwMakeContextCurrent(nullptr);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

//	std::this_thread::sleep_for(std::chrono::seconds(2));



// =================================================================================================
// =================================================================================================
// =================================================================================================


//// -------------------------------------------------------------------------------------------------
//struct MyFrame {
//	GLFWwindow* window;
//	const char* title;
//	float r, g, b;
//	std::thread* th;
//	libv::gl::GL gl;
//	//	libv::WorkerThread thread;
//
//	MyFrame(GLFWwindow* window, const char* title, float r, float g, float b, std::thread* th) :
//		window(window),
//		title(title),
//		r(r), g(g), b(b),
//		th(th) { }
//};
//
//// -------------------------------------------------------------------------------------------------
//
//std::unique_ptr<libv::WorkerThread> threadIO;
//
//std::shared_ptr<libv::gl::ShaderProgram> shaderDebug, shaderDepth;
//std::shared_ptr<libv::gl::Texture> texture0, texture1, texture2;
////std::shared_ptr<libv::gl::Model> model0, model1, model2, model3, model4;
//
//float angle = 0;
//
//std::atomic_bool running{true};
//
//// -------------------------------------------------------------------------------------------------
//
//#define checkGLEWSupport(ext) VLOG_INFO(libv::log(), "GLEW: %-40s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")
//
//void initGLEW() {
//	if (GLenum err = glewInit() != GLEW_OK)
//		VLOG_ERROR(libv::log(), "Failed to initialize glew: %s", (const char*) glewGetErrorString(err));
//
//	VLOG_INFO(libv::log(), "GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
//	VLOG_INFO(libv::log(), "GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
//	VLOG_INFO(libv::log(), "GL Version: %s", (const char*) glGetString(GL_VERSION));
//
//	checkGLEWSupport(GL_VERSION_3_3);
//	checkGLEWSupport(GL_VERSION_4_5);
//	checkGLEWSupport(GL_ARB_draw_elements_base_vertex);
//	checkGLEWSupport(GL_ARB_gpu_shader_fp64);
//
//	checkGL();
//}
//
//void initGL() {
//	glEnable(GL_DEPTH_TEST); //Depth
//	glDepthFunc(GL_LESS);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
//	glEnable(GL_BLEND); //Alpha
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_FRONT);
//	glFrontFace(GL_CCW); //Counter clockwise polys only
//
//	glEnable(GL_TEXTURE_2D);
//
//	glPolygonMode(GL_FRONT_AND_BACK, true ? GL_FILL : GL_LINE);
//	checkGL();
//}
//
//void initGLSL() {
//	checkGL();
//}
//
//static void error_callback(int code, const char* description) {
//	VLOG_ERROR(libv::log(), "GLFW %d: %s", code, description);
//}
//
//// -------------------------------------------------------------------------------------------------
//
//static void thread_init(MyFrame*) { }
//
//static void thread_render(MyFrame* data) {
//	auto& gl = data->gl;
//
//	gl.pushMatrixProjection();
//	gl.pushMatrixView();
//	gl.pushMatrixModel();
//
//	gl.matrixProjection() = glm::perspective(1.f, 900.f / 600.f, 1.f, 1000.f);
//	gl.matrixView() = glm::lookAt(glm::vec3(20.f, 15.f, 20.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
//	gl.matrixView() *= glm::rotate(angle / 90, glm::vec3(0, 1, 0));
//
//	// ---------------------------------------------------------------------------------------------
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glMultMatrixf(glm::value_ptr(gl.matrixMVP()));
//
//	//	texture0->bind(libv::gl::TextureType::diffuse);
//	//	libv::gl::renderCube(-9, 0, 0, 4.0f);
//	//	texture0->unbind(libv::gl::TextureType::diffuse);
//	//	texture1->bind(libv::gl::TextureType::diffuse);
//	//	libv::gl::renderCube(0, 0, 0, 5.0f);
//	//	texture1->unbind(libv::gl::TextureType::diffuse);
//	//	texture2->bind(libv::gl::TextureType::diffuse);
//	//	libv::gl::renderCube(11, 0, 0, 6.0f);
//	//	texture2->unbind(libv::gl::TextureType::diffuse);
//
//	// ---------------------------------------------------------------------------------------------
//
//	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, -16));
//	//	model1->render(gl);
//	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
//	//	model2->render(gl);
//	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
//	//	model3->render(gl);
//	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
//	//	model4->render(gl);
//	//
//	//	libv::glsl::textureDiffuseSampler = static_cast<uint32_t> (libv::gl::TextureType::diffuse);
//	//	libv::glsl::textureNormalSampler = static_cast<uint32_t> (libv::gl::TextureType::normal);
//	//	libv::glsl::textureAmbientSampler = static_cast<uint32_t> (libv::gl::TextureType::ambient);
//	//
//	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
//	//	gl.matrixModel() *= glm::scale(glm::vec3(3, 3, 3));
//	//	texture1->bind(libv::gl::TextureType::diffuse); //d
//	//	texture2->bind(libv::gl::TextureType::normal); //n
//	//	texture3->bind(libv::gl::TextureType::ambient); //a
//	//	model5->render(gl);
//	//	texture3->unbind(libv::gl::TextureType::ambient); //a
//	//	texture2->unbind(libv::gl::TextureType::normal); //n
//	//	texture1->unbind(libv::gl::TextureType::diffuse); //d
//
//	gl.popMatrixModel();
//	gl.popMatrixView();
//	gl.popMatrixProjection();
//
//	checkGL();
//}
//
//static void thread_main(MyFrame *data) {
//	glfwMakeContextCurrent(data->window);
//	glfwSwapInterval(1);
//
//	initGLEW();
//	initGL();
//	initGLSL();
//
//	thread_init(data);
//
//	libv::Timer timer;
//	size_t time = 0, i = 0;
//	while (running) {
//		const float v = (float) fabs(sin(glfwGetTime() * 2.f));
//		glClearColor(data->r * v, data->g * v, data->b * v, 0.f);
//		glClear(GL_COLOR_BUFFER_BIT);
//		checkGL();
//
//		thread_render(data);
//
//		glfwSwapBuffers(data->window);
//
//		i++;
//		time += timer.time().count();
//		if (time > 1'000'000'000) {
//			VLOG_INFO(libv::log(), "FPS in window [%s]: %d", data->title, i);
//			i = 0;
//			time -= 1'000'000'000;
//		}
//	}
//
//	checkGL();
//	glfwMakeContextCurrent(nullptr);
//}
//
//// -------------------------------------------------------------------------------------------------
//
//void init() {
//	shaderDebug = std::make_shared<libv::gl::ShaderProgram>("Simple Debug",
//			std::make_shared<libv::gl::Shader>("Data/Shader/debug0.vs"),
//			std::make_shared<libv::gl::Shader>("Data/Shader/debug0.fs"));
//	shaderDepth = std::make_shared<libv::gl::ShaderProgram>("Simple Depth",
//			std::make_shared<libv::gl::Shader>("Data/Shader/depth.vs"),
//			std::make_shared<libv::gl::Shader>("Data/Shader/depth.fs"));
//	texture0 = std::make_shared<libv::gl::Texture>(
//			"Data/Texture/asteorid_02_diffuse.dds");
//	texture1 = std::make_shared<libv::gl::Texture>(
//			"Data/Texture/asteorid_02_normal.dds");
//	texture2 = std::make_shared<libv::gl::Texture>(
//			"Data/Texture/asteorid_02_ambient.dds");
//	//	model1 = new libv::gl::Model(*modelManager, "test_group.dae.pb");
//	//	model2 = new libv::gl::Model(*modelManager, "fighter_01_eltanin.dae.pb");
//	//	model3 = new libv::gl::Model(*modelManager, "test_sp.dae.pb");
//	//	model4 = new libv::gl::Model(*modelManager, "projectile_missile_01_hellfire.0001.dae.pb");
//	//	model5 = new libv::gl::Model(*modelManager, "asteroid_02.dae.pb");
//}
//
//void term() {
//	shaderDebug.reset();
//	shaderDepth.reset();
//	texture0.reset();
//	texture1.reset();
//	texture2.reset();
//}
//
//int main(void) {
//	libv::log().output(std::cout);
//
//	MyFrame frames[] = {
//		{nullptr, "Example 0", 1.0f, 0.8f, 0.8f, nullptr},
//		{nullptr, "Example 1", 0.8f, 1.0f, 0.8f, nullptr},
//		{nullptr, "Example 2", 0.8f, 0.8f, 1.0f, nullptr}
//	};
//	const int count = 3;
//
//	glfwSetErrorCallback(error_callback);
//
//	if (!glfwInit()) {
//		VLOG_ERROR(libv::log(), "Failed to initialize GLFW.");
//		exit(EXIT_FAILURE);
//	}
//
//	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
//
//	auto globalGLContext = glfwCreateWindow(50, 50, "", nullptr, nullptr);
//
//	init();
//
//	for (int i = 0; i < count; i++) {
//		frames[i].window = glfwCreateWindow(200, 200, frames[i].title, nullptr, globalGLContext);
//		if (!frames[i].window) {
//			glfwTerminate();
//			VLOG_ERROR(libv::log(), "Failed to create GLFW window.");
//			exit(EXIT_FAILURE);
//		}
//
//		glfwSetWindowPos(frames[i].window, 200 + 250 * i, 200);
//		glfwShowWindow(frames[i].window);
//
//		std::thread* th = new std::thread(thread_main, &frames[i]);
//		frames[i].th = th;
//	}
//
//	while (running) {
//		glfwWaitEvents();
//
//		for (int i = 0; i < count; i++) {
//			if (glfwWindowShouldClose(frames[i].window))
//				running = GL_FALSE;
//		}
//	}
//
//	for (int i = 0; i < count; i++)
//		frames[i].th->join();
//
//	term();
//	glfwTerminate();
//
//	exit(EXIT_SUCCESS);
//}
//
////	std::this_thread::sleep_for(std::chrono::seconds(2));
