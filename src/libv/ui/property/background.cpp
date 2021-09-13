// Project: libv.ui, File: src/libv/ui/property/color.hpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/property/background.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/component/detail/core_component.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/context/context_ui_link.hpp>
#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BaseBackground {
	int32_t ref_count = 0;
	friend void intrusive_ptr_add_ref(BaseBackground*);
	friend void intrusive_ptr_release(BaseBackground*);

//public:
//	static libv::intrusive_ptr<BaseBackground> parse_string(std::string_view);
//	static libv::intrusive_ptr<BaseBackground> parse_lua(const sol::object& object);

public:
	virtual void render(class Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) = 0;
	[[nodiscard]] virtual std::string to_string() const = 0;
	[[nodiscard]] virtual libv::vec2i size() const noexcept = 0;
	[[nodiscard]] virtual bool equal_to(const BaseBackground& other) const noexcept = 0;

public:
	virtual ~BaseBackground() = default;
};

void intrusive_ptr_add_ref(BaseBackground* var) {
	++var->ref_count;
}

void intrusive_ptr_release(BaseBackground* var) {
	if (--var->ref_count == 0)
		delete var;
}

// =================================================================================================

class BackgroundNone : public BaseBackground {
private:
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
		(void) r;
		(void) pos;
		(void) size;
		(void) component;
	}

	virtual std::string to_string() const override {
		return "none";
	}

	virtual libv::vec2i size() const noexcept override {
		return libv::vec2i{0, 0};
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundNone&>(other_base);
		(void) other;
		return true;
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundColor : public BaseBackground {
private:
	Color color;
	ShaderQuad_view shader;

public:
	BackgroundColor(Color color, ShaderQuad_view shader) :
			color(color),
			shader(std::move(shader)) {}

private:
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
		(void) component;

		r.quad(pos, size, color, shader);
	}

	virtual std::string to_string() const override {
		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
	}

	virtual libv::vec2i size() const noexcept override {
		return libv::vec2i{0, 0};
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundColor&>(other_base);
		return color == other.color && shader == other.shader;
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundTexture : public BaseBackground {
private:
	Color color;
	Texture2D_view texture;
	ShaderImage_view shader;

public:
	BackgroundTexture(Color color, Texture2D_view texture, ShaderImage_view shader) :
			color(color),
			texture(std::move(texture)),
			shader(std::move(shader)) {}

private:
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
		(void) component;

		r.texture_2D(pos, size, {0, 0}, {1, 1}, color, texture, shader);
	}

	virtual std::string to_string() const override {
//		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
		return "Not implemented yet"; // !!!
	}

	virtual libv::vec2i size() const noexcept override {
		return texture->size();
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundTexture&>(other_base);
		return color == other.color && texture == other.texture && shader == other.shader;
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundBorder : public BaseBackground {
private:
	Color color;
	Texture2D_view texture;
	ShaderImage_view shader;

public:
	BackgroundBorder(Color color, Texture2D_view texture, ShaderImage_view shader) :
			color(color),
			texture(std::move(texture)),
			shader(std::move(shader)) {}

private:
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
		(void) component;

		// y3   12--13--14--15
		//      | / | / | / |
		// y2   8---9---10--11
		//      | / | / | / |
		// y1   4---5---6---7
		//      | / | / | / |
		// y0   0---1---2---3
		//
		//      x0  x1  x2  x3

		const auto border_pos = min(texture->size().cast<float>(), size) * 0.5f;
		const auto border_tex = min(size / max(cast<float>(texture->size()), 1.0f) * 0.5f, 0.5f);

		const auto p0 = pos;
		const auto p1 = pos + border_pos;
		const auto p2 = pos + size - border_pos;
		const auto p3 = pos + size;

		const auto t0 = libv::vec2f{0.0f, 0.0f};
		const auto t1 = border_tex;
		const auto t2 = 1.0f - border_tex;
		const auto t3 = libv::vec2f{1.0f, 1.0f};

		r.begin_triangles();

		r.vertex({p0.x, p0.y, 0}, {t0.x, t0.y}, color);
		r.vertex({p1.x, p0.y, 0}, {t1.x, t0.y}, color);
		r.vertex({p2.x, p0.y, 0}, {t2.x, t0.y}, color);
		r.vertex({p3.x, p0.y, 0}, {t3.x, t0.y}, color);

		r.vertex({p0.x, p1.y, 0}, {t0.x, t1.y}, color);
		r.vertex({p1.x, p1.y, 0}, {t1.x, t1.y}, color);
		r.vertex({p2.x, p1.y, 0}, {t2.x, t1.y}, color);
		r.vertex({p3.x, p1.y, 0}, {t3.x, t1.y}, color);

		r.vertex({p0.x, p2.y, 0}, {t0.x, t2.y}, color);
		r.vertex({p1.x, p2.y, 0}, {t1.x, t2.y}, color);
		r.vertex({p2.x, p2.y, 0}, {t2.x, t2.y}, color);
		r.vertex({p3.x, p2.y, 0}, {t3.x, t2.y}, color);

		r.vertex({p0.x, p3.y, 0}, {t0.x, t3.y}, color);
		r.vertex({p1.x, p3.y, 0}, {t1.x, t3.y}, color);
		r.vertex({p2.x, p3.y, 0}, {t2.x, t3.y}, color);
		r.vertex({p3.x, p3.y, 0}, {t3.x, t3.y}, color);

		r.index_strip({4, 0, 5, 1, 6, 2, 7, 3});
		r.index_strip({3, 8}); // jump
		r.index_strip({8, 4, 9, 5, 10, 6, 11, 7});
		r.index_strip({7, 12}); // jump
		r.index_strip({12, 8, 13, 9, 14, 10, 15, 11});

		r.end(texture, shader);
	}

	virtual std::string to_string() const override {
//		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
		return "Not implemented yet"; // !!!
	}

	virtual libv::vec2i size() const noexcept override {
		return texture->size();
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundBorder&>(other_base);
		return color == other.color && texture == other.texture && shader == other.shader;
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundPattern : public BaseBackground {
private:
	Color color;
	Texture2D_view texture;
	ShaderImage_view shader;

public:
	BackgroundPattern(Color color, Texture2D_view texture, ShaderImage_view shader) :
			color(color),
			texture(std::move(texture)),
			shader(std::move(shader)) {}

private:
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
		(void) component;

		const auto pattern_size = texture->size().cast<float>();

		r.texture_2D(pos, size, {0, 0}, 1.0f / pattern_size * size, {0, 0, 1, 1}, color, texture, shader);
	}

	virtual std::string to_string() const override {
//		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
		return "Not implemented yet"; // !!!
	}

	virtual libv::vec2i size() const noexcept override {
		return texture->size();
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundPattern&>(other_base);
		return color == other.color && texture == other.texture && shader == other.shader;
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundPaddingPattern : public BaseBackground {
private:
	Color color;
	Texture2D_view texture;
	ShaderImage_view shader;

public:
	BackgroundPaddingPattern(Color color, Texture2D_view texture, ShaderImage_view shader) :
			color(color),
			texture(std::move(texture)),
			shader(std::move(shader)) {}

private:
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
		// in the middle use texture as pattern but leave out non padding area

		// y3   12--13--14--15
		//      | / | / | / |
		// y2   8---9---10--11
		//      | / |   | / |
		// y1   4---5---6---7
		//      | / | / | / |
		// y0   0---1---2---3
		//
		//      x0  x1  x2  x3

		const auto content_bounds = component.getInnerContentBounds();
		const auto content_pos = xy(content_bounds);
		const auto content_size = zw(content_bounds);

		const auto pattern_size = texture->size().cast<float>();

		const auto p0 = pos;
		const auto p1 = pos + content_pos;
		const auto p2 = content_pos + content_size;
		const auto p3 = pos + size;

		const auto t0 = (p0 - pos) / pattern_size;
		const auto t1 = (p1 - pos) / pattern_size;
		const auto t2 = (p2 - pos) / pattern_size;
		const auto t3 = (p3 - pos) / pattern_size;

		r.begin_triangles();

		r.vertex({p0.x, p0.y, 0}, {t0.x, t0.y}, color);
		r.vertex({p1.x, p0.y, 0}, {t1.x, t0.y}, color);
		r.vertex({p2.x, p0.y, 0}, {t2.x, t0.y}, color);
		r.vertex({p3.x, p0.y, 0}, {t3.x, t0.y}, color);

		r.vertex({p0.x, p1.y, 0}, {t0.x, t1.y}, color);
		r.vertex({p1.x, p1.y, 0}, {t1.x, t1.y}, color);
		r.vertex({p2.x, p1.y, 0}, {t2.x, t1.y}, color);
		r.vertex({p3.x, p1.y, 0}, {t3.x, t1.y}, color);

		r.vertex({p0.x, p2.y, 0}, {t0.x, t2.y}, color);
		r.vertex({p1.x, p2.y, 0}, {t1.x, t2.y}, color);
		r.vertex({p2.x, p2.y, 0}, {t2.x, t2.y}, color);
		r.vertex({p3.x, p2.y, 0}, {t3.x, t2.y}, color);

		r.vertex({p0.x, p3.y, 0}, {t0.x, t3.y}, color);
		r.vertex({p1.x, p3.y, 0}, {t1.x, t3.y}, color);
		r.vertex({p2.x, p3.y, 0}, {t2.x, t3.y}, color);
		r.vertex({p3.x, p3.y, 0}, {t3.x, t3.y}, color);

		r.index_strip({4, 0, 5, 1, 6, 2, 7, 3});
		r.index_strip({3, 8}); // jump

		r.index_strip({8, 4, 9, 5});
		r.index_strip({5, 10}); // jump (skip middle quad)
		r.index_strip({10, 6, 11, 7});

		r.index_strip({7, 12}); // jump
		r.index_strip({12, 8, 13, 9, 14, 10, 15, 11});

		r.end(texture, shader);
	}

	virtual std::string to_string() const override {
//		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
		return "Not implemented yet"; // !!!
	}

	virtual libv::vec2i size() const noexcept override {
		return texture->size();
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundPaddingPattern&>(other_base);
		return color == other.color && texture == other.texture && shader == other.shader;
	}
};

// -------------------------------------------------------------------------------------------------

//class BackgroundGradientLinear : public BaseBackground {
//private:
//	enum class PointMode {
//		fixed,
//		relative, /// [0..1]
//	};
//	enum class Corner {
//		top_left,
//		top_center,
//		top_right,
//		center_left,
//		center_center,
//		center_right,
//		bottom_left,
//		bottom_center,
//		bottom_right,
//	};
//	struct Point {
//		float pos_x;
//		float pos_y;
//		PointMode mode_x;
//		PointMode mode_y;
//		Corner anchor;
//		Color color;
//	};
//
//private:
//	std::vector<Point> points;
//
////private:
////	libv::vec2f start;
////	PointMode start_mode;
////	libv::vec2f end;
////	PointMode end_mode;
////	std::vector<Color> colors;
//
//	ShaderQuad_view shader;
//
//public:
//	BackgroundGradientLinear(std::vector<Point> points, ShaderQuad_view shader) :
//			points(std::move(points)),
//			shader(std::move(shader)) {}
//
//private:
//	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
////		(void) r;
////		(void) pos;
////		(void) size;
////		(void) component;
//
////		const auto p0 = pos;
////		const auto p1 = pos + border_pos;
////		const auto p2 = pos + size - border_pos;
////		const auto p3 = pos + size;
////
////		const auto t0 = libv::vec2f{0.0f, 0.0f};
////		const auto t1 = border_tex;
////		const auto t2 = 1.0f - border_tex;
////		const auto t3 = libv::vec2f{1.0f, 1.0f};
//
////		r.begin_triangles();
//
////		for () {
////
////		}
//
////		r.vertex({p0.x, p0.y, 0}, {t0.x, t0.y}, color);
////		r.vertex({p1.x, p0.y, 0}, {t1.x, t0.y}, color);
////		r.vertex({p2.x, p0.y, 0}, {t2.x, t0.y}, color);
////		r.vertex({p3.x, p0.y, 0}, {t3.x, t0.y}, color);
////
////		r.vertex({p0.x, p1.y, 0}, {t0.x, t1.y}, color);
////		r.vertex({p1.x, p1.y, 0}, {t1.x, t1.y}, color);
////		r.vertex({p2.x, p1.y, 0}, {t2.x, t1.y}, color);
////		r.vertex({p3.x, p1.y, 0}, {t3.x, t1.y}, color);
////
////		r.vertex({p0.x, p2.y, 0}, {t0.x, t2.y}, color);
////		r.vertex({p1.x, p2.y, 0}, {t1.x, t2.y}, color);
////		r.vertex({p2.x, p2.y, 0}, {t2.x, t2.y}, color);
////		r.vertex({p3.x, p2.y, 0}, {t3.x, t2.y}, color);
////
////		r.vertex({p0.x, p3.y, 0}, {t0.x, t3.y}, color);
////		r.vertex({p1.x, p3.y, 0}, {t1.x, t3.y}, color);
////		r.vertex({p2.x, p3.y, 0}, {t2.x, t3.y}, color);
////		r.vertex({p3.x, p3.y, 0}, {t3.x, t3.y}, color);
////
////		r.index_strip({4, 0, 5, 1, 6, 2, 7, 3});
////		r.index_strip({3, 8}); // jump
////		r.index_strip({8, 4, 9, 5, 10, 6, 11, 7});
////		r.index_strip({7, 12}); // jump
////		r.index_strip({12, 8, 13, 9, 14, 10, 15, 11});
//
////		r.end(shader);
//	}
//	virtual std::string to_string() const override {
////		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
//		return "Not implemented yet"; // !!!
//	}
//};

// =================================================================================================

void Background::render(class Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) const {
	fragment->render(r, pos, size, component);
}

std::string Background::to_string() const {
	return fragment->to_string();
}

libv::vec2i Background::size() const noexcept {
	return fragment->size();
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] bool operator==(const Background& lhs, const Background& rhs) noexcept {
	if (lhs.fragment == nullptr || rhs.fragment == nullptr)
		return false;

	if (typeid(*lhs.fragment) != typeid(*rhs.fragment))
		return false;

	return lhs.fragment->equal_to(*rhs.fragment);
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] Background Background::none() {
	return Background{libv::make_intrusive<BackgroundNone>()};
}

[[nodiscard]] Background Background::color(Color color) {
	return Background{libv::make_intrusive<BackgroundColor>(color, current_thread_context().shaderQuad())};
}

[[nodiscard]] Background Background::color(Color color, ShaderQuad_view shader) {
	return Background{libv::make_intrusive<BackgroundColor>(color, std::move(shader))};
}

[[nodiscard]] Background Background::texture(Color color, Texture2D_view texture) {
	return Background{libv::make_intrusive<BackgroundTexture>(color, std::move(texture), current_thread_context().shaderImage())};
}

[[nodiscard]] Background Background::texture(Color color, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive<BackgroundTexture>(color, std::move(texture), std::move(shader))};
}

[[nodiscard]] Background Background::border(Color color, Texture2D_view texture) {
	return Background{libv::make_intrusive<BackgroundBorder>(color, std::move(texture), current_thread_context().shaderImage())};
}

[[nodiscard]] Background Background::border(Color color, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive<BackgroundBorder>(color, std::move(texture), std::move(shader))};
}

[[nodiscard]] Background Background::pattern(Color color, Texture2D_view texture) {
	return Background{libv::make_intrusive<BackgroundPattern>(color, std::move(texture), current_thread_context().shaderImage())};
}

[[nodiscard]] Background Background::pattern(Color color, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive<BackgroundPattern>(color, std::move(texture), std::move(shader))};
}

[[nodiscard]] Background Background::padding_pattern(Color color, Texture2D_view texture) {
	return Background{libv::make_intrusive<BackgroundPaddingPattern>(color, std::move(texture), current_thread_context().shaderImage())};
}

[[nodiscard]] Background Background::padding_pattern(Color color, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive<BackgroundPaddingPattern>(color, std::move(texture), std::move(shader))};
}

//[[nodiscard]] static Background Background::gradient_linear(std::vector<GradientPoint> points) {
//
//}
//
// [[nodiscard]] static Background Background::gradient_linear(std::vector<GradientPoint> points, ShaderQuad_view shader) {
//
//}


// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
