

Rename example folder to sandbox
Create example folder with some of it...


cutting vm3 from vgl? has some advantage and vui doesnt need it... so why keep it around if it too
much pain to integrate


Colors:
0.236f, 0.311f, 0.311f
0.500f, 0.170f, 0.090f
0.227f, 1.000f, 0.810f
Neat little page for similar and alternativ color picking: http://www.colorhexa.com/3affce

Base class for lights and cameras...


move everything from vl to libv
vl([^a-zA-Z0-9_]|$)			libv$1
VL([^a-zA-Z0-9_]|$)			LIBV$1


license		https://www.youtube.com/watch?v=cJIi-hIlCQM
be aware that i will need copyright for every contribution into vl


http://www.cmake.org/cmake/help/v3.3/command/configure_file.html

Optimalizált fordítás - cmake:
https://github.com/sakra/cotire

P1 - std::future family

Priority levels for each operation
IN - in-context operation (fast and not context sensitive)
       GL Task        | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 Initialization       |   0000   |
 D.UIShaderProgram    |   1100   | Unloading as ShaderProgram
 D.ShaderProgram      |   1150   | Unloading as ShaderProgram
 D.UIShader           |   1200   | Unloading as Shader
 D.Shader             |   1250   | Unloading as Shader
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Render-Immediate     |   2000   | Starts by a timer when render time-window runs out
 Unload Texture       |   3100   |
 Unload Model         |   3200   |
 Unload Font          |   3300   |
 Unload ShaderProgram |   3400   |
 Unload Shader        |   3500   |
 Load UIShaderProgram |   4100   | Unloading as ShaderProgram
 Load ShaderProgram   |   4150   |
 Load UIShader        |   4200   | Unloading as Shader
 Load Shader          |   4250   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |
 Termination          |   9800   |
 Render-Residual      |   9900   | Always stays in queue as last operation

       IO Task        | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 D.UIShader File      |   1100   | Unloading as Shader File
 D.Shader File        |   1200   | Unloading as Shader File
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Unload Texture       |    IN    |
 Unload Model         |    IN    |
 Unload Font          |    IN    |
 Unload Shader File   |    IN    |
 Load UIShader File   |   4100   | Unloading as Shader File
 Load Shader File     |   4150   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |

top layer std::function and forward!

Move frame from disconnected monitor

Tracing every event for right state enums / defines / handlers:
	Char
	CharMods
	CursorEnter
	CursorPos
	Drop
	FramebufferSize
	Key
	MouseButton
	Scroll
	WindowClose
	WindowFocus
	WindowIconify
	WindowPos
	WindowRefresh
	WindowSize
	Monitor


What is GLFW_USE_DWM_SWAP_INTERVAL?.... Test it off


Priority levels for each operation
IN - in-context operation (FAST and not context sensitive)
       GL Task        | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 Initialization       |   0100   |
 D.UIShaderProgram    |   1100   | Unloading as ShaderProgram
 D.ShaderProgram      |   1150   | Unloading as ShaderProgram
 D.UIShader           |   1200   | Unloading as Shader
 D.Shader             |   1250   | Unloading as Shader
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Render-Immediate     |   2000   | Starts by a timer when render time-window runs out
 Unload Texture       |   3100   |
 Unload Model         |   3200   |
 Unload Font          |   3300   |
 Unload ShaderProgram |   3400   |
 Unload Shader        |   3500   |
 Load UIShaderProgram |   4100   | Unloading as ShaderProgram
 Load ShaderProgram   |   4150   |
 Load UIShader        |   4200   | Unloading as Shader
 Load Shader          |   4250   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |
 Termination          |   9800   |
 Render-Residual      |   9900   | Always stays in queue as last operation

       IO Task        | Priority | Note
:-------------------- | --------:|:-----------------------------------------------------------------
 D.UIShader File      |   1100   | Unloading as Shader File
 D.Shader File        |   1200   | Unloading as Shader File
 D.Font               |   1300   | Unloading as Font
 D.UITexture          |   1400   | Unloading as Texture
 D.Model              |   1500   | Unloading as Model
 D.Texture            |   1600   | Unloading as Texture
 Unload Texture       |    IN    |
 Unload Model         |    IN    |
 Unload Font          |    IN    |
 Unload Shader File   |    IN    |
 Load UIShader File   |   4100   | Unloading as Shader File
 Load Shader File     |   4150   |
 Load Font            |   4300   |
 Load UITexture       |   4400   |
 Load Model           |   4500   |
 Load Texture         |   4600   |


variant
https://github.com/JasonL9000/cppcon14


two phase lookup, what, why, how?

-----
CMake resource folder
Cube / Sky Textures http://sourceforge.net/projects/spacescape/
Skeleton animation

Render Target
Shadow Pass
Multi Pass

VM4 Animated mesh

http://www.oldunreal.com/editing/s3tc/ARB_texture_compression.pdf
http://ogldev.atspace.co.uk/www/tutorial43/tutorial43.html
OpenGL Reference page: https://www.opengl.org/sdk/docs/man/


http://hmijailblog.blogspot.hu/2013/09/type-punning-aliasing-unions-strict.html

----

Adopt TCLAP http://tclap.sourceforge.net if suitable

vl::log(__VL_ERROR__, __VL_POC__, __VL_GL__, "msg", args...)
struct vl::POC = position of code

Beside Component's shared_ptr provide a way of non owning ptr which is visible at call:
	container.add(testComponent, vl::non_owning); //simular as std::adopt_lock

Timed Event / Timer Support / Timer thread
Resettable and clearable timer / timer tasks

Config Entry:
	- default
	- description
	- isset
	- name
	- type
	- value

Optimizing for prefetcher.
Optimizing for minimum number of opengl bind.
In ui static and dynamic render options for different components
Resource pack

----
Fresnel shader - Atmosphere
Cook-Torrance shader - Metal
Minnaert - More depth?
OrenNayar - More avg lambert


> RANDOM
template<class URNG>
int rnd(int n, URNG &engine) {
    using dist_t = std::uniform_int_distribution<>;
    using param_t = dist_t::param_type;

    static dist_t dist;
    param_t params{0,n-1};

    return dist(engine, params);
}

//struct UniformLight {
//	Uniform<int> type;
//	Uniform<bool> enabled;
//
//	Uniform<glm::vec3> position;
//	Uniform<glm::vec3> direction;
//	Uniform<glm::vec4> diffuse;
//	Uniform<glm::vec4> specular;
//
//	Uniform<double> range;
//	Uniform<double> intensity;
//	Uniform<double> innerCosAngle;
//	Uniform<double> outerCosAngle;
//
//	Uniform<bool> shadowCast;
//	Uniform<int> shadowMapSampler;
//	Uniform<glm::mat4> shadowMVPTmat;
//
//	UniformLight(const std::string& name);
//	void operator=(const Light &v);
//};

diffuse(1.0f, 1.0f, 1.0f, 1.0f),
specular(0.8f, 0.8f, 0.8f, 1.0f),
emission(0.0f, 0.0f, 0.0f, 1.0f),
ambient(0.1f, 0.1f, 0.1f, 1.0f),
reflective(1.0f, 1.0f, 1.0f, 1.0f),
shininess(32.0f) { }


GLuint fullScreenQuadVBO;

void cleanFullScreenQuad() {
	glDeleteBuffers(1, &fullScreenQuadVBO);
}

void initFullScreenQuad() {
	float vertexData[]{
		0.0f, 0.0f, -1.0f, -1.0f,
		1.0f, 0.0f, 1.0f, -1.0f,
		0.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	glGenBuffers(1, &fullScreenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fullScreenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof (float)) * 16, vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vglPassFullScreenQuad() {
	glBindBuffer(GL_ARRAY_BUFFER, fullScreenQuadVBO);
	glEnableVertexAttribArray(ATTRIBUTE_POSITION);
	glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD0);

	glVertexAttribPointer(ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof (float) * 4, ((void*) (sizeof (float) * 2)));
	glVertexAttribPointer(ATTRIBUTE_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof (float) * 4, ((void*) (sizeof (float) * 0)));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(ATTRIBUTE_TEXCOORD0);
	glDisableVertexAttribArray(ATTRIBUTE_POSITION);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vglViewportFullScreen() {
	glViewport(0, 0, options.graphics.resolution.get().x
			, options.graphics.resolution.get().y);
}

