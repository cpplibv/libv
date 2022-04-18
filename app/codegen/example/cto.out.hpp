// Project: libv, File: app/codegen/example/cto.out.hpp
// Generated source code
// Generator version: v3.0.1
// Input file: app/codegen/example/cto.lua

#pragma once

//
#include <libv/math/vec.hpp>
#include <libv/serial/archive/binary_fwd.hpp>
#include <string>
#include <vector>


namespace space {

// -------------------------------------------------------------------------------------------------

struct CTO_ChatMessage {
	std::string user_name;
	std::string message;

public:
	explicit inline CTO_ChatMessage(std::string user_name, std::string message) :
			user_name(std::move(user_name)), message(std::move(message)) { }
	void encode(libv::archive::BinaryInput& ar);
	void decode(libv::archive::BinaryOutput& ar);
};

struct CTO_LobbyStatus {
	struct Entry {
		struct EntryInner {
			std::string user_name = "Name";
			float ping = 42;

		public:
			explicit inline EntryInner(std::string user_name, float ping) :
					user_name(std::move(user_name)), ping(ping) { }
			void encode(libv::archive::BinaryInput& ar);
			void decode(libv::archive::BinaryOutput& ar);
		};

		EntryInner inner;
		std::string user_name;
		float ping;

	public:
		explicit inline Entry(EntryInner inner, std::string user_name, float ping) :
				inner(std::move(inner)), user_name(std::move(user_name)), ping(ping) { }
		void encode(libv::archive::BinaryInput& ar);
		void decode(libv::archive::BinaryOutput& ar);
	};

	std::vector<Entry> pings;

public:
	explicit inline CTO_LobbyStatus(std::vector<Entry> pings) :
			pings(std::move(pings)) { }
	void encode(libv::archive::BinaryInput& ar);
	void decode(libv::archive::BinaryOutput& ar);
};

struct CTO_FleetMove {
	libv::vec3f target_position;

public:
	explicit inline CTO_FleetMove(libv::vec3f target_position) :
			target_position(target_position) { }
	void encode(libv::archive::BinaryInput& ar);
	void decode(libv::archive::BinaryOutput& ar);
};

struct CTO_FleetMove2 {
	libv::vec3f target_position;
	int x = 32;

public:
	explicit inline CTO_FleetMove2(libv::vec3f target_position, int x) :
			target_position(target_position), x(x) { }
	void encode(libv::archive::BinaryInput& ar);
	void decode(libv::archive::BinaryOutput& ar);
};

// -------------------------------------------------------------------------------------------------

} // namespace space
