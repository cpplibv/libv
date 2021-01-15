// Project: libv.net, File: src/libv/net/mtcp/simple_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

// ext
// std
// pro




#include <libv/net/mtcp/message.hpp>
#include <libv/net/mtcp/acceptor.hpp>

//#include <libv/net/mtcp/simple_server.hpp>
#include <unordered_set>
#include <system_error>

#include <memory>
#include <queue>
#include <system_error>
#include <unordered_set>
#include <memory>
#include <system_error>
#include <queue>
#include <mutex>
#include <set>
#include <unordered_map>
#include <iostream>



// TODO P5: wish: set(CMAKE_CXX_STANDARD 20)
// TODO P5: SimpleServer: design adopt API in a way that even outgoing connections could be adopted by the server (complication is start and receives)
// HTTPS download?: https://curl.se/libcurl/c/url2file.html
// https://stackoverflow.com/questions/28264313/ssl-certificates-and-boost-asio
// https://github.com/yhirose/cpp-httplib


namespace libv {
namespace net {
namespace mtcp {

// -------------------------------------------------------------------------------------------------

struct SimpleClient {
	virtual void on_connect() = 0;
	virtual void on_error(std::error_code ec) = 0;
	virtual void on_receive(Message message) = 0;
//	virtual void on_send(Message message) = 0;
	virtual void on_disconnect() = 0;
};

// -------------------------------------------------------------------------------------------------

struct SimpleConnectionToken {
	libv::net::mtcp::ConnectionCB connection;
};

class BaseConnectionContainer {
	virtual void remove(SimpleConnectionToken*) = 0;
};

// -------------------------------------------------------------------------------------------------

class SimpleSession {
private:
	SimpleConnectionToken connection;
	BaseConnectionContainer* container;

protected:
	explicit SimpleSession(SimpleConnectionToken connection);

protected:
	void send(Message message);
	void broadcast_group(Message message, bool send_to_self = false);
	void broadcast_server(Message message, bool send_to_self = false);

private:
//	virtual void on_connect() = 0;
//	virtual void on_error(std::error_code ec) = 0;
	virtual void on_receive(Message message) = 0;
	virtual void on_send(Message message) = 0;
//	virtual void on_disconnect() = 0;
};

// -------------------------------------------------------------------------------------------------

class BaseConnectionSet : BaseConnectionContainer {
	struct Entry {
		//std::shared_ptr<SimpleSession> object;

		std::shared_ptr<SimpleSession> object;
		//libv::net::mtcp::ConnectionCB connection;
	};

	std::mutex mutex;
	std::unordered_set<Entry> storage;

//	void emplace(SimpleSession);
	template <typename... Args>
	void emplace(SimpleSession, Args&& ...);
//	void erase(SimpleSession);

	virtual void remove(SimpleSession) override;
};

template <typename T>
class ConnectionSet : BaseConnectionSet {

};

class BaseConnectionQueue : BaseConnectionContainer {
	struct Entry {
		std::shared_ptr<SimpleSession> object;
		//libv::net::mtcp::ConnectionCB connection;
	};

	std::mutex mutex;
	std::queue<Entry> storage;

public:
	void push_back(SimpleSession);

	template <typename... Args>
	void emplace_back(SimpleSession, Args&& ...);

	SimpleSession pop_front();

private:
	virtual void remove(SimpleSession) override;
};

template <typename T = void>
class ConnectionQueue : BaseConnectionQueue {

};

// -------------------------------------------------------------------------------------------------

struct SimpleServer {
	std::unique_ptr<class ImplSimpleServer> self;

//	virtual on_accept(libv::net::mtcp::ConnectionAsyncCB connection) override;
	void on_accept(libv::net::mtcp::ConnectionAsyncCB connection);

public:
	void listen(uint16_t port, int backlog = 4) noexcept;
	void listen(libv::net::mtcp::Endpoint endpoint, int backlog = 4) noexcept;
	void start() noexcept;

public:
	void adopt(libv::net::mtcp::ConnectionAsyncCB connection);

private:
	virtual void on_connect(SimpleConnectionToken connection) = 0;
	virtual void on_error(SimpleConnectionToken connection, std::error_code ec) = 0;
	virtual void on_disconnect(SimpleConnectionToken connection) = 0;
};

// -------------------------------------------------------------------------------------------------

} // namespace mtcp
} // namespace net
} // namespace libv


// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================
// =================================================================================================



class ResourceSession : public libv::net::mtcp::SimpleSession {

};

class ResourceServer : public libv::net::mtcp::SimpleServer {
	std::mutex mutex;

	libv::update::ResourceServer::Settings settings;

//	libv::net::mtcp::ConnectionSet<ResourceSession> sessions;
//	std::set<ResourceSession*> active_peers;
//	std::queue<ResourceSession*> queued_peers;

	libv::net::mtcp::ConnectionSet<ResourceSession> active_peers;
	libv::net::mtcp::ConnectionQueue<> queued_peers;

	int file_maps;

public:


private:
	virtual void on_connect(libv::net::mtcp::SimpleConnectionToken connection) override {
		const auto lock = std::unique_lock(mutex);

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

	virtual void on_error(libv::net::mtcp::SimpleConnectionToken connection, std::error_code ec) override {
		const auto lock = std::unique_lock(mutex);

		// log ec;
	}

	virtual void on_disconnect(libv::net::mtcp::SimpleConnectionToken connection) override {
		const auto lock = std::unique_lock(mutex);

		if (!queued_peers.empty()) {
			if (active_peers.size() < settings.limit_peer_count_active || settings.limit_peer_count_active == 0) {
				active_peers.emplace(queued_peers.pop_front(), *this);
			}
		}
	}
};

// =================================================================================================

struct ChatClient {
	uint32_t roomID;
	std::unordered_map<uint32_t, std::string> client_names;
	std::unordered_map<uint32_t, std::string> channel_names;


};

struct ChatSession {
	class ChatServer& server;
	std::set<class ChatRoom*> rooms;

	virtual void on_connect(libv::net::mtcp::SimpleConnectionToken connection) override {
		const auto lock = std::unique_lock(mutex);
		every_clients.emplace(connection, *this);
	}
//	virtual void on_error(libv::net::mtcp::SimpleConnectionToken connection, std::error_code ec) override {
//		const auto lock = std::unique_lock(mutex);
//		// log ec;
//	}
	virtual void on_disconnect(std::error_code ec) override {
		const auto lock = std::unique_lock(mutex);
		for (auto& room : rooms)
			room->leave(this);
	}
};

struct ChatMessage {
	uint32_t roomID;
	uint32_t clientID;
//	int utc_timestamp;
//	std::string sender;
//	libv::serial::limited<std::string, 16 * 1024> message;
	std::string message;
};

struct ChatRoom {
	uint32_t id;
	std::set<class ChatSession*> clients;
//	libv::net::mtcp::ConnectionSet<ChatRoomSession> clients;

	void enter(ChatSession& cs) {
		clients.emplace(&cs);
	}
	void leave(ChatSession& cs) {
		clients.erase(&cs);
	}
	void send(std::string text) {
		for (const auto& client : clients)
			client.send(cs, text);
	}
};

class ChatServer : public libv::net::mtcp::SimpleServer {
	std::mutex mutex;
	libv::net::mtcp::ConnectionSet<ChatSession> every_clients;

public:
	std::unordered_map<std::string, ChatRoom> chat_rooms;


private:
	virtual void on_connect(libv::net::mtcp::SimpleConnectionToken connection) override {
		const auto lock = std::unique_lock(mutex);
		every_clients.emplace(connection, *this);
	}
	virtual void on_error(libv::net::mtcp::SimpleConnectionToken connection, std::error_code ec) override {
		const auto lock = std::unique_lock(mutex);
		// log ec;
	}
	virtual void on_disconnect(libv::net::mtcp::SimpleConnectionToken connection) override {
		const auto lock = std::unique_lock(mutex);
		// log dc
	}
};

// =================================================================================================
// =================================================================================================

class ChatServer : public libv::net::mtcp::SimpleServer {
	std::mutex mutex;
	libv::net::mtcp::ConnectionSet<ChatSession> every_clients;

public:
	std::unordered_map<std::string, ChatRoom> chat_rooms;


private:
	virtual void on_connect(libv::net::mtcp::ConnectionAsyncCB connection) override {
		const auto lock = std::unique_lock(mutex);
		every_clients.emplace(connection, *this);
	}
	virtual void on_error(std::error_code ec) override {
		const auto lock = std::unique_lock(mutex);
		// log ec;
	}
	virtual void on_disconnect(libv::net::mtcp::SimpleConnectionToken connection) override {
		const auto lock = std::unique_lock(mutex);
		// log dc
	}
};


// =================================================================================================
// =================================================================================================

int main() {
	ResourceServer server;
	server.listen(29701, 4);
	server.start();

	std::cout << "Hello, World!" << std::endl;
	return 0;
}
