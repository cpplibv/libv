// File: matrix_stack.hpp - Created on 2017.11.26. 02:58 - Author: Vader

#pragma once

// std
#include <vector>
// libv
#include <libv/math/angle.hpp>
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
// pro
#include <libv/gl/assert.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

template <typename Mat>
class MatrixStack {
private:
	std::vector<Mat> stack;

public:
	using value_type = Mat;
	using T = typename Mat::value_type;

public:
	inline MatrixStack() {
		stack.emplace_back();
		identity();
	}
	inline MatrixStack<Mat>& push() {
		LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
		stack.push_back(top());
		return *this;
	}
	inline MatrixStack<Mat>& push(const Mat& mx) {
		stack.push_back(mx);
		return *this;
	}
	[[nodiscard]] inline auto push_guard() noexcept {
		push();
		return Guard([this] { pop(); });
	}
	[[nodiscard]] inline auto push_guard(const Mat& mx) noexcept {
		push(mx);
		return Guard([this] { pop(); });
	}
	inline MatrixStack<Mat>& pop() {
		// Asserting for 1 because because empty stack is not allowed
		LIBV_GL_DEBUG_ASSERT(stack.size() > 1);
		stack.pop_back();
		return *this;
	}
	inline decltype(auto) top() const {
		LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
		return stack.back();
	}
	inline decltype(auto) top() {
		LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
		return stack.back();
	}
	inline MatrixStack& operator=(const Mat& mat) & {
		top() = mat;
		return *this;
	}
	inline decltype(auto) operator*(const vec4_t<T>& v) const {
		return top() * v;
	}
	inline decltype(auto) operator*(const Mat& mat) const {
		return top() * mat;
	}
	inline decltype(auto) operator*(const MatrixStack<Mat>& ms) const {
		return top() * ms.top();
	}
	inline decltype(auto) operator*=(const Mat& mat) {
		top() = top() * mat;
		return *this;
	}
	inline decltype(auto) operator*=(const MatrixStack<Mat>& ms) {
		top() = top() * ms.top();
		return *this;
	}
	inline operator Mat() const {
		return top();
	}
	inline operator Mat&() {
		return top();
	}
	inline operator const Mat&() const {
		return top();
	}

	// ---------------------------------------------------------------------------------------------

	inline decltype(auto) rotate(const Radian<T> a, const vec3_t<T>& v) {
		top().rotate(a, v);
		return *this;
	}

	inline decltype(auto) rotate(const Radian<T> a, const T x, const T y, const T z) {
		top().rotate(a, vec3_t<T>(x, y, z));
		return *this;
	}

	inline decltype(auto) scale(const T s) {
		top().scale(vec3_t<T>(s, s, s));
		return *this;
	}
	inline decltype(auto) scale(const T x, const T y, const T z) {
		top().scale(vec3_t<T>(x, y, z));
		return *this;
	}
	inline decltype(auto) scale(const vec3_t<T>& v) {
		top().scale(v);
		return *this;
	}

	inline decltype(auto) translate(const T x, const T y, const T z) {
		top().translate(vec3_t<T>(x, y, z));
		return *this;
	}
	inline decltype(auto) translate(const vec3_t<T>& v) {
		top().translate(v);
		return *this;
	}

	inline decltype(auto) identity() {
		top() = Mat::identity();
		return *this;
	}

	// -------------------------------------------------------------------------------------------------

	inline decltype(auto) size() const {
		return stack.size();
	}

};

// -------------------------------------------------------------------------------------------------s

} // namespace gl
} // namespace libv
