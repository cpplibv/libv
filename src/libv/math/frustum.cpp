// Project: libv.math, File: src/libv/math/frustum.cpp

// hpp
#include <libv/math/frustum.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

frustum::frustum(libv::vec3f nbl_, libv::vec3f nbr_, libv::vec3f ntr_, libv::vec3f ntl_,
		libv::vec3f fbl_, libv::vec3f fbr_, libv::vec3f ftr_, libv::vec3f ftl_) noexcept {
	planes_[left] = plane::from_three_point(nbl_, fbl_, ftl_);
	planes_[right] = plane::from_three_point(nbr_, ftr_, fbr_);
	planes_[up] = plane::from_three_point(ntl_, ftl_, ftr_);
	planes_[down] = plane::from_three_point(nbl_, fbr_, fbl_);
	planes_[near] = plane::from_three_point(ntr_, nbl_, ntl_);
	planes_[far] = plane::from_three_point(ftr_, ftl_, fbl_);

	corners_[nbl] = nbl_;
	corners_[nbr] = nbr_;
	corners_[ntr] = ntr_;
	corners_[ntl] = ntl_;
	corners_[fbl] = fbl_;
	corners_[fbr] = fbr_;
	corners_[ftr] = ftr_;
	corners_[ftl] = ftl_;
}

//frustum::frustum(libv::vec3f eye, libv::vec3f fbl_, libv::vec3f fbr_, libv::vec3f ftr_, libv::vec3f ftl_, float near_) noexcept {
//	planes[left] = Plane(eye, fbr_, fbl_);
//	planes[right] = Plane(eye, ftl_, ftr_);
//	planes[up] = Plane(eye, fbl_, ftl_);
//	planes[down] = Plane(eye, ftr_, fbr_);
//	//planes[near] = Plane(...);
//	planes[far] = Plane(fbl_, fbr_, ftr_);
//
//
//	corners[fbl] = fbl_;
//	corners[fbr] = fbr_;
//	corners[ftr] = ftr_;
//	corners[ftl] = ftl_;
//
//	const auto near_n = -planes[far].normal;
//	const auto near_p = (eye + near_n * near_);
//	planes[near] = Plane(near_n, near_p);
//
//
//	corners[nbl] = nbl_;
//	corners[nbr] = nbr_;
//	corners[ntr] = ntr_;
//	corners[ntl] = ntl_;
////	planes[near] = Plane(
////			eye + libv::normalize(b - eye) * near_,
////			eye + libv::normalize(a - eye) * near_,
////			eye + libv::normalize(c - eye) * near_
////	);
//}

// -------------------------------------------------------------------------------------------------

const std::array<libv::vec3f, 8>& frustum::corners() const noexcept {
	return corners_;
}

const std::array<frustum::plane, 6>& frustum::planes() const noexcept {
	return planes_;
}

// -------------------------------------------------------------------------------------------------

frustum::position frustum::point_in_frustum(vec3f point) const noexcept {
	const auto left_dist = planes_[left].distance_from_point(point);
	if (left_dist < 0)
		return position::outside;
	const auto right_dist = planes_[right].distance_from_point(point);
	if (right_dist < 0)
		return position::outside;
	const auto up_dist = planes_[up].distance_from_point(point);
	if (up_dist < 0)
		return position::outside;
	const auto down_dist = planes_[down].distance_from_point(point);
	if (down_dist < 0)
		return position::outside;
	const auto near_dist = planes_[near].distance_from_point(point);
	if (near_dist < 0)
		return position::outside;
	const auto far_dist = planes_[far].distance_from_point(point);
	if (far_dist < 0)
		return position::outside;

	return position::inside;
}

frustum::position frustum::sphere_in_frustum(vec3f point, float radius) const noexcept {
	const auto left_dist = planes_[left].distance_from_point(point);
	if (left_dist < -radius)
		return position::outside;
	const auto right_dist = planes_[right].distance_from_point(point);
	if (right_dist < -radius)
		return position::outside;
	const auto up_dist = planes_[up].distance_from_point(point);
	if (up_dist < -radius)
		return position::outside;
	const auto down_dist = planes_[down].distance_from_point(point);
	if (down_dist < -radius)
		return position::outside;
	const auto near_dist = planes_[near].distance_from_point(point);
	if (near_dist < -radius)
		return position::outside;
	const auto far_dist = planes_[far].distance_from_point(point);
	if (far_dist < -radius)
		return position::outside;

	if (left_dist < radius)
		return position::intersect;
	if (right_dist < radius)
		return position::intersect;
	if (up_dist < radius)
		return position::intersect;
	if (down_dist < radius)
		return position::intersect;
	if (near_dist < radius)
		return position::intersect;
	if (far_dist < radius)
		return position::intersect;

	return position::inside;
}

// =================================================================================================

frustum_culler_inf::frustum_culler_inf(planef left_, planef right_, planef up_, planef down_, planef near_) noexcept {
	planes_[left] = left_;
	planes_[right] = right_;
	planes_[up] = up_;
	planes_[down] = down_;
	planes_[near] = near_;
	// planes_[far] = far_;
}

bool frustum_culler_inf::test_point(vec3f point) const noexcept {
	if (planes_[left].distance_from_point(point) < 0)
		return false;
	if (planes_[right].distance_from_point(point) < 0)
		return false;
	if (planes_[up].distance_from_point(point) < 0)
		return false;
	if (planes_[down].distance_from_point(point) < 0)
		return false;
	if (planes_[near].distance_from_point(point) < 0)
		return false;
	// if (planes_[far].distance_from_point(point) < 0)
	// 	return false;

	return true;
}

bool frustum_culler_inf::test_sphere(vec3f position, float radius) const noexcept {
	if (planes_[left].distance_from_point(position) < -radius)
		return false;
	if (planes_[right].distance_from_point(position) < -radius)
		return false;
	if (planes_[up].distance_from_point(position) < -radius)
		return false;
	if (planes_[down].distance_from_point(position) < -radius)
		return false;
	if (planes_[near].distance_from_point(position) < -radius)
		return false;
	// if (planes_[far].distance_from_point(position) < -radius)
	// 	return false;

	return true;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv

// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================

// "frustum from projection view matrix"
// https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
//
// comboMatrix = projection_matrix * Matrix4_Transpose(modelview_matrix)
//
//void extract_planes_from_projmat(
//        const float mat[4][4],
//        float left[4], float right[4],
//        float bottom[4], float top[4],
//        float near[4], float far[4])
//{
//    for (int i = 4; i--; ) left[i]   = mat[i][3] + mat[i][0];
//    for (int i = 4; i--; ) right[i]  = mat[i][3] - mat[i][0];
//    for (int i = 4; i--; ) bottom[i] = mat[i][3] + mat[i][1];
//    for (int i = 4; i--; ) top[i]    = mat[i][3] - mat[i][1];
//    for (int i = 4; i--; ) near[i]   = mat[i][3] + mat[i][2];
//    for (int i = 4; i--; ) far[i]    = mat[i][3] - mat[i][2];
//}

// =================================================================================================

//// Source: https://www.youtube.com/watch?v=o6zDfDkOFIc
//
//struct Foo {
//	 private static final float OFFSET = 10;
//	 private static final Vector4f UP = new Vector4f(0, 1, 0, 0);
//	 private static final Vector4f FORWARD = new Vector4f(0, 0, -1, 0);
//	 private static final float SHADOW_DISTANCE = 100;
//
//	 private float minX, maxX;
//	 private float minY, maxY;
//	 private float minZ, maxZ;
//	 private Matrix4f lightViewMatrix;
//	 private Camera cam;
//
//	 Update() {
//		 Matrix4f rotation = calculateCameraRotationMatrix();
//		 Vector3f forwardVector = new Vector3f(Matrix4f.transform(rotation, FORWARD, null));
//
//		 Vector3f toFar = new Vector3f(forwardVector);
//		 toFar.scale(SHADOW_DISTANCE);
//		 Vector3f toNear = new Vector3f(forwardVector);
//		 toNear.scale(MasterRenderer.NEAR_PLANE);
//		 Vector3f centerNear = Vector3f.add(toNear, cam.getPosition(), null);
//		 Vector3f centerFar = Vector3f.add(toFar, cam.getPosition(), null);
//
//		 Vector4f[] points = calculateFrustumVertices(rotation, forwardVector, centerNear, centerFar);
//
//		 boolean first = true;
//		 for (Vector4f point : points) {
//			 if (first) {
//				 minX = point.x;
//				 maxX = point.x;
//				 minY = point.y;
//				 maxY = point.y;
//				 minZ = point.z;
//				 maxZ = point.z;
//				 first = false;
//				 continue;
//			 }
//			 if (point.x > maxX) {
//				 maxX = point.x;
//			 } else if (point.x < minX) {
//				 minX = point.x;
//			 }
//			 if (point.y > maxY) {
//				 maxY = point.y;
//			 } else if (point.y < minY) {
//				 minY = point.y;
//			 }
//			 if (point.z > maxZ) {
//				 maxZ = point.z;
//			 } else if (point.z < minZ) {
//				 minZ = point.z;
//			 }
//		 }
//		 maxZ += OFFSET;
//	 }
//
//	 private Matrix4f calculateCameraRotationMatrix() {
//		 Matrix4f rotation = new Matrix4f();
//		 rotation.rotate((float) Math.toradians(-cam.getYaw()), new Vector3f(0, 1, 0));
//		 rotation.rotate((float) Math.toradians(-cam.getPitch()), new Vector3f(1, 0, 0));
//		 return rotation;
//	 }
//
//	 Vector4f[] calculateFrustumVertices(Matrix4f rotation, Vector3f forwardVector,
//				 Vector3f centerNear, Vector3f centerFar) {
//		 Vector3f upVector = new Vector3f(Matrix4f.transform(rotation, UP, null));
//		 Vector3f rightVector = Vector3f.cross(forwardVector, upVector, null);
//		 Vector3f downVector = new Vector3f(-upVector.x, -upVector.y, -upVector.z);
//		 Vector3f leftVector = new Vector3f(-rightVector.x, -rightVector.y, -rightVector.z);
//		 Vector3f farTop = Vector3f.add(centerFar, new Vector3f(upVector.x * farHeight,
//				 upVector.y * farHeight, upVector.z * farHeight), null);
//		 Vector3f farBottom = Vector3f.add(centerFar, new Vector3f(downVector.x * farHeight,
//				 downVector.y * farHeight, downVector.z * farHeight), null);
//		 Vector3f nearTop = Vector3f.add(centerNear, new Vector3f(upVector.x * nearHeight,
//				 upVector.y * nearHeight, upVector.z * nearHeight), null);
//		 Vector3f nearBottom = Vector3f.add(centerNear, new Vector3f(downVector.x * nearHeight,
//				 downVector.y * nearHeight, downVector.z * nearHeight), null);
//		 Vector4f[] points = new Vector4f[8];
//		 points[0] = calculateLightSpaceFrustumCorner(farTop, rightVector, farWidth);
//		 points[1] = calculateLightSpaceFrustumCorner(farTop, leftVector, farWidth);
//		 points[2] = calculateLightSpaceFrustumCorner(farBottom, rightVector, farWidth);
//		 points[3] = calculateLightSpaceFrustumCorner(farBottom, leftVector, farWidth);
//		 points[4] = calculateLightSpaceFrustumCorner(nearTop, rightVector, nearWidth);
//		 points[5] = calculateLightSpaceFrustumCorner(nearTop, leftVector, nearWidth);
//		 points[6] = calculateLightSpaceFrustumCorner(nearBottom, rightVector, nearWidth);
//		 points[7] = calculateLightSpaceFrustumCorner(nearBottom, leftVector, nearWidth);
//		 return points;
//	 }
//
//	 protected Vector3f getCenter() {
//		 float x = (minX + maxX) / 2f;
//		 float y = (minY + maxY) / 2f;
//		 float z = (minZ + maxZ) / 2f;
//		 Vector4f cen = new Vector4f(x, y, z, 1);
//		 Matrix4f invertedLight = new Matrix4f();
//		 Matrix4f.invert(lightViewMatrix, invertedLight);
//		 return new Vector3f(Matrix4f.transform(invertedLight, cen, null));
//	 }
//};
