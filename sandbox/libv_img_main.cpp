// Project: libv, File: sandbox/libv_immer_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/img/save.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <iostream>
#include <string>


// -------------------------------------------------------------------------------------------------

int main() {
	std::vector<libv::vec3f> v3f;
	std::vector<libv::vec4f> v4f;
	std::vector<libv::vec3uc> v3uc;
	std::vector<libv::vec4uc> v4uc;

	static constexpr size_t size_x = 256;
	static constexpr size_t size_y = 256;
	static constexpr size_t size_fx = static_cast<float>(size_x);
	static constexpr size_t size_fy = static_cast<float>(size_y);

	const auto add = [&](float x, float y, float z, float w) {
		const auto xi = static_cast<uint8_t>(x * 255.f);
		const auto yi = static_cast<uint8_t>(y * 255.f);
		const auto zi = static_cast<uint8_t>(z * 255.f);
		const auto wi = static_cast<uint8_t>(w * 255.f);

		v3f.emplace_back(x, y, z);
		v4f.emplace_back(x, y, z, w);
		v3uc.emplace_back(xi, yi, zi);
		v4uc.emplace_back(xi, yi, zi, wi);
	};

	const auto save = [&](const auto fn, const auto& data, const auto name) {
		auto sr = fn(data.data(), size_x, size_y);
		if (!sr)
			std::cout << "Failed to save: " << name << std::endl;
		else
			libv::write_file_or_throw(name, sr.span());
	};

	const auto save_all = [&](const auto fn, std::string name) {
		save(fn, v3f,  "tmp/" + name + "_v3f." + name);
		save(fn, v4f,  "tmp/" + name + "_v4f." + name);
		save(fn, v3uc, "tmp/" + name + "_v3uc." + name);
		save(fn, v4uc, "tmp/" + name + "_v4uc." + name);
	};

	for (size_t x = 0; x < size_x; ++x) {
		for (size_t y = 0; y < size_y; ++y) {
			const auto fx = static_cast<float>(x);
			const auto fy = static_cast<float>(y);

			const auto r = fx / size_fx;
			const auto g = fy / size_fy;
			const auto d = std::min(std::sqrt(r * r + g * g), 1.0f);

			add(r, g, 0, d);
		}
	}

	try {
		save_all([](const auto&... a) { return libv::img::save_bmp(a...); }, "bmp");
		save_all([](const auto&... a) { return libv::img::save_dds(a...); }, "dds");
		save_all([](const auto&... a) { return libv::img::save_jpg(a...); }, "jpg");
		save_all([](const auto&... a) { return libv::img::save_png(a...); }, "png");
		save_all([](const auto&... a) { return libv::img::save_tga(a...); }, "tga");
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
