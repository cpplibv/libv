#

set(STAR_BUILD_VERSION_MAJOR 0 CACHE STRING "BUILD_VERSION_MAJOR")
set(STAR_BUILD_VERSION_MINOR 0 CACHE STRING "BUILD_VERSION_MINOR")
set(STAR_BUILD_VERSION_PATCH 0 CACHE STRING "BUILD_VERSION_PATCH")
set(STAR_BUILD_VERSION_NAME "version-name" CACHE STRING "BUILD_VERSION_NAME")
set(STAR_BUILD_VARIANT "dev" CACHE STRING "BUILD_VARIANT")
set(STAR_BUILD_ID 0 CACHE STRING "BUILD_ID")
set(STAR_BUILD_DATETIME "2000-01-01T00:00:00Z" CACHE STRING "BUILD_DATETIME")
set(STAR_BUILD_UUID 00000000-0000-0000-0000-000000000000 CACHE STRING "BUILD_UUID")

wish_create_library(
		TARGET star::game STATIC
		SOURCE game/*.cpp
		CONFIGURE_SOURCE game/version.cpp.in

		#	GENERATE codegen app/star/message/*.in.lua
		#	LINK   libv::lua
		#	LINK   libv::vm4
		LINK libv::log libv::sys
		LINK libv::utility
		LINK libv::mt
		LINK libv::frame libv::ctrl libv::ui
		LINK libv::sun libv::rev libv::glr libv::color
		LINK libv::serial
		LINK libv::noise
)

wish_create_executable(
		TARGET star
		SOURCE star_main.cpp
		LINK star::game libv::arg
)

wish_resource_mapping(
		TARGET star
		MAPPING_FILE wish/resource_mapping.hpp
		MAPPING_FUNCTION wish::resource_mapping

		RESOURCE res/style.lua
		RESOURCE @/res/font/arialn.ttf
		RESOURCE @/res/font/calibril.ttf
		RESOURCE @/res/font/consola.ttf
		RESOURCE @/res/texture/light_border.png
		RESOURCE @/res/texture/stripes_border.png
)

wish_package(
		TARGET star
		PACKAGE_DESTINATION package/star
)

wish_create_executable(
		TARGET test_star
		SOURCE test/*.cpp
		LINK ext_catch star::game
)
