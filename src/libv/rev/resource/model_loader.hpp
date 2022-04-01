// Project: libv.rev, File: src/libv/rev/resource/model_loader.hpp

#pragma once

// fwd
#include <libv/rev/fwd.hpp>
// std
#include <memory>
#include <string_view>
// pro
#include <libv/rev/resource/material_override.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

/// Handler type
class ModelLoader {
private:
	std::shared_ptr<InternalModelLoader> self;

public:
	explicit ModelLoader(const std::shared_ptr<InternalResourceManager>& irm, const MaterialLoader& materialLoader);
	inline ModelLoader(const ModelLoader&) noexcept = default;
	inline ModelLoader& operator=(const ModelLoader&) & noexcept = default;
	inline ModelLoader(ModelLoader&&) noexcept = default;
	inline ModelLoader& operator=(ModelLoader&&) & noexcept = default;
	inline ~ModelLoader() = default;

public:
	[[nodiscard]] Model load(std::string_view name);
	[[nodiscard]] Model load(std::string_view name, Material material, int64_t movID = -1);
	[[nodiscard]] Model load(std::string_view name, MaterialScanFunc scanFn, int64_t movID = -1);
	[[nodiscard]] Model load(std::string_view name, MaterialLoaderFunc loadFn, int64_t movID = -1);
	[[nodiscard]] Model fallback() const;
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
