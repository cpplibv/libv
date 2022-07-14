//
// Generated source code
// Generator version: v4.0.0
// Input file: app/space/message/cto.in.lua

//
#include <libv/math/vec_serial.hpp>
#include <libv/serial/archive/binary.hpp>
#include <libv/serial/serial.hpp>
#include <libv/serial/types/std_chrono.hpp>
#include <libv/serial/types/std_string.hpp>
#include <libv/serial/types/std_vector.hpp>
#include <space/message/cto.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

void CTO_Introduction::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("version", version));
}

void CTO_Introduction::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("version", version));
}

// -------------------------------------------------------------------------------------------------

void CTO_ClientJoined::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("joined_at", joined_at));
	ar(LIBV_NVP_FORCED("version", version));
}

void CTO_ClientJoined::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("user_name", user_name));
	ar(LIBV_NVP_FORCED("joined_at", joined_at));
	ar(LIBV_NVP_FORCED("version", version));
}

// -------------------------------------------------------------------------------------------------

void CTO_ClientLeave::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("userID", userID));
}

void CTO_ClientLeave::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("userID", userID));
}

// -------------------------------------------------------------------------------------------------

void CTO_ChatMessage::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("sent_at", sent_at));
	ar(LIBV_NVP_FORCED("message", message));
}

void CTO_ChatMessage::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("sent_at", sent_at));
	ar(LIBV_NVP_FORCED("message", message));
}

// -------------------------------------------------------------------------------------------------

void CTO_LobbyStatus::Entry::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("ping", ping));
}

void CTO_LobbyStatus::Entry::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("ping", ping));
}

// -------------------------------------------------------------------------------------------------

void CTO_LobbyStatus::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("pings", pings));
}

void CTO_LobbyStatus::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("pings", pings));
}

// -------------------------------------------------------------------------------------------------

void CTO_LobbyClose::save(libv::archive::BinaryOutput& ar) const {
	(void) ar;
}

void CTO_LobbyClose::load(libv::archive::BinaryInput& ar) {
	(void) ar;
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetSpawn::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("position", position));
}

void CTO_FleetSpawn::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("position", position));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetSelect::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("fleetID", fleetID));
}

void CTO_FleetSelect::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("fleetID", fleetID));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetSelectAdd::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("fleetID", fleetID));
}

void CTO_FleetSelectAdd::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("fleetID", fleetID));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetClearSelection::save(libv::archive::BinaryOutput& ar) const {
	(void) ar;
}

void CTO_FleetClearSelection::load(libv::archive::BinaryInput& ar) {
	(void) ar;
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetSelectBox::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("fleetIDs", fleetIDs));
}

void CTO_FleetSelectBox::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("fleetIDs", fleetIDs));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetMove::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

void CTO_FleetMove::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetMoveQueue::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

void CTO_FleetMoveQueue::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetAttackFleet::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("targetFleetID", targetFleetID));
}

void CTO_FleetAttackFleet::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("targetFleetID", targetFleetID));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetAttackFleetQueue::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("targetFleetID", targetFleetID));
}

void CTO_FleetAttackFleetQueue::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("targetFleetID", targetFleetID));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetAttackPlanet::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("targetPlanetID", targetPlanetID));
}

void CTO_FleetAttackPlanet::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("targetPlanetID", targetPlanetID));
}

// -------------------------------------------------------------------------------------------------

void CTO_FleetAttackPlanetQueue::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("targetPlanetID", targetPlanetID));
}

void CTO_FleetAttackPlanetQueue::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("targetPlanetID", targetPlanetID));
}

// -------------------------------------------------------------------------------------------------

void CTO_ClearFleets::save(libv::archive::BinaryOutput& ar) const {
	(void) ar;
}

void CTO_ClearFleets::load(libv::archive::BinaryInput& ar) {
	(void) ar;
}

// -------------------------------------------------------------------------------------------------

void CTO_Shuffle::save(libv::archive::BinaryOutput& ar) const {
	(void) ar;
}

void CTO_Shuffle::load(libv::archive::BinaryInput& ar) {
	(void) ar;
}

// -------------------------------------------------------------------------------------------------

void CTO_PlanetSpawn::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("position", position));
}

void CTO_PlanetSpawn::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("position", position));
}

// -------------------------------------------------------------------------------------------------

void CTO_ClearPlanets::save(libv::archive::BinaryOutput& ar) const {
	(void) ar;
}

void CTO_ClearPlanets::load(libv::archive::BinaryInput& ar) {
	(void) ar;
}

// -------------------------------------------------------------------------------------------------

void CTO_TrackView::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("eye", eye));
	ar(LIBV_NVP_FORCED("target", target));
	ar(LIBV_NVP_FORCED("mouse_direction", mouse_direction));
}

void CTO_TrackView::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("eye", eye));
	ar(LIBV_NVP_FORCED("target", target));
	ar(LIBV_NVP_FORCED("mouse_direction", mouse_direction));
}

// -------------------------------------------------------------------------------------------------

void CTO_CameraWarpTo::save(libv::archive::BinaryOutput& ar) const {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

void CTO_CameraWarpTo::load(libv::archive::BinaryInput& ar) {
	ar(LIBV_NVP_FORCED("userID", userID));
	ar(LIBV_NVP_FORCED("target_position", target_position));
}

// -------------------------------------------------------------------------------------------------

} // namespace space
