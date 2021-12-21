// Project: libv.serial, File: src/libv/serial/archive/binary.hpp

#pragma once

// fwd
#include <libv/serial/archive/binary_fwd.hpp>
// ext
#include <cereal/cereal.hpp>
// libv
#include <libv/meta/derived_from_specialization.hpp>
#include <libv/meta/lnv.hpp>
#include <libv/utility/bytes/input_bytes.hpp>
#include <libv/utility/bytes/output_bytes.hpp>
#include <libv/utility/endian.hpp>
// std
#include <cstring>
#include <limits>
#include <type_traits>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <std::size_t DataSize>
inline void swap_bytes(std::byte* data) {
	for (std::size_t i = 0, end = DataSize / 2; i < end; ++i)
		std::swap(data[i], data[DataSize - i - 1]);
}

} // namespace detail ------------------------------------------------------------------------------

template <typename CRTP = void>
class BasicBinaryInput :
		public cereal::InputArchive<libv::meta::lnv_t<CRTP, BasicBinaryInput<void>>, cereal::AllowEmptyClassElision | cereal::IgnoreNVP> {
public:
	using ArchiveType = libv::meta::lnv_t<CRTP, BasicBinaryInput<void>>;

private:
	libv::input_bytes input_stream;
	std::size_t input_it = 0;

public:
	/// Construct, loading from the provided stream
	/// @param stream The stream to read from. Should be opened with std::ios::binary flag.
	explicit BasicBinaryInput(libv::input_bytes input) :
			cereal::InputArchive<ArchiveType, cereal::AllowEmptyClassElision | cereal::IgnoreNVP>(static_cast<ArchiveType*>(this)),
			input_stream(input) {
	}

	~BasicBinaryInput() noexcept = default;

	template <std::size_t DataSize>
	inline void loadBinary(void* const data, std::size_t size) {
		const auto readSize = input_stream.read(reinterpret_cast<std::byte*>(data), input_it, size);
		input_it += size; // (readSize would yield same result)

		if (readSize != size)
			throw cereal::Exception("Failed to read " + std::to_string(size) + " bytes from input stream! Read " + std::to_string(readSize));

		if constexpr (!is_network_endian())
			for (std::size_t i = 0; i < size; i += DataSize)
				detail::swap_bytes<DataSize>(reinterpret_cast<std::byte*>(data) + i);
	}
};

// -------------------------------------------------------------------------------------------------

template <typename CRTP = void>
class BasicBinaryOutput :
		public cereal::OutputArchive<libv::meta::lnv_t<CRTP, BasicBinaryOutput<void>>, cereal::AllowEmptyClassElision | cereal::IgnoreNVP> {
public:
	using ArchiveType = libv::meta::lnv_t<CRTP, BasicBinaryOutput<void>>;

private:
	libv::output_bytes output_stream;
	std::size_t output_it = 0;

public:
	/// Construct, outputting to the provided stream
	/// @param stream The stream to output to. Should be opened with std::ios::binary flag.
	explicit BasicBinaryOutput(libv::output_bytes output) :
			cereal::OutputArchive<ArchiveType, cereal::AllowEmptyClassElision | cereal::IgnoreNVP>(static_cast<ArchiveType*>(this)),
			output_stream(output) {
	}

	~BasicBinaryOutput() noexcept = default;

	template <std::size_t DataSize>
	inline void saveBinary(const void* const data, std::size_t size) {
//		std::size_t writtenSize = 0;

		if (output_stream.size() < output_it + size)
			throw cereal::Exception("Failed to write " + std::to_string(size) + " bytes to output stream! Output stream only has " + std::to_string(output_stream.size() - output_it) + " byte storage left");

		if constexpr (!is_network_endian() && DataSize != 1) {
			for (std::size_t i = 0; i < size; i += DataSize) {
				std::byte swap_buffer[DataSize];
				std::memcpy(swap_buffer, reinterpret_cast<const std::byte*>(data) + i, DataSize);
				detail::swap_bytes<DataSize>(swap_buffer);

				output_stream.write(swap_buffer, output_it, DataSize);
				output_it += DataSize;
			}
		} else {
			output_stream.write(reinterpret_cast<const std::byte*>(data), output_it, size);
			output_it += size;
		}

//		if (writtenSize != size)
//			throw cereal::Exception("Failed to write " + std::to_string(size) + " bytes to output stream! Wrote " + std::to_string(writtenSize));
	}
};

// -------------------------------------------------------------------------------------------------
// Common BinaryArchive serialization functions (Found by ADL)

/// Loading for POD types from portable binary
template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryInput>
inline typename std::enable_if<std::is_arithmetic<T>::value, void>::type
CEREAL_LOAD_FUNCTION_NAME(Archive& ar, T& t) {
	static_assert(!std::is_floating_point<T>::value ||
					(std::is_floating_point<T>::value && std::numeric_limits<T>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");
	ar.template loadBinary<sizeof(T)>(std::addressof(t), sizeof(t));
}

/// Saving for POD types to portable binary
template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryOutput>
inline typename std::enable_if<std::is_arithmetic<T>::value, void>::type
CEREAL_SAVE_FUNCTION_NAME(Archive& ar, T const& t) {
	static_assert(!std::is_floating_point<T>::value ||
					(std::is_floating_point<T>::value && std::numeric_limits<T>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");
	ar.template saveBinary<sizeof(T)>(std::addressof(t), sizeof(t));
}

/// Serializing NVP types to portable binary
template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryInput>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, cereal::NameValuePair<T>& t) {
	ar(t.value);
}

template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryOutput>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, cereal::NameValuePair<T>& t) {
	ar(t.value);
}

/// Serializing SizeTags to portable binary
template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryInput>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, cereal::SizeTag<T>& t) {
	ar(t.size);
}

template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryOutput>
inline void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, cereal::SizeTag<T>& t) {
	ar(t.size);
}

/// Loading binary data from portable binary
template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryInput>
inline void CEREAL_LOAD_FUNCTION_NAME(Archive& ar, cereal::BinaryData<T>& bd) {
	using TT = typename std::remove_pointer<T>::type;
	static_assert(!std::is_floating_point<TT>::value ||
					(std::is_floating_point<TT>::value && std::numeric_limits<TT>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");

	ar.template loadBinary<sizeof(TT)>(bd.data, static_cast<std::size_t>(bd.size));
}

/// Saving binary data to portable binary
template <typename Archive, typename T>
	requires libv::meta::derived_from_specialization<Archive, BasicBinaryOutput>
inline void CEREAL_SAVE_FUNCTION_NAME(Archive& ar, cereal::BinaryData<T> const& bd) {
	using TT = typename std::remove_pointer<T>::type;
	static_assert(!std::is_floating_point<TT>::value ||
					(std::is_floating_point<TT>::value && std::numeric_limits<TT>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");

	ar.template saveBinary<sizeof(TT)>(bd.data, static_cast<std::size_t>(bd.size));
}

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

// register archives for polymorphic support
CEREAL_REGISTER_ARCHIVE(::libv::archive::BasicBinaryOutput<>)
CEREAL_REGISTER_ARCHIVE(::libv::archive::BasicBinaryInput<>)
