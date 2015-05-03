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
#include "vl/gl/model.hpp"
#include "vl/gl/shader.hpp"
#include "vl/gl/uniform.hpp"
#include "vl/gl/vgl.hpp"
#include "vl/gl/texture.hpp"

//<<< http://hmijailblog.blogspot.hu/2013/09/type-punning-aliasing-unions-strict.html

using namespace vl;

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

vl::Context ioctx("IO");
vl::Context glctx("GL");
vl::gl::ModelContext context(glctx, ioctx);
vl::gl::TextureContext txctx(glctx, ioctx);

vl::gl::Shader* shaderSimple;
gl::ModelManager* modelManager;
gl::TextureManager* textureManager;
gl::Texture* texture1;
gl::Texture* texture2;
gl::Texture* texture3;
gl::Model* model1;
gl::Model* model2;
gl::Model* model3;
gl::Model* model4;
gl::Model* model5;

void loop() {
	checkGL();
	static double prevTime = 0;
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	angle += deltaTime * 30;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl::pushPMat();
	gl::pushVMat();
	gl::pushMMat();

	gl::PMat() = glm::perspective(1.f, 900.f / 600.f, 1.f, 1000.f);
	gl::VMat() = glm::lookAt(glm::vec3(20.f, 15.f, 20.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
	gl::VMat() *= glm::rotate(angle / 90, glm::vec3(0, 1, 0));

	gl::MMat() *= glm::translate(glm::vec3(0, 0, -16));
	model1->render();
	gl::MMat() *= glm::translate(glm::vec3(0, 0, 8));
	model2->render();
	gl::MMat() *= glm::translate(glm::vec3(0, 0, 8));
	model3->render();
	gl::MMat() *= glm::translate(glm::vec3(0, 0, 8));
	model4->render();
	
	glsl::textureDiffuseSampler = static_cast<uint32_t>(gl::TextureType::diffuse);
	glsl::textureNormalSampler = static_cast<uint32_t>(gl::TextureType::normal);
	glsl::textureAmbientSampler = static_cast<uint32_t>(gl::TextureType::ambient);
	
	gl::MMat() *= glm::translate(glm::vec3(0, 0, 8));
	gl::MMat() *= glm::scale(glm::vec3(3, 3, 3));
	texture1->bind(gl::TextureType::diffuse); //d
	texture2->bind(gl::TextureType::normal); //n
	texture3->bind(gl::TextureType::ambient); //a
	model5->render();
	texture3->unbind(gl::TextureType::ambient); //a
	texture2->unbind(gl::TextureType::normal); //n
	texture1->unbind(gl::TextureType::diffuse); //d

	gl::popMMat();
	gl::popVMat();
	gl::popPMat();

	glfwSwapBuffers(window);
	glfwPollEvents();
	checkGL();

	if (!glfwWindowShouldClose(window)) {
		context.gl.executeAsync(loop);
	} else {
		glfwTerminate();
		context.gl.terminate();
	}
}

void main2() {
	initGLFW();
	initGLEW();
	initGL();
	initGLSL();

	shaderSimple = new gl::Shader("Debug", "Data/Shader/debug0.vs", "Data/Shader/debug0.fs");
//	shaderSimple = new gl::Shader("Simple", "Data/Shader/simple.vs", "Data/Shader/simple.fs");
	shaderSimple->loadProgram();
	shaderSimple->printActiveUniforms();
	shaderSimple->useProgram();

	checkGL();
}

int main(int, char**) {
	vl::log().output(std::cout);

	context.gl.executeSync(main2);

	textureManager = new gl::TextureManager(&txctx);
	modelManager = new gl::ModelManager(&context);
//	texture1 = new gl::Texture(*textureManager, "Data/Texture/6poly_metal_01_diffuse.dds");
	texture1 = new gl::Texture(*textureManager, "Data/Texture/asteorid_02_diffuse.dds");
	texture2 = new gl::Texture(*textureManager, "Data/Texture/asteorid_02_normal.dds");
	texture3 = new gl::Texture(*textureManager, "Data/Texture/asteorid_02_ambient.dds");
	model1 = new gl::Model(*modelManager, "test_group.dae.pb");
	model2 = new gl::Model(*modelManager, "fighter_01_eltanin.dae.pb");
	model3 = new gl::Model(*modelManager, "test_sp.dae.pb");
	model4 = new gl::Model(*modelManager, "projectile_missile_01_hellfire.0001.dae.pb");
	model5 = new gl::Model(*modelManager, "asteroid_02.dae.pb");

	context.gl.executeAsync(loop);

	context.gl.join();
	context.io.terminate();
	context.io.join();

	return 0;
}