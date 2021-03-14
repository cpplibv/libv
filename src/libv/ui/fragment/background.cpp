// Project: libv.ui, File: src/libv/ui/fragment/background.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/ui/fragment/background.hpp>
// ext
// libv
#include <libv/math/vec.hpp>
// std
// pro
#include <libv/ui/context/context_render.hpp>



#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_quad.hpp>

#include <libv/ui/property/color.hpp>
#include <libv/ui/property/shader_image.hpp>
#include <libv/ui/property/texture_2D.hpp>

#include <libv/ui/texture_2D.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BackgroundNone : Background {
private:
	virtual libv::vec2f size() override {
		return {0, 0};
	}
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, libv::vec4f padding) override {
		(void) r;
		(void) pos;
		(void) size;
		(void) padding;
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundColor : Background {
private:
	Color color;
	ShaderQuad_view shader;

public:
	BackgroundColor(Color color, ShaderQuad_view shader) :
		color(color),
		shader(std::move(shader)) {}

private:
	virtual libv::vec2f size() override {
		return {0, 0};
	}
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, libv::vec4f padding) override {
		(void) padding;

		r.quad(pos, size, color, shader);
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundTexture : Background {
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
	virtual libv::vec2f size() override {
		return {0, 0};
	}
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, libv::vec4f padding) override {
		(void) padding;

		r.texture_2D(pos, size, {0, 0}, {1, 1}, color, texture, shader);
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundBorder : Background {
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
	virtual libv::vec2f size() override {
		return {0, 0};
	}
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, libv::vec4f padding) override {
		(void) padding;

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
};

// -------------------------------------------------------------------------------------------------

class BackgroundGradientLinear : Background {
private:
	enum class PointMode {
		fixed,
		relative, /// [0..1]
	};
	enum class Corner {
		top_left,
		top_center,
		top_right,
		center_left,
		center_center,
		center_right,
		bottom_left,
		bottom_center,
		bottom_right,
	};
	struct Point {
		float pos_x;
		float pos_y;
		PointMode mode_x;
		PointMode mode_y;
		Corner anchor;
		Color color;
	};

private:
	std::vector<Point> points;

//private:
//	libv::vec2f start;
//	PointMode start_mode;
//	libv::vec2f end;
//	PointMode end_mode;
//	std::vector<Color> colors;

	ShaderQuad_view shader;

public:
	BackgroundGradientLinear(std::vector<Point> points, ShaderQuad_view shader) :
		points(std::move(points)),
		shader(std::move(shader)) {}

private:
	virtual libv::vec2f size() override {
		return {0, 0};
	}
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, libv::vec4f padding) override {
//		(void) r;
//		(void) pos;
//		(void) size;
//		(void) padding;

//		const auto p0 = pos;
//		const auto p1 = pos + border_pos;
//		const auto p2 = pos + size - border_pos;
//		const auto p3 = pos + size;
//
//		const auto t0 = libv::vec2f{0.0f, 0.0f};
//		const auto t1 = border_tex;
//		const auto t2 = 1.0f - border_tex;
//		const auto t3 = libv::vec2f{1.0f, 1.0f};

//		r.begin_triangles();

//		for () {
//
//		}

//		r.vertex({p0.x, p0.y, 0}, {t0.x, t0.y}, color);
//		r.vertex({p1.x, p0.y, 0}, {t1.x, t0.y}, color);
//		r.vertex({p2.x, p0.y, 0}, {t2.x, t0.y}, color);
//		r.vertex({p3.x, p0.y, 0}, {t3.x, t0.y}, color);
//
//		r.vertex({p0.x, p1.y, 0}, {t0.x, t1.y}, color);
//		r.vertex({p1.x, p1.y, 0}, {t1.x, t1.y}, color);
//		r.vertex({p2.x, p1.y, 0}, {t2.x, t1.y}, color);
//		r.vertex({p3.x, p1.y, 0}, {t3.x, t1.y}, color);
//
//		r.vertex({p0.x, p2.y, 0}, {t0.x, t2.y}, color);
//		r.vertex({p1.x, p2.y, 0}, {t1.x, t2.y}, color);
//		r.vertex({p2.x, p2.y, 0}, {t2.x, t2.y}, color);
//		r.vertex({p3.x, p2.y, 0}, {t3.x, t2.y}, color);
//
//		r.vertex({p0.x, p3.y, 0}, {t0.x, t3.y}, color);
//		r.vertex({p1.x, p3.y, 0}, {t1.x, t3.y}, color);
//		r.vertex({p2.x, p3.y, 0}, {t2.x, t3.y}, color);
//		r.vertex({p3.x, p3.y, 0}, {t3.x, t3.y}, color);
//
//		r.index_strip({4, 0, 5, 1, 6, 2, 7, 3});
//		r.index_strip({3, 8}); // jump
//		r.index_strip({8, 4, 9, 5, 10, 6, 11, 7});
//		r.index_strip({7, 12}); // jump
//		r.index_strip({12, 8, 13, 9, 14, 10, 15, 11});

//		r.end(shader);
	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundPattern : Background {
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
	virtual libv::vec2f size() override {
		return {0, 0};
	}
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, libv::vec4f padding) override {

//		(void) r;
//		(void) pos;
//		(void) size;
//		(void) padding;

	}
};

// -------------------------------------------------------------------------------------------------

class BackgroundPaddingPattern : Background {
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
	virtual libv::vec2f size() override {
		return {0, 0};
	}
	virtual void render(Renderer& r, libv::vec2f pos, libv::vec2f size, libv::vec4f padding) override {

//		(void) r;
//		(void) pos;
//		(void) size;
//		(void) padding;

//		9 border table except the middle
//		in the middle use texture as pattern but leave out non padding area

//		Required by reference picture ref_ui_bg_padding_pattern

	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
