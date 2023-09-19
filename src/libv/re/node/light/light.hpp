// Project: libv.re, File: src/libv/re/node/light/light.hpp

#pragma once

#include <libv/math/vec.hpp>
#include <libv/re/fwd.hpp>
#include <libv/re/node/light/light_type.hpp>
#include <libv/re/node/node.hpp>


namespace libv {
namespace re {

// -------------------------------------------------------------------------------------------------

class Light : public Node {
public:
	using ptr = Light_ptr;
	using cptr = Light_cptr;

private: // TODO P5: getters setters?
public:
	LightType type = LightType::point; // Point = 1, Spot = 2, Directional = 3

	// positionW are derived from node.transform
	// directionW are derived from node.transform

	libv::vec3f color = {1.f, 1.f, 1.f};
	float intensity = 1.f;

	float cosAngleInner = 0.0f; /// Spot light only
	float cosAngleOuter = 0.0f; /// Spot light only

	LightAttenuationFormula attenuationFormula = LightAttenuationFormula::lisyarus; /// Point and Spot light only
	float range = 10.0f; /// Point and Spot light only
	float falloff = 1.0f; /// Point and Spot light only

public:
	inline Light() :
		Node(NodeType::light) {
	}
	explicit inline Light(LightType type) :
		Node(NodeType::light),
		type(type) {
	}
	explicit inline Light(const Transform& transform) :
		Node(NodeType::light, transform) {
	}
	inline Light(const Transform& transform, LightType type) :
		Node(NodeType::light, transform),
		type(type) {
	}
	virtual ~Light() = default;

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}

public:
	virtual void queue(ContextQueue& ctx) override;
	virtual void queueOverride(ContextQueue& ctx, ContextQueueOverride& ov) override;
	// [[nodiscard]] virtual libv::mat4f projection(libv::vec2f canvasSize) const;
	// [[nodiscard]] virtual libv::mat4f projectionRevereZ(libv::vec2f canvasSize) const;
	// [[nodiscard]] virtual libv::mat4f projectionRevereZInf(libv::vec2f canvasSize) const;
	// [[nodiscard]] virtual libv::mat4f view() const;
	// [[nodiscard]] virtual float near() const;
	// [[nodiscard]] virtual float far() const;

	//libv::mat4f Light::getPmat() {
	//	if (type == LightType::dir)
	//		return libv::mat4f::ortho(-90, 90, -90, 90, -90, 90);
	//	else if (type == LightType::point)
	//		return libv::mat4f::ortho(-30, 30, -30, 30, -10, 150);
	//	else if (type == LightType::spot)
	//		return libv::mat4f::perspective(std::acos(outerCosAngle) * 180.0f / libv::pi_v<float> * 2.0f, 1.0f, range / 15.0f, range);
	//	else
	//		assert(false);
	//}
	//
	//libv::mat4f Light::getVmat() {
	//	if (type == LightType::dir)
	//		return libv::mat4f::lookAt(libv::vec3f(0, 0, 0), direction, libv::vec3f(0, 1, 0));
	//	else if (type == LightType::point)
	//		return libv::mat4f::ortho(-30, 30, -30, 30, -10, 150);
	//	else if (type == LightType::spot)
	//		return libv::mat4f::lookAt(position, position + direction, libv::vec3f(0, 1, 0));
	//	else
	//		assert(false);
	//}
};

class LightDirectional : public Light {
public:
	using ptr = LightDirectional_ptr;
	using cptr = LightDirectional_cptr;

public:
	inline LightDirectional() :
		Light(LightType::directional) {
	}
	explicit inline LightDirectional(
			const Transform& transform,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) :
		Light(transform, LightType::directional) {
		this->color = color;
		this->intensity = intensity;
	}
	explicit inline LightDirectional(
			libv::vec3f direction,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) :
		Light(LightType::directional) {
		this->transform.rotation = libv::quatf::look_at(direction.normalize_copy(), libv::vec3f{0, 0, 1});
		this->color = color;
		this->intensity = intensity;
	}

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(
			const Transform& transform,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) {
		return ptr::make(transform, color, intensity);
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(
			libv::vec3f direction,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) {
		return ptr::make(direction, color, intensity);
	}
};

class LightPoint : public Light {
public:
	using ptr = LightPoint_ptr;
	using cptr = LightPoint_cptr;

public:
	inline LightPoint() :
		Light(LightType::point) {
	}
	explicit inline LightPoint(
			const Transform& transform,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) :
		Light(transform, LightType::point) {
		this->color = color;
		this->intensity = intensity;
	}
	explicit inline LightPoint(
			libv::vec3f position,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) :
		Light(LightType::point) {
		this->transform.position = position;
		this->color = color;
		this->intensity = intensity;
	}

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(
			const Transform& transform,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) {
		return ptr::make(transform, color, intensity);
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(
			libv::vec3f position,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) {
		return ptr::make(position, color, intensity);
	}
};

class LightSpot : public Light {
public:
	using ptr = LightSpot_ptr;
	using cptr = LightSpot_cptr;

public:
	inline LightSpot() :
		Light(LightType::spot) {
	}
	explicit inline LightSpot(
			const Transform& transform,
			float cosAngleInner = 0.8f,
			float cosAngleOuter = 0.6f,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) :
		Light(transform, LightType::spot) {
		this->cosAngleInner = cosAngleInner;
		this->cosAngleOuter = cosAngleOuter;
		this->color = color;
		this->intensity = intensity;
	}
	explicit inline LightSpot(
			libv::vec3f position,
			libv::vec3f direction,
			float cosAngleInner = 0.8f,
			float cosAngleOuter = 0.6f,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) :
		Light(LightType::spot) {
		this->transform.position = position;
		this->transform.rotation = libv::quatf::look_at(direction.normalize_copy(), libv::vec3f{0, 0, 1});
		this->cosAngleInner = cosAngleInner;
		this->cosAngleOuter = cosAngleOuter;
		this->color = color;
		this->intensity = intensity;
	}

	[[nodiscard]] static LIBV_FORCE_INLINE ptr create() {
		return ptr::make();
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(
			const Transform& transform,
			float cosAngleInner = 0.8f,
			float cosAngleOuter = 0.6f,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) {
		return ptr::make(transform, cosAngleInner, cosAngleOuter, color, intensity);
	}
	[[nodiscard]] static LIBV_FORCE_INLINE ptr create(
			libv::vec3f position,
			libv::vec3f direction,
			float cosAngleInner = 0.8f,
			float cosAngleOuter = 0.6f,
			libv::vec3f color = libv::vec3f{1, 1, 1},
			float intensity = 1.f) {
		return ptr::make(position, direction, cosAngleInner, cosAngleOuter, color, intensity);
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace re
} // namespace libv
