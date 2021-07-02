// Project: libv, File: app/space/icons.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/vec.hpp>
#include <libv/ui/chrono.hpp>
// std
#include <vector>


namespace app {

// -------------------------------------------------------------------------------------------------

//struct ScreenPickableType {
//	float radius_direct;
//	float radius_indirect;
//
//	constexpr inline ScreenPickable(float radiusDirect, float radiusIndirect) noexcept :
//		radius_direct(radiusDirect),
//		radius_indirect(radiusIndirect) {}
//};

//struct Fleet : ScreenPickable {
struct Fleet {
//	ScreenPickableType* screen_pick_type;

//	enum class CommandType {
//		movement,
//		attack,
////		follow,
////		merge,
////		block,
////		land,
////		...,
//	};
//
//	struct Command {
//		CommandType type;
//		libv::vec3f target;
////		int32_t target;
//	};

public:
	libv::vec3f position;
	libv::vec3f target;
//	libv::vec3f movement;
//	CommandArrow command_arrow; // <<< Yes, but no, maybe-, Think about it how should ownership and references fork regarding renderers
//	std::vector<Command> commands;

public:
	explicit Fleet(libv::vec3f position) :
//		ScreenPickable(50.f, 100.f),
			position(position),
			target(position) {}

public:
//	void queue_command(CommandType type, libv::vec3f target) {
//		commands.emplace_back(type, target);
////		command_arrow.add(target, color);
//	}
//
//	void queue_command(CommandType type, int32_t target) {
//
//	}

	void update(libv::ui::time_duration delta_time) {
		const auto dt = static_cast<float>(delta_time.count());
		const auto [len, dir] = (target - position).length_and_dir();

		if (len < dt)
			position = target;
		else
			position = position + dir * dt;
	}
};

//struct Map {
//	FleetRender fleet_render;
//	std::vector<Fleet> fleets;
//
//	void render_fleets(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) {
//		for (const auto& fleet : fleets) {
//			const auto m_guard = gl.model.push_guard();
//			gl.model.translate(fleet.position);
//			fleet_render.render(gl, uniform_stream);
//		}
//	}
//
//	void render_fleet_arrows(libv::glr::Queue& gl, libv::vec2f canvas_size, libv::glr::UniformBuffer& uniform_stream) {
//		for (const auto& fleet : fleets)
//			fleet.command_arrow.render(gl, canvas_size, uniform_stream);
//	}
//};

// -------------------------------------------------------------------------------------------------

struct SpaceState {
//	app::CameraPlayer& camera;
//
//	float angle = 0.0f;
//	float time = 0.0f;
//	float test_sin_time = 0.0f;

	std::vector<Fleet> fleets;

public:
//	SpaceState(app::CameraPlayer& camera) :
//		camera(camera) {
//	}

	void update(libv::ui::time_duration delta_time) {
		for (auto& fleet : fleets)
			fleet.update(delta_time);
	}


};

// -------------------------------------------------------------------------------------------------

} // namespace app
