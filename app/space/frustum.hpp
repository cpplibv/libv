// Project: libv, File: app/space/space_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/math/vec.hpp>
#include <libv/math/mat.hpp>

// -------------------------------------------------------------------------------------------------

struct Frustum {
	libv::vec3f pos;
	libv::vec3f size;
	libv::vec3f dir;
};

// Source: https://www.youtube.com/watch?v=o6zDfDkOFIc
//
//	private static final float OFFSET = 10;
//	private static final Vector4f UP = new Vector4f(0, 1, 0, 0);
//	private static final Vector4f FORWARD = new Vector4f(0, 0, -1, 0);
//	private static final float SHADOW_DISTANCE = 100;
//
//	private float minX, maxX;
//	private float minY, maxY;
//	private float minZ, maxZ;
//	private Matrix4f lightViewMatrix;
//	private Camera cam;
//
//	Update() {
//		Matrix4f rotation = calculateCameraRotationMatrix();
//		Vector3f forwardVector = new Vector3f(Matrix4f.transform(rotation, FORWARD, null));
//
//		Vector3f toFar = new Vector3f(forwardVector);
//		toFar.scale(SHADOW_DISTANCE);
//		Vector3f toNear = new Vector3f(forwardVector);
//		toNear.scale(MasterRenderer.NEAR_PLANE);
//		Vector3f centerNear = Vector3f.add(toNear, cam.getPosition(), null);
//		Vector3f centerFar = Vector3f.add(toFar, cam.getPosition(), null);
//
//		Vector4f[] points = calculateFrustumVertices(rotation, forwardVector, centerNear,
//				centerFar);
//
//		boolean first = true;
//		for (Vector4f point : points) {
//			if (first) {
//				minX = point.x;
//				maxX = point.x;
//				minY = point.y;
//				maxY = point.y;
//				minZ = point.z;
//				maxZ = point.z;
//				first = false;
//				continue;
//			}
//			if (point.x > maxX) {
//				maxX = point.x;
//			} else if (point.x < minX) {
//				minX = point.x;
//			}
//			if (point.y > maxY) {
//				maxY = point.y;
//			} else if (point.y < minY) {
//				minY = point.y;
//			}
//			if (point.z > maxZ) {
//				maxZ = point.z;
//			} else if (point.z < minZ) {
//				minZ = point.z;
//			}
//		}
//		maxZ += OFFSET;
//	}
//
//	private Matrix4f calculateCameraRotationMatrix() {
//		Matrix4f rotation = new Matrix4f();
//		rotation.rotate((float) Math.toRadians(-cam.getYaw()), new Vector3f(0, 1, 0));
//		rotation.rotate((float) Math.toRadians(-cam.getPitch()), new Vector3f(1, 0, 0));
//		return rotation;
//	}
//
//	Vector4f[] calculateFrustumVertices(Matrix4f rotation, Vector3f forwardVector,
//				Vector3f centerNear, Vector3f centerFar) {
//		Vector3f upVector = new Vector3f(Matrix4f.transform(rotation, UP, null));
//		Vector3f rightVector = Vector3f.cross(forwardVector, upVector, null);
//		Vector3f downVector = new Vector3f(-upVector.x, -upVector.y, -upVector.z);
//		Vector3f leftVector = new Vector3f(-rightVector.x, -rightVector.y, -rightVector.z);
//		Vector3f farTop = Vector3f.add(centerFar, new Vector3f(upVector.x * farHeight,
//				upVector.y * farHeight, upVector.z * farHeight), null);
//		Vector3f farBottom = Vector3f.add(centerFar, new Vector3f(downVector.x * farHeight,
//				downVector.y * farHeight, downVector.z * farHeight), null);
//		Vector3f nearTop = Vector3f.add(centerNear, new Vector3f(upVector.x * nearHeight,
//				upVector.y * nearHeight, upVector.z * nearHeight), null);
//		Vector3f nearBottom = Vector3f.add(centerNear, new Vector3f(downVector.x * nearHeight,
//				downVector.y * nearHeight, downVector.z * nearHeight), null);
//		Vector4f[] points = new Vector4f[8];
//		points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
//		points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
//		points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
//		points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
//		points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
//		points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
//		points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
//		points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
//		return points;
//	}
//
//	protected Vector3f getCenter() {
//		float x = (minX + maxX) / 2f;
//		float y = (minY + maxY) / 2f;
//		float z = (minZ + maxZ) / 2f;
//		Vector4f cen = new Vector4f(x, y, z, 1);
//		Matrix4f invertedLight = new Matrix4f();
//		Matrix4f.invert(lightViewMatrix, invertedLight);
//		return new Vector3f(Matrix4f.transform(invertedLight, cen, null));
//	}
//
//	protected float getWidth() {
//		return maxX - minX;
//	}
//
//	protected float getHeight() {
//		return maxY - minY;
//	}
//
//	protected float getLength() {
//		return maxZ - minZ;
//	}
//

Frustum contain_from_different_dir(Frustum orig, libv::vec3f dir) {
}

auto foo() {
	libv::vec3f sun_dir;

	Frustum camera;
	Frustum shadow_box = contain_from_different_dir(camera, sun_dir);

	libv::mat4f light_matP = libv::mat4f::ortho(shadow_box.getWidth(), shadow_box.getHeight(), shadow_box.getLenght());
	libv::mat4f light_matMV = libv::mat4f::lookAt(sun_dir, {0, 0, 0}, {0, 0, 1});

	const auto light_matMVP = light_matMV * light_matP;
}
