// Project: libv.update, File: src/libv/update/resource_server/resource_peer.lpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec.hpp>
// std
#include <memory>
#include <optional>
// pro
#include <libv/update/common/protocol_res.hpp>
//#include <libv/update/resource_server/resource_server_state.hxx>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class ResourceFile;
class ServerState;

// -------------------------------------------------------------------------------------------------

//class ResourcePeer : SimpleConnection<ResourcePeer, libv::archive::Binary, msg_res> {
//class ResourcePeer {
class ResourcePeer : public libv::net::mtcp::ConnectionHandler<ResourcePeer> {
	static inline libv::serial::CodecServer<ResourcePeer, libv::archive::Binary> codec{msg_res{}};

	template <typename T>
	void send(const T& response) {
		connection.send_async(codec.encode(response));
	}

	virtual void on_connect(error_code ec) override;
	virtual void on_receive(error_code ec, message_view m) override;
	virtual void on_send(error_code ec, message_view m) override;
	virtual void on_disconnect(error_code ec) override;

	// ---

//	std::mutex mutex;

	struct Task {
		uint64_t requested_offset = 0;
		uint64_t requested_amount = 0;
		uint64_t next_offset = 0;
		std::shared_ptr<ResourceFile> resource;
		bool cancelled = false;
	};

	std::optional<Task> current_task;
	std::optional<Task> next_task;

//	libv::mt::async_value<ResourceFileData> selected_resource;
	std::shared_ptr<ServerState> server;

//	std::chrono::steady_clock::time_point last_action;

public:
	explicit ResourcePeer(std::shared_ptr<ServerState> server);

public:
	void cancel_current_task();
	void continue_current_task();
	void receive(const msg_res::RequestResource& request);
	void receive(const msg_res::RequestCancel& request);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
