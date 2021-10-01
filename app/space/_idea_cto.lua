
--member:
--	name : string
--	type : string
--	use_move : boolean
--
--clazz:
--	name : string
--	members : list<member>

local out = function(str)
	print((str == "" and "" or __ident) .. str);
end

local generate_encode = function(archive)
	return function(clazz)
		out("")
		out("virtual void encode(" .. archive .. "& ar) {")
		for member in clazz.members do
			out("\tar & LIBV_NAMED_NVP(\"" .. member.name .. "\", " .. member.name .. ");")
		end
		out("}")
	end
end

local generate_decode = function(archive)
	return function(clazz)
		out("")
		out("virtual void decode(" .. archive .. "& ar) {")
		for member in clazz.members do
			out("\tar & LIBV_NAMED_NVP(\"" .. member.name .. "\", " .. member.name .. ");")
		end
		out("}")
	end
end

local generate_constructor = function(clazz)
	if next(clazz.members) == nil then
		return -- No member
	end

	local args_list = ""
	local init_list = ""

	for member in clazz.members do
		if (args_list ~= "") then
			args_list = args_list .. ", "
			init_list = init_list .. ", "
		end

		args_list = args_list .. member.type .. " " .. member.name

		if member.use_move then
			init_list = init_list .. member.name .. "(std::move(" .. member.name .. "))"
		else
			init_list = init_list .. member.name .. "(" .. member.name .. ")"
		end
	end

	out("")
	out(clazz.name .. "(" .. args_list .. ") :")
	out("\t\t" .. init_list .. " { }")
end




include("<libv/math/vec.hpp>")
include("<string>")
include("<vector>")


include("<libv/serial/archive/binary.hpp>")
enable(generate_constructor)
enable(generate_encode("libv::archive::BinaryInput"))
enable(generate_decode("libv::archive::BinaryOutput"))

namespace("app")

out("struct BaseCTO {")
out("	static constexpr uint8_t index;")
out("	virtual void encode(libv::archive::BinaryInput& ar) = 0;")
out("	virtual void decode(libv::archive::BinaryOutput& ar) = 0;")
out("};")

struct("CTO_ChatMessage", "BaseCTO") {
	member_mv("std::string", "user_name"),
	member_mv("std::string", "message"),
}

struct("CTO_LobbyStatus", "BaseCTO") {
	struct("Entry") {
		member_mv("std::string", "user_name"),
		member_v("float", "ping"),
	},

	member_mv("std::vector<Entry>", "pings"),
}

struct("CTO_FleetMove", "BaseCTO") {
	member_v("libv::vec3f", "target_position"),
}

