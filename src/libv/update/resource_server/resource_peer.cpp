// Project: libv.update, File: src/libv/update/resource_server/resource_peer.cpp

// hpp
#include <libv/update/resource_server/resource_peer.hxx>
// pro
#include <libv/update/resource_server/resource_file.hxx>
#include <libv/update/resource_server/resource_server_state.hxx>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

void ResourcePeer::on_connect(error_code ec) {
	if (ec)
		return;

	server->join(connection_from_this());
}

void ResourcePeer::on_receive(error_code ec, message_view m) {
	if (ec)
		return;

	codec.decode(*this, m.as_str());
}

void ResourcePeer::on_send(error_code ec, message_view m) {
	if (ec)
		return;

	if (codec.is<msg_res::ResponseResourceData>(m.as_bin()))
		continue_current_task();
}

void ResourcePeer::on_disconnect(error_code ec) {
	server->leave(connection_from_this());
}

// ---

ResourcePeer::ResourcePeer(std::shared_ptr<ServerState> server) :
	server(std::move(server)) {}

//void ResourcePeer::on_send(message msg) {
//	if (codec.is<msg_res::ResponseResourceData>(msg))
//		continue_current_task();
//}

void ResourcePeer::cancel_current_task() {
	if (current_task)
		current_task->cancelled = true;
}

void ResourcePeer::continue_current_task() {
	auto& task = *current_task;

	if (task.cancelled) {
		send(msg_res::ResponseResourceEnd{true});
		if (next_task) {
			current_task = std::move(next_task);
			next_task.reset();
		} else {
			current_task.reset();
		}
		return;
	}

	const auto amount_left = task.requested_offset + task.requested_amount - task.next_offset;
	const auto amount_allowed = server->settings().resource_network_chunk_size;
	const auto amount = std::min(amount_left, amount_allowed);
	const auto offset = task.next_offset;
	task.next_offset += amount;

	if (amount != 0)
		// There something to send
		send(msg_res::ResponseResourceData{offset, task.resource->copy_chunk(offset, amount)});

	if (amount_left == amount) {
		// Task is finished
		send(msg_res::ResponseResourceEnd{false});

		if (next_task) {
			current_task = std::move(next_task);
			next_task.reset();
		} else {
			current_task.reset();
		}
	}
}

void ResourcePeer::receive(const msg_res::RequestResource& request) {
	cancel_current_task();

	const auto lookup_result = server->get_resource(request.name, request.offset, request.amount);

	if (lookup_result.invalid_request) {
		send(msg_res::ResponseResourceInvalid{});
	} else if (lookup_result.not_found) {
		send(msg_res::ResponseResourceNotFound{});
	} else {
		// Start new task
		const auto resource_size = lookup_result.resource->size();
		send(msg_res::ResponseResourceDescription{request.name, resource_size});
//		send(msg_res::ResponseResourceDescription{request.name, resource_size, resource_signature});

		if (request.amount == 0)
			// The request only a header request
			send(msg_res::ResponseResourceEnd{false});
		else {
			auto& task = !current_task ? current_task : next_task;
			task.emplace(
					request.offset,
					request.amount,
					request.offset,
					std::move(lookup_result.resource)
			);
			continue_current_task();
		}
	}
}

void ResourcePeer::receive(const msg_res::RequestCancel& request) {
	(void) request;
	cancel_current_task();
}

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
