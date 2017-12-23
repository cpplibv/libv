// File: program.hpp - Created on 2017.12.20. 11:27 - Author: Vader

#pragma once

// libv
#include <libv/utility/observer_ptr.hpp>
#include <libv/gl/program_object.hpp>
// std
#include <memory>
#include <string>
#include <variant>
#include <vector>
// pro
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_stream.hpp>


namespace libv {
namespace gl {

class GL;

} // namespace gl
namespace glr {

// -------------------------------------------------------------------------------------------------

class Remote;
class AttorneyMeshRemote;

struct RemoteProgram {
	libv::gl::Program program;

	bool created = false;
	bool dirty = false;

	//uint32_t ref_count = 0;

	bool dirty_binding = false;
	bool dirty_source = false;
	bool dirty_location = false;

	UniformStream uniformStream;
	std::vector<int32_t> uniform_locations;

public:
	std::string vertex;
	std::string geometry;
	std::string fragment;
	std::string compute;
	std::string tess_control;
	std::string tess_evaluation;
	std::vector<UniformBlockBinding> pending_bindings;
	std::vector<std::string> uniform_identifiers;

private:
	/// Can be nullptr if the object is not yet associated with a remote OpenGL context
	libv::observer_ptr<Remote> remote = nullptr;

private:
	void create(libv::gl::GL& gl, Remote& remote_) noexcept;
	void update(libv::gl::GL& gl, Remote& remote_) noexcept;

public:
	void use(libv::gl::GL& gl, Remote& remote_) noexcept;
	~RemoteProgram() noexcept;
};

// -------------------------------------------------------------------------------------------------

struct Program {
	friend class AttorneyProgramRemote;

private:
	std::shared_ptr<RemoteProgram> remote;

public:
	Program() noexcept;

public:
	void vertex(std::string source) noexcept;
	void geometry(std::string source) noexcept;
	void fragment(std::string source) noexcept;
	void compute(std::string source) noexcept;
	void tess_control(std::string source) noexcept;
	void tess_evaluation(std::string source) noexcept;

public:
	void block_binding(UniformBlockBinding binding) noexcept;

public:
	template <typename T>
	inline void uniform(const Uniform_t<T> uniform, const T& value) noexcept {
		remote->uniformStream.set(uniform.location, value);
	}

public:
	void assign(Uniform& uniform_, std::string identifier) noexcept;

	template <typename T>
	inline void assign(Uniform_t<T>& uniform_, std::string identifier, const T& value) noexcept {
		assign(uniform_, identifier);
		uniform(uniform_, value);
	}
};

struct AttorneyProgramRemote {
	friend class Queue;
	inline static RemoteProgram& remote(const Program& program) noexcept {
		return *program.remote;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv


//struct ProgramRemote {
//	libv::gl::Program program;
//
////	libv::gl::Uniform_vec3f uniformCameraPositionW;
////	libv::gl::Uniform_mat4f uniformMatrixMVP;
////	libv::gl::Uniform_mat4f uniformMatrixModel;
////	libv::gl::Uniform_mat4f uniformMatrixModelView;
////	libv::gl::Uniform_mat4f uniformMatrixProjection;
////	libv::gl::Uniform_mat4f uniformMatrixView;
////	libv::gl::Uniform_vec2f uniformMousePositionH; // Not sure about this one
////	libv::gl::Uniform_vec2f uniformMousePositionS; // Not sure about this one
////	libv::gl::Uniform_vec2f uniformResolution;
////	libv::gl::Uniform_float uniformTimeReal;
////	libv::gl::Uniform_float uniformTimeSimulation;
//
//	ProgramRemote(const Shader& vs, const Shader& fs) {
//		program.createLink(vs, fs);
////		assignBaseUniforms();
//	}
//	ProgramRemote(const Shader& vs, const Shader& gs, const Shader& fs) {
//		program.createLink(vs, gs, fs);
////		assignBaseUniforms();
//	}
//	~ProgramRemote() {
//		program.destroy();
//	}
//
////	void assignBaseUniforms() {
////		program.assign(uniformCameraPositionW, "cameraPositionW");
////		program.assign(uniformMatrixMVP, "matrixMVP");
////		program.assign(uniformMatrixModel, "matrixModel");
////		program.assign(uniformMatrixModelView, "matrixModelView");
////		program.assign(uniformMatrixProjection, "matrixProjection");
////		program.assign(uniformMatrixView, "matrixView");
////		program.assign(uniformMousePositionH, "mousePositionH");
////		program.assign(uniformMousePositionS, "mousePositionS");
////		program.assign(uniformResolution, "resolution");
////		program.assign(uniformTimeReal, "timeReal");
////		program.assign(uniformTimeSimulation, "timeSimulation");
////	}
//
//	void use(Queue& queue) {
//		queue.program(program);
//
////		if (uniformMatrixMVP.isActive())
////			queue.uniform(uniformMatrixMVP, mvp());
////		if (uniformMatrixModel.isActive())
////			queue.uniform(uniformMatrixModel, model);
////		if (uniformMatrixModelView.isActive())
////			queue.uniform(uniformMatrixModelView, view * model);
////		if (uniformMatrixProjection.isActive())
////			queue.uniform(uniformMatrixProjection, projection);
////		if (uniformMatrixView.isActive())
////			queue.uniform(uniformMatrixView, view);
////
////		if (uniformCameraPositionW.isActive())
////			queue.uniform(uniformCameraPositionW, eye());
////
////		if (uniformTimeReal.isActive())
////			queue.uniform(uniformTimeReal, timeFrame);
////		if (uniformTimeSimulation.isActive())
////			queue.uniform(uniformTimeSimulation, timeSimulation);
////
////		if (uniformMousePositionH.isActive())
////			queue.uniform(uniformMousePositionH, mousePosition / resolution);
////		if (uniformMousePositionS.isActive())
////			queue.uniform(uniformMousePositionS, mousePosition);
////		if (uniformResolution.isActive())
////			queue.uniform(uniformResolution, resolution);
//	}
//};
