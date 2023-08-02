// Project: libv.math, File: src/libv/math/matBB.hpp

#pragma once

// fwd
#include <libv/math/mat_fwd.hpp>
// ext
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/matrix_transform_2d.hpp>
// #include <glm/mat2x2.hpp>
// #include <glm/mat2x3.hpp>
// #include <glm/mat2x4.hpp>
// #include <glm/mat3x2.hpp>
// #include <glm/mat3x3.hpp>
// #include <glm/mat3x4.hpp>
// #include <glm/mat4x2.hpp>
// #include <glm/mat4x3.hpp>
// #include <glm/mat4x4.hpp>
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>
// #include <glm/vec4.hpp>
// pro
#include <libv/math/angle.hpp>
#include <libv/math/vec.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <int R, int C, typename T>
struct matBB_t {
	using value_type = T;
	static constexpr int Column = C;
	static constexpr int Row = R;

private:
	libv::vec_t<R, T> data_[C]{0};

public:
	// constexpr inline matBB_t() {
	// constexpr inline matBB_t(libv::uninitialized_t) {
	// constexpr inline matBB_t(T main_diagonal) {
	// constexpr inline matBB_t(T <[R*C]>...) {
	// constexpr inline matBB_t(vec<C or R, T> <[R or C]>...) {

	// constexpr inline matBB_t(const matBB_t&) = default;
	// constexpr inline matBB_t(matBB_t&&) = default;
	// constexpr inline matBB_t& operator=(const matBB_t&) & = default;
	// constexpr inline matBB_t& operator=(matBB_t&&) & = default;
	// constexpr inline ~matBB_t() = default;

public: // --- Named constructors ----------------------------------------------------------------------

	// [[nodiscard]] static constexpr inline matBB_t from_rows(...) noexcept {
	// [[nodiscard]] static constexpr inline matBB_t from_columns(...) noexcept {

	// [[nodiscard]] static constexpr inline matBB_t create_perspective(const T fovy, const T aspect, const T near, const T far) noexcept {
	// [[nodiscard]] static constexpr inline matBB_t create_lookAt(const libv::vec3_t<T> eye, const libv::vec3_t<T> target, const libv::vec3_t<T> up) noexcept {
	// [[nodiscard]] static constexpr inline matBB_t create_ortho(const T left, const T right, const T bottom, const T top) noexcept {

	[[nodiscard]] static constexpr inline matBB_t create_ortho(const libv::vec2_t<T> position, const libv::vec2_t<T> size) noexcept {
		return create_ortho(position.x, position.x + size.x, position.y, position.y + size.y);
	}

	[[nodiscard]] static constexpr inline matBB_t create_ortho(const libv::vec2_t<T> position, const libv::vec2_t<T> size, const T near, const T far) noexcept {
		return create_ortho(position.x, position.x + size.x, position.y, position.y + size.y, near, far);
	}

	// [[nodiscard]] static constexpr inline matBB_t create_ortho(const T left, const T right, const T bottom, const T top, const T near, const T far) noexcept {

	[[nodiscard]] static constexpr inline matBB_t create_ortho(const libv::vec3_t<T> position, const libv::vec3_t<T> size) noexcept {
		return create_ortho(position.x, position.x + size.x, position.y, position.y + size.y, position.z, position.z + size.z);
	}

	// [[nodiscard]] static constexpr inline matBB_t create_translate(const libv::vec3_t<T>& v) noexcept {
	// [[nodiscard]] static constexpr inline matBB_t create_translate(const T x, const T y, const T z) noexcept {
	//
	// [[nodiscard]] static constexpr inline matBB_t create_rotate(const radian<T> angle, const libv::vec3_t<T>& axis) noexcept {
	// [[nodiscard]] static constexpr inline matBB_t create_rotate(const radian<T> angle, const T x, const T y, const T z) noexcept {
	//
	// [[nodiscard]] static constexpr inline matBB_t create_scale(const libv::vec3_t<T>& v) noexcept {
	// [[nodiscard]] static constexpr inline matBB_t create_scale(const T x, const T y, const T z) noexcept {

	[[nodiscard]] static constexpr inline matBB_t create_identity() noexcept {
		return matBB_t(T{1});
	}

	[[nodiscard]] static constexpr inline matBB_t create_texture() noexcept
			requires (Column == 4 && Row == 4) {
		return matBB_t(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0);
	}

	// -------------------------------------------------------------------------------------------------

public:
	[[nodiscard]] constexpr T* data() noexcept {
		return data_[0].data();
	}
	[[nodiscard]] constexpr const T* data() const noexcept {
		return data_[0].data();
	}

	// [[nodiscard]] constexpr inline vec_t<R, T>& operator[](int col) noexcept {
	// [[nodiscard]] constexpr inline const vec_t<R, T>& operator[](int col) const noexcept {
	// [[nodiscard]] constexpr inline vec_t<R, T>& at(int col) noexcept {
	// [[nodiscard]] constexpr inline const vec_t<R, T>& at(int col) const noexcept {

	// [[nodiscard]] constexpr inline vec_t<R, T> row(int col) const noexcept {
	// [[nodiscard]] constexpr inline vec_t<C, T> column(int col) const noexcept {

	// [[nodiscard]] constexpr inline T& operator[](int col, int row) noexcept {
	// [[nodiscard]] constexpr inline const T& operator[](int col, int row) const noexcept {
	// [[nodiscard]] constexpr inline T& at(int col, int row) noexcept {
	// [[nodiscard]] constexpr inline const T& at(int col, int row) const noexcept {

	// -------------------------------------------------------------------------------------------------

public:
	// [[nodiscard]] constexpr inline vec_t<R, T> operator*(const vec_t<R, T>& vec) const noexcept {
	// [[nodiscard]] constexpr inline matBB_t operator*(const matBB_t& mat) const noexcept {

	// -------------------------------------------------------------------------------------------------

public:
	// requires Row == Column
	// [[nodiscard]] constexpr inline value_type determinant() const noexcept {

	// requires Row == Column
	// constexpr inline matBB_t& inverse() noexcept {
	// requires Row == Column
	// [[nodiscard]] constexpr inline matBB_t inverse_copy() const noexcept {

	// constexpr inline matBB_t& rotate(const radian<T> angle, const libv::vec3_t<T>& axis) noexcept
	// 		requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {
	// [[nodiscard]] constexpr inline matBB_t rotate_copy(const radian<T> angle, const libv::vec3_t<T>& axis) const noexcept
	// 		requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {

	// constexpr inline matBB_t& rotate(const T angle, const libv::vec3_t<T>& axis) noexcept
	// 		requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {
	// [[nodiscard]] constexpr inline matBB_t rotate_copy(const T angle, const libv::vec3_t<T>& axis) const noexcept
	// 		requires (Column == 4 && Row == 4 || Column == 3 && Row == 3) {

	// constexpr inline matBB_t& rotate(const glm::quat& quat) noexcept {
	// [[nodiscard]] constexpr inline matBB_t rotate_copy(const glm::quat& quat) const noexcept {

	// constexpr inline matBB_t& scale(const libv::vec3_t<T>& v) noexcept {
	// [[nodiscard]] constexpr inline matBB_t scale_copy(const libv::vec3_t<T>& v) const noexcept {

	// constexpr inline matBB_t& translate(const libv::vec3_t<T>& v) noexcept {
	// [[nodiscard]] constexpr inline matBB_t translate_copy(const libv::vec3_t<T>& v) const noexcept {

	constexpr inline matBB_t& translate(const T x, const T y, const T z) noexcept {
		return translate(libv::vec3_t<T>(x, y, z));
	}
	[[nodiscard]] constexpr inline matBB_t translate_copy(const T x, const T y, const T z) const noexcept {
		return translate_copy(libv::vec3_t<T>(x, y, z));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
