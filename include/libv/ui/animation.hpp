// File:   animation.hpp Author: Vader Created on 15 March 2019, 00:59

#pragma once


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

//template <typename T>
//struct KeyFrame {
//	T value;
//	time_duration key;
//	libv::vec4f timecurve = {0, 0, 1, 1}; /// cubic-bezier
//};
//
//template <typename T>
//struct Timeline {
//	std::vector<KeyFrame<T>> keys;
//};
//
//struct Animation {
//	Timeline<int>* timeline;
//	time_point start;
//	float playback_rate = 1.0f;
//	float num_iteration = 1;
//	// How does pause / resume works?
//	// How does alternate works?
//
//	//	enum class Mode { forward, backward, alternate, alternate_mirror };
//	//	Mode mode = Mode::forward;
//	//	bool alternate;
//
////	ECS_ID id
////	time_point start;
////	time_point pause;
////	time_duration duration;
////	time_duration delay;
////	std::optional<time_point> scheduled_pause;
////	IterationCount::Value iterations;
////	FillMode::Type fill_mode;
////	TimingFunction timing;
////	PlayState play_state
////	float iterations_passed = 0.f;
////	float playback_rate = 1.0f;
//};
//
//struct AnimationHandler {
//	void play();
//	void toggle();
//	void pause();
//	void stop();
//};
//
//////    linear:     libv::vec3f(0, 0, 1, 1)
//////    ease:       libv::vec3f(0.25, 0.1, 0.25, 1)
//////    ease-in:    libv::vec3f(0.42, 0, 1, 1)
//////    ease-inout: libv::vec3f(0.42, 0, 0.58, 1)
//////    ease-out:   libv::vec3f(0, 0, 0.58, 1)
//////    custom:     libv::vec3f(x0, y0, x1, y1)
////// http://cubic-bezier.com
////// https://www.davrous.com/2011/12/05/introduction-to-css3-animations/
//
//struct AnimationSystem {
//	std::vector<Timeline<float>> opacity;
//	std::vector<Timeline<Color>> color;
//	std::vector<Timeline<Size>> size;
//	std::vector<Timeline<libv::vec3f>> render_position;
//	std::vector<Timeline<libv::vec3f>> render_size;
//
//	std::vector<Animation> active_animations;
//	std::vector<Animation> inactive_animations;
//
////	AnimationEvents;
//
//	template <typename T> // Not that easy
//	void update() {
//
//		const auto now = time_point{std::chrono::steady_clock::now()};
//
//		for (const auto& animation : active_animations) {
//
////////			libv::lower_bound(timeline.keys, now)
//////			timeline.keys[];
////			const auto& time = animation.timeline->keys[0].time;
////			if constexpr (interpolatable) {
//				const auto& timecurve = animation.timeline->keys[0].timecurve;
//				const auto& a = animation.timeline->keys[0].value;
//				const auto& b = animation.timeline->keys[0 + 1].value;
//
//				const auto t = (now - animation.start).count();
//				const auto r = libv::math::bezierCurvePoint(timecurve.x, timecurve.y, timecurve.z, timecurve.w, t);
//
//				const auto value = a * r + (1 - r) * b;
////			}
//		}
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
