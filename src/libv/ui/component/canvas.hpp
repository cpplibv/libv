// Project: libv.ui, File: src/libv/ui/component/canvas.hpp

#pragma once

// fwd
#include <libv/ui/component/fwd.hpp>
// libv
#include <libv/glr/fwd.hpp>
#include <libv/utility/observer_ref.hpp>
// std
#include <memory>
// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/detail/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreCanvasAdaptor;

class CanvasBase {
	friend CoreCanvasAdaptor;

private:
	libv::observer_ref<CoreComponent> core;

protected:
	libv::vec2f canvas_position;
	libv::vec2f canvas_size;

public:
	[[nodiscard]] libv::vec2f calculate_local_mouse_coord() const noexcept;

public:
	void focus();

protected:
	virtual void attach() { }
	virtual void create(libv::glr::Queue& glr) { (void) glr; }
	virtual void update(time_duration delta_time) { (void) delta_time; }
	virtual void render(libv::glr::Queue& glr) = 0;
	virtual void destroy(libv::glr::Queue& glr) { (void) glr; }

public:
	virtual ~CanvasBase() = default;
};

// -------------------------------------------------------------------------------------------------

class CanvasAdaptor : public ComponentAPI<Component, CanvasAdaptor, CoreCanvasAdaptor, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;

public:
	static constexpr std::string_view component_type = "canvas";
	static core_ptr create_core(std::string name, std::unique_ptr<CanvasBase>&& canvas_object);

protected:
	[[nodiscard]] CanvasBase& object_base() noexcept;
	[[nodiscard]] const CanvasBase& object_base() const noexcept;
};

template <typename T>
class CanvasAdaptorT : public ComponentAPI<CanvasAdaptor, CanvasAdaptorT<T>, CoreCanvasAdaptor, EventHostGeneral> {
	using BaseAPI = ComponentAPI<CanvasAdaptor, CanvasAdaptorT<T>, CoreCanvasAdaptor, EventHostGeneral>;

public:
	explicit CanvasAdaptorT(core_ptr ptr) :
		BaseAPI(ptr) {
	}

	template <typename... Args>
	explicit CanvasAdaptorT(std::string name, Args&&... args) :
		BaseAPI(std::move(name), std::make_unique<T>(std::forward<Args>(args)...)) {
	}

	template <typename... Args>
	explicit CanvasAdaptorT(GenerateName_t name = {}, Args&&... args) :
		BaseAPI(std::move(name), std::make_unique<T>(std::forward<Args>(args)...)) {
	}

public:
	inline T& object() noexcept {
		return static_cast<T&>(this->object_base());
	}

	inline const T& object() const noexcept {
		return static_cast<const T&>(this->object_base());
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
