// Project: libv.update, File: src/libv/update/resource_client/resource_server_peer.lpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
#include <libv/math/exp_moving_avg.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/codec.hpp>
#include <libv/utility/interval_registry.hpp>
#include <libv/utility/storage_size.hpp>
#include <libv/utility/timer.hpp>
// std
//#include <deque>
//#include <fstream>
#include <chrono>
#include <mutex>
// pro
#include <libv/update/log.hpp>
#include <libv/update/common/protocol_res.hpp>
//#include <libv/update/resource_client/resource_file.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

class TaskPool;
class ResourceTask;

// -------------------------------------------------------------------------------------------------

class ResourceServerPeer : public libv::net::mtcp::ConnectionHandler<ResourceServerPeer> {
	static inline libv::serial::CodecClient<ResourceServerPeer, libv::archive::Binary> codec{msg_res{}};

	template <typename T>
	void send(const T& response) {
		connection.send_async(codec.encode(response));
	}

	virtual void on_connect(error_code ec) override;
	virtual void on_receive(error_code ec, message_view m) override;
	virtual void on_send(error_code ec, message_view m) override;
	virtual void on_disconnect(error_code ec) override;

	// ---

private:
	std::mutex mutex;

	/// Last time the connection received a data message
//	std::chrono::system_clock::time_point last_data_transfer;
	libv::Timer last_data_transfer;
	/// Transfer rate of data messages in the last 10 second in byte per second
	libv::exp_moving_avg<double, 10> transfer_rate_avg_bps{libv::KB(256)};
//	libv::exponential_moving_average<double, 10, 0.8> transfer_rate_avg_bps{libv::KB(256)}; // TODO P4: GCC 11 NTTP adjust match to 0.8

	std::shared_ptr<TaskPool> task_pool;

	std::shared_ptr<ResourceTask> assigned_task_;
	bool assigned_prepare;
	libv::IntervalRegistry assigned_work_;

public:
	explicit ResourceServerPeer(std::shared_ptr<TaskPool> task_pool, libv::net::Address address) :
		task_pool(std::move(task_pool)) {
		connection.connect_async(std::move(address));
	}

public:
	std::size_t download_rate_bps();
	void assign_task(std::shared_ptr<ResourceTask> task, uint64_t offset, uint64_t amount, bool prepare);
//	void cancel_task();

public:
	void receive(const msg_res::ResponseBusy& response);
	void receive(const msg_res::ResponseResourceInvalid& response);
	void receive(const msg_res::ResponseResourceNotFound& response);
	void receive(const msg_res::ResponseResourceDescription& response);
	void receive(const msg_res::ResponseResourceData& response);
	void receive(const msg_res::ResponseResourceEnd& response);
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
