// Project: libv.re, File: src/libv/re/uniform/builtin.hpp

#pragma once

#include <libv/math/mat.hpp>
#include <libv/math/vec.hpp>
#include <libv/re/common/block_binding.hpp>
#include <libv/re/node/light/light_type.hpp>
#include <libv/re/uniform/padding.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

struct BlockModelMatrices {
	// Packed std140
	static constexpr BlockBinding binding = BlockBinding::model;
	static constexpr std::string_view name = builtinBlockNameModel;

	libv::mat4f matMV;
	libv::mat4f matM;
	libv::mat4f matMInverse;

	// === 192 Byte

	// libv::mat3f matN; // inverse transpose of matMV (might not be necessary)

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(matMV, "matMV");
		access(matM, "matM");
		access(matMInverse, "matMInverse");
	}
};

struct BlockRenderPass {
	// Packed std140
	static constexpr BlockBinding binding = BlockBinding::pass;
	static constexpr std::string_view name = builtinBlockNamePass;

	libv::mat4f matP;            /// Current projection matrix
	// libv::mat4f matPinv;      /// Inverse of the current projection matrix
	libv::mat4f matV;            /// Current view matrix
	libv::mat4f matVP;           /// Current view * projection matrix
	libv::mat4f matVPinv;        /// Inverse of the current view * projection matrix

	libv::vec3f eyeW;            /// World space position of the camera
	float timeSimLooping;        /// Simulation time in seconds looping between -1800 and 1800 starting from 0

	libv::vec3f cameraForwardW;
	float timeSimUncapped;       /// Simulation time in seconds

	libv::vec3f cameraRightW;
	float timeRealLooping;       /// Real time in seconds looping between -1800 and 1800 starting from 0

	libv::vec3f cameraUpW;
	float timeRealUncapped;      /// Real time in seconds

	libv::vec2f canvasSize;      /// Render resolution in pixels
	libv::vec2f pixelSize;       /// Pixel size in NDC (Normalized Device Coordinates): 1.0 / canvasSize

	libv::vec2f mousePosition;   /// Mouse position relative to the canvas (can be outside canvas)
	float cameraNear;
	float cameraFar;

	// === 352 Byte

	// float cameraFovx;
	// float cameraFovy;
	// float aspectRatio;
	// Padding<4> _p0;

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(matP, "matP");
		// access(matPinv, "matPinv");
		access(matV, "matV");
		access(matVP, "matVP");
		access(matVPinv, "matVPinv");
		access(eyeW, "eyeW");
		access(timeSimLooping, "timeSimLooping");
		access(cameraForwardW, "cameraForwardW");
		access(timeSimUncapped, "timeSimUncapped");
		access(cameraRightW, "cameraRightW");
		access(timeRealLooping, "timeRealLooping");
		access(cameraUpW, "cameraUpW");
		access(timeRealUncapped, "timeRealUncapped");
		access(canvasSize, "canvasSize");
		access(pixelSize, "pixelSize");
		access(mousePosition, "mousePosition");
		access(cameraNear, "cameraNear");
		access(cameraFar, "cameraFar");
	}

	// Additional stuff that can be found in unity:
	// https://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
	//
	// _ProjectionParams
	// 1.0 / cameraFar
	//
	// _ScreenParams
	// 1.0 + 1.0 / canvasSize.x
	// 1.0 + 1.0 / canvasSize.y
	//
	// _ZBufferParams, Used to linearize Z buffer values:
	// 1.0 - cameraFar / cameraNear
	// cameraFar / cameraNear
	// (1.0 - cameraFar / cameraNear) / cameraFar
	// (cameraFar / cameraNear) / cameraFar
	//
	// unity_OrthoParams,
	// orthographic camera width / height
	// 'float bool' for orthographic or perspective camera
	//
	// unity_CameraWorldClipPlanes[6]
	// Camera frustum plane world space equations, in this order: left, right, bottom, top, near, far.
	//
	// Fog and Ambient
	//
	// Name 	Type 	Value
	// unity_AmbientSky         fixed4 Sky ambient lighting color in gradient ambient lighting case.
	// unity_AmbientEquator     fixed4 Equator ambient lighting color in gradient ambient lighting case.
	// unity_AmbientGround      fixed4 Ground ambient lighting color in gradient ambient lighting case.
	// unity_IndirectSpecColor  fixed4 If you use a skybox, this is the average color of the skybox, which Unity calculates using the DC component of the spherical harmonics data in the ambient probe. Otherwise this is black.
	// UNITY_LIGHTMODEL_AMBIENT fixed4 Ambient lighting color (sky color in gradient ambient case). Legacy variable.
	// unity_FogColor           fixed4 Fog color.
	// unity_FogParams          float4 Parameters for fog calculation:
	// 			(density / sqrt(ln(2)), // useful for Exp2 fog mode
	// 			density / ln(2),        // y for Exp mode
	// 			–1/(end-start),         // Linear mode
	// 			end/(end-start)).       // Linear mode00
};

// -------------------------------------------------------------------------------------------------

struct UniformLightDirectional {
	// Packed std140

	static constexpr std::string_view name = "LightDirectional300";

	libv::vec3f directionW;
	Padding<4> _p0;

	libv::vec3f radiance;
	Padding<4> _p1;

	// === 32 byte

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(directionW, "directionW");
		access(_p0, "_p0");
		access(radiance, "radiance");
		access(_p1, "_p1");
	}
};

struct UniformLightPoint {
	// Packed std140

	static constexpr std::string_view name = "LightPoint300";

	libv::vec3f positionW;
	float range;

	libv::vec3f radiance;
	float falloff;

	LightAttenuationFormula attenuationFormula;
	Padding<12> _p0;

	// === 48 byte

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(positionW, "positionW");
		access(range, "range");
		access(radiance, "radiance");
		access(falloff, "falloff");
		access(attenuationFormula, "attenuationFormula");
		access(_p0, "_p0");
	}
};

struct UniformLightSpot {
	// Packed std140

	static constexpr std::string_view name = "LightSpot300";

	libv::vec3f positionW;
	float range;

	libv::vec3f directionW;
	float cosAngleInner;

	libv::vec3f radiance;
	float cosAngleOuter;

	float falloff;
	LightAttenuationFormula attenuationFormula;
	Padding<8> _p0;

	// === 64 byte

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(positionW, "positionW");
		access(range, "range");
		access(directionW, "directionW");
		access(cosAngleInner, "cosAngleInner");
		access(radiance, "radiance");
		access(cosAngleOuter, "cosAngleOuter");
		access(falloff, "falloff");
		access(attenuationFormula, "attenuationFormula");
		access(_p0, "_p0");
	}
};

//	bool shadowCast = false;
//	uint32_t frameBuffer;
//	uint32_t shadowDepthTexture; //Texture sampler for shadow map. The textureSamplers layout is 10+i where 'i' is the index of the light!
//	uint32_t shadowMapSampler;
//	libv::mat4f shadowMVPTmat; //MVPT mat

struct BlockLights {
	// Packed std140
	static constexpr BlockBinding binding = BlockBinding::lights;
	static constexpr std::string_view name = builtinBlockNameLights;

	uint32_t numLightDirectionals = 0;
	uint32_t numLightPoints = 0;
	uint32_t numLightSpots = 0;
	Padding<4> _p0;
	// libv::vec3f ambientRadiance;
	// Padding<4> _p1;
	std::array<UniformLightDirectional, 4> lightDirectionals;
	std::array<UniformLightPoint, 8> lightPoints;
	std::array<UniformLightSpot, 8> lightSpots;

	// = 1040 byte

	template <typename Access> constexpr void access_uniforms(Access& access) {
		access(numLightDirectionals, "numLightDirectionals");
		access(numLightPoints, "numLightPoints");
		access(numLightSpots, "numLightSpots");
		access(_p0, "_p0");
		// access(ambientRadiance, "ambientRadiance");
		// access(_p1, "_p1");
		access(lightDirectionals, "lightDirectionals");
		access(lightSpots, "lightSpots");
		access(lightPoints, "lightPoints");
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
