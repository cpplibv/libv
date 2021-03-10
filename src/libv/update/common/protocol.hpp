// Project: libv.update, File: src/libv/update/common/protocol.hpp, Author: Császár Mátyás [Vader]

#pragma once

// libv
//#include <libv/arg/arg.hpp>
#include <libv/net/address.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/serial/types/std_vector.hpp>
// std
#include <iostream>
// pro
#include <libv/update/version_number.hpp>
#include <libv/update/update_signature.hpp>


namespace libv {
namespace update {

// -------------------------------------------------------------------------------------------------

struct msg {
	struct ReportVersion {
		std::string program;
		std::string variant;
		version_number version;

	public:
		template <class Archive>
		inline void serialize(Archive& ar) {
			ar & LIBV_NVP(program);
			ar & LIBV_NVP(variant);
			ar & LIBV_NVP(version);
		}
	};

	/// If the client is reporting or requesting an version that is not supported or invalid
	struct VersionNotSupported {
	};

	/// If the client is reporting the latest version
	struct VersionUpToDate {
	};

	/// If the client is reporting an outdated version
	struct UpdateRoute {
		struct UpdateInfoEntry {
			version_number version_source;
			version_number version_target;
			uint64_t size;
			update_signature signature;

			template <class Archive> inline void serialize(Archive& ar) {
				ar & LIBV_NVP(version_source);
				ar & LIBV_NVP(version_target);
				ar & LIBV_NVP(size);
			    ar & LIBV_NVP(signature);
			}
		};

	public:
		std::string program; // Sending back the program and variant names are not necessary, but could be used to remap
		std::string variant; // Sending back the program and variant names are not necessary, but could be used to remap
		std::vector<UpdateInfoEntry> updates;
//		std::vector<libv::net::Address> resource_servers;

	public:
		template <class Archive> inline void serialize(Archive& ar) {
			ar & LIBV_NVP(program);
			ar & LIBV_NVP(variant);
			ar & LIBV_NVP(updates);
//			ar & LIBV_NVP(resource_servers);
		}
	};

	// ---------------------------------------------------------------------------------------------

	template <typename Codec>
	static constexpr inline void message_types(Codec& codec) {
		// 1) Client -> Server
		codec.template type_client<10, ReportVersion>();

		// 2) Server -> Client
		codec.template type_server<20, VersionNotSupported>();
		codec.template type_server<21, VersionUpToDate>();
		codec.template type_server<22, UpdateRoute>();
	}
};

// -------------------------------------------------------------------------------------------------

} // namespace update
} // namespace libv
