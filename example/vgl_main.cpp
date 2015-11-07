// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
// vl
#include <vl/worker_thread.hpp>
// std
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// pro
#include <vl/gl/gl.hpp>
#include <vl/gl/log.hpp>
#include <vl/gl/model.hpp>
#include <vl/gl/services.hpp>
#include <vl/gl/shader.hpp>
#include <vl/gl/texture.hpp>
#include <vl/gl/uniform.hpp>

GLFWwindow* window;
float angle = 0;

void errcb(int code, const char* ptr) {
	VLOG_ERROR(vl::log(), "GLFW %d: %s", code, ptr);
}

void initGLFW() {
	glfwSetErrorCallback(errcb);
	if (!glfwInit())
		return VLOG_ERROR(vl::log(), "Failed to initialize GLFW.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	window = glfwCreateWindow(900, 600, "VGL Demo", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return VLOG_ERROR(vl::log(), "Failed to create GLFW window.");
	}
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	//		glfwSwapInterval(0);
	glfwSwapInterval(1);
	checkGL();
}

#define checkGLEWSupport(ext) VLOG_INFO(vl::log(), "GLEW: %-40s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")

void initGLEW() {
	//	glewExperimental = true;
	if (GLenum err = glewInit() != GLEW_OK)
		VLOG_ERROR(vl::log(), "Failed to initialize glew: %s", (const char*) glewGetErrorString(err));
	checkGL();

	VLOG_INFO(vl::log(), "GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
	VLOG_INFO(vl::log(), "GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
	VLOG_INFO(vl::log(), "GL Version: %s", (const char*) glGetString(GL_VERSION));
	checkGL();

	checkGLEWSupport(GL_VERSION_3_0);
	checkGLEWSupport(GL_VERSION_3_1);
	checkGLEWSupport(GL_VERSION_3_2);
	checkGLEWSupport(GL_VERSION_3_3);
	checkGLEWSupport(GL_VERSION_4_0);
	checkGLEWSupport(GL_VERSION_4_1);
	checkGLEWSupport(GL_VERSION_4_2);
	checkGLEWSupport(GL_VERSION_4_3);
	checkGLEWSupport(GL_VERSION_4_4);
	checkGLEWSupport(GL_VERSION_4_5);
	checkGLEWSupport(GL_ARB_draw_elements_base_vertex);
	checkGLEWSupport(GL_ARB_gpu_shader_fp64);

	checkGL();
}

void initGL() {
	glClearColor(0.942f, 0.981f, 0.957f, 1.0f);

	glEnable(GL_DEPTH_TEST); //Depth
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
	glEnable(GL_BLEND); //Alpha
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW); //Counter clockwise polys only

	glEnable(GL_TEXTURE_2D);

	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	checkGL();
}

void initGLSL() {
	checkGL();
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<vl::WorkerThread> threadIO;
std::unique_ptr<vl::WorkerThread> threadGL;

std::shared_ptr<vl::gl::ShaderProgramProxy> shaderDebug;
std::shared_ptr<vl::gl::ShaderProgramProxy> shaderDepth;
std::shared_ptr<vl::gl::Texture> texture1;
std::shared_ptr<vl::gl::Texture> texture2;
std::shared_ptr<vl::gl::Texture> texture3;
//std::shared_ptr<vl::gl::Model> model1;
//std::shared_ptr<vl::gl::Model> model2;
//std::shared_ptr<vl::gl::Model> model3;
//std::shared_ptr<vl::gl::Model> model4;
//std::shared_ptr<vl::gl::Model> model5;
vl::gl::GL gl;

// -------------------------------------------------------------------------------------------------

void term_gl();

// =================================================================================================

void loop_gl() {
	checkGL();
	static double prevTime = 0;
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	angle += static_cast<float> (deltaTime * 30);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl.pushMatrixProjection();
	gl.pushMatrixView();
	gl.pushMatrixModel();

	gl.matrixProjection() = glm::perspective(1.f, 900.f / 600.f, 1.f, 1000.f);
	gl.matrixView() = glm::lookAt(glm::vec3(20.f, 15.f, 20.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	gl.matrixView() *= glm::rotate(angle / 90, glm::vec3(0, 1, 0));

	// ---------------------------------------------------------------------------------------------
	// Debug block
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(glm::value_ptr(gl.matrixMVP()));

//	texture1->bind(vl::gl::TextureType::diffuse);
//	vl::gl::renderCube(-9, 0, 0, 4.0f);
//	texture1->unbind(vl::gl::TextureType::diffuse);
//	texture2->bind(vl::gl::TextureType::diffuse);
//	vl::gl::renderCube(0, 0, 0, 5.0f);
//	texture2->unbind(vl::gl::TextureType::diffuse);
//	texture3->bind(vl::gl::TextureType::diffuse);
//	vl::gl::renderCube(11, 0, 0, 6.0f);
//	texture3->unbind(vl::gl::TextureType::diffuse);
	// ---------------------------------------------------------------------------------------------

	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, -16));
	//	model1->render(gl);
	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	//	model2->render(gl);
	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	//	model3->render(gl);
	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	//	model4->render(gl);
	//
	//	vl::glsl::textureDiffuseSampler = static_cast<uint32_t> (vl::gl::TextureType::diffuse);
	//	vl::glsl::textureNormalSampler = static_cast<uint32_t> (vl::gl::TextureType::normal);
	//	vl::glsl::textureAmbientSampler = static_cast<uint32_t> (vl::gl::TextureType::ambient);
	//
	//	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	//	gl.matrixModel() *= glm::scale(glm::vec3(3, 3, 3));
	//	texture1->bind(vl::gl::TextureType::diffuse); //d
	//	texture2->bind(vl::gl::TextureType::normal); //n
	//	texture3->bind(vl::gl::TextureType::ambient); //a
	//	model5->render(gl);
	//	texture3->unbind(vl::gl::TextureType::ambient); //a
	//	texture2->unbind(vl::gl::TextureType::normal); //n
	//	texture1->unbind(vl::gl::TextureType::diffuse); //d

	gl.popMatrixModel();
	gl.popMatrixView();
	gl.popMatrixProjection();

	glfwSwapBuffers(window);
	glfwPollEvents();
	checkGL();

	if (!glfwWindowShouldClose(window))
		threadGL->executeAsync(loop_gl);
	else
		term_gl();
}

void main_gl() {
	initGLFW();
	initGLEW();
	initGL();
	initGLSL();

	//	shaderSimple->printActiveUniforms();
	//	shaderSimple->useProgram();

	checkGL();
}

void term_gl() {
	checkGL();
	shaderDebug.reset();
	shaderDepth.reset();

	texture1.reset();
	texture2.reset();
	texture3.reset();

	//	model1.reset();
	//	model2.reset();
	//	model3.reset();
	//	model4.reset();
	//	model5.reset();

	checkGL();

	threadGL->executeAsync([] {
		checkGL();
		glfwTerminate();
		threadGL->terminate(); // Allowing GL thread to join into main
	}, 9800);
}

int main(int, char**) {
	vl::log().output(std::cout);

	threadIO.reset(new vl::WorkerThread("IO"));
	threadGL.reset(new vl::WorkerThread("GL"));

	vl::gl::ServiceTexture st(threadIO.get(), threadGL.get());
	vl::gl::ServiceShader ss(threadIO.get(), threadGL.get());

	threadGL->executeSync(main_gl);

	shaderDebug = std::make_shared<vl::gl::ShaderProgramProxy>(&ss, "Simple Debug", "Data/Shader/debug0.fs", "Data/Shader/debug0.vs");
	shaderDepth = std::make_shared<vl::gl::ShaderProgramProxy>(&ss, "Simple Depth", "Data/Shader/depth.fs", "Data/Shader/depth.vs");
	texture1 = std::make_shared<vl::gl::Texture>(&st, "Data/Texture/asteorid_02_diffuse.dds");
	texture2 = std::make_shared<vl::gl::Texture>(&st, "Data/Texture/asteorid_02_normal.dds");
	texture3 = std::make_shared<vl::gl::Texture>(&st, "Data/Texture/asteorid_02_ambient.dds");
	//	model1 = new vl::gl::Model(*modelManager, "test_group.dae.pb");
	//	model2 = new vl::gl::Model(*modelManager, "fighter_01_eltanin.dae.pb");
	//	model3 = new vl::gl::Model(*modelManager, "test_sp.dae.pb");
	//	model4 = new vl::gl::Model(*modelManager, "projectile_missile_01_hellfire.0001.dae.pb");
	//	model5 = new vl::gl::Model(*modelManager, "asteroid_02.dae.pb");

	threadGL->executeAsync(loop_gl);

	threadGL->join();
	threadIO->terminate();
	threadIO->join();

	threadGL.reset();
	threadIO.reset();

	return 0;
}