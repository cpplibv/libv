-- dump.lua

--- TODO P4: output stream or string return for dump functions and do not direct print

--- Dumps a variable reqursively 4 space formated into the stdout
--- @param var - Variable to dump
--- @param prefix - Prefix used as first level indentation
local function dump(var, prefix)
	local function dump_impl(seen, var, level, name)
		if (type(var) ~= "table") then
			print(string.format("%s%s = %s", level, name, var))
			return
		end

		if seen[var] then
			print(string.format("%s%s = <<seen %s>>", level, name, tostring(var)))
			return
		end
		--		seen[var] = #seen
		seen[var] = true

		if name then
			print(string.format("%s%s", level, name))
		end

		local sorted_keys = {}
		--local n = 0
		for key in pairs(var) do
			--n = n + 1
			--sorted_keys[n] = key
			sorted_keys[#sorted_keys + 1] = key
		end

		table.sort(sorted_keys, function (lhs, rhs)
			if type(lhs) == type(rhs) and type(lhs) ~= "table" then
				return lhs < rhs
			else
				return tostring(lhs) < tostring(rhs)
			end
		end)

		for _, key in pairs(sorted_keys) do
			if name then
				dump_impl(seen, var[key], level .. "    ", tostring(key))
			else
				dump_impl(seen, var[key], level, tostring(key))
			end
		end
	end

	dump_impl({}, var, "", prefix)
end

return dump
