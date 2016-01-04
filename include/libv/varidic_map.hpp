// File: VMap.hpp, Created on 2014. március 31. 16:48, Author: Vader

#pragma once

// libv
#include <libv/void_guard.hpp>
// std
#include <atomic>
#include <map>
#include <memory>

//TODO P5: Implement some kind of foreach (Maybe external function as / or "callback" for it?)
//TODO P5: Add functionality for custom address type
//				(idea: need to switch back to template addressing and both type and
//				value is important (hard (imo impossible)))
//TODO P5: Change default_value to default constructed value and / or pair (?mimic std::map?)
//TODO P5: I think it is possible to eliminate one indirection without UB. Slicing without data is safe?
//				Implemented a funny pair fix to it, but i think this also can be eliminated

namespace libv {

template<template<typename...> class Container = std::map>
struct VaridicMap {
	using Address_t = int;
	using Value_t = std::pair<void*, std::unique_ptr<VoidGuardBase>>;

	template<typename T>
	class Address {
		friend class VaridicMap;
		const Address_t address = VaridicMap::nextAddress();
	public:
		const T defaultValue;

		Address() = default;
		template<typename... Args>
		Address(Args&&... args) : defaultValue(std::forward<Args>(args)...) { }
	};

	class SetterProxy {
		friend class VaridicMap;
		VaridicMap& object;
		SetterProxy(VaridicMap& object) : object(object) { }
	public:
		template<typename T, typename... Args>
		inline SetterProxy& operator()(const VaridicMap::Address<T>& address, Args&&... args) {
			object.set(address, std::forward<Args>(args)...);
			return *this;
		}
	};

	template <typename T> T& at(const VaridicMap::Address<T>& address);
	template <typename T> const T& at(const VaridicMap::Address<T>& address) const;
	template <typename T> T& get(const VaridicMap::Address<T>& address);
	template <typename T> const T& get(const VaridicMap::Address<T>& address) const;
	template <typename T> T& operator[](const VaridicMap::Address<T>& address);
	template <typename T> const T& operator[](const VaridicMap::Address<T>& address) const;

	template <typename T> bool remove(const VaridicMap::Address<T>& address);
	size_t size() const;
	template <typename T, typename... Args> inline void set(const VaridicMap::Address<T>& address, Args&&... args);
	inline SetterProxy set();

private:
	static inline Address_t nextAddress() {
		static std::atomic<Address_t> lastAddress{0};
		return lastAddress++;
	}
	template <typename T> T* atPtrImpl(const VaridicMap::Address<T>& address) const;

	Container<Address_t, Value_t> dataMap;
};
//------------------------------------------------------------------------------
template<template<typename...> class Container> template<typename T>
inline T* VaridicMap<Container>::atPtrImpl(const VaridicMap::Address<T>& address) const {
	return (reinterpret_cast<T*> (dataMap.at(address.address).first));
}
//------------------------------------------------------------------------------
template<template<typename...> class Container> template<typename T>
inline T& VaridicMap<Container>::at(const VaridicMap::Address<T>& address) {
	return *atPtrImpl(address);
}
template<template<typename...> class Container> template<typename T>
inline const T& VaridicMap<Container>::at(const VaridicMap::Address<T>& address) const {
	return *atPtrImpl(address);
}
template<template<typename...> class Container> template<typename T>
inline T& VaridicMap<Container>::get(const VaridicMap::Address<T>& address) {
	try {
		return at(address);
	} catch (const std::out_of_range& e) {
		return address.defaultValue;
	}
}
template<template<typename...> class Container> template<typename T>
inline const T& VaridicMap<Container>::get(const VaridicMap::Address<T>& address) const {
	try {
		return at(address);
	} catch (const std::out_of_range& e) {
		return address.defaultValue;
	}
}
template<template<typename...> class Container> template<typename T>
inline T& VaridicMap<Container>::operator[](const VaridicMap::Address<T>& address) {
	return at(address);
}
template<template<typename...> class Container> template<typename T>
inline const T& VaridicMap<Container>::operator[](const VaridicMap::Address<T>& address) const {
	return at(address);
}

// -------------------------------------------------------------------------------------------------
template<template<typename...> class Container> template<typename T>
inline bool VaridicMap<Container>::remove(const VaridicMap::Address<T>& address) {
	try {
		dataMap.erase(address.address);
		return true;
	} catch (const std::out_of_range& e) {
		return false;
	}
}
template<template<typename...> class Container> template<typename T, typename... Args>
inline void VaridicMap<Container>::set(const VaridicMap<Container>::Address<T>& address, Args&&... args) {
	std::unique_ptr<VoidGuardBase> value(new VoidGuard<T> (std::forward<Args>(args)...));
	dataMap[address.address] = std::make_pair(value->ptr(), std::move(value));
}
template<template<typename...> class Container>
inline typename VaridicMap<Container>::SetterProxy VaridicMap<Container>::set(){
	return {*this};
}
template<template<typename...> class Container>
inline size_t VaridicMap<Container>::size() const {
	return dataMap.size();
}

} //namespace libv