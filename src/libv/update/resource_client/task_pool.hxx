// Project: libv.update, File: src/libv/update/resource_client/task_pool.lpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
#include <boost/container/flat_set.hpp>
// libv
#include <libv/mt/binary_latch.hpp>
#include <libv/net/mtcp/connection_he.hpp>
// std
#include <deque>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
// pro
//#include <libv/update/log.hpp>
#include <libv/update/resource_client/resource_task_file.hxx>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ResourceServerPeer;
class ActiveResourceTask;

// -------------------------------------------------------------------------------------------------

class TaskPool : std::enable_shared_from_this<TaskPool> {
	using Connection = libv::net::mtcp::Connection<ResourceServerPeer>;

private:
	std::mutex mutex;
	libv::mt::binary_latch done_bl;

private:
	libv::net::IOContext& io_context;
	std::vector<libv::net::Address> server_addresses;

	boost::container::flat_set<Connection> connections;
	std::vector<Connection> idles;

private:
	std::deque<std::shared_ptr<ResourceTask>> waiting_task_queue;
	std::deque<ActiveResourceTask> active_task_queue;

//	size_t settings_limit_keep_do_not_split = libv::KB(512);
//	size_t settings_limit_always_split = libv::KB(512);
	size_t settings_download_block_size_in_time = 5;

public:
	explicit TaskPool(libv::net::IOContext& io_context, std::vector<libv::net::Address> servers);
	~TaskPool();

private:
	void _assign_next_task_or_enter_idle(Connection connection);
	std::optional<TaskPool::Connection> _grab_idle();

public:
	void queue_task_file(std::string identifier, std::filesystem::path filepath);
//	void queue_task_file(std::string identifier, std::filesystem::path filepath, resource_signature signature);
//	void queue_task_file(std::string identifier, std::filesystem::path filepath, size_t size);
//	void queue_task_file(std::string identifier, std::filesystem::path filepath, size_t size, resource_signature signature);
	void wait();

	void add_active_task(std::shared_ptr<ResourceTask> task, size_t offset, size_t size);
	void task_finished(Connection connection);

	void connect(Connection connection);
	void disconnect(Connection connection);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
