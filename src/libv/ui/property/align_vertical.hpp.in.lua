-- Project: libv.ui, File: src/libv/ui/property/align_vertical.hpp.in.lua, Author: Császár Mátyás [Vader]

enum("AlignVertical", "int32_t")
namespace("libv::ui")

rate = property("rate", "float")
justified = property("justified", "bool")

value("top",         "top",         { rate("1.0f"), justified("false") })
value("center",      "center",      { rate("0.5f"), justified("false") })
value("bottom",      "bottom",      { rate("0.0f"), justified("false") })
-- TODO P4: libv.ui: it would be nice to have a baseline vertical alignment
value("justify",     "justify",     { rate("1.0f"), justified("true") })
value("justify_all", "justify-all", { rate("1.0f"), justified("true") })

--		parse_alias("justify-all", "justify_all")
