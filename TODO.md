--- STACK ------------------------------------------------------------------------------------------

string2D size respect (new line)
-- commit --

elevate line in string based on descent
-- commit --

cleanup / review the full render process
-- commit --

use VAO for string2D
-- commit --

sandbox multiple label under each other
-- commit --

font cache
-- commit --

shader cache
-- commit --

remove vm3 from gl
-- commit --

FIX: Size management - find reason of div by zero? (left-right=0 div)
FIX: 3 5 [libv.ui.glfw] 65537 - The GLFW library is not initialized // This is a core issue
-- commit --

?kill def renderer
-- commit --

--- AWAITING ---------------------------------------------------------------------------------------

pretty flow, refactor line, build line, accum line...
LIBV_ASSERT, LIBV_DEBUG_ASSERT, LIBV_STATIC_ASSERT in utility header
glEnable(GL_DEBUG_OUTPUT);
take a look at frame and component events
replace every raw ptr with a smart counter part (incl observer_ptr)
look after and variant https://isocpp.org/blog/2016/01/cpp-language-support-for-pattern-matching-and-variants
look after any
moving vec costume getter functions from member to public -> reducing symbols...
provide exception free api EVERYWHERE!
seg fault in resource! (just run libv_test in debug...) // however it will be rewritten

--- ABANDONED --------------------------------------------------------------------------------------

LIBV_STRONG_TYPEDEF(int, Severity);
log thread naming

// -------------------------------------------------------------------------------------------------

Idea for rendering lasers:

Most of the work will be done by the shader.
2 quad rendered common edge is the center of the laser.
Both quad has the same texture space, but one of them is flipped.
By texture coord shader can calculate the distance from center.
Texture is seamless cloud-like mostly longish pattern with.
Animation is based on uniform offset for texture.
There can be multiple texture with different animation offset direction.
Low alpha values on edges. Opacity can be transformed from linear to some exponential.
Multiple quad-pair could be used along the same laser.
Each low angle eye-normals quad has decreased opacity or even better if per-fragment calculated.

// -------------------------------------------------------------------------------------------------

Idea for world gen:

"Weighted entries useful in Arrays that can be randomly picked from."
http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/
http://devmag.org.za/2009/04/25/perlin-noise/
http://gamedev.stackexchange.com/questions/31241/random-map-generation

// -------------------------------------------------------------------------------------------------

Fully separating the ui from the frame handler is completely possible and desirable.
It is also possible do it without any dependency between the two with an additional "UIFrame lib".

cutting vm3 from vgl? has some advantage and vui doesnt need it... so why keep it around if it too
much pain to integrate

// -------------------------------------------------------------------------------------------------

separate pattern and color would make a huge difference in a lot of cases

Colors:
0.236f, 0.311f, 0.311f
0.500f, 0.170f, 0.090f
0.227f, 1.000f, 0.810f
Neat little page for similar and alternative color picking: http://www.colorhexa.com/3affce

Base class for lights and cameras...


license		https://www.youtube.com/watch?v=cJIi-hIlCQM
be aware that i will need copyright for every contribution into libv


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
Resource pack

----
Fresnel shader - Atmosphere
Cook-Torrance shader - Metal
Minnaert - More depth?
OrenNayar - More avg lambert


--- PASTEBIN ---------------------------------------------------------------------------------------

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


//glm::mat4 Light::getPmat() {
//	if (type == spotLight)
//		return perspective<float>(acos(outerCosAngle) * 180.0f / PI * 2, 1.0f, range / 15.0f, range); //2szeres outer sz�g, mivel nek�nk nem a 'fele' kell hanem a teljes 'sug�r'
//	else if (type == dirLight)
//		return ortho<float>(-90, 90, -90, 90, -90, 90);
//	else //if (type == pointLight)
//		return ortho<float>(-30, 30, -30, 30, -10, 150);
//}
//
//glm::mat4 Light::getVmat() {
//	if (type == spotLight)
//		return lookAt(position, position + direction, glm::glm::vec3(0, 1, 0));
//	else if (type == dirLight)
//		return lookAt(glm::vec3(0, 0, 0), direction, glm::glm::vec3(0, 1, 0));
//	else //if (type == pointLight)
//		return ortho<float>(-30, 30, -30, 30, -10, 150);
//}


std::this_thread::sleep_for(std::chrono::seconds(2));
