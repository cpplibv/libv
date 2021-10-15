// Project: libv.utility, File: src/libv/utility/intrusive_ptr.hpp

#pragma once

// ext
#include <boost/intrusive_ptr.hpp>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>


namespace libv {

// -------------------------------------------------------------------------------------------------

/// Usage:
///
///	Forward declaration:
///		class Style;
///		void intrusive_ptr_add_ref(Style*);
///		void intrusive_ptr_release(Style*);
///
///	Declaration:
///		class Style {
///			int32_t ref_count = 0;
///			friend void intrusive_ptr_add_ref(Style*);
///			friend void intrusive_ptr_release(Style*);
///			// ... Actual members
/// 	};
///
///	Implementation:
///		void intrusive_ptr_add_ref(Style* style) {
///			++style->ref_count;
///		}
///
///		void intrusive_ptr_release(Style* style) {
///			if (--style->ref_count == 0)
///				delete style;
///		}

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
