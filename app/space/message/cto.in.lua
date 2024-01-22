
--member:
--	identifier : string
--	type : string
--	init_value : string
--	use_move : bool
--
--clazz:
--	identifier : string
--	struct : bool
--	scope_class : string
--	members : table<member>

local generate_constructor = function(out, clazz)
	if #clazz.members == 0 then
		return -- No member
	end

	local args_list = ""
	local init_list = ""

	for _, member in ipairs(clazz.members) do
		if (args_list ~= "") then
			args_list = args_list .. ", "
			init_list = init_list .. ", "
		end

		args_list = args_list .. member.type .. " " .. member.identifier

		if member.use_move then
			init_list = init_list .. member.identifier .. "(std::move(" .. member.identifier .. "))"
		else
			init_list = init_list .. member.identifier .. "(" .. member.identifier .. ")"
		end
	end

	out("\n")
	out("\t" .. clazz.identifier .. "() = default;\n")
	--out("\texplicit inline " .. clazz.identifier .. "(" .. args_list .. ") :\n")
	--out("\t\t\t" .. init_list .. " {}\n")
	out("\texplicit inline " .. clazz.identifier .. "(" .. args_list .. ") : " .. init_list .. " {}\n")
end

local generate_encode_hpp = function(archive)
	return function(out, clazz)
		out("\tvoid save(" .. archive .. "& ar) const;\n")
	end
end

local generate_encode_cpp = function(archive)
	return function(out, clazz)
		out("\n")
		out("void " .. clazz.scope_class .. "::save(" .. archive .. "& ar) const {\n")
		if #clazz.members == 0 then
			out("\t(void) ar;\n")
		end
		for _, member in ipairs(clazz.members) do
			out("\tar(LIBV_NVP_FORCED(\"" .. member.identifier .. "\", " .. member.identifier .. "));\n")
		end
		out("}\n")
	end
end

local generate_decode_hpp = function(archive)
	return function(out, clazz)
		out("\tvoid load(" .. archive .. "& ar);\n")
	end
end

local generate_decode_cpp = function(archive)
	return function(out, clazz)
		out("\n")
		out("void " .. clazz.scope_class .. "::load(" .. archive .. "& ar) {\n")
		if #clazz.members == 0 then
			out("\t(void) ar;\n")
		end
		for _, member in ipairs(clazz.members) do
			out("\tar(LIBV_NVP_FORCED(\"" .. member.identifier .. "\", " .. member.identifier .. "));\n")
		end
		out("}\n")
	end
end

----------------------------------------------------------------------------------------------------

local cto_struct = function (name, id, description)
	--table.insert(description, derive_from("BaseCTO"))
	table.insert(description, member_sc("libv::serial::CodecMessageID", "id", tostring(id)))
	return struct(name, description)
end

-- =================================================================================================

include_hpp("libv/math/vec.hpp")
include_hpp("libv/serial/archive/binary_fwd.hpp")
include_hpp("libv/serial/codec_message_id.hpp")
include_hpp("space/sim/ids.hpp")
include_hpp("chrono")
include_hpp("cstdint")
include_hpp("string")
include_hpp("vector")

include_cpp("libv/math/vec_serial.hpp")
include_cpp("libv/serial/archive/binary.hpp")
include_cpp("libv/serial/serial.hpp")
include_cpp("libv/serial/types/std_string.hpp")
include_cpp("libv/serial/types/std_vector.hpp")
include_cpp("libv/serial/types/std_chrono.hpp")
include_cpp("space/message/cto.hpp")

enable_hpp(generate_constructor)
enable_hpp(generate_encode_hpp("libv::archive::BinaryOutput"))
enable_cpp(generate_encode_cpp("libv::archive::BinaryOutput"))
enable_hpp(generate_decode_hpp("libv::archive::BinaryInput"))
enable_cpp(generate_decode_cpp("libv::archive::BinaryInput"))
--enable_hpp(generate_process_hpp("space::Simulation"))
--enable_cpp(generate_process_cpp("space::Simulation"))

namespace("space")

--- CTO - Command Transfer Object
---
--- Main group of CTO messages:
--- 	Lobby: Lobby messages relate to the network lobby state replication to clients. All info comes from the host
---				Delay acceptable
--- 	State:
---				Delay not acceptable, always highest priority messages
--- 	Tracking: WIP: Camera position, Selections and other view related tracking information
---				Delay acceptable

-- =================================================================================================

--struct("BaseCTO", {
--	member_v("virtual function", "xxx"),
--})

----------------------------------------------------------------------------------------------------

cto_struct("CTO_Introduction", 0, {
	--member_v("UserID", "userID"),
	member_mv("std::string", "user_name"),
	member_v("uint64_t", "version", 0),
})

----------------------------------------------------------------------------------------------------

cto_struct("CTO_ClientJoined", 20, {
	member_v("UserID", "userID", "invalidUserID"),
	member_mv("std::string", "user_name"),
	member_v("std::chrono::system_clock::time_point", "joined_at"),
	member_v("uint64_t", "version", 0),
})

cto_struct("CTO_ClientLeave", 21, {
	member_v("UserID", "userID", "invalidUserID"),
})

--cto_struct("CTO_ClientKick", 22, {
--	member_v("UserID", "userID", "invalidUserID"),
--})

cto_struct("CTO_ChatMessage", 23, {
	member_v("UserID", "userID", "invalidUserID"),
	member_v("std::chrono::system_clock::time_point", "sent_at"),
	member_mv("std::string", "message"),
})

cto_struct("CTO_LobbyStatus", 24, {
	struct("Entry", {
		member_v("UserID", "userID", "invalidUserID"),
		member_v("float", "ping"),
		--member_v("float", "jitter"),
		--member_v("float", "packet_loss"),
	}),

	member_mv("std::vector<Entry>", "pings"),
})

cto_struct("CTO_LobbyClose", 25, {
})

----------------------------------------------------------------------------------------------------

cto_struct("CTO_FleetSpawn", 30, {
	--member_v("FactionID", "factionID", "invalidFactionID"),
	--member_v("FleetID", "fleetID", "invalidFleetID"),
	member_v("libv::vec3f", "position"),
})

cto_struct("CTO_FleetSelect", 31, {
	member_v("FleetID", "fleetID", "invalidFleetID"),
})

cto_struct("CTO_FleetSelectAdd", 32, {
	member_v("FleetID", "fleetID", "invalidFleetID"),
})

cto_struct("CTO_FleetClearSelection", 33, {
})

cto_struct("CTO_FleetSelectBox", 34, {
	member_mv("std::vector<FleetID>", "fleetIDs"),
})

cto_struct("CTO_FleetMove", 35, {
	member_v("libv::vec3f", "target_position"),
})

cto_struct("CTO_FleetMoveQueue", 36, {
	member_v("libv::vec3f", "target_position"),
})

cto_struct("CTO_FleetAttackFleet", 37, {
	member_v("FleetID", "targetFleetID", "invalidFleetID"),
})

cto_struct("CTO_FleetAttackFleetQueue", 38, {
	member_v("FleetID", "targetFleetID", "invalidFleetID"),
})

cto_struct("CTO_FleetAttackPlanet", 39, {
	member_v("PlanetID", "targetPlanetID", "invalidPlanetID"),
})

cto_struct("CTO_FleetAttackPlanetQueue", 40, {
	member_v("PlanetID", "targetPlanetID", "invalidPlanetID"),
})

cto_struct("CTO_ClearFleets", 41, {
})

cto_struct("CTO_Shuffle", 42, {
})

cto_struct("CTO_PlanetSpawn", 43, {
	--member_v("FactionID", "factionID", "invalidFactionID"),
	--member_v("FleetID", "fleetID", "invalidFleetID"),
	member_v("libv::vec3f", "position"),
})

cto_struct("CTO_ClearPlanets", 44, {
})

----------------------------------------------------------------------------------------------------

cto_struct("CTO_TrackView", 50, {
	member_v("UserID", "userID", "invalidUserID"),
	member_v("libv::vec3f", "eye"),
	member_v("libv::vec3f", "target"),
	member_v("libv::vec3f", "mouse_direction"),
})

cto_struct("CTO_CameraWarpTo", 51, {
	member_v("UserID", "userID", "invalidUserID"),
	member_v("libv::vec3f", "target_position"),
})

--cto_struct("CTO_CameraMovement", 52, {
--	member_v("UserID", "userID", "invalidUserID"),
--	member_v("libv::vec3f", "eye"),
--	member_v("libv::vec3f", "target"),
--	member_v("libv::vec3f", "mouse_direction"),
--})

--cto_struct("CTO_MouseMovement", 53, {
--	member_v("UserID", "userID", "invalidUserID"),
--	member_v("libv::vec3f", "mouse_position"),
--	member_v("libv::vec3f", "mouse_direction"),
--})
