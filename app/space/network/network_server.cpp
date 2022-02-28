// Project: libv, File: app/space/network/network_server.cpp

// hpp
#include <space/network/network_server.hpp>
#include <space/network/network_server.hxx>
// libv
#include <libv/net/error.hpp>
// pro
#include <space/game/game_thread.hpp>
#include <space/game/user.hpp>
#include <space/log.hpp>
#include <space/network/codec.hpp>
#include <space/sim/playout/playout.hpp>
#include <space/sim/universe.hpp>
#include <space/sim/simulation.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

NetworkLobby::NetworkLobby(GameThread& game_thread, Playout& playout, User& hosting_user) :
	game_thread(game_thread),
	playout(playout) {

	// Self join the hosted lobby
	const auto program_version = 0; // TODO P1: Fetch global version
	const auto now = std::chrono::system_clock::now();
	lobby.process(CTO_ClientJoined{UserID{0}, hosting_user.name, now, program_version});
}

void NetworkLobby::connecting(libv::net::mtcp::Connection<NetworkPeer> peer) {
	std::unique_lock lock(mutex);
	connectings.emplace(std::move(peer));
}

void NetworkLobby::join(libv::net::mtcp::Connection<NetworkPeer> peer, std::string name, uint64_t version) {
	std::unique_lock lock(mutex);

	//	peer.connection().send_async(network_encode(CTO_Welcome{peer->userID}));

	const auto now = std::chrono::system_clock::now();
	auto join_event = CTO_ClientJoined{peer->userID, name, now, version};
	_broadcast(network_encode(join_event)); // Calling broadcast before adding to participants list

	// <<< Auto drop connection after N sec of no introduction
	connectings.erase(peer);
	participants.insert(peer);

	lobby.process(std::move(join_event));
	peer.connection().send_async(network_encode(static_cast<SnapshotLobby&>(lobby)));

//	peer.connection().send_async(network_encode(static_cast<SnapshotUniverse&>(universe)));
	{
		std::vector<std::byte> result;
		{
			libv::archive::BinaryOutput oar{result};
			network_codec.encode(oar, SimulationSnapshotTag{});
			playout.saveSimulationSnapshot(oar, SnapshotType::shared);
		}
		peer.connection().send_async(std::move(result));
	}
//	peer.connection().send_async(network_encode(SnapshotUniverse(universe)));
//	peer.connection().send_async(network_encode(universe));

	// TODO P1: Universe Snapshot here (after lobby snapshot are done, but maybe not in the same "task"). Maybe even async it
}

void NetworkLobby::leave(libv::net::mtcp::Connection<NetworkPeer> peer) {
	std::unique_lock lock(mutex);
	participants.erase(peer);
	connectings.erase(peer);

	auto leave_event = CTO_ClientLeave{peer->userID};
	_broadcast(network_encode(leave_event));

	lobby.process(std::move(leave_event));
}

template <typename Message>
		requires requires { typename Message::lobby_command; }
void NetworkLobby::process(NetworkPeer& peer, libv::net::mtcp::message&& raw_message, Message&& message) {
	std::unique_lock lock(mutex);

	(void) peer;

	// <<< Remap userID (for chat messages) before broadcasting it
	// <<< Verify time stamp within <config:3> sec, rewrite wth now() if mismatches
	// Excess .copy_bin(), view would be enough, but remap will happen, so dont need to worry that much about it
	// or it should be used with message_view_proxy to eliminate the copy for each peer
	_broadcast(raw_message.copy_bin());
	lobby.process(std::move(message));
}

void NetworkLobby::broadcast(std::vector<std::byte>&& msg) {
	std::unique_lock lock(mutex);
	_broadcast(std::move(msg));
}

void NetworkLobby::_broadcast(std::vector<std::byte>&& msg) {
	for (const auto& participant : participants)
		participant.connection().send_async(msg);
}

void NetworkLobby::disconnect_all() {
	std::unique_lock lock(mutex);

//	for (const auto& peer : connectings)
//		log_space.fatal("[Peer {}] (connecting) ref_count {}", +peer->userID, peer.handler().ref_count());
//	for (const auto& peer : participants)
//		log_space.fatal("[Peer {}] (connected) ref_count {}", +peer->userID, peer.handler().ref_count());

	for (const auto& peer : connectings)
		peer.connection().cancel_and_disconnect_async();

	for (const auto& peer : participants)
		peer.connection().cancel_and_disconnect_async();
}

// =================================================================================================

NetworkPeer::NetworkPeer(std::shared_ptr<NetworkLobby> lobby) :
	lobby(std::move(lobby)) {
}

NetworkPeer::~NetworkPeer() {
	log_space.trace("[Peer {}] ~NetworkPeer", ID);
}

void NetworkPeer::on_connect() {
	lobby->connecting(connection_from_this());
}

void NetworkPeer::on_connect_error(error_code ec) {
	log_space.error("[Peer {}] on_connect {}", ID, libv::net::to_string(ec));
}

void NetworkPeer::on_receive(message&& m) {
	log_space.trace("[Peer {}] on_receive: {}", ID, debug_binary_as_json(m.as_bin()));

	try {
		libv::archive::Binary::input iar(m.as_bin());
		network_codec.decode(iar, [this, &m]<typename T>(T&& message) {
			if constexpr(std::is_same_v<T, CTO_Introduction>) {
				if (introduced)
					throw ProtocolViolation();

				log_space.trace("[Peer {}] introduce itself as {} with v{}", ID, message.user_name, message.version);
				lobby->join(connection_from_this(), std::move(message.user_name), message.version);
				introduced = true;

			} else {
				if (!introduced)
					throw ProtocolViolation();

				if constexpr(requires { typename T::lobby_command; }) {
					lobby->process(*this, std::move(m), std::move(message));

				} else if constexpr(requires { typename T::state_command; }) {
					lobby->broadcast(m.copy_bin());
					lobby->playout.process_from_network(std::move(message));

//				} else if constexpr(requires { typename T::track_command; }) {
//					game_thread.execute([m = std::move(message)] { lobby->tracking.process(std::move(m)); });
//					tracking.process(std::move(message));

				} else
					return Codec::RejectMessage{};
			}
		});

	} catch (const ProtocolViolation& e) {
		log_space.error("[Peer {}] ProtocolViolation {}", ID, e.what());
		connection.cancel_and_disconnect_async();

	} catch (const Codec::UnexpectedIDException& e) {
		log_space.error("[Peer {}] UnexpectedIDException {}", ID, libv::to_underlying(e.id));
		connection.cancel_and_disconnect_async();

	} catch (const std::exception& e) {
		log_space.error("[Peer {}] Exception {}", ID, e.what());
		connection.cancel_and_disconnect_async();
	}
}

void NetworkPeer::on_receive_error(error_code ec, message&&) {
	log_space.error("[Peer {}] on_receive_error {}", ID, libv::net::to_string(ec));
}

void NetworkPeer::on_send(message&& m) {
	log_space.trace("[Peer {}] on_send: {}", ID, debug_binary_as_json(m.as_bin()));
}

void NetworkPeer::on_send_error(error_code ec, message&& m) {
	log_space.error("[Peer {}] on_send_error {} message would have been: {}", ID, libv::net::to_string(ec), debug_binary_as_json(m.as_bin()));
}

void NetworkPeer::on_disconnect(error_code ec) {
	if (ec)
		log_space.error("[Peer {}] on_disconnect {}", ID, libv::net::to_string(ec));
	else
		log_space.trace("[Peer {}] on_disconnect", ID);

	lobby->leave(connection_from_this());
}

// =================================================================================================

AcceptorHandler::AcceptorHandler(std::shared_ptr<NetworkLobby>&& net_lobby) :
	net_lobby(std::move(net_lobby)) {}

AcceptorHandler::~AcceptorHandler() {
	log_space.trace("~AcceptorHandler");
	net_lobby->disconnect_all();
}

libv::net::mtcp::Connection<> AcceptorHandler::on_accept(io_context& io_context) {
	return libv::net::mtcp::Connection<NetworkPeer>(io_context, net_lobby);
}

void AcceptorHandler::on_accept(error_code ec) {
	log_space.error("Failed to accept incoming connection: {}", libv::net::to_string(ec));
}

// =================================================================================================

//NetworkServer::NetworkServer(uint16_t server_port, GameThread& game_thread, Playout& playout, Universe& universe, User& hosting_user) :
//	self(std::make_unique<ImplNetworkServer>(std::make_shared<NetworkLobby>(game_thread, playout, universe, hosting_user))) {
NetworkServer::NetworkServer(uint16_t server_port, GameThread& game_thread, Playout& playout, User& hosting_user) :
	self(std::make_unique<ImplNetworkServer>(std::make_shared<NetworkLobby>(game_thread, playout, hosting_user))) {

	if (auto ec = self->acceptor.acceptor().listen(server_port, 4))
		throw std::system_error(ec);

	self->acceptor.acceptor().accept_async();
}

NetworkServer::~NetworkServer() {
	log_space.trace("~NetworkServer");
}

void NetworkServer::broadcast(std::vector<std::byte> message) {
	self->acceptor->net_lobby->broadcast(std::move(message));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
