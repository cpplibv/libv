// Project: libv.serial, File: src/libv/serial/archive/json_any.hpp

#pragma once

// fwd
#include <libv/serial/archive/json_any_fwd.hpp>
// ext
#include <vide/archives/json.hpp>
#include <vide/vide.hpp>
// libv
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
class BasicJSONAnyInput : private detail::JSONAnyInputTempStream, public vide::JSONInputArchive {
private:
	static std::string read_all_from_input_stream(libv::input_bytes& input_stream) {
		std::string result;
		result.resize(input_stream.size());
		input_stream.read(reinterpret_cast<std::byte*>(result.data()), 0, input_stream.size());
		return result;
	}

public:
	explicit inline BasicJSONAnyInput(libv::input_bytes input_stream) :
		detail::JSONAnyInputTempStream{read_all_from_input_stream(input_stream)},
		JSONInputArchive(temp_ss_stream) {
	}

	BasicJSONAnyInput(const BasicJSONAnyInput&) = delete;
	BasicJSONAnyInput& operator=(const BasicJSONAnyInput&) & = delete;
	inline BasicJSONAnyInput(BasicJSONAnyInput&&) noexcept = default;
	inline BasicJSONAnyInput& operator=(BasicJSONAnyInput&&) & noexcept = default;

	inline ~BasicJSONAnyInput() noexcept = default;

//public:
//	[[nodiscard]] inline std::size_t tell_it() const noexcept {
//		return temp_ss_stream.tellp();
//	}
};

/// Not efficient temporary implementation that uses an additional string stream to bypass std / cereal stream limitations
/// Remove when possible
template <typename CRTP = void>
class BasicJSONAnyOutput : public vide::OutputArchive<libv::meta::lnv_t<CRTP, BasicJSONAnyOutput<void>>, vide::TextArchive> {
public:
	using ArchiveType = libv::meta::lnv_t<CRTP, BasicJSONAnyOutput<void>>;
	using Options = vide::JSONOutputArchive::Options;

private:
	libv::output_bytes output_stream;
	std::ostringstream temp_ss_stream;
	std::optional<vide::JSONOutputArchive> impl;

public:
	explicit inline BasicJSONAnyOutput(libv::output_bytes output_stream, const Options& options = Options::Default()) :
		output_stream(output_stream),
		impl(std::in_place, this->temp_ss_stream, options) {
	}

	BasicJSONAnyOutput(const BasicJSONAnyOutput&) = delete;
	BasicJSONAnyOutput& operator=(const BasicJSONAnyOutput&) & = delete;
	inline BasicJSONAnyOutput(BasicJSONAnyOutput&&) noexcept = default;
	inline BasicJSONAnyOutput& operator=(BasicJSONAnyOutput&&) & noexcept = default;

	inline ~BasicJSONAnyOutput() {
		impl.reset();
		const auto result = std::move(temp_ss_stream).str();
		output_stream.write(reinterpret_cast<const std::byte*>(result.data()), 0, result.size());
	}

//public:
//	[[nodiscard]] inline std::size_t tell_it() const noexcept {
//		return temp_ss_stream.tellp();
//	}

public:
	template <typename As, typename T>
	inline void process_as(As& as, T&& var) {
		impl->process_as(as, std::forward<T>(var));
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace archive
} // namespace libv

VIDE_REGISTER_ARCHIVE(::libv::archive::BasicJSONAnyInput<>)
VIDE_REGISTER_ARCHIVE(::libv::archive::BasicJSONAnyOutput<>)
