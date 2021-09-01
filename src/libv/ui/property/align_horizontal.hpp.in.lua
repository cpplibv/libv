-- Project: libv.ui, File: src/libv/ui/property/align_horizontal.hpp.in.lua, Author: Császár Mátyás [Vader]

enum("AlignHorizontal", "int32_t")
namespace("libv::ui")

rate = property("rate", "float")
justified = property("justified", "bool")

value("right",       "right",       { rate("1.0f"), justified("false") })
value("center",      "center",      { rate("0.5f"), justified("false") })
value("left",        "left",        { rate("0.0f"), justified("false") })
-- TODO P5: libv.ui: it would be nice to have a baseline horizontal alignment, even if it wont be ever used, and is basically a left align, still.
value("justify",     "justify",     { rate("0.0f"), justified("true") })
value("justify_all", "justify-all", { rate("0.0f"), justified("true") })

--		parse_alias("justify-all", "justify_all")
