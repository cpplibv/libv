// Project: libv.serial, File: src/libv/serial/archive/binary.hpp

#pragma once

// fwd
#include <libv/serial/archive/binary_fwd.hpp>
// ext
#include <cereal/cereal.hpp>
// libv
#include <libv/utility/bytes/input_bytes.hpp>
#include <libv/utility/bytes/output_bytes.hpp>
#include <libv/utility/endian.hpp>
// std
#include <cstring>
#include <limits>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

namespace detail {

template <size_t DataSize>
inline void swap_bytes(std::byte* data) {
	for (size_t i = 0, end = DataSize / 2; i < end; ++i)
		std::swap(data[i], data[DataSize - i - 1]);
}

} // namespace detail ------------------------------------------------------------------------------

class BinaryInput : public cereal::InputArchive<BinaryInput, cereal::AllowEmptyClassElision> {
private:
	libv::input_bytes input_stream;
	size_t input_it = 0;

public:
	/// Construct, loading from the provided stream
	/// @param stream The stream to read from. Should be opened with std::ios::binary flag.
	explicit BinaryInput(libv::input_bytes input) :
		InputArchive<BinaryInput, cereal::AllowEmptyClassElision>(this),
		input_stream(input) {
	}

	~BinaryInput() noexcept = default;

	template <size_t DataSize>
	inline void loadBinary(void* const data, size_t size) {
		const auto readSize = input_stream.read(reinterpret_cast<std::byte*>(data), input_it, size);
		input_it += size; // (readSize would yield same result)

		if (readSize != size)
			throw cereal::Exception("Failed to read " + std::to_string(size) + " bytes from input stream! Read " + std::to_string(readSize));

		if constexpr (!is_network_endian())
			for (size_t i = 0; i < size; i += DataSize)
				detail::swap_bytes<DataSize>(reinterpret_cast<std::byte*>(data) + i);
	}
};

// -------------------------------------------------------------------------------------------------

class BinaryOutput : public cereal::OutputArchive<BinaryOutput, cereal::AllowEmptyClassElision> {
private:
	libv::output_bytes output_stream;
	size_t output_it = 0;

public:
	/// Construct, outputting to the provided stream
	/// @param stream The stream to output to. Should be opened with std::ios::binary flag.
	explicit BinaryOutput(libv::output_bytes output) :
		OutputArchive<BinaryOutput, cereal::AllowEmptyClassElision>(this),
		output_stream(output) {
	}

	~BinaryOutput() noexcept = default;

	template <size_t DataSize>
	inline void saveBinary(const void* const data, size_t size) {
//		size_t writtenSize = 0;

		if (output_stream.size() < output_it + size)
			throw cereal::Exception("Failed to write " + std::to_string(size) + " bytes to output stream! Output stream only has " + std::to_string(output_stream.size() - output_it) + " byte storage left");

		if constexpr (!is_network_endian() && DataSize != 1) {
			for (size_t i = 0; i < size; i += DataSize) {
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

/// Saving for POD types to portable binary
template<class T> inline
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
CEREAL_SAVE_FUNCTION_NAME(BinaryOutput& ar, T const& t) {
	static_assert( !std::is_floating_point<T>::value ||
			(std::is_floating_point<T>::value && std::numeric_limits<T>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");
	ar.template saveBinary<sizeof (T)>(std::addressof(t), sizeof (t));
}

/// Loading for POD types from portable binary
template<class T> inline
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
CEREAL_LOAD_FUNCTION_NAME(BinaryInput& ar, T& t) {
	static_assert( !std::is_floating_point<T>::value ||
			(std::is_floating_point<T>::value && std::numeric_limits<T>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");
	ar.template loadBinary<sizeof (T)>(std::addressof(t), sizeof (t));
}

/// Serializing NVP types to portable binary
template <class Archive, class T> inline
CEREAL_ARCHIVE_RESTRICT(BinaryInput, BinaryOutput)
CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, cereal::NameValuePair<T>& t) {
	ar(t.value);
}

/// Serializing SizeTags to portable binary
template <class Archive, class T> inline
CEREAL_ARCHIVE_RESTRICT(BinaryInput, BinaryOutput)
CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, cereal::SizeTag<T>& t) {
	ar(t.size);
}

/// Saving binary data to portable binary
template <class T> inline
void CEREAL_SAVE_FUNCTION_NAME(BinaryOutput& ar, cereal::BinaryData<T> const& bd) {
	using TT = typename std::remove_pointer<T>::type;
	static_assert( !std::is_floating_point<TT>::value ||
			(std::is_floating_point<TT>::value && std::numeric_limits<TT>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");

	ar.template saveBinary<sizeof (TT)>(bd.data, static_cast<size_t>(bd.size));
}

/// Loading binary data from portable binary
template <class T> inline
void CEREAL_LOAD_FUNCTION_NAME(BinaryInput& ar, cereal::BinaryData<T>& bd) {
	using TT = typename std::remove_pointer<T>::type;
	static_assert( !std::is_floating_point<TT>::value ||
			(std::is_floating_point<TT>::value && std::numeric_limits<TT>::is_iec559),
			"Portable binary only supports IEEE 754 standardized floating point");

	ar.template loadBinary<sizeof (TT)>(bd.data, static_cast<size_t>(bd.size));
}

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

// register archives for polymorphic support
CEREAL_REGISTER_ARCHIVE(::libv::archive::BinaryOutput)
CEREAL_REGISTER_ARCHIVE(::libv::archive::BinaryInput)

// tie input and output archives together
CEREAL_SETUP_ARCHIVE_TRAITS(::libv::archive::BinaryInput, ::libv::archive::BinaryOutput)
