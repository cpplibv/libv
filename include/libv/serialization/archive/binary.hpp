// File: portable_binary.hpp Author: Vader Created on 2017. 05. 21., 16:07

#pragma once

// ext
#include <cereal/cereal.hpp>
// std
#include <istream>
#include <limits>
#include <ostream>


namespace libv {
namespace archive {

// -------------------------------------------------------------------------------------------------

namespace detail {

inline bool is_big_endian() {
	volatile uint32_t endian_mark{0x01020304}; // Pinned in memory to read endianness at runtime
	return reinterpret_cast<volatile uint8_t&>(endian_mark) == 1;
}

inline bool is_little_endian() {
	volatile uint32_t endian_mark{0x01020304}; // Pinned in memory to read endianness at runtime
	return reinterpret_cast<volatile uint8_t&>(endian_mark) == 4;
}

template <size_t DataSize>
inline void swap_bytes(uint8_t* data) {
	for (size_t i = 0, end = DataSize / 2; i < end; ++i)
		std::swap(data[i], data[DataSize - i - 1]);
}

} // namespace detail ------------------------------------------------------------------------------

class BinaryOutput : public cereal::OutputArchive<BinaryOutput, cereal::AllowEmptyClassElision> {

private:
	bool convertEndian;
	std::ostream& os;

public:
	/// Construct, outputting to the provided stream
	/// @param stream The stream to output to. Should be opened with std::ios::binary flag.
	BinaryOutput(std::ostream& stream) :
		OutputArchive<BinaryOutput, cereal::AllowEmptyClassElision>(this),
		convertEndian(detail::is_little_endian()),
		os(stream) {
	}

	~BinaryOutput() noexcept = default;

	template <size_t DataSize>
	inline void saveBinary(const void* const data, size_t size) {
		size_t writtenSize = 0;

		if (convertEndian)
			for (size_t i = 0; i < size; i += DataSize)
				for (size_t j = 0; j < DataSize; ++j)
					writtenSize += static_cast<size_t>(os.rdbuf()->sputn(reinterpret_cast<const char*>(data) + DataSize - j - 1 + i, 1));
		else
			writtenSize = static_cast<size_t>(os.rdbuf()->sputn(reinterpret_cast<const char*>(data), size));

		if (writtenSize != size)
			throw cereal::Exception("Failed to write " + std::to_string(size) + " bytes to output stream! Wrote " + std::to_string(writtenSize));
	}
};

// -------------------------------------------------------------------------------------------------

class BinaryInput : public cereal::InputArchive<BinaryInput, cereal::AllowEmptyClassElision> {

private:
	bool convertEndian;
	std::istream& is;

public:
	/// Construct, loading from the provided stream
	/// @param stream The stream to read from. Should be opened with std::ios::binary flag.
	BinaryInput(std::istream& stream) :
		InputArchive<BinaryInput, cereal::AllowEmptyClassElision>(this),
		convertEndian(detail::is_little_endian()),
		is(stream) {
	}

	~BinaryInput() noexcept = default;

	template <size_t DataSize>
	inline void loadBinary(void* const data, size_t size) {
		auto const readSize = static_cast<size_t>(is.rdbuf()->sgetn(reinterpret_cast<char*>(data), size));

		if (readSize != size)
			throw cereal::Exception("Failed to read " + std::to_string(size) + " bytes from input stream! Read " + std::to_string(readSize));

		if (convertEndian)
			for (size_t i = 0; i < size; i += DataSize)
				detail::swap_bytes<DataSize>(reinterpret_cast<uint8_t*>(data) + i);
	}
};

// -------------------------------------------------------------------------------------------------
// Common BinaryArchive serialization functions

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
