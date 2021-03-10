// Project: libv.update, File: src/libv/update/client/update_client.cpp, Author: Császár Mátyás [Vader]

// hpp
#include <libv/update/client/update_client.hpp>
// libv
#include <libv/process/lock_file.hpp>
#include <libv/utility/enum.hpp>
#include <libv/utility/read_file.hpp>
#include <libv/utility/uniform_distribution.hpp>
#include <libv/utility/write_file.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
// pro
#include <libv/update/client/network_client.hpp>
#include <libv/update/client/patch_applier.hpp>
#include <libv/update/log.hpp>
#include <libv/update/patch.hpp>





//#include <libv/process/lock_file.hpp>
//#include <libv/utility/read_file.hpp>
//#include <libv/utility/write_file.hpp>
// std
#include <filesystem>
#include <memory>
#include <string>
#include <fstream>
// pro
//#include <libv/update/patch_applier.hpp>
//#include <libv/update/patch.hpp>
#include <libv/serial/serial.hpp>
//#include <libv/net/mtcp/acceptor.hpp>
#include <libv/net/mtcp/connection_he.hpp>
#include <libv/net/mtcp/endpoint.hpp>
#include <libv/net/address.hpp>
#include <libv/utility/storage_size.hpp>

#include <libv/state/state.hpp>
#include <libv/update/common/protocol.hpp>



//#include <vector>
// pro
//#include <libv/update/patch.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct ImplUpdateClient {
public:
	UpdateClient::Settings settings;
	msg::UpdateRoute update_info;

public:
	libv::net::IOContext io_context{settings.num_thread_net};

public:
	update_check_result check_for_update() {
		std::random_device rd; // Expensive random number engine (but only a few uses, no need to spin up an mt)

		update_check_result check_result = update_check_result::communication_error;
		log_update.trace("Checking update for program: [{}] variant: [{}] version: [{}]", settings.program_name, settings.program_variant, libv::to_value(settings.current_version));

		for (size_t i = 0;; i++) {
			std::ranges::shuffle(settings.update_servers, rd);

			for (const auto& server_address : settings.update_servers) {
				log_update.trace("Connecting to update server server: {} (Attempt {})", server_address, i + 1);

				UpdateNetworkClient connection(io_context, server_address);
				check_result = connection.check_version(settings.program_name, settings.program_variant, settings.current_version);

				log_update.trace("check_result: {}", libv::to_value(check_result)); // <<< dumping return

				if (check_result == update_check_result::version_outdated)
					update_info = connection.update_info();
				if (check_result != update_check_result::communication_error)
					break;
			}

			if (check_result != update_check_result::communication_error)
				break; // Break if we got back a valid response

			if (i >= 2)
				break; // Break after 3 attempt (on every server)

			const auto wait_min = std::chrono::milliseconds(static_cast<uint64_t>(1000 * std::pow(3, i)));     // 1, 3, 9 sec
			const auto wait_max = std::chrono::milliseconds(static_cast<uint64_t>(1000 * std::pow(3, i + 1))); // 3, 9, 27 sec
			const auto wait = libv::make_uniform_distribution_inclusive(wait_min, wait_max)(rd);
			log_update.trace("Waiting {} ms before retrying ({})...", wait.count(), i + 1);
			std::this_thread::sleep_for(wait);
		}

		return check_result;
	}

	void update() {
		//void UpdateClient::update(
		//		std::filesystem::path root_dir,
		//		std::vector<std::filesystem::path> patches,
		//		version_number current,
		//		version_number target) {
		//
		//	version_current = current;
		//	version_target = target;
		//
		////	filepath_root_dir = root_dir;
		////	filepath_lock = root_dir / settings.filename_lock;
		////	filepath_journey = root_dir / settings.filename_lock;
		//}

		try {
			libv::process::lock_file_guard lock_file(settings.path_lock_file);

//		const auto journey_exists = std::filesystem::exists(filepath_journey);
//		if (journey_exists) {
//			const auto journey_content = libv::read_file_or_throw(filepath_journey);
//
//
//		}

//		std::error_code ec;
//
//		const auto journey_exists = std::filesystem::exists(filepath_journey, ec);
//		if (ec)
//			fail;
//
//		if (journey_exists) {
////			const auto journey_content = libv::read_file(filepath_journey, ec);
////			if (ec)
////				fail;
//
////			root_dir / id_of_journey_content
////			id_of_journey_content
//
//			const auto journey_exists = std::filesystem::exists(id_of_journey_content, ec);
//			if (ec || !journey_exists) {
//				// exists failure can just assume invalid content of journey file
//				libv::write_file(filepath_journey, id_for_patch, ec);
//				if (ec)
//					fail;
//			}
//
//			patch = Patch::load_from_file(id_for_patch_or_journey_content);
//
//			const auto continue_ = journey_exists and journey_is_good;
//
//			PatchApplier applier(root_dir, patch, journey_exists and journey_good);
//			while (applier.progress());
//			const auto success = applier.failures().empty();
//
//			if (!success)
//				fail;
//
//		} else {
//
//
//		}

		} catch (const std::filesystem::filesystem_error& e) {
			//e.code();

			if (e.path2().empty())
				log_update.error("{} {}", e.what(), e.path1());
			else
				log_update.error("{} {} {}", e.what(), e.path1(), e.path2());

		} catch (const std::exception& e) {
			log_update.error("{}", e.what());
		}
	}
};

// -------------------------------------------------------------------------------------------------

UpdateClient::UpdateClient(Settings settings) :
	self(std::make_unique<ImplUpdateClient>(std::move(settings))) {
}

UpdateClient::~UpdateClient() {
	// For the sake of forward declared unique_ptr
}

update_check_result UpdateClient::check_for_update() {
	return self->check_for_update();
}

void UpdateClient::update() {
	return self->update();
}

// -------------------------------------------------------------------------------------------------
// State Machine
//
//  main_state(
//		on_entry<_>                                         = wait_info,
//		on<net_io_error>                                    = terminate
//  )
//	wait_info(
//		on_entry<_>                 / send_report_version,
//		on<msg_VersionNotSupported> / version_not_supported = terminate,
//		on<msg_VersionUpToDate>     / version_up_to_date    = terminate,
//		on<msg_UpdateInfo>          / receive_version_info  = wait_chunk,
//	)
//	wait_chunk(
//		on_entry<_>                 / request_next_chunk,
//		on<msg_PatchNotSupported>   / patch_not_supported   = terminate,
//		on<msg_PatchChunk>          / save_and_request_next_chunk,
//	)

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
//
//	result react(const ev_UpdateInfo& event) {
//		event.value.version_latest;
//		event.value.version_next;
//		event.value.patch_chunk_num;
//		event.value.patch_signature;
//	}
//
//	result react(const ev_VersionNotSupported&) {
//		// <<< LOG and handle error as fatal
//		return terminate();
//	}
//
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
//
//	result react(const ev_PatchChunk& event) {
//
////		if (file.tellp() != event.value.index * patch_chunk_size)
////			file.write(reinterpret_cast<const char*>(event.value.data.data()), event.value.data.size());
////
////		// <<< it_was_the_last_chunk
////		if (not it_was_the_last_chunk)
////			return discard_event();
////		else
//		// <<< LOG and record success
//		return terminate();
//	}
//
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

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
