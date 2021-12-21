// Project: libv.serial, File: src/libv/serial/archive/json_any.hpp

#pragma once

// fwd
#include <libv/serial/archive/json_any_fwd.hpp>
// ext
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
// libv
#include <libv/meta/derived_from_specialization.hpp>
#include <libv/utility/bytes/input_bytes.hpp>
#include <libv/utility/bytes/output_bytes.hpp>
// std
#include <optional>
#include <sstream>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

namespace detail {

struct JSONAnyInputTempStream {
	std::istringstream temp_ss_stream;
	explicit inline JSONAnyInputTempStream(std::string&& str) : temp_ss_stream(std::move(str)) {}
};

} // namespace detail

// -------------------------------------------------------------------------------------------------

/// Not efficient temporary implementation that uses an additional string stream to bypass std / cereal stream limitations
/// Remove when possible
template <typename CRTP = void>
class BasicJSONAnyInput : private detail::JSONAnyInputTempStream, public cereal::JSONInputArchive {
private:
	static std::string read_all_from_input_stream(libv::input_bytes& input_stream) {
		std::string result;
		result.resize(input_stream.size());
		input_stream.read(reinterpret_cast<std::byte*>(result.data()), 0, input_stream.size());
		return result;
	}

public:
	explicit BasicJSONAnyInput(libv::input_bytes input_stream) :
		detail::JSONAnyInputTempStream{read_all_from_input_stream(input_stream)},
		JSONInputArchive(temp_ss_stream) {
	}
};

/// Not efficient temporary implementation that uses an additional string stream to bypass std / cereal stream limitations
/// Remove when possible
template <typename CRTP = void>
class BasicJSONAnyOutput : public cereal::OutputArchive<libv::meta::lnv_t<CRTP, BasicJSONAnyOutput<void>>>, public cereal::traits::TextArchive {
public:
	using ArchiveType = libv::meta::lnv_t<CRTP, BasicJSONAnyOutput<void>>;
	using Options = cereal::JSONOutputArchive::Options;

private:
	libv::output_bytes output_stream;
	std::ostringstream temp_ss_stream;
	std::optional<cereal::JSONOutputArchive> impl;

public:
	explicit BasicJSONAnyOutput(libv::output_bytes output_stream, const Options& options = Options::Default()) :
		cereal::OutputArchive<ArchiveType>(static_cast<ArchiveType*>(this)),
		output_stream(output_stream),
		impl(std::in_place, this->temp_ss_stream, options) {
	}

	~BasicJSONAnyOutput() {
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

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void prologue(Archive&, cereal::NameValuePair<T> const&) {}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void epilogue(Archive&, cereal::NameValuePair<T> const&) {}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void prologue(Archive&, cereal::DeferredData<T> const&) {}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void epilogue(Archive&, cereal::DeferredData<T> const&) {}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void prologue(Archive& ar, cereal::SizeTag<T> const&) {
	ar.makeArray();
}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void epilogue(Archive&, cereal::SizeTag<T> const&) {}

template <typename Archive, class T>
	requires (libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput> &&
		!std::is_arithmetic_v<T> &&
		!cereal::traits::has_minimal_base_class_serialization<T, cereal::traits::has_minimal_output_serialization, Archive>::value &&
		!cereal::traits::has_minimal_output_serialization<T, Archive>::value)
inline void prologue(Archive& ar, T const&) {
	ar.startNode();
}

template <typename Archive, class T>
	requires (libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput> &&
		!std::is_arithmetic_v<T> &&
		!cereal::traits::has_minimal_base_class_serialization<T, cereal::traits::has_minimal_output_serialization, Archive>::value &&
		!cereal::traits::has_minimal_output_serialization<T, Archive>::value)
inline void epilogue(Archive& ar, T const&) {
	ar.finishNode();
}

template <typename Archive>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void prologue(Archive& ar, std::nullptr_t const&) {
	ar.writeName();
}

template <typename Archive>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void epilogue(Archive&, std::nullptr_t const&) {}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput> &&
			std::is_arithmetic_v<T>
inline void prologue(Archive& ar, T const&) {
	ar.writeName();
}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput> &&
			std::is_arithmetic_v<T>
inline void epilogue(Archive&, T const&) {}

template <typename Archive, class CharT, class Traits, class Alloc>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void prologue(Archive& ar, std::basic_string<CharT, Traits, Alloc> const&) {
	ar.writeName();
}

template <typename Archive, class CharT, class Traits, class Alloc>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void epilogue(Archive&, std::basic_string<CharT, Traits, Alloc> const&) {}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, cereal::NameValuePair<T> const& t) {
	ar.setNextName(t.name);
	ar(t.value);
}

template <typename Archive>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, std::nullptr_t const& t) {
	ar.saveValue(t);
}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput> &&
			std::is_arithmetic_v<T>
inline void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, T const& t) {
	ar.saveValue(t);
}

template <typename Archive, class CharT, class Traits, class Alloc>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, std::basic_string<CharT, Traits, Alloc> const& str) {
	ar.saveValue(str);
}

template <typename Archive, class T>
	requires libv::meta::derived_from_specialization<Archive, BasicJSONAnyOutput>
inline void CEREAL_SAVE_FUNCTION_NAME(Archive&, cereal::SizeTag<T> const&) {
	// nothing to do here, we don't explicitly save the size
}

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

CEREAL_REGISTER_ARCHIVE(::libv::archive::BasicJSONAnyInput<>)
CEREAL_REGISTER_ARCHIVE(::libv::archive::BasicJSONAnyOutput<>)
