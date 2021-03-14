// Project: libv.update, File: src/libv/update/resource_client/task_pool.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource_client/task_pool.lpp>
// libv

// pro
#include <libv/update/resource_client/resource_client_connection.lpp>

//// std
//#include <deque>
//#include <fstream>
//// pro
////#include <libv/update/log.hpp>
//#include <libv/update/resource_client/resource_file.lpp>
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











void TaskPool::foo() {
	Connection connection;

	const auto download_speed_bps = connection->download_rate_bps();
	const auto subtask_max_size = download_speed_bps * 5;

	std::shared_ptr<ResourceTask> base_task;
	std::shared_ptr<ActiveResourceTask> active_task(base_task);
	active_task->file.create(base_task->size);

	const auto next_interval = active_task->work.next_marked();
	const auto next_work_offset = next_interval.offset;
	const auto next_work_size = std::min(next_interval.size, subtask_max_size);

	std::shared_ptr<ActiveResourceSubTask> active_sub_task(active_task);

	active_task->work.unmark(next_work_offset, next_work_size);
	active_sub_task->work.mark(next_work_offset, next_work_size);

	connection->assign_task(active_sub_task);
}

void TaskPool::hand_back_task(std::shared_ptr<ActiveResourceSubTask> task) {
	auto lock = std::unique_lock(mutex);

	auto& parent_work = task->parent_task->work;
	const auto& subtask_work = task->work;

	parent_work.mark(subtask_work);
}

void TaskPool::queue_task(std::shared_ptr<ResourceTask> task) {
	auto lock = std::unique_lock(mutex);
	task_queue.emplace_back(std::move(task));
	done_bl.reset();

	if (idles.empty())
		return;

	const auto last_it = idles.begin() + (idles.size() - 1);
	const auto last = std::move(*last_it);
	idles.erase(last_it);

	lock.unlock();

	last->assign_task();
}

void TaskPool::wait() {
	done_bl.wait();
}

void TaskPool::done(Connection connection, std::shared_ptr<ActiveResourceTask> task) {
//	const auto lock = std::unique_lock(mutex);
//
//	std::optional<ResourceTask> result;
//
//	if (queue.empty()) {
//		idles.emplace(std::move(connection));
//		if (idles.size() == connections.size())
//			done.raise();
//
//	} else {
//		result.emplace(std::move(queue.front()));
//		queue.pop_front();
//	}
//
//	return result;
}

void TaskPool::connect(Connection connection) {
	const auto lock = std::unique_lock(mutex);
	connections.emplace(connection);
	// mark connection as idle
}

void TaskPool::disconnect(Connection connection) {
	const auto lock = std::unique_lock(mutex);
	connections.erase(connection);
	// hand back grabbed but unfinished task
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
