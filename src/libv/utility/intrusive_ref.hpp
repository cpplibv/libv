// Project: libv.utility, File: src/libv/utility/intrusive_ref.hpp

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
