// File: gl_host.hpp - Created on 2017.10.28. 07:14 - Author: Vader

#pragma once

// ext
#include <GL/glew.h>
// libv
#include <libv/math/vec.hpp>
#include <libv/gl/gl.hpp>
// std
#include <thread>
#include <vector>
// pro
#include <libv/glr/queue.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class RemoteMesh;

// -------------------------------------------------------------------------------------------------

// Alternative name could be: RemoteGL or DeferredGL
struct Remote {
private:
public: // !!! Not public
	gl::GL gl;

private:
//	std::vector<RemoteMesh> meshes;
//	std::vector<boost::intrusive_ptr<RemoteMesh>> meshes;
//	std::vector<std::function<void()>> updateTasks;

private:
	std::thread::id contextThreadID;
	bool initalized = false;

public:
	inline explicit Remote(bool initalize = false);
	inline ~Remote();

public:
	inline Queue queue();
	inline void execute(Queue&& queue);

public:
	/// Introduces a sequence point
	inline void clearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(to_value(gl::Buffer::Color));
		LIBV_GL_DEBUG_CHECK();
	}
	/// Introduces a sequence point
	inline void clearDepth() {
		glClear(to_value(gl::Buffer::Depth));
		LIBV_GL_DEBUG_CHECK();
	}

public:
	inline void create();
	inline void render();
	inline void destroy();
};

// -------------------------------------------------------------------------------------------------

inline Remote::Remote(bool initalize) {
	if (initalize)
		create();
}

inline Remote::~Remote() {
	if (initalized) {
		LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == contextThreadID);
		destroy();
	}
}

inline Queue Remote::queue() {
	LIBV_GL_DEBUG_ASSERT(initalized);

	return {};
}

inline void Remote::create() {
	// load glew
	contextThreadID = std::this_thread::get_id();
	initalized = true;
}

inline void Remote::execute(Queue&& queue) {
	LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == contextThreadID);
	LIBV_GL_DEBUG_ASSERT(initalized);

	queue.execute();
//	processDestroyQueues();
}

inline void Remote::destroy() {
	LIBV_GL_DEBUG_ASSERT(std::this_thread::get_id() == contextThreadID);
	LIBV_GL_DEBUG_ASSERT(initalized);

//	processDestroyQueues();

	initalized = false;
}

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv































namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//struct UIShader {
//	bool dirty = true;
//	std::string name;
//	std::string source;
//	libv::gl::Shader shader;
//	libv::gl::ShaderType type;
//	UIGL& owner;
//};
//
//struct UIShaderProxy {
//	UIShader& owner;
//	uint32_t index;
//	// 32bit padding could be utilized for: generation ID, proxy ID or flags
//};
//
//struct UIProgram {
//	bool dirty = true;
//	libv::gl::Program program;
//	std::string name;
//	UIShaderProxy vs;
//	UIShaderProxy fs;
//	UIShaderProxy gs;
//	UIGL& owner;
//};
//
//struct UIProgramProxy {
//	UIProgram& owner;
//	uint32_t index;
//	// 32bit padding could be utilized for: generation ID, proxy ID or flags
//};
//
//template <typename T>
//struct UIUniform {
//	libv::gl::Uniform<T> uniform;
//	const char* name;
//	UIProgram& owner;
//};
//
//template <typename T>
//struct UIUniformProxy {
//	UIUniform<T>& owner;
//	uint32_t index;
//	// 32bit padding could be utilized for: generation ID, proxy ID or flags
//};
//
//struct UITextureProxy {
//};



//template <template <typename> typename... Attributes>
//struct Vertex : Attributes<Vertex>... {
//	auto bind() {
////		vao.bindAttribute(vbo, 0, &Vertex::pos, false);
////		vao.bindAttribute(vbo, 8, &Vertex::tex, false);
//	}
//};
//
//template <typename... Attributes>
//class VertexBuilder {
//#define LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(ATTR_NAME, TYPE, FIELD_NAME) \
//	private: template <size_t N, typename CRTP> struct ATTR##ATTR_NAME { TYPE FIELD_NAME; \
//	protected: constexpr inline size_t index = N; \
//		constexpr static inline auto ptr = &CRTP::FIELD_NAME; } \
//	public: template <size_t N>	using ATTR_NAME = VertexBuilder<Attributes..., ATTR_NAME<N>>;
//
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Position, ::libv::vec3f, position);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Normal  , ::libv::vec3f, normal  );
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Color0  , ::libv::vec4f, color0  );
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Color1  , ::libv::vec4f, color1  );
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Color2  , ::libv::vec4f, color2  );
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Color3  , ::libv::vec4f, color3  );
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture0, ::libv::vec2f, texture0);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture1, ::libv::vec2f, texture1);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture2, ::libv::vec2f, texture2);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture3, ::libv::vec2f, texture3);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture4, ::libv::vec2f, texture4);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture5, ::libv::vec2f, texture5);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture6, ::libv::vec2f, texture6);
//	LIBV_UI_GENERATE_VERTEX_ATTRIBUTE(Texture7, ::libv::vec2f, texture7);
//
//#undef LIBV_UI_GENERATE_VERTEX_ATTRIBUTE

//	template <size_t N> struct Position { constexpr inline size_t index = N; libv::vec3f position; };
//	template <size_t N> struct Normal   { constexpr inline size_t index = N; libv::vec3f normal;   };
//	template <size_t N> struct Color0   { constexpr inline size_t index = N; libv::vec4f color0;   };
//	template <size_t N> struct Color1   { constexpr inline size_t index = N; libv::vec4f color1;   };
//	template <size_t N> struct Color2   { constexpr inline size_t index = N; libv::vec4f color2;   };
//	template <size_t N> struct Color3   { constexpr inline size_t index = N; libv::vec4f color3;   };
//	template <size_t N> struct Texture0 { constexpr inline size_t index = N; libv::vec2f texture0; };
//	template <size_t N> struct Texture1 { constexpr inline size_t index = N; libv::vec2f texture1; };
//	template <size_t N> struct Texture2 { constexpr inline size_t index = N; libv::vec2f texture2; };
//	template <size_t N> struct Texture3 { constexpr inline size_t index = N; libv::vec2f texture3; };
//	template <size_t N> struct Texture4 { constexpr inline size_t index = N; libv::vec2f texture4; };
//	template <size_t N> struct Texture5 { constexpr inline size_t index = N; libv::vec2f texture5; };
//	template <size_t N> struct Texture6 { constexpr inline size_t index = N; libv::vec2f texture6; };
//	template <size_t N> struct Texture7 { constexpr inline size_t index = N; libv::vec2f texture7; };

//public:
//	using type = Vertex<Attributes...>;
//	template <size_t N>	using Position = VertexBuilder<Attributes..., Position<N>>;
//	template <size_t N>	using Normal   = VertexBuilder<Attributes..., Normal<N>>;
//	template <size_t N>	using Color0   = VertexBuilder<Attributes..., Color0<N>>;
//	template <size_t N>	using Color1   = VertexBuilder<Attributes..., Color1<N>>;
//	template <size_t N>	using Color2   = VertexBuilder<Attributes..., Color2<N>>;
//	template <size_t N>	using Color3   = VertexBuilder<Attributes..., Color3<N>>;
//	template <size_t N>	using Texture0 = VertexBuilder<Attributes..., Texture0<N>>;
//	template <size_t N>	using Texture1 = VertexBuilder<Attributes..., Texture1<N>>;
//	template <size_t N>	using Texture2 = VertexBuilder<Attributes..., Texture2<N>>;
//	template <size_t N>	using Texture3 = VertexBuilder<Attributes..., Texture3<N>>;
//	template <size_t N>	using Texture4 = VertexBuilder<Attributes..., Texture4<N>>;
//	template <size_t N>	using Texture5 = VertexBuilder<Attributes..., Texture5<N>>;
//	template <size_t N>	using Texture6 = VertexBuilder<Attributes..., Texture6<N>>;
//	template <size_t N>	using Texture7 = VertexBuilder<Attributes..., Texture7<N>>;
//};


// -------------------------------------------------------------------------------------------------

//struct Batch {
//	uint16_t num_texture;
//	uint16_t num_uniform;
//	uint16_t num_vao;
////	size_t start;
////	size_t size;
//
//	// Pass
//	//	UIProgramProxy program;
//	// Uniforms
//	// std::vector<unsiforms> uniforms;
//	// VAO
//};

//struct BatchProcess {
//	std::vector<Batch> batches;
//
//	// Pass passes;
//	std::vector<UIProgramProxy> program;
//	std::vector<libv::gl::TextureChannel, UITextureProxy> textures;
//	//	std::vector<UIUniform> uniform;
//	std::vector<libv::gl::VertexArray> vao;
//};
//
//struct ExecutionQueue {
//	std::vector<BatchProcess> processes;
//
//};

} // namespace ui
} // namespace libv




// Debug -------------------------------------------------------------------------------------------

// TODO P5: Move into GLE and use OpenGL 3.0
//
//// <editor-fold defaultstate="collapsed" desc="inline void renderCube(float x, float y, float z, float size) {...">
//inline void renderCube(float x, float y, float z, float size) {
//	glBegin(GL_QUADS);
//
//	//Right
//	glNormal3f(1.0f, 0.0f, 0.0f);
//	glTexCoord2f(0, 0);
//	glVertex3f(x + size, y - size, z - size);
//	glTexCoord2f(0, 1);
//	glVertex3f(x + size, y - size, z + size);
//	glTexCoord2f(1, 1);
//	glVertex3f(x + size, y + size, z + size);
//	glTexCoord2f(1, 0);
//	glVertex3f(x + size, y + size, z - size);
//
//	//Left
//	glNormal3f(-1.0f, 0.0f, 0.0f);
//	glTexCoord2f(0, 0);
//	glVertex3f(x - size, y - size, z - size);
//	glTexCoord2f(0, 1);
//	glVertex3f(x - size, y + size, z - size);
//	glTexCoord2f(1, 1);
//	glVertex3f(x - size, y + size, z + size);
//	glTexCoord2f(1, 0);
//	glVertex3f(x - size, y - size, z + size);
//
//	//Top
//	glNormal3f(0.0f, 1.0f, 0.0f);
//	glTexCoord2f(0, 0);
//	glVertex3f(x - size, y + size, z - size);
//	glTexCoord2f(0, 1);
//	glVertex3f(x + size, y + size, z - size);
//	glTexCoord2f(1, 1);
//	glVertex3f(x + size, y + size, z + size);
//	glTexCoord2f(1, 0);
//	glVertex3f(x - size, y + size, z + size);
//
//	//Bottom
//	glNormal3f(0.0f, -1.0f, 0.0f);
//	glTexCoord2f(0, 0);
//	glVertex3f(x - size, y - size, z - size);
//	glTexCoord2f(0, 1);
//	glVertex3f(x - size, y - size, z + size);
//	glTexCoord2f(1, 1);
//	glVertex3f(x + size, y - size, z + size);
//	glTexCoord2f(1, 0);
//	glVertex3f(x + size, y - size, z - size);
//
//	//Near
//	glNormal3f(0.0f, 0.0f, -1.0f);
//	glTexCoord2f(0, 0);
//	glVertex3f(x - size, y - size, z - size);
//	glTexCoord2f(0, 1);
//	glVertex3f(x + size, y - size, z - size);
//	glTexCoord2f(1, 1);
//	glVertex3f(x + size, y + size, z - size);
//	glTexCoord2f(1, 0);
//	glVertex3f(x - size, y + size, z - size);
//
//	//Far
//	glNormal3f(0.0f, 0.0f, 1.0f);
//	glTexCoord2f(0, 0);
//	glVertex3f(x - size, y - size, z + size);
//	glTexCoord2f(0, 1);
//	glVertex3f(x - size, y + size, z + size);
//	glTexCoord2f(1, 1);
//	glVertex3f(x + size, y + size, z + size);
//	glTexCoord2f(1, 0);
//	glVertex3f(x + size, y - size, z + size);
//
//	glEnd();
//}
//// </editor-fold>
//
//// <editor-fold defaultstate="collapsed" desc="inline void renderXYZ(float x, float y, float z, float size) {...">
//inline void renderXYZ(float x, float y, float z, float size) {
//	glBegin(GL_LINES);
//	glColor3f(1, 0, 0);
//	glVertex3f(x, y, z);
//	glVertex3f(x + size, y, z);
//	glColor3f(0, 1, 0);
//	glVertex3f(x, y, z);
//	glVertex3f(x, y + size, z);
//	glColor3f(0, 0, 1);
//	glVertex3f(x, y, z);
//	glVertex3f(x, y, z + size);
//	glEnd();
//	glColor3f(1, 1, 1);
//}
//// </editor-fold>
