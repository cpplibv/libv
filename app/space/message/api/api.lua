
--- @class type_clazz

--- @class type_out

--- @class type_member

-- -------------------------------------------------------------------------------------------------

---
--- Add an include entry to the hpp header file
---
---@param path string
---
function include_hpp(path) end

---
--- Add an include entry to the cpp source file
---
---@param path string
---
function include_cpp(path) end

-- -------------------------------------------------------------------------------------------------

---
---@param generator fun(o:type_out, c:type_clazz)
---
function enable_hpp(generator) end

---
---@param generator fun(o:type_out, c:type_clazz)
---
function enable_cpp(generator) end

---
---@param identifier string
---
function namespace(identifier) end

-- -------------------------------------------------------------------------------------------------

---
---@generic T
---@param identifier string
---@param type string
---@param init_value T
---
function member_mv(identifier, type, init_value) end

---
---@generic T
---@param identifier string
---@param type string
---@param init_value T
---
function member_v(identifier, type, init_value) end

---
---@generic T
---@param identifier string
---@param type string
---@param init_value T
---
function member_sc(identifier, type, init_value) end

---
---@param identifier string
---@param members table<type_member | type_clazz>
---@return type_clazz
---
function struct(identifier, members) end
