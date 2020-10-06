// Project: libv.ui, File: app/gen_ui_theme/effect.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/thread/thread_bulk.hpp>
// std
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
// pro
#include <gen_ui_theme/image.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline T saturate(T value) noexcept {
	if (value < 0)
		return 0;
	else if (value > 1)
		return 1;
	else
		return value;
}

template <typename T>
[[nodiscard]] constexpr inline T mix(T v0, T v1, T t) noexcept {
	return (T{1} - t) * v0 + t * v1;
}

[[nodiscard]] constexpr inline libv::vec4f blend(libv::vec4f src, libv::vec4f dst) noexcept {
	const auto src_rgb = xyz(src);
	const auto src_a = src.w;
	const auto dst_rgb = xyz(dst);
	const auto dst_a = dst.w;

	const auto out_a = src_a + dst_a * (1 - src_a);
	const auto out_rgb = (src_rgb * src_a + dst_rgb * dst_a * (1 - src_a)) / out_a;

	if (abs(out_a) < 0.00001f)
		return src;
	else
		return libv::vec4f{out_rgb, out_a};
}

[[nodiscard]] constexpr inline libv::vec4f add_layer(libv::vec4f output, libv::vec4f color, float shape) noexcept {
	return blend(color * libv::vec4f(1, 1, 1, shape), output);
}

// -------------------------------------------------------------------------------------------------

struct Effect {
//	const bool allow_multithread = false;
	const bool allow_multithread = true;

	virtual void apply(Image& image, size_t x, size_t y) const noexcept = 0;
	virtual ~Effect() = default;
};

struct EffectGlow : Effect {
	float size;    /// px
	float falloff; /// distance exponent
	bool inward;
	libv::vec4f color;

public:
	explicit inline EffectGlow(float size, float falloff, libv::vec4f color) noexcept :
		size(std::abs(size)),
		falloff(falloff),
		inward(size < 0.0f),
		color(color) {}

private:
	virtual void apply(Image& image, size_t x, size_t y) const noexcept override {
		const auto signed_dist = image.sdistance(x, y);
		auto& output = image.color(x, y);

		const auto box = mix(0.f, 1.f, -signed_dist);
		const auto mask = saturate(box);

		float shape;
		
		if (inward) {
			shape = (signed_dist + 1 + size) / size;
			shape = saturate(saturate(shape) - (1 - mask));
		} else {
			shape = (-signed_dist + size) / size;
			shape = saturate(saturate(shape) - mask);
		}

		shape = std::pow(shape, falloff);
		output = add_layer(output, color, shape);
	}
};

struct EffectRoundedBox : Effect {
	libv::vec2f pos;
	libv::vec2f size;

	float corner_size;
	float corner_sharpness;

public:
	EffectRoundedBox(libv::vec2f pos, libv::vec2f size, float cornerSize, float cornerSharpness) :
		pos(pos),
		size(size),
		corner_size(cornerSize),
		corner_sharpness(cornerSharpness) {}

private:
	virtual void apply(Image& image, size_t x, size_t y) const noexcept override {
		const auto fx = static_cast<float>(x);
		const auto fy = static_cast<float>(y);

		const auto l_pos = ((libv::vec2f{fx, fy} - pos) - size * 0.5f);
		const auto l_size = size * 0.5f - corner_size;

		// Source https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm
		const auto p = l_pos;
		const auto b = l_size;
		const auto r = corner_size;

		const auto q = abs(p) - b;
		const auto sd = libv::vec::max(q, 0.0f).length() + std::min(std::max(q.x, q.y), 0.0f) - r;

		image.sdistance(x, y) = sd;
	}
};

// -------------------------------------------------------------------------------------------------

class EffectApplyEngine {
	libv::mt::thread_bulk threads;
	std::atomic_size_t next_row = 0;

public:
	explicit EffectApplyEngine(size_t n) : threads(n) {}

private:
	bool work(Image& image, Effect& effect) {
		size_t y = next_row++;
		if (y >= image.size().y)
			return false;

		for (size_t x = 0; x < image.size().x; ++x) {
			effect.apply(image, x, y);
		}

		return true;
	}

public:
	void process(Image& image, const std::vector<std::unique_ptr<Effect>>& effects) {
		for (const auto& effect : effects) {
			next_row = 0;
			if (effect->allow_multithread)
				threads.execute_and_wait([this, &image, &effect] { return work(image, *effect); });
			else
				while (work(image, *effect));
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
