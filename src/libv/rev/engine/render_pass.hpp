// Created by Vader on 2023.01.09..

#pragma once

// libv
#include <libv/math/frustum.hpp>
#include <libv/math/vec.hpp>


namespace libv {
namespace rev {

// -------------------------------------------------------------------------------------------------

class RenderPass {
	libv::vec3f eye_;
	libv::frustum viewFrustum_;

public:
	RenderPass(const libv::vec3f& eye, const libv::frustum& viewFrustum) :
		eye_(eye),
		viewFrustum_(viewFrustum) {}

public:
	[[nodiscard]] constexpr inline const libv::vec3f& eye() const noexcept {
		return eye_;
	}

	[[nodiscard]] constexpr inline const libv::frustum& viewFrustum() const noexcept {
		return viewFrustum_;
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace rev
} // namespace libv
