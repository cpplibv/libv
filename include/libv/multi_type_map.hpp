// File: VMap.hpp, Created on 2014. március 31. 16:48, Author: Vader

#pragma once

// libv
#include <libv/memory.hpp>
#include <libv/void_guard.hpp>
// std
#include <atomic>
#include <map>
#include <memory>

// TODO P4: Adopt boost::any and eliminate VoidGuard
// TODO P4: Restriction for ("POD" not pod but something that can be copied OR would only be POD be good?
//			, would need an additional lookup map?, its a map... flat map... but map. small_flat_map?)
//			and use "merged" allocation and usage of small_vector
//			So lost in prev sentence... nvm: so one map will stay, this will be flat_map yep,
//			but to memory allocation for VoidGuards could be merged into one cache frendly small_vector
// TODO P5: Implement some kind of foreach (Maybe external function as / or "callback" for it?)

namespace libv {

namespace detail {
struct BasicMultiTypeMapAddressProvider {
	using Address_t = size_t;
	static inline Address_t nextAddress() {
		static std::atomic<Address_t> lastAddress{0};
		return ++lastAddress;
	}
};
} //namespace detail

template<template<typename...> class Container = std::map>
struct BasicMultiTypeMap {
private:
	using Address_t = size_t;
	using Value_t = std::pair<void*, std::unique_ptr<VoidGuardBase>>;

private:
	Container<Address_t, Value_t> dataMap;

	// Key -----------------------------------------------------------------------------------------
public:
	template<typename T>
	class Key {
		friend class BasicMultiTypeMap<Container>;
		const Address_t address = detail::BasicMultiTypeMapAddressProvider::nextAddress();
	public:
		Key() = default;
	};

	// OptionalValue -------------------------------------------------------------------------------
public:
	template<typename T>
	class OptionalValue {
	private:
		T* ptr;
	public:
		OptionalValue(T* ptr) : ptr(ptr) { }
		T& operator*() {
			return *ptr;
		}
		const T& operator*() const {
			return *ptr;
		}
		T* operator->() noexcept {
			return ptr;
		}
		const T* operator->() const noexcept {
			return ptr;
		}
		explicit operator bool() const noexcept {
			return ptr != nullptr;
		}
		bool is_initialized() const noexcept {
			return ptr != nullptr;
		}
		T& value() {
			return *ptr;
		}
		const T& value() const {
			return *ptr;
		}
	};

	// Element access ------------------------------------------------------------------------------
public:
	/// Access specified element with bounds checking
	template <typename T> T& at(const Key<T>& key) {
		return *reinterpret_cast<T*> (dataMap.at(key.address).first);
	}
	/// Access specified element with bounds checking
	template <typename T> const T& at(const Key<T>& key) const {
		return *reinterpret_cast<const T*> (dataMap.at(key.address).first);
	}
	/// Access specified element performs an insertion if such key does not already exist.
	template <typename T> T& operator[](const Key<T>& key) {
		auto it = dataMap.find(key.address);
		if (it == dataMap.end()) {
			std::unique_ptr<VoidGuardBase> value(new VoidGuard<T>());
			auto ptr = value->ptr();
			dataMap.emplace(key.address, std::make_pair(value->ptr(), std::move(value)));
			return *reinterpret_cast<T*> (ptr);
		} else {
			return *reinterpret_cast<T*> (it->second.first);
		}
	}

	// Capacity ------------------------------------------------------------------------------------
public:
	/// Checks whether the container is empty
	bool empty() {
		return dataMap.empty();
	}
	/// returns the number of elements
	size_t size() {
		return dataMap.size();
	}

	// Modifiers -----------------------------------------------------------------------------------
public:
	/// clears the contents
	void clear() {
		return dataMap.clear();
	}
	/// inserts elements
	template <typename T> T& insert(const Key<T>& key, const T& value) {
		std::unique_ptr<VoidGuardBase> voidValue(new VoidGuard<T>(value));
		auto ptr = voidValue->ptr();
		dataMap.emplace(key.address, std::make_pair(voidValue->ptr(), std::move(voidValue)));
		return *reinterpret_cast<T*> (ptr);
	}
	/// constructs element in-place
	template <typename T, typename... Args> T& emplace(const Key<T>& key, Args&&... args) {
		std::unique_ptr<VoidGuardBase> voidValue(new VoidGuard<T>(std::forward<Args>(args)...));
		auto ptr = voidValue->ptr();
		dataMap.emplace(key.address, std::make_pair(voidValue->ptr(), std::move(voidValue)));
		return *reinterpret_cast<T*> (ptr);
	}
	/// erases elements
	template <typename T> void erase(const Key<T>& key) {
		dataMap.erase(key.address);
	}

	// Lookup  -------------------------------------------------------------------------------------
public:
	/// finds element with specific key
	template <typename T> OptionalValue<T> find(const Key<T>& key) const {
		auto it = dataMap.find(key.address);
		if (it != dataMap.end()) {
			return OptionalValue<T>(reinterpret_cast<T*> (it->second.first));
		} else {
			return OptionalValue<T>(nullptr);
		}
	}

	// ---------------------------------------------------------------------------------------------
};

// -------------------------------------------------------------------------------------------------

using MultiTypeMap = BasicMultiTypeMap<>;

} //namespace libv