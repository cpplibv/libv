// File: font.hpp Author: Vader Created on 2016. szeptember 16., 23:38

#pragma once

// libv
#include <libv/ui/font/font_2D.hpp>
// pro
#include <libv/ui/resource/resource.hpp>

namespace libv {
namespace ui {

// -------------------------------------------------------------------------------------------------

class UIFont2D : public Resource<UIFont2D> {
private:
	ui::Font2D font_;
	File_view file;
public:
	UIFont2D(const File_view& file);
	~UIFont2D() { }

private:
	ResourceStepResult loadIN();
	ResourceStepResult loadGL();
	ResourceStepResult unloadIN();
	ResourceStepResult unloadGL();

public:
	static constexpr const char ResourceLabel[] = "Font2D";
	static constexpr Step ResourceLoadSequence[] = {
		{loadIN, ThreadAffinity::ANY},
		{loadGL, ThreadAffinity::GL}
	};
	static constexpr Step ResourceUnloadSequence[] = {
		{unloadIN, ThreadAffinity::IN},
		{unloadGL, ThreadAffinity::GL}
	};
	// TODO P4: This system does not handle failed step and rollback, it only handles full load / unload pairs
	// TODO P4: This system requires a definition for this sequence arrays, this could be better.

public:
	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	inline decltype(auto) getLineAdvance(uint32_t size) {
		return font_.getLineAdvance(size);
	}
	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	inline decltype(auto) getDescender(uint32_t size) {
		return font_.getLineAdvance(size);
	}
	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	inline decltype(auto) getKerning(uint32_t left, uint32_t right, uint32_t size) {
		return font_.getKerning(left, right, size);
	}
	/**
	 * @state This operation requires loaded state
	 * @context ANY
	 */
	inline decltype(auto) getCharacter(uint32_t unicode, uint32_t size) {
		return font_.getCharacter(unicode, size);
	}
	/** Bind the font's texture to the openGL state machine
	 * @state This operation requires loaded state
	 * @context GL
	 */
	inline void bind() {
		font_.updateGL();
		font_.bind();
	}
	/** Bind the font's texture to the openGL state machine
	 * @state This operation requires loaded state
	 * @context GL
	 */
	inline auto bindGuard() {
		return gl::BindGuard<UIFont2D>(*this);
	}
	/** Unbind the font's shader and texture to the openGL state machine
	 * @state This operation requires loaded state
	 * @context GL
	 */
	inline void unbind() {
		font_.unbind();
	}
};

// -------------------------------------------------------------------------------------------------

} //namespace ui
} //namespace libv
