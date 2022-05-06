#pragma once


const vec4 qidentity = vec4(0, 0, 0, 1); /// X Y Z W

// -------------------------------------------------------------------------------------------------

vec4 qneg(const vec4 a) {
	return -a;
}

vec4 qadd(const vec4 lhs, const vec4 rhs) {
	return lhs + rhs;
}

vec4 qsub(const vec4 lhs, const vec4 rhs) {
	return lhs - rhs;
}

vec4 qmul(const vec4 lhs, const float rhs) {
	return lhs * rhs;
}

vec4 qmul(const vec4 lhs, const vec4 rhs) {
	return vec4(
			lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y, // X
			lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z, // Y
			lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x, // Z
			lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z  // W
	);
}

vec4 qdiv(const vec4 lhs, const float rhs) {
	return lhs * (1.0 / rhs);
}

// -------------------------------------------------------------------------------------------------

float qlength(const vec4 a) {
	return length(a);
}

float qlength_sq(const vec4 a) {
	return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}

// -------------------------------------------------------------------------------------------------

float qdot(const vec4 lhs, const vec4 rhs) {
	return dot(lhs, rhs);
}

vec4 qcross(const vec4 lhs, const vec4 rhs) {
	return vec4(
			lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y, // X
			lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z, // Y
			lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x, // Z
			lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z  // W
	);
}

vec4 qconj(const vec4 a) {
	return vec4(-a.xyz, a.w);
}

vec4 qinverse(const vec4 a) {
	return qconj(a) * (1.0 / qlength_sq(a));
}

vec4 qnormalize(const vec4 a) {
	float len = qlength(a);
	return a * (1.0 / len);
}

// -------------------------------------------------------------------------------------------------

/// @param vec - unit vector
vec3 qrotate(const vec4 quat, const vec3 vec) {
	const vec3 quatVector = quat.xyz;
	const vec3 uv = cross(quatVector, vec);
	const vec3 uuv = cross(quatVector, uv);

	return vec + ((uv * quat.w) + uuv) * 2.0;
}

//vec3 qrotate(vec3 vec, vec4 quat) {
// return qmul(qmul(quat, vec4(p, 0.0)), qinverse(quat)).xyz;
//	return qmul(qmul(quat, vec4(p, 0.0)), qconj(quat)).xyz;
//}

// -------------------------------------------------------------------------------------------------

/// @param start - unit vector
/// @param dest - unit vector
vec4 qfromUnitVectors(const vec3 start, const vec3 dest) {
	const float m = sqrt(2.0 + 2.0 * dot(start, dest));
	const vec3 w = (1.0 / m) * cross(start, dest);
	return vec4(w.x, w.y, w.z, 0.5 * m);
}

/// @param start - not required to be a unit vector
/// @param dest - not required to be a unit vector
vec4 qfromNonUnitVectors(const vec3 start, const vec3 dest) {
	const vec3 w = cross(start, dest);
	vec4 q = vec4(w.x, w.y, w.z, dot(start, dest));
	q.w += qlength(q);
	return qnormalize(q);
}

/// A given angle of rotation about a given axis
/// @param axis - unit vector
vec4 qAngleAxis(float angle, vec3 axis) {
	float sn = sin(angle * 0.5);
	float cs = cos(angle * 0.5);
	return vec4(axis * sn, cs);
}

// -------------------------------------------------------------------------------------------------

//vec4 qlookat(vec3 forward, vec3 up);
//vec4 qslerp(vec4 a, vec4 b, float t);
