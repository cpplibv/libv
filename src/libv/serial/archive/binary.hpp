// Project: libv.serial, File: src/libv/serial/archive/binary.hpp

#pragma once

// fwd
#include <libv/serial/archive/binary_fwd.hpp>
// ext
#include <cereal/cereal.hpp>
#include <cereal/concept.hpp>
// libv
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
	using Base = cereal::InputArchive<libv::meta::lnv_t<CRTP, BasicBinaryInput<void>>, cereal::AllowEmptyClassElision | cereal::IgnoreNVP>;

private:
	libv::input_bytes input_stream;
	std::size_t input_it = 0;

public:
	/// Construct, loading from the provided stream
	/// @param stream The stream to read from. Should be opened with std::ios::binary flag.
	explicit inline BasicBinaryInput(libv::input_bytes input) :
			input_stream(input) {
	}

	BasicBinaryInput(const BasicBinaryInput&) = delete;
	BasicBinaryInput& operator=(const BasicBinaryInput&) & = delete;
	inline BasicBinaryInput(BasicBinaryInput&&) noexcept = default;
	inline BasicBinaryInput& operator=(BasicBinaryInput&&) & noexcept = default;

	inline ~BasicBinaryInput() noexcept = default;

public:
	[[nodiscard]] inline std::size_t tell_it() const noexcept {
		return input_it;
	}

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

	// --- process_as remapping ------------------------------------------------------------------------
public:
	using Base::process_as;

	template <class As, typename T>
	inline void process_as(As& as, cereal::NameValuePair<T>& t) {
		as(t.value);
	}

	template <class As, typename T>
	inline void process_as(As& as, cereal::SizeTag<T>& t) {
		as(t.size);
	}

	template <class As, cereal::arithmetic T>
	inline void process_as(As&, T& t) {
		static_assert(!std::is_floating_point_v<T> || std::numeric_limits<T>::is_iec559,
				"Portable binary only supports IEEE 754 standardized floating point");

		loadBinary<sizeof(T)>(std::addressof(t), sizeof(t));
	}

	template <class As, class T>
	inline void process_as(As&, cereal::BinaryData<T>& t) {
		using TT = std::remove_pointer_t<T>;

		static_assert(!std::is_floating_point_v<TT> || std::numeric_limits<TT>::is_iec559,
				"Portable binary only supports IEEE 754 standardized floating point");

		loadBinary<sizeof(TT)>(t.data, static_cast<std::streamsize>(t.size));
	}
};

// -------------------------------------------------------------------------------------------------

template <typename CRTP = void>
class BasicBinaryOutput :
		public cereal::OutputArchive<libv::meta::lnv_t<CRTP, BasicBinaryOutput<void>>, cereal::AllowEmptyClassElision | cereal::IgnoreNVP> {
public:
	using ArchiveType = libv::meta::lnv_t<CRTP, BasicBinaryOutput<void>>;
	using Base = cereal::OutputArchive<libv::meta::lnv_t<CRTP, BasicBinaryOutput<void>>, cereal::AllowEmptyClassElision | cereal::IgnoreNVP>;

private:
	libv::output_bytes output_stream;
	std::size_t output_it = 0;

public:
	/// Construct, outputting to the provided stream
	/// @param stream The stream to output to. Should be opened with std::ios::binary flag.
	explicit inline BasicBinaryOutput(libv::output_bytes output) :
			output_stream(output) {
	}

	BasicBinaryOutput(const BasicBinaryOutput&) = delete;
	BasicBinaryOutput& operator=(const BasicBinaryOutput&) & = delete;
	inline BasicBinaryOutput(BasicBinaryOutput&&) noexcept = default;
	inline BasicBinaryOutput& operator=(BasicBinaryOutput&&) & noexcept = default;

	inline ~BasicBinaryOutput() noexcept = default;

public:
	[[nodiscard]] inline std::size_t tell_it() const noexcept {
		return output_it;
	}

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

	// --- process_as remapping ------------------------------------------------------------------------
public:
	using Base::process_as;

	template <class As, typename T>
	inline void process_as(As& as, const cereal::NameValuePair<T>& t) {
		as(t.value);
	}

	template <class As, typename T>
	inline void process_as(As& as, const cereal::SizeTag<T>& t) {
		as(t.size);
	}

	template <class As, cereal::arithmetic T>
	inline void process_as(As&, const T& t) {
		static_assert(!std::is_floating_point_v<T> || std::numeric_limits<T>::is_iec559,
				"Portable binary only supports IEEE 754 standardized floating point");

		saveBinary<sizeof(T)>(std::addressof(t), sizeof(t));
	}

	template <class As, class T>
	inline void process_as(As&, const cereal::BinaryData<T>& t) {
		using TT = std::remove_pointer_t<T>;

		static_assert(!std::is_floating_point_v<TT> || std::numeric_limits<TT>::is_iec559,
				"Portable binary only supports IEEE 754 standardized floating point");

		saveBinary<sizeof(TT)>(t.data, static_cast<std::streamsize>(t.size));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

// register archives for polymorphic support
CEREAL_REGISTER_ARCHIVE(::libv::archive::BasicBinaryOutput<>)
CEREAL_REGISTER_ARCHIVE(::libv::archive::BasicBinaryInput<>)
