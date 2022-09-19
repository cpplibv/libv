// Project: libv.utility, File: src/libv/utility/cloud.hpp

#pragma once

// fwd
#include <libv/utility/cloud_fwd.hpp>
// libv
#include <libv/meta/force_inline.hpp>
#include <libv/utility/type_key.hpp>
// std
#include <memory>
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

class ImplCloud;

// NOTE: It is possible to extend or implement Cloud with an owning version: Simple 8 byte deleter function with the object pointer

/// Handler type
class Cloud {
public:
	using object_ptr = void*;
	using type_uid = libv::type_uid;

private:
	std::shared_ptr<ImplCloud> self;

public:
	Cloud();
	inline Cloud(const Cloud&) noexcept = default;
	inline Cloud& operator=(const Cloud&) & noexcept = default;
	inline Cloud(Cloud&&) noexcept = default;
	inline Cloud& operator=(Cloud&&) & noexcept = default;
	inline ~Cloud() = default;

private:
	void aux_set(type_uid object_type, object_ptr ptr);
	void aux_set(std::string name, type_uid object_type, object_ptr ptr);

	[[nodiscard]] object_ptr aux_get(type_uid object_type) const noexcept;
	[[nodiscard]] object_ptr aux_get(const std::string_view name, type_uid object_type) const noexcept;
//	[[nodiscard]] object_ptr aux_get(const std::string_view name, type_uid object_type) const noexcept;

	void aux_remove(type_uid object_type) const noexcept;
	void aux_remove(const std::string_view name, type_uid object_type) const noexcept;

//	void aux_push(type_uid object_type, object_ptr ptr);
//	void aux_pop(type_uid object_type) noexcept;

//	void aux_push(std::string name, type_uid object_type, object_ptr ptr);
//	void aux_pop(std::string name, type_uid object_type) noexcept;

public:
	template <typename Object>
	LIBV_FORCE_INLINE void set(Object* ptr) {
		aux_set(libv::type_key<Object>(), ptr);
	}

	template <typename Object>
	LIBV_FORCE_INLINE void set(const std::string& name, Object* ptr) {
		aux_set(name, libv::type_key<Object>(), ptr);
	}

	template <typename Object>
	[[nodiscard]] LIBV_FORCE_INLINE Object* get() const noexcept {
		return static_cast<Object*>(aux_get(libv::type_key<Object>()));
	}

	template <typename Object>
	[[nodiscard]] LIBV_FORCE_INLINE Object* get(const std::string_view name) const noexcept {
		return static_cast<Object*>(aux_get(name, libv::type_key<Object>()));
	}

	template <typename Object>
	LIBV_FORCE_INLINE void remove() noexcept {
		aux_remove(libv::type_key<Object>());
	}

	template <typename Object>
	LIBV_FORCE_INLINE void remove(const std::string_view name) noexcept {
		aux_remove(name, libv::type_key<Object>());
	}

//	template <typename Object>
//	LIBV_FORCE_INLINE void push(Object* ptr) {
//		aux_push(libv::type_key<Object>(), ptr);
//	}
//
//	template <typename Object>
//	LIBV_FORCE_INLINE void pop(Object* ptr) noexcept {
//		aux_pop(libv::type_key<Object>());
//	}

public:
	[[nodiscard]] std::size_t num_object() const noexcept;
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
