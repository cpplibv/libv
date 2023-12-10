// Project: libv.re, File: src/libv/re/core/render_state.hpp

#pragma once

// std
#include <bit>
#include <cstdint>
// libv
#include <libv/gl/enum.hpp> // IWYU pragma: export
#include <libv/gl/fwd.hpp>
#include <libv/meta/force_inline.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct RenderState {
	using type = uint64_t;

	/// 0 = Disable
	/// 1 = Enable
	static constexpr uint32_t sizeCapabilityBlend = 1;
	type capabilityBlend : sizeCapabilityBlend = 0; // Default: Disable
	static constexpr uint32_t sizeCapabilityCullFace = 1;
	type capabilityCullFace : sizeCapabilityCullFace = 1; // Default: Enable
	static constexpr uint32_t sizeCapabilityDepthTest = 1;
	type capabilityDepthTest : sizeCapabilityDepthTest = 1; // Default: Enable
	// static constexpr uint32_t sizeCapabilityMultisample = 1;
	// type capabilityMultisample : sizeCapabilityMultisample = 1; // Default: Enable // Implemented but ran out of bits, most likely never used
	static constexpr uint32_t sizeCapabilityRasterizerDiscard = 1;
	type capabilityRasterizerDiscard : sizeCapabilityRasterizerDiscard = 0; // Default: Disable
	static constexpr uint32_t sizeCapabilityScissorTest = 1;
	type capabilityScissorTest : sizeCapabilityScissorTest = 0; // Default: Disable
	static constexpr uint32_t sizeCapabilityStencilTest = 1;
	type capabilityStencilTest : sizeCapabilityStencilTest = 0; // Default: Disable
	// static constexpr uint32_t sizeCapabilityTextureCubeMapSeamless = 1;
	// type capabilityTextureCubeMapSeamless : sizeCapabilityTextureCubeMapSeamless = 1; // Default: Enable // Implemented but ran out of bits, most likely never used

	// /// 4 bit mask for RGBA channels
	// static constexpr uint32_t sizeMaskColor = 4;
	// type maskColor : sizeMaskColor = 0b1111; // Default: RGBA // Implemented but ran out of bits, most likely never used

	/// 0 = Disable
	/// 1 = Enable
	static constexpr uint32_t sizeMaskDepth = 1;
	type maskDepth : sizeMaskDepth = 0; // Default: Disable
	// Default is chosen to facilitate depth prime, Normally Enable would be a better default

	/// 8 bit mask
	static constexpr uint32_t sizeMaskStencil = 8;
	type maskStencil : sizeMaskStencil = 0x00; // Default: 0x00

	/// 0 = LEqual,
	/// 1 = GEqual,
	/// 2 = Less,
	/// 3 = Greater,
	/// 4 = Equal,
	/// 5 = NotEqual,
	/// 6 = Always,
	/// 7 = Never,
	static constexpr uint32_t sizeStencilFunction = 3;
	type stencilFunction_ : sizeStencilFunction = 6; // Default: Always

	/// 8 bit value
	static constexpr uint32_t sizeStencilFunctionReference = 8;
	type stencilFunctionReference_ : sizeStencilFunctionReference = 0x00; // Default: 0x00

	/// 8 bit mask
	static constexpr uint32_t sizeStencilFunctionMask = 8;
	type stencilFunctionMask_ : sizeStencilFunctionMask = 0xFF; // Default: 0xFF

	// 0 = Keep
	// 1 = Zero
	// 2 = Replace
	// 3 = Increase
	// 4 = IncreaseWrap
	// 5 = Decrease
	// 6 = DecreaseWrap
	// 7 = Invert
	static constexpr uint32_t sizeStencilOpStencilFail = 3;
	type stencilOpStencilFail_ : sizeStencilOpStencilFail = 0; // Default: GL_KEEP

	// 0 = Keep
	// 1 = Zero
	// 2 = Replace
	// 3 = Increase
	// 4 = IncreaseWrap
	// 5 = Decrease
	// 6 = DecreaseWrap
	// 7 = Invert
	static constexpr uint32_t sizeStencilOpDepthFail = 3;
	type stencilOpDepthFail_ : sizeStencilOpDepthFail = 0; // Default: GL_KEEP

	// 0 = Keep
	// 1 = Zero
	// 2 = Replace
	// 3 = Increase
	// 4 = IncreaseWrap
	// 5 = Decrease
	// 6 = DecreaseWrap
	// 7 = Invert
	static constexpr uint32_t sizeStencilOpStencilAndDepthPass = 3;
	type stencilOpStencilAndDepthPass_ : sizeStencilOpStencilAndDepthPass = 0; // Default: GL_KEEP

	/// 0 = CW,
	/// 1 = CCW,
	static constexpr uint32_t sizeFrontFace = 1;
	type frontFace_ : sizeFrontFace = 1; // Default: CCW

	/// 0 = Back,
	/// 1 = Front,
	static constexpr uint32_t sizeCullFace = 1;
	type cullFace_ : sizeCullFace = 0; // Default: Back

	/// 0 = Fill,
	/// 1 = Line,
	/// 2 = Point,
	static constexpr uint32_t sizePolygonMode = 2;
	type polygonMode_ : sizePolygonMode = 0; // Default: Fill

	/// 0 = LEqual,
	/// 1 = GEqual,
	/// 2 = Less,
	/// 3 = Greater,
	/// 4 = Equal,
	/// 5 = NotEqual,
	/// 6 = Always,
	/// 7 = Never,
	static constexpr uint32_t sizeDepthFunction = 3;
	type depthFunction_ : sizeDepthFunction = 1; // Default: GEqual
	// Default is chosen to facilitate depth prime and ReverseZ, Normally Less would be a better default

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
	static constexpr uint32_t sizeBlendSrc = 5;
	type blendSrc_ : sizeBlendSrc = 15; // Default: SourceAlpha

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
	static constexpr uint32_t sizeBlendDst = 5;
	type blendDst_ : sizeBlendDst = 11; // Default: One_Minus_SourceAlpha

	/// 0 = No clip plane active,
	/// 1 = CLIP_PLANE0,
	/// 2 = CLIP_PLANE0 CLIP_PLANE1,
	/// 3 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2,
	/// 4 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2 CLIP_PLANE3,
	/// 5 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2 CLIP_PLANE3 CLIP_PLANE4,
	/// 6 = CLIP_PLANE0 CLIP_PLANE1 CLIP_PLANE2 CLIP_PLANE3 CLIP_PLANE4 CLIP_PLANE5,
	static constexpr uint32_t sizeClipPlanes = 3;
	type clipPlanes_ : sizeClipPlanes = 0; // Default: 0, No clip plane active

	///	Unused zero initialized padding bits
	type _reserved:1 = 0;

public:
	[[nodiscard]] constexpr LIBV_FORCE_INLINE type as_int() const noexcept {
		return std::bit_cast<type>(*this);
	}

public:
	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE bool operator==(const RenderState& lhs, const RenderState& rhs) noexcept {
		return lhs.as_int() == rhs.as_int();
	}

	[[nodiscard]] friend constexpr LIBV_FORCE_INLINE bool operator!=(const RenderState& lhs, const RenderState& rhs) noexcept {
		return lhs.as_int() != rhs.as_int();
	}

private:
	[[nodiscard]] static constexpr inline uint32_t sizeToBits(uint32_t numBits) noexcept {
		return (1u << numBits) - 1u;
	}

public:
	constexpr inline void mask_blend() noexcept {
		capabilityBlend = sizeToBits(sizeCapabilityBlend);
	}
	constexpr inline void blend(bool value) noexcept {
		capabilityBlend = value ? 1 : 0;
	}
	[[nodiscard]] constexpr inline bool blend() const noexcept {
		return capabilityBlend != 0;
	}
	constexpr inline void mask_cull() noexcept {
		capabilityCullFace = sizeToBits(sizeCapabilityCullFace);
	}
	constexpr inline void cull(bool value) noexcept {
		capabilityCullFace = value ? 1 : 0;
	}
	[[nodiscard]] constexpr inline bool cull() const noexcept {
		return capabilityCullFace != 0;
	}
	constexpr inline void mask_depthTest() noexcept {
		capabilityDepthTest = sizeToBits(sizeCapabilityDepthTest);
	}
	constexpr inline void depthTest(bool value) noexcept {
		capabilityDepthTest = value ? 1 : 0;
	}
	[[nodiscard]] constexpr inline bool depthTest() const noexcept {
		return capabilityDepthTest != 0;
	}
	// constexpr inline void mask_multisample(bool value) noexcept {
	// 	capabilityMultisample = sizeToBits(sizeMultisample;
	// }
	// constexpr inline void multisample(bool value) noexcept {
	// 	capabilityMultisample = value ? 1 : 0;
	// }
	// [[nodiscard]] constexpr inline bool multisample() const noexcept {
	// 	return capabilityMultisample != 0;
	// }
	constexpr inline void mask_rasterizerDiscard() noexcept {
		capabilityRasterizerDiscard = sizeToBits(sizeCapabilityRasterizerDiscard);
	}
	constexpr inline void rasterizerDiscard(bool value) noexcept {
		capabilityRasterizerDiscard = value ? 1 : 0;
	}
	[[nodiscard]] constexpr inline bool rasterizerDiscard() const noexcept {
		return capabilityRasterizerDiscard != 0;
	}
	constexpr inline void mask_scissorTest() noexcept {
		capabilityRasterizerDiscard = sizeToBits(sizeCapabilityScissorTest);
	}
	constexpr inline void scissorTest(bool value) noexcept {
		capabilityScissorTest = value ? 1 : 0;
	}
	[[nodiscard]] constexpr inline bool scissorTest() const noexcept {
		return capabilityScissorTest != 0;
	}
	constexpr inline void mask_stencilTest() noexcept {
		capabilityScissorTest = sizeToBits(sizeCapabilityStencilTest);
	}
	constexpr inline void stencilTest(bool value) noexcept {
		capabilityStencilTest = value ? 1 : 0;
	}
	[[nodiscard]] constexpr inline bool stencilTest() const noexcept {
		return capabilityStencilTest != 0;
	}
	// constexpr inline void mask_textureCubeMapSeamless() noexcept {
	// 	capabilityTextureCubeMapSeamless = sizeToBits(sizeTextureCubeMapSeamless;
	// }
	// constexpr inline void textureCubeMapSeamless(bool value) noexcept {
	// 	capabilityTextureCubeMapSeamless = value ? 1 : 0;
	// }
	// [[nodiscard]] constexpr inline bool textureCubeMapSeamless() const noexcept {
	// 	return capabilityTextureCubeMapSeamless != 0;
	// }

	constexpr inline void mask_depthWrite() noexcept {
		maskDepth = sizeToBits(sizeMaskDepth);
	}
	constexpr inline void depthWrite(bool value) noexcept {
		maskDepth = value ? 1 : 0;
	}
	[[nodiscard]] constexpr inline bool depthWrite() const noexcept {
		return maskDepth != 0;
	}

	// constexpr inline void mask_colorWrite() noexcept {
	// 	maskColor = sizeToBits(sizeColorWrite;
	// }
	// constexpr inline void colorWrite(bool r, bool g, bool b, bool a) noexcept {
	// 	maskColor = 0b1111 & static_cast<uint8_t>(
	// 			(r ? 1u << 3u : 0) |
	// 			(g ? 1u << 2u : 0) |
	// 			(b ? 1u << 1u : 0) |
	// 			(a ? 1u << 0u : 0));
	// }
	// [[nodiscard]] constexpr inline uint8_t colorWrite() const noexcept {
	// 	assert(maskColor <= 0b1111 && "Invalid maskColor enum value");
	// 	return maskColor;
	// }

	constexpr inline void mask_stencilWrite() noexcept {
		maskStencil = sizeToBits(sizeMaskStencil);
	}
	constexpr inline void stencilWrite(uint8_t mask) noexcept {
		maskStencil = mask;
	}
	[[nodiscard]] constexpr inline uint8_t stencilWrite() const noexcept {
		return maskStencil;
	}

	constexpr inline void mask_stencilFunction() noexcept {
		stencilFunction_ = sizeToBits(sizeStencilFunction);
	}
	// No set() alias for stencilFunction
	constexpr inline void stencilFunction(libv::gl::TestFunction stencilFunction) noexcept {
		switch (stencilFunction) {
		case libv::gl::TestFunction::LEqual: stencilFunction_ = 0; return;
		case libv::gl::TestFunction::GEqual: stencilFunction_ = 1; return;
		case libv::gl::TestFunction::Less: stencilFunction_ = 2; return;
		case libv::gl::TestFunction::Greater: stencilFunction_ = 3; return;
		case libv::gl::TestFunction::Equal: stencilFunction_ = 4; return;
		case libv::gl::TestFunction::NotEqual: stencilFunction_ = 5; return;
		case libv::gl::TestFunction::Always: stencilFunction_ = 6; return;
		case libv::gl::TestFunction::Never: stencilFunction_ = 7; return;
		}
		assert(false && "Invalid TestFunction enum value");
	}
	[[nodiscard]] constexpr inline libv::gl::TestFunction stencilFunction() const noexcept {
		switch (stencilFunction_) {
		case 0: return libv::gl::TestFunction::LEqual;
		case 1: return libv::gl::TestFunction::GEqual;
		case 2: return libv::gl::TestFunction::Less;
		case 3: return libv::gl::TestFunction::Greater;
		case 4: return libv::gl::TestFunction::Equal;
		case 5: return libv::gl::TestFunction::NotEqual;
		case 6: return libv::gl::TestFunction::Always;
		case 7: return libv::gl::TestFunction::Never;
		}
		assert(false && "Invalid TestFunction state value");
		return libv::gl::TestFunction::Always;
	}

	constexpr inline void mask_stencilFunctionReference() noexcept {
		stencilFunctionReference_ = sizeToBits(sizeStencilFunctionReference);
	}
	constexpr inline void stencilFunctionReference(uint8_t mask) noexcept {
		stencilFunctionReference_ = mask;
	}
	[[nodiscard]] constexpr inline uint8_t stencilFunctionReference() const noexcept {
		return stencilFunctionReference_;
	}

	constexpr inline void mask_stencilFunctionMask() noexcept {
		stencilFunctionMask_ = sizeToBits(sizeStencilFunctionMask);
	}
	constexpr inline void stencilFunctionMask(uint8_t mask) noexcept {
		stencilFunctionMask_ = mask;
	}
	[[nodiscard]] constexpr inline uint8_t stencilFunctionMask() const noexcept {
		return stencilFunctionMask_;
	}

	constexpr inline void mask_stencilOpStencilFail() noexcept {
		stencilOpStencilFail_ = sizeToBits(sizeStencilOpStencilFail);
	}
	constexpr inline void stencilOpStencilFail(libv::gl::StencilAction stencilFailAction) noexcept {
		switch (stencilFailAction) {
		case libv::gl::StencilAction::Keep: stencilOpStencilFail_ = 0; return;
		case libv::gl::StencilAction::Zero: stencilOpStencilFail_ = 1; return;
		case libv::gl::StencilAction::Replace: stencilOpStencilFail_ = 2; return;
		case libv::gl::StencilAction::Increase: stencilOpStencilFail_ = 3; return;
		case libv::gl::StencilAction::IncreaseWrap: stencilOpStencilFail_ = 4; return;
		case libv::gl::StencilAction::Decrease: stencilOpStencilFail_ = 5; return;
		case libv::gl::StencilAction::DecreaseWrap: stencilOpStencilFail_ = 6; return;
		case libv::gl::StencilAction::Invert: stencilOpStencilFail_ = 7; return;
		}
		assert(false && "Invalid StencilAction enum value");
	}
	[[nodiscard]] constexpr inline libv::gl::StencilAction stencilOpStencilFail() const noexcept {
		switch (stencilOpStencilFail_) {
		case 0: return libv::gl::StencilAction::Keep;
		case 1: return libv::gl::StencilAction::Zero;
		case 2: return libv::gl::StencilAction::Replace;
		case 3: return libv::gl::StencilAction::Increase;
		case 4: return libv::gl::StencilAction::IncreaseWrap;
		case 5: return libv::gl::StencilAction::Decrease;
		case 6: return libv::gl::StencilAction::DecreaseWrap;
		case 7: return libv::gl::StencilAction::Invert;
		}
		assert(false && "Invalid StencilAction state value");
		return libv::gl::StencilAction::Keep;
	}

	constexpr inline void mask_stencilOpDepthFail() noexcept {
		stencilOpDepthFail_ = sizeToBits(sizeStencilOpDepthFail);
	}
	constexpr inline void stencilOpDepthFail(libv::gl::StencilAction depthFailAction) noexcept {
		switch (depthFailAction) {
		case libv::gl::StencilAction::Keep: stencilOpDepthFail_ = 0; return;
		case libv::gl::StencilAction::Zero: stencilOpDepthFail_ = 1; return;
		case libv::gl::StencilAction::Replace: stencilOpDepthFail_ = 2; return;
		case libv::gl::StencilAction::Increase: stencilOpDepthFail_ = 3; return;
		case libv::gl::StencilAction::IncreaseWrap: stencilOpDepthFail_ = 4; return;
		case libv::gl::StencilAction::Decrease: stencilOpDepthFail_ = 5; return;
		case libv::gl::StencilAction::DecreaseWrap: stencilOpDepthFail_ = 6; return;
		case libv::gl::StencilAction::Invert: stencilOpDepthFail_ = 7; return;
		}
		assert(false && "Invalid StencilAction enum value");
	}
	[[nodiscard]] constexpr inline libv::gl::StencilAction stencilOpDepthFail() const noexcept {
		switch (stencilOpDepthFail_) {
		case 0: return libv::gl::StencilAction::Keep;
		case 1: return libv::gl::StencilAction::Zero;
		case 2: return libv::gl::StencilAction::Replace;
		case 3: return libv::gl::StencilAction::Increase;
		case 4: return libv::gl::StencilAction::IncreaseWrap;
		case 5: return libv::gl::StencilAction::Decrease;
		case 6: return libv::gl::StencilAction::DecreaseWrap;
		case 7: return libv::gl::StencilAction::Invert;
		}
		assert(false && "Invalid StencilAction state value");
		return libv::gl::StencilAction::Keep;
	}

	constexpr inline void mask_stencilOpStencilAndDepthPass() noexcept {
		stencilOpStencilAndDepthPass_ = sizeToBits(sizeStencilOpStencilAndDepthPass);
	}
	constexpr inline void stencilOpStencilAndDepthPass(libv::gl::StencilAction stencilAndDepthPassAction) noexcept {
		switch (stencilAndDepthPassAction) {
		case libv::gl::StencilAction::Keep: stencilOpStencilAndDepthPass_ = 0; return;
		case libv::gl::StencilAction::Zero: stencilOpStencilAndDepthPass_ = 1; return;
		case libv::gl::StencilAction::Replace: stencilOpStencilAndDepthPass_ = 2; return;
		case libv::gl::StencilAction::Increase: stencilOpStencilAndDepthPass_ = 3; return;
		case libv::gl::StencilAction::IncreaseWrap: stencilOpStencilAndDepthPass_ = 4; return;
		case libv::gl::StencilAction::Decrease: stencilOpStencilAndDepthPass_ = 5; return;
		case libv::gl::StencilAction::DecreaseWrap: stencilOpStencilAndDepthPass_ = 6; return;
		case libv::gl::StencilAction::Invert: stencilOpStencilAndDepthPass_ = 7; return;
		}
		assert(false && "Invalid StencilAction enum value");
	}
	[[nodiscard]] constexpr inline libv::gl::StencilAction stencilOpStencilAndDepthPass() const noexcept {
		switch (stencilOpStencilAndDepthPass_) {
		case 0: return libv::gl::StencilAction::Keep;
		case 1: return libv::gl::StencilAction::Zero;
		case 2: return libv::gl::StencilAction::Replace;
		case 3: return libv::gl::StencilAction::Increase;
		case 4: return libv::gl::StencilAction::IncreaseWrap;
		case 5: return libv::gl::StencilAction::Decrease;
		case 6: return libv::gl::StencilAction::DecreaseWrap;
		case 7: return libv::gl::StencilAction::Invert;
		}
		assert(false && "Invalid StencilAction state value");
		return libv::gl::StencilAction::Keep;
	}

	constexpr inline void mask_frontFace() noexcept {
		frontFace_ = sizeToBits(sizeFrontFace);
	}
	constexpr inline void set(libv::gl::FrontFace frontFace) noexcept {
		switch (frontFace) {
		case libv::gl::FrontFace::CW: frontFace_ = 0; return;
		case libv::gl::FrontFace::CCW: frontFace_ = 1; return;
		}
		assert(false && "Invalid FrontFace enum value");
	}
	constexpr inline void frontFace(libv::gl::FrontFace frontFace) noexcept {
		set(frontFace);
	}
	[[nodiscard]] constexpr inline libv::gl::FrontFace frontFace() const noexcept {
		switch (frontFace_) {
		case 0: return libv::gl::FrontFace::CW;
		case 1: return libv::gl::FrontFace::CCW;
		}
		assert(false && "Invalid FrontFace state value");
		return libv::gl::FrontFace::CCW;
	}

	constexpr inline void mask_cullFace() noexcept {
		cullFace_ = sizeToBits(sizeCullFace);
	}
	constexpr inline void set(libv::gl::CullFace cullFace) noexcept {
		switch (cullFace) {
		case libv::gl::CullFace::Back: cullFace_ = 0; return;
		case libv::gl::CullFace::Front: cullFace_ = 1; return;
		}
		assert(false && "Invalid CullFace enum value");
	}
	constexpr inline void cullFace(libv::gl::CullFace cullFace) noexcept {
		set(cullFace);
	}
	[[nodiscard]] constexpr inline libv::gl::CullFace cullFace() const noexcept {
		switch (cullFace_) {
		case 0: return libv::gl::CullFace::Back;
		case 1: return libv::gl::CullFace::Front;
		}
		assert(false && "Invalid CullFace state value");
		return libv::gl::CullFace::Back;
	}

	constexpr inline void mask_polygonMode() noexcept {
		polygonMode_ = sizeToBits(sizePolygonMode);
	}
	constexpr inline void set(libv::gl::PolygonMode polygonMode) noexcept {
		switch (polygonMode) {
		case libv::gl::PolygonMode::Fill: polygonMode_ = 0; return;
		case libv::gl::PolygonMode::Line: polygonMode_ = 1; return;
		case libv::gl::PolygonMode::Point: polygonMode_ = 2; return;
		}
		assert(false && "Invalid PolygonMode enum value");
	}
	constexpr inline void polygonMode(libv::gl::PolygonMode polygonMode) noexcept {
		set(polygonMode);
	}
	[[nodiscard]] constexpr inline libv::gl::PolygonMode polygonMode() const noexcept {
		switch (polygonMode_) {
		case 0: return libv::gl::PolygonMode::Fill;
		case 1: return libv::gl::PolygonMode::Line;
		case 2: return libv::gl::PolygonMode::Point;
		}
		assert(false && "Invalid PolygonMode state value");
		return libv::gl::PolygonMode::Fill;
	}

	constexpr inline void mask_depthFunction() noexcept {
		depthFunction_ = sizeToBits(sizeDepthFunction);
	}
	// No set() alias for depthFunction
	constexpr inline void depthFunction(libv::gl::TestFunction depthFunction) noexcept {
		switch (depthFunction) {
		case libv::gl::TestFunction::LEqual: depthFunction_ = 0; return;
		case libv::gl::TestFunction::GEqual: depthFunction_ = 1; return;
		case libv::gl::TestFunction::Less: depthFunction_ = 2; return;
		case libv::gl::TestFunction::Greater: depthFunction_ = 3; return;
		case libv::gl::TestFunction::Equal: depthFunction_ = 4; return;
		case libv::gl::TestFunction::NotEqual: depthFunction_ = 5; return;
		case libv::gl::TestFunction::Always: depthFunction_ = 6; return;
		case libv::gl::TestFunction::Never: depthFunction_ = 7; return;
		}
		assert(false && "Invalid TestFunction enum value");
	}
	[[nodiscard]] constexpr inline libv::gl::TestFunction depthFunction() const noexcept {
		switch (depthFunction_) {
		case 0: return libv::gl::TestFunction::LEqual;
		case 1: return libv::gl::TestFunction::GEqual;
		case 2: return libv::gl::TestFunction::Less;
		case 3: return libv::gl::TestFunction::Greater;
		case 4: return libv::gl::TestFunction::Equal;
		case 5: return libv::gl::TestFunction::NotEqual;
		case 6: return libv::gl::TestFunction::Always;
		case 7: return libv::gl::TestFunction::Never;
		}
		assert(false && "Invalid TestFunction state value");
		return libv::gl::TestFunction::Less;
	}

	constexpr inline void mask_blendSrc() noexcept {
		blendSrc_ = sizeToBits(sizeBlendSrc);
	}
	// No set() alias for srcBlend
	constexpr inline void blendSrc(libv::gl::BlendFunction blendSrc) noexcept {
		switch (blendSrc) {
		case libv::gl::BlendFunction::ConstantAlpha: blendSrc_ = 0; return;
		case libv::gl::BlendFunction::ConstantColor: blendSrc_ = 1; return;
		case libv::gl::BlendFunction::DestinationAlpha: blendSrc_ = 2; return;
		case libv::gl::BlendFunction::DestinationColor: blendSrc_ = 3; return;
		case libv::gl::BlendFunction::One: blendSrc_ = 4; return;
		case libv::gl::BlendFunction::One_Minus_ConstantAlpha: blendSrc_ = 5; return;
		case libv::gl::BlendFunction::One_Minus_ConstantColor: blendSrc_ = 6; return;
		case libv::gl::BlendFunction::One_Minus_DestinationAlpha: blendSrc_ = 7; return;
		case libv::gl::BlendFunction::One_Minus_DestinationColor: blendSrc_ = 8; return;
		case libv::gl::BlendFunction::One_Minus_Source1Alpha: blendSrc_ = 9; return;
		case libv::gl::BlendFunction::One_Minus_Source1Color:  blendSrc_ = 10; return;
		case libv::gl::BlendFunction::One_Minus_SourceAlpha:  blendSrc_ = 11; return;
		case libv::gl::BlendFunction::One_Minus_SourceColor:  blendSrc_ = 12; return;
		case libv::gl::BlendFunction::Source1Alpha:  blendSrc_ = 13; return;
		case libv::gl::BlendFunction::Source1Color:  blendSrc_ = 14; return;
		case libv::gl::BlendFunction::SourceAlpha:  blendSrc_ = 15; return;
		case libv::gl::BlendFunction::SourceAlphaSaturate:  blendSrc_ = 16; return;
		case libv::gl::BlendFunction::SourceColor:  blendSrc_ = 17; return;
		case libv::gl::BlendFunction::Zero:  blendSrc_ = 18; return;
		}
		assert(false && "Invalid BlendFunction enum value");
	}
	[[nodiscard]] constexpr inline libv::gl::BlendFunction blendSrc() const noexcept {
		switch (blendSrc_) {
		case 0: return libv::gl::BlendFunction::ConstantAlpha;
		case 1: return libv::gl::BlendFunction::ConstantColor;
		case 2: return libv::gl::BlendFunction::DestinationAlpha;
		case 3: return libv::gl::BlendFunction::DestinationColor;
		case 4: return libv::gl::BlendFunction::One;
		case 5: return libv::gl::BlendFunction::One_Minus_ConstantAlpha;
		case 6: return libv::gl::BlendFunction::One_Minus_ConstantColor;
		case 7: return libv::gl::BlendFunction::One_Minus_DestinationAlpha;
		case 8: return libv::gl::BlendFunction::One_Minus_DestinationColor;
		case 9: return libv::gl::BlendFunction::One_Minus_Source1Alpha;
		case 10: return libv::gl::BlendFunction::One_Minus_Source1Color;
		case 11: return libv::gl::BlendFunction::One_Minus_SourceAlpha;
		case 12: return libv::gl::BlendFunction::One_Minus_SourceColor;
		case 13: return libv::gl::BlendFunction::Source1Alpha;
		case 14: return libv::gl::BlendFunction::Source1Color;
		case 15: return libv::gl::BlendFunction::SourceAlpha;
		case 16: return libv::gl::BlendFunction::SourceAlphaSaturate;
		case 17: return libv::gl::BlendFunction::SourceColor;
		case 18: return libv::gl::BlendFunction::Zero;
		}
		assert(false && "Invalid BlendFunction state value");
		return libv::gl::BlendFunction::SourceAlpha;
	}

	constexpr inline void mask_blendDst() noexcept {
		blendDst_ = sizeToBits(sizeBlendDst);
	}
	// No set() alias for blendDst
	constexpr inline void blendDst(libv::gl::BlendFunction blendDst) noexcept {
		switch (blendDst) {
		case libv::gl::BlendFunction::ConstantAlpha: blendDst_ = 0; return;
		case libv::gl::BlendFunction::ConstantColor: blendDst_ = 1; return;
		case libv::gl::BlendFunction::DestinationAlpha: blendDst_ = 2; return;
		case libv::gl::BlendFunction::DestinationColor: blendDst_ = 3; return;
		case libv::gl::BlendFunction::One: blendDst_ = 4; return;
		case libv::gl::BlendFunction::One_Minus_ConstantAlpha: blendDst_ = 5; return;
		case libv::gl::BlendFunction::One_Minus_ConstantColor: blendDst_ = 6; return;
		case libv::gl::BlendFunction::One_Minus_DestinationAlpha: blendDst_ = 7; return;
		case libv::gl::BlendFunction::One_Minus_DestinationColor: blendDst_ = 8; return;
		case libv::gl::BlendFunction::One_Minus_Source1Alpha: blendDst_ = 9; return;
		case libv::gl::BlendFunction::One_Minus_Source1Color:  blendDst_ = 10; return;
		case libv::gl::BlendFunction::One_Minus_SourceAlpha:  blendDst_ = 11; return;
		case libv::gl::BlendFunction::One_Minus_SourceColor:  blendDst_ = 12; return;
		case libv::gl::BlendFunction::Source1Alpha:  blendDst_ = 13; return;
		case libv::gl::BlendFunction::Source1Color:  blendDst_ = 14; return;
		case libv::gl::BlendFunction::SourceAlpha:  blendDst_ = 15; return;
		case libv::gl::BlendFunction::SourceAlphaSaturate:  blendDst_ = 16; return;
		case libv::gl::BlendFunction::SourceColor:  blendDst_ = 17; return;
		case libv::gl::BlendFunction::Zero:  blendDst_ = 18; return;
		}
		assert(false && "Invalid BlendFunction enum value");
	}
	[[nodiscard]] constexpr inline libv::gl::BlendFunction blendDst() const noexcept {
		switch (blendDst_) {
		case 0: return libv::gl::BlendFunction::ConstantAlpha;
		case 1: return libv::gl::BlendFunction::ConstantColor;
		case 2: return libv::gl::BlendFunction::DestinationAlpha;
		case 3: return libv::gl::BlendFunction::DestinationColor;
		case 4: return libv::gl::BlendFunction::One;
		case 5: return libv::gl::BlendFunction::One_Minus_ConstantAlpha;
		case 6: return libv::gl::BlendFunction::One_Minus_ConstantColor;
		case 7: return libv::gl::BlendFunction::One_Minus_DestinationAlpha;
		case 8: return libv::gl::BlendFunction::One_Minus_DestinationColor;
		case 9: return libv::gl::BlendFunction::One_Minus_Source1Alpha;
		case 10: return libv::gl::BlendFunction::One_Minus_Source1Color;
		case 11: return libv::gl::BlendFunction::One_Minus_SourceAlpha;
		case 12: return libv::gl::BlendFunction::One_Minus_SourceColor;
		case 13: return libv::gl::BlendFunction::Source1Alpha;
		case 14: return libv::gl::BlendFunction::Source1Color;
		case 15: return libv::gl::BlendFunction::SourceAlpha;
		case 16: return libv::gl::BlendFunction::SourceAlphaSaturate;
		case 17: return libv::gl::BlendFunction::SourceColor;
		case 18: return libv::gl::BlendFunction::Zero;
		}
		assert(false && "Invalid BlendFunction state value");
		return libv::gl::BlendFunction::One_Minus_SourceAlpha;
	}

	constexpr inline void mask_clipPlanes() noexcept {
		clipPlanes_ = sizeToBits(sizeClipPlanes);
	}
	constexpr inline void clipPlanes(uint32_t count) noexcept {
		switch (count) {
		case 0: clipPlanes_ = 0; return;
		case 1: clipPlanes_ = 1; return;
		case 2: clipPlanes_ = 2; return;
		case 3: clipPlanes_ = 3; return;
		case 4: clipPlanes_ = 4; return;
		case 5: clipPlanes_ = 5; return;
		case 6: clipPlanes_ = 6; return;
		}
		assert(false && "Invalid clipPlanes value");
	}
	[[nodiscard]] constexpr inline uint32_t clipPlanes() const noexcept {
		assert(clipPlanes_ <= 6 && "Invalid clipPlanes state value");
		return clipPlanes_;
	}

public:
	void gl_apply(libv::GL& gl) const noexcept;
	void gl_applyDepth(libv::GL& gl) const noexcept;
};
static_assert(sizeof(RenderState) == sizeof(RenderState::type), "libv::re::RenderState layout error");

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
