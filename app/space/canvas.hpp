// Project: libv, File: app/space/render.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
//#include <libv/ctrl/fwd.hpp>
#include <libv/ui/component/canvas.hpp>
// pro
#include <space/camera.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

struct SpaceCanvas : libv::ui::CanvasBase {
public:
	bool main_canvas;
	GameSession& game_session;
	Universe& universe;
	Playout& playout;
	CameraPlayer& camera;
	CameraPlayer::screen_picker screen_picker;

	float angle = 0.0f;
	float time = 0.0f;
	float test_sin_time = 0.0f;

	Renderer& renderer;

public:
	SpaceCanvas(Renderer& renderer, GameSession& game_session, Universe& universe, Playout& playout, CameraPlayer& camera, bool main_canvas);

	virtual void update(libv::ui::time_duration delta_time) override;
	virtual void render(libv::glr::Queue& gl) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace app
