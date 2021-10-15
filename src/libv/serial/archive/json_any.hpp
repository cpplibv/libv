// Project: libv.serial, File: src/libv/serial/archive/json_any.hpp

#pragma once

// ext
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
// libv
#include <libv/utility/bytes/input_bytes.hpp>
#include <libv/utility/bytes/output_bytes.hpp>
// std
#include <sstream>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct JSONInputAnyTempStream {
	std::istringstream temp_ss_stream;
	explicit inline JSONInputAnyTempStream(std::string&& str) : temp_ss_stream(std::move(str)) {}
};

} // namespace detail

/// Not efficient temporary implementation that uses an additional string stream to bypass std / cereal stream limitations
/// Remove when possible
class JSONInputAny : private detail::JSONInputAnyTempStream, public cereal::JSONInputArchive {
private:
	static std::string read_all_from_input_stream(libv::input_bytes& input_stream) {
		std::string result;
		result.resize(input_stream.size());
		input_stream.read(reinterpret_cast<std::byte*>(result.data()), 0, input_stream.size());
		return result;
	}

public:
	explicit JSONInputAny(libv::input_bytes input_stream) :
		detail::JSONInputAnyTempStream{read_all_from_input_stream(input_stream)},
		JSONInputArchive(temp_ss_stream) {
	}
};

/// Not efficient temporary implementation that uses an additional string stream to bypass std / cereal stream limitations
/// Remove when possible
class JSONOutputAny : public cereal::OutputArchive<JSONOutputAny>, public cereal::traits::TextArchive {
public:
	using Options = cereal::JSONOutputArchive::Options;

private:
	libv::output_bytes output_stream;
	std::ostringstream temp_ss_stream;
	std::optional<cereal::JSONOutputArchive> impl;

public:
	explicit JSONOutputAny(libv::output_bytes output_stream, const Options& options = Options::Default()) :
		OutputArchive<JSONOutputAny>(this),
		output_stream(output_stream),
		impl(std::in_place, this->temp_ss_stream, options) {
	}

	~JSONOutputAny() {
		impl.reset();
		const auto result = std::move(temp_ss_stream).str();
		output_stream.write(reinterpret_cast<const std::byte*>(result.data()), 0, result.size());
	}

public:
	template <class T>
	inline void saveValue(const T& t) {
		impl->saveValue(t);
	}

	inline void makeArray() {
		impl->makeArray();
	}

	inline void startNode() {
		impl->startNode();
	}

	inline void finishNode() {
		impl->finishNode();
	}

	inline void writeName() {
		impl->writeName();
	}

	inline void setNextName(const char* name) {
		impl->setNextName(name);
	}
};

// -------------------------------------------------------------------------------------------------

template <class T>
inline void prologue(JSONOutputAny&, cereal::NameValuePair<T> const&) {}

template <class T>
inline void epilogue(JSONOutputAny&, cereal::NameValuePair<T> const&) {}

template <class T>
inline void prologue(JSONOutputAny&, cereal::DeferredData<T> const&) {}

template <class T>
inline void epilogue(JSONOutputAny&, cereal::DeferredData<T> const&) {}

template <class T>
inline void prologue(JSONOutputAny& ar, cereal::SizeTag<T> const&) {
	ar.makeArray();
}

template <class T>
inline void epilogue(JSONOutputAny&, cereal::SizeTag<T> const&) {}

template <class T, cereal::traits::EnableIf<!std::is_arithmetic<T>::value,
		!cereal::traits::has_minimal_base_class_serialization<T, cereal::traits::has_minimal_output_serialization, JSONOutputAny>::value,
		!cereal::traits::has_minimal_output_serialization<T, JSONOutputAny>::value> = cereal::traits::sfinae>
inline void prologue(JSONOutputAny& ar, T const&) {
	ar.startNode();
}

template <class T, cereal::traits::EnableIf<!std::is_arithmetic<T>::value,
		!cereal::traits::has_minimal_base_class_serialization<T, cereal::traits::has_minimal_output_serialization, JSONOutputAny>::value,
		!cereal::traits::has_minimal_output_serialization<T, JSONOutputAny>::value> = cereal::traits::sfinae>
inline void epilogue(JSONOutputAny& ar, T const&) {
	ar.finishNode();
}

inline void prologue(JSONOutputAny& ar, std::nullptr_t const&) {
	ar.writeName();
}

inline void epilogue(JSONOutputAny&, std::nullptr_t const&) {}

template <class T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = cereal::traits::sfinae>
inline void prologue(JSONOutputAny& ar, T const&) {
	ar.writeName();
}

template <class T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = cereal::traits::sfinae>
inline void epilogue(JSONOutputAny&, T const&) {}

template <class CharT, class Traits, class Alloc>
inline void prologue(JSONOutputAny& ar, std::basic_string<CharT, Traits, Alloc> const&) {
	ar.writeName();
}

template <class CharT, class Traits, class Alloc>
inline void epilogue(JSONOutputAny&, std::basic_string<CharT, Traits, Alloc> const&) {}

template <class T>
inline void CEREAL_SAVE_FUNCTION_NAME(JSONOutputAny& ar, cereal::NameValuePair<T> const& t) {
	ar.setNextName(t.name);
	ar(t.value);
}


inline void CEREAL_SAVE_FUNCTION_NAME(JSONOutputAny& ar, std::nullptr_t const& t) {
	ar.saveValue(t);
}

template <class T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = cereal::traits::sfinae>
inline void CEREAL_SAVE_FUNCTION_NAME(JSONOutputAny& ar, T const& t) {
	ar.saveValue(t);
}

template <class CharT, class Traits, class Alloc>
inline void CEREAL_SAVE_FUNCTION_NAME(JSONOutputAny& ar, std::basic_string<CharT, Traits, Alloc> const& str) {
	ar.saveValue(str);
}

template <class T>
inline void CEREAL_SAVE_FUNCTION_NAME(JSONOutputAny&, cereal::SizeTag<T> const&) {
	// nothing to do here, we don't explicitly save the size
}

// =================================================================================================

struct JSONAny {
	using input = JSONInputAny;
	using output = JSONOutputAny;
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

CEREAL_REGISTER_ARCHIVE(::libv::archive::JSONInputAny)
CEREAL_REGISTER_ARCHIVE(::libv::archive::JSONOutputAny)

CEREAL_SETUP_ARCHIVE_TRAITS(::libv::archive::JSONInputAny, ::libv::archive::JSONOutputAny)
