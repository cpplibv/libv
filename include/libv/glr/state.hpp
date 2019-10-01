// File:   state.hpp Author: Vader Created on 2018. december 29., 20:10

#pragma once

// libv
#include <libv/utility/guard.hpp>
// std
#include <cstdint>
#include <stack>
#include <vector>


namespace libv {
namespace glr {

// -------------------------------------------------------------------------------------------------

struct State {
	using type = uint64_t;

	type capabilityBlend:1 = 0;
	type capabilityCullFace:1 = 0;
	type capabilityDepthTest:1 = 0;
	type capabilityRasterizerDiscard:1 = 0;
	type capabilityScissorTest:1 = 0;
	type capabilityStencilTest:1 = 0;
	type capabilityTextureCubeMapSeamless:1 = 0;

	/// 0 = disable, 1 = enable
	type maskDepth:1 = 1;

	/// 0 = CW,
	/// 1 = CCW,
	type frontFaceCCW:1 = 1;

	/// 0 = Back,
	/// 1 = Front,
	type cullFace:1 = 1;

	/// 0 = Fill,
	/// 1 = Line,
	/// 2 = Point,
	type polygonMode:2 = 0;

	/// 0 = LEqual,
	/// 1 = GEqual,
	/// 2 = Less,
	/// 3 = Greater,
	/// 4 = Equal,
	/// 5 = NotEqual,
	/// 6 = Always,
	/// 7 = Never,
	type depthFunction:3 = 2;

	///	0 = ConstantAlpha,
	///	1 = ConstantColor,
	///	2 = DestinationAlpha,
	///	3 = DestinationColor,
	///	4 = One,
	///	5 = One_Minus_ConstantAlpha,
	///	6 = One_Minus_ConstantColor,
	///	7 = One_Minus_DestinationAlpha,
	///	8 = One_Minus_DestinationColor,
	///	9 = One_Minus_Source1Alpha,
	///	10 = One_Minus_Source1Color,
	///	11 = One_Minus_SourceAlpha,
	///	12 = One_Minus_SourceColor,
	///	13 = Source1Alpha,
	///	14 = Source1Color,
	///	15 = SourceAlpha,
	///	16 = SourceAlphaSaturate,
	///	17 = SourceColor,
	///	18 = Zero,
	type blendSrcFunction:5 = 15;

	///	0 = ConstantAlpha,
	///	1 = ConstantColor,
	///	2 = DestinationAlpha,
	///	3 = DestinationColor,
	///	4 = One,
	///	5 = One_Minus_ConstantAlpha,
	///	6 = One_Minus_ConstantColor,
	///	7 = One_Minus_DestinationAlpha,
	///	8 = One_Minus_DestinationColor,
	///	9 = One_Minus_Source1Alpha,
	///	10 = One_Minus_Source1Color,
	///	11 = One_Minus_SourceAlpha,
	///	12 = One_Minus_SourceColor,
	///	13 = Source1Alpha,
	///	14 = Source1Color,
	///	15 = SourceAlpha,
	///	16 = SourceAlphaSaturate,
	///	17 = SourceColor,
	///	18 = Zero,
	type blendDstFunction:5 = 11;

	/// 0 = No clip plane active,
	/// 1 = CLIP_PLANE0,
	/// 2 = CLIP_PLANE0 CLIP_PLANE1,
	/// 3 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2,
	/// 4 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2 CLIP_PLANE3,
	/// 5 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2 CLIP_PLANE3 CLIP_PLANE4,
	/// 6 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2 CLIP_PLANE3 CLIP_PLANE4 CLIP_PLANE5,
	type clipPlanes:3 = 0;

	///	unused zero initialized padding bits
	type _reserved:36 = 0;

	bool operator==(const State& rhs) const {
		return reinterpret_cast<const type&>(*this) == reinterpret_cast<const type&>(rhs);
	}

	bool operator!=(const State& rhs) const {
		return !(*this == rhs);
	}
};
static_assert(sizeof(State) == sizeof(State::type), "layout error");

// -------------------------------------------------------------------------------------------------

class StateStack {
private:
	std::stack<State, std::vector<State>> state_stack;

public:
	StateStack() {
		state_stack.emplace();
	}

public:
	inline void push() noexcept {
		state_stack.push(state_stack.top());
	}

	inline void pop() noexcept {
		state_stack.pop();
	}

	[[nodiscard]] inline auto push_guard() noexcept {
		push();
		return libv::Guard([this] { pop(); });
	}

	inline State state() const noexcept {
		return state_stack.top();
	}

public:
	inline void enableBlend() noexcept {
		state_stack.top().capabilityBlend = 1;
	}
	inline void enableCullFace() noexcept {
		state_stack.top().capabilityCullFace = 1;
	}
	inline void enableDepthTest() noexcept {
		state_stack.top().capabilityDepthTest = 1;
	}
	inline void enableRasterizerDiscard() noexcept {
		state_stack.top().capabilityRasterizerDiscard = 1;
	}
	inline void enableScissorTest() noexcept {
		state_stack.top().capabilityScissorTest = 1;
	}
	inline void enableStencilTest() noexcept {
		state_stack.top().capabilityStencilTest = 1;
	}
	inline void enableTextureCubeMapSeamless() noexcept {
		state_stack.top().capabilityTextureCubeMapSeamless = 1;
	}

	inline void disableBlend() noexcept {
		state_stack.top().capabilityBlend = 0;
	}
	inline void disableCullFace() noexcept {
		state_stack.top().capabilityCullFace = 0;
	}
	inline void disableDepthTest() noexcept {
		state_stack.top().capabilityDepthTest = 0;
	}
	inline void disableRasterizerDiscard() noexcept {
		state_stack.top().capabilityRasterizerDiscard = 0;
	}
	inline void disableScissorTest() noexcept {
		state_stack.top().capabilityScissorTest = 0;
	}
	inline void disableStencilTest() noexcept {
		state_stack.top().capabilityStencilTest = 0;
	}
	inline void disableTextureCubeMapSeamless() noexcept {
		state_stack.top().capabilityTextureCubeMapSeamless = 0;
	}

	inline void enableDepthMask() noexcept {
		state_stack.top().maskDepth = 1;
	}
	inline void disableDepthMask() noexcept {
		state_stack.top().maskDepth = 0;
	}

	inline void frontFaceCW() noexcept {
		state_stack.top().frontFaceCCW = 0;
	}
	inline void frontFaceCCW() noexcept {
		state_stack.top().frontFaceCCW = 1;
	}

	inline void cullBackFace() noexcept {
		state_stack.top().cullFace = 0;
	}
	inline void cullFrontFace() noexcept {
		state_stack.top().cullFace = 1;
	}

	inline void polygonModeFill() noexcept {
		state_stack.top().polygonMode = 0;
	}
	inline void polygonModeLine() noexcept {
		state_stack.top().polygonMode = 1;
	}
	inline void polygonModePoint() noexcept {
		state_stack.top().polygonMode = 2;
	}

	inline void depthFunctionLEqual() noexcept {
		state_stack.top().depthFunction = 0;
	}
	inline void depthFunctionGEqual() noexcept {
		state_stack.top().depthFunction = 1;
	}
	inline void depthFunctionLess() noexcept {
		state_stack.top().depthFunction = 2;
	}
	inline void depthFunctionGreater() noexcept {
		state_stack.top().depthFunction = 3;
	}
	inline void depthFunctionEqual() noexcept {
		state_stack.top().depthFunction = 4;
	}
	inline void depthFunctionNotEqual() noexcept {
		state_stack.top().depthFunction = 5;
	}
	inline void depthFunctionAlways() noexcept {
		state_stack.top().depthFunction = 6;
	}
	inline void depthFunctionNever() noexcept {
		state_stack.top().depthFunction = 7;
	}

	inline void blendSrc_ConstantAlpha() noexcept {
		state_stack.top().blendSrcFunction = 0;
	}
	inline void blendSrc_ConstantColor() noexcept {
		state_stack.top().blendSrcFunction = 1;
	}
	inline void blendSrc_DestinationAlpha() noexcept {
		state_stack.top().blendSrcFunction = 2;
	}
	inline void blendSrc_DestinationColor() noexcept {
		state_stack.top().blendSrcFunction = 3;
	}
	inline void blendSrc_One() noexcept {
		state_stack.top().blendSrcFunction = 4;
	}
	inline void blendSrc_One_Minus_ConstantAlpha() noexcept {
		state_stack.top().blendSrcFunction = 5;
	}
	inline void blendSrc_One_Minus_ConstantColor() noexcept {
		state_stack.top().blendSrcFunction = 6;
	}
	inline void blendSrc_One_Minus_DestinationAlpha() noexcept {
		state_stack.top().blendSrcFunction = 7;
	}
	inline void blendSrc_One_Minus_DestinationColor() noexcept {
		state_stack.top().blendSrcFunction = 8;
	}
	inline void blendSrc_One_Minus_Source1Alpha() noexcept {
		state_stack.top().blendSrcFunction = 9;
	}
	inline void blendSrc_One_Minus_Source1Color() noexcept {
		state_stack.top().blendSrcFunction = 10;
	}
	inline void blendSrc_One_Minus_SourceAlpha() noexcept {
		state_stack.top().blendSrcFunction = 11;
	}
	inline void blendSrc_One_Minus_SourceColor() noexcept {
		state_stack.top().blendSrcFunction = 12;
	}
	inline void blendSrc_Source1Alpha() noexcept {
		state_stack.top().blendSrcFunction = 13;
	}
	inline void blendSrc_Source1Color() noexcept {
		state_stack.top().blendSrcFunction = 14;
	}
	inline void blendSrc_SourceAlpha() noexcept {
		state_stack.top().blendSrcFunction = 15;
	}
	inline void blendSrc_SourceAlphaSaturate() noexcept {
		state_stack.top().blendSrcFunction = 16;
	}
	inline void blendSrc_SourceColor() noexcept {
		state_stack.top().blendSrcFunction = 17;
	}
	inline void blendSrc_Zero() noexcept {
		state_stack.top().blendSrcFunction = 18;
	}

	inline void blendDst_ConstantAlpha() noexcept {
		state_stack.top().blendDstFunction = 0;
	}
	inline void blendDst_ConstantColor() noexcept {
		state_stack.top().blendDstFunction = 1;
	}
	inline void blendDst_DestinationAlpha() noexcept {
		state_stack.top().blendDstFunction = 2;
	}
	inline void blendDst_DestinationColor() noexcept {
		state_stack.top().blendDstFunction = 3;
	}
	inline void blendDst_One() noexcept {
		state_stack.top().blendDstFunction = 4;
	}
	inline void blendDst_One_Minus_ConstantAlpha() noexcept {
		state_stack.top().blendDstFunction = 5;
	}
	inline void blendDst_One_Minus_ConstantColor() noexcept {
		state_stack.top().blendDstFunction = 6;
	}
	inline void blendDst_One_Minus_DestinationAlpha() noexcept {
		state_stack.top().blendDstFunction = 7;
	}
	inline void blendDst_One_Minus_DestinationColor() noexcept {
		state_stack.top().blendDstFunction = 8;
	}
	inline void blendDst_One_Minus_Source1Alpha() noexcept {
		state_stack.top().blendDstFunction = 9;
	}
	inline void blendDst_One_Minus_Source1Color() noexcept {
		state_stack.top().blendDstFunction = 10;
	}
	inline void blendDst_One_Minus_SourceAlpha() noexcept {
		state_stack.top().blendDstFunction = 11;
	}
	inline void blendDst_One_Minus_SourceColor() noexcept {
		state_stack.top().blendDstFunction = 12;
	}
	inline void blendDst_Source1Alpha() noexcept {
		state_stack.top().blendDstFunction = 13;
	}
	inline void blendDst_Source1Color() noexcept {
		state_stack.top().blendDstFunction = 14;
	}
	inline void blendDst_SourceAlpha() noexcept {
		state_stack.top().blendDstFunction = 15;
	}
	inline void blendDst_SourceAlphaSaturate() noexcept {
		state_stack.top().blendDstFunction = 16;
	}
	inline void blendDst_SourceColor() noexcept {
		state_stack.top().blendDstFunction = 17;
	}
	inline void blendDst_Zero() noexcept {
		state_stack.top().blendDstFunction = 18;
	}

	inline void clipPlanes(uint32_t i) noexcept {
		switch (i) {
		case 0: state_stack.top().clipPlanes = 0; break;
		case 1: state_stack.top().clipPlanes = 1; break;
		case 2: state_stack.top().clipPlanes = 2; break;
		case 3: state_stack.top().clipPlanes = 3; break;
		case 4: state_stack.top().clipPlanes = 4; break;
		case 5: state_stack.top().clipPlanes = 5; break;
		case 6: state_stack.top().clipPlanes = 6; break;
		default: // TODO P5: libv.gl: instead of default log and assert invalid input
			state_stack.top().clipPlanes = 6;
		}
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace glr
} // namespace libv
