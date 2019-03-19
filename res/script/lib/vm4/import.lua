
local class = require "lib.util.class"

--

local vm4 = {}

-- Importer

--local Importer = class(function (self, element_type, element_number)
----	self.size = element_number or 0
----	self.data = {}
----
----	if (element_type) then
----		for i = 1, self.size do
----			self.data[i] = element_type()
----		end
----	end
--end)
--
--function Vector:push_back(element)
--	self.size = self.size + 1
--	self.data[self.size] = element
--end
--
--function Vector:iter()
--	local i = 0
--	return Iterator(function ()
--		i = i + 1
--		if i <= self.size then
--			return self.data[i]
--		else
--			return nil
--		end
--	end)
--end

function vm4.material(tab)
	return tab
end

function vm4.node(tab)
	return tab
end

function vm4.import(path)
	return {
		path = path,
		material = {},
		mesh = {},
		lod = {},
	}
end

----------------------------------------------------------------------------------------------------

return vm4
