#

include_guard(GLOBAL)

### Sets CMAKE_CONFIGURATION_TYPES and CMAKE_BUILD_TYPE
### `DEFAULT` keyword marks the default configuration type
macro(wish_configurations)
    cmake_parse_arguments(arg "" "DEFAULT" "" ${ARGN})

#    set(CMAKE_CONFIGURATION_TYPES "Release;Dev;Debug" CACHE STRING "" FORCE)
    set(CMAKE_CONFIGURATION_TYPES "${arg_UNPARSED_ARGUMENTS};${arg_DEFAULT}" CACHE STRING "" FORCE)

    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE ${arg_DEFAULT})
    endif()

    string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
endmacro()
