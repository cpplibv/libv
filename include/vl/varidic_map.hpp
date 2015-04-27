// File: VMap.hpp, Created on 2014. március 31. 16:48, Author: Vader

#pragma once


// vl
#include <vl/void_guard.hpp>
// std
#include <map>
#include <memory>

//TODO P3: Implement some kind of foreach 
//TODO P5: Implement remaining and simular functionality from map for example : operator[]
//TODO P5: Implement ~~constness to: at / [] / lenght / etc...
//TODO P5: Add functionality for custom address type 
//				(idea: need to switch back to template addressing and both type and
//				value is important (hard (imo impossible)))

namespace vl {

struct VaridicMap {
	typedef int Address_t;

	template<typename T>
	class Address {
	private:
		friend class VaridicMap;
		const Address_t address = nextAddress();
	public:
		const T defaultValue;

		Address() = default;
		template<typename... Args>
		Address(Args&&... args) : defaultValue(std::forward<Args>(args)...) { }
	};

	template <typename T> T& at(const VaridicMap::Address<T>& address);
	template <typename T> const T& at(const VaridicMap::Address<T>& address) const;
	template <typename T> const T& get(const VaridicMap::Address<T>& address) const;

	template <typename T, typename... Args> inline void set(const VaridicMap::Address<T>& address, const Args&... args);
	template <typename T> bool remove(const VaridicMap::Address<T>& address);

private:
	static inline Address_t nextAddress() {
		static Address_t lastAddress = 0;
		return lastAddress++;
	}
	template <typename T> T* atPtrImpl(const VaridicMap::Address<T>& address) const;

	std::map <Address_t, std::unique_ptr<VoidGuardBase>> dataMap;
};
//------------------------------------------------------------------------------
template<typename T>
inline T* VaridicMap::atPtrImpl(const VaridicMap::Address<T>& address) const {
	return (reinterpret_cast<T*> (dataMap.at(address.address)->getPtr()));
}
//------------------------------------------------------------------------------
template<typename T, typename... Args>
inline void VaridicMap::set(const VaridicMap::Address<T>& address, const Args&... args) {
	dataMap[address.address] = std::unique_ptr<VoidGuardBase>(new VoidGuard<T> (args...)); //C++14: make unique
}
template<typename T>
inline T& VaridicMap::at(const VaridicMap::Address<T>& address) {
	return *atPtrImpl(address);
}
template<typename T>
inline const T& VaridicMap::at(const VaridicMap::Address<T>& address) const {
	return *atPtrImpl(address);
}
template<typename T>
inline const T& VaridicMap::get(const VaridicMap::Address<T>& address) const {
	try {
		return at(address);
	} catch (const std::out_of_range& e) {
		return address.defaultValue;
	}
}
template<typename T>
inline bool VaridicMap::remove(const VaridicMap::Address<T>& address) {
	try {
		dataMap.erase(address.address);
		return true;
	} catch (const std::out_of_range& e) {
		return false;
	}
}
} //namespace vl