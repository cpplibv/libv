// Project: libv.re, File: src/libv/re/common/texture_unit.hpp

#pragma once

#include <libv/gl/enum.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

using TextureUnit = libv::gl::TextureUnit; // Convenience alias

static constexpr TextureUnit unitDiffuse{0};
static constexpr TextureUnit unitBaseColor = unitDiffuse;
static constexpr TextureUnit unitNormal{1};
static constexpr TextureUnit unitOutput = unitNormal;
static constexpr TextureUnit unitSpecular{2};
static constexpr TextureUnit unitRMAO = unitSpecular;
static constexpr TextureUnit unitEmission{3};
static constexpr TextureUnit unitEnvironment{4};
static constexpr TextureUnit unitAmbient{5};
static constexpr TextureUnit unitHeight{6};
static constexpr TextureUnit unitShadow{7};
static constexpr TextureUnit unitSky{8};
static constexpr TextureUnit unitDepth{9};
// static constexpr TextureUnit unit{10};
// static constexpr TextureUnit unit{11};
// static constexpr TextureUnit unit{12};
// static constexpr TextureUnit unit{13};
// static constexpr TextureUnit unit{14};
// static constexpr TextureUnit unit{15};
static constexpr TextureUnit unitStencil{15}; // Basically debug only usage

// Jump Flood Algorithm
static constexpr TextureUnit unitJFAMask{0};
static constexpr TextureUnit unitJFAStep{1};
static constexpr TextureUnit unitJFAStepOutput{2};

// Post-processing
static constexpr TextureUnit unitPPScene{0};
static constexpr TextureUnit unitPPBloomPyramid{1};
static constexpr TextureUnit unitPPBloomPyramidOutput{2};
static constexpr TextureUnit unitPPVignette{3};
static constexpr TextureUnit unitPPBloomDirt{4};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
