// Project: libv.meta, File: src/libv/meta/force_inline.hpp

#pragma once


#ifdef _MSC_VER
#	define LIBV_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#	define LIBV_FORCE_INLINE __attribute__((always_inline)) inline
#else
#	define LIBV_FORCE_INLINE inline
#endif
