cmake_minimum_required (VERSION 3.1)
project (vl)

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

add_definitions(-m64)
add_definitions(-std=c++14)
add_definitions(-Wall -Wextra -Wno-comment -Wno-unused-local-typedefs)

#Macro
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLM_FORCE_RADIANS)

#Paths
set(PATH_BIN ${PROJECT_SOURCE_DIR}/bin)
set(PATH_LIB ${PROJECT_SOURCE_DIR}/lib)
set(PATH_EXT ..)
set(PATH_INC include)
set(PATH_SRC src)
set(PATH_TST test)
set(PATH_PRJ ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PATH_BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PATH_LIB})

#Includes
include_directories (${PATH_INC})
include_directories (${PATH_SRC})

#Sources
file(GLOB_RECURSE vgl_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/gl/*.cpp)
file(GLOB_RECURSE vgl_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/gl/*.cpp)
file(GLOB_RECURSE vl_src          RELATIVE ${PATH_PRJ} ${PATH_SRC}/vl/*.cpp)
file(GLOB_RECURSE vl_test_src     RELATIVE ${PATH_PRJ} ${PATH_TST}/vl/*.cpp)
file(GLOB_RECURSE vm3_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/vm3/*.cpp)
file(GLOB_RECURSE vm3_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/vm3/*.cpp)
file(GLOB_RECURSE vm3imp_src      RELATIVE ${PATH_PRJ} ${PATH_SRC}/vm3imp/*.cpp)
file(GLOB_RECURSE vm3imp_test_src RELATIVE ${PATH_PRJ} ${PATH_TST}/vm3imp/*.cpp)
file(GLOB_RECURSE vsig_src        RELATIVE ${PATH_PRJ} ${PATH_SRC}/sig/*.cpp)
file(GLOB_RECURSE vsig_test_src   RELATIVE ${PATH_PRJ} ${PATH_TST}/sig/*.cpp)
file(GLOB_RECURSE vui_src         RELATIVE ${PATH_PRJ} ${PATH_SRC}/ui/*.cpp)
file(GLOB_RECURSE vui_test_src    RELATIVE ${PATH_PRJ} ${PATH_TST}/ui/*.cpp)

#Externals
include_directories(${PATH_EXT}/UnitTest++/include)
include_directories(${PATH_EXT}/assimp-3.1.1-dae/include)
include_directories(${PATH_EXT}/boost_1_57_0/include)
include_directories(${PATH_EXT}/freetype/include)
include_directories(${PATH_EXT}/glew-1.11.0/include)
include_directories(${PATH_EXT}/glfw-3.1.1/include)
include_directories(${PATH_EXT}/gli-master)
include_directories(${PATH_EXT}/glm-master)

link_directories(${PATH_EXT}/UnitTest++)
link_directories(${PATH_EXT}/assimp-3.1.1-dae/lib)
link_directories(${PATH_EXT}/boost_1_57_0/lib)
link_directories(${PATH_EXT}/freetype)
link_directories(${PATH_EXT}/glew-1.11.0/lib)
link_directories(${PATH_EXT}/glfw-3.1.1/lib)

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
add_executable(vgl_main    ${PATH_SRC}/vgl_main.cpp)
add_executable(vgl_main2   ${PATH_SRC}/vgl_main2.cpp)
add_executable(vl_main     ${PATH_SRC}/vl_main.cpp)
add_executable(vm3_main    ${PATH_SRC}/vm3_main.cpp)
add_executable(vm3imp_main ${PATH_SRC}/vm3imp_main.cpp)
add_executable(vsig_main   ${PATH_SRC}/vsig_main.cpp)
add_executable(vui_main    ${PATH_SRC}/vui_main.cpp)
add_executable(vui_main2   ${PATH_SRC}/vui_main2.cpp)

#Link - Libraries
target_link_libraries(vgl    -static glew32 opengl32 glu32 vm3 boost_serialization-mgw49-mt-1_57)
target_link_libraries(vm3    -static boost_serialization-mgw49-mt-1_57)
target_link_libraries(vm3imp -static vm3 assimp)
target_link_libraries(vui    -static glew32 glfw3 opengl32 glu32 gdi32 freetype vgl)

#Link - Tests
target_link_libraries(vgl_test    UnitTest++ vgl)
target_link_libraries(vl_test     UnitTest++)
target_link_libraries(vm3_test    UnitTest++ vm3 boost_serialization-mgw49-mt-1_57)
target_link_libraries(vm3imp_test UnitTest++ vm3)
target_link_libraries(vsig_test   UnitTest++)
target_link_libraries(vui_test    UnitTest++)

#Link - Executables
target_link_libraries(vgl_main    -static glew32 glfw3 vgl)
target_link_libraries(vgl_main2   -static vgl)
target_link_libraries(vl_main     -static)
target_link_libraries(vm3_main    -static vm3)
target_link_libraries(vm3imp_main -static vm3imp)
target_link_libraries(vsig_main   -static)
target_link_libraries(vui_main    -static vui)
target_link_libraries(vui_main2   -static vui)
