// Project: libv.net, File: src/libv/net/connection_mtcp.cpp, Author: Császár Mátyás [Vader]

//// ext
//#include <netts/buffer.hpp>
//#include <netts/executor.hpp>
//#include <netts/internet.hpp>
//#include <netts/socket.hpp>
//// libv
//#include <libv/thread/binary_latch.hpp>
//#include <libv/utility/byte_swap.hpp>
//// std
//#include <memory>
//#include <queue>
//// pro
//#include <libv/net/io_context.hpp>
//#include <libv/net/log.hpp>


namespace libv {
namespace net {

// -------------------------------------------------------------------------------------------------

//using PacketHeader = uint32_t;
//
////constexpr uint32_t MTCP_PACKET_MAX_INITIAL_RESERVE = 1 * 1024 * 1024; // 1 MB
////constexpr uint32_t MTCP_PACKET_MAX_SIZE = 100 * 1024 * 1024; // 100 MB
//
//// -------------------------------------------------------------------------------------------------
//
//class ConnectionMTCP : public std::enable_shared_from_this<ConnectionMTCP> {
//private:
//	IOContext& io_context;
//	netts::ip::tcp::socket socket;
//	std::mutex mutex;
//	// std::mutex sync_queue_mutex; // just an idea
//
//	PacketHeader read_header; // Temporary buffer for subsequent read headers in network byte order
//	Message read_body; // Temporary buffer for subsequent read body
////	std::queue<HandlerRead> read_queue;
//
//	PacketHeader write_header; // Temporary buffer for subsequent write headers in network byte order
//	std::vector<Message> write_packets; // Write queue
////	std::queue<std::pair<Message, HandlerWrite>> write_queue; // Write queue
//
//public:
//	ConnectionMTCP(IOContext& io_context) noexcept;
//	ConnectionMTCP(IOContext& io_context, netts::ip::tcp::socket&& socket) noexcept;
//
//public:
//	template <typename HandlerConnectT>
//	void connect_async(const Address& address, HandlerConnectT&& handler) noexcept;
//	[[nodiscard]] OutcomeConnect connect_sync(const Address& address) noexcept;
//
//	template <typename HandlerReadT>
//	void read_async(HandlerReadT&& handler) noexcept;
//	template <typename HandlerReadRepeatT>
//	void read_async_repeat(HandlerReadRepeatT&& handler) noexcept;
//	[[nodiscard]] OutcomeRead read_sync() noexcept;
//
//	template <typename HandlerWriteT>
//	void write_async(Message packet, HandlerWriteT&& handler) noexcept;
//	[[nodiscard]] OutcomeWrite write_sync(Message packet) noexcept;
//
//	template <typename CloseHandlerT>
//	void close_async(CloseHandlerT&& handler) noexcept;
//	[[nodiscard]] OutcomeClose close_sync() noexcept;
//
//	[[nodiscard]] OutcomeEndpoint endpoint_local() noexcept;
//	[[nodiscard]] OutcomeEndpoint endpoint_remote() noexcept;
//
//private:
//	template <typename HandlerReadT>
//	void execute_read(HandlerReadT&& handler) noexcept;
//	template <typename HandlerReadT>
//	void execute_read_header(std::shared_ptr<ConnectionMTCP>&& self, HandlerReadT&& handler) noexcept;
//	template <typename HandlerReadT>
//	void execute_read_body(std::shared_ptr<ConnectionMTCP>&& self, HandlerReadT&& handler) noexcept;
//
//	template <typename HandlerWriteT>
//	void execute_write(HandlerWriteT&& handler) noexcept;
//	template <typename HandlerWriteT>
//	void execute_write_header(std::shared_ptr<ConnectionMTCP>&& self, HandlerWriteT&& handler) noexcept;
//	template <typename HandlerWriteT>
//	void execute_write_body(std::shared_ptr<ConnectionMTCP>&& self, HandlerWriteT&& handler) noexcept;
//};
//
//// -------------------------------------------------------------------------------------------------
//
//ConnectionMTCP::ConnectionMTCP(IOContext& io_context) noexcept :
//	io_context(io_context),
//	socket(io_context.context()) { }
//
//ConnectionMTCP::ConnectionMTCP(IOContext& io_context, netts::ip::tcp::socket&& socket) noexcept :
//	io_context(io_context),
//	socket(std::move(socket)) { }
//
//template <typename HandlerConnectT>
//void ConnectionMTCP::connect_async(const Address& address, HandlerConnectT&& handler) noexcept {
//	std::unique_lock guard(mutex);
//	io_context.resolve_async(address,
//			[self = shared_from_this(), handler = std::forward<HandlerConnectT>(handler)]
//			(const auto& ec, const auto& endpoints) mutable {
//		if (ec) {
//			handler(outcome::failure(ec));
//		} else {
//			auto& soc = self->socket;
//			netts::async_connect(soc, endpoints,
//					[self = std::move(self), handler = std::forward<HandlerConnectT>(handler)]
//					(const auto& ec, const auto& endpoint) {
//				if (ec) {
//					handler(outcome::failure(ec));
//				} else {
//					handler(outcome::success(endpoint));
//				}
//			});
//		}
//	});
//}
//
//OutcomeConnect ConnectionMTCP::connect_sync(const Address& address) noexcept {
//	std::unique_lock guard(mutex);
//
//
//}
//
//template <typename HandlerReadT>
//void ConnectionMTCP::read_async(HandlerReadT&& handler) noexcept {
//	execute_read(std::forward<HandlerReadT>(handler));
//}
//
//template <typename HandlerReadRepeatT>
//void ConnectionMTCP::read_async_repeat(HandlerReadRepeatT&& handler) noexcept {
//	execute_read(std::forward<HandlerReadRepeatT>(handler));
//	//		read_header_repeat(handler);
//}
//
//OutcomeRead ConnectionMTCP::read_sync() noexcept {
//	BinaryLatch latch;
//	OutcomeRead result = outcome::failure(netts::error::eof);
//	execute_read([&latch, &result](outcome::outcome<Message> message) {
//		result = std::move(message);
//		latch.raise();
//	});
//	latch.wait();
//	return result;
//}
//
//template <typename HandlerWriteT>
//void ConnectionMTCP::write_async(Message packet, HandlerWriteT&& handler) noexcept {
//	//		std::unique_lock<std::mutex> lock(mutex);
//	//		bool write_in_progress = !write_packets.empty();
//	//		write_packets.emplace_back(std::move(packet));
//	//		if (!write_in_progress) {
//	//			write_header(self);
//	//		}
//}
//
//OutcomeWrite ConnectionMTCP::write_sync(Message packet) noexcept {
//	BinaryLatch latch;
//	OutcomeWrite result;
//
//
//	//	std::unique_lock<std::mutex> lock(mutex);
//	//	bool write_in_progress = !write_packets.empty();
//	//	write_packets.emplace_back(std::move(packet));
//	//	if (!write_in_progress) {
//	//		write_header(self);
//	//	}
//	//
//	//
//	//	execute_write([&latch, &result](std::error_code ec) {
//	//		result = std::move(ec);
//	//		latch.raise();
//	//	});
//
//
//	latch.wait();
//	return result;
//}
//
//template <typename CloseHandlerT>
//void ConnectionMTCP::close_async(CloseHandlerT&& handler) noexcept {
//	//	    boost::asio::post(io_context_, [this]() { socket_.close(); });
//
//	//		std::error_code ec;
//	//		socket.shutdown(netts::ip::tcp::socket::shutdown_both, ec);
//	//		socket.shutdown(socket.shutdown_both, ec);
//	//		socket.close(ec);
//}
//
//OutcomeClose ConnectionMTCP::close_sync() noexcept {
//	//	    boost::asio::post(io_context_, [this]() { socket_.close(); });
//	//		return netts::error::fault;
//	//		wait
//	return std::error_code{};
//}
//
//OutcomeEndpoint ConnectionMTCP::endpoint_local() noexcept {
//		std::error_code ec;
//		auto endpoint = socket.local_endpoint(ec);
//		if (ec)
//			return outcome::failure(ec);
//		else
//			return outcome::success(std::move(endpoint));
//
//		// IDEA: as_outcome wrapper
//		//		return as_outcome(socket, &decltype(socket)::remote_endpoint);
//}
//
//OutcomeEndpoint ConnectionMTCP::endpoint_remote() noexcept {
//		std::error_code ec;
//		auto endpoint = socket.remote_endpoint(ec);
//		if (ec)
//			return outcome::failure(ec);
//		else
//			return outcome::success(std::move(endpoint));
//}
//
////private:
////	void close() {
////		std::error_code ec;
////		socket.close(ec);
////		if (ec)
////			log_net.trace("Error while closing socket. Reason: {}", ec.message());
////	}
//
//// TODO P5: Implement async safe 2 section read from netts in connection
////	void execute_read(std::shared_ptr<ConnectionMTCP>&& self, HandlerRead&& handler) {
////		netts::async_read(socket, netts::dynamic_buffer(read_body),
////				[this, self = std::move(self), handler = std::forward<>(handler)]
////				(const std::error_code& ec, size_t bytes_total) mutable {
////					if (ec) {
////						handler(outcome::failure(ec));
////						return;
////					}
////
////					if (bytes_total == 0) {
////						return sizeof PacketHeader;
////					} else if (bytes_total == sizeof PacketHeader) {
////						read_header = libv::network_to_host(reinterpret_cast<PacketHeader&>(read_body.data()));
////						return read_header;
////					} else if (total_read > sizeof PacketHeader) {
////
////					}
////
////					const auto read_body_size = libv::network_to_host(read_header);
////
////					if (total_read == sizeof read_header)
////						return sizeof read_header;
////
////					execute_read_body(std::move(self), std::forward<>(handler));
////				});
////	}
//
//template <typename HandlerReadT>
//void ConnectionMTCP::execute_read(HandlerReadT&& handler) noexcept {
//	execute_read_header(shared_from_this(), std::forward<HandlerReadT>(handler));
//}
//
//template <typename HandlerReadT>
//void ConnectionMTCP::execute_read_header(std::shared_ptr<ConnectionMTCP>&& self, HandlerReadT&& handler) noexcept {
//	netts::async_read(socket, netts::buffer(&read_header, sizeof read_header),
//			[this, self = std::move(self), handler = std::forward<HandlerReadT>(handler)]
//			(const std::error_code& ec, size_t) mutable {
//		if (ec)
//				handler(outcome::failure(ec));
//		else
//			execute_read_body(std::move(self), std::forward<HandlerReadT>(handler));
//		});
//}
//
//template <typename HandlerReadT>
//void ConnectionMTCP::execute_read_body(std::shared_ptr<ConnectionMTCP>&& self, HandlerReadT&& handler) noexcept {
//	const auto read_body_size = libv::network_to_host(read_header);
//	read_body.reserve(read_body_size);
//
//	netts::async_read(socket, netts::dynamic_buffer(read_body, read_body_size),
//			[this, self = std::move(self), handler = std::forward<HandlerReadT>(handler)]
//			(const std::error_code& ec, size_t) mutable {
//		if (ec)
//				handler(outcome::failure(ec));
//		else
//			handler(outcome::success(read_body));
//		});
//}
//
//// =================================================================================================
//
//template <typename HandlerWriteT>
//void ConnectionMTCP::execute_write(HandlerWriteT&& handler) noexcept {
//	execute_write_header(shared_from_this(), std::forward<HandlerWriteT>(handler));
//}
//
//template <typename HandlerWriteT>
//void ConnectionMTCP::execute_write_header(std::shared_ptr<ConnectionMTCP>&& self, HandlerWriteT&& handler) noexcept {
//	netts::async_write(socket, netts::buffer(write_packets[0]),
//			[this, self = std::move(self), handler = std::forward<HandlerWriteT>(handler)]
//			(const std::error_code& ec, size_t) {
//		if (ec)
//				handler(outcome::failure(ec));
//		else
//			handler(outcome::success(read_body));
//		});
//}
//
//template <typename HandlerWriteT>
//void ConnectionMTCP::execute_write_body(std::shared_ptr<ConnectionMTCP>&& self, HandlerWriteT&& handler) noexcept {
//}
//
//// =================================================================================================
////
////	void execute_write_header(const std::shared_ptr<ConnectionMTCP>& self) {
////		netts::async_write(socket_, netts::buffer(write_packets.front().buffer_header()),
////				[this, self](const auto& ec, auto) {
////					if (ec) {
////						std::error_code re_ec;
////						const auto& re = socket_.remote_endpoint(re_ec);
////						if (!re_ec)
////							log_net.trace("Failed to write packet header from {}:{}. Reason: {}", re.address(), re.port(), ec.message());
////						else
////							log_net.trace("Failed to write packet header from unknown. Reason: {}", ec.message());
////						//						park.leave(self);
////						return;
////					}
////
////					write_body(self);
////				});
////	}
////
////	void execute_write_body(const std::shared_ptr<ConnectionInternal>& self) {
////		netts::async_write(socket_, netts::buffer(write_packets.front().buffer_body()),
////				[this, self](const auto& ec, auto) {
////					if (ec) {
////						std::error_code re_ec;
////						const auto& re = socket_.remote_endpoint(re_ec);
////						if (!re_ec)
////							log_net.trace("Failed to write packet body from {}:{}. Reason: {}", re.address(), re.port(), ec.message());
////						else
////							log_net.trace("Failed to write packet body from unknown. Reason: {}", ec.message());
////
////						//						park.leave(self);
////						return;
////					}
////
////					write_packets.erase(write_packets.begin());
////					if (!write_packets.empty()) {
////						write_header(self);
////					}
////				});
////	}
//
//[[nodiscard]] std::shared_ptr<PolymorphicConnection> create_ConnectionMTCP(IOContext& io_context) noexcept {
//	return std::make_shared<FacadePolymorphicConnection<ConnectionMTCP>>(io_context);
//}
//
//[[nodiscard]] std::shared_ptr<PolymorphicConnection> create_ConnectionMTCP(IOContext& io_context, netts::ip::tcp::socket&& socket) noexcept {
//	return std::make_shared<FacadePolymorphicConnection<ConnectionMTCP>>(io_context, std::move(socket));
//}

// -------------------------------------------------------------------------------------------------

} // namespace net
} // namespace libv
