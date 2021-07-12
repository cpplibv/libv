// Project: libv, File: app/space/network_server.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <space/network_server.hpp>
// libv
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor_he.hpp>
// std
#include <mutex>
#include <string>
// pro
#include <space/log.hpp>
#include <space/playout.hpp>



#include <set>
#include <queue>



namespace app {

// -------------------------------------------------------------------------------------------------
//
//class chat_participant {
//public:
//	virtual void deliver(const std::string& msg) = 0;
//	virtual ~chat_participant() = default;
//};
//

[[nodiscard]] inline std::string_view as_sv(const std::span<const std::byte> s) noexcept {
	return std::string_view(reinterpret_cast<const char*>(s.data()), s.size());
}

// -------------------------------------------------------------------------------------------------

class NetworkPeer;

class NetworkLobby {
//	static constexpr int max_recent_msgs = 100;

private:
	std::mutex mutex;
	std::set<libv::net::mtcp::Connection<NetworkPeer>> participants;
//	std::deque<std::string> recent_msgs;

public:
	void join(libv::net::mtcp::Connection<NetworkPeer> participant) {
		std::unique_lock lock(mutex);
		participants.insert(participant);
//		for (const auto& msg : recent_msgs)
//			participant->deliver(msg);
	}

	void leave(libv::net::mtcp::Connection<NetworkPeer> participant) {
		std::unique_lock lock(mutex);
		participants.erase(participant);
	}

	void broadcast(const std::string& msg) {
		std::unique_lock lock(mutex);
//		recent_msgs.push_back(msg);
//		while (recent_msgs.size() > max_recent_msgs)
//			recent_msgs.pop_front();

//		for (const auto& participant : participants)
//			participant->deliver(msg);
	}

	void disconnect_all() {
		for (const auto& participant : participants)
			participant.connection().cancel_and_disconnect_async();
	}
};

//struct NetworkPeer : public chat_participant, public net::ConnectionHandler<chat_session> {
class NetworkPeer : public libv::net::mtcp::ConnectionHandler<NetworkPeer> {
private:
	std::shared_ptr<NetworkLobby> lobby;
	static inline int nextID = 0; int ID = nextID++;

public:
	explicit NetworkPeer(std::shared_ptr<NetworkLobby> lobby) :
		lobby(std::move(lobby)) {
	}

	~NetworkPeer() {
	}

private:
//	virtual void deliver(const std::string& msg) override {
	void deliver(const std::string& msg) {
		connection.send_async(msg);
	}

private:
	virtual void on_connect(error_code ec) override {
		if (!ec)
			lobby->join(connection_from_this());
	}
	virtual void on_receive(error_code ec, message m) override {
		if (!ec)
			lobby->broadcast(std::string(as_sv(m)));
	}
	virtual void on_send(error_code ec, message m) override {
		(void) ec;
		(void) m;
	}
	virtual void on_disconnect(error_code ec) override {
		(void) ec;
		lobby->leave(connection_from_this());
	}
};

class AcceptorHandler : public libv::net::mtcp::AcceptorHandler<AcceptorHandler> {
private:
	std::shared_ptr<NetworkLobby> lobby = std::make_shared<NetworkLobby>();

public:
	~AcceptorHandler() {
		lobby->disconnect_all();
	}

private:
	virtual libv::net::mtcp::Connection<> on_accept(io_context& io_context) override {
		return libv::net::mtcp::Connection<NetworkPeer>(io_context, lobby);
	}
	virtual void on_accept(error_code ec) override {
		log_space.error("Failed to accept incoming connection: {}:{}", ec, ec.message());
	}
};
using Acceptor = libv::net::mtcp::Acceptor<AcceptorHandler>;

// -------------------------------------------------------------------------------------------------

struct ImplNetworkServer {
	libv::net::IOContext io_context{4};
	Acceptor acceptor{io_context};
};

// -------------------------------------------------------------------------------------------------

NetworkServer::NetworkServer(uint16_t server_port, Playout& playout) :
	self(std::make_unique<ImplNetworkServer>()) {

	if (auto ec = self->acceptor.acceptor().listen(server_port, 4))
		throw std::system_error(ec);

	self->acceptor.acceptor().accept_async();
}

NetworkServer::~NetworkServer() {
}

// -------------------------------------------------------------------------------------------------

} // namespace app
