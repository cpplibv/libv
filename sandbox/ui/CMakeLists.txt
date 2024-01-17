#

wish_create_executable(
		TARGET sandbox_libv_ui
		SOURCE *.cpp
#		#	GENERATE codegen app/sandbox_libv_ui/message/*.in.lua
#		#	LINK   libv::lua
#		#	LINK   libv::vm4
#		LINK libv::log libv::sys
#		LINK libv::utility
#		LINK libv::mt
#		LINK libv::frame libv::ctrl libv::ui
#		LINK libv::sun libv::re libv::color
#		LINK libv::serial
#		LINK libv::noise
#		LINK sandbox_libv_ui::game libv::arg
		LINK   libv::log libv::ui libv::frame libv::utility
)

wish_resource_mapping(
		TARGET sandbox_libv_ui
		MAPPING_FILE wish/resource_mapping.hpp
		MAPPING_FUNCTION wish::resource_mapping

		RESOURCE style/*.lua
		RESOURCE @/res/font/arialn.ttf
		RESOURCE @/res/font/calibril.ttf
		RESOURCE @/res/font/consola.ttf
		RESOURCE @/res/texture/light_border.png
		RESOURCE @/res/texture/stripes_border.png
)

wish_package(
		TARGET sandbox_libv_ui
		PACKAGE_DESTINATION package/sandbox_libv_ui
)
