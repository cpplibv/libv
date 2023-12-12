// Project: libv.re, File: src/libv/re/core/scene.hpp

#pragma once

#include <libv/gl/fwd.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/common/debug_capture_mode.hpp>
#include <libv/re/fwd.hpp>

#include <memory>
#include <string>
#include <string_view>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Scene : public libv::ref_count_base {
public:
	// TODO P5: Remove double indirection (pimpl stays, intrusive_ptr goes)
	friend libv::ref_count_access;
	using ptr = Scene_ptr;
	using cptr = Scene_cptr;

private:
	std::unique_ptr<SceneInternal> self;

public:
	explicit Scene(std::string name);
	~Scene();

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(std::string name = "noname") {
		return ptr::make(std::move(name));
	}

public:
	void add(const Node_ptr& node);
	void add(Node_ptr&& node);
	void remove(const Node_ptr& node);

public:
	void render(libv::GL& gl, const Camera& camera, libv::vec2i canvasPosition, libv::vec2i canvasSize, libv::vec2f localMousePosition, double timeSimulation, double timeReal);
	// void create(libv::GL& gl) { }
	// void destroy(libv::GL& gl) { }
	// void update(libv::GL& gl);
	// void render(libv::re::Canvas&, libv::GL&) { }
	// void async_update(libv::time_duration deltaTime) { }
	// void update(libv::time_duration deltaTime) { }

public:
	void freezeViewFrustum(bool value) noexcept;
	[[nodiscard]] bool freezeViewFrustum() const noexcept;
	void debugPass(std::string_view mode, int index, DebugCaptureMode captureMode);
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
