// File: observer_ptr.hpp - Created on 2018.04.15. 03:37 - Author: Vader

#pragma once

// ext
#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

template <typename T>
using intrusive_ptr = boost::intrusive_ptr<T>;

template <typename T>
struct intrusive_base_unsafe : boost::intrusive_ref_counter<T, boost::thread_unsafe_counter> {
};

template <typename T>
struct intrusive_base_safe : boost::intrusive_ref_counter<T, boost::thread_safe_counter> {
};

template <typename T, typename... Args>
intrusive_ptr<T> make_intrusive(Args&&... args) {
    return intrusive_ptr<T>(new T(std::forward<Args>(args)...));
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
