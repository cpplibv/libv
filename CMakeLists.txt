#

cmake_minimum_required (VERSION 3.1)
project (libv)
include(ExternalProject)

include(cmake/util.cmake)

# --------------------------------------------------------------------------------------------------

set(CMAKE_VERBOSE_MAKEFILE off)
#set(CMAKE_BUILD_TYPE "DEBUG")
set(CMAKE_BUILD_TYPE "RELEASE")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor count: ${PROCESSOR_COUNT}")
message(STATUS "CXX compiler id: ${CMAKE_CXX_COMPILER_ID}")

# --------------------------------------------------------------------------------------------------

# TODO P2: use cmake copy function instead of "cp"
# TODO P2: flag or build options to turn on/off sanitizes
# TODO P2: debug symbol automated turn on/off
# TODO P3: test clang build
# TODO P5: test msvc build

# --------------------------------------------------------------------------------------------------

#Paths
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

# --------------------------------------------------------------------------------------------------

#Variables
set(TOOLSET)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

	add_compile_options(/Wall)
	set(TOOLSET msvc)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

	add_compile_options(-Weverything -Wmissing-override -Wno-comment
#		-Wconversion
		)
	set(TOOLSET clang)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")

	add_compile_options(-Wall -Wextra -Wpedantic -Wno-comment
#		-Wcast-qual
##		-Wconversion
#		-Wdelete-non-virtual-dtor
#		-Wduplicated-cond
#		-Wfloat-equal
#		-Winline
#		-Wnon-virtual-dtor
#		-Wold-style-cast
##		-Wshadow=compatible-local
#		-Wsuggest-final-methods
#		-Wsuggest-final-types
#		-Wundef
#		-Wuseless-cast
		)

	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0)
		add_compile_options(-Wsuggest-override)
	endif()

	if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")

		add_compile_options(-fno-omit-frame-pointer)
		# Sanitizers current only available on unix
		#add_compile_options(-fsanitize=address)
		#add_compile_options(-fsanitize=kernel-address)
		#add_compile_options(-fsanitize=leak)
		#add_compile_options(-fsanitize=thread)
		#add_compile_options(-fsanitize=undefined)
		#link_libraries(ubsan)
		#link_libraries(asan)
	endif()
	set(TOOLSET gcc)

endif()

string(LENGTH ${PATH_PRJ}_ LIBV_SHORT_PATH_CUTOFF)

# --------------------------------------------------------------------------------------------------

#Definitions
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	add_definitions(-O0)
	add_definitions(-ggdb3)
elseif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
	add_definitions(-Ofast)
else()
	message(WARNING "Failed to identify [${CMAKE_BUILD_TYPE}] as a build type")
endif()

add_definitions(-m64)
add_definitions(-std=c++17)

#Macro - Ext
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLM_FORCE_RADIANS)
#Macro - GLM
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
#Macro - Libv
add_definitions(-DLIBV_USE_GLM_BRIDGE)
add_definitions(-DLIBV_SHORT_PATH_CUTOFF=${LIBV_SHORT_PATH_CUTOFF})
#Macro - Assert
add_definitions(-DNDEBUG)

# --------------------------------------------------------------------------------------------------

if(NOT WIN32)
	link_libraries(pthread)
endif()

# --------------------------------------------------------------------------------------------------
# TODO P3: find openGL

if(WIN32)
	set(LIB_OPENGL opengl32)
	set(LIB_GLU glu32)
else()
	set(LIB_OPENGL GL)
	set(LIB_GLU GLU)
endif()

# --------------------------------------------------------------------------------------------------

set(EXTERNAL_PROJECT_LIST)

list(APPEND EXTERNAL_PROJECT_LIST ext_utf8cpp)
ExternalProject_Add(ext_utf8cpp
	GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
	GIT_TAG f029fcc2fbc7cd979925f198f7e6ca8170d45000
	PREFIX ${PATH_EXT_SRC}/utf8cpp
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/utf8cpp/include/utf8cpp
		COMMAND cp -r -f -T ${PATH_EXT_SRC}/utf8cpp/src/ext_utf8cpp/source ${PATH_EXT}/utf8cpp/include/utf8cpp
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_catch)
ExternalProject_Add(ext_catch
	GIT_REPOSITORY https://github.com/philsquared/Catch.git
	GIT_TAG v1.5.6
	PREFIX ${PATH_EXT_SRC}/catch
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  # header only, skip tests
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/catch/
		COMMAND cp -r -f ${PATH_EXT_SRC}/catch/src/ext_catch/include/ ${PATH_EXT}/catch/
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_glfw)
ExternalProject_Add(ext_glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.2.1
	PREFIX ${PATH_EXT_SRC}/glfw
	INSTALL_DIR ${PATH_EXT}/glfw
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glfw
		-DGLFW_BUILD_EXAMPLES=OFF
		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_DOCS=OFF
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_glew)
ExternalProject_Add(ext_glew
	GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
	GIT_TAG glew-cmake-1.11.0
	PREFIX ${PATH_EXT_SRC}/glew
	INSTALL_DIR ${PATH_EXT}/glew
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glew
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_fmt)
ExternalProject_Add(ext_fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 94d387c
	PREFIX ${PATH_EXT_SRC}/fmt
	INSTALL_DIR ${PATH_EXT}/fmt
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/fmt
		-DFMT_DOC=OFF
		-DFMT_TEST=OFF
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_glm)
ExternalProject_Add(ext_glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.8
	BUILD_COMMAND "" #glm is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/glm
	INSTALL_DIR ${PATH_EXT}/glm
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glm
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_gli)
ExternalProject_Add(ext_gli
	GIT_REPOSITORY https://github.com/g-truc/gli.git
	GIT_TAG 0.8.1.0
	BUILD_COMMAND "" #gli is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/gli
	INSTALL_DIR ${PATH_EXT}/gli
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/gli
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/gli/include/gli/
		COMMAND cp -r -f ${PATH_EXT_SRC}/gli/src/ext_gli/gli/ ${PATH_EXT}/gli/include/
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_range)
ExternalProject_Add(ext_range
	GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
	GIT_TAG 0ed88b12844792a83d2d0919ef49a341fdca28d1
	CONFIGURE_COMMAND ""
	BUILD_COMMAND "" #range is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/range
	INSTALL_COMMAND
		COMMAND mkdir -p ${PATH_EXT}/range/
		COMMAND cp -r -f ${PATH_EXT_SRC}/range/src/ext_range/include/ ${PATH_EXT}/range/
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_freetype)
ExternalProject_Add(ext_freetype
	GIT_REPOSITORY http://git.sv.nongnu.org/r/freetype/freetype2.git
	GIT_TAG VER-2-5-5
	PREFIX ${PATH_EXT_SRC}/freetype
	INSTALL_DIR ${PATH_EXT}/freetype
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/freetype
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_assimp)
ExternalProject_Add(ext_assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v3.3
	PREFIX ${PATH_EXT_SRC}/assimp
	INSTALL_DIR ${PATH_EXT}/assimp
	CMAKE_ARGS
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/assimp
		-DASSIMP_BUILD_ASSIMP_TOOLS=OFF
		-DASSIMP_BUILD_TESTS=OFF
		-DBUILD_SHARED_LIBS=OFF
		-DASSIMP_NO_EXPORT=ON
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
		-DASSIMP_BUILD_COLLADA_IMPORTER=OFF
		-DASSIMP_BUILD_CSM_IMPORTER=OFF
		-DASSIMP_BUILD_DXF_IMPORTER=OFF
		-DASSIMP_BUILD_FBX_IMPORTER=OFF
		-DASSIMP_BUILD_GLTF_IMPORTER=OFF
		-DASSIMP_BUILD_HMP_IMPORTER=OFF
		-DASSIMP_BUILD_IFC_IMPORTER=OFF
		# -DASSIMP_BUILD_IRR_IMPORTER=OFF re-disable if assimp provide build options for correct disable
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
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_boost)
set(Boost_Version 1.61.0)
string(REPLACE "." "_" Boost_Version_Underscore ${Boost_Version})

set(ext_boost_bootstrap_command)
if(NOT WIN32)
	set(ext_boost_bootstrap_command ./bootstrap.sh)
	set(ext_boost_b2_command ./b2 --ignore-site-config)
else()
	set(ext_boost_bootstrap_command cmd.exe //c bootstrap.bat ${TOOLSET})
	set(ext_boost_b2_command cmd.exe //c b2.exe)
endif()

message(STATUS "Boost version: " ${Boost_Version})
ExternalProject_Add(ext_boost
	PREFIX ${PATH_EXT_SRC}/boost
	URL http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.zip
	URL_MD5 015ae4afa6f3e597232bfe1dab949ace
	CONFIGURE_COMMAND ${ext_boost_bootstrap_command}
	BUILD_COMMAND ${ext_boost_b2_command} install
		toolset=${TOOLSET}
		variant=release
		link=static
		runtime-link=static
		threading=multi
		--address-model=64
		--prefix=${PATH_EXT}/boost
		--layout=tagged
		--with-serialization
		--with-system
		--with-filesystem
		-j${PROCESSOR_COUNT}
	BUILD_IN_SOURCE 1
	EXCLUDE_FROM_ALL 1
	INSTALL_DIR ${PATH_EXT}/boost
	INSTALL_COMMAND ""
)

# --------------------------------------------------------------------------------------------------

#Includes
include_directories (${PATH_INC})
include_directories (${PATH_SRC})

#Externals
include_directories(SYSTEM ${PATH_EXT}/assimp/include)
include_directories(SYSTEM ${PATH_EXT}/boost/include)
include_directories(SYSTEM ${PATH_EXT}/catch/include)
include_directories(SYSTEM ${PATH_EXT}/fmt/include)
include_directories(SYSTEM ${PATH_EXT}/freetype/include/freetype2)
include_directories(SYSTEM ${PATH_EXT}/glew/include)
include_directories(SYSTEM ${PATH_EXT}/glfw/include)
include_directories(SYSTEM ${PATH_EXT}/gli/include)
include_directories(SYSTEM ${PATH_EXT}/glm/include)
include_directories(SYSTEM ${PATH_EXT}/range/include)
include_directories(SYSTEM ${PATH_EXT}/utf8cpp/include)

link_directories(${PATH_EXT}/assimp/lib)
link_directories(${PATH_EXT}/boost/lib)
link_directories(${PATH_EXT}/fmt/lib)
link_directories(${PATH_EXT}/freetype/lib)
link_directories(${PATH_EXT}/glew/lib)
link_directories(${PATH_EXT}/glfw/lib)

# Sources ------------------------------------------------------------------------------------------

#Sources
file(GLOB_RECURSE libv_src        RELATIVE ${PATH_PRJ} ${PATH_SRC}/libv/*.cpp)
file(GLOB_RECURSE vframe_src      RELATIVE ${PATH_PRJ} ${PATH_SRC}/frame/*.cpp)
file(GLOB_RECURSE vgl_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/gl/*.cpp)
file(GLOB_RECURSE vlog_src        RELATIVE ${PATH_PRJ} ${PATH_SRC}/log/*.cpp)
file(GLOB_RECURSE vm3_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/vm3/*.cpp)
file(GLOB_RECURSE vm3imp_src      RELATIVE ${PATH_PRJ} ${PATH_SRC}/vm3imp/*.cpp)
file(GLOB_RECURSE vsig_src        RELATIVE ${PATH_PRJ} ${PATH_SRC}/sig/*.cpp)
file(GLOB_RECURSE vui_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/ui/*.cpp)

#Test Sources
file(GLOB_RECURSE libv_test_src   RELATIVE ${PATH_PRJ} ${PATH_TST}/libv/*.cpp)
file(GLOB_RECURSE vgl_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/gl/*.cpp)
file(GLOB_RECURSE vlog_test_src   RELATIVE ${PATH_PRJ} ${PATH_TST}/log/*.cpp)
file(GLOB_RECURSE vm3_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/vm3/*.cpp)
file(GLOB_RECURSE vm3imp_test_src RELATIVE ${PATH_PRJ} ${PATH_TST}/vm3imp/*.cpp)
file(GLOB_RECURSE vsig_test_src   RELATIVE ${PATH_PRJ} ${PATH_TST}/sig/*.cpp)
file(GLOB_RECURSE vparse_test_src RELATIVE ${PATH_PRJ} ${PATH_TST}/parse/*.cpp)
file(GLOB_RECURSE vui_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/ui/*.cpp)

# Targets ------------------------------------------------------------------------------------------

#Libraries
add_library(libv   STATIC ${libv_src})
add_library(vframe STATIC ${vframe_src})
add_library(vgl    STATIC ${vgl_src})
add_library(vlog   STATIC ${vlog_src})
add_library(vm3    STATIC ${vm3_src})
add_library(vm3imp STATIC ${vm3imp_src})
add_library(vsig   STATIC ${vsig_src})
add_library(vui    STATIC ${vui_src})

#Executables - Tests
add_executable(libv_test   ${libv_test_src})
add_executable(vgl_test    ${vgl_test_src})
add_executable(vlog_test   ${vlog_test_src})
add_executable(vm3_test    ${vm3_test_src})
add_executable(vm3imp_test ${vm3imp_test_src})
add_executable(vparse_test ${vparse_test_src})
add_executable(vsig_test   ${vsig_test_src})
add_executable(vui_test    ${vui_test_src})

#Executables - Sandbox
add_executable(libv_main   ${PATH_SNB}/libv_main.cpp)
add_executable(vframe_main ${PATH_SNB}/vframe_main.cpp)
add_executable(vgl_main    ${PATH_SNB}/vgl_main.cpp)
add_executable(vgl_main2   ${PATH_SNB}/vgl_main2.cpp)
add_executable(vlog_main   ${PATH_SNB}/vlog_main.cpp)
add_executable(vm3_main    ${PATH_SNB}/vm3_main.cpp)
add_executable(vm3imp_main ${PATH_SNB}/vm3imp_main.cpp)
add_executable(vsig_main   ${PATH_SNB}/vsig_main.cpp)
add_executable(vui_main    ${PATH_SNB}/vui_main.cpp)
add_executable(vui_main2   ${PATH_SNB}/vui_main2.cpp)

#Custom Targets
add_custom_target(externals DEPENDS ${EXTERNAL_PROJECT_LIST})
add_custom_target(ext DEPENDS externals)

# Linkage ------------------------------------------------------------------------------------------

#Link - Libraries
target_link_libraries(vframe -static fmt glfw3 gdi32 ${LIB_OPENGL} ${LIB_GLU})
target_link_libraries(vgl    -static glew ${LIB_OPENGL} ${LIB_GLU})
target_link_libraries(vlog   -static)
target_link_libraries(vm3    -static boost_serialization-mt-s)
target_link_libraries(vm3imp -static vm3 fmt assimp zlibstatic)
target_link_libraries(vui    -static fmt glew glfw3 ${LIB_OPENGL} ${LIB_GLU} gdi32
							boost_system-mt-s boost_filesystem-mt-s freetype vgl)
#target_link_libraries(vframe -static fmt glew glfw3 ${LIB_OPENGL} ${LIB_GLU} gdi32
#							boost_system-mt-s boost_filesystem-mt-s freetype vgl)

#Link - Tests
target_link_libraries(libv_test   -static )
target_link_libraries(vgl_test    -static vgl)
target_link_libraries(vlog_test   -static fmt)
target_link_libraries(vm3_test    -static vm3 boost_serialization-mt-s)
target_link_libraries(vm3imp_test -static vm3)
target_link_libraries(vparse_test -static )
target_link_libraries(vsig_test   -static )
target_link_libraries(vui_test    -static vui fmt vlog)

#Link - Executables
target_link_libraries(libv_main   -static vlog fmt)
target_link_libraries(vframe_main -static vlog fmt vframe)
target_link_libraries(vgl_main    -static vlog fmt glew glfw3 vgl boost_system-mt-s boost_filesystem-mt-s)
target_link_libraries(vgl_main2   -static vlog fmt glew glfw3 vgl boost_system-mt-s boost_filesystem-mt-s)
target_link_libraries(vlog_main   -static fmt)
target_link_libraries(vm3_main    -static vlog fmt vm3 boost_system-mt-s boost_filesystem-mt-s)
target_link_libraries(vm3imp_main -static vlog fmt vm3imp boost_system-mt-s boost_filesystem-mt-s)
target_link_libraries(vsig_main   -static vlog fmt)
target_link_libraries(vui_main    -static vlog fmt vui)
target_link_libraries(vui_main2   -static vlog fmt vui)
