// Project: libv.update, File: src/libv/update/resource_client/resource_client_connection.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/resource_client/resource_client_connection.lpp>
// libv

// std

// pro
#include <libv/update/resource_client/task_pool.lpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

void ResourceClientConnection::on_connect(error_code ec) {
	if (ec)
		return;

	tasks->connect(connection_from_this());
}

void ResourceClientConnection::on_receive(error_code ec, message m) {
	if (ec)
		return;

	codec.decode(*this, m);
}

void ResourceClientConnection::on_send(error_code ec, message m) {
	if (ec)
		return;

//	if (codec.is<msg_res::ResponseResourceData>(m))
//		continue_current_task();
}

void ResourceClientConnection::on_disconnect(error_code ec) {
	tasks->disconnect(connection_from_this());
}

// -------------------------------------------------------------------------------------------------

size_t ResourceClientConnection::download_rate_bps() {
	auto lock = std::unique_lock(mutex);

	return transfer_rate_avg_bps.value();
}

void ResourceClientConnection::assign_task(std::shared_ptr<ActiveResourceTask> task, size_t offset, size_t amount) {
	auto lock = std::unique_lock(mutex);

	active_task = std::move(task);
	send(msg_res::RequestResource(active_task->name, offset, amount));
}

// -------------------------------------------------------------------------------------------------

void ResourceClientConnection::receive(const msg_res::ResponseBusy& response) {
	auto lock = std::unique_lock(mutex);

//	tasks->busy(connection_from_this(), response.busy_time);
}

void ResourceClientConnection::receive(const msg_res::ResponseResourceInvalid& response) {
	auto lock = std::unique_lock(mutex);

//	tasks->invalid(connection_from_this());
}

void ResourceClientConnection::receive(const msg_res::ResponseResourceNotFound& response) {
	auto lock = std::unique_lock(mutex);

//	tasks->not_found(connection_from_this());
}

void ResourceClientConnection::receive(const msg_res::ResponseResourceDescription& response) {
	auto lock = std::unique_lock(mutex);

	active_task->create(response.size);

	// Reset last_data_transfer to exclude any communication overhead from transfer rate measurements
	last_data_transfer = std::chrono::system_clock::now();
}

void ResourceClientConnection::receive(const msg_res::ResponseResourceData& response) {
	auto lock = std::unique_lock(mutex);

	const auto now = std::chrono::system_clock::now();
	const auto delta_sec = std::chrono::duration_cast<double, std::chrono::seconds>(now - last_data_transfer).count();
	transfer_rate_avg_bps.add(response.data.size(), delta_sec);
	last_data_transfer = now;

	active_task->write(response.offset, response.data);
}

void ResourceClientConnection::receive(const msg_res::ResponseResourceDone& response) {
	std::shared_ptr<ActiveResourceTask> temp_task;

	{
		auto lock = std::unique_lock(mutex);
		temp_task = std::move(active_task);
	}

	tasks->done(connection_from_this(), std::move(temp_task));
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
