


---
--- Import and executes a lua source file. Source files are only executed once and results are cached.
--- Repeated calls to import will return their cached value
---
---@param path string
---@return any
import = function(path) end

---
ui = {}

---
---@param name string
---@return function
ui.style = function(name) end

---
---@param state_conditions string
---@return function
function state(state_conditions) end

---
---@param nest_name string
---@return function
function nest(nest_name) end
