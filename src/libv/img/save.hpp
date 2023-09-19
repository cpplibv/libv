// Project: libv.img, File: src/libv/img/save.hpp

#pragma once

// libv
#include <libv/math/vec.hpp>
// pro
#include <libv/img/save_result.hpp>


namespace libv {
namespace img {

// -------------------------------------------------------------------------------------------------

enum class save_format {
	bmp, /// BMP - Backend note: RGB, RGBX (where X is magenta scales) [uncompressed]
	dds, /// DDS - Backend note: RGB as DXT1, RGBA as DXT5
	jpg, /// JPG - Backend note: R, RGB, RGBX (where X is unused)
	png, /// PNG
	tga, /// TGA - Backend note: Greyscale, RGB, RGBA [uncompressed]
	qoi, /// QOI
};

// -------------------------------------------------------------------------------------------------

save_result save(save_format f, const libv::vec3f* data, std::size_t size_x, std::size_t size_y, int quality = 80);
save_result save(save_format f, const libv::vec4f* data, std::size_t size_x, std::size_t size_y, int quality = 80);
save_result save(save_format f, const libv::vec3uc* data, std::size_t size_x, std::size_t size_y, int quality = 80);
save_result save(save_format f, const libv::vec4uc* data, std::size_t size_x, std::size_t size_y, int quality = 80);

// -------------------------------------------------------------------------------------------------

save_result save_bmp(const libv::vec3f* data, std::size_t size_x, std::size_t size_y);
save_result save_bmp(const libv::vec4f* data, std::size_t size_x, std::size_t size_y);
save_result save_bmp(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y);
save_result save_bmp(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y);

save_result save_dds(const libv::vec3f* data, std::size_t size_x, std::size_t size_y); /// DXT1
save_result save_dds(const libv::vec4f* data, std::size_t size_x, std::size_t size_y); /// DXT5
save_result save_dds(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y); /// DXT1
save_result save_dds(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y); /// DXT5

save_result save_jpg(const libv::vec3f* data, std::size_t size_x, std::size_t size_y, int quality = 80);
save_result save_jpg(const libv::vec4f* data, std::size_t size_x, std::size_t size_y, int quality = 80);
save_result save_jpg(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y, int quality = 80);
save_result save_jpg(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y, int quality = 80);

save_result save_png(const libv::vec3f* data, std::size_t size_x, std::size_t size_y);
save_result save_png(const libv::vec4f* data, std::size_t size_x, std::size_t size_y);
save_result save_png(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y);
save_result save_png(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y);

save_result save_tga(const libv::vec3f* data, std::size_t size_x, std::size_t size_y);
save_result save_tga(const libv::vec4f* data, std::size_t size_x, std::size_t size_y);
save_result save_tga(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y);
save_result save_tga(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y);

save_result save_qoi(const libv::vec3f* data, std::size_t size_x, std::size_t size_y);
save_result save_qoi(const libv::vec4f* data, std::size_t size_x, std::size_t size_y);
save_result save_qoi(const libv::vec3uc* data, std::size_t size_x, std::size_t size_y);
save_result save_qoi(const libv::vec4uc* data, std::size_t size_x, std::size_t size_y);

// -------------------------------------------------------------------------------------------------

} // namespace img
} // namespace libv
