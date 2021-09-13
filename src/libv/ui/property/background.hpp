// Project: libv.ui, File: src/libv/ui/property/color.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/math/vec_fwd.hpp>
#include <libv/utility/intrusive_ptr.hpp>
// pro
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/shader_quad.hpp>
#include <libv/ui/property/texture_2D.hpp>
// std
#include <string>
#include <string_view>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BaseBackground;
void intrusive_ptr_add_ref(BaseBackground*);
void intrusive_ptr_release(BaseBackground*);

// -------------------------------------------------------------------------------------------------

class Background {
private:
	libv::intrusive_ptr<BaseBackground> fragment;

public:
	explicit inline Background() : fragment(nullptr) {} // !!! (? use none instead)
	explicit inline Background(intrusive_ptr<libv::ui::BaseBackground> fragment) : fragment(std::move(fragment)) {}

public:
	void render(class Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) const;
	[[nodiscard]] std::string to_string() const;

public:
	[[nodiscard]] friend inline bool operator==(const Background& lhs, const Background& rhs) noexcept {
		return lhs.fragment == rhs.fragment;
	}
	[[nodiscard]] friend inline bool operator!=(const Background& lhs, const Background& rhs) noexcept {
		return !(lhs == rhs);
	}

	[[nodiscard]] static Background none();
	[[nodiscard]] static Background color(Color color);
	[[nodiscard]] static Background color(Color color, ShaderQuad_view shader);
	[[nodiscard]] static Background texture(Color color, Texture2D_view texture);
	[[nodiscard]] static Background texture(Color color, Texture2D_view texture, ShaderImage_view shader);
	[[nodiscard]] static Background border(Color color, Texture2D_view texture);
	[[nodiscard]] static Background border(Color color, Texture2D_view texture, ShaderImage_view shader);
	[[nodiscard]] static Background pattern(Color color, Texture2D_view texture);
	[[nodiscard]] static Background pattern(Color color, Texture2D_view texture, ShaderImage_view shader);
	[[nodiscard]] static Background padding_pattern(Color color, Texture2D_view texture);
	[[nodiscard]] static Background padding_pattern(Color color, Texture2D_view texture, ShaderImage_view shader);
//	[[nodiscard]] static Background gradient_linear(std::vector<GradientPoint> points);
//	[[nodiscard]] static Background gradient_linear(std::vector<GradientPoint> points, ShaderQuad_view shader);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
