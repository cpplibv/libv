// Project: libv.ui, File: src/libv/ui/style/style_loader.hpp

#pragma once

#include <libv/fsw/fwd.hpp>
#include <libv/ui/fwd.hpp>

#include <memory>


namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class ImplStyleLoader;

class StyleLoader {
private:
	std::unique_ptr<ImplStyleLoader> self;

public:
	StyleLoader(UI& ui, Settings& settings, libv::fsw::Watcher& fsw);
	~StyleLoader();

public:
	void load(std::string_view fileIdentifier);
	void terminate();
};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv
