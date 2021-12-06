// Project: libv.diff, File: src/libv/diff/diff.cpp

// hpp
#include <libv/diff/diff.hpp>
// ext
#include <hdiffpatch/HDiff/diff.h>
#include <hdiffpatch/HPatch/patch.h>


namespace libv {
namespace diff {
namespace detail {

// -------------------------------------------------------------------------------------------------

static inline auto input_bytes_adaptor(libv::input_bytes& s) noexcept {
	hpatch_TStreamInput stream;
	stream.streamSize = s.size();
	stream.streamImport = reinterpret_cast<void*>(&s);
	stream.read = +[](
			const hpatch_TStreamInput* stream,
			hpatch_StreamPos_t readFromPos,
			unsigned char* out_data,
			unsigned char* out_data_end) {

		const auto out_ptr = reinterpret_cast<std::byte*>(out_data);
		const auto pos = readFromPos;
		const auto size = out_data_end - out_data;

		auto s = reinterpret_cast<libv::input_bytes*>(stream->streamImport);
		s->read(out_ptr, pos, size);
		return hpatch_TRUE;
	};

	return stream;
}

static inline auto output_bytes_adaptor(libv::output_bytes& s) noexcept {
	hpatch_TStreamOutput stream;
	stream.streamSize = s.size();
	stream.streamImport = reinterpret_cast<void*>(&s);
	stream.read_writed = nullptr;
	stream.write = +[](
			const hpatch_TStreamOutput* stream,
			hpatch_StreamPos_t writeToPos,
			const unsigned char* data,
			const unsigned char* data_end) {

		const auto ptr = reinterpret_cast<const std::byte*>(data);
		const auto pos = writeToPos;
		const auto size = data_end - data;

		auto s = reinterpret_cast<libv::output_bytes*>(stream->streamImport);
		s->write(ptr, pos, size);
		return hpatch_TRUE;
	};

	return stream;
}

// -------------------------------------------------------------------------------------------------

void aux_create_diff(libv::input_bytes old, libv::input_bytes new_, libv::output_bytes out_diff, std::size_t match_block_size) {
	auto old_stream = input_bytes_adaptor(old);
	auto new_stream = input_bytes_adaptor(new_);
	auto diff_stream = output_bytes_adaptor(out_diff);

	create_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr, match_block_size);
}

diff_info aux_get_diff_info(libv::input_bytes diff) {
	auto diff_stream = input_bytes_adaptor(diff);

	hpatch_compressedDiffInfo info;
	const auto success = getCompressedDiffInfo(&info, &diff_stream);

	return success ? diff_info{info.oldDataSize, info.newDataSize} : diff_info{};
}

bool aux_check_diff(libv::input_bytes old, libv::input_bytes new_, libv::input_bytes diff) {
	auto old_stream = input_bytes_adaptor(old);
	auto new_stream = input_bytes_adaptor(new_);
	auto diff_stream = input_bytes_adaptor(diff);

	return check_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr);
}

bool apply_patch(libv::input_bytes old, libv::input_bytes diff, libv::output_bytes out_new) {
	const auto diff_info = get_diff_info(diff);

	if (!diff_info)
		return false;

	if (diff_info.new_size != 0)
		// Probe last byte to cause pre allocation
		out_new.write(nullptr, diff_info.new_size, 0);

	auto old_stream = input_bytes_adaptor(old);
	auto diff_stream = input_bytes_adaptor(diff);
	auto new_stream = output_bytes_adaptor(out_new);

	// HDiffPatch API: patch_decompress requires the new stream's size to be exact
	// (To why I have no idea because this is an output and not an input, but there, have it your way)
	new_stream.streamSize = std::min(new_stream.streamSize, diff_info.new_size);

	return patch_decompress(&new_stream, &old_stream, &diff_stream, nullptr);
}

// -------------------------------------------------------------------------------------------------

} // namespace detail
} // namespace diff
} // namespace libv
