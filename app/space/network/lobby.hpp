// Project: libv, File: app/space/network/lobby.hpp

#pragma once

// fwd
#include <space/fwd.hpp>
// libv
#include <libv/serial/codec_message_id.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_chrono.hpp>
#include <libv/serial/types/std_deque.hpp>
#include <libv/serial/types/std_memory.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <libv/utility/nexus_fwd.hpp>
// std
#include <chrono>
#include <deque>
#include <memory>
#include <string>
#include <vector>
// pro
#include <space/universe/ids.hpp> // TODO P2: UserID should be moved somewhere else


namespace space {

// -------------------------------------------------------------------------------------------------

struct SnapshotLobby {
	static constexpr libv::serial::CodecMessageID id{10};

protected:
	struct Client {
		UserID userID;
		std::string name;
		std::chrono::system_clock::time_point joined_at;
		uint64_t version;

		float ping = -1;
//		float jitter = -1;
//		float packet_loss = -1;

		template <typename Archive> void serialize(Archive& ar) {
			ar & LIBV_NVP(userID);
			ar & LIBV_NVP(name);
			ar & LIBV_NVP(joined_at);
			ar & LIBV_NVP(version);

			ar & LIBV_NVP(ping);
//			ar & LIBV_NVP(jitter);
//			ar & LIBV_NVP(packet_loss);
		}
	};

	struct ChatEntry {
		std::shared_ptr<Client> sender;
		std::chrono::system_clock::time_point sent_at;
		std::string message;

		template <typename Archive> void serialize(Archive& ar) {
			ar & LIBV_NVP(sender);
			ar & LIBV_NVP(sent_at);
			ar & LIBV_NVP(message);
		}
	};

public:
	std::vector<std::shared_ptr<Client>> clients;
	std::deque<ChatEntry> chat_entries;

public:
	template <typename Archive> void serialize(Archive& ar) {
		ar & LIBV_NVP(clients);
		ar & LIBV_NVP(chat_entries);
	}
};

/// Common lobby state shared between clients and server
class Lobby : public SnapshotLobby {
	friend NetworkLobby;
public:
	struct OnClientJoin {
		std::shared_ptr<Client> client;
	};

	struct OnClientLeave {
		std::shared_ptr<Client> client;
	};

	struct OnChatMessage {
		// Note: Not sure if allowing references without ownership is acceptable for lobby
		const ChatEntry& entry;
	};

private:
	GameThread& game_thread;

public:
	explicit inline Lobby(GameThread& game_thread) noexcept :
		game_thread(game_thread) {}

	~Lobby();

	void process(CTO_ClientJoined&& message);
	void process(CTO_ClientLeave&& message);
	void process(CTO_ChatMessage&& message);
	void process(CTO_LobbyStatus&& message);
//	void process(CTO_LobbyClose&& message);

	void process(SnapshotLobby&& message);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
