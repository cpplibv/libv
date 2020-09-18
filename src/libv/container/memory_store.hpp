// File:   memory_store.hpp Author: Vader Created on 2018. december 29., 19:52

#pragma once

// std
#include <cstring>
#include <vector>


// Note: This file is just an idea for a type erased raw memory container

namespace libv {

// -------------------------------------------------------------------------------------------------

struct MemoryStore {
	std::vector<std::byte> data;

	template <typename T>
	void push_back(const T& value) {
		auto size = data.size();
		data.resize(size + sizeof(T));
		std::memcpy(data.data() + size, &value, sizeof(T));
	}

	template <typename T>
	inline void reserve(size_t count) {
		data.reserve(sizeof(T) * count);
	}

	template <typename T>
	inline void resize(size_t count) {
		data.resize(sizeof(T) * count);
	}

	template <typename T>
	inline void clear() {
		data.clear();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
