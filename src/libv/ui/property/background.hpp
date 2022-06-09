// Project: libv.ui, File: src/libv/ui/property/background.hpp

#pragma once

// fwd
#include <libv/ui/fwd.hpp>
// libv
#include <libv/math/vec_fwd.hpp>
#include <libv/utility/memory/intrusive_ptr.hpp>
// pro
#include <libv/ui/property/color.hpp>
#include <libv/ui/property/padding.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/shader_quad.hpp>
#include <libv/ui/property/texture_2D.hpp>
// std
#include <string>


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
	explicit Background() noexcept; // Uses none
	explicit inline Background(intrusive_ptr<libv::ui::BaseBackground> fragment) noexcept : fragment(std::move(fragment)) {}

public:
	void render(class Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) const;
	[[nodiscard]] std::string to_string() const;
	[[nodiscard]] libv::vec2i size() const noexcept;

public:
//	[[nodiscard]] friend bool operator==(const Background& lhs, const Background& rhs) noexcept;
	friend bool operator==(const Background& lhs, const Background& rhs) noexcept;
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
	[[nodiscard]] static Background padding_pattern(Color color, Padding inner_padding, Texture2D_view texture);
	[[nodiscard]] static Background padding_pattern(Color color, Padding inner_padding, Texture2D_view texture, ShaderImage_view shader);
	[[nodiscard]] static Background border_padding_pattern(Color color_border, Color color_pattern, Padding inner_padding, Padding border_extent, Texture2D_view texture);
	[[nodiscard]] static Background border_padding_pattern(Color color_border, Color color_pattern, Padding inner_padding, Padding border_extent, Texture2D_view texture, ShaderImage_view shader);
//	[[nodiscard]] static Background gradient_linear(std::vector<GradientPoint> points);
//	[[nodiscard]] static Background gradient_linear(std::vector<GradientPoint> points, ShaderQuad_view shader);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
