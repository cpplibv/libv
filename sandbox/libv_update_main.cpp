// Project: libv.update, File: sandbox/libv_update_main.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/log/log.hpp>
#include <libv/net/address.hpp>
#include <libv/net/error.hpp>
#include <libv/net/io_context.hpp>
#include <libv/net/mtcp/acceptor_he.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/utility/concat.hpp>
// std
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <mutex>
// pro
#include <libv/update/resource_server/resource_server.hpp>


// =================================================================================================

std::mutex cout_mutex;
#define DEBUG_COUT(...) {cout_mutex.lock(); std::cout << "        " << __VA_ARGS__ << std::endl; cout_mutex.unlock();}
//#define DEBUG_COUT(...) {}

namespace net {

template <typename T = void> using Connection = libv::net::mtcp::Connection<T>;
template <typename T> using ConnectionHandler = libv::net::mtcp::ConnectionHandler<T>;
template <typename T = void> using Acceptor = libv::net::mtcp::Acceptor<T>;
template <typename T> using AcceptorHandler = libv::net::mtcp::AcceptorHandler<T>;

} // namespace net

// -------------------------------------------------------------------------------------------------

inline libv::LoggerModule log_sandbox{libv::logger_stream, "sandbox"};

struct Config {
	uint16_t port = 25090;
	size_t num_client_io_thread = 2;
	size_t num_server_io_thread = 4;
//	size_t num_client_io_thread = 1;
//	size_t num_server_io_thread = 1;
} const config;

// =================================================================================================

class resource_participant {
public:
	virtual void deliver(const std::string& msg) = 0;
	virtual ~resource_participant() = default;
};

// -------------------------------------------------------------------------------------------------

class resource_room {
	static constexpr int max_recent_msgs = 100;

private:
	std::mutex mutex;
//	std::set<net::Connection<resource_participant>> participants;
//	std::deque<std::string> recent_msgs;

	libv::update::ResourceServer::Settings settings;
	std::deque<net::Connection<resource_participant>> active_peers;
	std::set<net::Connection<resource_participant>> queued_peers;

	int file_maps;

public:
	void join(net::Connection<resource_participant> participant) {
		std::unique_lock lock(mutex);
		DEBUG_COUT("[Room] join");
//		participants.insert(participant);
//		for (const auto& msg : recent_msgs)
//			participant->deliver(msg);

		if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
			active_peers.emplace(connection, *this);
			return;
		}

		if (queued_peers.size() < settings.limit_peer_count_queue || settings.limit_peer_count_queue == 0) {
			queued_peers.emplace(connection);
			return;
		}

		// Drop connection
		// Access to codec is questionable here?
//		connection.send(ResponseBusy{load_trend.busy_time()});
	}

	void leave(net::Connection<resource_participant> participant) {
		std::unique_lock lock(mutex);
		DEBUG_COUT("[Room] leave");
//		participants.erase(participant);

		if (!queued_peers.empty()) {
			if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
				active_peers.emplace(queued_peers.pop_front(), *this);
			}
		}
	}

	void deliver(const std::string& msg) {
		std::unique_lock lock(mutex);
		DEBUG_COUT("[Room] deliver '" << msg << '\'');
//		recent_msgs.push_back(msg);
//		while (recent_msgs.size() > max_recent_msgs)
//			recent_msgs.pop_front();

		for (const auto& participant : participants)
			participant->deliver(msg);
	}

	void disconnect_all() {
		std::unique_lock lock(mutex);
		DEBUG_COUT("[Room] disconnect_all");
		for (const auto& participant : active_peers)
			participant.connection().cancel_and_disconnect_async();
		for (const auto& participant : queued_peers)
			participant.connection().cancel_and_disconnect_async();
	}
};

// -------------------------------------------------------------------------------------------------

class resource_session : public resource_participant, public net::ConnectionHandler<resource_session> {
private:
	std::shared_ptr<resource_room> room;
	static inline int nextID = 0; int ID = nextID++;

public:
	explicit resource_session(std::shared_ptr<resource_room> room) :
		room(std::move(room)) {
	}

	~resource_session() {
		DEBUG_COUT("[Session " << ID << "] ~resource_session");
	}
private:
	virtual void deliver(const std::string& msg) override {
		connection.send_async(msg);
	}

private:
	virtual void on_connect(error_code ec) override {
		if (!ec)
			room->join(connection_from_this());

		DEBUG_COUT("[Session " << ID << "] on_connect " << libv::net::to_string(ec));
	}
	virtual void on_receive(error_code ec, message_view m) override {
		if (!ec)
			room->deliver(std::string(m.as_str()));

		DEBUG_COUT("[Session " << ID << "] on_receive " << libv::net::to_string(ec) << " '" << (ec ? std::string("") : m) << "'");
	}
	virtual void on_send(error_code ec, message_view m) override {
		(void) ec;
		(void) m;
		DEBUG_COUT("[Session " << ID << "] on_send " << libv::net::to_string(ec) << " '" << (ec ? std::string("") : m) << "'");
	}
	virtual void on_disconnect(error_code ec) override {
		(void) ec;
		room->leave(connection_from_this());
		DEBUG_COUT("[Session " << ID << "] on_disconnect " << libv::net::to_string(ec));
	}
};

// -------------------------------------------------------------------------------------------------

class resource_server : public net::AcceptorHandler<resource_server> {
public:
	std::shared_ptr<resource_room> room = std::make_shared<resource_room>();

public:
	inline explicit resource_server(uint16_t port) {
		if (auto ec = acceptor.listen(port, 4))
			throw std::system_error(ec);

		acceptor.accept_async();
	}

	~resource_server() {
		DEBUG_COUT("[Server] ~resource_server");
		room->disconnect_all();
	}

private:
	virtual net::Connection<> on_accept(io_context& io_context) override {
		DEBUG_COUT("[Server] on_accept");
		return net::Connection<resource_session>(io_context, room);
	}
	virtual void on_accept(error_code ec) override {
		DEBUG_COUT("[Server] on_accept error " << libv::net::to_string(ec));
	}
};

// =================================================================================================

class resource_client : public net::ConnectionHandler<resource_client> {
private:
	std::string name;

public:
	inline explicit resource_client(std::string_view address, std::string_view name) :
			name(name) {
		connection.connect_async(libv::net::Address(address, config.port));
	}

	~resource_client() {
		DEBUG_COUT("[" << name << "] ~resource_client");
	}

public:
	void write(const std::string& message) {
		connection.send_async(name + ": " + message);
	}

private:
	virtual void on_connect(error_code ec) override {
		if (!ec)
			write("Hello");
		DEBUG_COUT("[" << name << "] on_connect " << libv::net::to_string(ec));
	}

	virtual void on_receive(error_code ec, message_view m) override {
		if (!ec)
			std::cout << m.as_str() << std::endl;

		DEBUG_COUT("[" << name << "] on_receive " << libv::net::to_string(ec) << " '" << (ec ? "" : m.as_str()) << "'");
	}

	virtual void on_send(error_code ec, message_view m) override {
		(void) ec;
		(void) m;
		DEBUG_COUT("[" << name << "] on_send " << libv::net::to_string(ec) << " '" << (ec ? "" : m.as_str()) << "'");
	}

	virtual void on_disconnect(error_code ec) override {
		(void) ec;
		DEBUG_COUT("[" << name << "] on_disconnect " << libv::net::to_string(ec));
	}
};

// =================================================================================================

void main_server() {
	auto io_context = libv::net::IOContext{config.num_server_io_thread};
	auto server = net::Acceptor<resource_server>(io_context, config.port);

	for (std::string line; std::getline(std::cin, line);) {
		if (line.starts_with('/')) {
			if (line == "/quit")
				break;
			else
				std::cout << "Unknown command: " << line << std::endl;
		} else {
			server->room->deliver(libv::concat("Server: ", line));
		}
	}

	server.clear();
	io_context.join();
}

void main_client(std::string_view address, std::string_view name) {
	auto io_context = libv::net::IOContext{config.num_client_io_thread};
	auto client = net::Connection<resource_client>(io_context, address, name);

	for (std::string line; std::getline(std::cin, line);) {
		if (line.starts_with('/')) {
			if (line == "/quit")
				break;
			else
				std::cout << "Unknown command: " << line << std::endl;
		} else {
			client->write(line);
		}
	}

	client.clear();
	io_context.join();
}

int main(int argc, char** argv) {
	libv::logger_stream.setFormat("                {severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	if (argc < 2 || (std::string_view(argv[1]) == "c" && argc < 4)) {
		std::cout << "Usage: s | c address name" << std::endl;
		return EXIT_FAILURE;
	}

	if (std::string_view(argv[1]) == "c")
		main_client(argv[2], argv[3]);
	else
		main_server();

	return EXIT_SUCCESS;
}

// -------------------------------------------------------------------------------------------------
