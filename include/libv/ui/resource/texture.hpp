// File:   ui_texture.hpp Author: Vader Created on 2016. szeptember 6., 15:21

#pragma once

// libv
#include <libv/gl/texture.hpp>
// pro
#include <libv/ui/resource/resource.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

struct UITexture2D : public Resource<UITexture2D> {
private:
	ResourceStepResult load();
	ResourceStepResult unload();

public:
	static constexpr const char ResourceLabel[] = "Texture";
	static constexpr Step ResourceLoadSequence[] = {
		{load, ThreadAffinity::IO}
	};
	static constexpr Step ResourceUnloadSequence[] = {
		{unload, ThreadAffinity::GL}
	};

	// ---------------------------------------------------------------------------------------------
private:
	//	gl::Texture2D texture;
	File_view file;

public:
	UITexture2D(const File_view& file) : file(file) { }

};

// -------------------------------------------------------------------------------------------------

} // namespace ui
} // namespace libv