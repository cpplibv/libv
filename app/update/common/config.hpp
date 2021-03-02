// Project: libv.update, File: app/update/common/config.hpp, Author: Császár Mátyás [Vader]

// std
#include <cstdint>
#include <string_view>


namespace app {

// -------------------------------------------------------------------------------------------------

static constexpr std::string_view default_address = "localhost";
static constexpr uint16_t default_port = 25090;
static constexpr uint16_t default_net_thread = 4;

// -------------------------------------------------------------------------------------------------

} // namespace app
