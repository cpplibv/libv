#

cmake_minimum_required (VERSION 3.1)
project (vl)
include(ExternalProject)

include(cmake/util.cmake)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor count: ${PROCESSOR_COUNT}")
message(STATUS "CXX compiler id: ${CMAKE_CXX_COMPILER_ID}")

# --------------------------------------------------------------------------------------------------

# TODO P2: flag or build options to turn on/off sanitizes
# TODO P2: debug symbol automated turn on/off
# TODO P3: backward compatibility for pre gcc 5.1 because suggest-override
# TODO P3: test clang build
# TODO P5: test msvc build

# --------------------------------------------------------------------------------------------------

#set(CMAKE_VERBOSE_MAKEFILE off)
set(CMAKE_BUILD_TYPE "DEBUG")
#set(CMAKE_BUILD_TYPE "RELEASE")

#Definitions
if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
	add_definitions(-O0)
	add_definitions(-ggdb3)
else() #elseif(CMAKE_BUILD_TYPE EQUAL "RELEASE")
	add_definitions(-Ofast)
endif()

add_definitions(-m64)
add_definitions(-std=c++14)

#Macro
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLM_FORCE_RADIANS)

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
set(PATH_TST test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PATH_LIB})

#Variables
set(TOOLSET)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

	add_compile_options(/Wall)
	set(TOOLSET msvc)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")

	add_compile_options(-Weverything -Wconversion -Wmissing-override -Wno-comment)
	set(TOOLSET clang)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")

	add_compile_options(-Wall -Wextra -Wconversion -Wno-comment)
	add_compile_options(-Wsuggest-override) #IF > GNU 5.1

	if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
		add_compile_options(-fsanitize=address)
		#add_compile_options(-fsanitize=kernel-address)
		#add_compile_options(-fsanitize=leak)
		#add_compile_options(-fsanitize=thread)
		#add_compile_options(-fsanitize=undefined)
	endif()
	set(TOOLSET gcc)

endif()

if(WIN32)
	set(LIB_OPENGL opengl32)
	set(LIB_GLU glu32)
else()
	set(LIB_OPENGL GL)
	set(LIB_GLU GLU)
endif()

# --------------------------------------------------------------------------------------------------

set(EXTERNAL_PROJECT_LIST)

list(APPEND EXTERNAL_PROJECT_LIST ext_unittest)
ExternalProject_Add(ext_unittest
	GIT_REPOSITORY https://github.com/philsquared/Catch.git
	GIT_TAG master
	PREFIX ${PATH_EXT_SRC}/unittest
	INSTALL_DIR ${PATH_EXT}/unittest
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/unittest
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""  #catch is header only, skip tests
	INSTALL_COMMAND
		mkdir -p ${PATH_EXT}/unittest/ &&
		cp -r -f ${PATH_EXT_SRC}/unittest/src/ext_unittest/include/ ${PATH_EXT}/unittest/
	EXCLUDE_FROM_ALL 1
)

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
	BUILD_COMMAND "" #glm is header only, skip tests
	PREFIX ${PATH_EXT_SRC}/glm
	INSTALL_DIR ${PATH_EXT}/glm
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PATH_EXT}/glm
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_gli)
ExternalProject_Add(ext_gli
	GIT_REPOSITORY https://github.com/g-truc/gli.git
	GIT_TAG 0.7.0.0
	BUILD_COMMAND "" #gli is header only, skip tests
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
		-DASSIMP_BUILD_X_IMPORTER=OFF
		-DASSIMP_BUILD_3DS_IMPORTER=OFF
		-DASSIMP_BUILD_MD3_IMPORTER=OFF
		-DASSIMP_BUILD_MDL_IMPORTER=OFF
		-DASSIMP_BUILD_MD2_IMPORTER=OFF
		-DASSIMP_BUILD_PLY_IMPORTER=OFF
		-DASSIMP_BUILD_ASE_IMPORTER=OFF
		-DASSIMP_BUILD_OBJ_IMPORTER=OFF
		-DASSIMP_BUILD_HMP_IMPORTER=OFF
		-DASSIMP_BUILD_SMD_IMPORTER=OFF
		-DASSIMP_BUILD_MDC_IMPORTER=OFF
		-DASSIMP_BUILD_MD5_IMPORTER=OFF
		-DASSIMP_BUILD_STL_IMPORTER=OFF
		-DASSIMP_BUILD_LWO_IMPORTER=OFF
		-DASSIMP_BUILD_DXF_IMPORTER=OFF
		-DASSIMP_BUILD_NFF_IMPORTER=OFF
		-DASSIMP_BUILD_RAW_IMPORTER=OFF
		-DASSIMP_BUILD_OFF_IMPORTER=OFF
		-DASSIMP_BUILD_AC_IMPORTER=OFF
		-DASSIMP_BUILD_BVH_IMPORTER=OFF
		-DASSIMP_BUILD_IRRMESH_IMPORTER=OFF
		-DASSIMP_BUILD_IRR_IMPORTER=OFF
		-DASSIMP_BUILD_Q3D_IMPORTER=OFF
		-DASSIMP_BUILD_B3D_IMPORTER=OFF
		-DASSIMP_BUILD_COLLADA_IMPORTER=OFF
		-DASSIMP_BUILD_TERRAGEN_IMPORTER=OFF
		-DASSIMP_BUILD_CSM_IMPORTER=OFF
		-DASSIMP_BUILD_3D_IMPORTER=OFF
		-DASSIMP_BUILD_LWS_IMPORTER=OFF
		-DASSIMP_BUILD_OGRE_IMPORTER=OFF
		-DASSIMP_BUILD_OPENGEX_IMPORTER=OFF
		-DASSIMP_BUILD_MS3D_IMPORTER=OFF
		-DASSIMP_BUILD_COB_IMPORTER=OFF
		-DASSIMP_BUILD_BLEND_IMPORTER=OFF
		-DASSIMP_BUILD_Q3BSP_IMPORTER=OFF
		-DASSIMP_BUILD_NDO_IMPORTER=OFF
		-DASSIMP_BUILD_IFC_IMPORTER=OFF
		-DASSIMP_BUILD_XGL_IMPORTER=OFF
		-DASSIMP_BUILD_FBX_IMPORTER=OFF
		-DASSIMP_BUILD_ASSBIN_IMPORTER=OFF
		-DASSIMP_BUILD_C4D_IMPORTER=OFF
	EXCLUDE_FROM_ALL 1
)

list(APPEND EXTERNAL_PROJECT_LIST ext_boost)
set(Boost_Version 1.58.0)
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
		--with-serialization
		--with-system
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
include_directories(SYSTEM ${PATH_EXT}/freetype/include/freetype2)
include_directories(SYSTEM ${PATH_EXT}/glew/include)
include_directories(SYSTEM ${PATH_EXT}/glfw/include)
include_directories(SYSTEM ${PATH_EXT}/gli/include)
include_directories(SYSTEM ${PATH_EXT}/glm/include)
include_directories(SYSTEM ${PATH_EXT}/unittest/include)

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
add_executable(vgl_test    ${vgl_test_src})
add_executable(vl_test     ${vl_test_src})
add_executable(vm3_test    ${vm3_test_src})
add_executable(vm3imp_test ${vm3imp_test_src})
add_executable(vsig_test   ${vsig_test_src})
add_executable(vui_test    ${vui_test_src})

#Executables
add_executable(vgl_main    ${PATH_EXP}/vgl_main.cpp)
add_executable(vl_main     ${PATH_EXP}/vl_main.cpp)
add_executable(vm3_main    ${PATH_EXP}/vm3_main.cpp)
add_executable(vm3imp_main ${PATH_EXP}/vm3imp_main.cpp)
add_executable(vsig_main   ${PATH_EXP}/vsig_main.cpp)
add_executable(vui_main    ${PATH_EXP}/vui_main.cpp)
add_executable(vui_main2   ${PATH_EXP}/vui_main2.cpp)

#Custom Targets
add_custom_target(externals DEPENDS ${EXTERNAL_PROJECT_LIST})
add_custom_target(ext DEPENDS externals)

#Link - Libraries
target_link_libraries(vgl    -static glew ${LIB_OPENGL} ${LIB_GLU} vm3 boost_serialization-mt-s)
target_link_libraries(vm3    -static boost_serialization-mt-s)
target_link_libraries(vm3imp -static vm3 assimp zlibstatic)
target_link_libraries(vui    -static glew glfw3 ${LIB_OPENGL} ${LIB_GLU} gdi32 freetype vgl)

#Link - Tests
target_link_libraries(vgl_test    -static vgl)
target_link_libraries(vl_test     -static )
target_link_libraries(vm3_test    -static vm3 boost_serialization-mt-s)
target_link_libraries(vm3imp_test -static vm3)
target_link_libraries(vsig_test   -static )
target_link_libraries(vui_test    -static )

#Link - Executables
target_link_libraries(vgl_main    -static glew glfw3 vgl)
target_link_libraries(vl_main     -static )
target_link_libraries(vm3_main    -static vm3)
target_link_libraries(vm3imp_main -static vm3imp)
target_link_libraries(vsig_main   -static )
target_link_libraries(vui_main    -static vui)
target_link_libraries(vui_main2   -static vui)
