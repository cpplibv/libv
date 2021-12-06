// Project: libv.net, File: sandbox/libv_net_main.cpp

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
	std::size_t num_client_io_thread = 2;
	std::size_t num_server_io_thread = 4;
//	std::size_t num_client_io_thread = 1;
//	std::size_t num_server_io_thread = 1;
} const config;

// =================================================================================================

class chat_participant {
public:
	virtual void deliver(const std::string& msg) = 0;
	virtual ~chat_participant() = default;
};

// -------------------------------------------------------------------------------------------------

class chat_room {
	static constexpr int max_recent_msgs = 100;

private:
	std::mutex mutex;
	std::set<net::Connection<chat_participant>> participants;
	std::deque<std::string> recent_msgs;

public:
	void join(net::Connection<chat_participant> participant) {
		DEBUG_COUT("[Room] join");
		std::unique_lock lock(mutex);
		participants.insert(participant);
		for (const auto& msg : recent_msgs)
			participant->deliver(msg);
	}

	void leave(net::Connection<chat_participant> participant) {
		DEBUG_COUT("[Room] leave");
		std::unique_lock lock(mutex);
		participants.erase(participant);
	}

	void deliver(const std::string& msg) {
		DEBUG_COUT("[Room] deliver '" << msg << '\'');
		std::unique_lock lock(mutex);
		recent_msgs.push_back(msg);
		while (recent_msgs.size() > max_recent_msgs)
			recent_msgs.pop_front();

		for (const auto& participant : participants)
			participant->deliver(msg);
	}

	void disconnect_all() {
		DEBUG_COUT("[Room] disconnect_all");
		for (const auto& participant : participants)
			participant.connection().cancel_and_disconnect_async();
	}
};

// -------------------------------------------------------------------------------------------------

class chat_session : public chat_participant, public net::ConnectionHandler<chat_session> {
private:
	std::shared_ptr<chat_room> room;
	static inline int nextID = 0; int ID = nextID++;

public:
	explicit chat_session(std::shared_ptr<chat_room> room) :
		room(std::move(room)) {
	}

	~chat_session() {
		DEBUG_COUT("[Session " << ID << "] ~chat_session");
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

		DEBUG_COUT("[Session " << ID << "] on_receive " << libv::net::to_string(ec) << " '" << (ec ? "" : m.as_str()) << "'");
	}
	virtual void on_send(error_code ec, message_view m) override {
		(void) ec;
		(void) m;
		DEBUG_COUT("[Session " << ID << "] on_send " << libv::net::to_string(ec) << " '" << (ec ? "" : m.as_str()) << "'");
	}
	virtual void on_disconnect(error_code ec) override {
		(void) ec;
		room->leave(connection_from_this());
		DEBUG_COUT("[Session " << ID << "] on_disconnect " << libv::net::to_string(ec));
	}
};

// -------------------------------------------------------------------------------------------------

class chat_server : public net::AcceptorHandler<chat_server> {
public:
	std::shared_ptr<chat_room> room = std::make_shared<chat_room>();

public:
	inline explicit chat_server(uint16_t port) {
		if (auto ec = acceptor.listen(port, 4))
			throw std::system_error(ec);

		acceptor.accept_async();
	}

	~chat_server() {
		DEBUG_COUT("[Server] ~chat_server");
		room->disconnect_all();
	}

private:
	virtual net::Connection<> on_accept(io_context& io_context) override {
		DEBUG_COUT("[Server] on_accept");
		return net::Connection<chat_session>(io_context, room);
	}
	virtual void on_accept(error_code ec) override {
		DEBUG_COUT("[Server] on_accept error " << libv::net::to_string(ec));
	}
};

// =================================================================================================

class chat_client : public net::ConnectionHandler<chat_client> {
private:
	std::string name;

public:
	inline explicit chat_client(std::string_view address, std::string_view name) :
		name(name) {
		connection.connect_async(libv::net::Address(address, config.port));
	}

	~chat_client() {
		DEBUG_COUT("[" << name << "] ~chat_client");
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

		DEBUG_COUT("[" << name << "] on_receive " << libv::net::to_string(ec) << " '" << (ec ? std::string_view("") : m.as_str()) << "'");
	}

	virtual void on_send(error_code ec, message_view m) override {
		(void) ec;
		(void) m;
		DEBUG_COUT("[" << name << "] on_send " << libv::net::to_string(ec) << " '" << (ec ? std::string_view("") : m.as_str()) << "'");
	}

	virtual void on_disconnect(error_code ec) override {
		(void) ec;
		DEBUG_COUT("[" << name << "] on_disconnect " << libv::net::to_string(ec));
	}
};

// =================================================================================================

//void main_auto_server() {
//	auto io_context = libv::net::IOContext{config.num_server_io_thread};
//
//	{
//		std::this_thread::sleep_for(std::chrono::seconds(4));
//		auto server = net::Acceptor<chat_server>(io_context, config.port);
//		std::this_thread::sleep_for(std::chrono::seconds(6));
//	}
//
//	io_context.join();
//}
//
//void main_auto_client(int client_id) {
//	auto io_context = libv::net::IOContext{config.num_client_io_thread};
//
//	{
//		auto client = net::Connection<chat_client>(io_context, "Client " + std::to_string(client_id));
////		auto client = io_context.connection<chat_client>("client" + std::to_string(client_id));
//
////		auto client = chat_client(io_context);
////		client.connection.connect_async(libv::net::Address("localhost", config.port));
////		auto client = chat_client(io_context);
////		client.connect_async("localhost", config.port);
//
//		for (int i = 0; i < 5; i++) {
//			std::this_thread::sleep_for(std::chrono::seconds(1));
//			client->write(libv::concat("Message ", i));
//		}
//	}
//
//	io_context.join();
//}
//
//int main(int, char**) {
////	libv::logger_stream.setFormat("{severity} {thread_id:2} {module:8}: {message}\n");
//	std::cout << libv::logger_stream;
//
//	std::vector<std::jthread> threads;
//
//	threads.emplace_back(main_auto_server);
//
//	for (std::size_t i = 0; i < 14; ++i) {
//		threads.emplace_back(main_auto_client, i);
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//	}
//
//	return 0;
//}

void main_server() {
	auto io_context = libv::net::IOContext{config.num_server_io_thread};
	auto server = net::Acceptor<chat_server>(io_context, config.port);

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
	auto client = net::Connection<chat_client>(io_context, address, name);

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
