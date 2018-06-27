#

cmake_minimum_required(VERSION 3.7)
project(libv)

include(ExternalProject)
include(cmake/wish_create.cmake)
include(cmake/wish_git.cmake)
include(cmake/wish_util.cmake)
include(cmake/wish_warning.cmake)

# --------------------------------------------------------------------------------------------------

set(CMAKE_VERBOSE_MAKEFILE off)
#set(CMAKE_BUILD_TYPE "DEBUG")
#set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_BUILD_TYPE "DEV")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor count: ${WISH_PROCESSOR_COUNT}")
message(STATUS "CXX compiler id: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX compiler version: ${CMAKE_CXX_COMPILER_VERSION}")

# TODO ---------------------------------------------------------------------------------------------

# TODO P3: Use the correct OpenGL linking cmake build-in options
# TODO P4: Think about and try to remove PATH_EXT_IDE
# ... see in Iris CMakeLists.txt

# Paths --------------------------------------------------------------------------------------------

set(PATH_EXT_IDE ext)
set(PATH_EXT     ${CMAKE_SOURCE_DIR}/ext)
set(PATH_EXT_SRC ${CMAKE_SOURCE_DIR}/ext_src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

string(LENGTH ${CMAKE_SOURCE_DIR}_ LIBV_SHORT_PATH_CUTOFF)

# Warning flags ------------------------------------------------------------------------------------

wish_warning(
	MSVC /Wall

	Clang -Weverything
	Clang -Wmissing-override
	Clang -Wno-comment
	Clang -Wconversion

	GNU -Wall
	GNU -Wextra
	GNU -Wpedantic
	GNU -Wcast-qual
	GNU -Wdelete-non-virtual-dtor
	GNU -Wdouble-promotion
	GNU -Wduplicated-cond
	GNU -Wlogical-op
	GNU -Wnon-virtual-dtor
	GNU -Wnull-dereference
	GNU -Wold-style-cast
	GNU -Wsuggest-override
	GNU -Wundef
	GNU -Warray-bounds

	# Enablement in progress:
	# GNU -Wuseless-cast # in vec_base_t has some false positive issue with this one, but otherwise clear
	# GNU -Wconversion # 130+, most of it is in vec and the rest is in gl
	# GNU -Wfloat-equal # in tests 40+ false positive

	# Only enable time-to-time to check on what is going on:
	# GNU -Winline
	# GNU -Wsuggest-final-methods # in signal 50+ function
	# GNU -Wsuggest-final-types # in signal 1 type

	GNU VERSION_GREATER 7.0 -Wduplicated-branches
	GNU VERSION_GREATER 7.0 -Wrestrict
	GNU VERSION_GREATER 7.0 -Wshadow-compatible-local
	GNU VERSION_GREATER 8.0 -Wcast-align=strict

	# Warnings that I don't care about
	GNU -Wno-comment
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
		# Sanitizers are current only available on unix
		# add_compile_options(-fsanitize=address)
		# add_compile_options(-fsanitize=kernel-address)
		# add_compile_options(-fsanitize=leak)
		# add_compile_options(-fsanitize=thread)
		# add_compile_options(-fsanitize=undefined)
		# link_libraries(ubsan)
		# link_libraries(asan)
	endif()
endif()

# Definitions --------------------------------------------------------------------------------------

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
	wish_static_link_std()

elseif(CMAKE_BUILD_TYPE STREQUAL "DEV")
	add_compile_options(-Ofast)

else()
	message(WARNING "Failed to identify [${CMAKE_BUILD_TYPE}] as a build type")
endif()

add_compile_options(-m64)
add_compile_options(-std=c++17)

# Macro - Assert
add_definitions(-DNDEBUG)
# Macro - Git
add_definitions(-DGIT_BRANCH="${WISH_GIT_BRANCH}")
add_definitions(-DGIT_COMMIT_HASH="${WISH_GIT_COMMIT_HASH}")
# Macro - Libv
add_definitions(-DLIBV_SHORT_PATH_CUTOFF=${LIBV_SHORT_PATH_CUTOFF})

# Includes Directories -----------------------------------------------------------------------------

include_directories(include)
include_directories(src)

# === Package ======================================================================================

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)

# === External =====================================================================================

wish_create_group(GROUP_EXTERNAL_PROJECT) # External -----------------------------------------------

wish_create_external(
	NAME assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v4.1.0
	CMAKE_ARGS
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
		-DASSIMP_BUILD_AMF_IMPORTER=OFF
		-DASSIMP_BUILD_ASE_IMPORTER=OFF
		-DASSIMP_BUILD_ASSBIN_IMPORTER=OFF
		-DASSIMP_BUILD_ASSXML_IMPORTER=OFF
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
		-DASSIMP_BUILD_IRRMESH_IMPORTER=OFF
		-DASSIMP_BUILD_IRR_IMPORTER=OFF
		-DASSIMP_BUILD_LWO_IMPORTER=OFF
		-DASSIMP_BUILD_LWS_IMPORTER=OFF
		-DASSIMP_BUILD_MD2_IMPORTER=OFF
		-DASSIMP_BUILD_MD3_IMPORTER=OFF
		-DASSIMP_BUILD_MD5_IMPORTER=OFF
		-DASSIMP_BUILD_MDC_IMPORTER=OFF
		-DASSIMP_BUILD_MDL_IMPORTER=OFF
		-DASSIMP_BUILD_MMD_IMPORTER=OFF
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
		-DASSIMP_BUILD_X3D_IMPORTER=OFF
		-DASSIMP_BUILD_XGL_IMPORTER=OFF
		-DASSIMP_BUILD_X_IMPORTER=OFF
	LINK assimp IrrXML zlibstatic
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
wish_create_external(
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
		-j${WISH_PROCESSOR_COUNT}
	BUILD_IN_SOURCE 1
	INSTALL_COMMAND ""
	#LINK system stacktrace
)

wish_create_external(
	NAME catch
	GIT_REPOSITORY https://github.com/philsquared/Catch.git
	GIT_TAG v1.5.6
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/catch/include/catch/
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/catch/src/get_catch/include/ ${PATH_EXT}/catch/include/catch/
)

wish_create_external(
	NAME cereal
	GIT_REPOSITORY https://github.com/USCiLab/cereal.git
	GIT_TAG v1.2.2
	CMAKE_ARGS
		-DJUST_INSTALL_CEREAL=ON
	DEFINE CEREAL_XML_STRING_VALUE="root"
)

wish_create_external(
	NAME clara
	GIT_REPOSITORY https://github.com/philsquared/Clara.git
	GIT_TAG v1.1.4
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/clara/include/clara/
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/clara/src/get_clara/single_include ${PATH_EXT}/clara/include/clara/
)

wish_create_external(
	NAME efsw
	GIT_REPOSITORY https://github.com/mnafees/efsw.git
	GIT_TAG 9bc94939f154bc756def4e9788d83e1f164300f3
	# TODO P3: this ext_efsw install command looks off
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/efsw/include/
		COMMAND mkdir -p ${PATH_EXT}/efsw/lib/
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/efsw/src/get_efsw/include/ ${PATH_EXT}/efsw/include/
		COMMAND cp -r -f ${PATH_EXT_SRC}/efsw/src/get_efsw-build/libefsw.a ${PATH_EXT}/efsw/lib/
	LINK efsw
)

wish_create_external(
	NAME fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 5.0.0
	CMAKE_ARGS
		-DFMT_DOC=OFF
		-DFMT_TEST=OFF
	LINK fmt
)

wish_create_external(
	NAME freetype
	GIT_REPOSITORY http://git.sv.nongnu.org/r/freetype/freetype2.git
	GIT_TAG VER-2-5-5
	LINK freetype
	INCLUDE_DIR include/freetype2
)

wish_create_external(
	NAME glew
	GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
	GIT_TAG glew-cmake-1.11.0
	LINK glew
	DEFINE GLEW_STATIC
)

wish_create_external(
	NAME glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.2.1
	CMAKE_ARGS
		-DGLFW_BUILD_EXAMPLES=OFF
		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_DOCS=OFF
	LINK glfw3 gdi32
)

wish_create_external(
	NAME gli
	GIT_REPOSITORY https://github.com/g-truc/gli.git
	GIT_TAG 8e43030b3e12bb58a4663d85adc5c752f89099c0
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/gli/include/gli/
		COMMAND cp -r -f ${PATH_EXT_SRC}/gli/src/get_gli/gli/ ${PATH_EXT}/gli/include/
)

wish_create_external(
	NAME glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.9.0
	SKIP_BUILD
	DEFINE GLM_ENABLE_EXPERIMENTAL
	DEFINE GLM_FORCE_RADIANS
)

wish_create_external(
	NAME lua
	GIT_REPOSITORY https://github.com/LuaDist/lua.git
	GIT_TAG 5.3.2
	CMAKE_ARGS
		-DBUILD_SHARED_LIBS=OFF
	LINK lua
)

wish_create_external(
	NAME net_ts
	GIT_REPOSITORY https://github.com/chriskohlhoff/networking-ts-impl.git
	GIT_TAG 3524b4408d26a67af683bfd2aad6b0b6b5684b36
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/net_ts/
		COMMAND cp -r -f ${PATH_EXT_SRC}/net_ts/src/get_net_ts/include/ ${PATH_EXT}/net_ts/
)

wish_create_external(
	NAME range
	GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
	GIT_TAG 0.3.5
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/range/
		COMMAND cp -r -f ${PATH_EXT_SRC}/range/src/get_range/include/ ${PATH_EXT}/range/
)

wish_create_external(
	NAME soil
	GIT_REPOSITORY https://github.com/xosdy/soil.git
	GIT_TAG master
	LINK soil
)

wish_create_external(
	NAME sol
	GIT_REPOSITORY https://github.com/ThePhD/sol2.git
	GIT_TAG v2.19.5
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/sol/include/sol/
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/sol/src/get_sol/sol/ ${PATH_EXT}/sol/include/sol/
	DEFINE SOL_USE_BOOST
)

wish_create_external(
	NAME sml
	GIT_REPOSITORY https://github.com/boost-experimental/sml.git
	GIT_TAG 8968695b40ce1c9dad7ba78d8220f5b7bf458d08
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/sml/
		COMMAND cp -r -f ${PATH_EXT_SRC}/sml/src/get_sml/include/ ${PATH_EXT}/sml/
)

wish_create_external(
	NAME utf8cpp
	GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
	GIT_TAG 1537543999978d3a0464560a9a940d6140c6ba59
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/utf8cpp/include/utf8cpp/
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/utf8cpp/src/get_utf8cpp/source ${PATH_EXT}/utf8cpp/include/utf8cpp
)

wish_end_group()

# === Targets ======================================================================================

wish_create_group(GROUP_LIBRARY) # Libraries -------------------------------------------------------

wish_create_library(
	TARGET libv_container INTERFACE
)

wish_create_library(
	TARGET libv_ecs INTERFACE
	LINK   ext_boost ext_range
)

wish_create_library(
	TARGET libv_frame STATIC
	SOURCE src/libv/frame/*.cpp
	LINK   libv_log libv_math ext_boost ext_glfw ext_utf8cpp ${OPENGL_LIBRARIES} Threads::Threads
)

wish_create_library(
	TARGET libv_gl STATIC
	SOURCE src/libv/gl/*.cpp
	LINK   libv_log libv_math ext_glew ext_gli ext_soil ${OPENGL_LIBRARIES}
)

wish_create_library(
	TARGET libv_glr INTERFACE
	LINK   ext_boost
)

wish_create_library(
	TARGET libv_log INTERFACE
	LINK   ext_fmt Threads::Threads
)

wish_create_library(
	TARGET libv_lua INTERFACE
	LINK   ext_sol ext_lua
)

wish_create_library(
	TARGET libv_math INTERFACE
	LINK   ext_glm # Remove (This will only happen with a vec concept)
	LINK   ext_range # For concepts
)

wish_create_library(
	TARGET libv_meta INTERFACE
	LINK   ext_boost
)

wish_create_library(
	TARGET libv_parse INTERFACE
	LINK   ext_boost
	LINK   libv_math # Remove?
)

wish_create_library(
	TARGET libv_range INTERFACE
	LINK   ext_boost ext_range
)

wish_create_library(
	TARGET libv_serialization INTERFACE
	LINK   ext_cereal
	LINK   ext_boost # Remove
)

wish_create_library(
	TARGET libv_sig STATIC
	SOURCE src/libv/sig/*.cpp
)

wish_create_library(
	TARGET libv_thread INTERFACE
	LINK   Threads::Threads
)

wish_create_library(
	TARGET libv_utility INTERFACE
	LINK   ext_efsw stdc++fs Threads::Threads
)

wish_create_library(
	TARGET libv_vm4 INTERFACE
	LINK   libv_math libv_serialization ext_boost
)

wish_create_library(
	TARGET libv_vm4imp STATIC
	SOURCE src/libv/vm4imp/*.cpp
	LINK   libv_log libv_vm4 ext_assimp stdc++fs
)

wish_end_group()
wish_create_group(GROUP_SANDBOX) # Sandboxes -------------------------------------------------------

wish_create_executable(
	TARGET sandbox_libv_libv
	SOURCE sandbox/libv_main.cpp
	LINK   libv_log ext_boost
)

wish_create_executable(
	TARGET sandbox_libv_ecs
	SOURCE sandbox/libv_ecs_main.cpp
	LINK   libv_log libv_ecs
)

wish_create_executable(
	TARGET sandbox_libv_frame
	SOURCE sandbox/libv_frame_main.cpp
	LINK   libv_log libv_frame
)

wish_create_executable(
	TARGET sandbox_libv_gl
	SOURCE sandbox/libv_gl_main.cpp
	LINK   libv_log libv_gl ext_glfw stdc++fs
)

wish_create_executable(
	TARGET sandbox_libv_gl2
	SOURCE sandbox/libv_gl_main2.cpp
	LINK   libv_log libv_gl ext_glfw stdc++fs
)

wish_create_executable(
	TARGET sandbox_libv_gl3
	SOURCE sandbox/libv_gl_main3.cpp
	LINK   libv_log libv_gl ext_boost ext_glfw stdc++fs
)

wish_create_executable(
	TARGET sandbox_libv_log
	SOURCE sandbox/libv_log_main.cpp
	LINK   libv_log
)

wish_create_executable(
	TARGET sandbox_libv_reflection
	SOURCE sandbox/libv_reflection_main.cpp
	LINK   libv_serialization
)

wish_create_executable(
	TARGET sandbox_libv_reflection2
	SOURCE sandbox/libv_reflection2_main.cpp
	LINK   libv_serialization
)

wish_create_executable(
	TARGET sandbox_libv_serialize
	SOURCE sandbox/libv_serialize_main.cpp
	LINK   libv_serialization
)

wish_create_executable(
	TARGET sandbox_libv_serialize_cereal
	SOURCE sandbox/libv_serialize_cereal_main.cpp
	LINK   ext_boost ext_cereal
)

wish_create_executable(
	TARGET sandbox_libv_sig
	SOURCE sandbox/libv_sig_main.cpp
	LINK   libv_log
)

wish_create_executable(
	TARGET sandbox_libv_sml
	SOURCE sandbox/libv_sml_main.cpp
	LINK   libv_log ext_sml
)

wish_create_executable(
	TARGET sandbox_libv_vm4imp
	SOURCE sandbox/libv_vm4imp_main.cpp
	LINK   libv_log libv_vm4imp
)

wish_end_group()

# Tests --------------------------------------------------------------------------------------------

# TODO P5: This should be an object library, but ran into troubles with target_link_libraries against object library
wish_create_library(
	TARGET obj_test_runner STATIC
	SOURCE test/runner.cpp
	LINK   libv_log ext_catch ext_fmt ext_boost
)

wish_create_group(GROUP_UNIT_TEST)

wish_create_executable(
	TARGET test_libv_ecs
	SOURCE test/libv/ecs/*.cpp
	LINK   obj_test_runner libv_range
)

wish_create_executable(
	TARGET test_libv_frame
	SOURCE test/libv/frame/*.cpp
	LINK   obj_test_runner libv_frame
)

wish_create_executable(
	TARGET test_libv_log
	SOURCE test/libv/log/*.cpp
	LINK   obj_test_runner
)

wish_create_executable(
	TARGET test_libv_math
	SOURCE test/libv/math/*.cpp
	LINK   obj_test_runner libv_math
)

wish_create_executable(
	TARGET test_libv_parse
	SOURCE test/libv/parse/*.cpp
	LINK   obj_test_runner libv_parse
)

wish_create_executable(
	TARGET test_libv_range
	SOURCE test/libv/range/*.cpp
	LINK   obj_test_runner libv_range
)

wish_create_executable(
	TARGET test_libv_sig
	SOURCE test/libv/sig/*.cpp
	LINK   obj_test_runner libv_sig
)

wish_create_executable(
	TARGET test_libv_utility
	SOURCE test/libv/utility/*.cpp
	LINK   obj_test_runner libv_utility
)

wish_end_group()

# groups -------------------------------------------------------------------------------------------

add_custom_target(group_library DEPENDS ${GROUP_LIBRARY})
add_custom_target(group_sandbox DEPENDS ${GROUP_SANDBOX})
add_custom_target(group_unit_test DEPENDS ${GROUP_UNIT_TEST})
add_custom_target(group_externals DEPENDS ${GROUP_EXTERNAL_PROJECT})

add_custom_target(ext DEPENDS group_externals)
add_custom_target(tests DEPENDS group_unit_test)

wish_create_ide_target()
