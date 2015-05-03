cmake_minimum_required (VERSION 3.1)
project (vl)
include(ExternalProject)

include(cmake/util.cmake)
message(STATUS "Processor count: ${PROCESSOR_COUNT}")

#enable_testing()
#set(CMAKE_VERBOSE_MAKEFILE off)

#Definitions
#IF DEBUG TODO
add_definitions(-Ofast)
#add_definitions(-O2)
#add_definitions(-O0)
#EF DEBUG TODO
#add_definitions(-ggdb)
#FI DEBUG TODO

#if(NOT PIC)
#	add_definitions(-fPIC)
#endif()

add_definitions(-m64)
add_definitions(-std=c++14)
add_definitions(-Wall -Wextra -Wno-comment -Wno-unused-local-typedefs)

#Macro
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLM_FORCE_RADIANS)

#Paths
set(PATH_BIN ${PROJECT_SOURCE_DIR}/bin)
set(PATH_LIB ${PROJECT_SOURCE_DIR}/lib)
set(PATH_PRJ ${CMAKE_CURRENT_SOURCE_DIR})
set(PATH_EXT ${PATH_PRJ}/ext)
set(PATH_EXT_SRC ${PATH_PRJ}/ext_src)
set(PATH_INC include)
set(PATH_SRC src)
set(PATH_TST test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PATH_LIB})

#Variables
set(EXTERNAL_PROJECT_LIST)
set(TOOLSET gcc)

# --------------------------------------------------------------------------------------------------

#list(APPEND EXTERNAL_PROJECT_LIST ext_unittest)
#ExternalProject_Add(ext_unittest
#	GIT_REPOSITORY https://github.com/unittest-cpp/unittest-cpp.git
#	GIT_TAG master
#	PREFIX ${PATH_EXT_SRC}/unittest
#	INSTALL_DIR ${PATH_EXT}/unittest
#	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/unittest
#	EXCLUDE_FROM_ALL 1
#)

list(APPEND EXTERNAL_PROJECT_LIST ext_glfw)
ExternalProject_Add(ext_glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.1.1
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

list(APPEND EXTERNAL_PROJECT_LIST ext_glm)
ExternalProject_Add(ext_glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG master
	PREFIX ${PATH_EXT_SRC}/glm
	INSTALL_DIR ${PATH_EXT}/glm
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glm
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_gli)
ExternalProject_Add(ext_gli 
	GIT_REPOSITORY https://github.com/g-truc/gli.git
	GIT_TAG 0.5.1.1
	PREFIX ${PATH_EXT_SRC}/gli
	INSTALL_DIR ${PATH_EXT}/gli
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/gli
	INSTALL_COMMAND 
		mkdir -p ${PATH_EXT}/gli/include/gli/ &&
		cp -r -f ${PATH_EXT_SRC}/gli/src/ext_gli/gli/ ${PATH_EXT}/gli/include/
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
set(ASSIMP_SKIP_LIST 
"-DASSIMP_BUILD_NO_X_IMPORTER"
"-DASSIMP_BUILD_NO_3DS_IMPORTER"
"-DASSIMP_BUILD_NO_MD3_IMPORTER"
"-DASSIMP_BUILD_NO_MDL_IMPORTER"
"-DASSIMP_BUILD_NO_MD2_IMPORTER"
"-DASSIMP_BUILD_NO_PLY_IMPORTER"
"-DASSIMP_BUILD_NO_ASE_IMPORTER"
"-DASSIMP_BUILD_NO_OBJ_IMPORTER"
"-DASSIMP_BUILD_NO_HMP_IMPORTER"
"-DASSIMP_BUILD_NO_SMD_IMPORTER"
"-DASSIMP_BUILD_NO_MDC_IMPORTER"
"-DASSIMP_BUILD_NO_MD5_IMPORTER"
"-DASSIMP_BUILD_NO_STL_IMPORTER"
"-DASSIMP_BUILD_NO_LWO_IMPORTER"
"-DASSIMP_BUILD_NO_DXF_IMPORTER"
"-DASSIMP_BUILD_NO_NFF_IMPORTER"
"-DASSIMP_BUILD_NO_RAW_IMPORTER"
"-DASSIMP_BUILD_NO_OFF_IMPORTER"
"-DASSIMP_BUILD_NO_AC_IMPORTER"
"-DASSIMP_BUILD_NO_BVH_IMPORTER"
"-DASSIMP_BUILD_NO_IRRMESH_IMPORTER"
"-DASSIMP_BUILD_NO_IRR_IMPORTER"
"-DASSIMP_BUILD_NO_Q3D_IMPORTER"
"-DASSIMP_BUILD_NO_B3D_IMPORTER"
"-DASSIMP_BUILD_NO_COLLADA_IMPORTER"
"-DASSIMP_BUILD_NO_TERRAGEN_IMPORTER"
"-DASSIMP_BUILD_NO_CSM_IMPORTER"
"-DASSIMP_BUILD_NO_3D_IMPORTER"
"-DASSIMP_BUILD_NO_LWS_IMPORTER"
"-DASSIMP_BUILD_NO_OGRE_IMPORTER"
"-DASSIMP_BUILD_NO_OPENGEX_IMPORTER"
"-DASSIMP_BUILD_NO_MS3D_IMPORTER"
"-DASSIMP_BUILD_NO_COB_IMPORTER"
"-DASSIMP_BUILD_NO_BLEND_IMPORTER"
"-DASSIMP_BUILD_NO_Q3BSP_IMPORTER"
"-DASSIMP_BUILD_NO_NDO_IMPORTER"
"-DASSIMP_BUILD_NO_IFC_IMPORTER"
"-DASSIMP_BUILD_NO_XGL_IMPORTER"
"-DASSIMP_BUILD_NO_FBX_IMPORTER"
"-DASSIMP_BUILD_NO_ASSBIN_IMPORTER"
"-DASSIMP_BUILD_NO_C4D_IMPORTER")
string(REPLACE ";" " " ASSIMP_SKIP_STR "${ASSIMP_SKIP_LIST}")
#TODO P2: assimp no importers

ExternalProject_Add(ext_assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG master
	PREFIX ${PATH_EXT_SRC}/assimp
	INSTALL_DIR ${PATH_EXT}/assimp
	CMAKE_ARGS
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
		-DCMAKE_INSTALL_PREFIX=${PATH_EXT}/assimp
		-DASSIMP_BUILD_ASSIMP_TOOLS=OFF
		-DASSIMP_BUILD_TESTS=OFF
		-DBUILD_SHARED_LIBS=OFF
		-DASSIMP_NO_EXPORT=ON
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_boost)
set(Boost_Version 1.58.0)
string(REPLACE "." "_" Boost_Version_Underscore ${Boost_Version})

set(ext_boost_bootstrap_command)
if(NOT WIN32)
	set(ext_boost_bootstrap_command ./bootstrap.sh)
	set(ext_boost_b2_command ./b2)
else()
	set(ext_boost_bootstrap_command cmd.exe //c bootstrap.bat ${TOOLSET})
	set(ext_boost_b2_command cmd.exe //c b2.exe)
endif()

message(STATUS "Boost_Version: " ${Boost_Version})
ExternalProject_Add(ext_boost
	PREFIX ${PATH_EXT_SRC}/boost
	URL http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.zip
	URL_MD5 b0605a9323f1e960f7434dbbd95a7a5c
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
		--without-context --without-coroutine #workaround for missing ml64 on win, until not used i don't care
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
include_directories(${PATH_EXT}/unittest/include)
include_directories(${PATH_EXT}/assimp/include)
include_directories(${PATH_EXT}/boost/include)
include_directories(${PATH_EXT}/freetype/include/freetype2)
include_directories(${PATH_EXT}/glew/include)
include_directories(${PATH_EXT}/glfw/include)
include_directories(${PATH_EXT}/gli/include)
include_directories(${PATH_EXT}/glm/include)

link_directories(${PATH_EXT}/unittest/lib)
link_directories(${PATH_EXT}/assimp/lib)
link_directories(${PATH_EXT}/boost/lib)
link_directories(${PATH_EXT}/freetype/lib)
link_directories(${PATH_EXT}/glew/lib)
link_directories(${PATH_EXT}/glfw/lib)

#Sources
file(GLOB_RECURSE vgl_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/gl/*.cpp)
file(GLOB_RECURSE vl_src          RELATIVE ${PATH_PRJ} ${PATH_SRC}/vl/*.cpp)
file(GLOB_RECURSE vm3_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/vm3/*.cpp)
file(GLOB_RECURSE vm3imp_src      RELATIVE ${PATH_PRJ} ${PATH_SRC}/vm3imp/*.cpp)
file(GLOB_RECURSE vsig_src        RELATIVE ${PATH_PRJ} ${PATH_SRC}/sig/*.cpp)
file(GLOB_RECURSE vui_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/ui/*.cpp)

#Test Sources
file(GLOB_RECURSE vgl_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/gl/*.cpp)
file(GLOB_RECURSE vl_test_src     RELATIVE ${PATH_PRJ} ${PATH_TST}/vl/*.cpp)
file(GLOB_RECURSE vm3_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/vm3/*.cpp)
file(GLOB_RECURSE vm3imp_test_src RELATIVE ${PATH_PRJ} ${PATH_TST}/vm3imp/*.cpp)
file(GLOB_RECURSE vsig_test_src   RELATIVE ${PATH_PRJ} ${PATH_TST}/sig/*.cpp)
file(GLOB_RECURSE vui_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/ui/*.cpp)

#Libraries
add_library(vgl    STATIC ${vgl_src})
add_library(vl     STATIC ${vl_src})
add_library(vm3    STATIC ${vm3_src})
add_library(vm3imp STATIC ${vm3imp_src})
add_library(vsig   STATIC ${vsig_src})
add_library(vui    STATIC ${vui_src})

#Tests
#add_executable(vgl_test    ${vgl_test_src})
#add_executable(vl_test     ${vl_test_src})
#add_executable(vm3_test    ${vm3_test_src})
#add_executable(vm3imp_test ${vm3imp_test_src})
#add_executable(vsig_test   ${vsig_test_src})
#add_executable(vui_test    ${vui_test_src})

#Executables
add_executable(vgl_main    ${PATH_SRC}/vgl_main.cpp)
add_executable(vgl_main2   ${PATH_SRC}/vgl_main2.cpp)
add_executable(vl_main     ${PATH_SRC}/vl_main.cpp)
add_executable(vm3_main    ${PATH_SRC}/vm3_main.cpp)
add_executable(vm3imp_main ${PATH_SRC}/vm3imp_main.cpp)
add_executable(vsig_main   ${PATH_SRC}/vsig_main.cpp)
add_executable(vui_main    ${PATH_SRC}/vui_main.cpp)
add_executable(vui_main2   ${PATH_SRC}/vui_main2.cpp)

#Costum Targets
add_custom_target(externals DEPENDS ${EXTERNAL_PROJECT_LIST})
add_custom_target(ext DEPENDS externals)

#Link - Libraries
target_link_libraries(vgl    -static glew opengl32 glu32 vm3 boost_serialization-mt-s)
target_link_libraries(vm3    -static boost_serialization-mt-s)
target_link_libraries(vm3imp -static vm3 assimp zlibstatic)
target_link_libraries(vui    -static glew glfw3 opengl32 glu32 gdi32 freetype vgl)

#Link - Tests
#target_link_libraries(vgl_test    UnitTest++ vgl)
#target_link_libraries(vl_test     UnitTest++)
#target_link_libraries(vm3_test    UnitTest++ vm3 boost_serialization-mt-s)
#target_link_libraries(vm3imp_test UnitTest++ vm3)
#target_link_libraries(vsig_test   UnitTest++)
#target_link_libraries(vui_test    UnitTest++)

#Link - Executables
target_link_libraries(vgl_main    -static glew glfw3 vgl)
target_link_libraries(vgl_main2   -static vgl)
target_link_libraries(vl_main     -static)
target_link_libraries(vm3_main    -static vm3)
target_link_libraries(vm3imp_main -static vm3imp)
target_link_libraries(vsig_main   -static)
target_link_libraries(vui_main    -static vui)
target_link_libraries(vui_main2   -static vui)
