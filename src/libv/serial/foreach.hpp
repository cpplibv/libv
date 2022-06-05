// Project: libv.serial, File: src/libv/serial/serial.hpp

#pragma once

// ext
#include <vide/vide.hpp>


namespace libv {
namespace serial {

// -------------------------------------------------------------------------------------------------

/// Calls callback with every member (except for vide::NameValuePair, vide::SizeTag, vide::BinaryData)
template <typename F, bool allow_mutate>
class foreach_archive : public vide::OutputArchive<foreach_archive<F, allow_mutate>> {
private:
	F func;

public:
	template <typename F2>
	explicit inline foreach_archive(F2&& func) :
		func(std::forward<F2>(func)) {}

private:
	template <typename T>
	inline void call_func(T&& var) {
		if constexpr(std::is_invocable_v<F, decltype(var)>) {
			if constexpr(allow_mutate)
				func(const_cast<T&>(var));
			else
				func(var);
		}
	}

public:
	template <typename As, typename T>
	inline void process_as(As& as, const vide::NameValuePair<T>& var) {
		as(var.value);
	}

	template <typename As, typename T>
	inline void process_as(As&, const vide::SizeTag<T>&) {
		// Visitation is no-op, type serializers will call into the archive
	}

	template <typename As, typename T>
	inline void process_as(As&, const vide::BinaryData<T>&) {
		// Visitation is no-op, type serializers will call into the archive
	}

	template <typename As, vide::arithmetic T>
	inline void process_as(As&, const T& var) {
		call_func(var);
	}

	template <typename As, typename T>
	inline void process_as(As& as, const T& var) {
		call_func(var);
		vide::OutputArchive<foreach_archive<F, allow_mutate>>::process_as(as, var);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename F, bool allow_mutate>
class foreach_nvp_archive : public vide::OutputArchive<foreach_nvp_archive<F, allow_mutate>> {
private:
	F func;

public:
	template <typename F2>
	explicit inline foreach_nvp_archive(F2&& func) :
		func(std::forward<F2>(func)) {}

private:
	template <typename T>
	inline void call_func(T&& var) {
		if constexpr(std::is_invocable_v<F, decltype(var)>) {
//			if constexpr(allow_mutate)
//				func(const_cast<T&>(var));
//			else
			func(var);
		}
	}

public:
	using vide::OutputArchive<foreach_nvp_archive<F, allow_mutate>>::process_as;

	template <typename As, typename T>
	inline void process_as(As& as, const vide::NameValuePair<T>& var) {
		call_func(var);
		as(var.value);
	}

	template <typename As, typename T>
	inline void process_as(As&, const vide::SizeTag<T>&) {
		// Visitation is no-op, type serializers will call into the archive
	}

	template <typename As, typename T>
	inline void process_as(As&, const vide::BinaryData<T>&) {
		// Visitation is no-op, type serializers will call into the archive
	}

	template <typename As, vide::arithmetic T>
	inline void process_as(As&, const T&) {
		// Visitation is no-op, type serializers will call into the archive
	}

//	template <typename As, typename T>
//	inline void process_as(As& as, const vide::NameValuePair<T>& var) {
//		call_func(var);
//		as(var.value);
//	}
};

// -------------------------------------------------------------------------------------------------

/// Calls callback with every member (except for vide::NameValuePair, vide::SizeTag, vide::BinaryData)
template <typename T, typename F>
void foreach(T& object, F&& func) {
	auto ar = foreach_archive<F, true>(std::forward<F>(func));
	ar(object);
}

/// Calls callback with every member (except for vide::NameValuePair, vide::SizeTag, vide::BinaryData)
template <typename T, typename F>
void foreach(const T& object, F&& func) {
	auto ar = foreach_archive<F, false>(std::forward<F>(func));
	ar(object);
}


template <typename T, typename F>
void foreach_nvp(T& object, F&& func) {
	auto ar = foreach_nvp_archive<F, true>(std::forward<F>(func));
	ar(object);
}

template <typename T, typename F>
void foreach_nvp(const T& object, F&& func) {
	auto ar = foreach_nvp_archive<F, false>(std::forward<F>(func));
	ar(object);
}

// -------------------------------------------------------------------------------------------------

} // namespace serial
} // namespace libv
