// Project: libv.rev, File: src/libv/rev/resource/material.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/vm4/vertex_attribute.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class BaseMaterial : public libv::ref_count_base<BaseMaterial> {
	friend libv::ref_count_access;

public:
	virtual void bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) = 0;
	virtual bool utilizes(libv::vm4::VertexAttribute) {
		// TODO P5: Ability to not load the unused attributes into the vertices
		//		switch (attr) {
		//			case libv::vm4::VertexAttribute::position:   return true;
		//			case libv::vm4::VertexAttribute::normal:     return true;
		//			case libv::vm4::VertexAttribute::tangent:    return false;
		//			case libv::vm4::VertexAttribute::bitangent:  return false;
		//			case libv::vm4::VertexAttribute::texture0:   return true;
		//			case libv::vm4::VertexAttribute::boneID:     return false;
		//			case libv::vm4::VertexAttribute::boneWeight: return false;
		//		}
		return true;
	}
	virtual ~BaseMaterial() = default;
};

// -------------------------------------------------------------------------------------------------

/// Handler type
class Material {
	libv::intrusive2_ptr<BaseMaterial> self;

public:
	explicit constexpr inline Material(const intrusive2_ptr<BaseMaterial>& self) noexcept :
		self(self) {}

	explicit constexpr inline Material(intrusive2_ptr<BaseMaterial>&& self) noexcept :
		self(std::move(self)) {}

	template <typename T, typename... Args>
	[[nodiscard]] static constexpr inline Material create(Args&&... args) {
		return Material{make_intrusive2_ptr<T>(std::forward<Args>(args)...)};
	}

public:
	inline void bind(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) const noexcept {
		self->bind(glr, uniform_stream);
	}

	[[nodiscard]] inline bool utilizes(libv::vm4::VertexAttribute attribute) const noexcept {
		return self->utilizes(attribute);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
