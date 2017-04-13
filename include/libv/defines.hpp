// File: defines.hpp Author: Vader Created on 2017. április 13., 21:57

#pragma once

#include <cstring>


// -------------------------------------------------------------------------------------------------

// TODO P4: Move __LIBV_SHORT_FILE__ and __LIBV_SHORT_PATH__ into a better place
#ifndef LIBV_SHORT_PATH_CUTOFF
#    define LIBV_SHORT_PATH_CUTOFF 0
#endif

#define __LIBV_SHORT_FILE__ (std::strrchr(__FILE__, '/') + 1)
#define __LIBV_SHORT_PATH__ (__FILE__ + LIBV_SHORT_PATH_CUTOFF)

#define LIBV_POC ::libv::CodePosition{__LIBV_SHORT_PATH__, __FUNCTION__, __LINE__}

// -------------------------------------------------------------------------------------------------

