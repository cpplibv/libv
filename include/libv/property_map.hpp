// File: VMap.hpp, Created on 2014. március 31. 16:48, Author: Vader

#pragma once

// ext
#include <boost/optional.hpp>
// libv
#include <libv/void_guard.hpp>
// std
#include <atomic>
#include <map>
#include <memory>

// TODO P2: Remove exceptions
// TODO P4: Adopt boost::optional for getters
// TODO P4: Adopt boost::any
// TODO P4: Restriction for POD and use "merged" allocation and usage of small_vector
// TODO P5: Implement some kind of foreach (Maybe external function as / or "callback" for it?)
// TODO P5: Add functionality for custom address type
//				(idea: need to switch back to template addressing and both type and
//				value is important (hard (imo impossible)))
//				i think it is possible, however then address should provide more features such as
//				default...
// TODO P5: default_value or pair as return type for specific getters? (?mimic std::map?)
// TODO P5: I think it is possible to eliminate one indirection without UB. Slicing without data is safe?
//				Implemented a funny pair fix to it, but i think this also can be eliminated

namespace libv {

template<template<typename...> class Container = std::map>
struct BasicPropertyMap {
	using Address_t = int;
	using Value_t = std::pair<void*, std::unique_ptr<VoidGuardBase>>;

private:
	Container<Address_t, Value_t> dataMap;

public:
	template<typename T>
	class Address {
		friend class BasicPropertyMap<Container>;
		const Address_t address = BasicPropertyMap<Container>::nextAddress();
	public:
		Address() = default;
	};

	class SetterProxy {
		friend class BasicPropertyMap<Container>;
		BasicPropertyMap<Container>& object;
		SetterProxy(BasicPropertyMap<Container>& object) : object(object) { }
	public:
		template<typename T, typename... Args>
		inline SetterProxy& operator()(const BasicPropertyMap<Container>::Address<T>& address, Args&&... args) {
			object.set(address, std::forward<Args>(args)...);
			return *this;
		}
	};
	template <typename T> boost::optional<T&> get(const BasicPropertyMap<Container>::Address<T>& address) {
		try {
			return boost::optional<T&>(atElement(address));
		} catch (const std::out_of_range& e) {
			return boost::optional<T&>();
		}
	}
	template <typename T> const boost::optional<T&> get(const BasicPropertyMap<Container>::Address<T>& address) const {
		try {
			return boost::optional<T&>(atElement(address));
		} catch (const std::out_of_range& e) {
			return boost::optional<T&>();
		}
	}
	template <typename T> boost::optional<T&> operator[](const BasicPropertyMap<Container>::Address<T>& address) {
		return boost::optional<T&>(atElement(address));
	}
	template <typename T> const boost::optional<T&> operator[](const BasicPropertyMap<Container>::Address<T>& address) const {
		return boost::optional<T&>(atElement(address));
	}
	template <typename T> bool remove(const BasicPropertyMap<Container>::Address<T>& address) {
		try {
			dataMap.erase(address.address);
			return true;
		} catch (const std::out_of_range& e) {
			return false;
		}
	}
	size_t size() const {
		return dataMap.size();
	}
	template <typename T, typename... Args>
	inline void set(const BasicPropertyMap<Container>::Address<T>& address, Args&&... args) {
		std::unique_ptr<VoidGuardBase> value(new VoidGuard<T> (std::forward<Args>(args)...));
		dataMap[address.address] = std::make_pair(value->ptr(), std::move(value));
	}
	inline SetterProxy set() {
		return SetterProxy(*this);
	}

private:
	static inline Address_t nextAddress() {
		static std::atomic<Address_t> lastAddress{0};
		return ++lastAddress;
	}
	template <typename T> T& atElement(const BasicPropertyMap<Container>::Address<T>& address) const {
		return *(reinterpret_cast<T*> (dataMap.at(address.address).first));
	}
};

// -------------------------------------------------------------------------------------------------

using PropertyMap = BasicPropertyMap<>;

} //namespace libv