// Project: libv.re, File: src/libv/re/core/queue_context.hpp

#pragma once

#include <libv/re/fwd.hpp>
#include <libv/re/core/context/context_frame.hpp> // For convenience

#include <span>


namespace libv::re {

// -------------------------------------------------------------------------------------------------

class ContextQueue {
private:
	SceneInternal& internalScene;
public:
	ContextFrame& frame;

public:
	inline ContextQueue(SceneInternal& internalScene, ContextFrame& frame) :
		internalScene(internalScene),
		frame(frame) {
	}

public:
	void queue(const Light& light);
	void queue(Node* node, const libv::mat4f& matM, Mesh* mesh, Material* material, SubObjectIndex subObjectIndex);
	std::span<libv::vec2f> queueDepthQuery(ReadbackQueryIndex& queryIndex, uint32_t count);

public:
	[[nodiscard]] bool testVisibility(const libv::mat4f& mat, const BoundingSphere& bs);
};

// -------------------------------------------------------------------------------------------------

} // namespace libv::re
