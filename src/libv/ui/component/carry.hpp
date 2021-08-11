// Project: libv.ui, File: src/libv/ui/component/carry.hpp, Author: Császár Mátyás [Vader]

#pragma once

// fwd
#include <libv/ui/component/fwd.hpp>
// libv
#include <libv/glr/queue_fwd.hpp>
#include <libv/utility/observer_ref.hpp>
// std
#include <memory>
// pro
#include <libv/ui/chrono.hpp>
#include <libv/ui/component/detail/component_api.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class CoreHUDAdaptor;

class HUDBase {
	friend CoreHUDAdaptor;

public:
	[[nodiscard]] libv::vec2f calculate_local_mouse_coord() const noexcept;

protected:
	virtual void create(libv::glr::Queue& glr) { (void) glr; }
	virtual void update(time_duration delta_time) = 0;
	virtual void render(libv::glr::Queue& glr) = 0;
	virtual void destroy(libv::glr::Queue& glr) { (void) glr; }

public:
	virtual ~HUDBase() = default;
};

// -------------------------------------------------------------------------------------------------

class HUDAdaptor : public ComponentAPI<Component, HUDAdaptor, CoreHUDAdaptor, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;

public:
	static constexpr std::string_view component_type = "canvas";
	static core_ptr create_core(std::string name, std::unique_ptr<HUDBase>&& canvas_object);

protected:
	HUDBase& object_base() noexcept;
	const HUDBase& object_base() const noexcept;
};

template <typename T>
class HUDAdaptorT : public ComponentAPI<HUDAdaptor, HUDAdaptorT<T>, CoreHUDAdaptor, EventHostGeneral> {
	using BaseAPI = ComponentAPI<HUDAdaptor, HUDAdaptorT<T>, CoreHUDAdaptor, EventHostGeneral>;

public:
	explicit HUDAdaptorT(core_ptr ptr) :
		BaseAPI(ptr) {
	}

	template <typename... Args>
	explicit HUDAdaptorT(std::string name, Args&&... args) :
		BaseAPI(std::move(name), std::make_unique<T>(std::forward<Args>(args)...)) {
	}

	template <typename... Args>
	explicit HUDAdaptorT(GenerateName_t name = {}, Args&&... args) :
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
// -------------------------------------------------------------------------------------------------

struct HUDMenuBar : public HUD<libv::ui::PanelLine> {

};

void foo() {

}

// -------------------------------------------------------------------------------------------------

// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================


class Storage : public ComponentAPI<Component, Storage, class CoreStorage, EventHostSubmittable> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "button";
	static core_ptr create_core(std::string name);

public:
	void color(Color value);
	[[nodiscard]] const Color& color() const noexcept;

	void image(Texture2D_view value);
	[[nodiscard]] const Texture2D_view& image() const noexcept;

	void shader(ShaderImage_view value);
	[[nodiscard]] const ShaderImage_view& shader() const noexcept;

public:
	void align_horizontal(AlignHorizontal value);
	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;

	void align_vertical(AlignVertical value);
	[[nodiscard]] AlignVertical align_vertical() const noexcept;

	void font(Font2D_view value);
	[[nodiscard]] const Font2D_view& font() const noexcept;

	void font_size(FontSize value);
	[[nodiscard]] FontSize font_size() const noexcept;

	void font_color(Color value);
	[[nodiscard]] const Color& font_color() const noexcept;

public:
	void text(std::string value);
	[[nodiscard]] const std::string& text() const noexcept;
};


// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
