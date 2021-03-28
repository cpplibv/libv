// Project: libv.update, File: src/libv/update/resource_client/task_pool.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource_client/task_pool.lpp>
// libv
#include <libv/algo/erase_unstable.hpp>
// std
//#include <algorithm>
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

std::shared_ptr<ActiveResourceSubTask> TaskPool::_assign_subtask_or_idle(Connection connection) {
	const auto download_rate_bps = connection->download_rate_bps();
//	const auto subtask_max_size = download_speed_bps * 5;
//
//	std::shared_ptr<ResourceTask> base_task;
//	std::shared_ptr<ActiveResourceTask> active_task(base_task);
//	active_task->file.create(base_task->size);
//
//	const auto next_interval = active_task->work.next_marked();
//	const auto next_work_offset = next_interval.offset;
//	const auto next_work_size = std::min(next_interval.size, subtask_max_size);
//
//	std::shared_ptr<ActiveResourceSubTask> active_sub_task(active_task);
//
//	active_task->work.unmark(next_work_offset, next_work_size);
//	active_sub_task->work.mark(next_work_offset, next_work_size);

	if (subtask)
		connection->assign_task(std::move(subtask));
	else
		idles.emplace_back(std::move(connection));
}

void TaskPool::queue_task(std::shared_ptr<ResourceTask> task) {
	const auto lock = std::unique_lock(mutex);

//	task_queue.emplace_back(std::move(task));
//	done_bl.reset();
//
//	if (idles.empty())
//		return;
//
//	const auto last_it = idles.begin() + (idles.size() - 1);
//	const auto last = std::move(*last_it);
//	idles.erase(last_it);
//
//	lock.unlock();
//
//	last->assign_task();

	while (true) {
		if (!idles.empty())
			queue_this_task();
			return;

		if (no_more_subtask_from_this_one_to_hand_out())
			return;

		hand_out_subtask_to_one_idle();
		erase_that_idle_from_idles();
	}

	// pass a subtasks to every idle task
}

void TaskPool::wait() {
	done_bl.wait();
}

void TaskPool::done(Connection connection, std::shared_ptr<ActiveResourceTask> task) {
	const auto lock = std::unique_lock(mutex);

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

	if (task->remaining_work.empty())
		libv::erase_unstable(task_actives, task);

	_assign_subtask_or_idle(std::move(connection));
	if (task_actives.empty() && task_queue.empty());
		done_bl.raise();
}

void TaskPool::connect(Connection connection) {
	const auto lock = std::unique_lock(mutex);

	connections.emplace(connection);
	_assign_subtask_or_idle(std::move(connection));
}

void TaskPool::disconnect(Connection connection) {
	const auto lock = std::unique_lock(mutex);

	maintain_task_actives_question_mark();

	if (const auto assigned_task = connection->assigned_task())
		// Hand back the remaining sub task's work to the main task
		assigned_task->parent_task->remaining_work.mark(assigned_task->work);

	connections.erase(connection);
	libv::erase_unstable(idles, connection);
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
