//

#pragma once

// libv
// #include <libv/glr/fwd.hpp>
// #include <libv/math/vec.hpp>
// #include <libv/utility/memory/intrusive2_ptr.hpp>
// #include <libv/vm4/fwd.hpp>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/resource/material_override.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class InternalStaticMesh;

/// Handler type
class StaticMesh {
// private:
// 	libv::intrusive2_ptr<InternalStaticMesh> self;
//
// public:
// 	// StaticMesh() = default; We could have a default ctor, we will see if we need one
// 	explicit StaticMesh(libv::intrusive2_ptr<InternalStaticMesh> internal);
// 	StaticMesh(const libv::vm4::StaticMesh& StaticMesh, const Material& mov);
// 	StaticMesh(const libv::vm4::StaticMesh& StaticMesh, const MaterialLoaderFunc& mov);
//
// 	StaticMesh(const StaticMesh&) noexcept;
// 	StaticMesh& operator=(const StaticMesh&) & noexcept;
// 	StaticMesh(StaticMesh&&) noexcept;
// 	StaticMesh& operator=(StaticMesh&&) & noexcept;
// 	~StaticMesh();
//
// public:
// 	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) const;
//
// public:
// 	[[nodiscard]] bool isLoaded() const noexcept;
// 	/// Requires loaded state
// 	[[nodiscard]] libv::vec3f AABB_max() const noexcept;
// 	/// Requires loaded state
// 	[[nodiscard]] libv::vec3f AABB_min() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
