// Project: libv.ui, File: src/libv/ui/component/panel_grid.hpp, Author: Császár Mátyás [Vader]

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/column_count.hpp>
#include <libv/ui/property/orientation2.hpp>
#include <libv/ui/property/spacing.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelGrid : public ComponentHandler<class CorePanelGrid, EventHostGeneral<PanelGrid>> {
public:
	explicit PanelGrid(std::string name);
	explicit PanelGrid(GenerateName_t = {}, const std::string_view type = "grid");
	explicit PanelGrid(core_ptr core) noexcept;

public:
	void column_count(ColumnCount value);
	[[nodiscard]] ColumnCount column_count() const noexcept;

	void orientation2(Orientation2 value);
	[[nodiscard]] Orientation2 orientation2() const noexcept;

	void spacing2(Spacing2 value);
	[[nodiscard]] Spacing2 spacing2() const noexcept;

public:
	void add(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
