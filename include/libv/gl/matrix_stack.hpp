// File: matrix_stack.hpp - Created on 2017.11.26. 02:58 - Author: Vader

#pragma once

// std
#include <stack>
#include <vector>
// libv
#include <libv/math/vec.hpp>
#include <libv/utility/guard.hpp>
// pro
#include <libv/gl/log.hpp>


namespace libv {
namespace gl {

// -------------------------------------------------------------------------------------------------

template <typename Mat>
class MatrixStack {
private:
	std::stack<Mat, std::vector<Mat>> stack;

public:
	using value_type = Mat;
	using T = typename Mat::value_type;

public:
	inline MatrixStack() {
		stack.emplace(static_cast<T>(1.0));
	}
	inline MatrixStack<Mat>& push() {
		stack.push(top());
		return *this;
	}
	inline MatrixStack<Mat>& push(const Mat& mx) {
		LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
		stack.push(mx);
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
		LIBV_GL_DEBUG_ASSERT(stack.size() > 1);
		stack.pop();
		return *this;
	}
	inline decltype(auto) top() const {
		LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
		return stack.top();
	}
	inline decltype(auto) top() {
		LIBV_GL_DEBUG_ASSERT(stack.size() > 0);
		return stack.top();
	}
	inline MatrixStack& operator=(const Mat& mat) {
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

	inline decltype(auto) rotate(const T& a, const vec3_t<T>& v) {
		top().rotate(a, v);
		return *this;
	}

	inline decltype(auto) scale(const T& s) {
		top().scale(vec3_t<T>(s, s, s));
		return *this;
	}
	inline decltype(auto) scale(const T& x, const T& y, const T& z) {
		top().scale(vec3_t<T>(x, y, z));
		return *this;
	}
	inline decltype(auto) scale(const vec3_t<T>& v) {
		top().scale(v);
		return *this;
	}

	inline decltype(auto) translate(const T& x, const T& y, const T& z) {
		top().translate(vec3_t<T>(x, y, z));
		return *this;
	}
	inline decltype(auto) translate(const vec3_t<T>& v) {
		top().translate(v);
		return *this;
	}

	inline decltype(auto) identity() {
		top() = libv::identity<4, T>();
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
