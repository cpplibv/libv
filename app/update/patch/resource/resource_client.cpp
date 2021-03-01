// Project: libv, File: app/update/patch/resource/resource_client.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <update/patch/resource/resource_client.hpp>
//// libv
////#include <libv/process/lock_file.hpp>
////#include <libv/utility/read_file.hpp>
////#include <libv/utility/write_file.hpp>
//// std
//#include <filesystem>
//#include <memory>
//#include <string>
//#include <fstream>
//// pro
////#include <update/patch/patch_applier.hpp>
////#include <update/patch/patch.hpp>
//
//
//
//
//
//#include <libv/serial/serialization.hpp>
//#include <libv/net/mtcp/acceptor.hpp>
//#include <libv/net/mtcp/connection.hpp>
//#include <libv/net/mtcp/endpoint.hpp>
//#include <libv/net/address.hpp>
//#include <libv/utility/storage_size.hpp>
//
//#include <libv/state/state.hpp>
//
////#include <vector>
//// pro
////#include <update/patch/patch.hpp>
//
//
//namespace libv {
//namespace update {
//
//// -------------------------------------------------------------------------------------------------
//
////struct UpdateClient {
////	version_number version_current;
////	UpdaterSettings settings;
////
////	std::filesystem::path filepath_root_dir;
////	std::filesystem::path filepath_executable;
////
////	std::vector<libv::net::Address> server_addresses;
////
////public:
////	void init(
////			std::filesystem::path root_dir,
////			std::filesystem::path executable,
////			std::vector<libv::net::Address> servers,
////			version_number current
////	) {
////		filepath_root_dir = std::move(root_dir);
////		filepath_executable = std::move(executable);
////		server_addresses = std::move(servers);
////		version_current = current;
////
////		std::random_device rng; // Using high quality random order
////		std::ranges::shuffle(server_addresses, rng);
////	}
////
////	void check_for_update() {
////		for (size_t i = 0; i < 3; i++) {
////			for (const auto& server_address : server_addresses) {
////				libv::net::mtcp::ConnectionAsyncCB connection;
////
//////				connection.handle_connect()
//////				connection.handle_disconnect()
//////				connection.handle_error()
//////				connection.handle_send()
//////				connection.handle_receive()
////
////				connection.connect(server_address);
//////				connection.connect(server_address, std::chrono::seconds{10});
////
////				connection.send(codec.encode(msg_ReportVersion{current_version}));
////				connection.receive(1);
////
////				connection_ended.wait();
////
////				handler = libv::overload(
////						[](const msg_VersionNotSupported&) {
////
////						},
////						[](const msg_VersionUpToDate&) {
////
////						},
////						[](const msg_UpdateInfo& info) {
////
////						}
////				);
////
//////				on_msg_VersionNotSupported() {
//////
//////				}
//////				on_msg_VersionUpToDate() {
//////
//////				}
//////				on_msg_UpdateInfo() {
//////
//////				}
////			}
////		}
////	}
////
////	void update_to_latest() {
////		connection.send(codec.encode(msg_RequestPatch{...}));
////
////		on_msg_PatchNotSupported() {
////
////		}
////		on_msg_PatchChunk() {
////
////		}
////	}
////};
//
//// -------------------------------------------------------------------------------------------------
//
////struct msg {
////
////	struct ReportVersion {
////		version_number version;
////
////	public:
////		template <class Archive>
////		inline void serialize(Archive& ar) {
////			ar & LIBV_NVP(version);
////		}
////	};
////
////	/// If the client is reporting or requesting an version that is not supported or invalid
////	struct VersionNotSupported {
////	};
////
////	/// If the client is reporting the latest version
////	struct VersionUpToDate {
////	};
////
////	/// If the client is reporting an outdated version
////	struct UpdateInfo {
////		version_number version_latest;
////		version_number version_next;
////		uint64_t patch_chunk_num;
////		uint64_t patch_signature;
////		//	std::vector<Address> content_servers;
////
////	public:
////		template <class Archive>
////		inline void serialize(Archive& ar) {
////			ar & LIBV_NVP(version_latest);
////			ar & LIBV_NVP(version_next);
////			ar & LIBV_NVP(patch_chunk_num);
////			ar & LIBV_NVP(patch_signature);
////		}
////	};
////
////	struct RequestPatch {
////		version_number version_source;
//////		version_number version_target;
////		uint64_t offset;
////		uint64_t amount;
////
////	public:
////		template <class Archive>
////		inline void serialize(Archive& ar) {
////			ar & LIBV_NVP(version_source);
//////			ar & LIBV_NVP(version_target);
////			ar & LIBV_NVP(offset);
////			ar & LIBV_NVP(amount);
////		}
////	};
////
////	//struct RequestMissingOrCorruptFile {
////	//  version_number version;
////	//	std::vector<std::string> files;
////	//
////	//public:
////	//	template <class Archive>
////	//	inline void serialize(Archive& ar) {
////	//		ar & LIBV_NVP(version);
////	//		ar & LIBV_NVP(files);
////	//	}
////	//};
////
////	/// If the client is requesting an patch that is not supported or invalid
////	struct PatchNotSupported {
////	};
////
////	struct PatchChunk {
////		uint64_t index;
////		std::vector<std::byte> data;
////
////	public:
////		template <class Archive>
////		inline void serialize(Archive& ar) {
////			ar & LIBV_NVP(index);
////			ar & LIBV_NVP(data);
////		}
////	};
////};
////
////struct net_io_error {};
//
//// =================================================================================================
//
////  main_state(
////		on_entry<_>                                         = wait_info,
////		on<net_io_error>                                    = terminate
////  )
////	wait_info(
////		on_entry<_>                 / send_report_version,
////		on<msg_VersionNotSupported> / version_not_supported = terminate,
////		on<msg_VersionUpToDate>     / version_up_to_date    = terminate,
////		on<msg_UpdateInfo>          / receive_version_info  = wait_chunk,
////	)
////	wait_chunk(
////		on_entry<_>                 / request_next_chunk,
////		on<msg_PatchNotSupported>   / patch_not_supported   = terminate,
////		on<msg_PatchChunk>          / save_and_request_next_chunk,
////	)
//
//// -------------------------------------------------------------------------------------------------
//// State Machine
//
////using ev_ReportVersion = libv::state::ev<msg::ReportVersion>;
////using ev_VersionNotSupported = libv::state::ev<msg::VersionNotSupported>;
////using ev_VersionUpToDate = libv::state::ev<msg::VersionUpToDate>;
////using ev_UpdateInfo = libv::state::ev<msg::UpdateInfo>;
////using ev_RequestPatch = libv::state::ev<msg::RequestPatch>;
//////using ev_RequestMissingOrCorruptFile = libv::state::ev<msg::RequestMissingOrCorruptFile>;
////using ev_PatchNotSupported = libv::state::ev<msg::PatchNotSupported>;
////using ev_PatchChunk = libv::state::ev<msg::PatchChunk>;
////using ev_NetIOError = libv::state::ev<net_io_error>;
//
//// ---
//
//struct UpdateClientSM : libv::state::machine<UpdateClientSM, struct Connected> {
//};
//
//struct Connected : libv::state::state<Connected, UpdateClientSM, struct WaitInfo> {
//	using reactions = react_list<ev_NetIOError>;
//
//	int connection = 0;
//
//	result react(const ev_NetIOError&) {
//		// <<< LOG and handle error as operation failure
//		return terminate();
//	}
//};
//
//struct WaitInfo : libv::state::state<WaitInfo, Connected> {
//	using reactions = react_list<ev_UpdateInfo, ev_VersionNotSupported, ev_VersionUpToDate>;
//
//	WaitInfo() {
//		context<Connected>().connection++;
//	}
//	result react(const ev_UpdateInfo& event) {
//		event.value.version_latest;
//		event.value.version_next;
//		event.value.patch_chunk_num;
//		event.value.patch_signature;
//	}
//	result react(const ev_VersionNotSupported&) {
//		// <<< LOG and handle error as fatal
//		return terminate();
//	}
//	result react(const ev_VersionUpToDate&) {
//		// <<< LOG and record success
//		return terminate();
//	}
//};
//
//struct DownloadingPatch : libv::state::state<DownloadingPatch, Connected> {
//	using reactions = react_list<ev_PatchChunk, ev_PatchNotSupported>;
//
//	std::ofstream file;
//
//	DownloadingPatch() {
//		context<Connected>().connection++;
////		msg::RequestPatch{version_source, version_target, first, last};
//	}
//	result react(const ev_PatchChunk& event) {
//
//		if (file.tellp() != event.value.index * patch_chunk_size)
//		file.write(reinterpret_cast<const char*>(event.value.data.data()), event.value.data.size());
//
//		// <<< it_was_the_last_chunk
//		if (not it_was_the_last_chunk)
//			return discard_event();
//		else
//			// <<< LOG and record success
//			return terminate();
//	}
//	result react(const ev_PatchNotSupported&) {
//		// <<< LOG and handle error as fatal, cleanup patch files?
//		return terminate();
//	}
//};
//
//// ---
//
//void foo() {
//	UpdateClientSM sm;
//
//	sm.initiate();
//	sm.process_event(ev_NetIOError());
//
//}
//
//// =================================================================================================
//
////struct PeerSession : GeneralServer<PeerSession> {
//struct PeerSession : GeneralSession {
//
//};
//
//struct BetterFileDownloaderServer : GeneralServer<PeerSession> {
//	int file_maps;
//
////	Acceptor acceptor;
////	ConnectionSet connections;
//
//	struct Peer {
//		void receive(std::span<std::byte> message);
//		//void send(std::span<std::byte> message);
//	};
//
////	std::shared_ptr<Peer> create_session() {
////
////	}
//
//	void connected(PeerSession& session) {
//
//	}
//
//	void disconnected(PeerSession& session) {
//
//	}
//};
//
//// -------------------------------------------------------------------------------------------------
//
//} // namespace update
//} // namespace libv
