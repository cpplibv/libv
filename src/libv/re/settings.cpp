// Project: libv.re, File: src/libv/re/settings.cpp

#include <fmt/format.h>

#include <libv/re/common/attribute.hpp>
#include <libv/re/common/texture_unit.hpp>
#include <libv/re/node/light/light_type.hpp>
#include <libv/re/settings.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

ResourceLoaderSettings::ResourceLoaderSettings(std::string basePath) {
	resourceMappings.emplace_back("", std::move(basePath));
}

ResourceLoaderSettingsShader::ResourceLoaderSettingsShader(std::string basePath) :
	ResourceLoaderSettings(std::move(basePath)) {

	fswReloadDelay = std::chrono::milliseconds(10);

	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_POSITION {}", attrPosition.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_UV {}", attrUV.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_NORMAL {}", attrNormal.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_TANGENT {}", attrTangent.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_BITANGENT {}", attrBitangent.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_BONE_INDICES {}", attrBoneIndices.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_BOND_WEIGHT {}", attrBondWeight.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_DIFFUSE {}", attrDiffuse.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_SPECULAR {}", attrSpecular.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_INSTANCE_MAT_ROW0 {}", attrInstanceMatRow0.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_INSTANCE_MAT_ROW1 {}", attrInstanceMatRow1.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_INSTANCE_MAT_ROW2 {}", attrInstanceMatRow2.channel));
	// globalDefines.emplace_back(fmt::format("VERTEX_ATTR_INSTANCE_VEC0 {}", attrInstanceVec0.channel));
	// globalDefines.emplace_back(fmt::format("VERTEX_ATTR_INSTANCE_VEC1 {}", attrInstanceVec1.channel));
	// globalDefines.emplace_back(fmt::format("VERTEX_ATTR_TESS_FACTOR {}", attrTessFactor.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_CUSTOM1 {}", attrCustom1.channel));
	globalDefines.emplace_back(fmt::format("VERTEX_ATTR_CUSTOM0 {}", attrCustom0.channel));

	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_DIFFUSE {}", +unitDiffuse));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_BASECOLOR {}", +unitBaseColor));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_NORMAL {}", +unitNormal));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_SPECULAR {}", +unitSpecular));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_RMAO {}", +unitRMAO));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_EMISSION {}", +unitEmission));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_ENVIRONMENT {}", +unitEnvironment));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_AMBIENT {}", +unitAmbient));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_HEIGHT {}", +unitHeight));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_SHADOW {}", +unitShadow));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_SKY {}", +unitSky));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_DEPTH {}", +unitDepth));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_OUTPUT {}", +unitOutput));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_STENCIL {}", +unitStencil));

	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_JFA_MASK {}", +unitJFAMask));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_JFA_STEP {}", +unitJFAStep));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_JFA_STEP_OUTPUT {}", +unitJFAStepOutput));

	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_PP_SCENE {}", +unitPPScene));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_PP_BLOOM_PYRAMID {}", +unitPPBloomPyramid));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_PP_BLOOM_PYRAMID_OUTPUT {}", +unitPPBloomPyramidOutput));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_PP_VIGNETTE {}", +unitPPVignette));
	globalDefines.emplace_back(fmt::format("TEXTURE_UNIT_PP_BLOOM_DIRT {}", +unitPPBloomDirt));

	globalDefines.emplace_back(fmt::format("LIGHT_ATTENUATION_FORMULA_PHYSICAL {}", +LightAttenuationFormula::physical));
	globalDefines.emplace_back(fmt::format("LIGHT_ATTENUATION_FORMULA_LISYARUS {}", +LightAttenuationFormula::lisyarus));
	globalDefines.emplace_back(fmt::format("LIGHT_ATTENUATION_FORMULA_LISYARUS_SHARP {}", +LightAttenuationFormula::lisyarusSharp));

	// Usages in shaders:
	//
	// layout(location = VERTEX_ATTR_POSITION) in vec3 vertexPosition;
	// layout(location = VERTEX_ATTR_UV) in vec2 vertexUV;
	// layout(location = VERTEX_ATTR_NORMAL) in vec3 vertexNormal;
	// layout(location = VERTEX_ATTR_TANGENT) in vec3 vertexTangent;
	// layout(location = VERTEX_ATTR_BITANGENT) in vec3 vertexBitangent;
	// layout(location = VERTEX_ATTR_BONE_INDICES) in ivec4 vertexBoneIndices;
	// layout(location = VERTEX_ATTR_BOND_WEIGHT) in vec4 vertexBondWeight;
	// layout(location = VERTEX_ATTR_DIFFUSE) in vec4 vertexDiffuse;
	// layout(location = VERTEX_ATTR_SPECULAR) in vec4 vertexSpecular;
	// layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW0) in vec4 vertexInstanceMatRow0;
	// layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW1) in vec4 vertexInstanceMatRow1;
	// layout(location = VERTEX_ATTR_INSTANCE_MAT_ROW2) in vec4 vertexInstanceMatRow2;
	// layout(location = VERTEX_ATTR_INSTANCE_VEC0) in vec4 vertexInstanceVec0;
	// layout(location = VERTEX_ATTR_INSTANCE_VEC1) in vec4 vertexInstanceVec1;
	// layout(location = VERTEX_ATTR_TESS_FACTOR) in vec4 vertexTessFactor;
	// layout(location = VERTEX_ATTR_INSTANCE_MAT) in mat4 vertexInstanceMat;
	//
	// layout(binding = TEXTURE_UNIT_DIFFUSE) uniform sampler2D textureDiffuse;
	// layout(binding = TEXTURE_UNIT_NORMAL) uniform sampler2D textureNormal;
	// layout(binding = TEXTURE_UNIT_SPECULAR) uniform sampler2D textureSpecular;
	// layout(binding = TEXTURE_UNIT_EMISSION) uniform sampler2D textureEmission;
	// layout(binding = TEXTURE_UNIT_ENVIRONMENT) uniform sampler2D textureEnvironment;
	// layout(binding = TEXTURE_UNIT_AMBIENT) uniform sampler2D textureAmbient;
	// layout(binding = TEXTURE_UNIT_HEIGHT) uniform sampler2D textureHeight;
	// layout(binding = TEXTURE_UNIT_SHADOW) uniform sampler2DShadow textureShadow;
	// layout(binding = TEXTURE_UNIT_SKY) uniform samplerCube textureSky;
	// layout(binding = TEXTURE_UNIT_DEPTH) uniform sampler2D textureDepth;
}

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
