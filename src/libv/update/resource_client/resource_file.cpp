// Project: libv.update, File: src/libv/update/resource_client/resource_file.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource_client/resource_file.lpp>


namespace libv {
namespace update {
namespace {

// -------------------------------------------------------------------------------------------------

struct MetaBlock {
	static constexpr uint64_t marker_value = 0xFFFFFFFF'AAAAAAAA;

	uint64_t offset;
	uint64_t amount;
	uint64_t checksum;
	uint64_t marker;
};

static_assert(sizeof(MetaBlock) == 4 * sizeof(uint64_t));

} // namespace -------------------------------------------------------------------------------------

ResourceFile::~ResourceFile() {
	if (!created)
		return;

	file.close();

	if (done())
		std::filesystem::resize_file(path, resource_size);
}

void ResourceFile::_recover_meta_blocks() {
	file.seekg(resource_size);

	while (true) {
		MetaBlock meta;
		file.read(reinterpret_cast<char*>(&meta), sizeof(meta));

		if (!file || meta.marker != MetaBlock::marker_value) {
			file.clear(); // Clear EOF or FAIL bits if any
			break; // discard rest of the meta blocks
		}

		metadata_cursor += sizeof(meta);
		remaining_intervals.unmark(meta.offset, meta.amount);
	}

	{
		// Clear the corrupted metadata if any left
		char zeros[sizeof(MetaBlock)] = {0};

		file.seekg(0, std::ios::end);
		auto end = file.tellg();

		file.seekp(metadata_cursor);
		auto start = file.tellp();

		for (auto i = start; i < end; i += sizeof(MetaBlock))
			file.write(zeros, sizeof(MetaBlock));
	}
}

std::span<const IntervalRegistry::Interval> ResourceFile::create(std::filesystem::path path_, size_t resource_size_) {
	const auto lock = std::scoped_lock(mutex);
//	if (created)
//		return;

	path = path_;
	resource_size = resource_size_;
	metadata_cursor = resource_size_;
	remaining_intervals.mark(0, resource_size);

	if (std::filesystem::exists(path)) {
		file.open(path, std::ios::binary | std::ios::out | std::ios::in);
		_recover_meta_blocks();
	} else {
		file.open(path, std::ios::binary | std::ios::out);
	}

	created = true;

	return remaining_intervals.marked_intervals();
}

void ResourceFile::write(size_t offset, std::span<const std::byte> data) {
	const auto lock = std::scoped_lock(mutex);
	assert(created);
	assert(offset + data.size() <= resource_size);

	// Write out data chunk
	file.seekp(offset);
	file.write(reinterpret_cast<const char*>(data.data()), data.size());

	// Write out meta data
	MetaBlock meta;
	meta.offset = offset;
	meta.amount = data.size();
	meta.marker = MetaBlock::marker_value;
	file.seekp(metadata_cursor);
	file.write(reinterpret_cast<const char*>(&meta), sizeof(meta));
	metadata_cursor += sizeof(meta);

	// Unmark interval
	remaining_intervals.unmark(offset, data.size());
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv

// =================================================================================================

//	{
//		try {
//			const auto f = libv::read_file_str_or_throw("resource_test_file");
//			const auto s = libv::hex_dump_with_ascii(f);
//			log_update.debug("Before ({}):\n{}\n", f.size(), s);
//		} catch (...) {
//			log_update.debug("No file beforehand");
//		}
//	} {
//		ResourceFile tmp_test_file("resource_test_file");
//		tmp_test_file.create(60);
//		std::string_view data = "Instructions unclear";
//		tmp_test_file.write(30, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(10, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(40, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(20, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
//		tmp_test_file.write(0, {reinterpret_cast<const std::byte*>(data.data()), data.size()});
////		tmp_test_file.close();
//	} {
//		const auto f = libv::read_file_str_or_throw("resource_test_file");
//		const auto s = libv::hex_dump_with_ascii(f);
//		log_update.debug("After ({}):\n{}\n", f.size(), s);
//	}
