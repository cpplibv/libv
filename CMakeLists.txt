#

cmake_minimum_required(VERSION 3.20)
project(libv)

#set(CMAKE_VERBOSE_MAKEFILE on)
#set(CMAKE_BUILD_TYPE DEBUG)
#set(CMAKE_BUILD_TYPE RELEASE)

set(WISH_REQUEST_VERSION v5.0.2)
include(cmake/wish.cmake)

# --------------------------------------------------------------------------------------------------

wish_configurations(DEFAULT Release Dev Debug)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Processor count: ${WISH_PROCESSOR_COUNT}")
message(STATUS "CXX compiler id: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
#message(STATUS "Git branch: ${WISH_GIT_BRANCH}")
#message(STATUS "Git commit: ${WISH_GIT_COMMIT_HASH}")

# Paths --------------------------------------------------------------------------------------------

set(PATH_EXT_IDE ext)
set(PATH_EXT     ${CMAKE_SOURCE_DIR}/ext)
set(PATH_EXT_SRC ${CMAKE_SOURCE_DIR}/ext_src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

# --- Options --------------------------------------------------------------------------------------

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
wish_force_colored_output(${FORCE_COLORED_OUTPUT})

option(SKIP_EXTERNAL_CONFIGURES "Do not configure external projects only use the fake interface targets" FALSE)
wish_skip_external_configures(${SKIP_EXTERNAL_CONFIGURES})

# --- Flags ----------------------------------------------------------------------------------------

wish_warning(
	MSVC /Wall

	Clang -Weverything
	Clang -Wmissing-override
	Clang -Wconversion

	GNU -Wall
	GNU -Warray-bounds
	GNU -Wcast-align=strict
	GNU -Wcast-qual
	GNU -Wconversion
	GNU -Wdelete-non-virtual-dtor
	GNU -Wdouble-promotion
	GNU -Wduplicated-branches
	GNU -Wduplicated-cond
	GNU -Wextra
	GNU -Wlogical-op
	GNU -Wmultistatement-macros
	GNU -Wnon-virtual-dtor # Causes some false positives with efsw (suppressed)
	GNU -Wold-style-cast
	GNU -Wpedantic
	GNU -Wrestrict
	GNU -Wshadow-compatible-local
	GNU -Wsuggest-override
	GNU -Wundef

	#	more warning:
	#GNU -Wmissing-include-dirs
	#GNU -Wdisabled-optimization,
	#GNU -Wpadded,
	#GNU -Wzero-as-null-pointer-constant,
	#	maybe even:
	#GNU -Wswitch-enum, and take a look at:
	#GNU -Wunsafe-loop-optimizations,
	#GNU -Wdangling-else

	# Enablement in progress:
	# GNU -Wuseless-cast # in vec_base_t has some false positive issue with this one, but otherwise clear
	# GNU -Wfloat-equal # some in tests, and some in ui might be true positive, ui got some true positive with ratio

	# Only enable time-to-time to check on what is going on:
	# GNU -Winline
	# GNU -Wsuggest-final-methods # in signal 50+ function
	# GNU -Wsuggest-final-types # in signal 1 type

	# With GCC 12.1 some false positives warnings appeared and some do ignore the system header includes
	GNU VERSION_GREATER 12.0 -Wno-array-bounds
	GNU VERSION_GREATER 12.0 -Wno-stringop-overread
	GNU VERSION_GREATER 12.0 -Wno-stringop-overflow

	# Warnings that I don't care about
	Clang -Wno-comment
	GNU -Wno-comment
	# GNU -Wnull-dereference # Causes too many false positives as of GCC 11.2
	# GNU -Werror
)

wish_compiler_flags(
	GNU -fcoroutines
	GNU -m64
	GNU -std=c++23
)

#wish_linker_flags(
#	BUILD_TYPE Release GNU -mwindows
#)
#add_link_options(
#	-mwindows
#)

wish_optimization_flags()

# Includes Directories -----------------------------------------------------------------------------

include_directories(app)
include_directories(src)

# === External ====================================================================================

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)

wish_group(group_external ext) # --- External -----------------------------------------------------

wish_create_external(
	NAME assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG v5.0.0
	CMAKE_ARGS
		-DBUILD_SHARED_LIBS=OFF
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
		-DASSIMP_BUILD_ASSIMP_TOOLS=OFF
		-DASSIMP_BUILD_TESTS=OFF
		-DASSIMP_BUILD_ZLIB=ON
		-DASSIMP_NO_EXPORT=ON
		#
		-DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF
		-DASSIMP_BUILD_FBX_IMPORTER=ON
	LINK assimp
	LINK IrrXML
	LINK zlibstatic
)

wish_create_external(
	NAME libbacktrace
	GIT_REPOSITORY https://github.com/ianlancetaylor/libbacktrace.git
	GIT_TAG d0f5e95a87a4d3e0a1ed6c069b5dae7cbab3ed2a
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND
		COMMAND ${CMAKE_COMMAND} -E rm -f ${PATH_EXT_SRC}/libbacktrace/src/get_libbacktrace/config.status # Workaround, sometimes it stucks and configure fails
		COMMAND bash -C ./configure # 'bash' is used for Windows cmd to not fail
	BUILD_COMMAND make -j${WISH_PROCESSOR_COUNT}
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/libbacktrace/include/libbacktrace/
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/libbacktrace/lib/
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/libbacktrace/src/get_libbacktrace/backtrace.h ${PATH_EXT}/libbacktrace/include/libbacktrace/backtrace.h
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/libbacktrace/src/get_libbacktrace/.libs/libbacktrace.a ${PATH_EXT}/libbacktrace/lib/libbacktrace.a
	LINK backtrace
)

set(Boost_Version 1.77.0)
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
	URL https://boostorg.jfrog.io/artifactory/main/release/${Boost_Version}/source/boost_${Boost_Version_Underscore}.zip
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND bash -C ./bootstrap.sh ${TOOLSET} # 'bash' is used for Windows cmd to not fail
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
#		--with-thread
		-j${WISH_PROCESSOR_COUNT}
	INSTALL_COMMAND "echo" "Skipping install..."

	DEFINE BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT
	DEFINE BOOST_DLL_USE_STD_FS=1
	DEFINE BOOST_STACKTRACE_USE_BACKTRACE
	DEFINE BOOST_STACKTRACE_BACKTRACE_INCLUDE_FILE=<libbacktrace/backtrace.h>

#	LINK boost_thread-mt-s-x64

	# For boost asio usage
	DEFINE _WIN32_WINNT=0x0501
	LINK boost_system-mt-s-x64
	LINK $<IF:$<BOOL:${WIN32}>, wsock32, >
	LINK $<IF:$<BOOL:${WIN32}>, ws2_32, >
)

wish_create_library(
	TARGET ext_boost_stacktrace INTERFACE
	LINK ext_boost ext_libbacktrace ole32 dbgeng
)

wish_create_external(
	NAME catch
	GIT_REPOSITORY https://github.com/catchorg/Catch2
	GIT_TAG v3.0.1
	CMAKE_ARGS
        -DCATCH_INSTALL_DOCS=OFF
        -DCATCH_INSTALL_EXTRAS=OFF
	LINK Catch2Main Catch2
)

wish_create_external(
	NAME clip
	GIT_REPOSITORY https://github.com/dacap/clip.git
	GIT_TAG 3c8f05b5d71a4d5ea7125f074323307293332a92
	CMAKE_ARGS
		-DCLIP_EXAMPLES=OFF
		-DCLIP_TESTS=OFF
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/clip/include/clip/
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/clip/lib/
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/clip/src/get_clip/clip.h ${PATH_EXT}/clip/include/clip/clip.h
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/clip/src/get_clip-build/libclip.a ${PATH_EXT}/clip/lib/libclip.a
	LINK clip
)

wish_create_external(
	NAME colony
	GIT_REPOSITORY https://github.com/mattreecebentley/plf_colony.git
	GIT_TAG 80cb82f542bb176ab7f7e3393ab530a79223bb13
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/colony/include/colony/
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/colony/src/get_colony/plf_colony.h ${PATH_EXT}/colony/include/colony/plf_colony.h
)

wish_create_external(
	NAME cppcoro
#	GIT_REPOSITORY https://github.com/andreasbuhr/cppcoro.git
#	GIT_TAG 7cc9433436fe8f2482138019cfaafce8e1d7a896
	GIT_REPOSITORY https://github.com/VaderY/cppcoro.git
	GIT_TAG a99556e1768b1b6b3c411b6f06e31934a5b56c26
	CMAKE_ARGS
		-DBUILD_TESTING=OFF
	LINK cppcoro
	LINK $<IF:$<BOOL:${WIN32}>,
			Synchronization, # Win 32
		>
)

wish_create_external(
	NAME ctre
	GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
	GIT_TAG 72ea41938bca052fa33d61469213beb76245c788
	CMAKE_ARGS
		-DCTRE_BUILD_TESTS=OFF
		-DCTRE_BUILD_PACKAGE=OFF
		-DCTRE_BUILD_PACKAGE_DEB=OFF
		-DCTRE_BUILD_PACKAGE_RPM=OFF
)

wish_create_external(
	NAME date
	GIT_REPOSITORY https://github.com/HowardHinnant/date.git
	GIT_TAG 3e376be2e9b4d32c946bd83c22601e4b7a1ce421
#	CMAKE_ARGS
#		-DENABLE_DATE_TESTING=OFF
#		-DUSE_SYSTEM_TZ_DB=ON
#		-DUSE_TZ_DB_IN_DOT=ON
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/date/include/
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PATH_EXT_SRC}/date/src/get_date/include/ ${PATH_EXT}/date/include/
)

wish_create_external(
	NAME hdiffpatch
	GIT_REPOSITORY https://github.com/VaderY/HDiffPatch.git
	GIT_TAG f931d8d6cc992e6744fea317327368e4b0103fbc
	CMAKE_ARGS
		-DHDIFF_VERSION_TAG="v0"
	LINK hdiffpatch
#	LINK z bz2 # compressors are not used
)

wish_create_external(
	NAME efsw
	GIT_REPOSITORY https://github.com/SpartanJ/efsw.git
	GIT_TAG 33d6ce34a53d2b2c5dfb856570c8c259fce2682a
	CMAKE_ARGS
		-DSTATIC_LIB=ON
	LINK efsw
)

wish_create_external(
	NAME fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 8.1.1
	CMAKE_ARGS
		-DFMT_DOC=OFF
		-DFMT_TEST=OFF
		-DBUILD_SHARED_LIBS=OFF
	LINK fmt
)

wish_create_external(
	NAME freetype
	GIT_REPOSITORY http://git.sv.nongnu.org/r/freetype/freetype2.git
	GIT_TAG f686ad46a3a9801ffa78063202250c68276330cb

#	GIT_REPOSITORY https://github.com/freetype/freetype
#	GIT_TAG VER-2-11-0
#	CMAKE_ARGS
#		-DFT_DISABLE_ZLIB=ON
#		-DFT_DISABLE_BZIP2=ON
#		-DFT_DISABLE_PNG=ON
#		-DFT_DISABLE_HARFBUZZ=ON
#		-DFT_DISABLE_BROTLI=ON
#	LINK freetype
#	LINK freetype bz2 zlibstatic ibs

	# TODO P5: Create dedicated external target for bz2
	# TODO P3: Create dedicated external target for zlibstatic
	LINK freetype bz2 zlibstatic
	INCLUDE_DIR include/freetype2
)

wish_create_external(
	NAME function2
	GIT_REPOSITORY https://github.com/Naios/function2.git
	GIT_TAG a92427bf25705287a47ffebd787dfec93033ccef
	CMAKE_ARGS
		-DBUILD_TESTING=OFF
)

wish_create_external(
	NAME glew
	GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
	GIT_TAG 679246fadb5780963151c676056772fedebab694
	CMAKE_ARGS
		-Dglew-cmake_BUILD_SHARED=OFF
		-Dglew-cmake_BUILD_STATIC=ON
	DEFINE GLEW_STATIC
	LINK glew
	LINK ${OPENGL_LIBRARIES}
)

wish_create_external(
	NAME glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 2c7ef5b480d7780455deed43aedc177b9fe3ac61
	CMAKE_ARGS
		-DGLFW_BUILD_EXAMPLES=OFF
		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_DOCS=OFF
	LINK glfw3
	LINK ${OPENGL_LIBRARIES}
#	LINK $<IF:$<PLATFORM_ID:Windows,MinGW>, gdi32,>
#	LINK $<IF:$<PLATFORM_ID:Linux>, dl X11 Xi Xrandr Xxf86vm Xinerama Xcursor rt,>
	LINK $<IF:$<BOOL:${WIN32}>,
			gdi32, # Win 32
			dl X11 Xi Xrandr Xxf86vm Xinerama Xcursor rt # Ubuntu 18.04
		>
)

wish_create_external(
	NAME gli
	GIT_REPOSITORY https://github.com/g-truc/gli.git
	GIT_TAG 0c171ee87fcfe35a7e0e0445adef06f92e0b6a91
	# TODO: Switch to latest gli:
#	GIT_TAG 779b99ac6656e4d30c3b24e96e0136a59649a869
	CMAKE_ARGS
		-DGLI_TEST_ENABLE=OFF
)

wish_create_external(
	NAME glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 761a842a595f58acfc01cc5c7760ff351feaaece
	CMAKE_ARGS
		-DGLM_TEST_ENABLE=OFF
	DEFINE GLM_ENABLE_EXPERIMENTAL
	DEFINE GLM_FORCE_RADIANS
)

wish_create_external(
	NAME openssl
	GIT_REPOSITORY https://github.com/openssl/openssl
	GIT_TAG OpenSSL_1_1_1k

	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND bash -c [=[[[ `uname -s` == MSYS_NT* ]] && source shell mingw64 && ./config no-tests no-shared || ./config no-tests no-shared]=]
			# Bash is used for Windows cmd to not fail by trying to execute the config.com
			# Windows MSYS_NT system would fail inside openssl configure, so switch to msys2's mingw64 shell
			# The && CONFIG and || CONFIG pattern is used because cmake eats the semicolon
	BUILD_COMMAND make -j${WISH_PROCESSOR_COUNT} # OpenSSL is 'make' based
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/openssl/include/openssl/
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/openssl/lib/
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PATH_EXT_SRC}/openssl/src/get_openssl/include/openssl/ ${PATH_EXT}/openssl/include/openssl/
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/openssl/src/get_openssl/libssl.a ${PATH_EXT}/openssl/lib/libssl.a
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/openssl/src/get_openssl/libcrypto.a ${PATH_EXT}/openssl/lib/libcrypto.a

#	LINK ssl
#	LINK wsock32 ws2_32

	LINK crypto
	LINK $<IF:$<BOOL:${WIN32}>, crypt32, >
)

wish_create_external(
	NAME immer
	GIT_REPOSITORY https://github.com/arximboldi/immer.git
	GIT_TAG 559ab37543a7076f9cfe9002720b0d5ad6ffc12a
)

wish_create_external(
	NAME lua
	GIT_REPOSITORY https://github.com/walterschell/Lua.git
	GIT_TAG 14f98e5fdcde3ccd7ea9188181dd7e50660a2999 # v5.4.4
	CMAKE_ARGS
		-DLUA_ENABLE_SHARED=OFF
		-DLUA_BUILD_BINARY=OFF
		-DLUA_BUILD_COMPILER=OFF
		-DLUA_SUPPORT_DL=OFF
	LINK lua_static
)

wish_create_external(
	NAME prometheus
	GIT_REPOSITORY https://github.com/jupp0r/prometheus-cpp.git
	GIT_TAG v0.13.0
	CMAKE_ARGS
		-DENABLE_TESTING=OFF
	LINK prometheus-cpp-pull
	LINK prometheus-cpp-push
	LINK prometheus-cpp-core
	LINK z
	LINK Threads::Threads
	LINK $<IF:$<BOOL:${WIN32}>, wsock32, >
	LINK $<IF:$<BOOL:${WIN32}>, ws2_32, >
)

wish_create_external(
	NAME range
	GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
	GIT_TAG 0487cca29e352e8f16bbd91fda38e76e39a0ed28
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/range/
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PATH_EXT_SRC}/range/src/get_range/include/ ${PATH_EXT}/range/include/
)

wish_create_external(
	NAME rp3d
#	GIT_REPOSITORY https://github.com/DanielChappuis/reactphysics3d
#	GIT_TAG c237cecf6871af44d54fc641af6f0dea15dd3187
#	Main reason for the fork is long namespace if not the monolith header is used
	GIT_REPOSITORY https://github.com/VaderY/reactphysics3d.git
	GIT_TAG patched_c237cecf
	CMAKE_ARGS
		-DCMAKE_BUILD_TYPE=RELEASE
#	LINK reactphysics3d
	LINK rp3d
)

wish_create_external(
	NAME soil2
	GIT_REPOSITORY https://github.com/VaderY/SOIL2.git
	GIT_TAG master
	LINK soil2 ${OPENGL_LIBRARIES}
)

wish_create_external(
	NAME sol2
	GIT_REPOSITORY https://github.com/ThePhD/sol2.git
	GIT_TAG v3.3.0
	CMAKE_ARGS
#		-DSOL2_BUILD_LUA=ON
#		-DSOL2_LUA_VERSION="5.4.4"
		-DSOL2_DOCS=OFF
	LINK ext_lua
)

wish_create_external(
	NAME sml
	GIT_REPOSITORY https://github.com/boost-experimental/sml.git
	GIT_TAG 7b6a54dd84067cfc3a42f869e2df1b65d38bd711
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/sml/
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PATH_EXT_SRC}/sml/src/get_sml/include/ ${PATH_EXT}/sml/include/
)

wish_create_external(
	NAME sqlite3
	GIT_REPOSITORY https://github.com/VaderY/sqlite3-cmake.git
	GIT_TAG a2cb89394d4c18c9055f07eb02663e8625ccf967
	CMAKE_ARGS
		-DBUILD_SHELL=OFF
	LINK sqlite3
)

wish_create_external(
	NAME utf8cpp
	GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
	GIT_TAG e6bde7819c60c453b720b5de8c7c5ee9ffd9805a
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/utf8cpp/include/utf8cpp/
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PATH_EXT_SRC}/utf8cpp/src/get_utf8cpp/source ${PATH_EXT}/utf8cpp/include/utf8cpp
)

wish_create_external(
	NAME jc_voronoi
	GIT_REPOSITORY https://github.com/JCash/voronoi
	GIT_TAG fc9ed1ce6661ccc73783ee9193fa77e8a673b839
	SKIP_CONFIGURE_AND_BUILD
	INSTALL_COMMAND
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PATH_EXT}/jc_voronoi/include/jc_voronoi/
		COMMAND ${CMAKE_COMMAND} -E copy ${PATH_EXT_SRC}/jc_voronoi/src/get_jc_voronoi/src/jc_voronoi.h ${PATH_EXT}/jc_voronoi/include/jc_voronoi/jc_voronoi.h
)

wish_create_external(
	NAME vide
	GIT_REPOSITORY https://github.com/cpplibv/vide.git
	GIT_TAG 0a8204b7d4e2ae83540cb8418efd21157f8202ab
#	GIT_REPOSITORY /e/dev/cpp/forks/vide
#	GIT_TAG dev
	CMAKE_ARGS
		-DJUST_INSTALL_VIDE=ON
)

# === Targets =====================================================================================

wish_group(group_generator generator) # --- Generator ---------------------------------------------

wish_generator(
	TARGET  enum
	COMMAND enum
	OUTPUT  REPLACE ".hpp.in.lua" ".hpp"
)

wish_generator(
	TARGET  codegen
	COMMAND codegen
	OUTPUT  REPLACE ".in.lua" ".hpp"
	OUTPUT  REPLACE ".in.lua" ".cpp"
# TODO P2: Enable header only and split gen
#	OUTPUT  REPLACE ".in.lua" ".hpp"
#	OUTPUT  REPLACE ".inh.lua" ".hpp"
#	OUTPUT  REPLACE ".ins.lua" ".hpp"
#	OUTPUT  REPLACE ".ins.lua" ".cpp"
)

#wish_generator(
#	TARGET  enum_split
#	COMMAND enum --split
#	OUTPUT  REPLACE .hpp.in.lua .hpp
#	OUTPUT  REPLACE .hpp.in.lua .cpp
#	OUTPUT  REGEX REPLACE "\\.hpp\\.in\\.lua$" ".hpp"
#	OUTPUT  REGEX REPLACE "\\.hpp\\.in\\.lua$" ".cpp"
#)

wish_group(group_library lib) # --- Library -------------------------------------------------------

wish_create_library(
	TARGET libv_algo STATIC
	SOURCE src/libv/algo/*.cpp
	LINK   libv_math
	LINK   ext_boost # For small_vector in wildcard.cpp, therefore could be private
)

wish_create_library(
	TARGET libv_arg INTERFACE
)

wish_create_library(
	TARGET libv_color INTERFACE
)

wish_create_library(
	TARGET libv_container INTERFACE
	LINK   libv_math
)

wish_create_library(
	TARGET libv_ctrl STATIC
	SOURCE src/libv/ctrl/*.cpp
	LINK   libv_math libv_input libv_utility ext_fmt ext_boost ext_utf8cpp
)

wish_create_library(
	TARGET libv_diff STATIC
	SOURCE src/libv/diff/*.cpp
	LINK   ext_hdiffpatch libv_hash libv_algo
)

wish_create_library(
	TARGET libv_ecs INTERFACE
	LINK   ext_boost ext_range
)

wish_create_library(
	TARGET libv_img STATIC
	SOURCE src/libv/img/*.cpp
	LINK   libv_math ext_soil2
)

wish_create_library(
	TARGET libv_input STATIC
	SOURCE src/libv/input/*.cpp
	LINK   libv_math ext_fmt ext_boost ext_utf8cpp
)

wish_create_library(
	TARGET libv_frame STATIC
	SOURCE src/libv/frame/*.cpp
	LINK   libv_log libv_math libv_input ext_boost ext_glfw Threads::Threads
)

wish_create_library(
	TARGET libv_fsw STATIC
	SOURCE src/libv/fsw/*.cpp
	LINK   libv_utility ext_efsw
	LINK   libv_mt
)

wish_create_library(
	TARGET libv_fwd INTERFACE
)

wish_create_library(
	TARGET libv_hash STATIC
	SOURCE src/libv/hash/*.cpp
	LINK   ext_boost libv_utility
)

wish_create_library(
	TARGET libv_gl STATIC
	SOURCE src/libv/gl/*.cpp
	LINK   libv_log libv_math libv_utility ext_ctre ext_glew ext_boost ext_gli ext_soil2
)

wish_create_library(
	TARGET libv_glr STATIC
	SOURCE src/libv/glr/*.cpp
	LINK   libv_gl ext_boost ext_range
)

wish_create_library(
	TARGET libv_log INTERFACE
	LINK   libv_mt ext_fmt
)

wish_create_library(
	TARGET libv_lua STATIC
	SOURCE src/libv/lua/*.cpp
	LINK   ext_sol2 ext_lua ext_fmt libv_math
)

wish_create_library(
	TARGET libv_math STATIC
	SOURCE src/libv/math/*.cpp
#	TARGET libv_math INTERFACE
	LINK   ext_glm # Remove (This will only happen with a vec concept / correct mat_t implementation)
	LINK   ext_boost # For gradient small_vector
	LINK   ext_fmt
	LINK   libv_fwd
)

wish_create_library(
	TARGET libv_meta INTERFACE
	LINK   ext_boost
)

wish_create_library(
	TARGET libv_mt STATIC
	SOURCE src/libv/mt/*.cpp
	LINK   libv_log Threads::Threads
	LINK   libv_utility # For unique_function and concat
)

wish_create_library(
	TARGET libv_net STATIC
	SOURCE src/libv/net/*.cpp
	LINK   libv_log libv_utility libv_meta ext_boost
#	LINK   libv_serial
)

wish_create_library(
	TARGET libv_noise STATIC
	SOURCE src/libv/noise/*.cpp
	LINK   libv_utility libv_math
#	LINK   libv_log
)

wish_create_library(
	TARGET libv_parse STATIC
	SOURCE src/libv/parse/*.cpp
	LINK   libv_math ext_boost
)

wish_create_library(
	TARGET libv_range INTERFACE
	LINK   ext_boost ext_range
)

wish_create_library(
	TARGET libv_rev STATIC
	SOURCE src/libv/rev/*.cpp
	LINK   libv_glr libv_gl libv_utility libv_fsw
#	LINK   ext_boost
	LINK   libv_algo libv_range libv_log ext_range ext_ctre # Source include engine stuff
	LINK   libv_vm4
)

wish_create_library(
	TARGET libv_security STATIC
	SOURCE src/libv/security/*.cpp
	LINK   ext_openssl
#	LINK   libv_utility
)

wish_create_library(
	TARGET libv_serial INTERFACE
	LINK   ext_vide
#	LINK   ext_boost # Remove
	LINK   libv_utility # For byte streams that might be removed in the future
)

wish_create_library(
	TARGET libv_sig STATIC
	SOURCE src/libv/sig/*.cpp
)

wish_create_library(
	TARGET libv_state INTERFACE
	LINK   ext_sml # For SML based
	LINK   ext_boost # For boost statechart based
)

wish_create_library(
	TARGET libv_sun STATIC
	SOURCE src/libv/sun/*.cpp
	LINK   libv_math libv_ctrl libv_log
)

wish_create_library(
	TARGET libv_sys STATIC
	SOURCE src/libv/sys/*.cpp
	LINK   libv_log ext_clip ext_fmt
)

wish_create_library(
	TARGET libv_ui STATIC
	SOURCE src/libv/ui/*.cpp
	GENERATE enum src/libv/ui/*.in.lua
	LINK   libv_glr libv_parse libv_lua libv_input ext_freetype ext_utf8cpp libv_sys
	LINK   libv_fsw
)

wish_create_library(
	TARGET libv_utility STATIC
	SOURCE src/libv/utility/*.cpp
	LINK   Threads::Threads ext_function2 ext_boost
)

#wish_create_library(
#	TARGET libv_update STATIC
#	SOURCE src/libv/update/*.cpp
#	LINK   libv_net libv_process libv_diff libv_serial libv_mt
#)

wish_create_library(
	TARGET libv_process STATIC
	SOURCE src/libv/process/*.cpp
	LINK   ext_boost # For interprocess file_lock
)

wish_create_library(
	TARGET libv_vm4 STATIC
	SOURCE src/libv/vm4/*.cpp
	LINK   libv_math libv_serial ext_boost
)

wish_create_library(
	TARGET libv_vm4imp STATIC
	SOURCE src/libv/vm4imp/*.cpp
	LINK   libv_log libv_vm4 ext_assimp
)

wish_group(group_application app) # --- Application -----------------------------------------------

wish_create_executable(
	TARGET codegen
	SOURCE app/codegen/codegen_main.cpp
	LINK   ext_fmt ext_range libv_lua libv_utility libv_algo
)

wish_create_executable(
	TARGET enum
	SOURCE app/enum/*.cpp
	LINK   ext_fmt ext_range libv_lua libv_utility
)

wish_create_executable(
	TARGET mimp
	SOURCE app/mimp/*.cpp
	LINK   libv_vm4imp libv_lua
	LINK   libv_utility libv_algo
#	LINK   ext_fmt ext_range libv_lua libv_utility libv_algo
)

#wish_create_executable(
#	TARGET space
#	SOURCE app/space/*.cpp
#	GENERATE codegen app/space/message/*.in.lua
##	LINK   ext_fmt ext_clip ext_range
#	LINK   libv_log libv_lua libv_frame libv_ctrl libv_rev libv_glr libv_ui libv_net libv_serial libv_color
#	LINK   libv_vm4
#	LINK   libv_sys
#)

wish_create_executable(
	TARGET space
	SOURCE app/space/*.cpp
	GENERATE codegen app/space/message/*.in.lua
#	LINK   ext_fmt ext_clip ext_range
	LINK   libv_log libv_lua libv_frame libv_ctrl libv_rev libv_glr libv_ui libv_net libv_serial libv_color
	LINK   libv_vm4 libv_noise libv_sun
	LINK   libv_sys
)

wish_create_executable(
	TARGET surface
	SOURCE app/surface/*.cpp
	LINK   libv_log libv_lua libv_frame libv_ctrl libv_rev libv_glr libv_ui libv_color libv_arg
	LINK   libv_vm4
	LINK   libv_sys
	LINK   libv_utility libv_math libv_noise
#	LINK   libv_net libv_serial
)

wish_create_executable(
	TARGET star
	SOURCE app/star/*.cpp
#	GENERATE codegen app/star/message/*.in.lua
	LINK   libv_log libv_sys
	LINK   libv_frame libv_ctrl libv_ui
	LINK   libv_sun libv_rev libv_glr libv_color
	LINK   libv_serial
#	LINK   libv_lua
#	LINK   libv_net
#	LINK   libv_vm4
)

wish_create_executable(
	TARGET theme
	#OUTPUT_FOLDER theme
	#OUTPUT_NAME theme
	#OUTPUT_RESOURCE res
	#RESOURCE app/theme/res
	SOURCE app/theme/*.cpp
	LINK   libv_log libv_ui libv_frame libv_glr libv_utility libv_fsw libv_lua libv_mt
#	LINK   libv_ctrl
)

## --- Update ---
#
#wish_create_library(
#	# TODO P5: Make this an object library
#	TARGET update_common STATIC
#	SOURCE app/update/common/*.cpp
#	LINK   libv_update
#
#	LINK   libv_net libv_process
#	LINK   libv_diff # Just for testing
#	LINK   libv_serial # Just for testing
#)
#
#wish_create_executable(
#	TARGET update_server
#	SOURCE app/update/update_server_main.cpp
#	LINK   libv_arg update_common
##	SOURCE app/update/common/*.cpp
##	SOURCE app/update/server_main.cpp
##	LINK   libv_arg libv_net
#)
#
#wish_create_executable(
#	TARGET resource_server
#	SOURCE app/update/resource_server_main.cpp
#	LINK   libv_arg update_common
##	SOURCE app/update/common/*.cpp
##	SOURCE app/update/server_main.cpp
##	LINK   libv_arg libv_net
#)
#
#wish_create_executable(
#	TARGET update_client
#	SOURCE app/update/client_main.cpp
#	LINK   libv_arg update_common
##	SOURCE app/update/common/*.cpp
##	SOURCE app/update/client_main.cpp
##	LINK   libv_arg libv_net
#)
#
#wish_create_executable(
#	TARGET create_update
#	SOURCE app/update/create_update.cpp
#	LINK   libv_log libv_arg libv_diff
#	LINK   libv_serial
#	LINK   libv_process # Just for testing
#	LINK   update_common # Just for testing
#)
#
#wish_create_executable(
#	TARGET apply_update
#	SOURCE app/update/apply_update.cpp
#	LINK   libv_log libv_arg libv_diff
#	LINK   libv_serial
#	LINK   libv_process # Just for testing
#	LINK   update_common # Just for testing
#)
#
#add_custom_target(update DEPENDS update_client update_server apply_update create_update resource_server)

# ---

wish_create_executable(
	TARGET app_binary_to_source
	SOURCE app/binary_to_source.cpp
	LINK   ext_fmt libv_utility
)

# app.vm4_viewer: Disable vm4_viewer build target until the project is not resurrected
#
#wish_create_executable(
#	TARGET app_vm4_viewer
#	#OUTPUT_FOLDER vm4_viewer
#	#OUTPUT_NAME vm4_viewer
#	#OUTPUT_RESOURCE res
#	#RESOURCE app/vm4_viewer/res
#	SOURCE app/vm4_viewer/*.cpp
#	LINK   libv_log libv_vm4imp libv_ui libv_frame libv_utility libv_fsw libv_ctrl
##	LINK   ext_date
#)

wish_create_executable(
	TARGET fsw
	SOURCE app/fsw.cpp
	LINK   ext_fmt libv_utility libv_fsw libv_mt
#	LINK   libv_arg
)

wish_create_executable(
	TARGET lua_runner
	SOURCE app/lua_runner.cpp
	LINK   libv_lua
)

wish_group(group_example example) # --- Example ---------------------------------------------------

wish_create_executable(
	TARGET example_libv_ctrl_camera
	SOURCE example/ctrl/camera/*.cpp
	LINK   libv_ctrl
)

wish_create_executable(
	TARGET example_libv_ctrl_hello
	SOURCE example/ctrl/hello/*.cpp
	LINK   libv_ctrl
)

wish_group(group_sandbox sandbox) # --- Sandbox ---------------------------------------------------

wish_create_executable(
	TARGET sandbox_libv_coro
	SOURCE sandbox/libv_coro_main.cpp
	LINK   ext_cppcoro libv_mt libv_utility
)

wish_create_executable(
	TARGET sandbox_libv_container
	SOURCE sandbox/libv_container_main.cpp
	LINK   ext_fmt libv_container libv_utility libv_math
)

wish_create_executable(
	TARGET sandbox_libv_colony
	SOURCE sandbox/libv_colony_main.cpp
	LINK   ext_colony
)

wish_create_executable(
	TARGET sandbox_libv_ctrl
	SOURCE sandbox/libv_ctrl_main.cpp
	LINK   libv_log libv_ctrl libv_frame libv_ui libv_utility
)

wish_create_executable(
	TARGET sandbox_libv_diff
	SOURCE sandbox/libv_diff_main.cpp
	LINK   libv_diff
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
	TARGET sandbox_libv_fsw
	SOURCE sandbox/libv_fsw_main.cpp
	LINK   libv_fsw libv_mt Threads::Threads
)

wish_create_executable(
	TARGET sandbox_libv_gl
	SOURCE sandbox/libv_gl_main.cpp
	LINK   libv_log libv_gl libv_utility ext_glfw
)

wish_create_executable(
	TARGET sandbox_libv_gl2
	SOURCE sandbox/libv_gl_main2.cpp
	LINK   libv_log libv_gl libv_utility ext_glfw
)

#wish_create_executable(
#	TARGET sandbox_libv_gl3
#	SOURCE sandbox/libv_gl_main3.cpp
#	LINK   libv_log libv_gl libv_utility ext_boost ext_glfw
#)

wish_create_executable(
	TARGET sandbox_libv_glr
	SOURCE sandbox/libv_glr_main.cpp
	LINK   libv_log libv_glr libv_utility libv_math ext_glfw
)

wish_create_executable(
	TARGET sandbox_libv_glr2
	SOURCE sandbox/libv_glr_main2.cpp
	LINK   libv_log libv_glr libv_utility libv_math ext_glfw
)

wish_create_executable(
	TARGET sandbox_libv_img
	SOURCE sandbox/libv_img_main.cpp
	LINK   libv_img libv_utility
)

wish_create_executable(
	TARGET sandbox_libv_immer
	SOURCE sandbox/libv_immer_main.cpp
	LINK   ext_immer
)

wish_create_executable(
	TARGET sandbox_libv_libv
	SOURCE sandbox/libv_main.cpp
	LINK   libv_log libv_utility
	LINK   libv_range
#	LINK   libv_ctrl ext_boost ext_boost_stacktrace ext_glm ext_range
	LINK   libv_diff
	LINK   libv_mt libv_process
)

wish_create_executable(
	TARGET sandbox_libv_log
	SOURCE sandbox/libv_log_main.cpp
	LINK   libv_log
)

wish_create_executable(
	TARGET sandbox_libv_lua
	SOURCE sandbox/libv_lua_main.cpp
	LINK   libv_log libv_lua
)

wish_create_executable(
	TARGET sandbox_libv_monitor
	SOURCE sandbox/libv_monitor_main.cpp
	LINK   ext_prometheus
)

wish_create_executable(
	TARGET sandbox_libv_net
	SOURCE sandbox/libv_net_main.cpp
	LINK   libv_net libv_log
)

wish_create_executable(
	TARGET sandbox_libv_noise
	SOURCE sandbox/libv_noise_main.cpp
	LINK   libv_noise libv_utility libv_log ext_fmt
)

wish_create_executable(
	TARGET sandbox_libv_physics
	SOURCE sandbox/libv_physics_main.cpp
	LINK   ext_rp3d libv_math
)

wish_create_executable(
	TARGET sandbox_libv_reflection
	SOURCE sandbox/libv_reflection_main.cpp
	LINK   libv_serial
)

wish_create_executable(
	TARGET sandbox_libv_reflection2
	SOURCE sandbox/libv_reflection2_main.cpp
	LINK   libv_serial
)

wish_create_executable(
	TARGET sandbox_libv_rev
	SOURCE sandbox/libv_rev_main.cpp
	LINK   libv_log libv_rev ext_glfw #libv_utility libv_math
)

wish_create_executable(
	TARGET sandbox_libv_rev_res
	SOURCE sandbox/libv_rev_res_main.cpp
	LINK   libv_log libv_rev ext_glfw libv_img
)

wish_create_executable(
	TARGET sandbox_libv_security
	SOURCE sandbox/libv_security_main.cpp
	LINK   libv_security libv_utility
)

wish_create_executable(
	TARGET sandbox_libv_serialize
	SOURCE sandbox/libv_serialize_main.cpp
	LINK   libv_serial
)

wish_create_executable(
	TARGET sandbox_libv_serialize_vide
	SOURCE sandbox/libv_serialize_vide_main.cpp
	LINK   ext_boost ext_vide libv_serial
)

wish_create_executable(
	TARGET sandbox_libv_sig
	SOURCE sandbox/libv_sig_main.cpp
	LINK   libv_log
)

wish_create_executable(
	TARGET sandbox_libv_sm
	SOURCE sandbox/libv_sm_main.cpp
	LINK   libv_log libv_state
)

wish_create_executable(
	TARGET sandbox_libv_sml
	SOURCE sandbox/libv_sml_main.cpp
	LINK   libv_log libv_state
)

wish_create_executable(
	TARGET sandbox_libv_sqlite3
	SOURCE sandbox/libv_sqlite3_main.cpp
	LINK   libv_utility ext_fmt ext_sqlite3
)

wish_create_executable(
	TARGET sandbox_libv_sys
	SOURCE sandbox/libv_sys_main.cpp
	LINK   libv_sys
)

#wish_create_executable(
#	TARGET sandbox_libv_update
#	SOURCE sandbox/libv_update_main.cpp
#	LINK   libv_update libv_net libv_log
#)

wish_create_executable(
	TARGET sandbox_libv_ui
	SOURCE sandbox/libv_ui_main.cpp
	LINK   libv_log libv_ui libv_frame libv_utility
)

wish_create_executable(
	TARGET sandbox_libv_ui_lua
	SOURCE sandbox/libv_ui_lua_main.cpp
	LINK   libv_log libv_ui libv_frame libv_utility libv_lua libv_fsw
)

wish_create_executable(
	TARGET sandbox_libv_vm4imp
	SOURCE sandbox/libv_vm4imp_main.cpp
	LINK   libv_log libv_vm4imp libv_utility libv_algo
)

wish_group(group_test test) # --- Test ------------------------------------------------------------

wish_create_executable(
	TARGET test_libv_algo
	SOURCE test/libv/algorithm/*.cpp
	LINK   ext_catch libv_algo ext_range
)

wish_create_executable(
	TARGET test_libv_arg
	SOURCE test/libv/arg/*.cpp
	LINK   ext_catch libv_arg
)

wish_create_executable(
	TARGET test_libv_color
	SOURCE test/libv/color/*.cpp
	LINK   ext_catch libv_color libv_utility libv_math
)

wish_create_executable(
	TARGET test_libv_container
	SOURCE test/libv/container/*.cpp
	LINK   ext_catch libv_container
)

wish_create_executable(
	TARGET test_libv_ctrl
	SOURCE test/libv/ctrl/*.cpp
	LINK   ext_catch libv_ctrl
)

wish_create_executable(
	TARGET test_libv_ecs
	SOURCE test/libv/ecs/*.cpp
	LINK   ext_catch libv_ecs libv_range
)

wish_create_executable(
	TARGET test_libv_frame
	SOURCE test/libv/frame/*.cpp
	LINK   ext_catch libv_frame
)

wish_create_executable(
	TARGET test_libv_fsw
	SOURCE test/libv/fsw/*.cpp
	LINK   ext_catch libv_fsw ext_boost
)

wish_create_executable(
	TARGET test_libv_fwd
	SOURCE test/libv/fwd/*.cpp
	LINK   ext_catch libv_fwd ext_fmt
)

wish_create_executable(
	TARGET test_libv_input
	SOURCE test/libv/input/*.cpp
	LINK   ext_catch libv_input libv_utility ext_glfw
)

wish_create_executable(
	TARGET test_libv_glr
	SOURCE test/libv/glr/*.cpp
	LINK   ext_catch libv_glr libv_math libv_meta
)

wish_create_executable(
	TARGET test_libv_log
	SOURCE test/libv/log/*.cpp
	LINK   ext_catch libv_log
)

wish_create_executable(
	TARGET test_libv_lua
	SOURCE test/libv/lua/*.cpp
	LINK   ext_catch libv_lua libv_parse
)

wish_create_executable(
	TARGET test_libv_math
	SOURCE test/libv/math/*.cpp
	LINK   ext_catch libv_math libv_utility
)

wish_create_executable(
	TARGET test_libv_mt
	SOURCE test/libv/mt/*.cpp
	LINK   ext_catch libv_mt
)

wish_create_executable(
	TARGET test_libv_net
	SOURCE test/libv/net/*.cpp
	LINK   ext_catch libv_net
)

wish_create_executable(
	TARGET test_libv_parse
	SOURCE test/libv/parse/*.cpp
	LINK   ext_catch libv_parse
)

wish_create_executable(
	TARGET test_libv_range
	SOURCE test/libv/range/*.cpp
	LINK   ext_catch ext_fmt libv_range
)

wish_create_executable(
	TARGET test_libv_rev
	SOURCE test/libv/rev/*.cpp
	LINK   ext_catch libv_rev
)

wish_create_executable(
	TARGET test_libv_serial
	SOURCE test/libv/serial/*.cpp
	LINK   ext_catch libv_serial
)

wish_create_executable(
	TARGET test_libv_sig
	SOURCE test/libv/sig/*.cpp
	LINK   ext_catch libv_sig
)

wish_create_executable(
	TARGET test_libv_ui
	SOURCE test/libv/ui/*.cpp
	LINK   ext_catch libv_ui
)

wish_create_executable(
	TARGET test_libv_utility
	SOURCE test/libv/utility/*.cpp
	LINK   ext_catch ext_fmt libv_utility
)

# -------------------------------------------------------------------------------------------------

wish_create_ide_target()
