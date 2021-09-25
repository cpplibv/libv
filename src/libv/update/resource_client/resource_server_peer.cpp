// Project: libv.update, File: src/libv/update/resource_client/resource_server_peer.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource_client/resource_server_peer.lpp>
// libv

// std

// pro
#include <libv/update/resource_client/task_pool.lpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

void ResourceServerPeer::on_connect(error_code ec) {
	if (ec)
		return;

	task_pool->connect(connection_from_this());
}

void ResourceServerPeer::on_receive(error_code ec, message_view m) {
	if (ec)
		return;

	codec.decode(*this, m.as_str());
}

void ResourceServerPeer::on_send(error_code ec, message_view m) {
	if (ec)
		return;

//	if (codec.is<msg_res::ResponseResourceData>(m))
//		continue_current_task();
}

void ResourceServerPeer::on_disconnect(error_code ec) {
	// Hand back unfinished work
	for (const auto& interval : assigned_work_.marked_intervals())
		task_pool->add_active_task(assigned_task_, interval.offset, interval.size);

	task_pool->disconnect(connection_from_this());
}

// -------------------------------------------------------------------------------------------------

size_t ResourceServerPeer::download_rate_bps() {
	auto lock = std::unique_lock(mutex);

	return transfer_rate_avg_bps.value();
}

void ResourceServerPeer::assign_task(std::shared_ptr<ResourceTask> task, uint64_t offset, uint64_t amount, bool prepare) {
	auto lock = std::unique_lock(mutex);

	assigned_task_ = std::move(task);
	assigned_prepare = prepare;
	assigned_work_.clear();
	assigned_work_.mark(offset, amount);
	send(msg_res::RequestResource(assigned_task_->identifier(), offset, amount));
}

// -------------------------------------------------------------------------------------------------

void ResourceServerPeer::receive(const msg_res::ResponseBusy& response) {
	auto lock = std::unique_lock(mutex);

//	task_pool->busy(connection_from_this(), response.busy_time);
}

void ResourceServerPeer::receive(const msg_res::ResponseResourceInvalid& response) {
	auto lock = std::unique_lock(mutex);

//	task_pool->invalid(connection_from_this());
}

void ResourceServerPeer::receive(const msg_res::ResponseResourceNotFound& response) {
	auto lock = std::unique_lock(mutex);

//	task_pool->not_found(connection_from_this());
}

void ResourceServerPeer::receive(const msg_res::ResponseResourceDescription& response) {
	auto lock = std::unique_lock(mutex);

	if (assigned_prepare) {
		// The task was only a prepare task
		const auto intervals = assigned_task_->start(response.size, response.signature);

		for (const auto& interval : intervals)
			task_pool->add_active_task(assigned_task_, interval.offset, interval.size);

		task_pool->task_finished(connection_from_this());
	} else {
		// Reset last_data_transfer to exclude any communication overhead from transfer rate measurements
		last_data_transfer.reset();
	}
}

void ResourceServerPeer::receive(const msg_res::ResponseResourceData& response) {
	auto lock = std::unique_lock(mutex);

	const auto delta_sec = last_data_transfer.timed_s().count();
	const auto delta_bytes = static_cast<double>(response.data.size());
	transfer_rate_avg_bps.add(delta_bytes, delta_sec);

	assigned_work_.unmark(response.offset, response.data.size());
	assigned_task_->write(response.offset, response.data);
}

void ResourceServerPeer::receive(const msg_res::ResponseResourceEnd& response) {
	// Hand back unfinished work
	for (const auto& interval : assigned_work_.marked_intervals())
		task_pool->add_active_task(assigned_task_, interval.offset, interval.size);

	assigned_task_.reset();
	task_pool->task_finished(connection_from_this());
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
