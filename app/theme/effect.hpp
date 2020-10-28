// Project: libv, File: app/theme/effect.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/mix.hpp>
#include <libv/math/saturate.hpp>
#include <libv/math/vec.hpp>
#include <libv/mt/thread_bulk.hpp>
// std
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
// pro
#include <theme/image.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

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

// =================================================================================================

struct EffectEngineContext {
	libv::mt::thread_bulk& threads;
	Image& image;
};

// -------------------------------------------------------------------------------------------------

struct BasePixelEffect {
	virtual void execute(EffectEngineContext& context) = 0;
	virtual ~BasePixelEffect() = default;
};

template <typename CRTP>
struct PixelEffect : BasePixelEffect {
public:
	virtual void execute(EffectEngineContext& context) override {
		std::atomic_size_t next_row = 0;
		const auto num_row = context.image.size().y;

		context.threads.execute_and_wait([&] {
			size_t y = next_row++;
			if (y >= num_row)
				return false;

			for (size_t x = 0; x < context.image.size().x; ++x)
				static_cast<CRTP&>(*this).apply(context.image, x, y);

			return true;
		});
	}
};

// -------------------------------------------------------------------------------------------------

struct EffectRoundedBox : PixelEffect<EffectRoundedBox> {
	libv::vec2f pos;
	libv::vec2f size;

	float corner_size;
	float corner_sharpness;

public:
	EffectRoundedBox(libv::vec2f pos, libv::vec2f size, float corner_size, float corner_sharpness) :
			pos(pos),
			size(size),
			corner_size(corner_size),
			corner_sharpness(corner_sharpness) {}

public:
	inline void apply(Image& image, size_t x, size_t y) const noexcept {
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

struct EffectGlow : PixelEffect<EffectGlow> {
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

public:
	inline void apply(Image& image, size_t x, size_t y) const noexcept {
		const auto signed_dist = image.sdistance(x, y);
		auto& output = image.color(x, y);

		const auto box = libv::mix(0.f, 1.f, -signed_dist);
		const auto mask = libv::saturate(box);

		float shape;

		if (inward) {
			shape = (signed_dist + 1 + size) / size;
			shape = libv::saturate(libv::saturate(shape) - (1 - mask));
		} else {
			shape = (-signed_dist + size) / size;
			shape = libv::saturate(libv::saturate(shape) - mask);
		}

		shape = std::pow(shape, falloff);
		output = blend(color * libv::vec4f(1, 1, 1, shape), output);
	}
};

// -------------------------------------------------------------------------------------------------

class EffectApplyEngine {
	libv::mt::thread_bulk threads;

public:
	explicit EffectApplyEngine(size_t n) : threads(n) {}

public:
	void process(Image& image, const std::vector<std::unique_ptr<BasePixelEffect>>& effects) {
		EffectEngineContext context(threads, image);
		for (const auto& effect : effects)
			effect->execute(context);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
