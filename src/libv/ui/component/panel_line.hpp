// Project: libv.ui, File: src/libv/ui/component/panel_line.hpp

#pragma once

// pro
#include <libv/ui/component/base_panel.hpp>
#include <libv/ui/component/detail/component_api.hpp>
#include <libv/ui/property/align.hpp>
#include <libv/ui/property/orientation.hpp>
#include <libv/ui/property/spacing.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelLine : public ComponentAPI<BasePanel, PanelLine, class CorePanelLine, EventHostGeneral> {
public:
	using ComponentAPI::ComponentAPI;
	static constexpr std::string_view component_type = "line";
	static core_ptr create_core(std::string name);

public:
	void align_horizontal(AlignHorizontal value);
	[[nodiscard]] AlignHorizontal align_horizontal() const noexcept;

	void align_vertical(AlignVertical value);
	[[nodiscard]] AlignVertical align_vertical() const noexcept;

	void orientation(Orientation value);
	[[nodiscard]] Orientation orientation() const noexcept;

	void spacing(Spacing value);
	[[nodiscard]] Spacing spacing() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
