// File:   type_info.hpp Author: Vader Created on 16 September 2019, 19:41

#pragma once

// std
#include <typeinfo>


namespace libv {

// -------------------------------------------------------------------------------------------------

// TODO P5: Find and a better name, and wrap the 3 type with a single type

// USAGE: std::unordered_map<TypeInfoRef, std::weak_ptr<T>, TypeInfoRefHasher, TypeInfoRefEqualTo> typed_cache;
using TypeInfoRef = std::reference_wrapper<const std::type_info>;

struct TypeInfoRefHasher {
    std::size_t operator()(TypeInfoRef code) const {
        return code.get().hash_code();
    }
};

struct TypeInfoRefEqualTo {
    bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const {
        return lhs.get() == rhs.get();
    }
};

// -------------------------------------------------------------------------------------------------

} // namespace libv
