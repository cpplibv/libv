// Project: libv.update, File: src/libv/update/common/protocol.hpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/net/address.hpp>
// std
#include <iostream>
// pro
//#include <update/common/client.hpp>
#include <update/common/config.hpp>
#include <update/common/log.hpp>
#include <libv/update.hpp>


// -------------------------------------------------------------------------------------------------
// =================================================================================================
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

#include <libv/serial/serial.hpp>

struct msg_ReportVersion {
	version_number version;

public:
	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(version);
	}
};

/// If the client is reporting or requesting an version that is not supported or invalid
struct msg_VersionNotSupported {
};

/// If the client is reporting the latest version
struct msg_VersionUpToDate {
};

/// If the client is reporting an outdated version
struct msg_UpdateInfo {
	version_number latest;
	patch_number next_patch;
	uint64_t num_chunk;
	uint64_t signature;
//	std::vector<Address> content_servers;

public:
	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(latest);
		ar & LIBV_NVP(next_patch);
		ar & LIBV_NVP(num_chunk);
		ar & LIBV_NVP(signature);
	}
};

struct msg_RequestPatch {
	patch_number patch;
	uint64_t first_chunk;
	uint64_t last_chunk;

public:
	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(patch);
		ar & LIBV_NVP(first_chunk);
		ar & LIBV_NVP(last_chunk);
	}
};

//struct msg_RequestMissingOrCorruptFile {
//  version_number version;
//	std::vector<std::string> files;
//
//public:
//	template <class Archive>
//	inline void serialize(Archive& ar) {
//		ar & LIBV_NVP(version);
//		ar & LIBV_NVP(files);
//	}
//};

/// If the client is requesting an patch that is not supported or invalid
struct msg_PatchNotSupported {
};

struct msg_PatchChunk {
	uint64_t chunk_id;
	std::vector<std::byte> data;

public:
	template <class Archive>
	inline void serialize(Archive& ar) {
		ar & LIBV_NVP(chunk_id);
		ar & LIBV_NVP(data);
	}
};

// -------------------------------------------------------------------------------------------------

// 1) Client -> Server
//  msg_ReportVersion
//
// 2) Server -> Client
//  msg_VersionNotSupported
//  msg_VersionUpToDate
//  msg_UpdateInfo
//
// 3) Client -> Server
//  msg_RequestPatch
//
// 4) Server -> Client
//  msg_PatchNotSupported
//  msg_PatchChunk

// -------------------------------------------------------------------------------------------------
// =================================================================================================
// =================================================================================================

#include <libv/serial/archive/binary.hpp>


template <typename T>
auto protocol_foo(T& handler) {
	Codec<T, libv::archive::Binary> codec{handler};

	codec.add<0, msg_ReportVersion>();

	codec.add<1, msg_VersionNotSupported>();
	codec.add<2, msg_VersionUpToDate>();
	codec.add<3, msg_UpdateInfo>();

	codec.add<4, msg_RequestPatch>();

	codec.add<5, msg_PatchNotSupported>();
	codec.add<6, msg_PatchChunk>();

	return codec;
}

void client_foo() {
	Client client;

	const auto codec = protocol_foo(client);

	const auto current_version = version_number{1001};

	connection.send(codec.encode(msg_ReportVersion{current_version}));
}

void server_foo() {
	Server server;

	const auto codec = protocol_foo(server);

	const auto client_version = codec.decode(connection.receive());
}

// -------------------------------------------------------------------------------------------------

struct Client {
//	Connection conn;

	// ---

//	void receive(const msg_ReportVersion& msg);

	void receive(const msg_VersionNotSupported& msg);
	void receive(const msg_VersionUpToDate& msg);
	void receive(const msg_UpdateInfo& msg);

//	void receive(const msg_RequestPatch& msg);

	void receive(const msg_PatchNotSupported& msg);
	void receive(const msg_PatchChunk& msg);
};

// ---

struct Session {
//	Connection conn;

	// ---

	void receive(const msg_ReportVersion& msg);
	void receive(const msg_RequestPatch& msg);
};

// ---

struct Server {
//	void accept_async(Endpoint endpoint, int number = 0, int backlog = 4);
//	void accept_async(uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint16_t port, int number = 0, int backlog = 4);
};
