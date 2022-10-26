// Project: libv.ui, File: src/libv/ui/component/base_panel.hpp

#pragma once

// libv
#include <libv/meta/force_inline.hpp>
// pro
#include <libv/ui/component_system/component_api.hpp>
#include <libv/ui/property/background.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class BasePanel : public ComponentAPI<Component, BasePanel, class CoreBasePanel, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "base-panel";
	[[nodiscard]] static core_ptr create_core(std::string name);
	[[nodiscard]] static bool castable(core_ptr) noexcept;

public:
	void background(Background value);
	[[nodiscard]] const Background& background() const noexcept;

public:
	void add(Component component);
	void add_front(Component component);
	void remove(Component& component);
	void remove(std::string_view component_name);
	void clear();

public:
	template <typename ComponentT>
	LIBV_FORCE_INLINE ComponentT add() {
		auto comp = ComponentT();
		add(comp);
		return comp;
	}
	template <typename ComponentT>
	LIBV_FORCE_INLINE ComponentT add_n(std::string name) {
		auto comp = ComponentT::n(std::move(name));
		add(comp);
		return comp;
	}
	template <typename ComponentT>
	LIBV_FORCE_INLINE ComponentT add_s(std::string_view style) {
		auto comp = ComponentT::s(style);
		add(comp);
		return comp;
	}
	template <typename ComponentT, typename... Args>
	LIBV_FORCE_INLINE ComponentT add_a(Args&&... args) {
		auto comp = ComponentT::a(std::forward<Args>(args)...);
		add(comp);
		return comp;
	}
	template <typename ComponentT>
	LIBV_FORCE_INLINE ComponentT add_ns(std::string name, std::string_view style) {
		auto comp = ComponentT::ns(std::move(name), style);
		add(comp);
		return comp;
	}
	template <typename ComponentT, typename... Args>
	LIBV_FORCE_INLINE ComponentT add_na(std::string name, Args&&... args) {
		auto comp = ComponentT::na(std::move(name), std::forward<Args>(args)...);
		add(comp);
		return comp;
	}
	template <typename ComponentT, typename... Args>
	LIBV_FORCE_INLINE ComponentT add_sa(std::string_view style, Args&&... args) {
		auto comp = ComponentT::sa(style, std::forward<Args>(args)...);
		add(comp);
		return comp;
	}
	template <typename ComponentT, typename... Args>
	LIBV_FORCE_INLINE ComponentT add_nsa(std::string name, std::string_view style, Args&&... args) {
		auto comp = ComponentT::nsa(std::move(name), style, std::forward<Args>(args)...);
		add(comp);
		return comp;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
