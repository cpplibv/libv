-- Project: libv.ui, File: src/libv/ui/property/scroll_mode.hpp.in.lua, Author: Császár Mátyás [Vader]

enum("ScrollMode", "int8_t")
namespace("libv::ui")

horizontal = property("horizontal", "bool")
vertical = property("vertical", "bool")

value("none",       "none"      , { horizontal("false"), vertical("false") })
value("both"      , "both"      , { horizontal("true" ), vertical("true" ) })
value("horizontal", "horizontal", { horizontal("true" ), vertical("false") })
value("vertical"  , "vertical"  , { horizontal("false"), vertical("true" ) })
