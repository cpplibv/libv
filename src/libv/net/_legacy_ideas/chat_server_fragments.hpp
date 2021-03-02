// Project: libv.net, File: src/libv/net/mtcp/simple_server.hpp, Author: Császár Mátyás [Vader]

#pragma once

//#include <libv/net/mtcp/message.hpp>
//#include <unordered_set>
//#include <system_error>
//#include <memory>
//#include <queue>
//#include <system_error>
//#include <unordered_set>
//#include <memory>
//#include <system_error>
//#include <queue>
//#include <mutex>
//#include <set>
//#include <unordered_map>
//#include <iostream>


// -------------------------------------------------------------------------------------------------

//class SimpleSession {
//	explicit SimpleSession(SimpleConnectionToken connection);
//
//	void send(Message message);
//	void broadcast_group(Message message, bool send_to_self = false);
//	void broadcast_server(Message message, bool send_to_self = false);
//};

struct ChatClient {
	uint32_t roomID;
	std::unordered_map<uint32_t, std::string> client_names;
	std::unordered_map<uint32_t, std::string> channel_names;
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
//	static constexpr int max_recent_msgs = 100;
//	std::deque<std::string> recent_msgs;
//	std::deque<Sessions> recent_sessions;

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
