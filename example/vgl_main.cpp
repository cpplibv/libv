// File: Main.cpp, Created on 2014.04.25. at 21:23, Author: Vader

// ext
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
// vl
#include <vl/context.hpp>
// std
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// pro
#include "vl/gl/log.hpp"
#include "vl/gl/model.hpp"
#include "vl/gl/shader.hpp"
#include "vl/gl/texture.hpp"
#include "vl/gl/uniform.hpp"
#include "vl/gl/vgl.hpp"

//<<< http://hmijailblog.blogspot.hu/2013/09/type-punning-aliasing-unions-strict.html

GLFWwindow* window;
float angle = 0;

void errcb(int code, const char* ptr) {
	VLOG_ERROR(vl::log(), "GLFW %d: %s", code, ptr);
}

void initGLFW() {
	glfwSetErrorCallback(errcb);
	if (!glfwInit())
		return VLOG_ERROR(vl::log(), "Failed to initialize GLFW.");

	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(900, 600, "VGL Demo", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return VLOG_ERROR(vl::log(), "Failed to create GLFW window.");
	}
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	//	glfwSwapInterval(0);
	glfwSwapInterval(1);
	checkGL();
}

#define checkGLEWSupport(ext) VLOG_INFO(vl::log(), "GLEW: %-40s %s", #ext, glewIsSupported(#ext) ? "[ SUPPORTED ]" : "[UNSUPPORTED]")

void initGLEW() {
	//	glewExperimental = true;
	if (GLenum err = glewInit() != GLEW_OK)
		VLOG_ERROR(vl::log(), "Failed to initialize glew: %s", (const char*) glewGetErrorString(err));

	VLOG_INFO(vl::log(), "GL Vendor: %s", (const char*) glGetString(GL_VENDOR));
	VLOG_INFO(vl::log(), "GL Renderer: %s", (const char*) glGetString(GL_RENDERER));
	VLOG_INFO(vl::log(), "GL Version: %s", (const char*) glGetString(GL_VERSION));

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
	glClearColor(0.942, 0.981, 0.957, 1.0);

	glEnable(GL_DEPTH_TEST); //Depth
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Alpha Type
	glEnable(GL_BLEND); //Alpha
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW); //Counter clockwise polys only

	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	checkGL();
}

void initGLSL() {
	checkGL();
}

/* ---------------------------------------------------------------------------- */

vl::WorkerThread* ioctx;
vl::WorkerThread* glctx;
vl::gl::ModelContext* mlctx;
vl::gl::TextureContext* txctx;

vl::gl::Shader* shaderSimple;
vl::gl::ModelManager* modelManager;
vl::gl::TextureManager* textureManager;
vl::gl::Texture* texture1;
vl::gl::Texture* texture2;
vl::gl::Texture* texture3;
vl::gl::Model* model1;
vl::gl::Model* model2;
vl::gl::Model* model3;
vl::gl::Model* model4;
vl::gl::Model* model5;
vl::gl::GL gl;

void loop() {
	checkGL();
	static double prevTime = 0;
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	angle += deltaTime * 30;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl.pushMatrixProjection();
	gl.pushMatrixView();
	gl.pushMatrixModel();

	gl.matrixProjection() = glm::perspective(1.f, 900.f / 600.f, 1.f, 1000.f);
	gl.matrixView() = glm::lookAt(glm::vec3(20.f, 15.f, 20.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	gl.matrixView() *= glm::rotate(angle / 90, glm::vec3(0, 1, 0));

	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, -16));
	model1->render(gl);
	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	model2->render(gl);
	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	model3->render(gl);
	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	model4->render(gl);

	vl::glsl::textureDiffuseSampler = static_cast<uint32_t> (vl::gl::TextureType::diffuse);
	vl::glsl::textureNormalSampler = static_cast<uint32_t> (vl::gl::TextureType::normal);
	vl::glsl::textureAmbientSampler = static_cast<uint32_t> (vl::gl::TextureType::ambient);

	gl.matrixModel() *= glm::translate(glm::vec3(0, 0, 8));
	gl.matrixModel() *= glm::scale(glm::vec3(3, 3, 3));
	texture1->bind(vl::gl::TextureType::diffuse); //d
	texture2->bind(vl::gl::TextureType::normal); //n
	texture3->bind(vl::gl::TextureType::ambient); //a
	model5->render(gl);
	texture3->unbind(vl::gl::TextureType::ambient); //a
	texture2->unbind(vl::gl::TextureType::normal); //n
	texture1->unbind(vl::gl::TextureType::diffuse); //d

	gl.popMatrixModel();
	gl.popMatrixView();
	gl.popMatrixProjection();

	glfwSwapBuffers(window);
	glfwPollEvents();
	checkGL();

	if (!glfwWindowShouldClose(window)) {
		glctx->executeAsync(loop);
	} else {
		glfwTerminate();
		glctx->terminate();
	}
}

void main2() {
	initGLFW();
	initGLEW();
	initGL();
	initGLSL();

	shaderSimple = new vl::gl::Shader("Debug", "Data/Shader/debug0.vs", "Data/Shader/debug0.fs");
	//shaderSimple = new vl::gl::Shader("Simple", "Data/Shader/simple.vs", "Data/Shader/simple.fs");
	shaderSimple->loadProgram();
	shaderSimple->printActiveUniforms();
	shaderSimple->useProgram();

	checkGL();
}

int main(int, char**) {
	vl::log().output(std::cout);

	ioctx = new vl::WorkerThread("IO");
	glctx = new vl::WorkerThread("GL");
	mlctx = new vl::gl::ModelContext(*glctx, *ioctx);
	txctx = new vl::gl::TextureContext(*glctx, *ioctx);

	glctx->executeSync(main2);

	textureManager = new vl::gl::TextureManager(txctx);
	modelManager = new vl::gl::ModelManager(mlctx);

	texture1 = new vl::gl::Texture(*textureManager, "Data/Texture/asteorid_02_diffuse.dds");
	texture2 = new vl::gl::Texture(*textureManager, "Data/Texture/asteorid_02_normal.dds");
	texture3 = new vl::gl::Texture(*textureManager, "Data/Texture/asteorid_02_ambient.dds");
	model1 = new vl::gl::Model(*modelManager, "test_group.dae.pb");
	model2 = new vl::gl::Model(*modelManager, "fighter_01_eltanin.dae.pb");
	model3 = new vl::gl::Model(*modelManager, "test_sp.dae.pb");
	model4 = new vl::gl::Model(*modelManager, "projectile_missile_01_hellfire.0001.dae.pb");
	model5 = new vl::gl::Model(*modelManager, "asteroid_02.dae.pb");

	glctx->executeAsync(loop);

	glctx->join();
	ioctx->terminate();
	ioctx->join();

	//delete shaderSimple;

	delete texture1;
	delete texture2;
	delete texture3;

	delete model1;
	delete model2;
	delete model3;
	delete model4;
	delete model5;

	delete modelManager;
	delete textureManager;

	delete ioctx;
	delete glctx;

	delete mlctx;
	delete txctx;

	return 0;
}