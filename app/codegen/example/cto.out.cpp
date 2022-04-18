// Project: libv, File: app/codegen/example/cto.out.cpp
// Generated source code
// Generator version: v3.0.1
// Input file: app/codegen/example/cto.lua

//
#include <codegen/example/cto.out.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/serial.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

void CTO_ChatMessage::encode(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("message", message));
}

void CTO_ChatMessage::decode(libv::archive::BinaryOutput& ar) {
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("message", message));
}

// -------------------------------------------------------------------------------------------------

void CTO_LobbyStatus::Entry::EntryInner::encode(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("ping", ping));
}

void CTO_LobbyStatus::Entry::EntryInner::decode(libv::archive::BinaryOutput& ar) {
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("ping", ping));
}

// -------------------------------------------------------------------------------------------------

void CTO_LobbyStatus::Entry::encode(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("inner", inner));
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("ping", ping));
}

void CTO_LobbyStatus::Entry::decode(libv::archive::BinaryOutput& ar) {
	ar(LIBV_NVP_FORCED("inner", inner));
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("ping", ping));
}

// -------------------------------------------------------------------------------------------------

void CTO_LobbyStatus::encode(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("pings", pings));
}

void CTO_LobbyStatus::decode(libv::archive::BinaryOutput& ar) {
	ar(LIBV_NVP_FORCED("pings", pings));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetMove::encode(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

void CTO_FleetMove::decode(libv::archive::BinaryOutput& ar) {
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetMove2::encode(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("target_position", target_position));
	ar(LIBV_NVP_FORCED("x", x));
}

void CTO_FleetMove2::decode(libv::archive::BinaryOutput& ar) {
	ar(LIBV_NVP_FORCED("target_position", target_position));
	ar(LIBV_NVP_FORCED("x", x));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
