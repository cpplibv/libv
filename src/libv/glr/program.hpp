// Project: libv.glr, File: src/libv/glr/program.hpp

#pragma once

// libv
#include <libv/gl/gl_fwd.hpp>
#include <libv/gl/program_object.hpp>
#include <libv/meta/identity.hpp>
#include <libv/utility/memory/observer_ptr.hpp>
// std
#include <memory>
#include <string>
#include <vector>
// pro
#include <libv/glr/uniform.hpp>
#include <libv/glr/uniform_block_binding.hpp>
#include <libv/glr/uniform_stream.hpp>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

class AttorneyMeshRemote;
class DestroyQueues;
class Remote;

struct RemoteProgram {
	libv::gl::Program program;

	bool created = false;
	bool dirty = false;

	//uint32_t ref_count = 0;

	bool dirty_binding = false;
	bool dirty_source = false;
	bool dirty_location = false;

	UniformStream uniformStream;

	std::vector<std::string> uniform_identifiers;
	std::vector<int32_t> uniform_locations;

	std::vector<UniformBlockBinding> block_bindings;
	std::size_t bound_block_count = 0;

public:
	std::string vertex;
	std::string geometry;
	std::string fragment;
	std::string compute;
	std::string tess_control;
	std::string tess_evaluation;

private:
	libv::observer_ptr<DestroyQueues> remote = nullptr;

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
	explicit Program(std::shared_ptr<RemoteProgram> remote) noexcept;

	Program base_ref() {
		return Program(remote);
	}

public:
	[[nodiscard]] inline bool operator==(const Program& other) const noexcept = default;

private:
	void mark_for_full_reload();

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
	inline void uniform(const Uniform_t<T> uniform, const libv::meta::identity_t<T>& value) noexcept {
		remote->uniformStream.set(uniform.location, value);
	}

public:
	void assign(Uniform& uniform_, std::string identifier) noexcept;

	template <typename T>
	inline void assign(Uniform_t<T>& uniform_, std::string identifier, const libv::meta::identity_t<T>& value) noexcept {
		assign(uniform_, identifier);
		uniform(uniform_, value);
	}

public:
	void _native_swap(libv::gl::Program& program) noexcept;
};

struct AttorneyProgramRemote {
	friend class Queue;
	static inline const std::shared_ptr<RemoteProgram>& remote(const Program& program) noexcept {
		return program.remote;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
