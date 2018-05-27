// File: log.hpp, Created on 2015. március 20. 2:37, Author: Vader

#pragma once

// libv
#include <libv/log/log.hpp>


namespace libv {
namespace vm4 {

// -------------------------------------------------------------------------------------------------

inline LoggerModule log_vm4{libv::logger, "libv.vm4"};

#define LIBV_LOG_VM4_TRACE(...) ::libv::vm4::log_vm4.trace(__VA_ARGS__)
#define LIBV_LOG_VM4_DEBUG(...) ::libv::vm4::log_vm4.debug(__VA_ARGS__)
#define LIBV_LOG_VM4_INFO( ...) ::libv::vm4::log_vm4.info (__VA_ARGS__)
#define LIBV_LOG_VM4_WARN( ...) ::libv::vm4::log_vm4.warn (__VA_ARGS__)
#define LIBV_LOG_VM4_ERROR(...) ::libv::vm4::log_vm4.error(__VA_ARGS__)
#define LIBV_LOG_VM4_FATAL(...) ::libv::vm4::log_vm4.fatal(__VA_ARGS__)

// -------------------------------------------------------------------------------------------------

} // namespace vm4
} // namespace libv
