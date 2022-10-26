// Project: libv.ui, File: src/libv/ui/component/image.cpp

// hpp
#include <libv/ui/component/image.hpp>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/context/context_layout.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/property_system/property_access.hpp>
#include <libv/ui/resource/shader_image.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct CoreImage : CoreComponent {
	using base_type = CoreComponent;
	using base_type::base_type;

public:
	struct Properties {
		PropertyR<Background> background;
	} property;

	template <typename T> static void access_properties(T& ctx);

public:
	virtual void doStyle(StyleAccess& access) override;
	virtual libv::vec3f doLayout1(const ContextLayout1& environment) override;
	virtual void doRender(Renderer& r) override;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
void CoreImage::access_properties(T& ctx) {
	ctx.property(
			[](auto& c) -> auto& { return c.property.background; },
			Background::none(),
			pgr::appearance, pnm::background,
			"Background"
	);
}

// -------------------------------------------------------------------------------------------------

void CoreImage::doStyle(StyleAccess& access) {
	access.self(*this);
}

libv::vec3f CoreImage::doLayout1(const ContextLayout1& environment) {
	(void) environment;
	const auto dynamic_size_image = property.background().size().cast<float>() + padding_size();

	return {dynamic_size_image, 0.f};
}

void CoreImage::doRender(Renderer& r) {
	property.background().render(r, {0, 0}, layout_size2(), *this);
}

// =================================================================================================

core_ptr Image::create_core(std::string name) {
	return create_core_ptr<CoreType>(std::move(name));
}

bool Image::castable(libv::ui::core_ptr core) noexcept {
	return dynamic_cast<CoreType*>(core) != nullptr;
}

// -------------------------------------------------------------------------------------------------

void Image::background(Background value) {
	AccessProperty::manual(self(), self().property.background, std::move(value));
}

[[nodiscard]] const Background& Image::background() const noexcept {
	return self().property.background();
}


// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
