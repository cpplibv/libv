// File: observer_ref.hpp - Created on 2018.04.15. 03:37 - Author: Vader

#pragma once

// pro
#include <libv/utility/intrusive_ptr.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
using intrusive_ref = intrusive_ptr<T>;

template <typename T, typename... Args>
intrusive_ref<T> make_intrusive_ref(Args&&... args) {
    return intrusive_ref<T>(new T(std::forward<Args>(args)...));
}

// -------------------------------------------------------------------------------------------------

} // namespace std
