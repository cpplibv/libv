#

wish_create_library(
		TARGET star::game STATIC
		SOURCE game/*.cpp

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
		SOURCE star_main.cpp version.cpp
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
		#	SOURCE ../star/game/surface/*.cpp
		LINK ext_catch star::game
)
