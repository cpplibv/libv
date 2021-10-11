// Project: libv.ui, File: src/libv/ui/property/background.cpp

// hpp
#include <libv/ui/property/background.hpp>
// ext
#include <fmt/format.h>
// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/ui/component/component_core.hpp>
#include <libv/ui/context/context_render.hpp>
#include <libv/ui/context/context_resource.hpp>
#include <libv/ui/context/context_ui.hpp>
#include <libv/ui/context/context_ui_link.hpp>
#include <libv/ui/resource/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

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

	virtual libv::vec2f size() const noexcept override {
		return libv::vec2f{0, 0};
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundNone&>(other_base);
		(void) other; // None's are always equal, they are stateless
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

	virtual libv::vec2f size() const noexcept override {
		return libv::vec2f{0, 0};
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
		return "Not implemented yet"; // TODO P5: Implement to_string
	}

	virtual libv::vec2f size() const noexcept override {
		return texture->size().cast<float>();
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
		const auto border_tex = min(size / max(texture->size().cast<float>(), 1.0f) * 0.5f, 0.5f);

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
		return "Not implemented yet"; // TODO P5: Implement to_string
	}

	virtual libv::vec2f size() const noexcept override {
		return texture->size().cast<float>();
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
		return "Not implemented yet"; // TODO P5: Implement to_string
	}

	virtual libv::vec2f size() const noexcept override {
		return texture->size().cast<float>();
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
	Padding inner_padding;
	Texture2D_view texture;
	ShaderImage_view shader;

public:
	BackgroundPaddingPattern(Color color, Padding inner_padding, Texture2D_view texture, ShaderImage_view shader) :
			color(color),
			inner_padding(inner_padding),
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
		const auto p3 = pos + size;
		const auto p1 = libv::vec::clamp(pos + content_pos - xy(inner_padding), p0, p3);
		const auto p2 = libv::vec::clamp(content_pos + content_size + zw(inner_padding), p1, p3);

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
		return "Not implemented yet"; // TODO P5: Implement to_string
	}

	virtual libv::vec2f size() const noexcept override {
		return texture->size().cast<float>();
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundPaddingPattern&>(other_base);
		return
				color == other.color &&
				inner_padding == other.inner_padding &&
				texture == other.texture &&
				shader == other.shader;
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundBorderPaddingPattern : public BaseBackground {
private:
	Color color_border;
	Color color_pattern;
	Padding inner_padding;
	Padding border_extent;
	Texture2D_view texture;
	ShaderImage_view shader;

public:
	BackgroundBorderPaddingPattern(Color color_border, Color color_pattern, Padding inner_padding, Padding border_extent, Texture2D_view texture, ShaderImage_view shader) :
			color_border(color_border),
			color_pattern(color_pattern),
			inner_padding(inner_padding),
			border_extent(border_extent),
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

		const auto texture_size = texture->size().cast<float>();

		const auto pattern_region_pos = pos + xy(border_extent);
		const auto pattern_region_size = size - xy(border_extent) - zw(border_extent);

		const auto pattern_texture_pos = xy(border_extent);
		const auto pattern_texture_size = texture_size - xy(border_extent) - zw(border_extent);

		r.begin_triangles();

		{
			// Border

			const auto p0 = pos;
			const auto p3 = pos + size;
			const auto p1 = libv::vec::clamp(pos + xy(border_extent), p0, p3);
			const auto p2 = libv::vec::clamp(pos + size - zw(border_extent), p1, p3);

			const auto t0 = libv::vec2f{0.0f, 0.0f};
			const auto t3 = libv::vec2f{1.0f, 1.0f};
			const auto t1 = libv::vec::clamp(xy(border_extent) / texture_size, t0, t3);
			const auto t2 = libv::vec::clamp(1.0f - zw(border_extent) / texture_size, t1, t3);

			r.vertex({p0.x, p0.y, 0}, {t0.x, t0.y}, color_border);
			r.vertex({p1.x, p0.y, 0}, {t1.x, t0.y}, color_border);
			r.vertex({p2.x, p0.y, 0}, {t2.x, t0.y}, color_border);
			r.vertex({p3.x, p0.y, 0}, {t3.x, t0.y}, color_border);

			r.vertex({p0.x, p1.y, 0}, {t0.x, t1.y}, color_border);
			r.vertex({p1.x, p1.y, 0}, {t1.x, t1.y}, color_border);
			r.vertex({p2.x, p1.y, 0}, {t2.x, t1.y}, color_border);
			r.vertex({p3.x, p1.y, 0}, {t3.x, t1.y}, color_border);

			r.vertex({p0.x, p2.y, 0}, {t0.x, t2.y}, color_border);
			r.vertex({p1.x, p2.y, 0}, {t1.x, t2.y}, color_border);
			r.vertex({p2.x, p2.y, 0}, {t2.x, t2.y}, color_border);
			r.vertex({p3.x, p2.y, 0}, {t3.x, t2.y}, color_border);

			r.vertex({p0.x, p3.y, 0}, {t0.x, t3.y}, color_border);
			r.vertex({p1.x, p3.y, 0}, {t1.x, t3.y}, color_border);
			r.vertex({p2.x, p3.y, 0}, {t2.x, t3.y}, color_border);
			r.vertex({p3.x, p3.y, 0}, {t3.x, t3.y}, color_border);

			r.index_strip({4, 0, 5, 1, 6, 2, 7, 3});
			// TODO P2: This pattern for jump is with index_strip is extreamly wasteful (~4 extra triangle), implement better support for jump
			r.index_strip({3, 8}); // jump

			r.index_strip({8, 4, 9, 5});
			r.index_strip({5, 10}); // jump (skip middle quad)
			r.index_strip({10, 6, 11, 7});

			r.index_strip({7, 12}); // jump
			r.index_strip({12, 8, 13, 9, 14, 10, 15, 11});
		}
		r.index_strip({11, 16 + 4}); // jump
		{
			// Pattern

			const auto p0 = pattern_region_pos;
			const auto p3 = pattern_region_pos + pattern_region_size;
			const auto p1 = libv::vec::clamp(content_pos - xy(inner_padding), p0, p3);
			const auto p2 = libv::vec::clamp(content_pos + content_size + zw(inner_padding), p1, p3);

			const auto t0 = (p0 - pattern_region_pos) / pattern_texture_size;
			const auto t1 = (p1 - pattern_region_pos) / pattern_texture_size;
			const auto t2 = (p2 - pattern_region_pos) / pattern_texture_size;
			const auto t3 = (p3 - pattern_region_pos) / pattern_texture_size;

			const auto tile = libv::vec4f(pattern_texture_pos / texture_size, pattern_texture_size / texture_size);

			r.vertex({p0.x, p0.y, 0}, {t0.x, t0.y}, tile, color_pattern);
			r.vertex({p1.x, p0.y, 0}, {t1.x, t0.y}, tile, color_pattern);
			r.vertex({p2.x, p0.y, 0}, {t2.x, t0.y}, tile, color_pattern);
			r.vertex({p3.x, p0.y, 0}, {t3.x, t0.y}, tile, color_pattern);

			r.vertex({p0.x, p1.y, 0}, {t0.x, t1.y}, tile, color_pattern);
			r.vertex({p1.x, p1.y, 0}, {t1.x, t1.y}, tile, color_pattern);
			r.vertex({p2.x, p1.y, 0}, {t2.x, t1.y}, tile, color_pattern);
			r.vertex({p3.x, p1.y, 0}, {t3.x, t1.y}, tile, color_pattern);

			r.vertex({p0.x, p2.y, 0}, {t0.x, t2.y}, tile, color_pattern);
			r.vertex({p1.x, p2.y, 0}, {t1.x, t2.y}, tile, color_pattern);
			r.vertex({p2.x, p2.y, 0}, {t2.x, t2.y}, tile, color_pattern);
			r.vertex({p3.x, p2.y, 0}, {t3.x, t2.y}, tile, color_pattern);

			r.vertex({p0.x, p3.y, 0}, {t0.x, t3.y}, tile, color_pattern);
			r.vertex({p1.x, p3.y, 0}, {t1.x, t3.y}, tile, color_pattern);
			r.vertex({p2.x, p3.y, 0}, {t2.x, t3.y}, tile, color_pattern);
			r.vertex({p3.x, p3.y, 0}, {t3.x, t3.y}, tile, color_pattern);

			r.index_strip({16 + 4, 16 + 0, 16 + 5, 16 + 1, 16 + 6, 16 + 2, 16 + 7, 16 + 3});
			r.index_strip({16 + 3, 16 + 8}); // jump

			r.index_strip({16 + 8, 16 + 4, 16 + 9, 16 + 5});
			r.index_strip({16 + 5, 16 + 10}); // jump (skip middle quad)
			r.index_strip({16 + 10, 16 + 6, 16 + 11, 16 + 7});

			r.index_strip({16 + 7, 16 + 12}); // jump
			r.index_strip({16 + 12, 16 + 8, 16 + 13, 16 + 9, 16 + 14, 16 + 10, 16 + 15, 16 + 11});
		}

		r.end(texture, shader);
	}

	virtual std::string to_string() const override {
//		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
		return "Not implemented yet"; // TODO P5: Implement to_string
	}

	virtual libv::vec2f size() const noexcept override {
		return texture->size().cast<float>();
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundBorderPaddingPattern&>(other_base);
		return
				color_border == other.color_border &&
				color_pattern == other.color_pattern &&
				inner_padding == other.inner_padding &&
				border_extent == other.border_extent &&
				texture == other.texture &&
				shader == other.shader;
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
//		return "Not implemented yet"; // TODO P5: Implement to_string
//	}
//};

// -------------------------------------------------------------------------------------------------

class BackgroundNested : public BaseBackground {
private:
	Background back;
	Background front;

public:
	BackgroundNested(Background&& back, Background&& front) :
		back(std::move(back)),
		front(std::move(front)) {}

private:
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) override {
		// TODO P1: Adjust pos and size based on front (passing in component might be missleading, might need another abstraction for "inner region" getter)
//		front.render(r, pos, size, component);
//		back.render(r, pos, size, component);
	}

	virtual std::string to_string() const override {
//		return fmt::format("color: rgba({}, {}, {}, {})", color.x, color.y, color.z, color.w);
		return "Not implemented yet"; // TODO P5: Implement to_string
	}

	virtual libv::vec2i size() const noexcept override {
		return max(back.size(), front.size());
	}

	virtual bool equal_to(const BaseBackground& other_base) const noexcept override {
		const auto& other = static_cast<const BackgroundNested&>(other_base);
		return
				back == other.back &&
				front == other.front;
	}
};


// =================================================================================================

/// BackgroundNone has only a single instance that is shared for all defaults and the 'none' itself
const auto global_shared_none = libv::make_intrusive2_ref<BackgroundNone>();

Background::Background() noexcept :
	fragment(global_shared_none) {
}

void Background::render(class Renderer& r, libv::vec2f pos, libv::vec2f size, CoreComponent& component) const {
	if (!r.cull_test(pos, size))
		return;
	fragment->render(r, pos, size, component);
}

// -------------------------------------------------------------------------------------------------

[[nodiscard]] Background Background::none() {
	return Background{global_shared_none};
}

[[nodiscard]] Background Background::color(Color color) {
	return Background{libv::make_intrusive2_ref<BackgroundColor>(color, current_thread_context().resource.shaderQuad())};
}

[[nodiscard]] Background Background::color(Color color, ShaderQuad_view shader) {
	return Background{libv::make_intrusive2_ref<BackgroundColor>(color, std::move(shader))};
}

[[nodiscard]] Background Background::texture(Color color, Texture2D_view texture) {
	return Background{libv::make_intrusive2_ref<BackgroundTexture>(color, std::move(texture), current_thread_context().resource.shaderImage())};
}

[[nodiscard]] Background Background::texture(Color color, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive2_ref<BackgroundTexture>(color, std::move(texture), std::move(shader))};
}

[[nodiscard]] Background Background::border(Color color, Texture2D_view texture) {
	return Background{libv::make_intrusive2_ref<BackgroundBorder>(color, std::move(texture), current_thread_context().resource.shaderImage())};
}

[[nodiscard]] Background Background::border(Color color, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive2_ref<BackgroundBorder>(color, std::move(texture), std::move(shader))};
}

[[nodiscard]] Background Background::pattern(Color color, Texture2D_view texture) {
	return Background{libv::make_intrusive2_ref<BackgroundPattern>(color, std::move(texture), current_thread_context().resource.shaderImage())};
}

[[nodiscard]] Background Background::pattern(Color color, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive2_ref<BackgroundPattern>(color, std::move(texture), std::move(shader))};
}

[[nodiscard]] Background Background::padding_pattern(Color color, Padding inner_padding, Texture2D_view texture) {
	return Background{libv::make_intrusive2_ref<BackgroundPaddingPattern>(color, inner_padding, std::move(texture), current_thread_context().resource.shaderImage())};
}

[[nodiscard]] Background Background::padding_pattern(Color color, Padding inner_padding, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive2_ref<BackgroundPaddingPattern>(color, inner_padding, std::move(texture), std::move(shader))};
}

[[nodiscard]] Background Background::border_padding_pattern(Color color_border, Color color_pattern, Padding inner_padding, Padding border_extent, Texture2D_view texture) {
	return Background{libv::make_intrusive2_ref<BackgroundBorderPaddingPattern>(color_border, color_pattern, inner_padding, border_extent, std::move(texture), current_thread_context().resource.shaderImage())};
}

[[nodiscard]] Background Background::border_padding_pattern(Color color_border, Color color_pattern, Padding inner_padding, Padding border_extent, Texture2D_view texture, ShaderImage_view shader) {
	return Background{libv::make_intrusive2_ref<BackgroundBorderPaddingPattern>(color_border, color_pattern, inner_padding, border_extent, std::move(texture), std::move(shader))};
}

//[[nodiscard]] static Background Background::gradient_linear(std::vector<GradientPoint> points) {
//
//}
//
// [[nodiscard]] static Background Background::gradient_linear(std::vector<GradientPoint> points, ShaderQuad_view shader) {
//
//}

[[nodiscard]] Background Background::nested(Background back, Background front) {
	return Background{libv::make_intrusive<BackgroundNested>(std::move(back), std::move(front))};
}

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
