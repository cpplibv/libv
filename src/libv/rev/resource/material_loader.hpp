// Project: libv.rev, File: src/libv/rev/resource/material_loader.hpp

#pragma once

// libv
#include <libv/vm4/fwd.hpp>
// std
#include <memory>
#include <string>
// pro
#include <libv/rev/fwd.hpp>
#include <libv/rev/resource/material_override.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class ImplMaterialLoader;

/// Handler type
class MaterialLoader {
private:
	std::shared_ptr<ImplMaterialLoader> self;

public:
	MaterialLoader(ShaderLoader shaderLoader, TextureLoader textureLoader);
	inline MaterialLoader(const MaterialLoader&) noexcept = default;
	inline MaterialLoader& operator=(const MaterialLoader&) & noexcept = default;
	inline MaterialLoader(MaterialLoader&&) noexcept = default;
	inline MaterialLoader& operator=(MaterialLoader&&) & noexcept = default;
	inline ~MaterialLoader() = default;

public:
	void register_material(std::string name, MaterialScanFunc func);

public:
	[[nodiscard]] Material load(const libv::vm4::Material& vm4Material);
	[[nodiscard]] Material fallback() const;
	[[nodiscard]] MaterialScanner scanner(const libv::vm4::Material& vm4Material) const;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
