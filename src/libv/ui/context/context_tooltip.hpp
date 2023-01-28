// Created by dalma.bobula on 1/15/2023.

#pragma once

//std
#include <memory>

#include <libv/ui/fwd.hpp>


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
