// Project: libv.re, File: src/libv/re/resource/material_loader.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/r.hpp> // Convenience include to access the libv::re::r.material object
#include <libv/re/resource/material_func.hpp>
#include <libv/utility/nexus_fwd.hpp>

#include <memory>
#include <string>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

/// Handler type
class MaterialLoader {
private:
	std::unique_ptr<ImplMaterialLoader> self;

public:
	MaterialLoader(const Settings& settings, libv::Nexus& nexus, libv::GL& gl);
	void init();
	void reset();
	~MaterialLoader();

public:
	void registerMaterial(std::string name, MaterialScanFunc func);

public:
	[[nodiscard]] Material_ptr load(const libv::vm4::Material& vm4Material);
	[[nodiscard]] Material_ptr fallback() const;
	[[nodiscard]] MaterialScanner scanner(const libv::vm4::Material& vm4Material) const;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
