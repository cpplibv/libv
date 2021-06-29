// Project: libv, File: app/space/icons.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/math/vec.hpp>
// std
//#include <cstdint>
//#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

//struct SpaceState {
//	app::CameraPlayer& camera;
//
//	float angle = 0.0f;
//	float time = 0.0f;
//	float test_sin_time = 0.0f;
//
//	std::vector<Fleet> fleets;
//
//public:
//	SpaceState(app::CameraPlayer& camera) :
//			camera(camera) {
//	}
//
//	void update(libv::ui::time_duration delta_time) {
//		const auto dtf = static_cast<float>(delta_time.count());
//		angle = std::fmod(angle + 5.0f * dtf, 360.0f);
//		time += dtf;
//		global_time += dtf;
//
//		// TODO P2: Value tracking UI component for debugging
////		libv::ui::value_tracker tracker(600 /*sample*/, 0.15, 0.85);
////		value_tracker(160);
////		value_tracker.pause();
////		value_tracker.resume();
////		value_tracker("camera.orbit_point", camera.orbit_point());
////		value_tracker("camera.orbit_distance", camera.orbit_distance());
////		value_tracker("camera.rotations", camera.rotations());
////		value_tracker.differential("camera.orbit_point", camera.orbit_point());
////		value_tracker.differential_focused("camera.orbit_point", camera.orbit_point(), 0.15, 0.85);
////		value_tracker.differential_focused_timed("camera.orbit_point", camera.orbit_point(), 0.15, 0.85);
//
//		if (global_test_mode != 0) {
//			test_sin_time += dtf;
//			auto t = (std::sin(test_sin_time / 10.f) * 0.5f + 0.5f);
//			if (global_test_mode == 1) {
//				camera.pitch(-t * libv::pi_f * 0.5f);
//			} else if (global_test_mode == 2) {
//				t = t > 0.5f ? 1.f - t : t;
//				camera.pitch(-t * libv::pi_f * 0.5f);
//			} else if (global_test_mode == 3) {
//				const float part = 4;
//				auto t = (std::sin(test_sin_time / 10.f * part) * 0.5f + 0.5f);
//				t = t > 0.5f ? 1.f - t : t;
//				camera.pitch(-t * libv::pi_f * 0.5f / part);
//			}
//		}
//
//		for (auto& fleet : fleets)
//			fleet.update(delta_time);
//	}
//};

// -------------------------------------------------------------------------------------------------

} // namespace app
