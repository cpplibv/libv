// Project: libv.algo, File: src/libv/algo/replace_all.hpp

#pragma once

// std
#include <string>
#include <string_view>


namespace libv {

// -------------------------------------------------------------------------------------------------

inline void replace_all_inplace(std::string& string, std::string_view search, std::string_view replace) {
    auto pos = string.find(search);
    while (pos != std::string::npos) {
        string.replace(pos, search.size(), replace);
        pos = string.find(search, pos + replace.size());
    }
}

[[nodiscard]] inline std::string replace_all_copy(std::string string, std::string_view search, std::string_view replace) {
    replace_all_inplace(string, search, replace);
    return string;
}

// -------------------------------------------------------------------------------------------------

} // namespace libv
