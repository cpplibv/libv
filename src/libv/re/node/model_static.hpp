// Project: libv.re, File: src/libv/re/node/model_static.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/node/model_node.hpp>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

class ModelStatic : public ModelNode {
public:
	using ptr = ModelStatic_ptr;
	using cptr = ModelStatic_cptr;

private:
	Model_ptr model;

public:
	explicit ModelStatic(const Model_ptr& model);
	ModelStatic(libv::vec3f position, const Model_ptr& model);
	ModelStatic(const Transform& transform, const Model_ptr& model);

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(const Model_ptr& model) {
		return ptr::make(model);
	}

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(libv::vec3f position, const Model_ptr& model) {
		return ptr::make(position, model);
	}

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(const Transform& transform, const Model_ptr& model) {
		return ptr::make(transform, model);
	}

public:
	virtual void queue(ContextQueue& ctx) override;
	virtual void queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) override;
	virtual void gl_update(ContextUpdate& ctx) override;
	virtual void gl_render(ContextRender& ctx, SubObjectIndex subObjectIndex) override;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
