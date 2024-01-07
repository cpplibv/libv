// Created by dalma.bobula on 1/15/2023.

#pragma once

#include <libv/ui/fwd.hpp>
#include <libv/ui/context/context_ui.hpp> // IWYU pragma: export // For convenience

#include <memory>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplContextTooltip;

class ContextTooltip {
	friend ImplUI;

private:
	std::unique_ptr<ImplContextTooltip> self;

public:
	ContextTooltip();
	~ContextTooltip();

private:
	void init();

public:
	Component overlay();
	void show_tooltip(Component owner, Component tooltip);
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
