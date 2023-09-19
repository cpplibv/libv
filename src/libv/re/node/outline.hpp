// Project: libv.re, File: src/libv/re/node/outline.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/core/context/context_queue_override.hpp>
#include <libv/re/node/proxy.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Outline : public Proxy {
public:
	using ptr = Outline_ptr;
	using cptr = Outline_cptr;

public:
	ContextQueueOverride materials;

public:
	explicit Outline(Node_ptr node, libv::vec4f color);

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(Node_ptr node, libv::vec4f color) {
		return ptr::make(std::move(node), color);
	}

public:
	virtual void queue(ContextQueue& ctx) override;
	virtual void queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) override;
	virtual void gl_update(ContextUpdate& ctx) override;
	virtual void gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
