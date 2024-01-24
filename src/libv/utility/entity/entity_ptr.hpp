// Project: libv.utility, File: src/libv/utility/entity/entity_ptr.hpp

#pragma once

#include <libv/utility/entity/entity_ptr_fwd.hpp>

// #include <bit>
// #include <cassert>
// #include <utility>

#include <libv/utility/entity/entity_store.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

//template <typename T>
//concept cx_entity = requires(T& var) {
//	{ var.ref_count } -> std::integral;
//};

struct entity_access {
	template <typename T>
	[[nodiscard]] static constexpr inline auto& ref_count(T& entity) noexcept {
		return entity.ref_count;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class entity_ptr {
	friend entity_store<T>;

private:
	T* ptr = nullptr;

public:
	constexpr inline void reset() noexcept {
		if (ptr == nullptr)
			return;

		const auto tmp = ptr;
		ptr = nullptr;

		if (--entity_access::ref_count(*tmp) != 0)
			return;

		auto store = entity_store<T>::store_from_pointer(tmp);
		// if (store == nullptr) // if ptr is not null, there isn't real reason to worry about null context
		//		return;

		store->destroy(tmp);
	}

protected:
	explicit constexpr inline entity_ptr(T* ptr) noexcept : ptr(ptr) {
		assert(ptr != nullptr && "Internal error: Entity_ptr cannot be null on direct construction");
		++entity_access::ref_count(*ptr);
	}

public:
	constexpr inline entity_ptr() noexcept = default;

	explicit(false) constexpr inline entity_ptr(std::nullptr_t) noexcept : ptr(nullptr) { }

	constexpr inline entity_ptr(const entity_ptr& other) noexcept : ptr(other.ptr) {
		if (ptr != nullptr)
			++entity_access::ref_count(*ptr);
	}

	constexpr inline entity_ptr& operator=(std::nullptr_t) & noexcept {
		reset();
		return *this;
	}

	constexpr inline entity_ptr& operator=(const entity_ptr& other) & noexcept {
		// NOTE: using this == &other instead of ptr == other.ptr to have all moved from state the same (and be reset)
		if (this == &other)
			return *this;

		reset();
		ptr = other.ptr;

		if (ptr != nullptr)
			++entity_access::ref_count(*ptr);

		return *this;
	}

	constexpr inline entity_ptr(entity_ptr&& other) noexcept : ptr(other.ptr) {
		other.ptr = nullptr;
	}

	constexpr inline entity_ptr& operator=(entity_ptr&& other) & noexcept {
		if (this == &other)
			return *this;

		reset();
		ptr = other.ptr;
		other.ptr = nullptr;
		return *this;
	}

	explicit(false) constexpr inline entity_ptr(const primary_entity_ptr<T>& other) noexcept :
		entity_ptr(static_cast<const entity_ptr&>(other)) { }

	constexpr inline entity_ptr& operator=(const primary_entity_ptr<T>& other) & noexcept {
		return *this = static_cast<const entity_ptr&>(other);
	}

	explicit(false) constexpr inline entity_ptr(primary_entity_ptr<T>&& other) noexcept :
		ptr(other.get()) {
		other.kill();
		other.ptr = nullptr;
	}

	constexpr inline entity_ptr& operator=(primary_entity_ptr<T>&& other) & noexcept {
		reset();
		ptr = other.ptr;
		other.kill();
		other.ptr = nullptr;
		return *this;
	}

	~entity_ptr() {
		reset();
	}

public:
	[[nodiscard]] constexpr inline void* context() const noexcept;
	[[nodiscard]] constexpr inline entity_store<T>* store() const noexcept;

public:
	[[nodiscard]] constexpr inline T& operator*() const noexcept {
		return assert(ptr != nullptr), *ptr;
	}
	[[nodiscard]] constexpr inline T* operator->() const noexcept {
		return assert(ptr != nullptr), ptr;
	}
	[[nodiscard]] explicit constexpr inline operator bool() const noexcept {
		return ptr != nullptr;
	}
//	[[nodiscard]] explicit(false) constexpr inline operator T*() const noexcept {
//		return ptr;
//	}
	[[nodiscard]] constexpr inline T* get() const noexcept {
		return ptr;
	}

public:
	/// Abandon the stored pointer without (dereferencing it and) decrementing its ref_count
	[[nodiscard]] constexpr inline T* abandon() noexcept {
		auto tmp = ptr;
		ptr = nullptr;
		return tmp;
	}

	/// Adopt a pointer without (dereferencing it and) incrementing its ref_count
	constexpr inline void adopt(T* ptr_) noexcept {
		reset();
		ptr = ptr_;
	}

public:
	[[nodiscard]] friend constexpr inline std::strong_ordering operator<=>(const entity_ptr& lhs, const entity_ptr& rhs) noexcept = default;
	[[nodiscard]] friend constexpr inline bool operator==(const entity_ptr& lhs, const entity_ptr& rhs) noexcept = default;

	[[nodiscard]] friend constexpr inline bool operator==(const entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr == nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator==(std::nullptr_t, const entity_ptr& p) noexcept {
		return nullptr == p.ptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr != nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(std::nullptr_t, const entity_ptr& p) noexcept {
		return nullptr != p.ptr;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
class primary_entity_ptr : public entity_ptr<T> {
	friend entity_store<T>;

private:
	explicit constexpr inline primary_entity_ptr(T* ptr) noexcept : entity_ptr<T>(ptr) {}

public:
	constexpr inline primary_entity_ptr() noexcept = default;

	constexpr inline primary_entity_ptr(const primary_entity_ptr&) noexcept = delete;
	constexpr inline primary_entity_ptr& operator=(const primary_entity_ptr&) & noexcept = delete;
	constexpr inline primary_entity_ptr(primary_entity_ptr&&) noexcept = default;
	constexpr inline primary_entity_ptr& operator=(primary_entity_ptr&& other) & noexcept {
		kill();
		static_cast<entity_ptr<T>&>(*this) = static_cast<entity_ptr<T>&&>(other);
		return *this;
	}

	explicit(false) constexpr inline primary_entity_ptr(std::nullptr_t) noexcept : entity_ptr<T>(nullptr) { }

	void reset() {
		kill();
		entity_ptr<T>::reset();
	}

	void kill() {
		if constexpr (requires { this->get()->kill(); } )
			if (this->get() != nullptr)
				this->get()->kill();
	}

	~primary_entity_ptr() {
		kill();
	}

public:
	[[nodiscard]] friend constexpr inline std::strong_ordering operator<=>(const primary_entity_ptr& lhs, const primary_entity_ptr& rhs) noexcept = default;
	[[nodiscard]] friend constexpr inline bool operator==(const primary_entity_ptr& lhs, const primary_entity_ptr& rhs) noexcept = default;

	[[nodiscard]] friend constexpr inline bool operator==(const primary_entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr == nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator==(std::nullptr_t, const primary_entity_ptr& p) noexcept {
		return nullptr == p.ptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(const primary_entity_ptr& p, std::nullptr_t) noexcept {
		return p.ptr != nullptr;
	}
	[[nodiscard]] friend constexpr inline bool operator!=(std::nullptr_t, const primary_entity_ptr& p) noexcept {
		return nullptr != p.ptr;
	}
};

// -------------------------------------------------------------------------------------------------

template <typename T>
[[nodiscard]] constexpr inline void* entity_ptr<T>::context() const noexcept {
	return entity_store<T>::context_from_pointer(*this);
}

template <typename T>
[[nodiscard]] constexpr inline entity_store<T>* entity_ptr<T>::store() const noexcept {
	return entity_store<T>::store_from_pointer(*this);
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
