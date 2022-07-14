# File: target.cmake, Created on 2017. 04. 14. 16:49, Author: Vader

include_guard(GLOBAL)

include(ExternalProject)

include(cmake/wish_compiler_flags.cmake)
include(cmake/wish_configuration.cmake)
include(cmake/wish_create.cmake)
include(cmake/wish_system.cmake)
include(cmake/wish_version.cmake)

# -------------------------------------------------------------------------------------------------

message(STATUS "Wish version: ${wish_version}")

# -------------------------------------------------------------------------------------------------

macro(wish_force_colored_output value)
    if (${value})
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            message(STATUS "Force colored output: GCC")
            add_compile_options(-fdiagnostics-color=always)
        elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            message(STATUS "Force colored output: Clang")
            add_compile_options(-fcolor-diagnostics)
        else()
            message(STATUS "Force colored output: False (Unknown compiler)")
        endif()
    else()
        message(STATUS "Force colored output: False")
    endif()
endmacro()

# -------------------------------------------------------------------------------------------------

macro(wish_skip_external_configures value)
    if(${value})
        wish_disable_configure_externals()
        message(STATUS "Skipping external project configurations")
    else()
        wish_enable_configure_externals()
        message(STATUS "Generating external project configurations")
    endif()
endmacro()

# -------------------------------------------------------------------------------------------------

macro(wish_optimization_flags)
    if(CMAKE_BUILD_TYPE STREQUAL "debug")
        add_compile_options(-Og)
        add_compile_options(-ggdb3)

    elseif(CMAKE_BUILD_TYPE STREQUAL "dev")
        add_compile_options(-O3)

    elseif(CMAKE_BUILD_TYPE STREQUAL "release")
        add_compile_options(-O3)
        add_compile_options(-flto)
        SET(CMAKE_AR "gcc-ar")
        SET(CMAKE_NM "gcc-nm")
        SET(CMAKE_RANLIB "gcc-ranlib")
    #	add_definitions(-DNDEBUG)
        wish_static_link_std()

    else()
        message(WARNING "Failed to identify [${CMAKE_BUILD_TYPE}] as a build type")
    endif()
endmacro()

# Definitions --------------------------------------------------------------------------------------

# ${WISH_DATE_LONG}
# ${WISH_DATE_SHORT}
# ${WISH_TIME_LONG}
# ${WISH_TIME_SHORT}
# ${WISH_GIT_BRANCH}
# ${WISH_GIT_COMMIT_HASH}

string(LENGTH ${CMAKE_SOURCE_DIR}_ WISH_SHORT_PATH_CUTOFF)
add_definitions(-DWISH_SHORT_PATH_CUTOFF=${WISH_SHORT_PATH_CUTOFF})
add_definitions(-DWISH_SHORT_PATH_PREFIX="${CMAKE_SOURCE_DIR}/")

# -------------------------------------------------------------------------------------------------
