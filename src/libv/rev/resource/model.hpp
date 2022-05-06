// Project: libv.rev, File: src/libv/rev/resource/model.hpp

#pragma once

// libv
#include <libv/glr/fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/memory/intrusive2_ptr.hpp>
#include <libv/vm4/fwd.hpp>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/resource/material_override.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

/// Handler type
class Model {
private:
	libv::intrusive2_ptr<InternalModel> self;

public:
	// Model() = default; We could have a default ctor, we will see if we need one
	explicit Model(libv::intrusive2_ptr<InternalModel> internal);
	Model(const libv::vm4::Model& model, const Material& mov);
	Model(const libv::vm4::Model& model, const MaterialLoaderFunc& mov);

	Model(const Model&) noexcept;
	Model& operator=(const Model&) & noexcept;
	Model(Model&&) noexcept;
	Model& operator=(Model&&) & noexcept;
	~Model();

public:
	void render(libv::glr::Queue& glr, libv::glr::UniformBuffer& uniform_stream) const;

public:
	[[nodiscard]] bool isLoaded() const noexcept;
	/// Requires loaded state
	[[nodiscard]] libv::vec3f AABB_max() const noexcept;
	/// Requires loaded state
	[[nodiscard]] libv::vec3f AABB_min() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
