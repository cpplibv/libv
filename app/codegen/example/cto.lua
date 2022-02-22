
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
	out("public:\n")
	out("\texplicit inline " .. clazz.identifier .. "(" .. args_list .. ") :\n")
	out("\t\t\t" .. init_list .. " { }\n")
end

local generate_encode_hpp = function(archive)
	return function(out, clazz)
		out("\tvoid encode(" .. archive .. "& ar);\n")
	end
end

local generate_encode_cpp = function(archive)
	return function(out, clazz)
		out("\n")
		out("void " .. clazz.scope_class .. "::encode(" .. archive .. "& ar) {\n")
		for _, member in ipairs(clazz.members) do
			out("\tar(LIBV_NVP_FORCED(\"" .. member.identifier .. "\", " .. member.identifier .. "));\n")
		end
		out("}\n")
	end
end

local generate_decode_hpp = function(archive)
	return function(out, clazz)
		out("\tvoid decode(" .. archive .. "& ar);\n")
	end
end

local generate_decode_cpp = function(archive)
	return function(out, clazz)
		out("\n")
		out("void " .. clazz.scope_class .. "::decode(" .. archive .. "& ar) {\n")
		for _, member in ipairs(clazz.members) do
			out("\tar(LIBV_NVP_FORCED(\"" .. member.identifier .. "\", " .. member.identifier .. "));\n")
		end
		out("}\n")
	end
end

----------------------------------------------------------------------------------------------------

include_hpp("libv/math/vec.hpp")
include_hpp("libv/serial/archive/binary_fwd.hpp")
include_hpp("string")
include_hpp("vector")

include_cpp("codegen/example/cto.out.hpp")
include_cpp("libv/serial/serial.hpp")
include_cpp("libv/serial/archive/binary.hpp")

enable_hpp(generate_constructor)
enable_hpp(generate_encode_hpp("libv::archive::BinaryInput"))
enable_cpp(generate_encode_cpp("libv::archive::BinaryInput"))
enable_hpp(generate_decode_hpp("libv::archive::BinaryOutput"))
enable_cpp(generate_decode_cpp("libv::archive::BinaryOutput"))

namespace("space")

--out("struct BaseCTO {")
--out("	static constexpr uint8_t index;")
--out("	virtual void encode(libv::archive::BinaryInput& ar) = 0;")
--out("	virtual void decode(libv::archive::BinaryOutput& ar) = 0;")
--out("};")

struct("CTO_ChatMessage", {
----struct("CTO_ChatMessage", "BaseCTO") {
	member_mv("std::string", "user_name"),
	member_mv("std::string", "message"),
})

struct("CTO_LobbyStatus", {
--struct("CTO_LobbyStatus", "BaseCTO") {
	struct("Entry", {
		struct("EntryInner", {
			member_mv("std::string", "user_name", "\"Name\""),
			member_v("float", "ping", "42"),
		}),

		member_mv("EntryInner", "inner"),
		member_mv("std::string", "user_name"),
		member_v("float", "ping"),
	}),

	member_mv("std::vector<Entry>", "pings"),
})

struct("CTO_FleetMove", {
--struct("CTO_FleetMove", "BaseCTO") {
	member_v("libv::vec3f", "target_position"),
})

struct("CTO_FleetMove2", {
--struct("CTO_FleetMove", "BaseCTO") {
	member_v("libv::vec3f", "target_position"),
	member_v("int", "x", "32"),
})
