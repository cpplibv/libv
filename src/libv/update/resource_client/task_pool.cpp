// Project: libv.update, File: src/libv/update/resource_client/task_pool.cpp

// hpp
#include <libv/update/resource_client/task_pool.hxx>
// libv
#include <libv/algo/erase_unstable.hpp>
// std
#include <algorithm>
#include <random>
// pro
#include <libv/update/resource_client/resource_server_peer.hxx>

//// std
//#include <deque>
//#include <fstream>
//// pro
////#include <libv/update/log.hpp>
//#include <libv/update/resource_client/resource_file.hxx>
//#include <libv/update/common/protocol_res.hpp>
//
//#include <libv/net/io_context.hpp>
//#include <libv/net/mtcp/connection_he.hpp>
//
//#include <libv/utility/read_file.hpp>
//#include <libv/utility/hex_dump.hpp>
//#include <utility>



namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct ActiveResourceTask {
	std::shared_ptr<ResourceTask> base_task;
	uint64_t offset;
	uint64_t size;
};

// -------------------------------------------------------------------------------------------------

TaskPool::TaskPool(libv::net::IOContext& io_context, std::vector<libv::net::Address> servers_) :
	io_context(io_context),
	server_addresses(std::move(servers_)) {

	std::random_device rd; // Expensive random number engine (but only a few uses, not worth to spin up a mt)
	std::ranges::shuffle(server_addresses, rd);

	const auto server_peer_count = std::min(static_cast<int>(server_addresses.size()), 2);
//	const auto server_peer_count = std::min(static_cast<int>(server_addresses.size()), settings.limit_active_server_peer_count);
	for (int i = 0; i < server_peer_count; ++i)
		connections.emplace_back(io_context, shared_from_this(), server_addresses[i]);

	// <<< In case of disconnect from a server_peer go for the next
	// <<< In case there is no more server_peer left and there are no more server report failure
}

TaskPool::~TaskPool() {

}

// -------------------------------------------------------------------------------------------------

void TaskPool::_assign_next_task_or_enter_idle(Connection connection) {
	const auto download_rate_bps = connection->download_rate_bps();
	const auto download_max = download_rate_bps * settings_download_block_size_in_time;

	if (active_task_queue.empty()) {
		if (waiting_task_queue.empty()) {
			idles.emplace_back(std::move(connection));

			if (idles.size() == connections.size())
				done_bl.raise();
			return;
		}

		auto& task = waiting_task_queue.front();
		connection->assign_task(std::move(task), 0, 0, true);
		waiting_task_queue.pop_front();
		return;
	}

	auto& task = active_task_queue.front();

	if (task.size <= download_max) {
		const auto offset = task.offset;
		const auto size = task.size;
		connection->assign_task(std::move(task.base_task), offset, size, false);
		active_task_queue.pop_front();

	} else {
		const auto offset = task.offset;
		const auto size = download_max;
		task.offset += size;
		task.size -= size;
		connection->assign_task(task.base_task, offset, size, false);
	}
}

std::optional<TaskPool::Connection> TaskPool::_grab_idle() {
	std::optional<TaskPool::Connection> result;

	if (!idles.empty()) {
		const auto last_it = idles.begin() + (idles.size() - 1);
		result.emplace(std::move(*last_it));
		idles.erase(last_it);
	}

	return result;
}

void TaskPool::queue_task_file(std::string identifier, std::filesystem::path filepath) {
	auto task = std::make_shared<ResourceTaskFile>(std::move(identifier), std::move(filepath));

	auto lock = std::unique_lock(mutex);
	if (auto idle = _grab_idle()) {
		idle.value()->assign_task(std::move(task), 0, 0, true);

	} else {
		waiting_task_queue.emplace_back(std::move(task));
	}
}

void TaskPool::add_active_task(std::shared_ptr<ResourceTask> task, std::size_t offset, std::size_t size) {
	auto lock = std::unique_lock(mutex);
	done_bl.reset();

	while (auto idle = _grab_idle()) {
		const auto& connection = idle.value();
		const auto download_rate_bps = connection->download_rate_bps();
		const auto download_max = download_rate_bps * settings_download_block_size_in_time;
		const auto task_size = std::min(size, download_max);
		connection->assign_task(task, offset, task_size, false);

		offset += task_size;
		size -= task_size;

		if (size == 0)
			return;
	}

	active_task_queue.emplace_back(std::move(task), offset, size);
}

void TaskPool::wait() {
	done_bl.wait();
}

void TaskPool::task_finished(Connection connection) {
	const auto lock = std::unique_lock(mutex);

	_assign_next_task_or_enter_idle(std::move(connection));
}

void TaskPool::connect(Connection connection) {
	const auto lock = std::unique_lock(mutex);

	connections.emplace(connection);

	_assign_next_task_or_enter_idle(std::move(connection));

//  if no connection was establiesed in X time
//      -> report failure
}

void TaskPool::disconnect(Connection connection) {
	const auto lock = std::unique_lock(mutex);

	connections.erase(connection);
	libv::erase_unstable(idles, connection);

//	if this was the last connection
//  	and we did not finished
//  	and it is not early stop case
//  	-> report failure
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
