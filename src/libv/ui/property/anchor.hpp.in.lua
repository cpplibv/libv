-- Project: libv.ui, File: src/libv/ui/property/anchor.hpp.in.lua, Author: Császár Mátyás [Vader]

enum("Anchor", "int32_t")
namespace("libv::ui")

include("libv/math/vec.hpp")

rate = property("rate", "libv::vec3f")

value("top_left",      "top-left",      { rate("libv::vec3f{0.0f, 1.0f, 0.0f}") })
value("top_center",    "top-center",    { rate("libv::vec3f{0.5f, 1.0f, 0.0f}") })
value("top_right",     "top-right",     { rate("libv::vec3f{1.0f, 1.0f, 0.0f}") })
value("center_left",   "center-left",   { rate("libv::vec3f{0.0f, 0.5f, 0.0f}") })
value("center_center", "center-center", { rate("libv::vec3f{0.5f, 0.5f, 0.0f}") })
value("center_right",  "center-right",  { rate("libv::vec3f{1.0f, 0.5f, 0.0f}") })
value("bottom_left",   "bottom-left",   { rate("libv::vec3f{0.0f, 0.0f, 0.0f}") })
value("bottom_center", "bottom-center", { rate("libv::vec3f{0.5f, 0.0f, 0.0f}") })
value("bottom_right",  "bottom-right",  { rate("libv::vec3f{1.0f, 0.0f, 0.0f}") })
