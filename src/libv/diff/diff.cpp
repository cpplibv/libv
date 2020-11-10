// Project: libv.diff, File: src/libv/diff/diff.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/diff/diff.hpp>
// ext
#include <hdiffpatch/HDiff/diff.h>
#include <hdiffpatch/HPatch/patch.h>
// std
#include <cstring>
#include <istream>
#include <ostream>


namespace libv {
namespace diff {

// -------------------------------------------------------------------------------------------------

[[nodiscard]] static inline bv as_bv(sv s) noexcept {
	return bv(reinterpret_cast<const std::byte*>(s.data()), s.size());
}

[[nodiscard]] static inline auto as_uc(const std::byte* p) noexcept {
	return reinterpret_cast<const unsigned char*>(p);
}

[[nodiscard]] static inline auto as_uc(std::byte* p) noexcept {
	return reinterpret_cast<unsigned char*>(p);
}

[[nodiscard]] static inline auto as_uc(std::string_view::pointer p) noexcept {
	return reinterpret_cast<unsigned char*>(p);
}

// -------------------------------------------------------------------------------------------------

template <typename T>
static auto write_dyn_fn(
		const hpatch_TStreamOutput* stream,
		hpatch_StreamPos_t writeToPos,
		const unsigned char* out_data,
		const unsigned char* out_data_end) {

	auto vec = reinterpret_cast<T*>(stream->streamImport);
	vec->resize(std::max(vec->size(), writeToPos + (out_data_end - out_data)));
	std::memcpy(vec->data() + writeToPos, out_data, out_data_end - out_data);

	return hpatch_TRUE;
}

static auto read_stream_fn(
		const hpatch_TStreamInput* stream,
		hpatch_StreamPos_t readFromPos,
		unsigned char* out_data,
		unsigned char* out_data_end) {

	const auto out_ptr = reinterpret_cast<char*>(out_data);
	const auto out_num = out_data_end - out_data;

	auto s = reinterpret_cast<std::istream*>(stream->streamImport);

	if (readFromPos != static_cast<hpatch_uint64_t>(s->tellg()))
		s->seekg(readFromPos);

	s->read(out_ptr, out_num);

	return hpatch_TRUE;
}

static auto write_stream_fn(
		const hpatch_TStreamOutput* stream,
		hpatch_StreamPos_t writeToPos,
		const unsigned char* data,
		const unsigned char* data_end) {

	const auto ptr = reinterpret_cast<const char*>(data);
	const auto num = data_end - data;

	auto s = reinterpret_cast<std::ostream*>(stream->streamImport);

	if (writeToPos != static_cast<hpatch_uint64_t>(s->tellp()))
		s->seekp(writeToPos);

	s->write(ptr, num);

	return hpatch_TRUE;
}

// -------------------------------------------------------------------------------------------------

template <typename T>
static inline auto fix_input(const T& view) noexcept {
	hpatch_TStreamInput stream;
	mem_as_hStreamInput(&stream, as_uc(view.data()), as_uc(view.data() + view.size()));
	return stream;
}

template <typename T>
static inline auto fix_output(T& out) noexcept {
	hpatch_TStreamOutput stream;
	mem_as_hStreamOutput(&stream, as_uc(out.data()), as_uc(out.data() + out.size()));
	return stream;
}

template <typename OUT_DYN>
static inline auto dyn_output(OUT_DYN& out) noexcept {
	hpatch_TStreamOutput stream;
	stream.streamImport = reinterpret_cast<void*>(&out);
	stream.streamSize = std::numeric_limits<size_t>::max();
	stream.read_writed = nullptr;
	stream.write = write_dyn_fn<OUT_DYN>;
	return stream;
}

static inline auto stream_input(std::istream& s) noexcept {
	s.seekg(0, std::ios::end);
	const auto stream_size = s.tellg();
	s.seekg(0, std::ios::beg);

	hpatch_TStreamInput stream;
	stream.streamSize = stream_size;
	stream.streamImport = reinterpret_cast<void*>(&s);
	stream.read = read_stream_fn;
	return stream;
}

static inline auto stream_output(std::ostream& s) noexcept {
	hpatch_TStreamOutput stream;
	stream.streamImport = reinterpret_cast<void*>(&s);
	stream.streamSize = std::numeric_limits<size_t>::max();
	stream.read_writed = nullptr;
	stream.write = write_stream_fn;
	return stream;
}

// -------------------------------------------------------------------------------------------------

template <typename OUT_DYN>
static inline void aux_create_diff(bv old, bv new_, OUT_DYN& out_diff, size_t match_block_size) {
	auto old_stream = fix_input(old);
	auto new_stream = fix_input(new_);
	auto diff_stream = dyn_output(out_diff);

	create_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr, match_block_size);
}

static inline void aux_create_diff_stream(std::istream& old, std::istream& new_, std::ostream& diff, size_t match_block_size) {
	auto old_stream = stream_input(old);
	auto new_stream = stream_input(new_);
	auto diff_stream = stream_output(diff);

	create_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr, match_block_size);
}

[[nodiscard]] static inline bool aux_check_diff(bv old, bv new_, bv diff) {
	auto old_stream = fix_input(old);
	auto new_stream = fix_input(new_);
	auto diff_stream = fix_input(diff);

	return check_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr);
}

[[nodiscard]] static inline bool aux_check_diff(std::istream& old, std::istream& new_, std::istream& diff) {
	auto old_stream = stream_input(old);
	auto new_stream = stream_input(new_);
	auto diff_stream = stream_input(diff);

	return check_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr);
}

template <typename OUT_DYN>
[[nodiscard]] static inline bool aux_apply_patch(bv old, bv diff, OUT_DYN& out_new) {
	const auto diff_info = get_diff_info(diff);

	if (!diff_info)
		return false;

	out_new.resize(diff_info.new_size);

	auto old_stream = fix_input(old);
	auto diff_stream = fix_input(diff);
	auto new_stream = fix_output(out_new);

	const auto success = patch_decompress(&new_stream, &old_stream, &diff_stream, nullptr);

	return success;
}

[[nodiscard]] static inline bool aux_apply_patch(std::istream& old, std::istream& diff, std::ostream& new_) {
	const auto diff_info = get_diff_info(diff);

	if (!diff_info)
		return false;

	auto old_stream = stream_input(old);
	auto diff_stream = stream_input(diff);
	auto new_stream = stream_output(new_);

	new_stream.streamSize = diff_info.new_size; // HDiffPatch requires stream size to be set on patch apply

	const auto success = patch_decompress(&new_stream, &old_stream, &diff_stream, nullptr);

	return success;
}

[[nodiscard]] static inline diff_info aux_get_diff_info(bv diff) {
	hpatch_compressedDiffInfo info;
	const auto success = getCompressedDiffInfo_mem(&info, as_uc(diff.data()), as_uc(diff.data() + diff.size()));

	return success ? diff_info{info.oldDataSize, info.newDataSize} : diff_info{};
}

[[nodiscard]] static inline diff_info aux_get_diff_info(std::istream& diff) {
	auto diff_stream = stream_input(diff);

	hpatch_compressedDiffInfo info;
	const auto success = getCompressedDiffInfo(&info, &diff_stream);

	return success ? diff_info{info.oldDataSize, info.newDataSize} : diff_info{};
}

// =================================================================================================

std::string create_diff(sv old, sv new_, size_t match_block_size) {
	std::string result;
	create_diff(old, new_, result, match_block_size);
	return result;
}

std::vector<std::byte> create_diff(bv old, bv new_, size_t match_block_size) {
	std::vector<std::byte> result;
	create_diff(old, new_, result, match_block_size);
	return result;
}

void create_diff(sv old, sv new_, std::string& out_diff, size_t match_block_size) {
	aux_create_diff(as_bv(old), as_bv(new_), out_diff, match_block_size);
}

void create_diff(bv old, bv new_, std::vector<std::byte>& out_diff, size_t match_block_size) {
	aux_create_diff(old, new_, out_diff, match_block_size);
}

void create_diff(std::istream& old, std::istream& new_, std::ostream& diff, size_t match_block_size) {
	aux_create_diff_stream(old, new_, diff, match_block_size);
}

// ---

bool check_diff(sv old, sv new_, sv diff) {
	return check_diff(as_bv(old), as_bv(new_), as_bv(diff));
}

bool check_diff(bv old, bv new_, bv diff) {
	return aux_check_diff(old, new_, diff);
}

bool check_diff(std::istream& old, std::istream& new_, std::istream& diff) {
	return aux_check_diff(old, new_, diff);
}

// ---

std::optional<std::string> apply_patch(sv old, sv diff) {
	std::optional<std::string> result(std::in_place);

	const auto success = apply_patch(old, diff, *result);
	if (!success)
		result.reset();

	return result;
}

std::optional<std::vector<std::byte>> apply_patch(bv old, bv diff) {
	std::optional<std::vector<std::byte>> result(std::in_place);

	const auto success = apply_patch(old, diff, *result);
	if (!success)
		result.reset();

	return result;
}

bool apply_patch(sv old, sv diff, std::string& out_new_) {
	return aux_apply_patch(as_bv(old), as_bv(diff), out_new_);
}

bool apply_patch(bv old, bv diff, std::vector<std::byte>& out_new_) {
	return aux_apply_patch(old, diff, out_new_);
}

bool apply_patch(std::istream& old, std::istream& diff, std::ostream& out_new_) {
	return aux_apply_patch(old, diff, out_new_);
}

// ---

diff_info get_diff_info(sv diff) {
	return get_diff_info(as_bv(diff));
}

diff_info get_diff_info(bv diff) {
	return aux_get_diff_info(diff);
}

diff_info get_diff_info(std::istream& diff) {
	return aux_get_diff_info(diff);
}

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
