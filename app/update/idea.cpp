// Project: libv, File: app/update/idea.cpp, Author: Császár Mátyás [Vader]

// libv
#include <libv/arg/arg.hpp>
#include <libv/net/address.hpp>
// std
#include <iostream>
// pro
//#include <update/common/client.hpp>
#include <update/common/config.hpp>
#include <update/common/log.hpp>
#include <update/patch.hpp>


// -------------------------------------------------------------------------------------------------
// =================================================================================================
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

#include <libv/serialization/serialization.hpp>

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

#include <span>
#include <typeindex>
#include <vector>


template <typename Handler, typename Archive>
struct Codec {
	using MessageID = uint8_t;
	using TypeID = std::type_index;
	using Message = std::vector<std::byte>;
	using Message_view = std::span<std::byte>;

	using DecodeFN = void(*)(Handler&, Message_view);
	using EncodeFN = Message(*)(const void* object);

	using IArchive = typename Archive::input;
	using OArchive = typename Archive::output;

private:
	struct Entry {
		MessageID id;
		TypeID type;
		EncodeFN encode_fn;
		DecodeFN decode_fn;
	};

private:
	Handler& handler;
	std::vector<Entry> entries;

public:
	explicit Codec(Handler& handler) : handler(handler) {}

public:
	template <MessageID ID, typename T>
	void add() {
		auto type = std::type_index(typeid(T));

		auto encode_fn = [](const void* object) {
			Message message;
			IArchive oar(message);
			oar << LIBV_NVP_NAMED("message", *static_cast<const T*>(object));

			return message;
		};

		auto decode_fn = [](Handler& handler, Message_view message) {
			T object;
			IArchive iar(message);
			iar >> LIBV_NVP_NAMED("message", object);

			handler.receive(std::move(object));
		};

		entries.emplace_back(ID, type, std::move(encode_fn), std::move(decode_fn));
	}

public:

	void decode(Message message) const {
		it = find();

		if (!it)
//			assert
			return;

		it->decode_fn();
	}

	Message aux_encode(std::type_index type, const void* object) const {
		Message message;
		it = find(type);

		if (!it)
//			assert
			return;

		return it->encode_fn();
	}

	template <typename T>
	[[nodiscard]] inline Message encode(const T& object) const {
		const auto type = std::type_index(typeid(T));
		const auto object_ptr = static_cast<const void*>(&object);

		return aux_encode(type, object_ptr);
	}
};


// -------------------------------------------------------------------------------------------------

#include <libv/serialization/archive/binary.hpp>


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

struct Message;

// ---

struct Client {
	Connection conn;

	void on_connect();
	void on_send(Message&&);
	void on_receive(Message&&);
	void on_disconnect();

	void send_async(Message&&);
	void disconnect_async();

	// ---

	void receive(const msg_ReportVersion& msg);

	void receive(const msg_VersionNotSupported& msg);
	void receive(const msg_VersionUpToDate& msg);
	void receive(const msg_UpdateInfo& msg);

	void receive(const msg_RequestPatch& msg);

	void receive(const msg_PatchNotSupported& msg);
	void receive(const msg_PatchChunk& msg);
};

// ---

struct Session {
	Connection conn;

	void on_connect();
	void on_send(Message&&);
	void on_receive(Message&&);
	void on_disconnect();

	void send_async(Message&&);
	void disconnect_async();
};

// ---

struct Server {
	void on_connect(Session&);
	void on_disconnect(Session&);

	void accept_async(Endpoint endpoint, int number = 0, int backlog = 4);
	void accept_async(uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint16_t port, int number = 0, int backlog = 4);
};



// =================================================================================================
// =================================================================================================
// =================================================================================================
// -------------------------------------------------------------------------------------------------

int main(int argc, const char** argv) {
	auto args = libv::arg::Parser("Updater server", "LIBV Updater server daemon");

//	const auto config_file = args.require<std::string>
//			("-c", "--config")
//			("config_file", "File path of the config file");
//
//	const auto manifest_file = args.require<std::string>
//			("-m", "--manifest")
//			("manifest_file", "File path of the manifest file");

	const auto address = args.require<std::string>
			("-a", "--address")
			("address", "Server IP address to connect to")
			= app::default_address;

	const auto port = args.require<uint16_t>
			("-p", "--port")
			("port", "Server TCP port to connect to")
			= app::default_port;

	const auto num_net_thread = args.require<uint16_t>
			("-t", "--net_thread")
			("net_thread", "Number of network IO threads")
			= app::default_net_thread;

	args.require_no_unused();

	if (!args.parse(argc, argv)) {
		std::cerr << args.error_message(100) << args.usage(100);
		return EXIT_FAILURE;
	}

	libv::logger_stream.setFormat("{severity} {thread_id} {module}: {message}, {file}:{line}\n");
	std::cout << libv::logger_stream;

	app::log_app.info("{}", args.report(100));

	app::UpdateClient client;
	client.update_from(libv::net::Address(address.value(), port.value()));

	for (std::string line; std::getline(std::cin, line);) {
		if (line == "quit")
			break;

		client.send(line);
	}

	return EXIT_SUCCESS;
}

