-- Project: libv.ui, File: src/libv/ui/property/anchor.hpp.in.lua, Author: Császár Mátyás [Vader]

enum("FocusMode", "int8_t")
namespace("libv::ui")

mouse = property("mouse", "bool")
focusable = property("focusable", "bool")

value("active",    "active",    { mouse("true"), focusable("true") })
value("passive",   "passive",   { mouse("false"), focusable("true") })
value("inactive",  "inactive",  { mouse("false"), focusable("false") })
