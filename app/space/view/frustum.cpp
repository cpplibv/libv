// Project: libv, File: app/space/view/frustum.cpp

// hpp
#include <space/view/frustum.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

Frustum::Position Frustum::sphereInFrustum(libv::vec3f point, float radius) const noexcept {
	const auto left_dist = planes[left].distance(point);
	if (left_dist < -radius)
		return Position::OUTSIDE;
	const auto right_dist = planes[right].distance(point);
	if (right_dist < -radius)
		return Position::OUTSIDE;
	const auto up_dist = planes[up].distance(point);
	if (up_dist < -radius)
		return Position::OUTSIDE;
	const auto down_dist = planes[down].distance(point);
	if (down_dist < -radius)
		return Position::OUTSIDE;
	const auto near_dist = planes[near].distance(point);
	if (near_dist < -radius)
		return Position::OUTSIDE;
	const auto far_dist = planes[far].distance(point);
	if (far_dist < -radius)
		return Position::OUTSIDE;

	if (left_dist < radius)
		return Position::INTERSECT;
	if (right_dist < radius)
		return Position::INTERSECT;
	if (up_dist < radius)
		return Position::INTERSECT;
	if (down_dist < radius)
		return Position::INTERSECT;
	if (near_dist < radius)
		return Position::INTERSECT;
	if (far_dist < radius)
		return Position::INTERSECT;

	return Position::INSIDE;
}

Frustum::Position Frustum::pointInFrustum(libv::vec3f point) const noexcept {
	const auto left_dist = planes[left].distance(point);
	if (left_dist < 0)
		return Position::OUTSIDE;
	const auto right_dist = planes[right].distance(point);
	if (right_dist < 0)
		return Position::OUTSIDE;
	const auto up_dist = planes[up].distance(point);
	if (up_dist < 0)
		return Position::OUTSIDE;
	const auto down_dist = planes[down].distance(point);
	if (down_dist < 0)
		return Position::OUTSIDE;
	const auto near_dist = planes[near].distance(point);
	if (near_dist < 0)
		return Position::OUTSIDE;
	const auto far_dist = planes[far].distance(point);
	if (far_dist < 0)
		return Position::OUTSIDE;

	return Position::INSIDE;
}

//Frustum::Frustum(libv::vec3f eye, libv::vec3f fbl_, libv::vec3f fbr_, libv::vec3f ftr_, libv::vec3f ftl_, float near_) noexcept {
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

Frustum::Frustum(libv::vec3f nbl_, libv::vec3f nbr_, libv::vec3f ntr_, libv::vec3f ntl_,
		libv::vec3f fbl_, libv::vec3f fbr_, libv::vec3f ftr_, libv::vec3f ftl_) noexcept {
	planes[left] = Plane(nbl_, fbl_, ftl_);
	planes[right] = Plane(nbr_, ftr_, fbr_);
	planes[up] = Plane(ntl_, ftl_, ftr_);
	planes[down] = Plane(nbl_, fbr_, fbl_);
	planes[near] = Plane(ntr_, nbl_, ntl_);
	planes[far] = Plane(ftr_, ftl_, fbl_);

	corners_[nbl] = nbl_;
	corners_[nbr] = nbr_;
	corners_[ntr] = ntr_;
	corners_[ntl] = ntl_;
	corners_[fbl] = fbl_;
	corners_[fbr] = fbr_;
	corners_[ftr] = ftr_;
	corners_[ftl] = ftl_;
//	);
}

//int Frustum::sphereInFrustum(libv::vec3f p, float radius) const noexcept {
//	if (planes[near].distance(p) < -radius)
//		return 0;
//	else if (planes[far].distance(p) < -radius)
//		return 1;
//	else if (planes[left].distance(p) < -radius)
//		return 2;
//	else if (planes[right].distance(p) < -radius)
//		return 3;
//	else if (planes[up].distance(p) < -radius)
//		return 4;
//	else if (planes[down].distance(p) < -radius)
//		return 5;
//	else if (planes[near].distance(p) < radius || planes[far].distance(p) < radius || planes[left].distance(p) < radius ||
//			planes[right].distance(p) < radius || planes[up].distance(p) < radius || planes[down].distance(p) < radius)
//		return 6;
//	else
//		return 7;
//}

// -------------------------------------------------------------------------------------------------

} // namespace space
