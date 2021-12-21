// Project: libv, File: app/space/network/network_client.cpp

// hpp
#include <space/network/network_client.hpp>
// libv
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/connection_he.hpp>
// std
//#include <mutex>
// pro
#include <space/game_thread.hpp>
#include <space/internal_events.hpp>
#include <space/log.hpp>
#include <space/network/codec.hpp>
#include <space/network/lobby.hpp>
#include <space/playout.hpp>
#include <space/user.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class ConnectionHandler : public libv::net::mtcp::ConnectionHandler<ConnectionHandler> {
private:
//	std::mutex mutex; // NOTE: Client network API is currently single threaded, this might change with io_context timers

	GameThread& game_thread;
	Playout& playout;
	Universe& universe;
	Lobby lobby{game_thread};

private:
	std::string name;
public:
	bool disconnectExpected = false;

public:
	inline ConnectionHandler(std::string_view server_address, uint16_t server_port, std::string name, GameThread& game_thread, Playout& playout, Universe& universe) :
			game_thread(game_thread),
			playout(playout),
			universe(universe),
			name(std::move(name)) {
		connection.connect_async(libv::net::Address(server_address, server_port));
	}

	~ConnectionHandler() {
	}

public:
	void write(std::vector<std::byte>&& message) {
		connection.send_async(std::move(message));
	}

private:
	virtual void on_connect(error_code ec) override {
		if (ec)
			return log_space.error("[{}] on_connect {}", name, libv::net::to_string(ec));

		const auto program_version = 0; // TODO P1: Fetch global version
		write(network_encode(CTO_Introduction{name, program_version}));
		log_space.trace("[{}] on_connect", name);
	}

	virtual void on_receive(error_code ec, message_view m) override {
		if (ec)
			return log_space.error("[{}] on_receive {}", name, libv::net::to_string(ec));

		log_space.trace("[{}] on_receive:\n{}", name, debug_binary_as_json(m.as_bin()));

		try {
//			UniverseArchive<libv::archive::Binary::input> iar{universe, false, m.as_bin()};
//			libv::archive::Binary::input iar(m.as_bin());
			SnapshotArchive<libv::archive::BasicBinaryInput> iar{false, m.as_bin()};
			network_codec.decode(iar, [this]<typename T>(T&& message) {
				if constexpr(std::is_same_v<T, SnapshotLobby>)
					lobby.process(std::move(message));

				else if constexpr(std::is_same_v<T, SnapshotUniverse>)
					game_thread.execute([&, snapshot = std::move(message)] {
						static_cast<SnapshotUniverse&>(universe) = std::move(snapshot);
					});

				else if constexpr(requires { typename T::lobby_command; })
					lobby.process(std::move(message));

				else if constexpr(requires { typename T::state_command; })
					playout.process_from_network(std::move(message));

//				else if constexpr(requires { T::track_command; })
//					tracking.process(message);

				else
					return Codec::RejectMessage{};
			});

		} catch (const ProtocolViolation& e) {
			log_space.error("[{}] ProtocolViolation {}", name, e.what());
			connection.cancel_and_disconnect_async();

		} catch (const Codec::UnexpectedIDException& e) {
			log_space.error("[{}] UnexpectedIDException {}", name, libv::to_underlying(e.id));
			connection.cancel_and_disconnect_async();

		} catch (const std::exception& e) {
			log_space.error("[{}] Exception {}", name, e.what());
			connection.cancel_and_disconnect_async();
		}
	}

	virtual void on_send(error_code ec, message_view m) override {
		if (ec)
			return log_space.error("[{}] on_send {} message would have been: {}", name, libv::net::to_string(ec), debug_binary_as_json(m.as_bin()));

		(void) m;

		log_space.trace("[{}] on_send:\n{}", name, debug_binary_as_json(m.as_bin()));
	}

	virtual void on_disconnect(error_code ec) override {
		if (ec)
			log_space.error("[{}] on_disconnect {} disconnectExpected {}", name, libv::net::to_string(ec), disconnectExpected);
		else
			log_space.trace("[{}] on_disconnect {} disconnectExpected {}", name, libv::net::to_string(ec), disconnectExpected);

		if (!disconnectExpected)
			game_thread.broadcast_on_thread(mc::RequestDestroyClient{});
	}
};

using Connection = libv::net::mtcp::Connection<ConnectionHandler>;

// =================================================================================================

struct ImplNetworkClient {
	libv::net::IOContext io_context{2};
	Connection connection;

	ImplNetworkClient(std::string server_address, uint16_t server_port, std::string name, GameThread& game_thread, Playout& playout, Universe& universe) :
			connection(io_context, std::move(server_address), server_port, std::move(name), game_thread, playout, universe) {}
};

// =================================================================================================

NetworkClient::NetworkClient(std::string server_address, uint16_t server_port, GameThread& game_thread, Playout& playout, Universe& universe, User& user) :
		self(std::make_unique<ImplNetworkClient>(std::move(server_address), server_port, user.name, game_thread, playout, universe)) {
}

NetworkClient::~NetworkClient() {
	self->connection->disconnectExpected = true;
//	self->connection.connection().disconnect_async();
//	self->io_context.join();
	// For the sake of forward declared unique_ptr
}

// -------------------------------------------------------------------------------------------------

void NetworkClient::send(std::vector<std::byte> message) {
	self->connection->write(std::move(message));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
