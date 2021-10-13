// Project: libv, File: app/space/canvas.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/ui/component/canvas.hpp>
// pro
#include <space/camera.hpp>
#include <space/renderer.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------
enum class StyleFlag : uint8_t{
	WORLD = 1 << 0,
	FILL = 1 << 1,
	WIREFRAME = 1 << 2,
	TWO_SIDED = 1 << 3,

	DEFAULT = FILL | WORLD
};

[[nodiscard]] constexpr inline StyleFlag operator|(StyleFlag lhs, StyleFlag rhs) noexcept {
	return static_cast<StyleFlag>(libv::to_underlying(lhs) | libv::to_underlying(rhs));
}

struct SpaceCanvas : libv::ui::CanvasBase {
	struct Line {
		libv::vec3f a;
		libv::vec3f b;
	};
public:
	bool main_canvas;
	GameSession& game_session;
	Universe& universe;
	Playout& playout;

//	CameraPlayer& camera;
	CameraPlayer camera;
	CameraPlayer::screen_picker screen_picker;

	float angle = 0.0f;
	float time = 0.0f;
	float test_sin_time = 0.0f;
	Line start_line;

//	std::vector<Line> debug_lines;

	Renderer& renderer;


public:
	SpaceCanvas(Renderer& renderer, GameSession& game_session, bool main_canvas);

	void add_debug_point(libv::vec3f a, float size, libv::vec4f color, StyleFlag style = StyleFlag::DEFAULT);
	void add_debug_line(libv::vec3f a, libv::vec3f b, libv::vec4f color, StyleFlag style = StyleFlag::DEFAULT);
	void add_debug_triangle(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec4f color, StyleFlag style = StyleFlag::DEFAULT);
	void add_debug_quad(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec4f color, StyleFlag style = StyleFlag::DEFAULT);
	void add_debug_sphere(libv::vec3f a, float radius, libv::vec4f color, StyleFlag style = StyleFlag::DEFAULT);
	void add_debug_frustum(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec3f e, libv::vec4f color_wire, libv::vec4f color_sides, StyleFlag style = StyleFlag::DEFAULT);
	void clear_debug_shapes();
	virtual void update(libv::ui::time_duration delta_time) override;
	virtual void render(libv::glr::Queue& gl) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
