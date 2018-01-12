#

cmake_minimum_required(VERSION 3.7)
project(libv)

include(ExternalProject)
include(cmake/util.cmake)
include(cmake/target.cmake)

# --------------------------------------------------------------------------------------------------

set(CMAKE_VERBOSE_MAKEFILE off)
#set(CMAKE_BUILD_TYPE "DEBUG")
#set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_BUILD_TYPE "DEV")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor count: ${PROCESSOR_COUNT}")
message(STATUS "CXX compiler id: ${CMAKE_CXX_COMPILER_ID}")

# TODO ---------------------------------------------------------------------------------------------

# TODO P2: There is a way to include thread this via cmake built-in options
# TODO P2: Use the correct OpenGL linking cmake build-in options
# TODO P2: Instead of USE_STATIC_LINK use cmake default if possible
# ... see in Iris CMakeLists.txt

# Paths --------------------------------------------------------------------------------------------

set(PATH_BIN ${PROJECT_SOURCE_DIR}/bin)
set(PATH_BLD build)
set(PATH_LIB ${PROJECT_SOURCE_DIR}/lib)
set(PATH_PRJ ${CMAKE_CURRENT_SOURCE_DIR})
set(PATH_EXT ${PATH_PRJ}/ext)
set(PATH_EXT_SRC ${PATH_PRJ}/ext_src)
set(PATH_INC include)
set(PATH_SRC src)
set(PATH_EXP example)
set(PATH_SNB sandbox)
set(PATH_TST test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PATH_LIB})

# Variables ----------------------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/Wall)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Weverything -Wmissing-override -Wno-comment -Wconversion)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(-Wall -Wextra -Wpedantic -Wno-comment
		-Wcast-qual
		-Wdelete-non-virtual-dtor
		-Wdouble-promotion
		-Wduplicated-cond
		-Wlogical-op
		-Wnon-virtual-dtor
		-Wnull-dereference
		-Wold-style-cast
		-Wsuggest-override
		-Wundef

# Enablement in progress:
#		-Wuseless-cast # in vec_base_t has some false positive issue with this one, but otherwise clear
# 		-Wconversion # 130+, most of it is in vec and the rest is in gl
# 		-Wfloat-equal # in tests 40+ false positive

# Only enable time-to-time to check on what is going on:
# 		-Winline
# 		-Wsuggest-final-methods # in signal 50+ function
# 		-Wsuggest-final-types # in signal 1 type
	)

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
		add_compile_options(
			-Wduplicated-branches
			-Wrestrict
			-Wshadow-compatible-local
		)
	endif()

	if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
		# Sanitizers current only available on unix
		# add_compile_options(-fsanitize=address)
		# add_compile_options(-fsanitize=kernel-address)
		# add_compile_options(-fsanitize=leak)
		# add_compile_options(-fsanitize=thread)
		# add_compile_options(-fsanitize=undefined)
		# link_libraries(ubsan)
		# link_libraries(asan)
	endif()
endif()

string(LENGTH ${PATH_PRJ}_ LIBV_SHORT_PATH_CUTOFF)

# Git ----------------------------------------------------------------------------------------------

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
  COMMAND git rev-parse --short=8 head
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

# Definitions --------------------------------------------------------------------------------------

set(USE_STATIC_LINK)

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	add_compile_options(-Og)
	add_compile_options(-ggdb3)
	add_compile_options(-fno-omit-frame-pointer)

elseif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	add_compile_options(-Ofast)
#	add_compile_options(-flto)
#	SET(CMAKE_AR "gcc-ar")
#	SET(CMAKE_NM "gcc-nm")
#	SET(CMAKE_RANLIB "gcc-ranlib")
	set(USE_STATIC_LINK -static)

elseif(CMAKE_BUILD_TYPE STREQUAL "DEV")
	add_definitions(-Ofast)

else()
	message(WARNING "Failed to identify [${CMAKE_BUILD_TYPE}] as a build type")
endif()

add_definitions(-m64)
add_definitions(-std=c++17)

# Macro - Ext
add_definitions(-DCEREAL_XML_STRING_VALUE="root")
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-DSOL_USE_BOOST)
# Macro - Libv
add_definitions(-DLIBV_SHORT_PATH_CUTOFF=${LIBV_SHORT_PATH_CUTOFF})
add_definitions(-DLIBV_USE_GLM_BRIDGE)
# Macro - Assert
add_definitions(-DNDEBUG)

# --------------------------------------------------------------------------------------------------

if(NOT WIN32)
	link_libraries(pthread)
endif()

# --------------------------------------------------------------------------------------------------

find_package(OpenGL REQUIRED)

# --------------------------------------------------------------------------------------------------

set(GROUP_EXTERNAL_PROJECT)


list(APPEND GROUP_EXTERNAL_PROJECT get_assimp)
ExternalProject_Add(get_assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v3.3.1
	PREFIX ${PATH_EXT_SRC}/assimp
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/assimp
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
		-DBUILD_SHARED_LIBS=OFF
		-DASSIMP_NO_EXPORT=ON
		-DASSIMP_BUILD_ASSIMP_TOOLS=OFF
		-DASSIMP_BUILD_TESTS=OFF
		#
		-DASSIMP_BUILD_COLLADA_IMPORTER=ON
		#
		-DASSIMP_BUILD_3DS_IMPORTER=OFF
		-DASSIMP_BUILD_3D_IMPORTER=OFF
		-DASSIMP_BUILD_3MF_IMPORTER=OFF
		-DASSIMP_BUILD_AC_IMPORTER=OFF
		-DASSIMP_BUILD_ASE_IMPORTER=OFF
		-DASSIMP_BUILD_ASSBIN_IMPORTER=OFF
		-DASSIMP_BUILD_B3D_IMPORTER=OFF
		-DASSIMP_BUILD_BLEND_IMPORTER=OFF
		-DASSIMP_BUILD_BVH_IMPORTER=OFF
		-DASSIMP_BUILD_COB_IMPORTER=OFF
		-DASSIMP_BUILD_CSM_IMPORTER=OFF
		-DASSIMP_BUILD_DXF_IMPORTER=OFF
		-DASSIMP_BUILD_FBX_IMPORTER=OFF
		-DASSIMP_BUILD_GLTF_IMPORTER=OFF
		-DASSIMP_BUILD_HMP_IMPORTER=OFF
		-DASSIMP_BUILD_IFC_IMPORTER=OFF
		-DASSIMP_BUILD_IRR_IMPORTER=OFF
		-DASSIMP_BUILD_LWO_IMPORTER=OFF
		-DASSIMP_BUILD_LWS_IMPORTER=OFF
		-DASSIMP_BUILD_MD2_IMPORTER=OFF
		-DASSIMP_BUILD_MD3_IMPORTER=OFF
		-DASSIMP_BUILD_MD5_IMPORTER=OFF
		-DASSIMP_BUILD_MDC_IMPORTER=OFF
		-DASSIMP_BUILD_MDL_IMPORTER=OFF
		-DASSIMP_BUILD_MS3D_IMPORTER=OFF
		-DASSIMP_BUILD_NDO_IMPORTER=OFF
		-DASSIMP_BUILD_NFF_IMPORTER=OFF
		-DASSIMP_BUILD_OBJ_IMPORTER=OFF
		-DASSIMP_BUILD_OFF_IMPORTER=OFF
		-DASSIMP_BUILD_OGRE_IMPORTER=OFF
		-DASSIMP_BUILD_OPENGEX_IMPORTER=OFF
		-DASSIMP_BUILD_PLY_IMPORTER=OFF
		-DASSIMP_BUILD_Q3BSP_IMPORTER=OFF
		-DASSIMP_BUILD_Q3D_IMPORTER=OFF
		-DASSIMP_BUILD_RAW_IMPORTER=OFF
		-DASSIMP_BUILD_SIB_IMPORTER=OFF
		-DASSIMP_BUILD_SMD_IMPORTER=OFF
		-DASSIMP_BUILD_STL_IMPORTER=OFF
		-DASSIMP_BUILD_TERRAGEN_IMPORTER=OFF
		-DASSIMP_BUILD_XGL_IMPORTER=OFF
		-DASSIMP_BUILD_X_IMPORTER=OFF
	EXCLUDE_FROM_ALL
)
add_library(ext_assimp INTERFACE)
target_include_directories(ext_assimp SYSTEM INTERFACE ${PATH_EXT}/assimp/include)
link_directories(${PATH_EXT}/assimp/lib)
target_link_libraries(ext_assimp INTERFACE assimp zlibstatic)


list(APPEND GROUP_EXTERNAL_PROJECT get_boost)
set(Boost_Version 1.66.0)
string(REPLACE "." "_" Boost_Version_Underscore ${Boost_Version})

set(TOOLSET)
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(TOOLSET msvc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(TOOLSET clang)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(TOOLSET gcc)
endif()

message(STATUS "Boost version: " ${Boost_Version})
ExternalProject_Add(get_boost
	PREFIX ${PATH_EXT_SRC}/boost
	URL https://dl.bintray.com/boostorg/release/${Boost_Version}/source/boost_${Boost_Version_Underscore}.zip
	CONFIGURE_COMMAND ./bootstrap.sh ${TOOLSET}
	BUILD_COMMAND ./b2 --ignore-site-config install
		toolset=${TOOLSET}
		variant=release
		link=static
		runtime-link=static
		threading=multi
		address-model=64
		--prefix=${PATH_EXT}/boost
		--layout=tagged
		--with-system
#		--with-stacktrace
		-j${PROCESSOR_COUNT}
	BUILD_IN_SOURCE 1
	EXCLUDE_FROM_ALL
	INSTALL_COMMAND ""
)
add_library(ext_boost INTERFACE)
target_include_directories(ext_boost SYSTEM INTERFACE ${PATH_EXT}/boost/include)
#link_directories(${PATH_EXT}/boost/lib)
#target_link_libraries(ext_boost INTERFACE system stacktrace)


list(APPEND GROUP_EXTERNAL_PROJECT get_catch)
ExternalProject_Add(get_catch
	GIT_REPOSITORY https://github.com/philsquared/Catch.git
	GIT_TAG v1.5.6
	PREFIX ${PATH_EXT_SRC}/catch
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/catch/
		COMMAND cp -r -f ${PATH_EXT_SRC}/catch/src/ext_catch/include/ ${PATH_EXT}/catch/
	EXCLUDE_FROM_ALL
)
add_library(ext_catch INTERFACE)
target_include_directories(ext_catch SYSTEM INTERFACE ${PATH_EXT}/catch/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_cereal)
ExternalProject_Add(get_cereal
	GIT_REPOSITORY https://github.com/USCiLab/cereal.git
	GIT_TAG v1.2.2
	PREFIX ${PATH_EXT_SRC}/cereal
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/cereal
		-DJUST_INSTALL_CEREAL=ON
	EXCLUDE_FROM_ALL
)
add_library(ext_cereal INTERFACE)
target_include_directories(ext_catch SYSTEM INTERFACE ${PATH_EXT}/cereal/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_clara)
ExternalProject_Add(get_clara
	GIT_REPOSITORY https://github.com/philsquared/Clara.git
	GIT_TAG 20b5c3abdc0637f3e73479340b63d368f9e0f2ce
	PREFIX ${PATH_EXT_SRC}/clara
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/clara/include/clara
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/clara/src/ext_clara/single_include ${PATH_EXT}/clara/include/clara
	EXCLUDE_FROM_ALL
)
add_library(ext_clara INTERFACE)
target_include_directories(ext_clara SYSTEM INTERFACE ${PATH_EXT}/clara/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_efsw)
ExternalProject_Add(get_efsw
	GIT_REPOSITORY https://github.com/mnafees/efsw.git
	GIT_TAG 9bc94939f154bc756def4e9788d83e1f164300f3
	PREFIX ${PATH_EXT_SRC}/efsw
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/efsw
	# TODO P3: this ext_efsw install command looks off
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/efsw/include
		COMMAND mkdir -p ${PATH_EXT}/efsw/lib
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/efsw/src/ext_efsw/include/ ${PATH_EXT}/efsw/include/
		COMMAND cp -r -f ${PATH_EXT_SRC}/efsw/src/ext_efsw-build/libefsw.a ${PATH_EXT}/efsw/lib/
	EXCLUDE_FROM_ALL
)
add_library(ext_efsw INTERFACE)
target_include_directories(ext_efsw SYSTEM INTERFACE ${PATH_EXT}/efsw/include)
link_directories(${PATH_EXT}/efsw/lib)
target_link_libraries(ext_efsw INTERFACE efsw)


list(APPEND GROUP_EXTERNAL_PROJECT get_fmt)
ExternalProject_Add(get_fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG bef89db6e7d10499a91affca7a933f264c7956d5
	PREFIX ${PATH_EXT_SRC}/fmt
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/fmt
		-DFMT_DOC=OFF
		-DFMT_TEST=OFF
	EXCLUDE_FROM_ALL
)
add_library(ext_fmt INTERFACE)
target_include_directories(ext_fmt SYSTEM INTERFACE ${PATH_EXT}/fmt/include)
link_directories(${PATH_EXT}/fmt/lib)
target_link_libraries(ext_fmt INTERFACE fmt)


list(APPEND GROUP_EXTERNAL_PROJECT get_freetype)
ExternalProject_Add(get_freetype
	GIT_REPOSITORY http://git.sv.nongnu.org/r/freetype/freetype2.git
	GIT_TAG VER-2-5-5
	PREFIX ${PATH_EXT_SRC}/freetype
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/freetype
	EXCLUDE_FROM_ALL
)
add_library(ext_freetype INTERFACE)
target_include_directories(ext_freetype SYSTEM INTERFACE ${PATH_EXT}/freetype/include/freetype2)
link_directories(${PATH_EXT}/freetype/lib)
target_link_libraries(ext_freetype INTERFACE freetype)


list(APPEND GROUP_EXTERNAL_PROJECT get_glew)
ExternalProject_Add(get_glew
	GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
	GIT_TAG glew-cmake-1.11.0
	PREFIX ${PATH_EXT_SRC}/glew
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glew
	EXCLUDE_FROM_ALL
)
add_library(ext_glew INTERFACE)
target_include_directories(ext_glew SYSTEM INTERFACE ${PATH_EXT}/glew/include)
link_directories(${PATH_EXT}/glew/lib)
target_link_libraries(ext_glew INTERFACE glew)


list(APPEND GROUP_EXTERNAL_PROJECT get_glfw)
ExternalProject_Add(get_glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.2.1
	PREFIX ${PATH_EXT_SRC}/glfw
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glfw
		-DGLFW_BUILD_EXAMPLES=OFF
		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_DOCS=OFF
	EXCLUDE_FROM_ALL
)
add_library(ext_glfw INTERFACE)
target_include_directories(ext_glfw SYSTEM INTERFACE ${PATH_EXT}/glfw/include)
link_directories(${PATH_EXT}/glfw/lib)
target_link_libraries(ext_glfw INTERFACE glfw3 gdi32)


list(APPEND GROUP_EXTERNAL_PROJECT get_gli)
ExternalProject_Add(get_gli
	GIT_REPOSITORY https://github.com/g-truc/gli.git
	GIT_TAG 0.8.2.0
	BUILD_COMMAND "" # gli is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/gli
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/gli
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/gli/include/gli/
		COMMAND cp -r -f ${PATH_EXT_SRC}/gli/src/ext_gli/gli/ ${PATH_EXT}/gli/include/
	EXCLUDE_FROM_ALL
)
add_library(ext_gli INTERFACE)
target_include_directories(ext_gli SYSTEM INTERFACE ${PATH_EXT}/gli/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_glm)
ExternalProject_Add(get_glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.8.5
	BUILD_COMMAND "" # glm is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/glm
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glm
	EXCLUDE_FROM_ALL
)
add_library(ext_glm INTERFACE)
target_include_directories(ext_glm SYSTEM INTERFACE ${PATH_EXT}/glm/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_lua)
ExternalProject_Add(get_lua
	GIT_REPOSITORY https://github.com/LuaDist/lua.git
	GIT_TAG 5.3.2
	PREFIX ${PATH_EXT_SRC}/lua
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/lua
		-DBUILD_SHARED_LIBS=OFF
	EXCLUDE_FROM_ALL
)
add_library(ext_lua INTERFACE)
target_include_directories(ext_lua SYSTEM INTERFACE ${PATH_EXT}/lua/include)
link_directories(${PATH_EXT}/lua/lib)
target_link_libraries(ext_lua INTERFACE soil)


list(APPEND GROUP_EXTERNAL_PROJECT get_net_ts)
ExternalProject_Add(get_net_ts
	GIT_REPOSITORY https://github.com/chriskohlhoff/networking-ts-impl.git
	GIT_TAG 6bf74be013e104e2893de245bc3379a6f3f20d54
	CONFIGURE_COMMAND ""
	BUILD_COMMAND "" # net_ts is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/net_ts
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/net_ts/
		COMMAND cp -r -f ${PATH_EXT_SRC}/net_ts/src/ext_net_ts/include/ ${PATH_EXT}/net_ts/
	EXCLUDE_FROM_ALL
)
add_library(ext_net_ts INTERFACE)
target_include_directories(ext_net_ts SYSTEM INTERFACE ${PATH_EXT}/net_ts/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_range)
ExternalProject_Add(get_range
	GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
	GIT_TAG 0ed88b12844792a83d2d0919ef49a341fdca28d1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND "" # range is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/range
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/range/
		COMMAND cp -r -f ${PATH_EXT_SRC}/range/src/ext_range/include/ ${PATH_EXT}/range/
	EXCLUDE_FROM_ALL
)
add_library(ext_range INTERFACE)
target_include_directories(ext_range SYSTEM INTERFACE ${PATH_EXT}/range/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_soil)
ExternalProject_Add(get_soil
	GIT_REPOSITORY https://github.com/xosdy/soil.git
	GIT_TAG master
	PREFIX ${PATH_EXT_SRC}/soil
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/soil
	EXCLUDE_FROM_ALL
)
add_library(ext_soil INTERFACE)
target_include_directories(ext_soil SYSTEM INTERFACE ${PATH_EXT}/soil/include)
link_directories(${PATH_EXT}/soil/lib)
target_link_libraries(ext_soil INTERFACE soil)


#create_external(
#	NAME soil
#	GIT_REPOSITORY https://github.com/xosdy/soil.git
#	GIT_TAG master
#	CMAKE_ARGS
#		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/soil
#	LINK soil
#)


list(APPEND GROUP_EXTERNAL_PROJECT get_sol)
ExternalProject_Add(get_sol
	GIT_REPOSITORY https://github.com/ThePhD/sol2.git
	GIT_TAG 16fc7d87ff0b9c2fc18268a420e0b058a67060a1
	PREFIX ${PATH_EXT_SRC}/sol
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/sol/include/sol
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/sol/src/ext_sol/sol/ ${PATH_EXT}/sol/include/sol/
	EXCLUDE_FROM_ALL
)
add_library(ext_sol INTERFACE)
target_include_directories(ext_sol SYSTEM INTERFACE ${PATH_EXT}/sol/include)


list(APPEND GROUP_EXTERNAL_PROJECT get_utf8cpp)
ExternalProject_Add(get_utf8cpp
	GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
	GIT_TAG f029fcc2fbc7cd979925f198f7e6ca8170d45000
	PREFIX ${PATH_EXT_SRC}/utf8cpp
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/utf8cpp/include/utf8cpp
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/utf8cpp/src/ext_utf8cpp/source ${PATH_EXT}/utf8cpp/include/utf8cpp
	EXCLUDE_FROM_ALL
)
add_library(ext_utf8cpp INTERFACE)
target_include_directories(ext_utf8cpp SYSTEM INTERFACE ${PATH_EXT}/utf8cpp/include)

# Directories --------------------------------------------------------------------------------------

# Includes
include_directories(${PATH_INC})
include_directories(${PATH_SRC})

# === Targets ======================================================================================

create_group(GROUP_LIBRARY) # Libraries ------------------------------------------------------------

create_library(
	TARGET libv_container INTERFACE
	LINK   ${USE_STATIC_LINK}
)

create_library(
	TARGET libv_ecs INTERFACE
	LINK   ${USE_STATIC_LINK} ext_boost ext_range
)

create_library(
	TARGET libv_frame STATIC
	SOURCE ${PATH_SRC}/libv/frame/*.cpp
	LINK   ${USE_STATIC_LINK} libv_log ext_boost ext_glfw ext_utf8cpp ${OPENGL_LIBRARIES}
)

create_library(
	TARGET libv_gl STATIC
	SOURCE ${PATH_SRC}/libv/gl/*.cpp
	LINK   ${USE_STATIC_LINK} libv_log ext_glew ext_gli ext_soil ${OPENGL_LIBRARIES}
)

create_library(
	TARGET libv_glr INTERFACE
	LINK   ${USE_STATIC_LINK} ext_boost
)

create_library(
	TARGET libv_log INTERFACE
	LINK   ${USE_STATIC_LINK} ext_fmt
)

create_library(
	TARGET libv_lua INTERFACE
	LINK   ${USE_STATIC_LINK} ext_sol ext_lua
)

create_library(
	TARGET libv_math INTERFACE
	LINK   ${USE_STATIC_LINK}
)

create_library(
	TARGET libv_meta INTERFACE
	LINK   ${USE_STATIC_LINK} ext_boost
)

create_library(
	TARGET libv_parse INTERFACE
	LINK   ${USE_STATIC_LINK} ext_boost
)

create_library(
	TARGET libv_range INTERFACE
	LINK   ${USE_STATIC_LINK} ext_boost ext_range
)

create_library(
	TARGET libv_serialization INTERFACE
	LINK   ${USE_STATIC_LINK} ext_boost
)

create_library(
	TARGET libv_sig STATIC
	SOURCE ${PATH_SRC}/libv/sig/*.cpp
	LINK   ${USE_STATIC_LINK}
)

create_library(
	TARGET libv_thread INTERFACE
	LINK   ${USE_STATIC_LINK}
)

create_library(
	TARGET libv_utility INTERFACE
	LINK   ${USE_STATIC_LINK} ext_efsw stdc++fs
)

create_library(
	TARGET libv_vm4 INTERFACE
	LINK   ${USE_STATIC_LINK} ext_boost
)

create_library(
	TARGET libv_vm4imp STATIC
	SOURCE ${PATH_SRC}/libv/vm4imp/*.cpp
	LINK   ${USE_STATIC_LINK} libv_log libv_vm4 ext_assimp stdc++fs
)

end_group()
create_group(GROUP_SANDBOX) # Sandboxes ------------------------------------------------------------

create_executable(
	TARGET sandbox_libv_libv
	SOURCE ${PATH_SNB}/libv_main.cpp
	LINK   ${USE_STATIC_LINK} libv_log ext_boost
)

create_executable(
	TARGET sandbox_libv_ecs
	SOURCE ${PATH_SNB}/libv_ecs_main.cpp
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET sandbox_libv_frame
	SOURCE ${PATH_SNB}/libv_frame_main.cpp
	LINK   ${USE_STATIC_LINK} libv_log libv_frame
)

create_executable(
	TARGET sandbox_libv_gl
	SOURCE ${PATH_SNB}/libv_gl_main.cpp
	LINK   ${USE_STATIC_LINK} libv_log libv_gl ext_glfw stdc++fs
)

create_executable(
	TARGET sandbox_libv_gl2
	SOURCE ${PATH_SNB}/libv_gl_main2.cpp
	LINK   ${USE_STATIC_LINK} libv_log libv_gl ext_glfw stdc++fs
)

create_executable(
	TARGET sandbox_libv_gl3
	SOURCE ${PATH_SNB}/libv_gl_main3.cpp
	LINK   ${USE_STATIC_LINK} libv_log libv_gl ext_glfw stdc++fs
)

create_executable(
	TARGET sandbox_libv_log
	SOURCE ${PATH_SNB}/libv_log_main.cpp
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET sandbox_libv_reflection
	SOURCE ${PATH_SNB}/libv_reflection_main.cpp
	LINK   ${USE_STATIC_LINK}
)

create_executable(
	TARGET sandbox_libv_reflection2
	SOURCE ${PATH_SNB}/libv_reflection2_main.cpp
	LINK   ${USE_STATIC_LINK}
)

create_executable(
	TARGET sandbox_libv_serialize
	SOURCE ${PATH_SNB}/libv_serialize_main.cpp
	LINK   ${USE_STATIC_LINK}
)

create_executable(
	TARGET sandbox_libv_serialize_cereal
	SOURCE ${PATH_SNB}/libv_serialize_cereal_main.cpp
	LINK   ${USE_STATIC_LINK} ext_boost
)

create_executable(
	TARGET sandbox_libv_sig
	SOURCE ${PATH_SNB}/libv_sig_main.cpp
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET sandbox_libv_vm4imp
	SOURCE ${PATH_SNB}/libv_vm4imp_main.cpp
	LINK   ${USE_STATIC_LINK} libv_log libv_vm4imp
)

end_group()

# Tests --------------------------------------------------------------------------------------------

create_object(
	TARGET obj_test_runner
	SOURCE ${PATH_TST}/runner.cpp
	LINK   ext_catch
)

create_group(GROUP_UNIT_TEST)

create_executable(
	TARGET test_libv
	SOURCE ${PATH_TST}/libv/libv/*.cpp
	OBJECT obj_test_runner
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET test_libv_ecs
	SOURCE ${PATH_TST}/libv/ecs/*.cpp
	OBJECT obj_test_runner
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET test_libv_frame
	SOURCE ${PATH_TST}/libv/frame/*.cpp
	OBJECT obj_test_runner
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET test_libv_gl
	SOURCE ${PATH_TST}/libv/gl/*.cpp
	OBJECT obj_test_runner
	LINK   ${USE_STATIC_LINK} libv_log libv_gl
)

create_executable(
	TARGET test_libv_log
	SOURCE ${PATH_TST}/libv/log/*.cpp
	OBJECT obj_test_runner
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET test_libv_parse
	SOURCE ${PATH_TST}/libv/parse/*.cpp
	OBJECT obj_test_runner
	LINK   ${USE_STATIC_LINK} libv_log
)

create_executable(
	TARGET test_libv_sig
	SOURCE ${PATH_TST}/libv/sig/*.cpp
	OBJECT obj_test_runner
	LINK   ${USE_STATIC_LINK} libv_log
)

end_group()

# groups -------------------------------------------------------------------------------------------

add_custom_target(group_library DEPENDS ${GROUP_LIBRARY})
add_custom_target(group_sandbox DEPENDS ${GROUP_SANDBOX})
add_custom_target(group_unit_test DEPENDS ${GROUP_UNIT_TEST})
add_custom_target(group_externals DEPENDS ${GROUP_EXTERNAL_PROJECT})
add_custom_target(ext DEPENDS group_externals)

