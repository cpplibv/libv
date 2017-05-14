// File: defines.hpp Author: Vader Created on 2017. április 13., 21:57

#pragma once

#include <cstring>


// -------------------------------------------------------------------------------------------------

#ifndef LIBV_SHORT_PATH_CUTOFF
#    define LIBV_SHORT_PATH_CUTOFF 0
#endif

#define __LIBV_SHORT_FILE__ (std::strrchr(__FILE__, '/') + 1)
#define __LIBV_SHORT_PATH__ (__FILE__ + LIBV_SHORT_PATH_CUTOFF)

// TODO P5: We need LIBV_POC definition included in this header.
#define LIBV_POC ::libv::CodePosition{__LIBV_SHORT_PATH__, __FUNCTION__, __LINE__}

// -------------------------------------------------------------------------------------------------

