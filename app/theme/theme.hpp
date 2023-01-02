// Project: libv, File: app/theme/theme.hpp

#pragma once

// libv
#include <libv/algo/pack_linear_lines.hpp>
#include <libv/algo/pack_rect_bin.hpp>
#include <libv/container/vector_2d.hpp>
#include <libv/math/vec.hpp>
#include <libv/meta/reflection_access.hpp>
#include <libv/serial/limit.hpp>
#include <libv/utility/concat.hpp>
// std
#include <vector>
#include <string>
#include <span>
#include <map>

#include <libv/ui/property_system/property.hpp>
// #include <libv/ui/style/style.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct AtlasEntry {
	libv::serial::limit<std::string, 1024> name;
	libv::vec2f texture_00;
	libv::vec2f texture_11;

	LIBV_REFLECTION_ACCESS(name);
	LIBV_REFLECTION_ACCESS(texture_00);
	LIBV_REFLECTION_ACCESS(texture_11);
};

// -------------------------------------------------------------------------------------------------

struct Atlas {
	libv::serial::limit<std::vector<AtlasEntry>, 2048> entries;
	libv::serial::limit<libv::vector_2D<libv::vec4uc>, 8192 * 8192> image;

//	LIBV_REFLECTION_ACCESS(entries);

public:
	explicit inline Atlas(libv::vec2z size) : image(size) {}
};

// -------------------------------------------------------------------------------------------------

struct Theme {
	struct Property {
		std::string name;
		libv::ui::PropertyDynamic value;
	};

	Atlas atlas;
	std::map<std::string, std::vector<Property>> properties;

//	LIBV_REFLECTION_ACCESS(atlas);
};

// -------------------------------------------------------------------------------------------------

//struct StyleCenter {
//	std::unordered_map<std::string, libv::intrusive2_ptr<Style>> styles;
//
//	libv::intrusive2_ptr<Style> ContextUI::style(const std::string_view style_name) {
//		// TODO P5: std::string(string_view) for hash lookup, I know there is or there will be a solution for it
//		const auto it = self->styles.find(std::string(style_name));
//		if (it != self->styles.end())
//			return it->second;
//
//		const auto result = self->styles.emplace(style_name, libv::make_intrusive<Style>(std::string(style_name))).first->second;
//		const auto lastDot = style_name.rfind('.');
//
//		if (lastDot != style_name.npos)
//			// Auto inherit based on dot naming hierarchy
//			result->inherit(style(style_name.substr(0, lastDot)));
//
//		log_ui.trace("Created style {}", style_name);
//
//		return result;
//	}
//};

// -------------------------------------------------------------------------------------------------

class AtlasBuilder {
	struct Entry {
		std::string name;
		libv::vector_2D<libv::vec4uc> image;
	};

	std::vector<Entry> entries;

private:
	template <typename SRC, typename DST>
	static inline void copy_2D(const SRC& src, libv::vec2z src_pos, DST& dst, libv::vec2z dst_pos, libv::vec2z size) noexcept {
		for (std::size_t y = 0; y < size.y; ++y)
			for (std::size_t x = 0; x < size.x; ++x)
				dst(dst_pos.x + x, dst_pos.y + y) = src(src_pos.x + x, src_pos.y + y);
	}

	template <typename SRC, typename DST>
	static inline void copy_2D_to(const SRC& src, DST& dst, libv::vec2z dst_pos) noexcept {
		const auto size = src.size();
		for (std::size_t y = 0; y < size.y; ++y)
			for (std::size_t x = 0; x < size.x; ++x)
				dst(dst_pos.x + x, dst_pos.y + y) = src(x, y);
	}

public:
	void add(std::string name, libv::vector_2D<libv::vec4uc> image) {
		entries.emplace_back(name, std::move(image));
	}

	Atlas build_atlas(libv::vec2i size) {
		std::vector<libv::vec2i> sizes(entries.size());

		for (std::size_t i = 0; i < entries.size(); ++i)
			sizes[i] = entries[i].image.size().cast<int32_t>();

//		libv::pack_rect_bin pack(size);
//      TODO P1: switch to rect_bin atlas packer

		const auto pack_result = app::pack_linear_lines(size, sizes);

		if (pack_result.size() != entries.size())
			throw std::out_of_range(libv::concat("Theme does not fit into the atlas image of size (", size.x, ", ", size.y, ")"));

		const auto sizef = size.cast<float>();
		auto atlas = Atlas(size.cast<std::size_t>());

		for (const auto& pack_item : pack_result) {
			const auto& name = entries[pack_item.id].name;
			const auto& image = entries[pack_item.id].image;

			const auto pack_position = pack_item.position;
			const auto pack_size = pack_item.size;

			const auto texture_00 = pack_position.cast<float>() / sizef;
			const auto texture_11 = (pack_position + pack_size).cast<float>() / sizef;

			copy_2D_to(image, atlas.image, pack_position.cast<std::size_t>());
			atlas.entries.emplace_back(name, texture_00, texture_11);
		}

		return atlas;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
