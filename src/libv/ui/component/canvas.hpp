// Project: libv.ui, File: src/libv/ui/component/canvas.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/glr/queue_fwd.hpp>
// pro
#include <libv/ui/component.hpp>
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

class CanvasAdaptor : public ComponentHandler<class CoreCanvasAdaptor, EventHostGeneral<CanvasAdaptor>> {
public:
	explicit CanvasAdaptor(std::string name);
	explicit CanvasAdaptor(GenerateName_t = {}, const std::string_view type = "canvas");
	explicit CanvasAdaptor(core_ptr core) noexcept;

public:
	void adopt(Canvas* canvas);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
