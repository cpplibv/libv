// Project: libv.img, File: src/libv/img/save.cpp

// hpp
#include <libv/img/save.hpp>
// libv
#include <libv/math/vec.hpp>
// ext
#include <SOIL2/SOIL2.h>
// std
#include <cassert>
#include <vector>


namespace libv {
namespace img {

// -------------------------------------------------------------------------------------------------
//
//#include <gli/convert.hpp>
//#include <gli/generate_mipmaps.hpp>
//#include <gli/save_dds.hpp>
//#include <gli/texture2d.hpp>
//
//enum class dds_format {
//	RGB_DXT1,
//	RGB_DXT1_SRGB,
//	RGBA_DXT1,
//	RGBA_DXT1_SRGB,
//	RGBA_DXT3,
//	RGBA_DXT3_SRGB,
//	RGBA_DXT5,
//	RGBA_DXT5_SRGB,
//};
//
//save_result save_dds(const libv::vec3f* data, std::size_t size_x, std::size_t size_y, dds_format format = dds_format::RGBA_DXT5);
//save_result save_dds(const libv::vec4f* data, std::size_t size_x, std::size_t size_y, dds_format format = dds_format::RGBA_DXT5);
//save_result save_dds(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y, dds_format format = dds_format::RGBA_DXT5);
//save_result save_dds(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y, dds_format format = dds_format::RGBA_DXT5);
//
// -------------------------------------------------------------------------------------------------
//
//template <typename T>
//static gli::texture2d create_gli_texture(const T* data, std::size_t size_x, std::size_t size_y, gli::format format) {
//
//	gli::texture2d gli_texture(format, gli::texture2d::extent_type(size_x, size_y), 1);
//	std::memcpy(gli_texture.data(), data, sizeof(T) * size_x * size_y);
//
//	return gli_texture;
//}
//
//// -------------------------------------------------------------------------------------------------
//
//struct formated_image_data_dds {
//	formated_image_data_view view;
//	std::vector<char> memory;
//};
//
//static save_result aux_save_dds(gli::texture2d gli_texture, dds_format format) {
//	gli::format dst_format = [](dds_format f) {
//		switch (f) {
//			case dds_format::RGB_DXT1: return gli::format::FORMAT_RGB_DXT1_UNORM_BLOCK8;
//			case dds_format::RGB_DXT1_SRGB: return gli::format::FORMAT_RGB_DXT1_SRGB_BLOCK8;
//			case dds_format::RGBA_DXT1: return gli::format::FORMAT_RGBA_DXT1_UNORM_BLOCK8;
//			case dds_format::RGBA_DXT1_SRGB: return gli::format::FORMAT_RGBA_DXT1_SRGB_BLOCK8;
//			case dds_format::RGBA_DXT3: return gli::format::FORMAT_RGBA_DXT3_UNORM_BLOCK16;
//			case dds_format::RGBA_DXT3_SRGB: return gli::format::FORMAT_RGBA_DXT3_SRGB_BLOCK16;
//			case dds_format::RGBA_DXT5: return gli::format::FORMAT_RGBA_DXT5_UNORM_BLOCK16;
//			case dds_format::RGBA_DXT5_SRGB: return gli::format::FORMAT_RGBA_DXT5_SRGB_BLOCK16;
//		}
//
//		assert(false && "Unknown dds format type");
//		return gli::format::FORMAT_RGBA_DXT5_UNORM_BLOCK16;
//	}(format);
//
////	gli::texture2d gli_texture = gli::generate_mipmaps(gli_texture, gli::FILTER_LINEAR);
//	gli::texture2d gli_compressed = gli::convert(gli_texture, dst_format);
//
//	auto result_body = std::make_shared<formated_image_data_dds>();
//
//	bool success = gli::save_dds(gli_compressed, result_body->memory);
//
//	if (!success) {
//		result_body->view.data = reinterpret_cast<const std::byte*>(result_body->memory.data());
//		result_body->view.size = result_body->memory.size();
//
//	} else {
//		result_body->view.data = nullptr;
//		result_body->view.size = 0;
//	}
//
//	auto result_view = std::shared_ptr<const formated_image_data_view>(std::move(result_body), &result_body->view);
//
//	return save_result{std::move(result_view)};
//}
//
// -------------------------------------------------------------------------------------------------
//
//save_result save_dds(const libv::vec3f* data, std::size_t size_x, std::size_t size_y, dds_format format) {
//	gli::format src_format = gli::format::FORMAT_RGB32_SFLOAT_PACK32;
//	return aux_save_dds(create_gli_texture<libv::vec3f>(data, size_x, size_y, src_format), format);
//}
//
//save_result save_dds(const libv::vec4f* data, std::size_t size_x, std::size_t size_y, dds_format format) {
//	gli::format src_format = gli::format::FORMAT_RGBA32_SFLOAT_PACK32;
//	return aux_save_dds(create_gli_texture<libv::vec4f>(data, size_x, size_y, src_format), format);
//}
//
//save_result save_dds(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y, dds_format format) {
//	gli::format src_format = gli::format::FORMAT_RGB8_UINT_PACK8;
//	return aux_save_dds(create_gli_texture<libv::vec3uc>(data, size_x, size_y, src_format), format);
//}
//
//save_result save_dds(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y, dds_format format) {
//	gli::format src_format = gli::format::FORMAT_RGBA8_UINT_PACK8;
//	return aux_save_dds(create_gli_texture<libv::vec4uc>(data, size_x, size_y, src_format), format);
//}

// =================================================================================================

struct formated_image_data_dds {
	formated_image_data_view view;
	std::unique_ptr<unsigned char[]> memory;
};

// -------------------------------------------------------------------------------------------------

template <typename T>
static save_result aux_save(const T* data, std::size_t size_x, std::size_t size_y, std::size_t channels, int format, int quality = 100) {
	const unsigned char* data_ptr;

	// Pre-process data
	using byte = unsigned char;
	std::unique_ptr<byte[]> processed_data;
	if constexpr (std::is_floating_point_v<typename T::value_type>) {
		processed_data.reset(new byte[size_x * size_y * channels]);
		for (std::size_t y = 0; y < size_y; ++y)
			for (std::size_t x = 0; x < size_x; ++x)
				for (std::size_t c = 0; c < channels; ++c) {
					auto v = data[size_x * y + x][c];
					v = v < 0 ? 0 : v;
					v = v > 1 ? 1 : v;
					processed_data[channels * size_x * y + channels * x + c] = static_cast<byte>(v * 255.f);
				}

		data_ptr = static_cast<const unsigned char*>(processed_data.get());
	} else {
		data_ptr = reinterpret_cast<const unsigned char*>(data);
	}

	// Save
	int imageEncodedSize = 0;
	unsigned char* imageEncodedData = SOIL_write_image_to_memory_quality(
			format,
			static_cast<int>(size_x),
			static_cast<int>(size_y),
			static_cast<int>(channels),
			data_ptr,
			quality,
			&imageEncodedSize
	);

	auto result_body = std::make_shared<formated_image_data_dds>();
	result_body->memory = std::unique_ptr<unsigned char[]>(imageEncodedData);

	// Setup view
	if (imageEncodedData == nullptr) {
		result_body->view.data = nullptr;
		result_body->view.size = 0;
	} else {
		result_body->view.data = reinterpret_cast<const std::byte*>(result_body->memory.get());
		result_body->view.size = static_cast<std::size_t>(imageEncodedSize);
	}

	// Aliasing shared_ptr constructor
	auto result_view = std::shared_ptr<const formated_image_data_view>(std::move(result_body), &result_body->view);

	return save_result{std::move(result_view)};
}

// -------------------------------------------------------------------------------------------------

static inline int to_soil_format(save_format f) noexcept {
	switch (f) {
		case save_format::bmp: return SOIL_SAVE_TYPE_BMP;
		case save_format::dds: return SOIL_SAVE_TYPE_DDS;
		case save_format::jpg: return SOIL_SAVE_TYPE_JPG;
		case save_format::png: return SOIL_SAVE_TYPE_PNG;
		case save_format::tga: return SOIL_SAVE_TYPE_TGA;
		case save_format::qoi: return SOIL_SAVE_TYPE_QOI;
	}

	assert(false && "Invalid save format");
	return SOIL_SAVE_TYPE_PNG;
}

save_result save(save_format f, const libv::vec3f* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 4, to_soil_format(f), quality);
}

save_result save(save_format f, const libv::vec4f* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 4, to_soil_format(f), quality);
}

save_result save(save_format f, const libv::vec3uc* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 4, to_soil_format(f), quality);
}

save_result save(save_format f, const libv::vec4uc* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 4, to_soil_format(f), quality);
}

// -------------------------------------------------------------------------------------------------

save_result save_bmp(const libv::vec3f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_BMP);
}

save_result save_bmp(const libv::vec4f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_BMP);
}

save_result save_bmp(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_BMP);
}

save_result save_bmp(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_BMP);
}

// ---

save_result save_dds(const libv::vec3f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_DDS);
}

save_result save_dds(const libv::vec4f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_DDS);
}

save_result save_dds(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_DDS);
}

save_result save_dds(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_DDS);
}

// ---

save_result save_jpg(const libv::vec3f* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_JPG, quality);
}

save_result save_jpg(const libv::vec4f* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_JPG, quality);
}

save_result save_jpg(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_JPG, quality);
}

save_result save_jpg(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y, int quality) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_JPG, quality);
}

// ---

save_result save_png(const libv::vec3f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_PNG);
}

save_result save_png(const libv::vec4f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_PNG);
}

save_result save_png(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_PNG);
}

save_result save_png(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_PNG);
}

// ---

save_result save_tga(const libv::vec3f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_TGA);
}

save_result save_tga(const libv::vec4f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_TGA);
}

save_result save_tga(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_TGA);
}

save_result save_tga(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_TGA);
}

// ---

save_result save_qoi(const libv::vec3f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_QOI);
}

save_result save_qoi(const libv::vec4f* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_QOI);
}

save_result save_qoi(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 3, SOIL_SAVE_TYPE_QOI);
}

save_result save_qoi(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y) {
	return aux_save(data, size_x, size_y, 4, SOIL_SAVE_TYPE_QOI);
}

// -------------------------------------------------------------------------------------------------

} // namespace img
} // namespace libv
