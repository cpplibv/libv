// Project: libv.diff, File: src/libv/diff/diff.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/diff/diff.hpp>
// ext
#include <hdiffpatch/HDiff/diff.h>
#include <hdiffpatch/HPatch/patch.h>
// std
#include <cstring>


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

// ---

template <typename T>
auto write_dyn_fn(
		const hpatch_TStreamOutput* stream,
		hpatch_StreamPos_t writeToPos,
		const unsigned char* out_data,
		const unsigned char* out_data_end) {

	auto vec = reinterpret_cast<T*>(stream->streamImport);
	vec->resize(std::max(vec->size(), writeToPos + (out_data_end - out_data)));
	std::memcpy(vec->data() + writeToPos, out_data, out_data_end - out_data);

	return hpatch_TRUE;
};

template <typename T>
auto read_back_fn(
		const hpatch_TStreamOutput* stream,
		hpatch_StreamPos_t readFromPos,
		unsigned char* data,
		unsigned char* data_end) {

	auto vec = reinterpret_cast<T*>(stream->streamImport);
	std::memcpy(data, vec->data() + readFromPos, data_end - data);

	return hpatch_TRUE;
};

// ---

template <typename OUT_DYN>
static inline void aux_create_diff(bv old, bv new_, OUT_DYN& out_diff, size_t match_block_size) {
	hpatch_TStreamInput old_stream;
	mem_as_hStreamInput(&old_stream, as_uc(old.data()), as_uc(old.data() + old.size()));

	hpatch_TStreamInput new_stream;
	mem_as_hStreamInput(&new_stream, as_uc(new_.data()), as_uc(new_.data() + new_.size()));

	hpatch_TStreamOutput diff_stream;
	diff_stream.streamImport = reinterpret_cast<void*>(&out_diff);
	diff_stream.streamSize = std::numeric_limits<size_t>::max();
	diff_stream.write = write_dyn_fn<OUT_DYN>;
	diff_stream.read_writed = read_back_fn<OUT_DYN>;

	create_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr, match_block_size);
}

[[nodiscard]] static inline bool aux_check_diff(bv old, bv new_, bv diff) {
	hpatch_TStreamInput old_stream;
	mem_as_hStreamInput(&old_stream, as_uc(old.data()), as_uc(old.data() + old.size()));

	hpatch_TStreamInput new_stream;
	mem_as_hStreamInput(&new_stream, as_uc(new_.data()), as_uc(new_.data() + new_.size()));

	hpatch_TStreamInput diff_stream;
	mem_as_hStreamInput(&diff_stream, as_uc(diff.data()), as_uc(diff.data() + diff.size()));

	return check_compressed_diff_stream(&new_stream, &old_stream, &diff_stream, nullptr);
}

template <typename OUT_DYN>
[[nodiscard]] static inline bool aux_apply_patch(bv old, bv diff, OUT_DYN& out_new) {

	const auto diff_info = get_diff_info(diff);

	if (!diff_info)
		return false;

	out_new.resize(diff_info.new_size);

	hpatch_TStreamInput old_stream;
	mem_as_hStreamInput(&old_stream, as_uc(old.data()), as_uc(old.data() + old.size()));

	hpatch_TStreamInput diff_stream;
	mem_as_hStreamInput(&diff_stream, as_uc(diff.data()), as_uc(diff.data() + diff.size()));

	hpatch_TStreamOutput new_stream;
	mem_as_hStreamOutput(&new_stream, as_uc(out_new.data()), as_uc(out_new.data() + out_new.size()));

	const auto success = patch_decompress(&new_stream, &old_stream, &diff_stream, nullptr);

	return success;
}

[[nodiscard]] static inline diff_info aux_get_diff_info(bv diff) {
	diff_info result;

	hpatch_compressedDiffInfo info;
	const auto success = getCompressedDiffInfo_mem(&info, as_uc(diff.data()), as_uc(diff.data() + diff.size()));

	if (success) {
		result.new_size = info.newDataSize;
		result.old_size = info.oldDataSize;
	} else {
		result.reset();
	}

	return result;
}

// -------------------------------------------------------------------------------------------------

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
	assert(false);
}

// ---

bool check_diff(sv old, sv new_, sv diff) {
	return check_diff(as_bv(old), as_bv(new_), as_bv(diff));
}

bool check_diff(bv old, bv new_, bv diff) {
	return aux_check_diff(old, new_, diff);
}

bool check_diff(std::istream& old, std::istream& new_, std::istream& diff) {
	assert(false); return false;
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

bool apply_patch(std::istream& old, std::istream& diff, std::ostream& new_) {
	assert(false); return false;
}

// ---

diff_info get_diff_info(sv diff) {
	return get_diff_info(as_bv(diff));
}

diff_info get_diff_info(bv diff) {
	return aux_get_diff_info(diff);
}

// -------------------------------------------------------------------------------------------------

} // namespace diff
} // namespace libv
