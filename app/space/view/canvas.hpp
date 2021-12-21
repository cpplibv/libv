// Project: libv, File: app/space/view/canvas.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/rev/post_processing.hpp>
#include <libv/rev/render_target.hpp>
#include <libv/ui/component/canvas.hpp>
// pro
#include <space/view/camera.hpp>
#include <space/renderer.hpp>


namespace space {

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

// -------------------------------------------------------------------------------------------------

struct SpaceCanvas : libv::ui::CanvasBase {
public:
	struct Line {
		// TODO P5: Move into a generalized geometry primitive neighborhood
		libv::vec3f a;
		libv::vec3f b;
	};

public:
	bool main_canvas;
	GameSession& game_session;
	Universe& universe;
	std::shared_ptr<Faction> controlledFaction;
	Playout& playout;

	CameraPlayer camera;
	CameraPlayer::screen_picker screen_picker;

private:
	float angle = 0.0f;
	float time = 0.0f;
	float test_sin_time = 0.0f;

public:
	enum class SelectionMode {
		none,
		d25,
		d3,
	};
	SelectionMode selectionMode = SelectionMode::none;
	libv::vec3f selectionStartGridPoint;
	libv::vec3f selectionStartDir;

private:
	libv::rev::RenderTarget renderTarget;
	libv::rev::PostProcessing postProcessing;
	Renderer& renderer;

public:
	SpaceCanvas(Renderer& renderer, GameSession& game_session, bool main_canvas);

	void add_debug_point(libv::vec3f a, libv::vec4f color, StyleFlag mode = StyleFlag::DEFAULT);
	void add_debug_line(libv::vec3f a, libv::vec3f b, libv::vec4f color, StyleFlag mode = StyleFlag::DEFAULT);
	void add_debug_triangle(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec4f color, StyleFlag mode = StyleFlag::DEFAULT);
	void add_debug_quad(libv::vec3f a, libv::vec3f b, libv::vec3f c, libv::vec3f d, libv::vec4f color, StyleFlag mode = StyleFlag::DEFAULT);
	void add_debug_circle(libv::vec3f center, float radius, libv::vec3f normal, libv::vec4f color, StyleFlag mode = StyleFlag::DEFAULT);
	void add_debug_sphere(libv::vec3f center, float radius, libv::vec4f color, int ring_count, int segment_count, StyleFlag mode = StyleFlag::DEFAULT);
	void add_debug_frustum(
			libv::vec3f nbl, libv::vec3f nbr, libv::vec3f ntr, libv::vec3f ntl,
			libv::vec3f fbl, libv::vec3f fbr, libv::vec3f ftr, libv::vec3f ftl,
			libv::vec4f color_wire, libv::vec4f color_sides, StyleFlag mode = StyleFlag::DEFAULT);
	void clear_debug_shapes();

private:
	virtual void attach() override;
	virtual void update(libv::ui::time_duration delta_time) override;
	virtual void render(libv::glr::Queue& glr) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace space
