// File: VMap.hpp, Created on 2014. március 31. 16:48, Author: Vader

#pragma once

// libv
#include <libv/void_guard.hpp>
// std
#include <atomic>
#include <map>
#include <memory>

//TODO P4: Adopt boost::optional for getters
//TODO P4: Adopt boost::any
//TODO P4: Restriction for POD and use "merged" allocation and usage of small_vector
//TODO P5: Implement some kind of foreach (Maybe external function as / or "callback" for it?)
//TODO P5: Add functionality for custom address type
//				(idea: need to switch back to template addressing and both type and
//				value is important (hard (imo impossible)))
//				i think it is possible, however then address should provide more features such as
//				default...
//TODO P5: default_value or pair as return type for specific getters? (?mimic std::map?)
//TODO P5: I think it is possible to eliminate one indirection without UB. Slicing without data is safe?
//				Implemented a funny pair fix to it, but i think this also can be eliminated

namespace libv {

template<template<typename...> class Container = std::map>
struct BasicPropertyMap {
	using Address_t = int;
	using Value_t = std::pair<void*, std::unique_ptr<VoidGuardBase>>;

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

	template <typename T> T* at(const BasicPropertyMap<Container>::Address<T>& address);
	template <typename T> const T* at(const BasicPropertyMap<Container>::Address<T>& address) const;
	template <typename T> T* get(const BasicPropertyMap<Container>::Address<T>& address);
	template <typename T> const T* get(const BasicPropertyMap<Container>::Address<T>& address) const;
	template <typename T> T* operator[](const BasicPropertyMap<Container>::Address<T>& address);
	template <typename T> const T* operator[](const BasicPropertyMap<Container>::Address<T>& address) const;

	template <typename T> bool remove(const BasicPropertyMap<Container>::Address<T>& address);
	size_t size() const;
	template <typename T, typename... Args> inline void set(const BasicPropertyMap<Container>::Address<T>& address, Args&&... args);
	inline SetterProxy set();

private:
	static inline Address_t nextAddress() {
		static std::atomic<Address_t> lastAddress{0};
		return lastAddress++;
	}
	template <typename T> T* atPtrImpl(const BasicPropertyMap<Container>::Address<T>& address) const;

	Container<Address_t, Value_t> dataMap;
};
//------------------------------------------------------------------------------
template<template<typename...> class Container> template<typename T>
inline T* BasicPropertyMap<Container>::atPtrImpl(const BasicPropertyMap<Container>::Address<T>& address) const {
	return (reinterpret_cast<T*> (dataMap.at(address.address).first));
}
//------------------------------------------------------------------------------
template<template<typename...> class Container> template<typename T>
inline T* BasicPropertyMap<Container>::at(const BasicPropertyMap<Container>::Address<T>& address) {
	return atPtrImpl(address);
}
template<template<typename...> class Container> template<typename T>
inline const T* BasicPropertyMap<Container>::at(const BasicPropertyMap<Container>::Address<T>& address) const {
	return atPtrImpl(address);
}
template<template<typename...> class Container> template<typename T>
inline T* BasicPropertyMap<Container>::get(const BasicPropertyMap<Container>::Address<T>& address) {
	try {
		return at(address);
	} catch (const std::out_of_range& e) {
		return nullptr;
	}
}
template<template<typename...> class Container> template<typename T>
inline const T* BasicPropertyMap<Container>::get(const BasicPropertyMap<Container>::Address<T>& address) const {
	try {
		return at(address);
	} catch (const std::out_of_range& e) {
		return nullptr;
	}
}
template<template<typename...> class Container> template<typename T>
inline T* BasicPropertyMap<Container>::operator[](const BasicPropertyMap<Container>::Address<T>& address) {
	return at(address);
}
template<template<typename...> class Container> template<typename T>
inline const T* BasicPropertyMap<Container>::operator[](const BasicPropertyMap<Container>::Address<T>& address) const {
	return at(address);
}

// -------------------------------------------------------------------------------------------------
template<template<typename...> class Container> template<typename T>
inline bool BasicPropertyMap<Container>::remove(const BasicPropertyMap<Container>::Address<T>& address) {
	try {
		dataMap.erase(address.address);
		return true;
	} catch (const std::out_of_range& e) {
		return false;
	}
}
template<template<typename...> class Container> template<typename T, typename... Args>
inline void BasicPropertyMap<Container>::set(const BasicPropertyMap<Container>::Address<T>& address, Args&&... args) {
	std::unique_ptr<VoidGuardBase> value(new VoidGuard<T> (std::forward<Args>(args)...));
	dataMap[address.address] = std::make_pair(value->ptr(), std::move(value));
}
template<template<typename...> class Container>
inline typename BasicPropertyMap<Container>::SetterProxy BasicPropertyMap<Container>::set(){
	return {*this};
}
template<template<typename...> class Container>
inline size_t BasicPropertyMap<Container>::size() const {
	return dataMap.size();
}

// -------------------------------------------------------------------------------------------------

using PropertyMap = BasicPropertyMap<>;

} //namespace libv