// Project: libv, File: app/space/network/network_server.cpp

#pragma once

// hpp
#include <space/network/network_server.hpp>
// libv
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor_he.hpp>
#include <libv/net/mtcp/connection_he.hpp>
// std
#include <mutex>
#include <set>
#include <string>
// pro
#include <space/network/lobby.hpp>
#include <space/universe/ids.hpp>


namespace app {

// -------------------------------------------------------------------------------------------------

class NetworkPeer;

class NetworkLobby {
//	static constexpr int max_recent_msgs = 100;

private:
	std::mutex mutex;
	std::set<libv::net::mtcp::Connection<NetworkPeer>> connectings;
	std::set<libv::net::mtcp::Connection<NetworkPeer>> participants;

public:
	GameThread& game_thread;
	Playout& playout;
	Universe& universe;
	Lobby lobby{game_thread};

public:
	explicit NetworkLobby(GameThread& game_thread, Playout& playout, Universe& universe, User& hosting_user);

private:
	void _broadcast(std::vector<std::byte>&& msg);

public:
	void connecting(libv::net::mtcp::Connection<NetworkPeer> participant);
	void join(libv::net::mtcp::Connection<NetworkPeer> participant, std::string name, uint64_t version);
	void leave(libv::net::mtcp::Connection<NetworkPeer> participant);

	template <typename Message>
			requires requires { typename Message::lobby_command; }
	void process(NetworkPeer& peer, libv::net::mtcp::message_body_view m, Message&& message);

	void broadcast(std::vector<std::byte>&& msg);
	void disconnect_all();
};

// -------------------------------------------------------------------------------------------------

//struct NetworkPeer : public chat_participant, public net::ConnectionHandler<chat_session> {
class NetworkPeer : public libv::net::mtcp::ConnectionHandler<NetworkPeer> {
private:
	std::shared_ptr<NetworkLobby> lobby;

	static inline int nextID = 1; // 0 is reserved for the host
	int ID = nextID++;

	bool introduced = false; // Aka mini state machine

public:
	UserID userID = UserID{ID};

public:
	explicit NetworkPeer(std::shared_ptr<NetworkLobby> lobby);
	~NetworkPeer();

private:
	virtual void on_connect(error_code ec) override;
	virtual void on_receive(error_code ec, message_view m) override;
	virtual void on_send(error_code ec, message_view m) override;
	virtual void on_disconnect(error_code ec) override;
};

// -------------------------------------------------------------------------------------------------

class AcceptorHandler : public libv::net::mtcp::AcceptorHandler<AcceptorHandler> {
//private:
public:
	std::shared_ptr<NetworkLobby> net_lobby;

public:
	explicit AcceptorHandler(std::shared_ptr<NetworkLobby>&& net_lobby);
	~AcceptorHandler();

private:
	virtual libv::net::mtcp::Connection<> on_accept(io_context& io_context) override;
	virtual void on_accept(error_code ec) override;
};

using Acceptor = libv::net::mtcp::Acceptor<AcceptorHandler>;

// =================================================================================================

struct ImplNetworkServer {
	libv::net::IOContext io_context{4};
	Acceptor acceptor;

	explicit inline ImplNetworkServer(std::shared_ptr<NetworkLobby>&& net_lobby) : acceptor(io_context, std::move(net_lobby)) {}
};

// -------------------------------------------------------------------------------------------------

} // namespace app
