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

# TODO P2: Put macro definitions per target
# TODO P3: Use the correct OpenGL linking cmake build-in options
# TODO P4: There is a way to include thread this via cmake built-in options
# TODO P4: Instead of USE_STATIC_LINK use cmake default if possible
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

create_group(GROUP_EXTERNAL_PROJECT) # External ----------------------------------------------------

create_external(
	NAME assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v3.3.1
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
	LINK assimp zlibstatic
)

set(Boost_Version 1.66.0)
set(TOOLSET)
string(REPLACE "." "_" Boost_Version_Underscore ${Boost_Version})
message(STATUS "Boost version: " ${Boost_Version})
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(TOOLSET msvc)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(TOOLSET clang)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(TOOLSET gcc)
endif()
create_external(
	NAME boost
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
	INSTALL_COMMAND ""
	#LINK system stacktrace
)

create_external(
	NAME catch
	GIT_REPOSITORY https://github.com/philsquared/Catch.git
	GIT_TAG v1.5.6
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/catch/
		COMMAND cp -r -f ${PATH_EXT_SRC}/catch/src/ext_catch/include/ ${PATH_EXT}/catch/
)

create_external(
	NAME cereal
	GIT_REPOSITORY https://github.com/USCiLab/cereal.git
	GIT_TAG v1.2.2
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/cereal
		-DJUST_INSTALL_CEREAL=ON
	DEFINE CEREAL_XML_STRING_VALUE="root"
)

create_external(
	NAME clara
	GIT_REPOSITORY https://github.com/philsquared/Clara.git
	GIT_TAG 20b5c3abdc0637f3e73479340b63d368f9e0f2ce
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/clara/include/clara
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/clara/src/ext_clara/single_include ${PATH_EXT}/clara/include/clara
)

create_external(
	NAME efsw
	GIT_REPOSITORY https://github.com/mnafees/efsw.git
	GIT_TAG 9bc94939f154bc756def4e9788d83e1f164300f3
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/efsw
	# TODO P3: this ext_efsw install command looks off
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/efsw/include
		COMMAND mkdir -p ${PATH_EXT}/efsw/lib
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/efsw/src/ext_efsw/include/ ${PATH_EXT}/efsw/include/
		COMMAND cp -r -f ${PATH_EXT_SRC}/efsw/src/ext_efsw-build/libefsw.a ${PATH_EXT}/efsw/lib/
	LINK efsw
)

create_external(
	NAME fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG bef89db6e7d10499a91affca7a933f264c7956d5
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/fmt
		-DFMT_DOC=OFF
		-DFMT_TEST=OFF
	LINK fmt
)

create_external(
	NAME freetype
	GIT_REPOSITORY http://git.sv.nongnu.org/r/freetype/freetype2.git
	GIT_TAG VER-2-5-5
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/freetype
	LINK freetype
	INCLUDEDIR include/freetype2
)

create_external(
	NAME glew
	GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
	GIT_TAG glew-cmake-1.11.0
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glew
	LINK glew
	DEFINE GLEW_STATIC
)

create_external(
	NAME glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.2.1
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glfw
		-DGLFW_BUILD_EXAMPLES=OFF
		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_DOCS=OFF
	LINK glfw3 gdi32
)

create_external(
	NAME gli
	GIT_REPOSITORY https://github.com/g-truc/gli.git
	GIT_TAG 0.8.2.0
	BUILD_COMMAND "" # gli is header only, skip tests
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/gli
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/gli/include/gli/
		COMMAND cp -r -f ${PATH_EXT_SRC}/gli/src/ext_gli/gli/ ${PATH_EXT}/gli/include/
)

create_external(
	NAME glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.8.5
	BUILD_COMMAND "" # glm is header only, skip tests
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glm
	DEFINE GLM_ENABLE_EXPERIMENTAL
	DEFINE GLM_FORCE_RADIANS
)

create_external(
	NAME lua
	GIT_REPOSITORY https://github.com/LuaDist/lua.git
	GIT_TAG 5.3.2
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/lua
		-DBUILD_SHARED_LIBS=OFF
	LINK lua
)

create_external(
	NAME net_ts
	GIT_REPOSITORY https://github.com/chriskohlhoff/networking-ts-impl.git
	GIT_TAG 6bf74be013e104e2893de245bc3379a6f3f20d54
	CONFIGURE_COMMAND ""
	BUILD_COMMAND "" # net_ts is header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/net_ts/
		COMMAND cp -r -f ${PATH_EXT_SRC}/net_ts/src/ext_net_ts/include/ ${PATH_EXT}/net_ts/
)

create_external(
	NAME range
	GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
	GIT_TAG 0ed88b12844792a83d2d0919ef49a341fdca28d1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND "" # range is header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/range/
		COMMAND cp -r -f ${PATH_EXT_SRC}/range/src/ext_range/include/ ${PATH_EXT}/range/
)

create_external(
	NAME soil
	GIT_REPOSITORY https://github.com/xosdy/soil.git
	GIT_TAG master
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/soil
	LINK soil
)

create_external(
	NAME sol
	GIT_REPOSITORY https://github.com/ThePhD/sol2.git
	GIT_TAG 16fc7d87ff0b9c2fc18268a420e0b058a67060a1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/sol/include/sol
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/sol/src/ext_sol/sol/ ${PATH_EXT}/sol/include/sol/
	DEFINE SOL_USE_BOOST
)

create_external(
	NAME utf8cpp
	GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
	GIT_TAG f029fcc2fbc7cd979925f198f7e6ca8170d45000
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/utf8cpp/include/utf8cpp
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/utf8cpp/src/ext_utf8cpp/source ${PATH_EXT}/utf8cpp/include/utf8cpp
)

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
	LINK   ${USE_STATIC_LINK} libv_log libv_math ext_glew ext_gli ext_soil ${OPENGL_LIBRARIES}
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
	LINK   ext_glm # Remove
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

