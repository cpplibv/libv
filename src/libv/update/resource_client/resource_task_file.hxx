// Project: libv.update, File: src/libv/update/resource_client/resource_task_file.lpp, Author: Császár Mátyás [Vader]

#pragma once

// std
#include <filesystem>
// pro
#include <libv/update/resource_client/resource_task.hxx>
#include <libv/update/resource_client/resource_file.hxx>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ResourceTaskFile : public ResourceTask {
	std::filesystem::path filepath_save;
	std::filesystem::path filepath_temp;

	ResourceFile file;

public:
	ResourceTaskFile(std::string identifier, std::filesystem::path filepath) :
		ResourceTask(std::move(identifier)),
		filepath_save(filepath),
		filepath_temp(filepath) {
		filepath_temp += ".temp";
	}

private:
	virtual interval_span do_start(uint64_t size) override {
		return file.create(filepath_temp, size);
	}

	virtual void do_write(uint64_t offset, std::span<const std::byte> data) override {
		file.write(offset, data);
	}

	virtual void do_done() override {
//		file.done();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
