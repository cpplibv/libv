// Project: libv.update, File: src/libv/update/common/res_protocol.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/arg/arg.hpp>
//#include <libv/net/address.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/serial/types/std_vector.hpp>
//#include <libv/state/state.hpp>
// std
//#include <iostream>
#include <string>
#include <vector>
// pro
#include <libv/update/version_number.hpp>
#include <libv/update/update_signature.hpp>



namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct res_msg {
	struct RequestResource {
		std::string resource_namespace;
		std::string resource_id;
		uint64_t offset;
		uint64_t amount;

		template <typename Archive> inline void serialize(Archive& ar) {
			ar & LIBV_NVP(resource_namespace);
			ar & LIBV_NVP(resource_id);
			ar & LIBV_NVP(offset);
			ar & LIBV_NVP(amount);
		}
	};

	struct RequestCancel {
		template <typename Archive> inline void serialize(Archive& ar) { (void) ar; }
	};

	struct ResponseBusy {
		uint64_t busy_time;
		// std::chrono::steady_clock::duration busy_time;

		template <typename Archive> inline void serialize(Archive& ar) {
			ar & LIBV_NVP(busy_time);
		}
	};

	struct ResponseResourceNotFound {
		template <typename Archive> inline void serialize(Archive& ar) { (void) ar; }
	};

	struct ResponseResourceInvalid {
		template <typename Archive> inline void serialize(Archive& ar) { (void) ar; }
	};

	struct ResponseResourceDescription {
		uint64_t size;
		template <typename Archive> inline void serialize(Archive& ar) {
			ar & LIBV_NVP(size);
		}
	};

	struct ResponseResourceData {
		uint64_t offset;
		std::vector<std::byte> data;

		template <typename Archive> inline void serialize(Archive& ar) {
			ar & LIBV_NVP(offset);
			ar & LIBV_NVP(data);
		}
	};

	struct ResponseResourceDone {
		template <typename Archive> inline void serialize(Archive& ar) { (void) ar; }
	};

	// ---------------------------------------------------------------------------------------------

	template <typename Codec>
	static constexpr inline void message_types(Codec& codec) {
		// 1) Client -> Server
		codec.template type_client<10, RequestResource>();
		codec.template type_client<11, RequestCancel>();

		// 2) Server -> Client
		codec.template type_server<20, ResponseBusy>();
		codec.template type_server<21, ResponseResourceNotFound>();
		codec.template type_server<22, ResponseResourceInvalid>();
		codec.template type_server<23, ResponseResourceDescription>();
		codec.template type_server<24, ResponseResourceData>();
		codec.template type_server<25, ResponseResourceDone>();
	}
};

//using ev_RequestResourceSelect = libv::state::ev<const ResourceMessages::RequestResourceSelect&>;
//using ev_RequestResourceDescription = libv::state::ev<const ResourceMessages::RequestResourceDescription&>;
//using ev_RequestResourceData = libv::state::ev<const ResourceMessages::RequestResourceData&>;
//using ev_RequestCancel = libv::state::ev<const ResourceMessages::RequestCancel&>;
//using ev_RequestEnd = libv::state::ev<const ResourceMessages::RequestEnd&>;
//
//using ev_ResponseBusy = libv::state::ev<const ResourceMessages::ResponseBusy&>;
////using ev_ResponseResourceInvalid = libv::state::ev<const ResourceMessages::ResponseResourceInvalid&>;
//using ev_ResponseResourceNotFound = libv::state::ev<const ResourceMessages::ResponseResourceNotFound&>;
//using ev_ResponseResourceDescription = libv::state::ev<const ResourceMessages::ResponseResourceDescription&>;
//using ev_ResponseResourceChunk = libv::state::ev<const ResourceMessages::ResponseResourceChunk&>;
//using ev_ResponseResourceEnd = libv::state::ev<const ResourceMessages::ResponseResourceEnd&>;
//
//using ev_NetIOError = libv::state::ev<net_io_error>;

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
