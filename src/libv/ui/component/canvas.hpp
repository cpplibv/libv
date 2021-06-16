// Project: libv.ui, File: src/libv/ui/component/canvas.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/glr/queue_fwd.hpp>
// pro
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/chrono.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct Canvas {
	libv::vec2f canvas_size;

public:
	virtual void create(libv::glr::Queue& glr) { (void) glr; }
	virtual void update(time_duration delta_time) = 0;
	virtual void render(libv::glr::Queue& glr) = 0;
	virtual void destroy(libv::glr::Queue& glr) { (void) glr; }
	virtual ~Canvas() = default;
};

class CanvasAdaptor : public ComponentAPI<Component, CanvasAdaptor, class CoreCanvasAdaptor, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "canvas";
	static core_ptr create_core(std::string name);

public:
	void adopt(Canvas* canvas);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
