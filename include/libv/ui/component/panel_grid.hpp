// File: panel.hpp - Created on 2017.10.22. 06:50 - Author: Vader

#pragma once

// pro
#include <libv/ui/component.hpp>
#include <libv/ui/property/column_count.hpp>
#include <libv/ui/property/orientation2.hpp>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class PanelGrid : public ComponenetHandler<class CorePanelGrid, EventHostGeneral<PanelGrid>> {
public:
	explicit PanelGrid(std::string name);
	explicit PanelGrid(GenerateName_t = {}, const std::string_view type = "grid");
	explicit PanelGrid(core_ptr core) noexcept;

public:
	void column_count(ColumnCount value);
	[[nodiscard]] ColumnCount column_count() const noexcept;

	void orientation2(Orientation2 value);
	[[nodiscard]] Orientation2 orientation2() const noexcept;

public:
	void add(Component component);
	void remove(Component& component);
	void clear();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
