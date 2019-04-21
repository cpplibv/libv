// File: string_2D.hpp Author: Vader Created on 2016. február 4., 6:25

#pragma once

// libv
#include <libv/glr/mesh.hpp>
#include <libv/math/vec.hpp>
// std
#include <string>
#include <string_view>
#include <vector>
// pro
#include <libv/ui/font_2D.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/font_size.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class Font2D;

//	struct Font {
//		FontEffect effect;   /// normal / outline / underline / strike-through ...
//		FontFamily family;   /// font-family: Consolas
//		FontSize size = 12;  /// font-size/line-height
//		FontStretch stretch; /// font-stretch - normal / ultra-condensed / extra-condensed / condensed / semi-condensed / semi-expanded / expanded / extra-expanded / ultra-expanded
//		FontStyle style;     /// font-style - normal / italic / oblique
//		FontVariant variant; /// font-variant - normal / small-caps
//		FontWeight weight;   /// font-weight - normal / bold / bolder / lighter / %number%
//
//		FontFamily family;   /// font-family: Consolas
//		uint64_t underline:1;
//		uint64_t strike_through:1;
//		uint64_t small_caps:1;
//		uint64_t italic:1;
//		uint64_t stretch:8; // 4.4 fixed point
//		uint64_t outline:12; // 8.4 fixed point
//		uint64_t weight:16; // 8.8 fixed point
//		uint64_t size:8;
//	};

class String2D {
private:
	bool dirty = true;

	libv::glr::Mesh mesh{libv::gl::Primitive::Triangles, libv::gl::BufferUsage::StaticDraw};

	std::shared_ptr<Font2D> font;
	FontSize fontSize = 12u;
	AlignHorizontal align = AlignHorizontal::Center;
	libv::vec2f limit;
	libv::vec3f position;

	std::string string;

public:
	String2D(std::string string = "", const AlignHorizontal align = AlignHorizontal::Center);

	void setAlign(const AlignHorizontal align);
	void setFont(std::shared_ptr<Font2D> font);
	void setFont(std::shared_ptr<Font2D> font, const FontSize fontSize);
	void setLimit(const libv::vec2f limit);
	inline void setLimit(const float x, const float y) {
		setLimit({x, y});
	}
	void setPosition(const libv::vec3f position_);
	inline void setPosition(const float x, const float y, const float z = 0) {
		setPosition({x, y, z});
	}
	void setSize(const FontSize fontSize);
	void setString(std::string string);
	inline void set(const AlignHorizontal align_, const libv::vec3f position_, const libv::vec2f limit_, std::shared_ptr<Font2D> font_, const FontSize size_, std::string string_) {
		setAlign(align_);
		setLimit(limit_);
		setFont(std::move(font_), size_);
		setPosition(position_);
		setString(std::move(string_));
		layout();
	}

public:
	void push_back(const std::string_view string_);
	void pop_back();

public:
	inline const std::string& getString() const {
		return string;
	}

	/// @param limit - The maximum available space that can be used to layout
	/// @note Negative \c limit values are representing unbounded limits
	/// @return the necessary space to layout while obeying the \c limit
	libv::vec2f getContent(libv::vec2f limit);

	/// @param x - The maximum available width that can be used to layout
	/// @param y - The maximum available height that can be used to layout
	/// @note Negative \c x and \c y values are representing unbounded limits
	/// @return the necessary space to layout while obeying the \c limit
	inline libv::vec2f getContent(const float x, const float y) {
		return getContent({x, y});
	}

public:
	void render(libv::glr::Queue& gl);

private:
	void layout();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
