-- range.lua

local class = require "lib.util.class"

--

local module = {}

-- Iterator

local Iterator = class(function (self, iter_func)
	self.iter_func = iter_func
end)

function Iterator.__call(self)
	return self.iter_func()
end

function Iterator:iter()
	return self.iter_func
end

-- Vector

local Vector = class(function (self, element_type, element_number)
	self.size = element_number or 0
	self.data = {}

	if (element_type) then
		for i = 1, self.size do
			self.data[i] = element_type()
		end
	end
end)

function Vector.__call(self)
	error("Vector cannot be used as an iterator"..
		"use 'variable:iter()' or 'range(variable)' for iterator access", 2)
end

function Vector:push_back(element)
	self.size = self.size + 1
	self.data[self.size] = element
end

function Vector:sort(comparator)
	table.sort(self.data, comparator)
end

function Vector:iter()
	local i = 0
	return Iterator(function ()
		i = i + 1
		if i <= self.size then
			return self.data[i]
		else
			return nil
		end
	end)
end

-- Operation

local function operation(func)
	local op = {}
	setmetatable(op, {
		__mod = function (lhs, _)
			return func(lhs:iter())
		end,
	})
	return op
end

-- Actions -----------------------------------------------------------------------------------------

-- Make

module.make = function (container_type)
	return operation(function (iterator)
		local result = container_type()
		local element = iterator()
		while element do
			result:push_back(element)
			element = iterator()
		end
		return result
	end)
end

---- Sort
--
--module.sort = function (comparator)
--	return operation(function (iterator)
--		local result = Vector()
--		local element = iterator()
--		while element do
--			result:push_back(element)
--			element = iterator()
--		end
--		result:sort(comparator)
--		return result:iter()
--	end)
--end

-- Views -------------------------------------------------------------------------------------------

-- Take

module.take = function (num)
	return operation(function (iterator)
		local i = 0
		return Iterator(function ()
			i = i + 1
			if i <= num then
				return iterator()
			else
				return nil
			end
		end)
	end)
end

--

---- Operation
--
--local function operation(func)
--	local op = {}
--	setmetatable(op, {
--		__call = function (_, ...)
--			return func(...)
--		end,
--		__mod = function (lhs, rhs)
--			return function (...)
--				rhs(lhs:iter(), ...)
--			end
--		end,
--	})
--	return func
--end
--
---- As
--
--module.as = operation(function (container_type, iterator)
--	local result = container_type()
--	local element = iterator()
--	while element do
--		result:push_back(element)
--	end
--	return result
--end)
--
---- Take
--
--module.take = operation(function (iterator, num)
--	local i = 0
--	return function ()
--		i = i + 1
--		if i <= num then
--			return iterator()
--		else
--			return nil
--		end
--	end
--end)
--
----

setmetatable(module, {
	__call = function (_, range)
		return range:iter()
	end,
})

module.Iterator = Iterator
module.Vector = Vector

return module
